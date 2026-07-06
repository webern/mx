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

ComplexTimeSignature ComplexTimeSignature::metered(MeteredTimeSignature value)
{
    ComplexTimeSignature result;
    result.myValue = std::move(value);
    return result;
}

ComplexTimeSignature ComplexTimeSignature::senzaMisura(std::string glyph)
{
    ComplexTimeSignature result;
    result.myValue = std::move(glyph);
    return result;
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
