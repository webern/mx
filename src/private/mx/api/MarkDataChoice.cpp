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

bool MarkDataChoice::operator==(const MarkDataChoice &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
