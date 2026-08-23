// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/Id.h"
#include "mx/api/PitchData.h"

#include <optional>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
// One retuned string in a scordatura: the pitch the string is tuned to, and optionally which
// string it is (1 is the highest-pitched string). When stringNumber is absent the accords are
// understood to run through the strings in order. tuningAlter/tuningCents alter the step in
// semitones and hundredths of a semitone, as in PitchData; tuningOctave is the octave number
// where 4 is the octave starting at middle C.
struct AccordData
{
    std::optional<int> stringNumber;
    Step tuningStep;
    int tuningAlter;
    double tuningCents;
    int tuningOctave;

    AccordData() : stringNumber{}, tuningStep{Step::c}, tuningAlter{0}, tuningCents{0.0}, tuningOctave{4}
    {
    }
};

MXAPI_EQUALS_BEGIN(AccordData)
MXAPI_EQUALS_MEMBER(stringNumber)
MXAPI_EQUALS_MEMBER(tuningStep)
MXAPI_EQUALS_MEMBER(tuningAlter)
MXAPI_DOUBLES_EQUALS_MEMBER(tuningCents)
MXAPI_EQUALS_MEMBER(tuningOctave)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(AccordData);

// A scordatura, MusicXML's <scordatura> element: tells a string player to retune, listing the
// altered tuning of each affected string. Common in Baroque violin music and in guitar
// notation (drop-D and similar tunings).
class ScordaturaData
{
  public:
    std::vector<AccordData> accords;
    std::optional<Id> id;

    ScordaturaData() : accords{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(ScordaturaData)
MXAPI_EQUALS_MEMBER(accords)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(ScordaturaData);
} // namespace api
} // namespace mx
