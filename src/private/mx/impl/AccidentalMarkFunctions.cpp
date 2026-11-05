// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/core/generated/AccidentalMark.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{

AccidentalMarkFunctions::AccidentalMarkFunctions(const core::AccidentalMark &inAccidentalMark, impl::Cursor inCursor)
    : myAccidentalMark{inAccidentalMark}, myCursor{inCursor}
{
}

api::MarkData AccidentalMarkFunctions::parseAccidentalMark() const
{
    const auto accidentalValue = myAccidentalMark.value();
    Converter converter;
    const auto markType = converter.convertAccidentalMark(accidentalValue);
    auto markData = api::MarkData{};
    markData.markType = markType;
    markData.tickTimePosition = myCursor.tickTimePosition;

    markData.name = std::string{accidentalValue.toString()};
    markData.positionData = impl::getPositionData(myAccidentalMark);

    return markData;
}

api::OrnamentAccidental AccidentalMarkFunctions::parseOrnamentAccidental() const
{
    Converter converter;
    api::OrnamentAccidental accidental;
    accidental.accidental = converter.convert(myAccidentalMark.value());
    accidental.positionData = impl::getPositionData(myAccidentalMark);
    return accidental;
}
} // namespace impl
} // namespace mx
