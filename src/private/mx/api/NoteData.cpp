// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/NoteData.h"

namespace mx
{
namespace api
{
NoteData::NoteData()
    : isRest{false}, isMeasureRest{false}, isUnpitched{false}, isDisplayStepOctaveSpecified{false}, isChord{false},
      isTieStart{false}, isTieStop{false}, tieLetRing{}, isGrace{false}, graceSlash{Bool::unspecified}, isCue{false},
      notehead{Notehead::normal}, noteheadFilled{Bool::unspecified}, noteheadSmufl{}, pitchData{},
      userRequestedVoiceNumber{VALUE_UNSPECIFIED}, writeStaffNumber{Bool::unspecified}, stem{Stem::unspecified},
      tickTimePosition{0}, durationData{}, beams{}, positionData{}, printData{}, noteAttachmentData{}, lyrics{}
{
}
} // namespace api
} // namespace mx
