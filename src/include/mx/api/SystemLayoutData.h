// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/LeftRight.h"
#include "mx/api/PageLayoutData.h"

#include <map>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
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

    /// The space between staves within the same system, in tenths. Applies to every staff of a part
    /// that does not have its own entry in staffDistances.
    OptionalDouble staffDistance;

    /// Per-staff spacing for multi-staff parts, keyed by zero-based staff index: the space between
    /// the bottom line of the previous staff and the top line of the keyed staff, in tenths.
    /// Normally empty; only add an entry when one staff needs its own spacing, e.g. extra room
    /// above an organ's pedal staff. For a given staff, an entry here wins; otherwise
    /// staffDistance applies.
    std::map<int, double> staffDistances;

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
