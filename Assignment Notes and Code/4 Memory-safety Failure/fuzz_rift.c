/* libFuzzer harness for the two-pass build.
 *
 * Build on a host with the clang fuzzer runtime:
 *     clang -fsanitize=fuzzer,address fuzz_rift.c -o fuzz_rift
 *
 * Each fuzzer input is treated as the regex capture. AddressSanitizer reports
 * the heap overflow as soon as libFuzzer supplies bytes that expand under
 * escaping, which it discovers without any seed corpus.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Pull in the vulnerable functions. The file's own main is renamed so that
 * libFuzzer can provide its own entry point.
 */
#define main rift_main
#include "rift_vuln.c"
#undef main

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    script_engine_t e = {.pos = NULL, .is_args = 1};
    script_engine_t le;
    memset(&le, 0, sizeof le);

    /* Length pass sizes the buffer, copy pass fills it. */
    size_t len = copy_capture_len(&le, data, size);

    unsigned char *buf = malloc(len ? len : 1);
    if (!buf)
    {
        return 0;
    }

    e.pos = buf;
    copy_capture(&e, data, size);
    free(buf);

    return 0;
}
