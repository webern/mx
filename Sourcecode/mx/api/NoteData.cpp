// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/NoteData.h"

namespace mx
{
    namespace api
    {
        NoteData::NoteData()
        : absoluteNoteIndex{ 0 }
        , isRest{ false }
        , isMeasureRest{ false }
        , isUnpitched{ false }
        , isDisplayStepOctaveSpecified{ false }
        , isChord{ false }
        , noteType{ NoteType::normal }
        , pitchData{}
        , staffIndex{ 0 }
        , userRequestedVoiceNumber{ -1 }
        , stem{ Stem::unspecified }
        , startTimeTicks{ 0 }
        , durationData{}
        , beams{}
        , positionData{}
        , printData{}
        , noteAttachmentData{}
        {
            
        }
    }
}
