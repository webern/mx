// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PageTextFunctions.h"
#include "mx/api/ScoreData.h"
#include "mx/core/Decimal.h"
#include "mx/core/generated/Credit.h"
#include "mx/core/generated/CreditChoice.h"
#include "mx/core/generated/CreditChoiceGroup.h"
#include "mx/core/generated/CreditChoiceGroupChoice.h"
#include "mx/core/generated/FormattedTextID.h"
#include "mx/core/generated/Image.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/core/generated/Tenths.h"
#include "mx/impl/FontFunctions.h"
#include "mx/impl/PositionFunctions.h"

namespace mx
{
namespace impl
{
namespace
{
api::PageImageData getImageData(const core::Image &image, int pageNumber)
{
    api::PageImageData out{};
    out.source = image.source();
    out.type = image.type();
    out.pageNumber = pageNumber;

    if (image.height().has_value())
    {
        out.height = image.height()->value().value();
    }

    if (image.width().has_value())
    {
        out.width = image.width()->value().value();
    }

    out.positionData = getPositionData(image);
    // The <credit-image> valign uses the credit-image-specific
    // ValignImage vocabulary, which PositionData does not model. Avoid
    // recording a misleading vertical-alignment value.
    out.positionData.verticalAlignment = api::VerticalAlignment::unspecified;
    return out;
}

core::Image makeCoreImage(const api::PageImageData &in)
{
    core::Image image{};
    image.setSource(in.source);
    image.setType(in.type);

    if (in.height >= 0.0)
    {
        image.setHeight(core::Tenths{core::Decimal{in.height}});
    }

    if (in.width >= 0.0)
    {
        image.setWidth(core::Tenths{core::Decimal{in.width}});
    }

    setAttributesFromPositionData(in.positionData, image);
    return image;
}
} // namespace

void createCredits(const api::ScoreData &inScoreData, core::ScoreHeaderGroup &outHeader)
{
    for (const auto &p : inScoreData.pageTextItems)
    {
        core::Credit credit;

        // The <credit> content model requires a credit-words/credit-symbol
        // (or credit-image). Always emit a <credit-words> here; an empty
        // string preserves a metadata-only credit (one that carried only
        // credit-type) without dropping it.
        {
            core::FormattedTextID words;
            words.setValue(p.text);
            impl::setAttributesFromFontData(p.fontData, words);
            impl::setAttributesFromPositionData(p.positionData, words);

            core::CreditChoiceGroupChoice groupChoice = core::CreditChoiceGroupChoice::creditWords(words);
            core::CreditChoiceGroup group;
            group.setChoice(groupChoice);
            credit.setChoice(core::CreditChoice::group(group));
        }

        // Emit every credit-type. Prefer the full list; fall back to the
        // legacy single `description` for callers that only set that.
        if (!p.creditTypes.empty())
        {
            for (const auto &ct : p.creditTypes)
            {
                credit.addCreditType(ct);
            }
        }
        else if (!p.description.empty())
        {
            credit.addCreditType(p.description);
        }

        if (p.pageNumber > 0)
        {
            credit.setPage(p.pageNumber);
        }

        outHeader.addCredit(credit);
    }

    for (const auto &img : inScoreData.pageImageItems)
    {
        core::Credit credit;
        credit.setChoice(core::CreditChoice::creditImage(makeCoreImage(img)));

        if (img.pageNumber > 0)
        {
            credit.setPage(img.pageNumber);
        }

        outHeader.addCredit(credit);
    }
}

void createCredits(const core::ScoreHeaderGroup &inHeader, api::ScoreData &outScoreData)
{
    for (const auto &c : inHeader.credit())
    {
        const int pageNumber = c.page().has_value() ? *c.page() : 0;

        if (c.choice().isCreditImage())
        {
            outScoreData.pageImageItems.push_back(getImageData(c.choice().asCreditImage(), pageNumber));
            continue;
        }

        // Otherwise this is a credit group. Capture its text (if it begins
        // with <credit-words>) and its credit-type(s). A credit with no
        // <credit-words> still survives as a metadata-only PageTextData.
        api::PageTextData pageText{};
        pageText.pageNumber = pageNumber;

        const auto &group = c.choice().asGroup();
        if (group.choice().isCreditWords())
        {
            const auto &words = group.choice().asCreditWords();
            pageText.text = words.value();
            pageText.positionData = impl::getPositionData(words);
            pageText.fontData = impl::getFontData(words);
        }

        for (const auto &ct : c.creditType())
        {
            pageText.creditTypes.emplace_back(ct);
        }

        if (!pageText.creditTypes.empty())
        {
            pageText.description = pageText.creditTypes.front();
        }

        outScoreData.pageTextItems.push_back(std::move(pageText));
    }
}
} // namespace impl
} // namespace mx
