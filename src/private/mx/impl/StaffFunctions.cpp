// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/StaffFunctions.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/Forward.h"
#include "mx/core/generated/Harmony.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/Note.h"
#include "mx/utility/Throw.h"
#include "mx/utility/Unused.h"

#include <map>
#include <optional>

namespace mx
{
namespace impl
{

int findStaffCountInOneMeasure(std::span<const core::MusicDataChoice> inMusic);
int findStaffCountInAllMeasures(std::span<const core::PartwiseMeasure> measures);

void createStavesFromMx(int inGlobalTicksPerQuarter, std::span<const core::PartwiseMeasure> inMxMeasures,
                        std::vector<api::StaffData> &outStaves)
{
    MX_UNUSED(inGlobalTicksPerQuarter);
    MX_UNUSED(inMxMeasures);
    MX_UNUSED(outStaves);
}

int findStaffCountInOneMeasure(std::span<const core::MusicDataChoice> inMusic)
{
    int outStaffCount = 1;

    for (const auto &mdc : inMusic)
    {
        if (mdc.kind() == core::MusicDataChoice::Kind::attributes)
        {
            const auto &item = mdc.asAttributes();
            int tempStaffCount = 1;

            if (item.staves().has_value())
            {
                tempStaffCount = item.staves()->value();

                if (tempStaffCount > outStaffCount)
                {
                    outStaffCount = tempStaffCount;
                }
            }
        }

        std::optional<int> staff;

        if (mdc.kind() == core::MusicDataChoice::Kind::note)
        {
            const auto &item = mdc.asNote();
            if (item.staff().has_value())
            {
                staff = item.staff()->value();
            }
        }

        if (mdc.kind() == core::MusicDataChoice::Kind::direction)
        {
            const auto &item = mdc.asDirection();
            if (item.staff().has_value())
            {
                staff = item.staff()->value();
            }
        }

        if (mdc.kind() == core::MusicDataChoice::Kind::forward)
        {
            const auto &item = mdc.asForward();
            if (item.staff().has_value())
            {
                staff = item.staff()->value();
            }
        }

        if (mdc.kind() == core::MusicDataChoice::Kind::harmony)
        {
            const auto &item = mdc.asHarmony();
            if (item.staff().has_value())
            {
                staff = item.staff()->value();
            }
        }

        if (staff)
        {
            const int tempVal = *staff;
            if (tempVal > outStaffCount)
            {
                outStaffCount = tempVal;
            }
        }

        // TODO - look at all the notes and items for staff values
    }
    return outStaffCount;
}

int findStaffCountInAllMeasures(std::span<const core::PartwiseMeasure> measures)
{
    int outStaffCount = 1;

    for (const auto &measure : measures)
    {
        const int tempCount = findStaffCountInOneMeasure(measure.musicData());

        if (tempCount > outStaffCount)
        {
            outStaffCount = tempCount;
        }
    }

    return outStaffCount;
}

} // namespace impl
} // namespace mx
