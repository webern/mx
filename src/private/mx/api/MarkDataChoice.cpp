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

MarkDataChoice::Kind MarkDataChoice::kind() const
{
    return std::holds_alternative<TremoloMarkData>(myValue) ? Kind::tremolo : Kind::none;
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

bool MarkDataChoice::operator==(const MarkDataChoice &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
