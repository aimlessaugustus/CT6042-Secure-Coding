/* rift_fixed_regressed.c -- rift_fixed.c with a simulated future regression.
 *
 * This is the upstream point-fix with one extra line that models a later
 * change reintroducing a stale escape flag, the same class of mistake that
 * left is_args unreset for 18 years. Because rift_fixed's copy pass is
 * unbounded, the desync overflows again. It exists only to demonstrate that
 * the point-fix has no defence beyond the flag being correct.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Mirrors ngx_http_script_engine_t: mutable state carried between script
 * operations.
 */
typedef struct
{
    unsigned char *pos; /* Write cursor used by the copy pass. */
    int is_args;        /* Indicates whether argument escaping is active. */
} script_engine_t;

/* Simplified model of NGX_ESCAPE_ARGS. '+' is sufficient to reproduce the
 * defect because it expands from one byte to three (%2B).
 */
static int needs_escape(unsigned char c)
{
    return c == '+';
}

/* Escape predicate consulted by both passes. */
static int escape_active(const script_engine_t *e)
{
    return e->is_args;
}

/* Mirrors ngx_http_script_regex_end_code.
 * The upstream fix adds the is_args reset here, so a stale argument flag
 * cannot leak into a following set or if capture.
 */
static void script_regex_end(script_engine_t *e)
{
    e->is_args = 0; /* Upstream fix, nginx commit 524977e. */
}

/* Mirrors ngx_http_script_copy_capture_len_code.
 * The length pass receives a zero-initialised sub-engine
 * (is_args == 0), so no escaping cost is included.
 */
static size_t copy_capture_len(const script_engine_t *le,
                               const unsigned char *cap,
                               size_t raw)
{
    if (escape_active(le))
    {
        size_t extra = 0;

        for (size_t i = 0; i < raw; i++)
        {
            extra += needs_escape(cap[i]) ? 2 : 0;
        }

        return raw + extra;
    }

    return raw; /* Raw length only. */
}

/* Mirrors ngx_http_script_copy_capture_code.
 * The copy pass is unbounded: it writes whatever the escaping produces,
 * trusting the length pass to have sized the buffer correctly.
 */
static void copy_capture(script_engine_t *e,
                         const unsigned char *cap,
                         size_t raw)
{
    static const char hex[] = "0123456789ABCDEF";

    if (escape_active(e))
    {
        for (size_t i = 0; i < raw; i++)
        {
            if (needs_escape(cap[i]))
            { /* 1 byte -> 3 bytes */
                *e->pos++ = '%';
                *e->pos++ = hex[cap[i] >> 4];
                *e->pos++ = hex[cap[i] & 0x0f];
            }
            else
            {
                *e->pos++ = cap[i];
            }
        }
    }
    else
    {
        memcpy(e->pos, cap, raw);
        e->pos += raw;
    }
}

int main(int argc, char **argv)
{
    /* Simulated regex capture ($1), influenced by the request URI. */
    const unsigned char *capture =
        (const unsigned char *)(argc > 1 ? argv[1] : "++++++++");

    size_t raw = strlen((const char *)capture);

    /* Main engine state after processing a rewrite replacement
     * containing '?'.
     */
    script_engine_t e = {
        .pos = NULL,
        .is_args = 1};

    /* End of rewrite handling. The fix clears the stale flag here. */
    script_regex_end(&e);

    /* Simulated future regression: a new directive re-sets the flag after
     * the reset, reintroducing the desync the point-fix had closed.
     */
    e.is_args = 1;

    /* Length pass sub-engine, equivalent to nginx's memset(..., 0). */
    script_engine_t le;
    memset(&le, 0, sizeof le);

    size_t len = copy_capture_len(&le, capture, raw);

    /* Allocate the size reported by the length pass. */
    unsigned char *buf = malloc(len);
    if (!buf)
    {
        return 1;
    }

    /* Copy pass writes using the main engine state. */
    e.pos = buf;
    copy_capture(&e, capture, raw);

    printf("raw capture      : %zu bytes\n", raw);
    printf("length pass alloc: %zu bytes\n", len);
    printf("copy pass wrote  : %td bytes\n",
           (ptrdiff_t)(e.pos - buf));
    printf("overflow         : %td bytes past the buffer\n",
           (ptrdiff_t)(e.pos - buf) - (ptrdiff_t)len);

    free(buf);
    return 0;
}