// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PitchData.h"

namespace mx
{
namespace api
{
// The pitch an open string is tuned to on a tablature staff, MusicXML's <staff-tuning>. line says
// which staff line the string is drawn on, counting from the bottom line as 1. tuningAlter and
// tuningCents alter tuningStep in semitones and hundredths of a semitone, as in PitchData;
// tuningOctave is the octave number where 4 is the octave starting at middle C.
struct StaffTuningData
{
    int line;
    Step tuningStep;
    int tuningAlter;
    double tuningCents;
    int tuningOctave;

    StaffTuningData() : line{1}, tuningStep{Step::c}, tuningAlter{0}, tuningCents{0.0}, tuningOctave{4}
    {
    }
};

MXAPI_EQUALS_BEGIN(StaffTuningData)
MXAPI_EQUALS_MEMBER(line)
MXAPI_EQUALS_MEMBER(tuningStep)
MXAPI_EQUALS_MEMBER(tuningAlter)
MXAPI_DOUBLES_EQUALS_MEMBER(tuningCents)
MXAPI_EQUALS_MEMBER(tuningOctave)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(StaffTuningData);
} // namespace api
} // namespace mx
