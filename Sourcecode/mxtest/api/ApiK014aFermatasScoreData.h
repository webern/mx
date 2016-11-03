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

        // 1
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
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermata );
        auto& markData = note->noteAttachmentData.marks.back();
        markData.positionData.isDefaultXSpecified = true;
        markData.positionData.defaultX = 1.0;
        markData.positionData.isDefaultYSpecified = true;
        markData.positionData.defaultY = 2.0;
        markData.positionData.isRelativeXSpecified = true;
        markData.positionData.relativeX = 3.0;
        markData.positionData.isRelativeYSpecified = true;
        markData.positionData.relativeY = 4.0;
        markData.printData.fontData.fontFamily.emplace_back( std::string{ "a" } );
        markData.printData.fontData.fontFamily.emplace_back( std::string{ "B" } );
        markData.printData.fontData.fontFamily.emplace_back( std::string{ "Charlie" } );
        markData.printData.fontData.style = mx::api::FontStyle::italic;
        markData.printData.fontData.weight = mx::api::FontWeight::bold;
        markData.printData.isColorSpecified = true;
        markData.printData.color.red = 1;
        markData.printData.color.green = 2;
        markData.printData.color.blue = 3;
        markData.printData.fontData.sizeType = mx::api::FontSizeType::point;
        markData.printData.fontData.sizePoint = 24.0;
        
        // 2
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataNormalUpright );
        
        // 3
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataNormalInverted );
        
        // 4
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 6;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataNormalUpright );
        
        // 5
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 6;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataNormalInverted );
        
        // 6
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataAngledUpright );
        
        // 7
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataAngledInverted );
        
        // 8
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 6;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataAngledUpright );
        
        // 9
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 6;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataAngledInverted );
        
        // 10
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataSquareUpright );
        
        // 11
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataSquareInverted );
        
        // 12
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 6;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataSquareUpright );
        
        // 13
        part.measures.emplace_back( MeasureData{} );
        measure = &part.measures.back();
        measure->staves.emplace_back( StaffData{} );
        staff = &measure->staves.back();
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 6;
        note->durationData.durationName = DurationName::whole;
        note->durationData.durationTimeTicks = 8;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.marks.emplace_back( MarkType::fermataSquareInverted );

        return score;
    }
}
