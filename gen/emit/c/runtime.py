"""The static C runtime support files (mx_runtime.h / mx_runtime.c).

These helpers are the shared substrate the generated types call into; they
carry the lenient-parse policies in one place, mirroring the Go runtime.
Formatting returns malloc'd strings (C has no other safe idiom for it);
parsing never fails -- malformed input degrades deterministically and range
clamping is the typed wrappers' job.
"""

from __future__ import annotations

from gen.emit.c.common import header_file, impl_file
from gen.plates.model import Plates

_HEADER_BODY = """\
/* Shared parse/format helpers for the generated MusicXML types. Lenient
   parses never fail: unparseable input becomes 0 (range clamping is the
   typed wrappers' job). Formatting returns malloc'd strings the caller
   frees. */

bool mx_try_parse_decimal(const char *s, double *out);
double mx_parse_decimal(const char *s);
bool mx_try_parse_int(const char *s, long *out);
long mx_parse_int(const char *s);

/* Shortest decimal spelling without exponent notation (8.5 -> "8.5",
   4 -> "4", 1e-6 -> "0.000001"). Malloc'd; caller frees. */
char *mx_format_decimal(double v);
char *mx_format_int(long v);

/* strdup that maps NULL to "". Malloc'd; caller frees. */
char *mx_strdup(const char *s);

/* The parse-failure message channel: parse functions returning NULL set it;
   the caller reads it before the next parse. Static storage. */
void mx_error_set(const char *fmt, ...);
const char *mx_error(void);"""

_IMPL_BODY = """\
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool mx_try_parse_decimal(const char *s, double *out) {
    *out = 0;
    if (!s || !*s)
        return false;
    char *end = NULL;
    double v = strtod(s, &end);
    if (end == s)
        return false;
    while (*end == ' ' || *end == '\\t' || *end == '\\n' || *end == '\\r')
        end++;
    if (*end != '\\0')
        return false;
    if (isnan(v) || isinf(v))
        return false;
    *out = v;
    return true;
}

double mx_parse_decimal(const char *s) {
    double v = 0;
    mx_try_parse_decimal(s, &v);
    return v;
}

bool mx_try_parse_int(const char *s, long *out) {
    *out = 0;
    if (!s || !*s)
        return false;
    char *end = NULL;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (errno == ERANGE || end == s)
        return false;
    while (*end == ' ' || *end == '\\t' || *end == '\\n' || *end == '\\r')
        end++;
    if (*end != '\\0')
        return false;
    *out = v;
    return true;
}

long mx_parse_int(const char *s) {
    long v = 0;
    if (mx_try_parse_int(s, &v))
        return v;
    double d = 0;
    if (mx_try_parse_decimal(s, &d))
        return (long)d; /* decimal-looking input truncates toward zero */
    return 0;
}

char *mx_format_decimal(double v) {
    /* Worst-case %.9f for a double: 309 integer digits + sign + dot + 9. */
    char buf[336];
    int n = snprintf(buf, sizeof(buf), "%.9f", v);
    if (n < 0 || n >= (int)sizeof(buf))
        return mx_strdup("0"); /* unreachable for finite doubles; never emit garbage */
    /* Trim trailing zeros, then a trailing dot; canonicalize "-0" to "0". */
    size_t len = strlen(buf);
    while (len > 0 && buf[len - 1] == '0')
        buf[--len] = '\\0';
    if (len > 0 && buf[len - 1] == '.')
        buf[--len] = '\\0';
    if (strcmp(buf, "-0") == 0) {
        buf[0] = '0';
        buf[1] = '\\0';
    }
    return mx_strdup(buf);
}

char *mx_format_int(long v) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%ld", v);
    return mx_strdup(buf);
}

char *mx_strdup(const char *s) {
    if (!s)
        s = "";
    size_t n = strlen(s) + 1;
    char *out = malloc(n);
    if (!out)
        abort(); /* the generator's runtime has no error channel for OOM */
    memcpy(out, s, n);
    return out;
}

static char mx_error_buf[512];

void mx_error_set(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(mx_error_buf, sizeof(mx_error_buf), fmt, args);
    va_end(args);
}

const char *mx_error(void) {
    return mx_error_buf;
}"""


def _substitute(body: str, plates: Plates) -> list[str]:
    # The canonical text spells helpers mx_*; the configured symbol prefix is
    # substituted so generated type code (which composes names from the same
    # prefix) and the runtime always agree.
    from gen.emit.c.common import fn_prefix

    return body.replace("mx_", fn_prefix(plates)).split("\n")


def runtime_stem(plates: Plates) -> str:
    return plates.target.file_prefix + "runtime"


def runtime_header(plates: Plates, stem: str) -> str:
    prefix = plates.target.prefix.upper() + "_" if plates.target.prefix else ""
    version = [
        "/* The MusicXML version of the schema this model was generated from.",
        "   Documents declaring a newer version may use types this model cannot",
        "   represent; harnesses gate on it. */",
        f'#define {prefix}SUPPORTED_MUSICXML_VERSION "{plates.schema_version}"',
        "",
    ]
    return header_file(
        plates, stem, version + _substitute(_HEADER_BODY, plates), ["<stdbool.h>"]
    )


def runtime_impl(plates: Plates, stem: str) -> str:
    return impl_file(plates, stem, _substitute(_IMPL_BODY, plates), [])
