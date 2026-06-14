// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/CurveData.h"
#include "mx/api/MarkData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

namespace mx
{
namespace api
{
enum class DurationName
{
    unspecified,
    maxima,
    longa,
    breve,
    whole,
    half,
    quarter,
    eighth,
    dur16th,
    dur32nd,
    dur64th,
    dur128th,
    dur256th,
    dur512th,
    dur1024th
};

constexpr double DUR_QUARTERS_VALUE_MAXIMA = 32.0;
constexpr double DUR_QUARTERS_VALUE_LONGA = 16.0;
constexpr double DUR_QUARTERS_VALUE_BREVE = 8.0;
constexpr double DUR_QUARTERS_VALUE_WHOLE = 4.0;
constexpr double DUR_QUARTERS_VALUE_HALF = 2.0;
constexpr double DUR_QUARTERS_VALUE_QUARTER = 1.0;
constexpr double DUR_QUARTERS_VALUE_EIGHTH = 1.0 / 2.0;
constexpr double DUR_QUARTERS_VALUE_16TH = 1.0 / 4.0;
constexpr double DUR_QUARTERS_VALUE_32ND = 1.0 / 8.0;
constexpr double DUR_QUARTERS_VALUE_64TH = 1.0 / 16.0;
constexpr double DUR_QUARTERS_VALUE_128TH = 1.0 / 32.0;
constexpr double DUR_QUARTERS_VALUE_256TH = 1.0 / 64.0;
constexpr double DUR_QUARTERS_VALUE_512TH = 1.0 / 128.0;
constexpr double DUR_QUARTERS_VALUE_1024TH = 1.0 / 256.0;

inline double applyDots(double inUnDottedValue, int inNumDots)
{
    double outValue = inUnDottedValue;
    double valueToAdd = inUnDottedValue / 0.5;

    for (int i = 0; i < inNumDots; ++i)
    {
        outValue += valueToAdd;
        valueToAdd /= 0.5;
    }

    return outValue;
}

struct DurationData
{
    DurationData();

    DurationName durationName; // i.e. quarter, eighth etc
    int durationDots;          // dots
    int durationTimeTicks;     // length of the note denominated in ticksPerQuarter
    bool isTied; // affects sound only. is the note combined sound-wise with the following note of the same pitch
    int timeModificationActualNotes;              // i.e. for a triplet this would be 3
    int timeModificationNormalNotes;              // i.e. for a triplet this would be 2
    api::DurationName timeModificationNormalType; // use this if the time modification ratio involves a different
                                                  // durationName than the main 'durationName'
    int timeModificationNormalTypeDots;           // in case the time modification ratio involves a dotted type
};

MXAPI_EQUALS_BEGIN(DurationData)
MXAPI_EQUALS_MEMBER(durationName)
MXAPI_EQUALS_MEMBER(durationDots)
MXAPI_EQUALS_MEMBER(durationTimeTicks)
MXAPI_EQUALS_MEMBER(isTied)
MXAPI_EQUALS_MEMBER(timeModificationActualNotes)
MXAPI_EQUALS_MEMBER(timeModificationNormalNotes)
MXAPI_EQUALS_MEMBER(timeModificationNormalType)
MXAPI_EQUALS_MEMBER(timeModificationNormalTypeDots)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(DurationData);
} // namespace api
} // namespace mx
