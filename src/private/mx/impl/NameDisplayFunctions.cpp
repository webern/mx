// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NameDisplayFunctions.h"

#include "mx/core/generated/AccidentalValue.h"
#include "mx/core/generated/FormattedText.h"
#include "mx/core/generated/NameDisplay.h"
#include "mx/core/generated/NameDisplayChoice.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"

#include <sstream>

namespace mx
{
namespace impl
{
std::string extractDisplayText(const core::NameDisplay &nameDisplay)
{
    std::stringstream ss;
    for (const auto &c : nameDisplay.choice())
    {
        if (c.isDisplayText())
        {
            ss << c.asDisplayText().value();
        }
        else if (c.isAccidentalText())
        {
            if (c.asAccidentalText().value().tag() == core::AccidentalValue::Tag::flat)
            {
                ss << "b"; // TODO - support accidental text correctly
            }
            else if (c.asAccidentalText().value().tag() == core::AccidentalValue::Tag::sharp)
            {
                ss << "#";
            }
        }
    }
    return ss.str();
}

void extractDisplayFormatting(const core::NameDisplay &nameDisplay, api::PrintData &outPrintData,
                              api::PositionData &outPositionData)
{
    for (const auto &c : nameDisplay.choice())
    {
        if (c.isDisplayText())
        {
            const auto &ft = c.asDisplayText();
            outPrintData = getPrintData(ft);
            outPositionData = getPositionData(ft);
            return; // the api keeps one run's formatting; the first run wins
        }
    }
}

core::NameDisplay makeNameDisplay(const std::string &text)
{
    return makeNameDisplay(text, api::PrintData{}, api::PositionData{});
}

core::NameDisplay makeNameDisplay(const std::string &text, const api::PrintData &printData,
                                  const api::PositionData &positionData)
{
    core::NameDisplay nameDisplay{};
    core::FormattedText ft{};
    ft.setValue(text);
    // Emit font/color and the print-style position attributes onto the
    // <display-text> run -- the modern, non-deprecated home for this
    // formatting. (print-object is not a <display-text> attribute, so the
    // PrintData::printObject field is intentionally not applied here.)
    setAttributesFromPrintData(printData, ft);
    setAttributesFromPositionData(positionData, ft);
    nameDisplay.addChoice(core::NameDisplayChoice::displayText(ft));
    return nameDisplay;
}
} // namespace impl
} // namespace mx
