// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/Cursor.h"
#include "mx/core/generated/PositiveDivisions.h"

#include <cmath>

namespace mx
{
namespace impl
{
Cursor::Cursor(int numStaves, int globalTicksPerQuarter)
    : timeSignature{}, ticksPerQuarter(globalTicksPerQuarter), tickTimePosition(0), voiceIndex(0), staffIndex(0),
      isBackupInProgress(false), isFirstMeasureInPart(true), isChordActive(false), myNumStaves(numStaves),
      myGlobalTicksPerQuarter(globalTicksPerQuarter)
{
}

int Cursor::getNumStaves() const
{
    return myNumStaves;
}

int Cursor::getGlobalTicksPerQuarter() const
{
    return myGlobalTicksPerQuarter;
}

void Cursor::reset()
{
    tickTimePosition = 0;
    staffIndex = 0;
    voiceIndex = 0;
    isBackupInProgress = false;
    isChordActive = false;
}

int Cursor::convertDurationToGlobalTickScale(const core::PositiveDivisions &duration) const
{
    return convertDurationToGlobalTickScale(static_cast<double>(duration.value().value()));
}

int Cursor::convertDurationToGlobalTickScale(double durationValue) const
{
    if (this->ticksPerQuarter == this->getGlobalTicksPerQuarter())
    {
        return static_cast<int>(std::ceil(durationValue - 0.5));
    }

    const double currentTicksPerQuarter = static_cast<double>(this->ticksPerQuarter);
    const double globalTicksPerQuarter = static_cast<double>(this->getGlobalTicksPerQuarter());
    const double convertedVal = durationValue * (globalTicksPerQuarter / currentTicksPerQuarter);
    return static_cast<int>(std::ceil(convertedVal - 0.5));
}

int Cursor::convertDurationToGlobalTickScale(int durationValue) const
{
    return convertDurationToGlobalTickScale(static_cast<double>(durationValue));
}
} // namespace impl
} // namespace mx
