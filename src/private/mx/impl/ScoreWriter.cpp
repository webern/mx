// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/ScoreWriter.h"
#include "mx/core/OneOrMore.h"
#include "mx/core/generated/GroupBarline.h"
#include "mx/core/generated/GroupName.h"
#include "mx/core/generated/GroupSymbol.h"
#include "mx/core/generated/Identification.h"
#include "mx/core/generated/PartGroup.h"
#include "mx/core/generated/PartList.h"
#include "mx/core/generated/PartListChoice.h"
#include "mx/core/generated/PartwisePart.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/core/generated/ScorePart.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/core/generated/TypedText.h"
#include "mx/core/generated/Work.h"
#include "mx/impl/Converter.h"
#include "mx/impl/EncodingFunctions.h"
#include "mx/impl/LayoutFunctions.h"
#include "mx/impl/PageTextFunctions.h"
#include "mx/impl/PartReader.h"
#include "mx/impl/PartWriter.h"
#include "mx/impl/TimeReader.h"

namespace mx
{
namespace impl
{
ScoreWriter::ScoreWriter(const api::ScoreData &inScoreData) : myScoreData{inScoreData}, myMutex{}, myOutScorePartwise{}
{
    myScoreData.sort();
}

core::ScorePartwise ScoreWriter::getScorePartwise() const
{
    std::lock_guard<std::mutex> lock{myMutex};
    myOutScorePartwise = core::ScorePartwise{};

    switch (this->myScoreData.musicXmlVersion)
    {
    case api::MusicXmlVersion::unspecified: {
        myOutScorePartwise.setVersion(std::nullopt);
    }
    break;

    case api::MusicXmlVersion::ThreePointZero: {
        myOutScorePartwise.setVersion(std::string{"3.0"});
    }
    break;

    default: {
        myOutScorePartwise.setVersion(std::nullopt);
    }
    break;
    }

    core::ScoreHeaderGroup header{};

    if (!myScoreData.workTitle.empty() || !myScoreData.workNumber.empty())
    {
        core::Work work{};
        if (!myScoreData.workTitle.empty())
        {
            work.setWorkTitle(myScoreData.workTitle);
        }
        if (!myScoreData.workNumber.empty())
        {
            work.setWorkNumber(myScoreData.workNumber);
        }
        header.setWork(work);
    }

    if (!myScoreData.movementTitle.empty())
    {
        header.setMovementTitle(myScoreData.movementTitle);
    }

    if (!myScoreData.movementNumber.empty())
    {
        header.setMovementNumber(myScoreData.movementNumber);
    }

    core::Identification identification{};
    bool hasIdentification = false;

    if (!myScoreData.composer.empty())
    {
        core::TypedText composer{};
        composer.setType(std::string{"composer"});
        composer.setValue(myScoreData.composer);
        identification.addCreator(composer);
        hasIdentification = true;
    }

    if (!myScoreData.lyricist.empty())
    {
        core::TypedText lyricist{};
        lyricist.setType(std::string{"lyricist"});
        lyricist.setValue(myScoreData.lyricist);
        identification.addCreator(lyricist);
        hasIdentification = true;
    }

    if (!myScoreData.copyright.empty())
    {
        core::TypedText copyright{};
        copyright.setType(std::string{"copyright"});
        copyright.setValue(myScoreData.copyright);
        identification.addRights(copyright);
        hasIdentification = true;
    }

    if (hasIdentification)
    {
        header.setIdentification(identification);
    }

    createEncoding(myScoreData.encoding, header);
    addDefaultsData(myScoreData.defaults, header);
    createCredits(myScoreData, header);

    using PartPair = std::pair<core::ScorePart, core::PartwisePart>;
    using PartPairs = std::vector<PartPair>;

    int partIndex = 0;
    PartPairs partPairs;
    for (const auto &partData : myScoreData.parts)
    {
        PartWriter partWriter{partData, partIndex, myScoreData.ticksPerQuarter, *this};
        partPairs.emplace_back(std::make_pair(partWriter.getScorePart(), partWriter.getPartwisePart()));
        ++partIndex;
    }

    myOutScorePartwise.setScoreHeader(header);

    partIndex = 0;
    for (const auto &partPair : partPairs)
    {
        addScorePart(partIndex, partPair.first);
        addPartwisePart(partIndex, partPair.second);
        ++partIndex;
    }

    return myOutScorePartwise;
}

const api::PartData &ScoreWriter::getPart(int inPartIndex) const
{
    return getScoreData().parts.at(inPartIndex);
}

void ScoreWriter::addScorePart(int partIndex, core::ScorePart scorePart) const
{
    core::ScoreHeaderGroup header = myOutScorePartwise.scoreHeader();
    core::PartList partList = header.partList();

    auto reverseStopsCompare = [&](api::PartGroupData &a, api::PartGroupData &b) { return a.number > b.number; };

    if (partIndex == 0)
    {
        if (partGroupStartExists(partIndex))
        {
            const auto grps = findPartGroupsByStartIndex(partIndex);

            for (const auto &g : grps)
            {
                partList.addPartGroup(makePartGroupStart(g));
            }
        }
        partList.setScorePart(scorePart);

        if (partGroupStopExists(partIndex))
        {
            auto grps = findPartGroupsByStopIndex(partIndex);
            std::sort(grps.begin(), grps.end(), reverseStopsCompare);

            for (const auto &g : grps)
            {
                partList.addChoice(core::PartListChoice::partGroup(makePartGroupStop(g)));
            }
        }
    }
    else
    {
        if (partGroupStartExists(partIndex))
        {
            const auto grps = findPartGroupsByStartIndex(partIndex);

            for (const auto &g : grps)
            {
                partList.addChoice(core::PartListChoice::partGroup(makePartGroupStart(g)));
            }
        }
        partList.addChoice(core::PartListChoice::scorePart(scorePart));

        if (partGroupStopExists(partIndex))
        {
            auto grps = findPartGroupsByStopIndex(partIndex);
            std::sort(grps.begin(), grps.end(), reverseStopsCompare);

            for (const auto &g : grps)
            {
                partList.addChoice(core::PartListChoice::partGroup(makePartGroupStop(g)));
            }
        }
    }

    header.setPartList(partList);
    myOutScorePartwise.setScoreHeader(header);
}

void ScoreWriter::addPartwisePart(int partIndex, core::PartwisePart partwisePart) const
{
    myOutScorePartwise.addPart(partwisePart);
    const auto parts = myOutScorePartwise.part();

    if (partIndex == 0 && parts.size() == 2)
    {
        // Remove the default placeholder part that was there before we added any real parts.
        // Reconstruct with only the real part (the last one added).
        core::OneOrMore<core::PartwisePart> newParts{parts[1]};
        myOutScorePartwise.setPart(newParts);
    }
}

bool ScoreWriter::partGroupStartExists(int partIndex) const
{
    for (const auto &g : myScoreData.partGroups)
    {
        if (g.firstPartIndex == partIndex)
        {
            return true;
        }
    }
    return false;
}

bool ScoreWriter::partGroupStopExists(int partIndex) const
{
    for (const auto &g : myScoreData.partGroups)
    {
        if (g.lastPartIndex == partIndex)
        {
            return true;
        }
    }
    return false;
}

std::vector<api::PartGroupData> ScoreWriter::findPartGroupsByStartIndex(int partIndex) const
{
    std::vector<api::PartGroupData> outGroups;
    for (const auto &g : myScoreData.partGroups)
    {
        if (g.firstPartIndex == partIndex)
        {
            outGroups.push_back(g);
        }
    }
    return outGroups;
}

std::vector<api::PartGroupData> ScoreWriter::findPartGroupsByStopIndex(int partIndex) const
{
    std::vector<api::PartGroupData> outGroups;
    for (const auto &g : myScoreData.partGroups)
    {
        if (g.lastPartIndex == partIndex)
        {
            outGroups.push_back(g);
        }
    }
    return outGroups;
}

core::PartGroup ScoreWriter::makePartGroupStart(const api::PartGroupData &apiGrp) const
{
    core::PartGroup mxGrp{};
    mxGrp.setType(core::StartStop::start());

    if (apiGrp.number >= 0)
    {
        mxGrp.setNumber(std::to_string(apiGrp.number));
    }

    if (apiGrp.name.size() > 0)
    {
        core::GroupName groupName{};
        groupName.setValue(apiGrp.name);
        mxGrp.setGroupName(groupName);
    }

    Converter converter;
    if (apiGrp.bracketType != api::BracketType::unspecified)
    {
        const auto symbolValue = converter.convert(apiGrp.bracketType);
        core::GroupSymbol groupSymbol{};
        groupSymbol.setValue(symbolValue);
        mxGrp.setGroupSymbol(groupSymbol);
    }

    // TODO - make group barline configurable

    core::GroupBarline groupBarline{};
    groupBarline.setValue(core::GroupBarlineValue::yes());
    mxGrp.setGroupBarline(groupBarline);

    return mxGrp;
}

core::PartGroup ScoreWriter::makePartGroupStop(const api::PartGroupData &apiGrp) const
{
    core::PartGroup mxGrp{};
    mxGrp.setType(core::StartStop::stop());
    if (apiGrp.number >= 0)
    {
        mxGrp.setNumber(std::to_string(apiGrp.number));
    }
    return mxGrp;
}

bool ScoreWriter::isSystemInfo(int measureIndex) const
{
    const auto iter = myScoreData.layout.find(measureIndex);
    if (iter == std::cend(myScoreData.layout))
    {
        return false;
    }
    return iter->second.system.isUsed();
}

std::optional<api::PageData> ScoreWriter::findPageLayoutData(api::MeasureIndex measureIndex) const
{
    const auto iter = myScoreData.layout.find(measureIndex);
    if (iter == std::cend(myScoreData.layout))
    {
        return std::nullopt;
    }
    if (!iter->second.page.isUsed())
    {
        return std::nullopt;
    }
    return iter->second.page;
}

api::SystemData ScoreWriter::getSystemData(int measureIndex) const
{
    const auto iter = myScoreData.layout.find(measureIndex);
    if (iter == std::cend(myScoreData.layout))
    {
        return api::SystemData{};
    }
    return iter->second.system;
}
} // namespace impl
} // namespace mx
