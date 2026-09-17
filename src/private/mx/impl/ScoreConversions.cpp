// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/ScoreConversions.h"

#include "mx/core/generated/PartList.h"
#include "mx/core/generated/PartListChoice.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PartwisePart.h"
#include "mx/core/generated/ScorePart.h"
#include "mx/core/generated/TimewiseMeasure.h"
#include "mx/core/generated/TimewisePart.h"

#include <algorithm>
#include <vector>

namespace mx
{
namespace impl
{

namespace
{

/* Create a simple list of parts from the score header */
std::vector<const core::ScorePart *> headerPartList(const core::ScoreHeaderGroup &header)
{
    std::vector<const core::ScorePart *> partlist;
    partlist.push_back(&header.partList().scorePart());
    for (const auto &choice : header.partList().choice())
    {
        if (choice.kind() == core::PartListChoice::Kind::scorePart)
        {
            partlist.push_back(&choice.asScorePart());
        }
    }
    return partlist;
}

} // namespace

core::ScoreTimewise partwiseTimewise(const core::ScorePartwise &inScore)
{
    core::ScoreTimewise outScore;
    outScore.setScoreHeader(inScore.scoreHeader());
    outScore.setVersion(inScore.version());

    /* find max number of measures */
    size_t measureCount = 0;
    for (const auto &p : inScore.part())
    {
        if (p.measure().size() > measureCount)
        {
            measureCount = p.measure().size();
        }
    }

    std::vector<core::TimewiseMeasure> outMeasures;

    /* Outer Loop, Once for each Measure */
    for (size_t currentMeasureIndex = 0; currentMeasureIndex < measureCount; ++currentMeasureIndex)
    {
        core::TimewiseMeasure newMeasure;
        std::vector<core::TimewisePart> newParts;

        /* First Nested Loop, for each Part */
        for (const auto *sp : headerPartList(inScore.scoreHeader()))
        {
            /* Find the Correct Part */
            const auto inParts = inScore.part();
            const auto partIter =
                std::find_if(inParts.begin(), inParts.end(), [&](const core::PartwisePart &searchingForThis) {
                    return searchingForThis.id().value() == sp->id().value();
                });
            if (partIter == inParts.end())
            {
                continue;
            }
            // The old pivot walked past-the-end (undefined behavior) when a
            // part had fewer measures than the longest part; bounds-check
            // instead (adjudication log entry 6).
            if (currentMeasureIndex >= partIter->measure().size())
            {
                continue;
            }
            const core::PartwiseMeasure &sourceMeasure = partIter->measure()[currentMeasureIndex];

            /* Create and add the Part Music to the outer loop's Measure */
            core::TimewisePart newPart;
            newPart.setID(partIter->id());
            newPart.setMusicData({sourceMeasure.musicData().begin(), sourceMeasure.musicData().end()});
            newParts.push_back(std::move(newPart));

            // As in the old pivot: the measure attributes come from the
            // (last) source part's measure at this index.
            newMeasure.setNumber(sourceMeasure.number());
            newMeasure.setText(sourceMeasure.text());
            newMeasure.setImplicit(sourceMeasure.implicit());
            newMeasure.setNonControlling(sourceMeasure.nonControlling());
            newMeasure.setWidth(sourceMeasure.width());
            newMeasure.setID(sourceMeasure.id());
        }

        core::OneOrMore<core::TimewisePart> parts;
        parts.setItems(std::move(newParts));
        newMeasure.setPart(std::move(parts));
        outMeasures.push_back(std::move(newMeasure));
    }

    if (!outMeasures.empty())
    {
        core::OneOrMore<core::TimewiseMeasure> measures;
        measures.setItems(std::move(outMeasures));
        outScore.setMeasure(std::move(measures));
    }
    return outScore;
}

core::ScorePartwise timewisePartwise(const core::ScoreTimewise &inScore, const DiagnosticsContext &diagnostics)
{
    core::ScorePartwise outScore;
    outScore.setScoreHeader(inScore.scoreHeader());
    outScore.setVersion(inScore.version());

    /* Create a PartwisePart for each part in the main list */
    std::vector<core::PartwisePart> outParts;
    for (const auto *sp : headerPartList(inScore.scoreHeader()))
    {
        core::PartwisePart newPart;
        newPart.setID(sp->id());

        /* inner loop: for each measure in inScore */
        std::vector<core::PartwiseMeasure> newMeasures;
        for (const auto &m : inScore.measure())
        {
            core::PartwiseMeasure newMeasure;
            newMeasure.setNumber(m.number());
            newMeasure.setText(m.text());
            newMeasure.setImplicit(m.implicit());
            newMeasure.setNonControlling(m.nonControlling());
            newMeasure.setWidth(m.width());
            newMeasure.setID(m.id());

            /* find the correct music data */
            const auto inParts = m.part();
            const auto partIter =
                std::find_if(inParts.begin(), inParts.end(), [&](const core::TimewisePart &searchingForThis) {
                    return searchingForThis.id().value() == newPart.id().value();
                });
            if (partIter != inParts.end())
            {
                newMeasure.setMusicData({partIter->musicData().begin(), partIter->musicData().end()});
            }
            newMeasures.push_back(std::move(newMeasure));
        }

        if (!newMeasures.empty())
        {
            core::OneOrMore<core::PartwiseMeasure> measures;
            measures.setItems(std::move(newMeasures));
            newPart.setMeasure(std::move(measures));
        }
        outParts.push_back(std::move(newPart));
    }

    if (!outParts.empty())
    {
        core::OneOrMore<core::PartwisePart> parts;
        parts.setItems(std::move(outParts));
        outScore.setPart(std::move(parts));
    }
    return outScore;
}

} // namespace impl
} // namespace mx
