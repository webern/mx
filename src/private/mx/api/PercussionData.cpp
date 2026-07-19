// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/PercussionData.h"

#include <utility>

namespace mx
{
namespace api
{

PercussionDataChoice::PercussionDataChoice() : myValue{GlassPercussion{}}
{
}

PercussionDataChoice::PercussionDataChoice(GlassPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(MetalPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(WoodPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(PitchedPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(MembranePercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(EffectPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(TimpaniPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(BeaterPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(StickPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(StickLocation value) : myValue{std::move(value)}
{
}

PercussionDataChoice::PercussionDataChoice(OtherPercussion value) : myValue{std::move(value)}
{
}

PercussionDataChoice::Kind PercussionDataChoice::kind() const
{
    return static_cast<Kind>(myValue.index());
}

bool PercussionDataChoice::isGlass() const
{
    return std::holds_alternative<GlassPercussion>(myValue);
}

bool PercussionDataChoice::isMetal() const
{
    return std::holds_alternative<MetalPercussion>(myValue);
}

bool PercussionDataChoice::isWood() const
{
    return std::holds_alternative<WoodPercussion>(myValue);
}

bool PercussionDataChoice::isPitched() const
{
    return std::holds_alternative<PitchedPercussion>(myValue);
}

bool PercussionDataChoice::isMembrane() const
{
    return std::holds_alternative<MembranePercussion>(myValue);
}

bool PercussionDataChoice::isEffect() const
{
    return std::holds_alternative<EffectPercussion>(myValue);
}

bool PercussionDataChoice::isTimpani() const
{
    return std::holds_alternative<TimpaniPercussion>(myValue);
}

bool PercussionDataChoice::isBeater() const
{
    return std::holds_alternative<BeaterPercussion>(myValue);
}

bool PercussionDataChoice::isStick() const
{
    return std::holds_alternative<StickPercussion>(myValue);
}

bool PercussionDataChoice::isStickLocation() const
{
    return std::holds_alternative<StickLocation>(myValue);
}

bool PercussionDataChoice::isOtherPercussion() const
{
    return std::holds_alternative<OtherPercussion>(myValue);
}

GlassPercussion PercussionDataChoice::glass() const
{
    if (const auto *value = std::get_if<GlassPercussion>(&myValue))
    {
        return *value;
    }
    return GlassPercussion{};
}

MetalPercussion PercussionDataChoice::metal() const
{
    if (const auto *value = std::get_if<MetalPercussion>(&myValue))
    {
        return *value;
    }
    return MetalPercussion{};
}

WoodPercussion PercussionDataChoice::wood() const
{
    if (const auto *value = std::get_if<WoodPercussion>(&myValue))
    {
        return *value;
    }
    return WoodPercussion{};
}

PitchedPercussion PercussionDataChoice::pitched() const
{
    if (const auto *value = std::get_if<PitchedPercussion>(&myValue))
    {
        return *value;
    }
    return PitchedPercussion{};
}

MembranePercussion PercussionDataChoice::membrane() const
{
    if (const auto *value = std::get_if<MembranePercussion>(&myValue))
    {
        return *value;
    }
    return MembranePercussion{};
}

EffectPercussion PercussionDataChoice::effect() const
{
    if (const auto *value = std::get_if<EffectPercussion>(&myValue))
    {
        return *value;
    }
    return EffectPercussion{};
}

TimpaniPercussion PercussionDataChoice::timpani() const
{
    if (const auto *value = std::get_if<TimpaniPercussion>(&myValue))
    {
        return *value;
    }
    return TimpaniPercussion{};
}

BeaterPercussion PercussionDataChoice::beater() const
{
    if (const auto *value = std::get_if<BeaterPercussion>(&myValue))
    {
        return *value;
    }
    return BeaterPercussion{};
}

StickPercussion PercussionDataChoice::stick() const
{
    if (const auto *value = std::get_if<StickPercussion>(&myValue))
    {
        return *value;
    }
    return StickPercussion{};
}

StickLocation PercussionDataChoice::stickLocation() const
{
    if (const auto *value = std::get_if<StickLocation>(&myValue))
    {
        return *value;
    }
    return StickLocation{};
}

OtherPercussion PercussionDataChoice::otherPercussion() const
{
    if (const auto *value = std::get_if<OtherPercussion>(&myValue))
    {
        return *value;
    }
    return OtherPercussion{};
}

bool PercussionDataChoice::operator==(const PercussionDataChoice &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
