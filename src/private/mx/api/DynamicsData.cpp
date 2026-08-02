// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/DynamicsData.h"

#include <utility>

namespace mx
{
namespace api
{

DynamicsComponent::DynamicsComponent() : myValue{StandardDynamic::p}
{
}

DynamicsComponent::DynamicsComponent(StandardDynamic value) : myValue{value}
{
}

DynamicsComponent::DynamicsComponent(OtherDynamicsData value) : myValue{std::move(value)}
{
}

DynamicsComponent::Kind DynamicsComponent::kind() const
{
    return isOther() ? Kind::other : Kind::standard;
}

bool DynamicsComponent::isStandard() const
{
    return std::holds_alternative<StandardDynamic>(myValue);
}

bool DynamicsComponent::isOther() const
{
    return std::holds_alternative<OtherDynamicsData>(myValue);
}

StandardDynamic DynamicsComponent::standard() const
{
    if (const auto *value = std::get_if<StandardDynamic>(&myValue))
    {
        return *value;
    }
    return StandardDynamic::p;
}

OtherDynamicsData DynamicsComponent::other() const
{
    if (const auto *value = std::get_if<OtherDynamicsData>(&myValue))
    {
        return *value;
    }
    return OtherDynamicsData{};
}

bool DynamicsComponent::operator==(const DynamicsComponent &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
