// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/core/generated/AccidentalMark.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{

// Reads how an accidental-mark is drawn into either api shape that holds it: a standalone mark's
// AccidentalMarkData, whose position and print data live on the MarkData, or an OrnamentAccidental,
// which has the same fields itself.
template <typename ACCIDENTAL_FIELDS>
void accidentalMarkFunctionsParseAttributes(const core::AccidentalMark &inAccidentalMark, ACCIDENTAL_FIELDS &outFields,
                                            api::PositionData &outPositionData, api::PrintData &outPrintData)
{
    Converter converter;
    outPositionData = impl::getPositionData(inAccidentalMark);
    outPrintData = impl::getPrintData(inAccidentalMark);
    if (inAccidentalMark.parentheses().has_value())
    {
        outFields.parentheses = converter.convert(*inAccidentalMark.parentheses());
    }
    if (inAccidentalMark.bracket().has_value())
    {
        outFields.bracket = converter.convert(*inAccidentalMark.bracket());
    }
    if (inAccidentalMark.size().has_value())
    {
        outFields.size = converter.convert(*inAccidentalMark.size());
    }
    if (inAccidentalMark.smufl().has_value())
    {
        outFields.smufl = inAccidentalMark.smufl()->toString();
    }
    outFields.id = impl::getId(inAccidentalMark);
}

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
    api::AccidentalMarkData payload;
    accidentalMarkFunctionsParseAttributes(myAccidentalMark, payload, markData.positionData, markData.printData);
    markData.choice = payload;

    return markData;
}

api::OrnamentAccidental AccidentalMarkFunctions::parseOrnamentAccidental() const
{
    Converter converter;
    api::OrnamentAccidental accidental;
    accidental.accidental = converter.convert(myAccidentalMark.value());
    accidentalMarkFunctionsParseAttributes(myAccidentalMark, accidental, accidental.positionData, accidental.printData);
    return accidental;
}
} // namespace impl
} // namespace mx
