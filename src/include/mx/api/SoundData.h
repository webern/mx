// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>

namespace mx
{
namespace api
{
// MusicXML Documentation: The sound element contains general playback parameters. They can stand
// alone within a part/measure, or be a component element within a direction.
//
// mx::api models the commonly-used scalar attributes of <sound>. The nested child elements
// (<midi-instrument>, <midi-device>, <play>, <swing>, <offset>) are intentionally not modeled.
//
// A SoundData is carried on DirectionData. When a DirectionData holds a SoundData but no other
// direction content, it round-trips as a standalone <sound> element within the measure. When a
// DirectionData holds other direction content in addition to the SoundData, the <sound> is written
// as a child of the <direction> element.
struct SoundData
{
    // tempo in quarter notes per minute. A value less than 0 means 'unspecified'.
    double tempo;

    // dynamics (MIDI velocity) as a percentage of the default forte value. A value less than 0
    // means 'unspecified'.
    double dynamics;

    // Dacapo indicates to go back to the beginning of the movement. When used it always has the
    // value 'yes'.
    Bool dacapo;

    // forward-repeat indicates that a forward repeat sign is implied but not displayed. When used
    // it always has the value 'yes'.
    Bool forwardRepeat;

    // Pizzicato in a sound element effects all following notes. 'yes' indicates pizzicato, 'no'
    // indicates arco.
    Bool pizzicato;

    // Segno and dalsegno are used for backwards jumps to a segno sign; coda and tocoda are used for
    // forward jumps to a coda sign. The fine attribute follows the final note or rest in a movement
    // with a da capo or dal segno direction. These are strings; an empty string means 'unspecified'.
    std::string segno;
    std::string dalsegno;
    std::string coda;
    std::string tocoda;
    std::string fine;

    SoundData()
        : tempo{-1.0}, dynamics{-1.0}, dacapo{Bool::unspecified}, forwardRepeat{Bool::unspecified},
          pizzicato{Bool::unspecified}, segno{}, dalsegno{}, coda{}, tocoda{}, fine{}
    {
    }

    bool isSpecified() const
    {
        return tempo >= 0.0 || dynamics >= 0.0 || dacapo != Bool::unspecified || forwardRepeat != Bool::unspecified ||
               pizzicato != Bool::unspecified || !segno.empty() || !dalsegno.empty() || !coda.empty() ||
               !tocoda.empty() || !fine.empty();
    }
};

MXAPI_EQUALS_BEGIN(SoundData)
MXAPI_DOUBLES_EQUALS_MEMBER(tempo)
MXAPI_DOUBLES_EQUALS_MEMBER(dynamics)
MXAPI_EQUALS_MEMBER(dacapo)
MXAPI_EQUALS_MEMBER(forwardRepeat)
MXAPI_EQUALS_MEMBER(pizzicato)
MXAPI_EQUALS_MEMBER(segno)
MXAPI_EQUALS_MEMBER(dalsegno)
MXAPI_EQUALS_MEMBER(coda)
MXAPI_EQUALS_MEMBER(tocoda)
MXAPI_EQUALS_MEMBER(fine)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SoundData);
} // namespace api
} // namespace mx
