// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/generated/MIDIInstrument.h"
#include "mx/core/generated/PartList.h"
#include "mx/core/generated/PartListChoice.h"
#include "mx/core/generated/ScorePart.h"
#include "mx/core/generated/ScorePartMIDIGroup.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/impl/ScoreWriter.h"

using namespace mx;
using namespace mx::impl;

namespace
{
// A part with playback data but no instrument id of its own, which is the case where the writer
// has to name the instrument itself.
api::PartData makeMidiPart(const std::string &partId, int channel)
{
    api::PartData partData;
    partData.uniqueId = partId;
    partData.name = "Flute";
    partData.instrumentData.midiData.name = "Flute";
    partData.instrumentData.midiData.channel = channel;

    api::MeasureData measure;
    api::StaffData staff;
    api::NoteData note;
    note.durationData.durationName = api::DurationName::quarter;
    note.durationData.durationTimeTicks = api::DEFAULT_TICKS_PER_QUARTER;
    staff.voices[0].notes.push_back(note);
    measure.staves.push_back(staff);
    partData.measures.push_back(measure);

    return partData;
}

api::ScoreData makeTwoMidiParts()
{
    api::ScoreData scoreData;
    scoreData.ticksPerQuarter = api::DEFAULT_TICKS_PER_QUARTER;
    scoreData.parts.push_back(makeMidiPart("P1", 1));
    scoreData.parts.push_back(makeMidiPart("P2", 2));
    return scoreData;
}

const core::ScorePart &firstScorePart(const core::ScorePartwise &scorePartwise)
{
    return scorePartwise.scoreHeader().partList().scorePart();
}

const core::ScorePart &secondScorePart(const core::ScorePartwise &scorePartwise)
{
    return scorePartwise.scoreHeader().partList().choice().front().asScorePart();
}

std::string scoreInstrumentId(const core::ScorePart &scorePart)
{
    return scorePart.scoreInstrument().front().id().value();
}

std::string midiInstrumentId(const core::ScorePart &scorePart)
{
    return scorePart.midiGroup().front().midiInstrument()->id().value();
}
} // namespace

TEST(partInstrumentId, ScoreWriter)
{
    // <midi-instrument> names an instrument defined in <part-list>, so the two elements have to
    // carry the same id even when the caller named no instrument id to start with.
    ScoreWriter scoreWriter{makeTwoMidiParts()};
    const auto scorePartwise = scoreWriter.getScorePartwise();

    const auto &first = firstScorePart(scorePartwise);
    const auto &second = secondScorePart(scorePartwise);

    CHECK_EQUAL(scoreInstrumentId(first), midiInstrumentId(first));
    CHECK_EQUAL(scoreInstrumentId(second), midiInstrumentId(second));
    CHECK(scoreInstrumentId(first) != scoreInstrumentId(second));
}

TEST(partInstrumentIdIsTheSameOnEveryWrite, ScoreWriter)
{
    ScoreWriter scoreWriter{makeTwoMidiParts()};
    const auto firstWrite = scoreWriter.getScorePartwise();

    // Writing an unrelated score in between used to shift the ids, because the counter that fed
    // them outlived the call.
    ScoreWriter otherWriter{makeTwoMidiParts()};
    static_cast<void>(otherWriter.getScorePartwise());

    const auto secondWrite = scoreWriter.getScorePartwise();
    CHECK_EQUAL(scoreInstrumentId(firstScorePart(firstWrite)), scoreInstrumentId(firstScorePart(secondWrite)));
}

TEST(aCallersInstrumentIdIsKept, ScoreWriter)
{
    auto scoreData = makeTwoMidiParts();
    scoreData.parts.at(0).instrumentData.uniqueId = "P1-I1";

    ScoreWriter scoreWriter{scoreData};
    const auto scorePartwise = scoreWriter.getScorePartwise();
    const auto &first = firstScorePart(scorePartwise);

    CHECK_EQUAL(std::string{"P1-I1"}, scoreInstrumentId(first));
    CHECK_EQUAL(std::string{"P1-I1"}, midiInstrumentId(first));
}

T_END

#endif
