// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Tests for writer-side spanner number assignment (issue #297). Spanners
// authored with SpannerNumber::makeIdentity carry no number of their own; the
// writer must assign MusicXML numbers from the actual serialization order,
// which differs from musical-time order when voices are interleaved with
// <backup>.

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/TestHelpers.h"

using namespace mx::api;
using namespace mxtest;

namespace spannerIdentityTest
{
constexpr int ticksPerQuarter = 2;

inline NoteData makeQuarter(int inTick, Step inStep, int inOctave)
{
    NoteData note;
    note.tickTimePosition = inTick;
    note.pitchData.step = inStep;
    note.pitchData.octave = inOctave;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = ticksPerQuarter;
    return note;
}

// The #297 scenario: one staff, two voices, two measures.
//
//   voice 1 (map key 0): m1 has no slurs; m2 carries slur "c" (tick 2 -> 4).
//   voice 2 (map key 1): m1 carries slur "b" (tick 0 -> 2) and starts slur "a"
//                        (tick 4); "a" stops in m2 at tick 0.
//
// Musically "a" and "c" do not overlap: "a" spans m1 tick 4 through m2 tick 0
// and "c" spans m2 ticks 2..4. But voice 1 of m2 is serialized before voice 2
// of m2, so a streaming reader encounters "c"'s start and stop while "a" is
// still open: they must carry different numbers.
inline ScoreData makeVoiceReorderingScore(const SpannerNumber &inA, const SpannerNumber &inB, const SpannerNumber &inC)
{
    ScoreData score;
    score.ticksPerQuarter = ticksPerQuarter;
    score.parts.emplace_back();
    auto &part = score.parts.back();

    // measure 1
    part.measures.emplace_back();
    {
        auto &measure = part.measures.back();
        measure.timeSignature = TimeChoice(TimeSignatureData{"4", "4"});
        measure.timeSignature.isImplicit = false;
        measure.staves.emplace_back();
        auto &staff = measure.staves.back();

        auto &voice1 = staff.voices[0];
        for (int i = 0; i < 4; ++i)
        {
            voice1.notes.push_back(makeQuarter(i * ticksPerQuarter, Step::g, 4));
        }

        auto &voice2 = staff.voices[1];
        for (int i = 0; i < 4; ++i)
        {
            voice2.notes.push_back(makeQuarter(i * ticksPerQuarter, Step::c, 3));
        }

        CurveStart bStart{CurveType::slur};
        bStart.number = inB;
        voice2.notes.at(0).noteAttachmentData.curveStarts.push_back(bStart);

        CurveStop bStop{CurveType::slur};
        bStop.number = inB;
        voice2.notes.at(1).noteAttachmentData.curveStops.push_back(bStop);

        CurveStart aStart{CurveType::slur};
        aStart.number = inA;
        voice2.notes.at(2).noteAttachmentData.curveStarts.push_back(aStart);
    }

    // measure 2
    part.measures.emplace_back();
    {
        auto &measure = part.measures.back();
        measure.timeSignature = TimeChoice(TimeSignatureData{"4", "4"});
        measure.timeSignature.isImplicit = true;
        measure.staves.emplace_back();
        auto &staff = measure.staves.back();

        auto &voice1 = staff.voices[0];
        for (int i = 0; i < 4; ++i)
        {
            voice1.notes.push_back(makeQuarter(i * ticksPerQuarter, Step::a, 4));
        }

        auto &voice2 = staff.voices[1];
        for (int i = 0; i < 4; ++i)
        {
            voice2.notes.push_back(makeQuarter(i * ticksPerQuarter, Step::d, 3));
        }

        CurveStart cStart{CurveType::slur};
        cStart.number = inC;
        voice1.notes.at(1).noteAttachmentData.curveStarts.push_back(cStart);

        CurveStop cStop{CurveType::slur};
        cStop.number = inC;
        voice1.notes.at(2).noteAttachmentData.curveStops.push_back(cStop);

        CurveStop aStop{CurveType::slur};
        aStop.number = inA;
        voice2.notes.at(0).noteAttachmentData.curveStops.push_back(aStop);
    }

    return score;
}
} // namespace spannerIdentityTest

// Three identity slurs. In the serialized stream: "b" opens and closes, "a"
// opens (all in measure 1 voice 2), then measure 2 serializes voice 1 first,
// so "c" opens and closes while "a" is still open. Expect "b" to take 1,
// "a" to reuse 1 (no stream overlap with "b"), and "c" to be forced to 2.
TEST(identityAssignsNumbersFromSerializationOrder, SpannerIdentity)
{
    using namespace spannerIdentityTest;
    const auto score = makeVoiceReorderingScore(SpannerNumber("a"), SpannerNumber("b"), SpannerNumber("c"));

    const auto xml = toXml(score);
    REQUIRE(!xml.empty());
    const auto roundTripped = fromXml(xml);

    REQUIRE(roundTripped.parts.size() == 1);
    const auto &measure1 = roundTripped.parts.at(0).measures.at(0);
    const auto &measure2 = roundTripped.parts.at(0).measures.at(1);
    const auto &m1Voice2 = measure1.staves.at(0).voices.at(1);
    const auto &m2Voice1 = measure2.staves.at(0).voices.at(0);
    const auto &m2Voice2 = measure2.staves.at(0).voices.at(1);

    // b
    REQUIRE(m1Voice2.notes.at(0).noteAttachmentData.curveStarts.size() == 1);
    CHECK(SpannerNumber(1) == m1Voice2.notes.at(0).noteAttachmentData.curveStarts.at(0).number);
    REQUIRE(m1Voice2.notes.at(1).noteAttachmentData.curveStops.size() == 1);
    CHECK(SpannerNumber(1) == m1Voice2.notes.at(1).noteAttachmentData.curveStops.at(0).number);

    // a: reuses 1 because b released it earlier in the stream
    REQUIRE(m1Voice2.notes.at(2).noteAttachmentData.curveStarts.size() == 1);
    CHECK(SpannerNumber(1) == m1Voice2.notes.at(2).noteAttachmentData.curveStarts.at(0).number);
    REQUIRE(m2Voice2.notes.at(0).noteAttachmentData.curveStops.size() == 1);
    CHECK(SpannerNumber(1) == m2Voice2.notes.at(0).noteAttachmentData.curveStops.at(0).number);

    // c: overlaps a in the stream (voice 1 of measure 2 is serialized before
    // voice 2 carries a's stop) so it must not share a's number
    REQUIRE(m2Voice1.notes.at(1).noteAttachmentData.curveStarts.size() == 1);
    CHECK(SpannerNumber(2) == m2Voice1.notes.at(1).noteAttachmentData.curveStarts.at(0).number);
    REQUIRE(m2Voice1.notes.at(2).noteAttachmentData.curveStops.size() == 1);
    CHECK(SpannerNumber(2) == m2Voice1.notes.at(2).noteAttachmentData.curveStops.at(0).number);
}

T_END

// An explicit spanner reserves its number while it is open in the stream: an
// identity spanner whose extent covers the explicit one must skip that number.
TEST(explicitAndIdentityShareThePool, SpannerIdentity)
{
    using namespace spannerIdentityTest;
    const auto score = makeVoiceReorderingScore(SpannerNumber("a"), SpannerNumber("b"), SpannerNumber(1));

    const auto xml = toXml(score);
    REQUIRE(!xml.empty());
    const auto roundTripped = fromXml(xml);

    const auto &measure1 = roundTripped.parts.at(0).measures.at(0);
    const auto &measure2 = roundTripped.parts.at(0).measures.at(1);
    const auto &m1Voice2 = measure1.staves.at(0).voices.at(1);
    const auto &m2Voice1 = measure2.staves.at(0).voices.at(0);
    const auto &m2Voice2 = measure2.staves.at(0).voices.at(1);

    // c is explicit and emitted verbatim
    CHECK(SpannerNumber(1) == m2Voice1.notes.at(1).noteAttachmentData.curveStarts.at(0).number);
    CHECK(SpannerNumber(1) == m2Voice1.notes.at(2).noteAttachmentData.curveStops.at(0).number);

    // a's stream extent covers c's reservation of 1, so a takes 2
    CHECK(SpannerNumber(2) == m1Voice2.notes.at(2).noteAttachmentData.curveStarts.at(0).number);
    CHECK(SpannerNumber(2) == m2Voice2.notes.at(0).noteAttachmentData.curveStops.at(0).number);

    // b does not overlap the reservation and still gets 1
    CHECK(SpannerNumber(1) == m1Voice2.notes.at(0).noteAttachmentData.curveStarts.at(0).number);
    CHECK(SpannerNumber(1) == m1Voice2.notes.at(1).noteAttachmentData.curveStops.at(0).number);
}

T_END

// Slur and tied numbers come from separate pools: two concurrent identity
// spanners of different classes both get number 1.
TEST(slurAndTiePoolsAreIndependent, SpannerIdentity)
{
    using namespace spannerIdentityTest;

    ScoreData score;
    score.ticksPerQuarter = ticksPerQuarter;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.push_back(makeQuarter(0, Step::c, 4));
    voice.notes.push_back(makeQuarter(ticksPerQuarter, Step::c, 4));

    CurveStart slurStart{CurveType::slur};
    slurStart.number = SpannerNumber("s");
    CurveStart tieStart{CurveType::tie};
    tieStart.number = SpannerNumber("t");
    voice.notes.at(0).noteAttachmentData.curveStarts.push_back(slurStart);
    voice.notes.at(0).noteAttachmentData.curveStarts.push_back(tieStart);

    CurveStop slurStop{CurveType::slur};
    slurStop.number = SpannerNumber("s");
    CurveStop tieStop{CurveType::tie};
    tieStop.number = SpannerNumber("t");
    voice.notes.at(1).noteAttachmentData.curveStops.push_back(slurStop);
    voice.notes.at(1).noteAttachmentData.curveStops.push_back(tieStop);

    const auto xml = toXml(score);
    REQUIRE(!xml.empty());
    const auto roundTripped = fromXml(xml);

    const auto &notes = roundTripped.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;
    REQUIRE(notes.at(0).noteAttachmentData.curveStarts.size() == 2);
    for (const auto &start : notes.at(0).noteAttachmentData.curveStarts)
    {
        CHECK(SpannerNumber(1) == start.number);
    }
    REQUIRE(notes.at(1).noteAttachmentData.curveStops.size() == 2);
    for (const auto &stop : notes.at(1).noteAttachmentData.curveStops)
    {
        CHECK(SpannerNumber(1) == stop.number);
    }
}

T_END

// Wedges now round-trip their numbers: an explicit number is emitted verbatim
// and an identity wedge gets an assigned number.
TEST(wedgeNumbersRoundTrip, SpannerIdentity)
{
    using namespace spannerIdentityTest;

    ScoreData score;
    score.ticksPerQuarter = ticksPerQuarter;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    for (int i = 0; i < 4; ++i)
    {
        voice.notes.push_back(makeQuarter(i * ticksPerQuarter, Step::e, 4));
    }

    DirectionData startDirection;
    startDirection.tickTimePosition = 0;
    WedgeStart wedgeStart;
    wedgeStart.wedgeType = WedgeType::crescendo;
    wedgeStart.number = SpannerNumber(3);
    startDirection.wedgeStarts.push_back(wedgeStart);
    staff.directions.push_back(startDirection);

    DirectionData stopDirection;
    stopDirection.tickTimePosition = 2 * ticksPerQuarter;
    WedgeStop wedgeStop;
    wedgeStop.number = SpannerNumber(3);
    stopDirection.wedgeStops.push_back(wedgeStop);
    staff.directions.push_back(stopDirection);

    const auto xml = toXml(score);
    REQUIRE(!xml.empty());
    const auto roundTripped = fromXml(xml);

    const auto &directions = roundTripped.parts.at(0).measures.at(0).staves.at(0).directions;
    bool foundStart = false;
    bool foundStop = false;
    for (const auto &direction : directions)
    {
        for (const auto &start : direction.wedgeStarts)
        {
            foundStart = true;
            CHECK(SpannerNumber(3) == start.number);
        }
        for (const auto &stop : direction.wedgeStops)
        {
            foundStop = true;
            CHECK(SpannerNumber(3) == stop.number);
        }
    }
    CHECK(foundStart);
    CHECK(foundStop);
}

T_END

#endif
