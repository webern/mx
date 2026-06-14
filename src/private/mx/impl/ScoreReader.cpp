// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/ScoreReader.h"
#include "mx/core/generated/AllMarginsGroup.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Identification.h"
#include "mx/core/generated/LayoutGroup.h"
#include "mx/core/generated/MarginType.h"
#include "mx/core/generated/Miscellaneous.h"
#include "mx/core/generated/MiscellaneousField.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/PageLayout.h"
#include "mx/core/generated/PageLayoutGroup.h"
#include "mx/core/generated/PageMargins.h"
#include "mx/core/generated/PartGroup.h"
#include "mx/core/generated/PartList.h"
#include "mx/core/generated/PartListChoice.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PartwisePart.h"
#include "mx/core/generated/Print.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/core/generated/ScorePart.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/core/generated/SystemLayout.h"
#include "mx/core/generated/SystemMargins.h"
#include "mx/core/generated/TypedText.h"
#include "mx/core/generated/Work.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/Converter.h"
#include "mx/impl/EncodingFunctions.h"
#include "mx/impl/LayoutFunctions.h"
#include "mx/impl/LcmGcd.h"
#include "mx/impl/PageTextFunctions.h"
#include "mx/impl/PartReader.h"
#include "mx/impl/TimeReader.h"
#include "mx/utility/StringToInt.h"
#include "mx/utility/Throw.h"

namespace mx
{
namespace impl
{
ScoreReader::ScoreReader(const core::ScorePartwise &inScorePartwise)
    : myScorePartwise{inScorePartwise}, myPartSet{inScorePartwise.part()}, myHeaderGroup{inScorePartwise.scoreHeader()},
      myMutex{}, myOutScoreData{}, myPartGroupStack{}
{
}

const core::PartwisePart *ScoreReader::findPartwisePart(const core::ScorePart &scorePart,
                                                        std::span<const core::PartwisePart> partwiseParts) const
{
    const auto spId = scorePart.id().value();

    for (const auto &pp : partwiseParts)
    {
        if (pp.id().value() == spId)
        {
            return &pp;
        }
    }

    return nullptr;
}

ScoreReader::ReconciledParts ScoreReader::reconcileParts(const core::ScorePartwise &inScorePartwise) const
{
    ReconciledParts outParts;
    int partIndex = 0;

    const auto &partList = inScorePartwise.scoreHeader().partList();
    const auto partwiseParts = inScorePartwise.part();

    for (const auto &preliminaryGroup : partList.partGroup())
    {
        handlePartGroup(partIndex, preliminaryGroup);
    }

    const auto *scorePart = &partList.scorePart();
    const auto *partwisePart = findPartwisePart(*scorePart, partwiseParts);
    if (scorePart && partwisePart)
    {
        outParts.emplace_back(ReconciledPart{scorePart, partwisePart});
    }

    ++partIndex;

    for (const auto &pgosp : partList.choice())
    {
        if (pgosp.isScorePart())
        {
            scorePart = &pgosp.asScorePart();
            partwisePart = findPartwisePart(*scorePart, partwiseParts);

            if (scorePart && partwisePart)
            {
                outParts.emplace_back(ReconciledPart{scorePart, partwisePart});
            }
            ++partIndex;
        }
        else if (pgosp.isPartGroup())
        {
            handlePartGroup(partIndex, pgosp.asPartGroup());
        }
    }

    auto sortingCompare = [&](api::PartGroupData &a, api::PartGroupData b) {
        if (a.firstPartIndex < b.firstPartIndex)
        {
            return true;
        }
        else if (a.firstPartIndex > b.firstPartIndex)
        {
            return false;
        }
        else if (a.number < b.number)
        {
            return true;
        }
        else if (a.number > b.number)
        {
            return false;
        }
        return false;
    };

    std::sort(myOutScoreData.partGroups.begin(), myOutScoreData.partGroups.end(), sortingCompare);

    return outParts;
}

api::ScoreData ScoreReader::getScoreData() const
{
    std::lock_guard<std::mutex> lock{myMutex};
    myOutScoreData = api::ScoreData{};
    myPartGroupStack = std::list<api::PartGroupData>{};

    myOutScoreData.ticksPerQuarter = findMaxDivisionsPerQuarter();

    if (myScorePartwise.version().has_value())
    {
        myOutScoreData.declaredMusicXmlVersion = *myScorePartwise.version();
        // Map any recognized MusicXML version to ThreePointZero (the only non-unspecified enum value).
        // The model supports MusicXML 4.0; 3.0 documents are backward compatible.
        if (*myScorePartwise.version() == "3.0" || *myScorePartwise.version() == "4.0")
        {
            myOutScoreData.musicXmlVersion = api::MusicXmlVersion::ThreePointZero;
        }
    }

    if (myHeaderGroup.work().has_value() && myHeaderGroup.work()->workTitle().has_value())
    {
        myOutScoreData.workTitle = *myHeaderGroup.work()->workTitle();
    }

    if (myHeaderGroup.work().has_value() && myHeaderGroup.work()->workNumber().has_value())
    {
        myOutScoreData.workNumber = *myHeaderGroup.work()->workNumber();
    }

    if (myHeaderGroup.movementTitle().has_value())
    {
        myOutScoreData.movementTitle = *myHeaderGroup.movementTitle();
    }

    if (myHeaderGroup.movementNumber().has_value())
    {
        myOutScoreData.movementNumber = *myHeaderGroup.movementNumber();
    }

    bool isComposerFound = false;
    bool isCopyrightFound = false;

    if (myHeaderGroup.identification().has_value())
    {
        const auto &ident = *myHeaderGroup.identification();

        for (const auto &i : ident.creator())
        {
            const bool hasType = i.type().has_value();
            const std::string typeStr = hasType ? *i.type() : std::string{};

            if (typeStr == "composer" && !isComposerFound)
            {
                myOutScoreData.composer = i.value();
                isComposerFound = true;
            }

            if (!isComposerFound && !hasType)
            {
                myOutScoreData.composer = i.value();
            }

            if (typeStr == "lyricist")
            {
                myOutScoreData.lyricist = i.value();
            }

            if (typeStr == "arranger")
            {
                myOutScoreData.lyricist = i.value();
            }

            if (typeStr == "publisher")
            {
                myOutScoreData.lyricist = i.value();
            }
        }

        for (const auto &r : ident.rights())
        {
            const bool hasType = r.type().has_value();
            const std::string typeStr = hasType ? *r.type() : std::string{};

            if (hasType && typeStr == "copyright" && !isCopyrightFound)
            {
                myOutScoreData.copyright = r.value();
                isCopyrightFound = true;
            }

            if (!isCopyrightFound && !hasType)
            {
                myOutScoreData.copyright = r.value();
            }
        }
        api::EncodingData encodingData;

        if (ident.encoding().has_value())
        {
            encodingData = createEncoding(*ident.encoding());
        }

        if (ident.miscellaneous().has_value())
        {
            for (const auto &m : ident.miscellaneous()->miscellaneousField())
            {
                std::string key;
                key = m.name();
                encodingData.miscelaneousFields.emplace_back(m.name(), m.value());
            }
        }

        myOutScoreData.encoding = std::move(encodingData);
    }

    if (myHeaderGroup.defaults().has_value())
    {
        myOutScoreData.defaults = createDefaults(myHeaderGroup);
    }

    createPageTextItems(myHeaderGroup, myOutScoreData.pageTextItems);

    auto partMap = reconcileParts(myScorePartwise);
    int divisionsValue = -1;
    for (const auto &reconciledPart : partMap)
    {
        PartReader reader{*reconciledPart.first, *reconciledPart.second, myOutScoreData.ticksPerQuarter,
                          myScorePartwise, divisionsValue};
        myOutScoreData.parts.emplace_back(reader.getPartData());
        const auto cursorReturn = reader.getCursor();
        divisionsValue = cursorReturn.ticksPerQuarter;
    }

    scanForSystemInfo();
    scanForPageInfo();

    api::ScoreData temp{std::move(myOutScoreData)};
    myOutScoreData = api::ScoreData{};
    return temp;
}

void ScoreReader::handlePartGroup(int partIndex, const core::PartGroup &inPartGroup) const
{
    const auto &startStop = inPartGroup.type();
    if (startStop.tag() == core::StartStop::Tag::start)
    {
        startPartGroup(partIndex, inPartGroup);
    }
    else
    {
        stopPartGroup(partIndex, inPartGroup);
    }
}

void ScoreReader::startPartGroup(int partIndex, const core::PartGroup &inPartGroup) const
{
    api::PartGroupData grpData;
    grpData.number = parsePartGroupNumber(inPartGroup);

    if (inPartGroup.groupName().has_value())
    {
        grpData.name = inPartGroup.groupName()->value();
    }

    if (inPartGroup.groupAbbreviation().has_value())
    {
        grpData.abbreviation = inPartGroup.groupAbbreviation()->value();
    }

    if (inPartGroup.groupSymbol().has_value())
    {
        Converter c;
        grpData.bracketType = c.convert(inPartGroup.groupSymbol()->value());
    }

    grpData.firstPartIndex = partIndex;

    // TODO - group name display
    // TODO - group abbreviation display
    // TODO - barline, etc

    myPartGroupStack.push_front(grpData);
}

void ScoreReader::stopPartGroup(int partIndex, const core::PartGroup &inPartGroup) const
{
    if (myPartGroupStack.empty())
    {
        return;
    }
    const int partGroupNumber = parsePartGroupNumber(inPartGroup);

    api::PartGroupData grpData;

    if (groupNumberExistsOnStack(partGroupNumber))
    {
        grpData = popGroupFromStack(partGroupNumber);
    }
    else
    {
        grpData = popMostRecentGroupFromStack();
    }

    grpData.lastPartIndex = partIndex > 0 ? partIndex - 1 : partIndex;

    myOutScoreData.partGroups.emplace_back(std::move(grpData));
}

bool ScoreReader::groupNumberExistsOnStack(int groupNumber) const
{
    for (const auto &g : myPartGroupStack)
    {
        if (g.number == groupNumber)
        {
            return true;
        }
    }
    return false;
}

api::PartGroupData ScoreReader::popGroupFromStack(int groupNumber) const
{
    auto lambda = [&](const api::PartGroupData &grp) { return grp.number == groupNumber; };

    auto iter = std::find_if(myPartGroupStack.cbegin(), myPartGroupStack.cend(), lambda);

    if (iter == myPartGroupStack.cend())
    {
        MX_BUG;
    }
    auto outGroup = *iter;
    myPartGroupStack.erase(iter);
    return outGroup;
}

api::PartGroupData ScoreReader::popMostRecentGroupFromStack() const
{
    if (myPartGroupStack.empty())
    {
        MX_BUG;
    }

    auto iter = myPartGroupStack.cbegin();
    auto outGroup = *iter;
    myPartGroupStack.erase(iter);
    return outGroup;
}

int ScoreReader::parsePartGroupNumber(const core::PartGroup &inPartGroup) const
{
    int num = -1;

    if (inPartGroup.number().has_value())
    {
        const auto str = *inPartGroup.number();
        bool isGroupNumberValid = utility::stringToInt(str, num);

        // TODO - support non-numeric group numbers if someone complains
        MX_ASSERT(isGroupNumberValid);
    }

    return num;
}

void ScoreReader::scanForSystemInfo() const
{
    // scan only the top part looking for system layout information
    if (myScorePartwise.part().empty())
    {
        return;
    }

    const auto &topPart = myScorePartwise.part().front();

    int measureIndex = 0;
    for (const auto &m : topPart.measure())
    {
        for (const auto &mdc : m.musicData())
        {
            if (!mdc.isPrint())
            {
                continue;
            }
            const auto &print = mdc.asPrint();
            auto systemData = api::SystemData{};
            if (print.newSystem().has_value())
            {
                const auto newSystem = print.newSystem()->tag() == core::YesNo::Tag::yes;
                systemData.newSystem = api::fromBool(newSystem);
            }
            const auto &layoutGroup = print.layout();
            if (layoutGroup.systemLayout().has_value())
            {
                const auto &systemLayout = *layoutGroup.systemLayout();
                if (systemLayout.systemMargins().has_value())
                {
                    const auto &margins = systemLayout.systemMargins()->leftRightMargins();
                    systemData.layout.margins =
                        api::LeftRight{margins.leftMargin().value().value(), margins.rightMargin().value().value()};
                }

                if (systemLayout.systemDistance().has_value())
                {
                    systemData.layout.systemDistance = systemLayout.systemDistance()->value().value();
                }

                if (systemLayout.topSystemDistance().has_value())
                {
                    systemData.layout.topSystemDistance = systemLayout.topSystemDistance()->value().value();
                }
            }
            if (systemData.isUsed())
            {
                auto &layout = myOutScoreData.layout[measureIndex];
                layout.system = systemData;
            }
        }
        ++measureIndex;
    }
}

void ScoreReader::scanForPageInfo() const
{
    if (myScorePartwise.part().empty())
    {
        return;
    }

    // scan only the top part looking for system layout information
    const auto &topPart = myScorePartwise.part().front();

    int measureIndex = 0;
    for (const auto &m : topPart.measure())
    {
        for (const auto &mdc : m.musicData())
        {
            if (!mdc.isPrint())
            {
                continue;
            }
            const auto &print = mdc.asPrint();
            api::PageData outPageData{};
            if (print.newPage().has_value())
            {
                outPageData.newPage = print.newPage()->tag() == core::YesNo::Tag::yes ? api::Bool::yes : api::Bool::no;
            }
            else
            {
                outPageData.newPage = api::Bool::unspecified;
            }
            if (print.pageNumber().has_value())
            {
                outPageData.pageNumber = *print.pageNumber();
            }
            const auto &layoutGroup = print.layout();
            if (layoutGroup.pageLayout().has_value())
            {
                const auto &pageLayout = *layoutGroup.pageLayout();
                if (pageLayout.group().has_value())
                {
                    outPageData.pageLayoutData.size =
                        api::SizeData{static_cast<double>(pageLayout.group()->pageHeight().value().value()),
                                      static_cast<double>(pageLayout.group()->pageWidth().value().value())};
                }
                for (const auto &pageMargins : pageLayout.pageMargins())
                {
                    // Warning - this implementation may not be exactly correct.
                    // musicxml.xsd says: "Page margins are specified either for both even and odd pages, or via
                    // separate odd and even page number values. The type attribute is not needed when used as part
                    // of a print element. If omitted when the page-margins type is used in the defaults element,
                    // "both" is the default value."
                    // This might mean that a print element can only affect the page layout of even pages (by
                    // appearing on an even page) or odd pages (by appearing on an odd page). I'm not sure, either
                    // way it seems doubtful that notation applications will agree.
                    auto t = core::MarginType::both();
                    if (pageMargins.type().has_value())
                    {
                        t = *pageMargins.type();
                    }
                    const auto &allMargins = pageMargins.allMargins();
                    const double left = allMargins.leftRightMargins().leftMargin().value().value();
                    const double right = allMargins.leftRightMargins().rightMargin().value().value();
                    const double top = allMargins.topMargin().value().value();
                    const double bottom = allMargins.bottomMargin().value().value();
                    const api::MarginsData margins{left, right, top, bottom};
                    const bool writeOdd =
                        t.tag() == core::MarginType::Tag::both || t.tag() == core::MarginType::Tag::odd;
                    const bool writeEven =
                        t.tag() == core::MarginType::Tag::both || t.tag() == core::MarginType::Tag::even;
                    if (writeOdd)
                    {
                        outPageData.pageLayoutData.margins.odd = margins;
                    }
                    if (writeEven)
                    {
                        outPageData.pageLayoutData.margins.even = margins;
                    }
                } // for each PageMargins element
            } // if hasPageLayout
            if (outPageData.isUsed())
            {
                auto &layout = myOutScoreData.layout[measureIndex];
                layout.page = outPageData;
                // TODO break?
            }
        } // for each mdc
        ++measureIndex;
    } // for each measure
}

int ScoreReader::findMaxDivisionsPerQuarter() const
{
    std::set<int> foundDivisions;

    for (const auto &pp : myScorePartwise.part())
    {
        for (const auto &m : pp.measure())
        {
            for (const auto &mdc : m.musicData())
            {
                if (!mdc.isAttributes())
                {
                    continue;
                }

                const auto &attrs = mdc.asAttributes();

                if (!attrs.divisions().has_value())
                {
                    continue;
                }

                const auto tempDiv = attrs.divisions()->value().value();
                const int tempDivInt = static_cast<int>(std::ceil(tempDiv - 0.5));
                if (tempDivInt > 0)
                {
                    foundDivisions.insert(tempDivInt);
                }
            }
        }
    }

    return mx::impl::leastCommonMultiple(foundDivisions);
}

} // namespace impl
} // namespace mx
