// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"
#include "mxtest/file/MxFileRepository.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
// Builds a single-part, single-measure score whose first staff carries one harmony direction with a
// single default chord. Tests mutate that chord in place via chordOf() before round-tripping.
ScoreData makeScoreWithChord()
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
    return score;
}

ChordData &chordOf(ScoreData &score)
{
    return score.parts.front().measures.front().staves.front().directions.front().chords.front();
}

const ChordData &firstChord(const ScoreData &score)
{
    return score.parts.front().measures.front().staves.front().directions.front().chords.front();
}
} // namespace

TEST(harmonyDegreeLayoutRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::dominantNinth;
    chord.stackDegrees = Bool::yes;
    chord.parenthesesDegrees = Bool::no;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("stack-degrees=\"yes\"") != std::string::npos);
    CHECK(xml.find("parentheses-degrees=\"no\"") != std::string::npos);

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    CHECK(Bool::yes == outChord.stackDegrees);
    CHECK(Bool::no == outChord.parenthesesDegrees);
}

T_END;

TEST(harmonyDegreeLayoutUnspecifiedIsNotWritten, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("stack-degrees") == std::string::npos);
    CHECK(xml.find("parentheses-degrees") == std::string::npos);

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    CHECK(Bool::unspecified == outChord.stackDegrees);
    CHECK(Bool::unspecified == outChord.parenthesesDegrees);
}

T_END;

TEST(harmonyDegreeLayoutFromFile, HarmonyExtrasApi)
{
    const auto score = mxtest::MxFileRepository::loadFile("kind.3.0.xml");
    const auto &outChord = firstChord(score);
    CHECK(Bool::yes == outChord.stackDegrees);
    CHECK(Bool::yes == outChord.parenthesesDegrees);
}

T_END;

// A degree the <kind> text already spells out is marked print-object="no" so it is not printed
// twice. Finale exports exactly this: <kind text="7sus4">suspended-fourth</kind> plus a hidden
// seventh.
TEST(harmonyDegreePrintObjectRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::suspendedFourth;
    chord.text = "7sus4";
    Extension seventh;
    seventh.extensionType = ExtensionType::add;
    seventh.extensionNumber = ExtensionNumber::seventh;
    seventh.extensionAlter = ExtensionAlter::none;
    seventh.printObject = Bool::no;
    chord.extensions.push_back(seventh);

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<degree print-object=\"no\">") != std::string::npos);

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    REQUIRE(outChord.extensions.size() == 1);
    CHECK(Bool::no == outChord.extensions.front().printObject);
    CHECK(ExtensionNumber::seventh == outChord.extensions.front().extensionNumber);
}

T_END;

TEST(harmonyDegreePrintObjectUnspecifiedIsNotWritten, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;
    Extension ninth;
    ninth.extensionType = ExtensionType::add;
    ninth.extensionNumber = ExtensionNumber::ninth;
    ninth.extensionAlter = ExtensionAlter::sharp;
    chord.extensions.push_back(ninth);

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("print-object") == std::string::npos);

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    REQUIRE(outChord.extensions.size() == 1);
    CHECK(Bool::unspecified == outChord.extensions.front().printObject);
}

T_END;

// The read path against a real Finale export: three chord symbols in this file hide a degree the
// kind text already spells out.
TEST(harmonyDegreePrintObjectFromFinaleExport, HarmonyExtrasApi)
{
    const auto score = mxtest::MxFileRepository::loadFile("BrookeWestSample.xml");
    int hiddenDegreeCount = 0;

    for (const auto &part : score.parts)
    {
        for (const auto &measure : part.measures)
        {
            for (const auto &staff : measure.staves)
            {
                for (const auto &direction : staff.directions)
                {
                    for (const auto &chord : direction.chords)
                    {
                        for (const auto &extension : chord.extensions)
                        {
                            if (extension.printObject == Bool::no)
                            {
                                ++hiddenDegreeCount;
                            }
                        }
                    }
                }
            }
        }
    }

    CHECK_EQUAL(3, hiddenDegreeCount);
}

T_END;

TEST(harmonyInversionRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;
    chord.hasInversion = true;
    chord.inversion = 2;

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    CHECK(outChord.hasInversion);
    CHECK_EQUAL(2, outChord.inversion);
    CHECK(HarmonyChordSource::root == outChord.harmonyChordSource);
    CHECK(Step::c == outChord.root);
}

T_END;

TEST(harmonyFrameUnplayedRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;
    chord.hasFrameData = true;
    chord.frameData.unplayed = "x";

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("unplayed=\"x\"") != std::string::npos);

    const auto out = mxtest::roundTrip(score);
    CHECK(out.parts.front().measures.front().staves.front().directions.front().chords.front().frameData.unplayed ==
          std::optional<std::string>{"x"});
}

TEST(harmonyFrameFirstFretRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;
    chord.hasFrameData = true;
    chord.frameData.isFirstFretSpecified = true;
    chord.frameData.firstFret = 5;
    chord.frameData.firstFretText = "5fr.";
    chord.frameData.firstFretLocation = FirstFretLocation::right;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("text=\"5fr.\"") != std::string::npos);
    CHECK(xml.find("location=\"right\"") != std::string::npos);

    const auto out = mxtest::roundTrip(score);
    const auto &outFrame = firstChord(out).frameData;
    CHECK(outFrame.isFirstFretSpecified);
    CHECK_EQUAL(5, outFrame.firstFret);
    CHECK(outFrame.firstFretText == std::optional<std::string>{"5fr."});
    CHECK(FirstFretLocation::right == outFrame.firstFretLocation);
}

T_END;

TEST(harmonyFrameFirstFretBareRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;
    chord.hasFrameData = true;
    chord.frameData.isFirstFretSpecified = true;
    chord.frameData.firstFret = 3;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<first-fret>3</first-fret>") != std::string::npos);

    const auto out = mxtest::roundTrip(score);
    const auto &outFrame = firstChord(out).frameData;
    CHECK_EQUAL(3, outFrame.firstFret);
    CHECK(!outFrame.firstFretText.has_value());
    CHECK(FirstFretLocation::unspecified == outFrame.firstFretLocation);
}

T_END;

TEST(harmonyFunctionRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.harmonyChordSource = HarmonyChordSource::function;
    chord.functionText = "V";
    chord.chordKind = ChordKind::major;

    const auto out = mxtest::roundTrip(score);
    const auto &outChord = firstChord(out);
    CHECK(HarmonyChordSource::function == outChord.harmonyChordSource);
    CHECK_EQUAL("V", outChord.functionText);
}

T_END;

TEST(harmonyNumeralRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.harmonyChordSource = HarmonyChordSource::numeral;
    chord.numeralRoot = 5;
    chord.numeralRootText = "V";
    chord.hasNumeralAlter = true;
    chord.numeralAlter = 0;
    chord.hasNumeralKey = true;
    chord.numeralKeyFifths = -3;
    chord.numeralMode = NumeralMode::minor;
    chord.chordKind = ChordKind::major;

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

// <harmony> carries the same system attribute as <direction>; a harmony-only direction writes it
// and reads it back.
TEST(harmonySystemRelationRoundTrip, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;
    auto &direction = score.parts.front().measures.front().staves.front().directions.front();
    direction.systemRelation = SystemRelation::alsoTop;

    const auto out = mxtest::roundTrip(score);
    const auto &outDirection = out.parts.front().measures.front().staves.front().directions.front();
    CHECK(SystemRelation::alsoTop == outDirection.systemRelation);
    CHECK(Step::c == firstChord(out).root);
}

T_END;

// A harmony-only direction left at the default writes no system attribute.
TEST(harmonySystemRelationUnspecified, HarmonyExtrasApi)
{
    auto score = makeScoreWithChord();
    auto &chord = chordOf(score);
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;

    const auto out = mxtest::roundTrip(score);
    const auto &outDirection = out.parts.front().measures.front().staves.front().directions.front();
    CHECK(SystemRelation::unspecified == outDirection.systemRelation);
}

T_END;

#endif
