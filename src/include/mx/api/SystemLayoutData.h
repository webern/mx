// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/LeftRight.h"
#include "mx/api/PageLayoutData.h"

#include <string>
#include <vector>

namespace mx
{
namespace api
{
/// A <staff-layout> scoped to one staff of a multi-staff part (MusicXML's number attribute):
/// the space between the bottom line of the previous staff and the top line of this one.
/// staffIndex is zero-based; a source with an explicit number attribute always populates
/// SystemLayoutData::staffDistances with one of these rather than the unscoped
/// SystemLayoutData::staffDistance, so the number round-trips faithfully.
class StaffDistanceData
{
  public:
    int staffIndex;
    double staffDistance;

    explicit inline StaffDistanceData(int inStaffIndex = INDEX_UNSPECIFIED, double inStaffDistance = DOUBLE_UNSPECIFIED)
        : staffIndex(inStaffIndex), staffDistance(inStaffDistance)
    {
    }
};

MXAPI_EQUALS_BEGIN(StaffDistanceData)
MXAPI_EQUALS_MEMBER(staffIndex)
MXAPI_DOUBLES_EQUALS_MEMBER(staffDistance)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(StaffDistanceData);

/// Margins and spacing for staff systems.
class SystemLayoutData
{
  public:
    /// The left and right margins for the system, in tenths.
    std::optional<LeftRight> margins;

    /// Distance from bottom line of one system to top line of next system, in tenths.
    OptionalDouble systemDistance;

    /// Distance from the top margin of the page to the top line of the first staff on the page, in tenths.
    OptionalDouble topSystemDistance;

    /// the space between staves within the same system, in tenths. Only populated when the source's
    /// <staff-layout> had no number attribute (the common single-staff-distance case); a source with
    /// an explicit number populates staffDistances instead. See StaffDistanceData.
    OptionalDouble staffDistance;

    /// Per-staff overrides of staffDistance for parts with more than one staff-scoped <staff-layout>
    /// (e.g. an organ's third staff needing its own spacing). Normally empty.
    std::vector<StaffDistanceData> staffDistances;

    /// Returns true if any of the members have values.
    inline bool isUsed() const
    {
        return margins || systemDistance || topSystemDistance || staffDistance || !staffDistances.empty();
    }

    explicit inline SystemLayoutData(std::optional<LeftRight> inMargins = std::nullopt,
                                     OptionalDouble inSystemDistance = std::nullopt,
                                     OptionalDouble inTopSystemDistance = std::nullopt,
                                     OptionalDouble inStaffDistance = std::nullopt)
        : margins(inMargins), systemDistance(inSystemDistance), topSystemDistance{inTopSystemDistance},
          staffDistance(inStaffDistance), staffDistances{}
    {
    }
};

MXAPI_EQUALS_BEGIN(SystemLayoutData)
MXAPI_EQUALS_MEMBER(margins)
MXAPI_EQUALS_MEMBER(systemDistance)
MXAPI_EQUALS_MEMBER(topSystemDistance)
MXAPI_EQUALS_MEMBER(staffDistance)
MXAPI_EQUALS_MEMBER(staffDistances)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SystemLayoutData);
} // namespace api
} // namespace mx
