// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/NoteData.h"

namespace mx
{
    namespace api
    {
        NoteData::NoteData()
        : isRest{ false }
        , isMeasureRest{ false }
        , isUnpitched{ false }
        , isDisplayStepOctaveSpecified{ false }
        , isChord{ false }
        , isTieStart{ false }
        , isTieStop{ false }
        , noteType{ NoteType::normal }
        , pitchData{}
        , userRequestedVoiceNumber{ -1 }
        , stem{ Stem::unspecified }
        , tickTimePosition{ 0 }
        , durationData{}
        , beams{}
        , positionData{}
        , printData{}
        , noteAttachmentData{}
        {
            
        }
    }
}
