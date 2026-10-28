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
#include "mx/core/generated/CreditChoiceGroupGroup.h"
#include "mx/core/generated/CreditChoiceGroupGroupChoice.h"
#include "mx/core/generated/FormattedTextID.h"
#include "mx/core/generated/Image.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/core/generated/Tenths.h"
#include "mx/impl/Converter.h"
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
    // <credit-image> reads valign-image, not valign; see PositionFunctions.h.
    out.positionData.verticalAlignment = getImageValign(image);
    return out;
}

core::Image makeCoreImage(const api::PageImageData &in, const DiagnosticsContext &diagnostics)
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
    // <credit-image> writes valign-image, not valign; see PositionFunctions.h.
    setImageValignFromVerticalAlignment(in.positionData.verticalAlignment, image);
    if (in.positionData.verticalAlignment == api::VerticalAlignment::baseline)
    {
        diagnostics.report(api::Severity::error, api::DiagnosticCode::droppedData, api::Location{},
                           "a credit-image valign of baseline is not written; an image has no baseline alignment");
    }
    return image;
}
} // namespace

// The first run of a credit and each run after it are written and read the same way, so they share
// these two helpers.
static core::FormattedTextID pageTextWordsFromSegment(const std::string &text, const api::FontData &fontData,
                                                      const api::PositionData &positionData,
                                                      api::HorizontalAlignment justify, api::Enclosure enclosure)
{
    core::FormattedTextID words;
    words.setValue(text);
    setAttributesFromFontData(fontData, words);
    setAttributesFromPositionData(positionData, words);

    const Converter converter;
    if (justify != api::HorizontalAlignment::unspecified)
    {
        words.setJustify(converter.convert(justify));
    }

    if (enclosure != api::Enclosure::unspecified)
    {
        words.setEnclosure(converter.convert(enclosure));
    }

    return words;
}

static api::PageTextSegment pageTextSegmentFromWords(const core::FormattedTextID &words)
{
    api::PageTextSegment segment{};
    segment.text = words.value();
    segment.positionData = getPositionData(words);
    segment.fontData = getFontData(words);

    const Converter converter;
    if (words.justify().has_value())
    {
        segment.justify = converter.convert(*words.justify());
    }

    if (words.enclosure().has_value())
    {
        segment.enclosure = converter.convert(*words.enclosure());
    }

    return segment;
}

void createCredits(const api::ScoreData &inScoreData, core::ScoreHeaderGroup &outHeader,
                   const DiagnosticsContext &diagnostics)
{
    for (const auto &p : inScoreData.pageTextItems)
    {
        core::Credit credit;

        // The <credit> content model requires a credit-words/credit-symbol
        // (or credit-image). Always emit a <credit-words> here; an empty
        // string preserves a metadata-only credit (one that carried only
        // credit-type) without dropping it. Any further runs of text follow
        // as more <credit-words>.
        {
            core::CreditChoiceGroup group;
            group.setChoice(core::CreditChoiceGroupChoice::creditWords(
                pageTextWordsFromSegment(p.text, p.fontData, p.positionData, p.justify, p.enclosure)));

            for (const auto &segment : p.continuations)
            {
                core::CreditChoiceGroupGroup next;
                next.setChoice(core::CreditChoiceGroupGroupChoice::creditWords(pageTextWordsFromSegment(
                    segment.text, segment.fontData, segment.positionData, segment.justify, segment.enclosure)));
                group.addGroup(std::move(next));
            }

            credit.setChoice(core::CreditChoice::group(std::move(group)));
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
        credit.setChoice(core::CreditChoice::creditImage(makeCoreImage(img, diagnostics)));

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

        // Otherwise this is a credit group. Capture its text and its
        // credit-type(s). A credit with no <credit-words> still survives as a
        // metadata-only PageTextData.
        api::PageTextData pageText{};
        pageText.pageNumber = pageNumber;

        // Every <credit-words> of the credit, in order. A credit-symbol, link or bookmark is skipped, so
        // the first run of text fills the PageTextData's own fields even when a symbol comes before it.
        std::vector<api::PageTextSegment> runs;
        const auto &group = c.choice().asGroup();
        if (group.choice().isCreditWords())
        {
            runs.push_back(pageTextSegmentFromWords(group.choice().asCreditWords()));
        }

        for (const auto &next : group.group())
        {
            if (next.choice().isCreditWords())
            {
                runs.push_back(pageTextSegmentFromWords(next.choice().asCreditWords()));
            }
        }

        if (!runs.empty())
        {
            const auto &first = runs.front();
            pageText.text = first.text;
            pageText.positionData = first.positionData;
            pageText.fontData = first.fontData;
            pageText.justify = first.justify;
            pageText.enclosure = first.enclosure;
            pageText.continuations.assign(runs.begin() + 1, runs.end());
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
