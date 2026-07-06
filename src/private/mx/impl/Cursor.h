// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MeasureData.h"
#include "mx/api/TimeChoice.h"

#include <map>

namespace mx
{
namespace core
{
class PositiveDivisions;
}

namespace impl
{
class Cursor
{
  public:
    api::TimeChoice timeSignature;

    // carried per-staff <time number="N"> overrides, keyed by staff index; persists measure-to-measure
    // like the singular timeSignature above
    std::map<int, api::TimeChoice> staffTimeSignatures;

    int ticksPerQuarter;
    int tickTimePosition;
    int voiceIndex;
    int staffIndex;
    bool isBackupInProgress;
    bool isFirstMeasureInPart;

    bool isChordActive;

    Cursor(int numStaves, int globalTicksPerQuarter);
    virtual ~Cursor() = default;
    Cursor(const Cursor &) = default;
    Cursor(Cursor &&) = default;
    Cursor &operator=(const Cursor &) = default;
    Cursor &operator=(Cursor &&) = default;

    int getNumStaves() const;
    int getGlobalTicksPerQuarter() const;

    // use this to clear the state
    // when starting a new measure
    void reset();

    int convertDurationToGlobalTickScale(const core::PositiveDivisions &duration) const;
    int convertDurationToGlobalTickScale(double durationValue) const;
    int convertDurationToGlobalTickScale(int duration) const;

  private:
    int myNumStaves;
    int myGlobalTicksPerQuarter;
};
} // namespace impl
} // namespace mx
