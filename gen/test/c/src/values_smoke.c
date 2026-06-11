/* Smoke test for the generated value types (the leaf node types): wire ->
   typed -> wire, including the leniency policies the corpus fixups encode
   (unknown enum literal -> first variant, unparseable number -> 0, clamping
   into declared ranges, exclusive-bound epsilon). Mirrors the Go target's
   TestValueSmoke. */

#include "mx_above_below.h"
#include "mx_beam_value.h"
#include "mx_breath_mark_value.h"
#include "mx_color.h"
#include "mx_comma_separated_text.h"
#include "mx_font_size.h"
#include "mx_instrument_sound.h"
#include "mx_midi_16.h"
#include "mx_note_type_value.h"
#include "mx_number_or_normal.h"
#include "mx_positive_divisions.h"
#include "mx_positive_integer_or_empty.h"
#include "mx_string_number.h"
#include "mx_tenths.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void expect(const char *name, const char *got, const char *want) {
    if (strcmp(got, want) != 0) {
        printf("FAIL %s: got \"%s\", want \"%s\"\n", name, got, want);
        failures++;
    }
}

static void expect_owned(const char *name, char *got, const char *want) {
    expect(name, got, want);
    free(got);
}

int main(void) {
    expect("enum round-trip",
           mx_above_below_to_string(mx_above_below_parse("below")), "below");
    expect("enum unknown falls back to first variant",
           mx_above_below_to_string(mx_above_below_parse("sideways")), "above");
    expect("enum digit-led variant",
           mx_note_type_value_to_string(MX_NOTE_TYPE_VALUE_1024TH), "1024th");
    expect("enum space-separated literal",
           mx_beam_value_to_string(mx_beam_value_parse("backward hook")),
           "backward hook");
    expect("enum empty literal",
           mx_breath_mark_value_to_string(mx_breath_mark_value_parse("")), "");

    expect_owned("number trailing zeros drop on reprint",
                 mx_tenths_to_string(mx_tenths_parse("8.50")), "8.5");
    expect_owned("number unparseable becomes zero",
                 mx_tenths_to_string(mx_tenths_parse("abc")), "0");
    expect_owned("number clamps to inclusive min",
                 mx_midi_16_to_string(mx_midi_16_parse("0")), "1");
    expect_owned("number clamps to inclusive max",
                 mx_midi_16_to_string(mx_midi_16_parse("99")), "16");
    expect_owned("number int leniently truncates decimals",
                 mx_midi_16_to_string(mx_midi_16_parse("3.7")), "3");
    expect_owned("number exclusive min clamps to epsilon",
                 mx_positive_divisions_to_string(mx_positive_divisions_parse("0")),
                 "0.000001");
    expect_owned("number implied positive-integer min",
                 mx_string_number_to_string(mx_string_number_parse("")), "1");
    expect_owned("negative decimal formats",
                 mx_tenths_to_string(mx_tenths_parse("-2.50")), "-2.5");
    expect_owned("negative zero canonicalizes",
                 mx_tenths_to_string(mx_tenths_parse("-0.0")), "0");

    MxAboveBelow strict;
    if (mx_above_below_try_parse("nope", &strict)) {
        printf("FAIL try_parse accepted an unknown literal\n");
        failures++;
    }
    MxTenths strict_n;
    if (mx_tenths_try_parse("12abc", &strict_n)) {
        printf("FAIL try_parse accepted a malformed number\n");
        failures++;
    }
    MxPositiveIntegerOrEmpty pz = mx_positive_integer_or_empty_parse("0");
    expect_owned("union primitive member clamps implied min",
                 mx_positive_integer_or_empty_to_string(pz), "1");

    MxColor color = mx_color_parse("#FF0000");
    expect("string passthrough", color, "#FF0000");
    free(color);

    MxFontSize fs = mx_font_size_parse("small");
    if (fs.kind != MX_FONT_SIZE_CSS_FONT_SIZE) {
        printf("FAIL union kind: got %d, want css-font-size\n", (int)fs.kind);
        failures++;
    }
    expect_owned("union picks enum member", mx_font_size_to_string(fs), "small");
    MxFontSize fs2 = mx_font_size_parse("24");
    expect_owned("union picks numeric member", mx_font_size_to_string(fs2), "24");

    MxNumberOrNormal nn = mx_number_or_normal_parse("normal");
    expect_owned("union literal member", mx_number_or_normal_to_string(nn), "normal");

    MxPositiveIntegerOrEmpty pe = mx_positive_integer_or_empty_parse("");
    expect_owned("union empty literal member",
                 mx_positive_integer_or_empty_to_string(pe), "");
    MxPositiveIntegerOrEmpty p5 = mx_positive_integer_or_empty_parse("5");
    expect_owned("union integer member",
                 mx_positive_integer_or_empty_to_string(p5), "5");

    MxInstrumentSound is = mx_instrument_sound_parse("brass.alphorn");
    if (is.kind != MX_INSTRUMENT_SOUND_SOUND_ID) {
        printf("FAIL open enum known id kind\n");
        failures++;
    }
    expect_owned("open enum known id", mx_instrument_sound_to_string(is),
                 "brass.alphorn");
    mx_instrument_sound_free(&is);
    MxInstrumentSound is2 = mx_instrument_sound_parse("synth.custom-thing");
    if (is2.kind != MX_INSTRUMENT_SOUND_STRING) {
        printf("FAIL open enum fallback kind\n");
        failures++;
    }
    expect_owned("open enum open string", mx_instrument_sound_to_string(is2),
                 "synth.custom-thing");
    mx_instrument_sound_free(&is2);

    /* comma-separated-text has bespoke handling (a `types` render-manifest
       row): storage stays the wire string, the items accessor splits the
       list on demand. */
    MxCommaSeparatedText csv =
        mx_comma_separated_text_parse("Times, Helvetica,sans-serif");
    expect("bespoke csv keeps the wire spelling", csv,
           "Times, Helvetica,sans-serif");
    size_t csv_count = 0;
    char **csv_items = mx_comma_separated_text_items(csv, &csv_count);
    if (csv_count != 3 || csv_items[3] != NULL) {
        printf("FAIL bespoke csv item count: got %zu\n", csv_count);
        failures++;
    } else {
        expect("bespoke csv item 0", csv_items[0], "Times");
        expect("bespoke csv item 1 (\", \" separator)", csv_items[1],
               "Helvetica");
        expect("bespoke csv item 2 (\",\" separator)", csv_items[2],
               "sans-serif");
    }
    mx_comma_separated_text_items_free(csv_items);
    free(csv);
    size_t empty_count = 99;
    char **empty_items = mx_comma_separated_text_items(NULL, &empty_count);
    if (empty_count != 0 || empty_items[0] != NULL) {
        printf("FAIL bespoke csv empty: got %zu items\n", empty_count);
        failures++;
    }
    mx_comma_separated_text_items_free(empty_items);

    if (failures == 0) {
        printf("values smoke: all checks passed\n");
        return 0;
    }
    printf("values smoke: %d failure(s)\n", failures);
    return 1;
}
