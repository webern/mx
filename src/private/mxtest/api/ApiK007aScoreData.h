// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"

#include <utility>

namespace
{

inline void addNoteToMeasure(mx::api::MarkData dynamic, mx::api::MeasureData *measureP)
{
    using namespace mx::api;
    auto staffP = &measureP->staves.at(0);
    int voice = 0;
    staffP->voices[voice].notes.emplace_back(NoteData{});
    auto noteP = &staffP->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::whole;
    noteP->durationData.durationTimeTicks = 8;
    auto &markData = noteP->noteAttachmentData.marks.emplace_back(std::move(dynamic));
    markData.tickTimePosition = noteP->tickTimePosition;
    markData.positionData.placement = Placement::below;
}

inline void addMeasureWithNote(mx::api::MarkData dynamic, mx::api::PartData &outPartData)
{
    using namespace mx::api;
    outPartData.measures.emplace_back(MeasureData{});
    auto measureP = &outPartData.measures.back();
    measureP->staves.emplace_back(StaffData{});
    addNoteToMeasure(std::move(dynamic), measureP);
}

inline void addFirstMeasureWithNote(mx::api::MarkData dynamic, mx::api::PartData &outPartData)
{
    using namespace mx::api;
    outPartData.measures.emplace_back(MeasureData{});
    auto measureP = &outPartData.measures.front();
    measureP->timeSignature = TimeChoice(TimeSignatureData{});
    measureP->timeSignature.isImplicit = false;
    measureP->staves.emplace_back(StaffData{});
    auto staffP = &measureP->staves.at(0);
    staffP->clefs.emplace_back(ClefData{});
    auto clefP = &staffP->clefs.back();
    clefP->symbol = ClefSymbol::g;
    clefP->line = 2;
    addNoteToMeasure(std::move(dynamic), measureP);
}
} // namespace

namespace mxtest
{
inline mx::api::ScoreData apiK007aScoreData()
{
    using namespace mx::api;
    ScoreData score;
    score.musicXmlVersion = mx::api::MusicXmlVersion::ThreePointZero;
    score.parts.emplace_back(PartData{});
    score.ticksPerQuarter = 2;
    auto &part = score.parts.front();
    part.uniqueId = "P1";
    part.name = "Dynamics";

    addFirstMeasureWithNote(StandardDynamic::p, part);
    addMeasureWithNote(StandardDynamic::pp, part);
    addMeasureWithNote(StandardDynamic::ppp, part);
    addMeasureWithNote(StandardDynamic::pppp, part);
    addMeasureWithNote(StandardDynamic::ppppp, part);
    addMeasureWithNote(StandardDynamic::pppppp, part);
    addMeasureWithNote(StandardDynamic::f, part);
    addMeasureWithNote(StandardDynamic::ff, part);
    addMeasureWithNote(StandardDynamic::fff, part);
    addMeasureWithNote(StandardDynamic::ffff, part);
    addMeasureWithNote(StandardDynamic::fffff, part);
    addMeasureWithNote(StandardDynamic::ffffff, part);
    addMeasureWithNote(StandardDynamic::mp, part);
    addMeasureWithNote(StandardDynamic::mf, part);
    addMeasureWithNote(StandardDynamic::sf, part);
    addMeasureWithNote(StandardDynamic::sfp, part);
    addMeasureWithNote(StandardDynamic::sfpp, part);
    addMeasureWithNote(StandardDynamic::fp, part);
    addMeasureWithNote(StandardDynamic::rf, part);
    addMeasureWithNote(StandardDynamic::rfz, part);
    addMeasureWithNote(StandardDynamic::sfz, part);
    addMeasureWithNote(StandardDynamic::sffz, part);
    addMeasureWithNote(StandardDynamic::fz, part);
    addMeasureWithNote(CompoundDynamicsData{{OtherDynamicsData{"dynamicNiente"}}}, part);

    return score;
}
} // namespace mxtest
