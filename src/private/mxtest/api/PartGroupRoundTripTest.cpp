// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
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

ScoreData makeTwoPartScore()
{
    ScoreData s;
    s.parts.push_back(makeSimplePart("P1", "Violin I"));
    s.parts.push_back(makeSimplePart("P2", "Violin II"));
    return s;
}
} // namespace

// Regression test for issue #219. A well-formed part-group spanning two parts
// must round-trip through mx::api with full fidelity. Before the fix the writer
// dropped group-abbreviation and both display names, and fabricated a constant
// <group-barline>yes</group-barline> regardless of input -- so the marked
// CHECKs below were the red lines that the fix turns green.
TEST(partGroupRoundTrip, fullFidelity)
{
    auto in = makeTwoPartScore();
    PartGroupData grp;
    grp.firstPartIndex = 0;
    grp.lastPartIndex = 1;
    grp.number = 1;
    grp.name = "Violins";
    grp.displayName = "Violins (display)";
    grp.abbreviation = "Vlns";
    grp.displayAbbreviation = "Vlns (display)";
    grp.bracketType = BracketType::bracket;
    grp.groupBarline = GroupBarline::no;
    in.partGroups.push_back(grp);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.partGroups.size() == 1);
    const auto &got = out.partGroups.at(0);
    CHECK_EQUAL(0, got.firstPartIndex);
    CHECK_EQUAL(1, got.lastPartIndex);
    CHECK_EQUAL(1, got.number);
    CHECK_EQUAL("Violins", got.name);
    CHECK_EQUAL("Violins (display)", got.displayName);      // red before #219 fix (dropped)
    CHECK_EQUAL("Vlns", got.abbreviation);                  // red before #219 fix (dropped)
    CHECK_EQUAL("Vlns (display)", got.displayAbbreviation); // red before #219 fix (dropped)
    CHECK(BracketType::bracket == got.bracketType);
    CHECK(GroupBarline::no == got.groupBarline); // red before #219 fix (fabricated yes)
}

// A part-group that carries no <group-barline> must NOT gain a fabricated one on
// write. (Previously every group was emitted with <group-barline>yes</group-barline>.)
TEST(partGroupRoundTrip, noFabricatedBarline)
{
    auto in = makeTwoPartScore();
    PartGroupData grp;
    grp.firstPartIndex = 0;
    grp.lastPartIndex = 1;
    grp.name = "Group";
    grp.bracketType = BracketType::brace;
    // groupBarline left unspecified on purpose.
    in.partGroups.push_back(grp);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.partGroups.size() == 1);
    CHECK(GroupBarline::unspecified == out.partGroups.at(0).groupBarline);
}

#endif
