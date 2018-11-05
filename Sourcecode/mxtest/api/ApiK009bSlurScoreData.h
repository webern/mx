// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/impl/Converter.h"
#include "mx/core/Enums.h"

namespace mxtest
{
    inline mx::api::ScoreData apiK009bSlurAttributesScoreData()
    {
        using namespace mx::api;
        ScoreData score;
        score.musicXmlVersion = mx::api::MusicXmlVersion::ThreePointZero;
        score.parts.emplace_back( PartData{} );
        score.ticksPerQuarter = 2;
        score.encoding.encodingDate.year = 2016;
        score.encoding.encodingDate.month = 11;
        score.encoding.encodingDate.day = 3;
        score.encoding.software.emplace_back( "My Hands" );
        score.movementTitle = "k009b_Slur_Attributes";
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
        note->tickTimePosition = 0;
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::c;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::quarter;
        note->durationData.durationTimeTicks = 2;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.curveStarts.emplace_back( CurveType::slur );
        auto& slurStart = note->noteAttachmentData.curveStarts.back();
        slurStart.numberLevel = 1;
        slurStart.curvePoints.isBezierXSpecified = true;
        slurStart.curvePoints.bezierX = 1.0;
        slurStart.curvePoints.isBezierYSpecified = true;
        slurStart.curvePoints.bezierY = 2.0;
        slurStart.curvePoints.positionData.isDefaultXSpecified = true;
        slurStart.curvePoints.positionData.defaultX = 3.0;
        slurStart.curvePoints.positionData.isDefaultYSpecified = true;
        slurStart.curvePoints.positionData.defaultY = 4.0;
        slurStart.curvePoints.positionData.placement = Placement::above;
        slurStart.curveOrientation = CurveOrientation::underhand;
        slurStart.lineData.lineType = LineType::dashed;
        slurStart.lineData.isDashLengthSpecified = true;
        slurStart.lineData.dashLength = 10.0;
        slurStart.lineData.isSpaceLengthSpecified = true;
        slurStart.lineData.spaceLength = 10.0;
        
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->tickTimePosition = 2;
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::d;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::quarter;
        note->durationData.durationTimeTicks = 2;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.curveContinuations.emplace_back( CurveType::slur );
        auto& continue1 = note->noteAttachmentData.curveContinuations.back();
        continue1.numberLevel = 1;
        continue1.isBezierX2Specified = true;
        continue1.bezierX2 = 5.0;
        continue1.isBezierY2Specified = true;
        continue1.bezierY2 = 6.0;
        continue1.curvePoints.positionData.isDefaultXSpecified = true;
        continue1.curvePoints.positionData.defaultX = 7.0;
        continue1.curvePoints.positionData.isDefaultYSpecified = true;
        continue1.curvePoints.positionData.defaultY = 8.0;
        continue1.curvePoints.positionData.placement = Placement::below;
        
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->tickTimePosition = 4;
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::e;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::quarter;
        note->durationData.durationTimeTicks = 2;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.curveContinuations.emplace_back( CurveType::slur );
        auto& continue2 = note->noteAttachmentData.curveContinuations.back();
        continue2.numberLevel = 1;
        continue2.curvePoints.isBezierXSpecified = true;
        continue2.curvePoints.bezierX = 9.0;
        continue2.curvePoints.isBezierYSpecified = true;
        continue2.curvePoints.bezierY = 10.0;
        continue2.curvePoints.isBezierOffsetSpecified = true;
        continue2.curvePoints.bezierOffset = 11;
        continue2.isBezierOffset2Specified = true;
        continue2.bezierOffset2 = 12;
        continue2.curvePoints.positionData.placement = Placement::above;

        
        staff->voices[0].notes.emplace_back( NoteData{} );
        note = &staff->voices[0].notes.back();
        note->tickTimePosition = 6;
        note->userRequestedVoiceNumber = 1;
        note->pitchData.step = Step::f;
        note->pitchData.octave = 4;
        note->durationData.durationName = DurationName::quarter;
        note->durationData.durationTimeTicks = 2;
        note->pitchData.accidental = Accidental::none;
        note->noteAttachmentData.curveStops.emplace_back( CurveType::slur );
        auto& end = note->noteAttachmentData.curveStops.back();
        end.numberLevel = 1;
        end.curvePoints.isBezierXSpecified = true;
        end.curvePoints.bezierX = 53.0;
        end.curvePoints.isBezierYSpecified = true;
        end.curvePoints.bezierY = 69.0;
        end.curvePoints.positionData.isDefaultXSpecified = true;
        end.curvePoints.positionData.defaultX = 18.0;
        end.curvePoints.positionData.isDefaultYSpecified = true;
        end.curvePoints.positionData.defaultY = 0.0;
        end.curvePoints.positionData.placement = Placement::below;
        
        return score;
    }
}
