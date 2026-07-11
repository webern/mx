// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/TimeReader.h"
#include "mx/api/ComplexTimeSignature.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Interchangeable.h"
#include "mx/core/generated/Time.h"
#include "mx/core/generated/TimeChoice.h"
#include "mx/core/generated/TimeChoiceGroup.h"
#include "mx/core/generated/TimeSignatureGroup.h"
#include "mx/impl/Converter.h"

#include <utility>
#include <vector>

namespace mx
{
namespace impl
{

static std::vector<api::TimeFraction> timeReaderFractions(std::span<const core::TimeSignatureGroup> inGroups)
{
    std::vector<api::TimeFraction> fractions;
    for (const auto &pair : inGroups)
    {
        fractions.push_back(api::TimeFraction{pair.beats(), pair.beatType()});
    }
    return fractions;
}

TimeReader::TimeReader(std::span<const core::MusicDataChoice> inMusicDataChoices) : myTimeSignatures{}
{
    for (const auto &mdc : inMusicDataChoices)
    {
        if (mdc.kind() == core::MusicDataChoice::Kind::attributes)
        {
            for (const auto &time : mdc.asAttributes().time())
            {
                myTimeSignatures.push_back(createTimeChoice(time));
            }
        }
    }
}

const std::vector<TimeReaderResult> &TimeReader::getTimeSignatures() const
{
    return myTimeSignatures;
}

TimeReaderResult TimeReader::createTimeChoice(const core::Time &inTime)
{
    Converter converter;
    api::TimeChoice timeChoice;

    const auto &choice = inTime.choice();
    if (choice.kind() == core::TimeChoice::Kind::senzaMisura)
    {
        // the string content is the display glyph (often empty); a senza-misura carries no symbol
        timeChoice = api::TimeChoice(api::ComplexTimeSignature(choice.asSenzaMisura()));
    }
    else
    {
        const auto &group = choice.asGroup();
        api::MeteredTimeSignature metered;
        metered.fractions = timeReaderFractions(group.timeSignature());

        if (inTime.symbol().has_value())
        {
            metered.symbol = converter.convert(*inTime.symbol());
        }
        if (inTime.separator().has_value())
        {
            metered.separator = converter.convert(*inTime.separator());
        }

        if (group.interchangeable().has_value())
        {
            const auto &core = *group.interchangeable();
            api::InterchangeableTimeSignature alternate;
            alternate.fractions = timeReaderFractions(core.timeSignature());
            if (core.timeRelation().has_value())
            {
                alternate.relation = converter.convert(*core.timeRelation());
            }
            if (core.symbol().has_value())
            {
                alternate.symbol = converter.convert(*core.symbol());
            }
            if (core.separator().has_value())
            {
                alternate.separator = converter.convert(*core.separator());
            }
            metered.interchangeable = std::move(alternate);
        }

        // complex() collapses back to simple when the meter is really just a plain fraction
        timeChoice = api::TimeChoice(api::ComplexTimeSignature(std::move(metered)));
    }

    timeChoice.isImplicit = false;
    if (inTime.printObject().has_value())
    {
        timeChoice.display = converter.convert(*inTime.printObject());
    }

    int staffIndex = api::INDEX_UNSPECIFIED;
    if (inTime.number().has_value())
    {
        staffIndex = static_cast<int>(inTime.number()->value()) - 1;
    }

    return TimeReaderResult{std::move(timeChoice), staffIndex};
}
} // namespace impl
} // namespace mx
