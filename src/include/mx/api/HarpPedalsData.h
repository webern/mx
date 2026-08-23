// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"
#include "mx/api/Id.h"
#include "mx/api/PitchData.h"
#include "mx/api/PositionData.h"

#include <optional>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
// One pedal's setting inside a harp pedal diagram: which pedal (named by the pitch step it
// controls) and its alteration. alter is -1 for the flat (pedal up), 0 for natural (pedal
// centered), and 1 for the sharp (pedal down) position; cents carries any microtonal part of
// the alteration (in hundredths of a semitone), as in PitchData.
struct HarpPedalTuning
{
    Step step;
    int alter;
    double cents;

    HarpPedalTuning() : step{Step::c}, alter{0}, cents{0.0}
    {
    }

    HarpPedalTuning(Step inStep, int inAlter) : step{inStep}, alter{inAlter}, cents{0.0}
    {
    }
};

MXAPI_EQUALS_BEGIN(HarpPedalTuning)
MXAPI_EQUALS_MEMBER(step)
MXAPI_EQUALS_MEMBER(alter)
MXAPI_DOUBLES_EQUALS_MEMBER(cents)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(HarpPedalTuning);

// A harp pedal diagram, MusicXML's <harp-pedals> element: shows the position of each of the
// harp's seven pedals. A complete diagram lists all seven in the standard order of the harp's
// pedal mechanism (D, C, B on the left foot; E, F, G, A on the right); pedalTunings preserves
// the order given. positionData captures default/relative x-y plus the horizontal and vertical
// alignment; its placement member is unused here because <harp-pedals> has no placement
// attribute (placement lives on the parent <direction>).
class HarpPedalsData
{
  public:
    std::vector<HarpPedalTuning> pedalTunings;
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<Id> id;

    HarpPedalsData() : pedalTunings{}, positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(HarpPedalsData)
MXAPI_EQUALS_MEMBER(pedalTunings)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(HarpPedalsData);
} // namespace api
} // namespace mx
