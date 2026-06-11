package corert_test

import (
	"testing"

	"github.com/webern/mx/gen/test/go/mx"
)

// TestValueSmoke exercises the generated value types (the leaf node types)
// end to end: wire -> typed -> wire, including the leniency policies the
// corpus fixups encode (unknown enum literal -> first variant, unparseable
// number -> 0, clamping into declared ranges, exclusive-bound epsilon).
func TestValueSmoke(t *testing.T) {
	cases := []struct {
		name string
		got  string
		want string
	}{
		{"enum round-trip", mx.ParseAboveBelow("below").String(), "below"},
		{"enum unknown falls back to first variant", mx.ParseAboveBelow("sideways").String(), "above"},
		{"enum digit-led variant", mx.NoteTypeValue1024th.String(), "1024th"},
		{"enum space-separated literal", mx.ParseBeamValue("backward hook").String(), "backward hook"},
		{"enum empty literal", mx.ParseBreathMarkValue("").String(), ""},
		{"number trailing zeros drop on reprint", mx.ParseTenths("8.50").String(), "8.5"},
		{"number unparseable becomes zero", mx.ParseTenths("abc").String(), "0"},
		{"number clamps to inclusive min", mx.ParseMIDI16("0").String(), "1"},
		{"number clamps to inclusive max", mx.ParseMIDI16("99").String(), "16"},
		{"number int leniently truncates decimals", mx.ParseMIDI16("3.7").String(), "3"},
		{"number exclusive min clamps to epsilon", mx.ParsePositiveDivisions("0").String(), "0.000001"},
		{"number implied positive-integer min", mx.ParseStringNumber("").String(), "1"},
		{"string passthrough", mx.ParseColor("#FF0000").String(), "#FF0000"},
		{"union picks enum member", mx.ParseFontSize("small").String(), "small"},
		{"union picks numeric member", mx.ParseFontSize("24").String(), "24"},
		{"union literal member", mx.ParseNumberOrNormal("normal").String(), "normal"},
		{"union empty literal member", mx.ParsePositiveIntegerOrEmpty("").String(), ""},
		{"union integer member", mx.ParsePositiveIntegerOrEmpty("5").String(), "5"},
		{"union primitive member clamps implied min", mx.ParsePositiveIntegerOrEmpty("0").String(), "1"},
		{"negative decimal formats", mx.ParseTenths("-2.50").String(), "-2.5"},
		{"negative zero canonicalizes", mx.ParseTenths("-0.0").String(), "0"},
	}
	for _, c := range cases {
		if c.got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, c.got, c.want)
		}
	}

	if v, ok := mx.TryParseAboveBelow("nope"); ok {
		t.Errorf("TryParse accepted an unknown literal: %v", v)
	}
	if fs := mx.ParseFontSize("small"); fs.Kind != mx.FontSizeCSSFontSize {
		t.Errorf("union kind: got %v, want CSSFontSize", fs.Kind)
	}

	// The schema's string facets are enforced by the strict parse: patterns
	// (anchored, ARGB color, XSD \c name-class expansion for SMuFL names)
	// and minimum lengths. The lenient parse keeps any value verbatim.
	patternCases := []struct {
		name string
		ok   bool
	}{
		{"color RGB", true},
		{"color ARGB", true},
		{"color bad hex", false},
		{"color unanchored tail", false},
		{"smufl accidental", true},
		{"smufl wrong prefix", false},
		{"comma-separated text", true},
		{"comma-separated empty item", false},
		{"measure-text min length", false},
	}
	patternResults := []bool{
		second(mx.TryParseColor("#FF0000")),
		second(mx.TryParseColor("#40800080")),
		second(mx.TryParseColor("#GG0000")),
		second(mx.TryParseColor("#FF0000 ")),
		second(mx.TryParseSMUFLAccidentalGlyphName("accSagittal5v7KleismaUp")),
		second(mx.TryParseSMUFLAccidentalGlyphName("noteheadBlack")),
		second(mx.TryParseCommaSeparatedText("Times, Helvetica")),
		second(mx.TryParseCommaSeparatedText("Times,,Helvetica")),
		second(mx.TryParseMeasureText("")),
	}
	for i, c := range patternCases {
		if patternResults[i] != c.ok {
			t.Errorf("%s: TryParse ok = %v, want %v", c.name, patternResults[i], c.ok)
		}
	}
	if got := mx.ParseColor("#GG0000").String(); got != "#GG0000" {
		t.Errorf("lenient parse must keep the value verbatim, got %q", got)
	}

	// yyyy-mm-dd has bespoke handling (a `types` render-manifest row):
	// storage stays the wire string, components parse as ints on demand.
	date := mx.ParseYyyyMmDd("2011-08-08")
	if date.Yyyy() != 2011 || date.Mm() != 8 || date.Dd() != 8 {
		t.Errorf("date components: got %d-%d-%d, want 2011-8-8",
			date.Yyyy(), date.Mm(), date.Dd())
	}
	if got := date.String(); got != "2011-08-08" {
		t.Errorf("bespoke date must keep the wire spelling, got %q", got)
	}
	if bce := mx.ParseYyyyMmDd("-0500-01-02"); bce.Yyyy() != -500 || bce.Mm() != 1 || bce.Dd() != 2 {
		t.Errorf("BCE date components: got %d, %d, %d", bce.Yyyy(), bce.Mm(), bce.Dd())
	}
	if junk := mx.ParseYyyyMmDd("not a date"); junk.Yyyy() != 0 || junk.Mm() != 0 || junk.Dd() != 0 {
		t.Errorf("unparseable date components must be 0")
	}
}

func second[T any](_ T, ok bool) bool { return ok }
