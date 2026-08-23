// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/core/generated/PartGroup.h"
#include "mx/core/generated/PartwisePart.h"
#include "mx/core/generated/ScorePart.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/impl/Cursor.h"
#include "mx/impl/SpannerResolver.h"

#include <mutex>
#include <optional>
#include <vector>

namespace mx
{
namespace impl
{
class ScoreWriter
{
  public:
    ScoreWriter(const api::ScoreData &inScoreData);

    core::ScorePartwise getScorePartwise() const;

    inline const api::ScoreData &getScoreData() const
    {
        return myScoreData;
    }

    /// Finds the part in ScoreData and returns it. Throws if out-of-range.
    const api::PartData &getPart(int inPartIndex) const;

    bool isSystemInfo(int measureIndex) const;
    std::optional<api::PageData> findPageLayoutData(api::MeasureIndex measureIndex) const;
    api::SystemData getSystemData(int measureIndex) const;

    // Get the writers shared spanner resolution state object.
    inline const SpannerResolver &getSpannerResolver() const
    {
        return mySpannerResolver;
    }

  private:
    api::ScoreData myScoreData;
    SpannerResolver mySpannerResolver;
    mutable std::mutex myMutex;
    mutable core::ScorePartwise myOutScorePartwise;

  private:
    void addScorePart(int partIndex, core::ScorePart scorePart) const;
    void addPartwisePart(int partIndex, core::PartwisePart partwisePart) const;
    bool partGroupStartExists(int partIndex) const;
    bool partGroupStopExists(int partIndex) const;
    std::vector<api::PartGroupData> findPartGroupsByStartIndex(int partIndex) const;
    std::vector<api::PartGroupData> findPartGroupsByStopIndex(int partIndex) const;
    core::PartGroup makePartGroupStart(const api::PartGroupData &apiGrp) const;
    core::PartGroup makePartGroupStop(const api::PartGroupData &apiGrp) const;
};
} // namespace impl
} // namespace mx
