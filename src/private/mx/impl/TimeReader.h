// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/TimeSignatureData.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/impl/Cursor.h"

#include <span>
#include <vector>

namespace mx
{
namespace core
{
class Time;
class TimeSignatureGroup;
} // namespace core

namespace impl
{
class TimeReader
{
  public:
    // The constructor will do all of the parsing
    // after that you can query the discovered time
    // element properties using accessors on the
    // cached data
    TimeReader(std::span<const core::MusicDataChoice> inMusicDataChoices);
    bool getIsTimeFound() const;

    // returns one TimeSignatureData per <time> element found in the first <attributes> block
    // that has any. staffIndex is populated from the <time number="..."> attribute (1-based,
    // converted to 0-based); it is NOT clamped against the part's staff count here - the caller
    // (MeasureReader) is responsible for clamping against the actual staff count.
    std::vector<mx::api::TimeSignatureData> getTimeSignatures() const;

  private:
    std::span<const core::MusicDataChoice> myMusicDataChoiceSet;
    bool myIsTimeFound;
    std::vector<mx::api::TimeSignatureData> myTimeSignatures;

  private:
    bool initialize();
    bool parseTime(const core::Time &time);
    bool parseTimeSignatureGroup(const core::Time &time, const core::TimeSignatureGroup &timeSig,
                                 mx::api::TimeSignatureData &outData);
};
} // namespace impl
} // namespace mx
