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
char *mx_strdup(const char *s);"""

_IMPL_BODY = """\
#include <math.h>
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
    long v = strtol(s, &end, 10);
    if (end == s)
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
    char buf[64];
    snprintf(buf, sizeof(buf), "%.9f", v);
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
    memcpy(out, s, n);
    return out;
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
    return header_file(plates, stem, _substitute(_HEADER_BODY, plates), ["<stdbool.h>"])


def runtime_impl(plates: Plates, stem: str) -> str:
    return impl_file(plates, stem, _substitute(_IMPL_BODY, plates), [])
