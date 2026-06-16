// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
// Builds a single-part, single-measure score whose first staff carries one harmony direction.
// The caller mutates the returned chord (via the out-param) before round-tripping.
ScoreData makeScoreWithChord(ChordData &outChord)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.name = "P1";
    part.uniqueId = "P1";
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.directions.emplace_back();
    auto &direction = staff.directions.back();
    direction.tickTimePosition = 0;
    direction.isStaffValueSpecified = true;
    direction.chords.emplace_back();
    outChord = ChordData{};
    return score;
}

const ChordData &firstChord(const ScoreData &score)
{
    return score.parts.front().measures.front().staves.front().directions.front().chords.front();
}
} // namespace

TEST(harmonyInversionRoundTrip, HarmonyExtrasApi)
{
    ChordData chord{};
    auto score = makeScoreWithChord(chord);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;
    chord.hasInversion = true;
    chord.inversion = 2;
    score.parts.front().measures.front().staves.front().directions.front().chords.front() = chord;

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    CHECK(outChord.hasInversion);
    CHECK_EQUAL(2, outChord.inversion);
    CHECK(HarmonyChordSource::root == outChord.harmonyChordSource);
    CHECK(Step::c == outChord.root);
}

T_END;

TEST(harmonyFunctionRoundTrip, HarmonyExtrasApi)
{
    ChordData chord{};
    auto score = makeScoreWithChord(chord);
    chord.harmonyChordSource = HarmonyChordSource::function;
    chord.functionText = "V";
    chord.chordKind = ChordKind::major;
    score.parts.front().measures.front().staves.front().directions.front().chords.front() = chord;

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    CHECK(HarmonyChordSource::function == outChord.harmonyChordSource);
    CHECK_EQUAL("V", outChord.functionText);
}

T_END;

TEST(harmonyNumeralRoundTrip, HarmonyExtrasApi)
{
    ChordData chord{};
    auto score = makeScoreWithChord(chord);
    chord.harmonyChordSource = HarmonyChordSource::numeral;
    chord.numeralRoot = 5;
    chord.numeralRootText = "V";
    chord.hasNumeralAlter = true;
    chord.numeralAlter = 0;
    chord.hasNumeralKey = true;
    chord.numeralKeyFifths = -3;
    chord.numeralMode = NumeralMode::minor;
    chord.chordKind = ChordKind::major;
    score.parts.front().measures.front().staves.front().directions.front().chords.front() = chord;

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    CHECK(HarmonyChordSource::numeral == outChord.harmonyChordSource);
    CHECK_EQUAL(5, outChord.numeralRoot);
    CHECK_EQUAL("V", outChord.numeralRootText);
    CHECK(outChord.hasNumeralAlter);
    CHECK_EQUAL(0, outChord.numeralAlter);
    CHECK(outChord.hasNumeralKey);
    CHECK_EQUAL(-3, outChord.numeralKeyFifths);
    CHECK(NumeralMode::minor == outChord.numeralMode);
}

T_END;

#endif
