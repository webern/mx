"""The static Go runtime support file.

These helpers are the shared substrate the generated types call into; they
carry the lenient-parse policies (see gen.emit.go.values) in one place. The
file is emitted through the same writer as the generated types so it carries
the marker and participates in pruning.
"""

from __future__ import annotations

from gen.emit.go.common import file_frame
from gen.plates.model import Plates

_BODY = '''\
// tryParseDecimal parses s strictly as a decimal number.
func tryParseDecimal(s string) (float64, bool) {
	v, err := strconv.ParseFloat(strings.TrimSpace(s), 64)
	if err != nil || math.IsNaN(v) || math.IsInf(v, 0) {
		return 0, false
	}
	return v, true
}

// parseDecimal is the lenient form: unparseable input becomes 0, so a
// malformed document still loads deterministically. Range clamping is the
// typed wrappers' job.
func parseDecimal(s string) float64 {
	v, _ := tryParseDecimal(s)
	return v
}

// tryParseInt parses s strictly as an integer.
func tryParseInt(s string) (int, bool) {
	v, err := strconv.ParseInt(strings.TrimSpace(s), 10, 64)
	if err != nil {
		return 0, false
	}
	return int(v), true
}

// parseInt is the lenient form: a decimal-looking value truncates toward
// zero, anything else becomes 0.
func parseInt(s string) int {
	if v, ok := tryParseInt(s); ok {
		return v
	}
	if v, ok := tryParseDecimal(s); ok {
		return int(v)
	}
	return 0
}

// formatDecimal prints the shortest decimal that round-trips the value,
// without exponent notation (8.5 -> "8.5", 4 -> "4"). Negative zero
// canonicalizes to "0" (matching the C runtime and the corert normalizer).
func formatDecimal(v float64) string {
	if v == 0 {
		return "0"
	}
	return strconv.FormatFloat(v, 'f', -1, 64)
}

func formatInt(v int) string {
	return strconv.Itoa(v)
}'''


def runtime_file(plates: Plates) -> str:
    return file_frame(plates, _BODY.split("\n"), imports=["math", "strconv", "strings"])
