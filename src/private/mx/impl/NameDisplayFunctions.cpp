// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NameDisplayFunctions.h"

#include "mx/core/generated/AccidentalValue.h"
#include "mx/core/generated/FormattedText.h"
#include "mx/core/generated/NameDisplay.h"
#include "mx/core/generated/NameDisplayChoice.h"

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

core::NameDisplay makeNameDisplay(const std::string &text)
{
    core::NameDisplay nameDisplay{};
    core::FormattedText ft{};
    ft.setValue(text);
    nameDisplay.addChoice(core::NameDisplayChoice::displayText(ft));
    return nameDisplay;
}
} // namespace impl
} // namespace mx
