// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
        , notehead{ Notehead::normal }
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
