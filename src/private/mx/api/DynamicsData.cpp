// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/DynamicsData.h"

#include <utility>

namespace mx
{
namespace api
{

std::string toString(StandardDynamic value)
{
    switch (value)
    {
    case StandardDynamic::p:
        return "p";
    case StandardDynamic::pp:
        return "pp";
    case StandardDynamic::ppp:
        return "ppp";
    case StandardDynamic::pppp:
        return "pppp";
    case StandardDynamic::ppppp:
        return "ppppp";
    case StandardDynamic::pppppp:
        return "pppppp";
    case StandardDynamic::f:
        return "f";
    case StandardDynamic::ff:
        return "ff";
    case StandardDynamic::fff:
        return "fff";
    case StandardDynamic::ffff:
        return "ffff";
    case StandardDynamic::fffff:
        return "fffff";
    case StandardDynamic::ffffff:
        return "ffffff";
    case StandardDynamic::mp:
        return "mp";
    case StandardDynamic::mf:
        return "mf";
    case StandardDynamic::sf:
        return "sf";
    case StandardDynamic::sfp:
        return "sfp";
    case StandardDynamic::sfpp:
        return "sfpp";
    case StandardDynamic::fp:
        return "fp";
    case StandardDynamic::rf:
        return "rf";
    case StandardDynamic::rfz:
        return "rfz";
    case StandardDynamic::sfz:
        return "sfz";
    case StandardDynamic::sffz:
        return "sffz";
    case StandardDynamic::fz:
        return "fz";
    case StandardDynamic::n:
        return "n";
    case StandardDynamic::pf:
        return "pf";
    case StandardDynamic::sfzp:
        return "sfzp";
    }
    return "p";
}

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

std::string toString(const DynamicsComponent &value)
{
    return value.isOther() ? value.other().text : toString(value.standard());
}

std::string toString(const CompoundDynamicsData &value)
{
    std::string result;
    for (const auto &component : value.components)
    {
        result += toString(component);
    }
    return result;
}

} // namespace api
} // namespace mx
