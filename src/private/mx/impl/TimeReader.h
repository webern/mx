// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/TimeChoice.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/Time.h"

#include <span>
#include <vector>

namespace mx
{
namespace impl
{
// A single translated <time>: the resulting TimeChoice plus the staff it is scoped to
// (INDEX_UNSPECIFIED for an unscoped <time> that applies to all staves).
struct TimeReaderResult
{
    api::TimeChoice timeChoice;
    int staffIndex;
};

// Finds every <time> element in a measure's <attributes> (there can be several: one unscoped plus
// per-staff <time number="N"> overrides) and translates each to a TimeChoice. The constructor does
// all of the work; query the results with getTimeSignatures().
class TimeReader
{
  public:
    explicit TimeReader(std::span<const core::MusicDataChoice> inMusicDataChoices);

    // Every translated <time>, in document order. Each TimeChoice has isImplicit == false.
    const std::vector<TimeReaderResult> &getTimeSignatures() const;

    // Translates one core::Time to one TimeChoice (all shapes: simple, composite, senza-misura,
    // interchangeable). isImplicit is left false; staff scoping is returned separately.
    static TimeReaderResult createTimeChoice(const core::Time &inTime);

  private:
    std::vector<TimeReaderResult> myTimeSignatures;
};
} // namespace impl
} // namespace mx
