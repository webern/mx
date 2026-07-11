// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/ComplexTimeSignature.h"

#include <cassert>
#include <utility>

namespace mx
{
namespace api
{

ComplexTimeSignature::ComplexTimeSignature() : myValue{MeteredTimeSignature{}}
{
}

ComplexTimeSignature::ComplexTimeSignature(MeteredTimeSignature value) : myValue{std::move(value)}
{
}

ComplexTimeSignature::ComplexTimeSignature(std::string glyph) : myValue{std::move(glyph)}
{
}

ComplexTimeSignature::Kind ComplexTimeSignature::kind() const
{
    return std::holds_alternative<MeteredTimeSignature>(myValue) ? Kind::metered : Kind::senzaMisura;
}

bool ComplexTimeSignature::isMetered() const
{
    return std::holds_alternative<MeteredTimeSignature>(myValue);
}

bool ComplexTimeSignature::isSenzaMisura() const
{
    return std::holds_alternative<std::string>(myValue);
}

const MeteredTimeSignature &ComplexTimeSignature::asMetered() const
{
    assert(isMetered());
    return *std::get_if<MeteredTimeSignature>(&myValue);
}

const std::string &ComplexTimeSignature::asSenzaMisura() const
{
    assert(isSenzaMisura());
    return *std::get_if<std::string>(&myValue);
}

bool ComplexTimeSignature::operator==(const ComplexTimeSignature &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
