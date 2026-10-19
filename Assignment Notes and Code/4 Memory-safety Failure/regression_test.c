/* regression_test.c -- CI guard against reintroduction of CVE-2026-42945.
 *
 * Runs the target build under AddressSanitizer on the overflow trigger and
 * passes only if it completes without a memory error. Point it at the fixed
 * binary in CI. Run against the vulnerable binary once to confirm the guard
 * actually catches the bug.
 *
 * The target must be built with -fsanitize=address. A missing or non-runnable
 * target is reported as an ERROR (exit 2), distinct from a FAIL, so a build
 * mistake can never look like a caught vulnerability.
 *
 * Build: gcc -O0 -g regression_test.c -o regression_test
 * Usage: ./regression_test ./rift_fixed_asan
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <asan-instrumented binary>\n", argv[0]);
        return 2;
    }

    /* Distinguish a missing build from a caught overflow. */
    if (access(argv[1], X_OK) != 0)
    {
        fprintf(stderr,
                "ERROR: cannot run %s -- build it first with -fsanitize=address\n",
                argv[1]);
        return 2;
    }

    char cmd[512];
    snprintf(cmd, sizeof cmd,
             "ASAN_OPTIONS=detect_leaks=0 %s '++++++++' >/dev/null 2>&1",
             argv[1]);

    int rc = system(cmd);
    if (rc == -1)
    {
        fprintf(stderr, "ERROR: failed to launch %s\n", argv[1]);
        return 2;
    }

    int ok = (rc == 0); /* Non-zero means ASan aborted on the overflow. */

    printf("%s : %s\n", ok ? "PASS" : "FAIL", argv[1]);
    return ok ? 0 : 1;
}