// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/EncodingFunctions.h"
#include "mx/core/Attribution.h"
#include "mx/core/generated/EncodingChoice.h"
#include "mx/core/generated/Identification.h"
#include "mx/core/generated/Miscellaneous.h"
#include "mx/core/generated/MiscellaneousField.h"
#include "mx/core/generated/Supports.h"
#include "mx/core/generated/TypedText.h"
#include "mx/core/generated/YyyyMmDd.h"

namespace mx
{
namespace impl
{
void createEncoding(const api::EncodingData &inEncoding, core::ScoreHeaderGroup &header)
{
    // The old code mutated co-allocated identification/encoding through
    // shared pointers; under value semantics we build local copies and
    // install them at the end if anything was set.
    core::Identification identification = header.identification().value_or(core::Identification{});
    core::Encoding encoding = identification.encoding().value_or(core::Encoding{});
    bool hasIdentification = header.identification().has_value();
    bool hasEncoding = identification.encoding().has_value();

    if (!inEncoding.encoder.empty())
    {
        hasIdentification = true;
        hasEncoding = true;
        core::TypedText encoder;
        encoder.setValue(inEncoding.encoder);
        encoding.addChoice(core::EncodingChoice::encoder(std::move(encoder)));
    }

    core::YyyyMmDd tryDate{inEncoding.encodingDate.year, inEncoding.encodingDate.month, inEncoding.encodingDate.day};
    const bool isYearValid = inEncoding.encodingDate.year == tryDate.year();
    const bool isMonthValid = inEncoding.encodingDate.month == tryDate.month();
    const bool isDayValid = inEncoding.encodingDate.day == tryDate.day();
    if (isYearValid || isMonthValid || isDayValid)
    {
        hasIdentification = true;
        hasEncoding = true;
        encoding.addChoice(core::EncodingChoice::encodingDate(tryDate));
    }

    if (!inEncoding.encodingDescription.empty())
    {
        hasIdentification = true;
        hasEncoding = true;
        encoding.addChoice(core::EncodingChoice::encodingDescription(inEncoding.encodingDescription));
    }

    for (const auto &s : inEncoding.software)
    {
        hasIdentification = true;
        hasEncoding = true;
        encoding.addChoice(core::EncodingChoice::software(s));
    }

    for (const auto &s : inEncoding.supportedItems)
    {
        hasIdentification = true;
        hasEncoding = true;
        core::Supports supports;

        if (!s.elementName.empty())
        {
            supports.setElement(core::NameToken::parse(s.elementName));
        }

        if (!s.attributeName.empty())
        {
            supports.setAttribute(core::NameToken::parse(s.attributeName));
        }

        if (!s.specificValue.empty())
        {
            supports.setValue(s.specificValue);
        }

        supports.setType(s.isSupported ? core::YesNo::yes() : core::YesNo::no());
        encoding.addChoice(core::EncodingChoice::supports(std::move(supports)));
    }

    bool hasMiscellaneous = identification.miscellaneous().has_value();
    core::Miscellaneous miscellaneous = identification.miscellaneous().value_or(core::Miscellaneous{});
    for (const auto &m : inEncoding.miscelaneousFields)
    {
        hasIdentification = true;
        hasEncoding = true;
        hasMiscellaneous = true;
        core::MiscellaneousField item;
        item.setName(m.key);
        item.setValue(m.value);
        miscellaneous.addMiscellaneousField(std::move(item));
    }

    if (hasMiscellaneous)
    {
        identification.setMiscellaneous(std::move(miscellaneous));
    }
    if (hasEncoding)
    {
        identification.setEncoding(std::move(encoding));
    }
    if (hasIdentification)
    {
        header.setIdentification(std::move(identification));
    }
}

api::EncodingData createEncoding(const core::Encoding &inEncoding)
{
    api::EncodingData outEncoding;
    bool isDateFound = false;
    bool isEncoderFound = false;
    bool isDescriptionFound = false;
    for (const auto &ec : inEncoding.choice())
    {
        switch (ec.kind())
        {
        case core::EncodingChoice::Kind::encodingDate: {
            if (!isDateFound)
            {
                outEncoding.encodingDate.year = ec.asEncodingDate().year();
                outEncoding.encodingDate.month = ec.asEncodingDate().month();
                outEncoding.encodingDate.day = ec.asEncodingDate().day();
                isDateFound = true;
            }
            break;
        }
        case core::EncodingChoice::Kind::encoder: {
            if (!isEncoderFound)
            {
                outEncoding.encoder = ec.asEncoder().value();
                isEncoderFound = true;
            }
            break;
        }
        case core::EncodingChoice::Kind::encodingDescription: {
            if (!isDescriptionFound)
            {
                outEncoding.encodingDescription = ec.asEncodingDescription();
                isDescriptionFound = true;
            }
            break;
        }
        case core::EncodingChoice::Kind::software: {
            // Drop mx's own provenance stamp: it is mx attribution, not user
            // content, and the writer re-adds the current one on every write.
            if (!std::string_view{ec.asSoftware()}.starts_with(core::kMxSoftwareMarker))
            {
                outEncoding.software.emplace_back(ec.asSoftware());
            }
            break;
        }
        case core::EncodingChoice::Kind::supports: {
            const auto &supportsElement = ec.asSupports();
            api::SupportedItem item;
            item.elementName = supportsElement.element().value();
            if (supportsElement.attribute().has_value())
            {
                item.attributeName = supportsElement.attribute()->value();
            }
            item.isSupported = (supportsElement.type() == core::YesNo::yes());
            if (supportsElement.value().has_value())
            {
                item.specificValue = *supportsElement.value();
            }
            outEncoding.supportedItems.push_back(std::move(item));
            break;
        }
        }
    }
    return outEncoding;
}
} // namespace impl
} // namespace mx
