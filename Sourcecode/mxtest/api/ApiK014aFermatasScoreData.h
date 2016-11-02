// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/api/Smufl.h"
#include "mx/impl/Converter.h"
#include "mx/core/Enums.h"

namespace mxtest
{
    inline mx::api::ScoreData apiK014aFermatasScoreData()
    {
        using namespace mx::api;
        ScoreData score;
        score.parts.emplace_back( PartData{} );
        score.ticksPerQuarter = 2;
        
        auto& part = score.parts.front();
        part.uniqueId = "P1";
        part.name = "MusicXML Part";

        part.measures.emplace_back( MeasureData{} );
        auto measure = &part.measures.back();
        measure->timeSignature.beats = 4;
        measure->timeSignature.beatType = 4;
        measure->timeSignature.isImplicit = false;
        measure->staves.emplace_back( StaffData{} );
        auto staff = &measure->staves.back();
        staff->clefs.emplace_back( ClefData{} );
        auto& clef = staff->clefs.back();
        clef.line = 2;
        
        staff->voices[0].notes.emplace_back( NoteData{} );
        auto note = &staff->voices[0].notes.back();
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->stem = Stem::up;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermata );
        
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->stem = Stem::up;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermata );
        
        return score;
    }
}
