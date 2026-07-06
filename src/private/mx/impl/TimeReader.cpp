// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/TimeReader.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Time.h"
#include "mx/core/generated/TimeChoice.h"
#include "mx/core/generated/TimeChoiceGroup.h"
#include "mx/core/generated/TimeSignatureGroup.h"
#include "mx/utility/Throw.h"

#include <cmath>
#include <set>
#include <string>

namespace mx
{
namespace impl
{

TimeReader::TimeReader(std::span<const core::MusicDataChoice> inMusicDataChoices)
    : myMusicDataChoiceSet{inMusicDataChoices}, myIsTimeFound{false}, myTimeSignatures{}
{
    myIsTimeFound = initialize();
}

bool TimeReader::getIsTimeFound() const
{
    return myIsTimeFound;
}

std::vector<mx::api::TimeSignatureData> TimeReader::getTimeSignatures() const
{
    return myTimeSignatures;
}

bool TimeReader::initialize()
{
    for (const auto &mdc : myMusicDataChoiceSet)
    {
        if (mdc.kind() == core::MusicDataChoice::Kind::attributes)
        {
            const auto &props = mdc.asAttributes();
            if (props.time().size() > 0)
            {
                bool anyParsed = false;
                for (const auto &time : props.time())
                {
                    if (parseTime(time))
                    {
                        anyParsed = true;
                    }
                }
                return anyParsed;
            }
        }
    }
    return false;
}

bool TimeReader::parseTime(const core::Time &time)
{
    const auto &timeChoice = time.choice();

    if (timeChoice.kind() == core::TimeChoice::Kind::group)
    {
        const auto sigGroupSet = timeChoice.asGroup().timeSignature();
        MX_ASSERT(sigGroupSet.size() > 0);

        // all TimeSignatureGroup entries in this <time> element share the same symbol/display/
        // staffIndex (those live on the <time> element itself); each contributes one component
        // to a single TimeSignatureData, so a composite time signature round-trips as one
        // TimeSignatureData with multiple components.
        api::TimeSignatureData timeSignatureData;
        bool isFirst = true;
        for (const auto &sigGroup : sigGroupSet)
        {
            if (isFirst)
            {
                parseTimeSignatureGroup(time, sigGroup, timeSignatureData);
                isFirst = false;
            }
            else
            {
                timeSignatureData.components.push_back(
                    api::TimeSignatureComponent{sigGroup.beats(), sigGroup.beatType()});
            }
        }
        myTimeSignatures.push_back(std::move(timeSignatureData));
        return true;
    }
    else
    {
        return false;
        // MX_THROW( "TODO - other time signature stuff" );
    }
}

bool TimeReader::parseTimeSignatureGroup(const core::Time &time, const core::TimeSignatureGroup &timeSig,
                                         mx::api::TimeSignatureData &outData)
{
    outData.setSimple(timeSig.beats(), timeSig.beatType());

    if (time.symbol().has_value())
    {
        if (*time.symbol() == core::TimeSymbol::common())
        {
            outData.symbol = api::TimeSignatureSymbol::common;
        }
        else if (*time.symbol() == core::TimeSymbol::cut())
        {
            outData.symbol = api::TimeSignatureSymbol::cut;
        }
        else if (*time.symbol() == core::TimeSymbol::singleNumber())
        {
            outData.symbol = api::TimeSignatureSymbol::singleNumber;
        }
    }
    else
    {
        outData.symbol = api::TimeSignatureSymbol::unspecified;
    }

    outData.display = api::Bool::unspecified;
    if (time.printObject().has_value())
    {
        bool isPrint = *time.printObject() == core::YesNo::yes();

        if (isPrint)
        {
            outData.display = api::Bool::yes;
        }
        else
        {
            outData.display = api::Bool::no;
        }
    }

    if (time.number().has_value())
    {
        outData.staffIndex = time.number()->value() - 1;
    }
    else
    {
        outData.staffIndex = api::INDEX_UNSPECIFIED;
    }

    outData.isImplicit = false;

    return true;
}
} // namespace impl
} // namespace mx
