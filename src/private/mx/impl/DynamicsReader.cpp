// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DynamicsReader.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/DynamicsChoice.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{

DynamicsReader::DynamicsReader(const core::Dynamics &inDynamic, impl::Cursor inCursor)
    : myDynamic{inDynamic}, myCursor{inCursor}
{
}

void DynamicsReader::parseDynamics(std::vector<api::MarkData> &outMarks) const
{
    const auto &choices = myDynamic.choice();
    if (choices.empty())
    {
        return;
    }

    Converter converter;
    auto markData = api::MarkData{};
    markData.tickTimePosition = myCursor.tickTimePosition;
    markData.positionData = impl::getPositionData(myDynamic);
    markData.printData = impl::getPrintData(myDynamic);

    if (choices.size() == 1)
    {
        const auto &choice = choices.front();
        const auto kind = choice.kind();
        markData.markType = converter.convertDynamic(kind);

        if (kind == core::DynamicsChoice::Kind::otherDynamics)
        {
            const auto &other = choice.asOtherDynamics();
            markData.name = other.value();
            api::OtherMarkData payload;
            if (other.smufl().has_value())
            {
                payload.smufl = other.smufl()->toString();
            }
            markData.choice = std::move(payload);
        }
        else
        {
            markData.name = dynamicsKindToName(kind);
        }
    }
    else
    {
        markData.markType = api::MarkType::compoundDynamics;
        api::CompoundDynamicsData compound;
        compound.components.reserve(choices.size());
        for (const auto &choice : choices)
        {
            if (choice.kind() == core::DynamicsChoice::Kind::otherDynamics)
            {
                const auto &other = choice.asOtherDynamics();
                api::OtherDynamicsData component;
                component.text = other.value();
                if (other.smufl().has_value())
                {
                    component.smufl = other.smufl()->toString();
                }
                compound.components.emplace_back(std::move(component));
            }
            else
            {
                compound.components.emplace_back(converter.convertStandardDynamic(choice.kind()));
            }
        }
        markData.choice = std::move(compound);
    }

    outMarks.emplace_back(std::move(markData));
}
} // namespace impl
} // namespace mx
