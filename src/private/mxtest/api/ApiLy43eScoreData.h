// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"

namespace mxtest
{
inline mx::api::ScoreData apiLy43eScoreData()
{
    using namespace mx::api;
    ScoreData score;
    score.parts.emplace_back(PartData{});
    score.ticksPerQuarter = 8;
    score.encoding.miscelaneousFields.emplace_back(
        "description", "A piano staff with dynamics and \n          clef changes, where each element (ffff, wedge and "
                       "clef changes) \n          applies only to one voice or one staff, respectively.");

    auto &part = score.parts.front();
    part.uniqueId = "P1";
    part.name = "MusicXML Part";

    // measure 1 - setup
    part.measures.emplace_back(MeasureData{});
    auto measureP = &part.measures.front();
    measureP->keys.emplace_back(KeyData{});
    auto keyP = &measureP->keys.back();
    keyP->mode = KeyMode::major;
    measureP->timeSignature = TimeChoice(TimeSignatureData{"4", "4", TimeSignatureSymbol::common});
    measureP->timeSignature.isImplicit = false;
    measureP->staves.emplace_back(StaffData{});
    measureP->staves.emplace_back(StaffData{});
    auto staff1P = &measureP->staves.at(0);
    staff1P->clefs.emplace_back(ClefData{});
    auto staff2P = &measureP->staves.at(1);
    staff2P->clefs.emplace_back(ClefData{});
    auto clefP = &staff2P->clefs.back();
    clefP->symbol = ClefSymbol::f;
    clefP->line = 4;
    staff1P->directions.emplace_back(DirectionData{});
    auto directionP = &staff1P->directions.back();
    directionP->placement = Placement::below;
    MarkData directionMark{StandardDynamic::ffff};
    directionMark.tickTimePosition = 0;
    directionP->directionTypes.emplace_back(DirectionChoice{directionMark});

    // measure 1 - music
    int voice = 0;
    staff1P->voices[voice].notes.emplace_back(NoteData{});
    auto noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::g;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->directions.emplace_back(DirectionData{});
    directionP = &staff1P->directions.back();
    directionP->tickTimePosition = 25;
    //        directionP->isOffsetSpecified = true;
    //        directionP->offset = 1;
    directionP->placement = Placement::below;
    directionP->isStaffValueSpecified = true;
    directionMark = MarkData{StandardDynamic::p};
    directionMark.tickTimePosition = directionP->tickTimePosition;
    directionP->directionTypes.emplace_back(DirectionChoice{directionMark});

    voice = 1;
    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 2;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->directions.emplace_back(DirectionData{});
    directionP = &staff2P->directions.back();
    directionP->tickTimePosition = 0;
    //        directionP->isOffsetSpecified = false;
    //        directionP->offset = 0;
    directionP->placement = Placement::below;
    directionP->isStaffValueSpecified = true;
    WedgeStart wedgeStart{};
    wedgeStart.wedgeType = WedgeType::crescendo;
    wedgeStart.isSpreadSpecified = true;
    wedgeStart.spread = 0.0;
    directionP->directionTypes.emplace_back(DirectionChoice{wedgeStart});

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 2;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->directions.emplace_back(DirectionData{});
    directionP = &staff2P->directions.back();
    directionP->tickTimePosition = 16;
    //        directionP->isOffsetSpecified = false;
    //        directionP->offset = 0;
    directionP->placement = Placement::unspecified;
    directionP->isStaffValueSpecified = true;
    WedgeStop wedgeStop{};
    wedgeStop.isSpreadSpecified = true;
    wedgeStop.spread = 15.0;
    directionP->directionTypes.emplace_back(DirectionChoice{wedgeStop});

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 3;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 3;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    // measure 2 - setup
    part.measures.emplace_back(MeasureData{});
    measureP = &part.measures.back();
    measureP->timeSignature = TimeChoice(TimeSignatureData{"4", "4", TimeSignatureSymbol::common});
    measureP->timeSignature.isImplicit = true;
    measureP->keys.emplace_back(KeyData{});
    keyP = &measureP->keys.back();
    keyP->fifths = 2;
    keyP->mode = KeyMode::major;
    measureP->staves.emplace_back(StaffData{});
    measureP->staves.emplace_back(StaffData{});
    staff1P = &measureP->staves.at(0);
    staff2P = &measureP->staves.at(1);
    staff2P->clefs.emplace_back(ClefData{});
    clefP = &staff2P->clefs.back();
    clefP->symbol = ClefSymbol::g;
    clefP->line = 2;

    // measure 2 - music
    voice = 0;
    staff1P = &measureP->staves.at(0);
    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 1;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    voice = 1;
    staff2P = &measureP->staves.at(1);
    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::f;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 1;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::g;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    // measure 3 - setup
    part.measures.emplace_back(MeasureData{});
    measureP = &part.measures.back();
    measureP->timeSignature = TimeChoice(TimeSignatureData{"4", "4", TimeSignatureSymbol::common});
    measureP->timeSignature.isImplicit = true;
    measureP->staves.emplace_back(StaffData{});
    measureP->staves.emplace_back(StaffData{});
    staff1P = &measureP->staves.at(0);
    staff2P = &measureP->staves.at(1);
    staff1P->clefs.emplace_back(ClefData{});
    clefP = &staff1P->clefs.back();
    clefP->symbol = ClefSymbol::c;
    clefP->line = 2;

    // measure 3 - music
    voice = 0;
    staff1P = &measureP->staves.at(0);
    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 1;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    voice = 1;
    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 1;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    // measure 4 - setup
    part.measures.emplace_back(MeasureData{});
    measureP = &part.measures.back();
    measureP->timeSignature = TimeChoice(TimeSignatureData{"4", "4", TimeSignatureSymbol::common});
    measureP->timeSignature.isImplicit = true;
    measureP->staves.emplace_back(StaffData{});
    measureP->staves.emplace_back(StaffData{});
    staff1P = &measureP->staves.at(0);
    staff2P = &measureP->staves.at(1);

    // measure 4 - music
    voice = 0;
    staff1P = &measureP->staves.at(0);
    staff1P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff1P->voices[voice].notes.back();
    measureP->timeSignature = TimeChoice(TimeSignatureData{"4", "4", TimeSignatureSymbol::common});
    measureP->timeSignature.isImplicit = true;
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::whole;
    noteP->isRest = true;
    noteP->durationData.durationTimeTicks = 32;

    voice = 1;
    staff2P->voices[voice].notes.emplace_back(NoteData{});
    noteP = &staff2P->voices[voice].notes.back();
    noteP->userRequestedVoiceNumber = voice + 1;
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::whole;
    noteP->isRest = true;
    noteP->durationData.durationTimeTicks = 32;

    measureP->barlines.emplace_back(BarlineData{});
    auto barlineP = &measureP->barlines.back();
    barlineP->tickTimePosition = std::numeric_limits<int>::max();
    barlineP->barlineType = BarlineType::lightHeavy;
    barlineP->location = mx::api::HorizontalAlignment::right;

    return score;
}
} // namespace mxtest
