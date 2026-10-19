/* Lightweight random fuzzer for the two-pass build.
 *
 * Generates random printable inputs and drives the vulnerable functions
 * in-process under AddressSanitizer, which aborts on the first out-of-bounds
 * write. The last candidate printed before the abort is the crashing input.
 * This is undirected rather than coverage-guided, and stands in for libFuzzer
 * where its runtime is unavailable.
 *
 * Build:
 *     gcc -O0 -g -fsanitize=address fuzz_random.c -o fuzz_random
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Reuse the vulnerable functions without the file's own main. */
#define main rift_main
#include "rift_vuln.c"
#undef main

/* One length pass plus one copy pass against a candidate capture. */
static void run_once(const unsigned char *cap, size_t raw)
{
    script_engine_t e = {.pos = NULL, .is_args = 1};
    script_engine_t le;
    memset(&le, 0, sizeof le);

    size_t len = copy_capture_len(&le, cap, raw);

    unsigned char *buf = malloc(len ? len : 1);
    if (!buf)
    {
        return;
    }

    e.pos = buf;
    copy_capture(&e, cap, raw); /* ASan aborts here once an input overflows. */
    free(buf);
}

int main(void)
{
    unsigned char cap[17];

    srand(1); /* Deterministic, so the figure reproduces. */

    for (int iter = 1;; iter++)
    {
        size_t raw = 1 + rand() % 16;

        for (size_t i = 0; i < raw; i++)
        {
            cap[i] = (unsigned char)(33 + rand() % 94); /* Printable ASCII. */
        }
        cap[raw] = '\0';

        printf("iter %3d  trying: %s\n", iter, cap);
        fflush(stdout);

        run_once(cap, raw);
    }

    return 0;
}
