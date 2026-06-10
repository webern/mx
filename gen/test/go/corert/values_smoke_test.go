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
	}
	for _, c := range cases {
		if c.got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, c.got, c.want)
		}
	}

	if v, ok := mx.TryParseAboveBelow("nope"); ok {
		t.Errorf("TryParse accepted an unknown literal: %v", v)
	}
	if fs := mx.ParseFontSize("small"); fs.Kind != mx.FontSizeKindCSSFontSize {
		t.Errorf("union kind: got %v, want CSSFontSize", fs.Kind)
	}
}
