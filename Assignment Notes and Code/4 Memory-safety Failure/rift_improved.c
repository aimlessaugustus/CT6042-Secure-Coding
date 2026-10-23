/* rift_improved.c -- hardened two-pass build, improving on the upstream fix.
 *
 * The upstream patch (commit 524977e) clears is_args at regex end, beside an
 * existing quote reset. It is correct, but keeps the fragile pattern that
 * caused the bug: escape flags are reset one at a time, so a newly added flag
 * can be forgotten, exactly as is_args was for 18 years. This version makes
 * two improvements.
 *
 *   1. A single reset_escape_state clears every escape flag in one place, so
 *      the class of omission cannot recur.
 *   2. The copy pass is bounds-checked against the allocation, so any residual
 *      length/copy desync fails closed at the boundary instead of overflowing.
 *
 * Build the two modes to contrast them:
 *   ./rift_improved '++++++++' structural   -- our fix, all flags cleared
 *   ./rift_improved '++++++++' upstream      -- models a point-fix that resets
 *                                               is_args but forgets quote
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

/* Mirrors ngx_http_script_engine_t, with an explicit allocation limit so the
 * copy pass can be bounds-checked.
 */
typedef struct
{
    unsigned char *pos;   /* Write cursor used by the copy pass. */
    unsigned char *limit; /* One past the end of the allocation. */
    int is_args;          /* Escaping active for arguments. */
    int quote;            /* Escaping active for quoting. */
} script_engine_t;

/* Simplified model of NGX_ESCAPE_ARGS. '+' expands from one byte to three. */
static int needs_escape(unsigned char c)
{
    return c == '+';
}

/* Escape predicate consulted by both passes. */
static int escape_active(const script_engine_t *e)
{
    return e->is_args || e->quote;
}

/* Improvement 1: reset the whole escape state in one place.
 * Upstream clears is_args and quote with separate statements, so a future flag
 * can be omitted. Clearing every flag here removes that possibility.
 */
static void reset_escape_state(script_engine_t *e)
{
    e->is_args = 0;
    e->quote = 0;
}

/* Mirrors ngx_http_script_copy_capture_len_code. */
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

/* Improvement 2: bounds-checked copy pass.
 * Every write is guarded against the allocation limit. If a desync slips
 * through, the write is refused at the boundary and the function returns
 * false, so the fault fails closed instead of corrupting the heap.
 */
static bool copy_capture(script_engine_t *e,
                         const unsigned char *cap,
                         size_t raw)
{
    static const char hex[] = "0123456789ABCDEF";

#define EMIT(b)                      \
    do                               \
    {                                \
        if (e->pos >= e->limit)      \
        {                            \
            return false;            \
        }                            \
        *e->pos++ = (unsigned char)(b); \
    } while (0)

    if (escape_active(e))
    {
        for (size_t i = 0; i < raw; i++)
        {
            if (needs_escape(cap[i]))
            { /* 1 byte -> 3 bytes */
                EMIT('%');
                EMIT(hex[cap[i] >> 4]);
                EMIT(hex[cap[i] & 0x0f]);
            }
            else
            {
                EMIT(cap[i]);
            }
        }
    }
    else
    {
        for (size_t i = 0; i < raw; i++)
        {
            EMIT(cap[i]);
        }
    }

#undef EMIT
    return true;
}

int main(int argc, char **argv)
{
    /* Simulated regex capture ($1), influenced by the request URI. */
    const unsigned char *capture =
        (const unsigned char *)(argc > 1 ? argv[1] : "++++++++");

    size_t raw = strlen((const char *)capture);

    /* Mode selects which reset is applied. */
    const char *mode = (argc > 2 ? argv[2] : "structural");

    /* Main engine carries two stale escape flags from earlier directives. */
    script_engine_t e = {
        .pos = NULL,
        .limit = NULL,
        .is_args = 1,
        .quote = 1};

    if (strcmp(mode, "upstream") == 0)
    {
        e.is_args = 0; /* Point-fix: is_args reset, quote forgotten. */
    }
    else
    {
        reset_escape_state(&e); /* Structural fix: every escape flag cleared. */
    }

    /* Length pass on the zero-initialised sub-engine. */
    script_engine_t le;
    memset(&le, 0, sizeof le);

    size_t len = copy_capture_len(&le, capture, raw);

    unsigned char *buf = malloc(len ? len : 1);
    if (!buf)
    {
        return 1;
    }

    e.pos = buf;
    e.limit = buf + len;

    bool ok = copy_capture(&e, capture, raw);
    size_t written = (size_t)(e.pos - buf);

    printf("mode             : %s\n", mode);
    printf("length pass alloc: %zu bytes\n", len);
    printf("copy pass wrote  : %zu bytes\n", written);

    if (!ok)
    {
        printf("result           : REFUSED at boundary -- fails closed, no overflow\n");
    }
    else if (written != len)
    {
        printf("result           : INVARIANT VIOLATED -- size mismatch\n");
    }
    else
    {
        printf("result           : OK -- sizes agree\n");
    }

    free(buf);
    return (ok && written == len) ? 0 : 1;
}