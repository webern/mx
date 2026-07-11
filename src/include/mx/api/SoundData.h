// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// The swing-type-value type: which note type (eighth or 16th) a SwingData ratio applies to.
// unspecified means the source did not state it, which MusicXML treats as "eighth".
enum class SwingNoteType
{
    unspecified,
    eighth,
    dur16th
};

// MusicXML Documentation: The swing element specifies whether or not to use swing playback, where
// consecutive on-beat/off-beat eighth or 16th notes are played with unequal nominal durations.
struct SwingData
{
    // true: <straight/>, no swing, consecutive notes have equal durations. false: a swing ratio
    // is in effect, described by ratioFirst/ratioSecond/noteType below.
    bool isStraight;

    // The swing ratio's two terms (e.g. 2 and 1 for a 2:1 ratio). Only meaningful when
    // isStraight is false.
    int ratioFirst;
    int ratioSecond;

    // Which note type the ratio applies to. Only meaningful when isStraight is false.
    SwingNoteType noteType;

    // A free-text description of the swing style (e.g. "Swing"). Empty means unspecified.
    std::string style;

    SwingData() : isStraight{true}, ratioFirst{0}, ratioSecond{0}, noteType{SwingNoteType::unspecified}, style{}
    {
    }
};

// MusicXML Documentation: The sound element contains general playback parameters. They can stand
// alone within a part/measure, or be a component element within a direction.
//
// mx::api models the commonly-used scalar attributes of <sound>, plus <swing>. The remaining
// nested child elements (<midi-instrument>, <midi-device>, <play>, <offset>) are intentionally
// not modeled.
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

    std::optional<SwingData> swing;

    SoundData()
        : tempo{DOUBLE_UNSPECIFIED}, dynamics{DOUBLE_UNSPECIFIED}, dacapo{Bool::unspecified},
          forwardRepeat{Bool::unspecified}, pizzicato{Bool::unspecified}, segno{}, dalsegno{}, coda{}, tocoda{}, fine{},
          swing{}
    {
    }

    bool isSpecified() const
    {
        return tempo >= 0.0 || dynamics >= 0.0 || dacapo != Bool::unspecified || forwardRepeat != Bool::unspecified ||
               pizzicato != Bool::unspecified || !segno.empty() || !dalsegno.empty() || !coda.empty() ||
               !tocoda.empty() || !fine.empty() || swing.has_value();
    }
};

MXAPI_EQUALS_BEGIN(SwingData)
MXAPI_EQUALS_MEMBER(isStraight)
MXAPI_EQUALS_MEMBER(ratioFirst)
MXAPI_EQUALS_MEMBER(ratioSecond)
MXAPI_EQUALS_MEMBER(noteType)
MXAPI_EQUALS_MEMBER(style)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SwingData);

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
MXAPI_EQUALS_MEMBER(swing)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SoundData);
} // namespace api
} // namespace mx
