// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Construction-safety tests for representative generated value types:
// clamping on construct and set, named-factory enums, structural bespoke
// shapes, and union dispatch. The compile-time must-NOT-compile probes live
// in mxtest/probe (script-driven).

#include "cpul/cpulTestHarness.h"

#include "mx/core/Decimal.h"
#include "mx/core/NameToken.h"
#include "mx/core/Token.h"
#include "mx/core/generated/AboveBelow.h"
#include "mx/core/generated/Color.h"
#include "mx/core/generated/CommaSeparatedText.h"
#include "mx/core/generated/EndingNumber.h"
#include "mx/core/generated/FontSize.h"
#include "mx/core/generated/InstrumentSound.h"
#include "mx/core/generated/MIDI16.h"
#include "mx/core/generated/Mode.h"
#include "mx/core/generated/NoteTypeValue.h"
#include "mx/core/generated/NumberOrNormal.h"
#include "mx/core/generated/Octave.h"
#include "mx/core/generated/PositiveDivisions.h"
#include "mx/core/generated/PositiveIntegerOrEmpty.h"
#include "mx/core/generated/RotationDegrees.h"
#include "mx/core/generated/SmuflAccidentalGlyphName.h"
#include "mx/core/generated/SmuflCodaGlyphName.h"
#include "mx/core/generated/SmuflWavyLineGlyphName.h"
#include "mx/core/generated/TimeOnly.h"
#include "mx/core/generated/YyyyMmDd.h"

#include <string>
#include <vector>

using namespace mx::core;

TEST(EnumFactoriesAndWire, Values)
{
    CHECK_EQUAL(std::string_view{"above"}, AboveBelow::above().toString());
    CHECK_EQUAL(std::string_view{"below"}, AboveBelow::below().toString());
    CHECK(AboveBelow::above() == AboveBelow{});
    CHECK(AboveBelow::above().tag() == AboveBelow::Tag::above);
}

TEST(EnumStrictAndLenientParse, Values)
{
    AboveBelow v;
    CHECK(AboveBelow::tryParse("below", v));
    CHECK(v == AboveBelow::below());
    CHECK(!AboveBelow::tryParse("sideways", v));
    // Lenient falls back to the first variant.
    CHECK(AboveBelow::parse("sideways") == AboveBelow::above());
    CHECK_EQUAL(std::string_view{"1024th"}, NoteTypeValue::_1024th().toString());
}

TEST(NumberClampOnConstructAndSet, Values)
{
    CHECK_EQUAL(1, MIDI16{0}.value());
    CHECK_EQUAL(16, MIDI16{99}.value());
    CHECK_EQUAL(7, MIDI16{7}.value());
    // The natural zero is the clamped default.
    CHECK_EQUAL(1, MIDI16{}.value());
    MIDI16 c{5};
    c.setValue(-3);
    CHECK_EQUAL(1, c.value());
}

TEST(NumberLexicalThenClampParse, Values)
{
    // The established corpus policy: lexically-strict parse, then clamp.
    CHECK_EQUAL(1, MIDI16::parse("0").value());
    CHECK_EQUAL(1, MIDI16::parse("garbage").value()); // 0, clamped
    MIDI16 v;
    CHECK(MIDI16::tryParse("12", v));
    CHECK_EQUAL(12, v.value());
    CHECK(!MIDI16::tryParse("12abc", v));
    CHECK_EQUAL(0, Octave::parse("-4").value());
    CHECK_EQUAL(9, Octave::parse("99").value());
}

TEST(DecimalClampAndTextFace, Values)
{
    // Exclusive bound: clamps just past it.
    CHECK(PositiveDivisions{Decimal{0.0}}.value().value() > 0.0);
    CHECK(PositiveDivisions::parse("-3").value().value() > 0.0);
    // An in-range parse keeps its exact spelling.
    CHECK_EQUAL(std::string{"1.50"}, PositiveDivisions::parse("1.50").toString());
    CHECK_EQUAL(std::string{"-180"}, RotationDegrees::parse("-700").toString());
}

TEST(ColorStructural, Values)
{
    Color c;
    CHECK_EQUAL(std::string{"#000000"}, c.toString());
    CHECK_EQUAL(std::string{"#FF0080"}, Color(0xFF, 0x00, 0x80).toString());
    CHECK_EQUAL(std::string{"#40800080"}, Color(0x40, 0x80, 0x00, 0x80).toString());
    Color parsed;
    CHECK(Color::tryParse("#FFEE00", parsed));
    CHECK_EQUAL(0xEE, parsed.green());
    CHECK(!parsed.alpha());
    CHECK(!Color::tryParse("#ff0000", parsed)); // lowercase is pattern-invalid
    CHECK_EQUAL(std::string{"#000000"}, Color::parse("garbage").toString());
}

TEST(DateStructural, Values)
{
    CHECK_EQUAL(std::string{"1900-01-01"}, YyyyMmDd{}.toString());
    CHECK_EQUAL(std::string{"2026-06-11"}, YyyyMmDd(2026, 6, 11).toString());
    // Clamp to real calendar ranges, leap years honored.
    CHECK_EQUAL(std::string{"2023-02-28"}, YyyyMmDd(2023, 2, 31).toString());
    CHECK_EQUAL(std::string{"2024-02-29"}, YyyyMmDd(2024, 2, 31).toString());
    YyyyMmDd d;
    CHECK(YyyyMmDd::tryParse("2002-01-25", d));
    CHECK_EQUAL(2002, d.year());
    CHECK(!YyyyMmDd::tryParse("2002-13-25", d));
    CHECK(!YyyyMmDd::tryParse("not-a-date", d));
}

TEST(CommaSeparatedTextStructural, Values)
{
    CommaSeparatedText t{std::vector<std::string>{"Times", "serif"}};
    CHECK_EQUAL(std::string{"Times, serif"}, t.toString());
    CHECK_EQUAL(std::size_t{2}, t.items().size());
    // Items repair: embedded commas become spaces, blanks drop.
    CommaSeparatedText repaired{std::vector<std::string>{"a,b", ""}};
    CHECK_EQUAL(std::string{"a b"}, repaired.toString());
    // Wire fidelity: a parsed value keeps its exact spelling.
    CHECK_EQUAL(std::string{"Times,serif"}, CommaSeparatedText::parse("Times,serif").toString());
}

TEST(TimeOnlyAndEndingNumberStructural, Values)
{
    CHECK_EQUAL(std::string{"1"}, TimeOnly{}.toString());
    TimeOnly t{std::vector<int>{2, 0, 3}};
    CHECK_EQUAL(std::string{"2, 1, 3"}, t.toString()); // 0 clamps to 1
    TimeOnly parsed;
    CHECK(TimeOnly::tryParse("1,3", parsed));
    CHECK_EQUAL(std::string{"1,3"}, parsed.toString()); // fidelity
    CHECK(!TimeOnly::tryParse("0,3", parsed));

    CHECK_EQUAL(std::string{""}, EndingNumber{}.toString());
    EndingNumber e;
    CHECK(EndingNumber::tryParse("  ", e)); // the blank-ending case
    CHECK_EQUAL(std::string{"  "}, e.toString());
    CHECK(EndingNumber::tryParse("1, 2", e));
    CHECK_EQUAL(std::size_t{2}, e.values().size());
}

TEST(SmuflPrefixedStructural, Values)
{
    SmuflCodaGlyphName coda;
    CHECK_EQUAL(std::string{"coda"}, coda.toString()); // optional suffix
    coda.setSuffix("Square");
    CHECK_EQUAL(std::string{"codaSquare"}, coda.toString());
    SmuflCodaGlyphName parsed;
    CHECK(SmuflCodaGlyphName::tryParse("codaJapanese", parsed));
    CHECK_EQUAL(std::string{"Japanese"}, parsed.suffix());
    CHECK(!SmuflCodaGlyphName::tryParse("segnoSerpent", parsed));

    SmuflAccidentalGlyphName acc;
    CHECK_EQUAL(std::string{"accX"}, acc.toString()); // required suffix placeholder
    SmuflAccidentalGlyphName kievan{SmuflAccidentalGlyphName::Prefix::kievanAccidental, "Sharp"};
    CHECK_EQUAL(std::string{"kievanAccidentalSharp"}, kievan.toString());
    // A suffix repaired to name characters cannot break the pattern.
    SmuflAccidentalGlyphName dirty{SmuflAccidentalGlyphName::Prefix::acc, "Sagittal 5v7!"};
    CHECK_EQUAL(std::string{"accSagittal5v7"}, dirty.toString());
}

TEST(SmuflWavyStructural, Values)
{
    SmuflWavyLineGlyphName w;
    CHECK_EQUAL(std::string{"wiggleX"}, w.toString());
    SmuflWavyLineGlyphName g{SmuflWavyLineGlyphName::Kind::guitarVibratoStroke, ""};
    CHECK_EQUAL(std::string{"guitarVibratoStroke"}, g.toString());
    SmuflWavyLineGlyphName parsed;
    CHECK(SmuflWavyLineGlyphName::tryParse("guitarWideVibratoStroke", parsed));
    CHECK_EQUAL(std::string{"Wide"}, parsed.part());
    CHECK(SmuflWavyLineGlyphName::tryParse("wiggleTrill", parsed));
    CHECK(!SmuflWavyLineGlyphName::tryParse("vibrato", parsed));
}

TEST(UnionDispatch, Values)
{
    FontSize fs;
    CHECK(fs.isDecimal()); // first member's natural zero
    FontSize css = FontSize::cssFontSize(CSSFontSize::parse("small"));
    CHECK(css.isCSSFontSize());
    CHECK_EQUAL(std::string{"small"}, css.toString());
    FontSize parsed;
    CHECK(FontSize::tryParse("14.5", parsed));
    CHECK(parsed.isDecimal());
    CHECK(FontSize::tryParse("x-large", parsed));
    CHECK(parsed.isCSSFontSize());
    CHECK(!FontSize::tryParse("enormous", parsed));
    // Lenient falls back to the first member's lenient parse.
    CHECK(FontSize::parse("enormous").isDecimal());

    NumberOrNormal n = NumberOrNormal::normal();
    CHECK(n.isNormal());
    CHECK_EQUAL(std::string{"normal"}, n.toString());

    // The primitive member's implied bounds clamp inside the union too.
    PositiveIntegerOrEmpty p = PositiveIntegerOrEmpty::positiveInteger(0);
    CHECK_EQUAL(std::string{"1"}, p.toString());
    PositiveIntegerOrEmpty e;
    CHECK(PositiveIntegerOrEmpty::tryParse("", e));
    CHECK(e.isEmpty());
    CHECK_EQUAL(std::string{""}, e.toString());

    // The open string member absorbs unknown identifiers, last.
    InstrumentSound s = InstrumentSound::parse("brass.alphorn");
    CHECK(s.isSoundID());
    InstrumentSound open = InstrumentSound::parse("custom.thing");
    CHECK(open.isString());
    CHECK_EQUAL(std::string{"custom.thing"}, open.toString());
}

TEST(ModePlainString, Values)
{
    Mode m{std::string{"dorian"}};
    CHECK_EQUAL(std::string{"dorian"}, m.value());
    CHECK_EQUAL(std::string{"dorian"}, Mode::parse("dorian").value());
}

TEST(TokenRepairsToNCName, Values)
{
    // xs:ID / xs:IDREF: the natural zero is a valid non-empty NCName, and
    // every door repairs (spaces and '#'/'!' stripped, a leading digit
    // dropped, emptiness becomes the placeholder "X").
    CHECK_EQUAL(std::string{"X"}, Token{}.toString());
    CHECK_EQUAL(std::string{"hasspacesandbad"}, Token{"has spaces and #bad!"}.toString());
    CHECK_EQUAL(std::string{"note"}, Token{"1note"}.toString()); // NCName forbids leading digit
    CHECK_EQUAL(std::string{"X"}, Token{""}.toString());
    Token t;
    t.setValue("P 1");
    CHECK_EQUAL(std::string{"P1"}, t.toString());
    Token parsed;
    CHECK(Token::tryParse("P1", parsed));
    CHECK(!Token::tryParse("1P", parsed));  // leading digit
    CHECK(!Token::tryParse("P 1", parsed)); // space
    CHECK(!Token::tryParse("", parsed));    // empty
}

TEST(NameTokenRepairsToNmtoken, Values)
{
    // xs:NMTOKEN is wider than an NCName: a leading digit is allowed
    // (<lyric number="1">), but whitespace and emptiness are not.
    CHECK_EQUAL(std::string{"X"}, NameToken{}.toString());
    CHECK_EQUAL(std::string{"1"}, NameToken{"1"}.toString()); // valid NMTOKEN
    CHECK_EQUAL(std::string{"verseone"}, NameToken{"verse one"}.toString());
    NameToken parsed;
    CHECK(NameToken::tryParse("1", parsed));
    CHECK(NameToken::tryParse("verse-1", parsed));
    CHECK(!NameToken::tryParse("verse one", parsed)); // space
    CHECK(!NameToken::tryParse("", parsed));          // empty
}
