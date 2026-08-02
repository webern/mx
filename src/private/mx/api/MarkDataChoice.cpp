// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/MarkDataChoice.h"

#include <utility>

namespace mx
{
namespace api
{

MarkDataChoice::MarkDataChoice() : myValue{std::monostate{}}
{
}

MarkDataChoice::MarkDataChoice(TremoloMarkData value) : myValue{std::move(value)}
{
}

MarkDataChoice::MarkDataChoice(ArpeggiateMarkData value) : myValue{std::move(value)}
{
}

MarkDataChoice::MarkDataChoice(NonArpeggiateMarkData value) : myValue{std::move(value)}
{
}

MarkDataChoice::MarkDataChoice(OtherMarkData value) : myValue{std::move(value)}
{
}

MarkDataChoice::MarkDataChoice(CompoundDynamicsData value) : myValue{std::move(value)}
{
}

MarkDataChoice::MarkDataChoice(OtherNotationMarkData value) : myValue{std::move(value)}
{
}

MarkDataChoice::Kind MarkDataChoice::kind() const
{
    if (std::holds_alternative<TremoloMarkData>(myValue))
    {
        return Kind::tremolo;
    }
    if (std::holds_alternative<ArpeggiateMarkData>(myValue))
    {
        return Kind::arpeggiate;
    }
    if (std::holds_alternative<NonArpeggiateMarkData>(myValue))
    {
        return Kind::nonArpeggiate;
    }
    if (std::holds_alternative<OtherMarkData>(myValue))
    {
        return Kind::otherMark;
    }
    if (std::holds_alternative<CompoundDynamicsData>(myValue))
    {
        return Kind::compoundDynamics;
    }
    if (std::holds_alternative<OtherNotationMarkData>(myValue))
    {
        return Kind::otherNotation;
    }
    return Kind::none;
}

bool MarkDataChoice::isNone() const
{
    return std::holds_alternative<std::monostate>(myValue);
}

bool MarkDataChoice::isTremolo() const
{
    return std::holds_alternative<TremoloMarkData>(myValue);
}

const TremoloMarkData MarkDataChoice::tremolo() const
{
    if (const auto *value = std::get_if<TremoloMarkData>(&myValue))
    {
        return *value;
    }
    return TremoloMarkData{};
}

bool MarkDataChoice::isArpeggiate() const
{
    return std::holds_alternative<ArpeggiateMarkData>(myValue);
}

const ArpeggiateMarkData MarkDataChoice::arpeggiate() const
{
    if (const auto *value = std::get_if<ArpeggiateMarkData>(&myValue))
    {
        return *value;
    }
    return ArpeggiateMarkData{};
}

bool MarkDataChoice::isNonArpeggiate() const
{
    return std::holds_alternative<NonArpeggiateMarkData>(myValue);
}

const NonArpeggiateMarkData MarkDataChoice::nonArpeggiate() const
{
    if (const auto *value = std::get_if<NonArpeggiateMarkData>(&myValue))
    {
        return *value;
    }
    return NonArpeggiateMarkData{};
}

bool MarkDataChoice::isOtherMark() const
{
    return std::holds_alternative<OtherMarkData>(myValue);
}

const OtherMarkData MarkDataChoice::otherMark() const
{
    if (const auto *value = std::get_if<OtherMarkData>(&myValue))
    {
        return *value;
    }
    return OtherMarkData{};
}

bool MarkDataChoice::isCompoundDynamics() const
{
    return std::holds_alternative<CompoundDynamicsData>(myValue);
}

const CompoundDynamicsData MarkDataChoice::compoundDynamics() const
{
    if (const auto *value = std::get_if<CompoundDynamicsData>(&myValue))
    {
        return *value;
    }
    return CompoundDynamicsData{};
}

bool MarkDataChoice::isOtherNotation() const
{
    return std::holds_alternative<OtherNotationMarkData>(myValue);
}

const OtherNotationMarkData MarkDataChoice::otherNotation() const
{
    if (const auto *value = std::get_if<OtherNotationMarkData>(&myValue))
    {
        return *value;
    }
    return OtherNotationMarkData{};
}

bool MarkDataChoice::operator==(const MarkDataChoice &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
