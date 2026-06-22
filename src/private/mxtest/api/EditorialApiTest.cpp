// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"

using namespace mx::api;

namespace
{
PartData makeSimplePart(const std::string &id, const std::string &name)
{
    VoiceData voice;
    NoteData n;
    n.tickTimePosition = 0;
    n.pitchData.step = Step::c;
    n.pitchData.octave = 5;
    n.durationData.durationName = DurationName::quarter;
    n.durationData.durationTimeTicks = DEFAULT_TICKS_PER_QUARTER;
    voice.notes.push_back(n);
    StaffData staff{};
    staff.voices.emplace(0, voice);
    MeasureData m;
    m.staves.push_back(staff);
    PartData pd;
    pd.uniqueId = id;
    pd.name = name;
    pd.measures.push_back(m);
    return pd;
}

// Distinct, non-default editorial so a wrong-field assignment in the reader/writer
// would surface as a value mismatch rather than passing by coincidence.
EditorialData makeEditorial(const std::string &footnoteText, const std::string &levelText)
{
    EditorialData ed;
    ed.isFootnoteSpecified = true;
    ed.footnote.text = footnoteText;
    ed.footnote.isColorSpecified = true;
    ed.footnote.colorData.red = 0x12;
    ed.footnote.colorData.green = 0x34;
    ed.footnote.colorData.blue = 0x56;

    ed.isLevelSpecified = true;
    ed.level.value = levelText;
    ed.level.reference = Bool::yes;
    ed.level.type = StartStopSingle::start;
    ed.level.parentheses = Bool::no;
    ed.level.bracket = Bool::yes;
    ed.level.size = SymbolSize::cue;
    return ed;
}

void checkEditorial(const EditorialData &got, const std::string &footnoteText, const std::string &levelText)
{
    CHECK(got.isFootnoteSpecified);
    CHECK_EQUAL(footnoteText, got.footnote.text);
    CHECK(got.footnote.isColorSpecified);
    CHECK_EQUAL(0x12, static_cast<int>(got.footnote.colorData.red));
    CHECK_EQUAL(0x34, static_cast<int>(got.footnote.colorData.green));
    CHECK_EQUAL(0x56, static_cast<int>(got.footnote.colorData.blue));

    CHECK(got.isLevelSpecified);
    CHECK_EQUAL(levelText, got.level.value);
    CHECK(Bool::yes == got.level.reference);
    CHECK(StartStopSingle::start == got.level.type);
    CHECK(Bool::no == got.level.parentheses);
    CHECK(Bool::yes == got.level.bracket);
    CHECK(SymbolSize::cue == got.level.size);
}
} // namespace

// Editorial (<footnote>/<level>) on a <part-group> must survive the api round-trip. Before the
// EditorialData feature the api dropped both elements (PartGroupData had a `// TODO - group
// editorial`).
TEST(editorialRoundTrip, partGroup)
{
    ScoreData in;
    in.parts.push_back(makeSimplePart("P1", "Violin I"));
    in.parts.push_back(makeSimplePart("P2", "Violin II"));

    PartGroupData grp;
    grp.firstPartIndex = 0;
    grp.lastPartIndex = 1;
    grp.number = 1;
    grp.name = "Violins";
    grp.bracketType = BracketType::bracket;
    grp.editorial = makeEditorial("pg-footnote", "pg-level");
    in.partGroups.push_back(grp);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.partGroups.size() == 1);
    checkEditorial(out.partGroups.at(0).editorial, "pg-footnote", "pg-level");
}

// Editorial (<footnote>/<level>) on a <direction> must survive the api round-trip.
TEST(editorialRoundTrip, direction)
{
    ScoreData in;
    in.ticksPerQuarter = DEFAULT_TICKS_PER_QUARTER;
    in.parts.push_back(makeSimplePart("P1", "Flute"));

    DirectionData dir;
    dir.tickTimePosition = 0;
    dir.marks.push_back(MarkData{MarkType::f});
    dir.marks.back().tickTimePosition = 0;
    dir.editorial = makeEditorial("dir-footnote", "dir-level");
    in.parts.at(0).measures.at(0).staves.at(0).directions.push_back(dir);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.parts.size() == 1);
    const auto &staff = out.parts.at(0).measures.at(0).staves.at(0);
    REQUIRE(staff.directions.size() >= 1);
    checkEditorial(staff.directions.at(0).editorial, "dir-footnote", "dir-level");
}

#endif
