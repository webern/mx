// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/impl/DiagnosticsContext.h"

#include <list>
#include <mutex>
#include <span>
#include <utility>
#include <vector>

namespace mx
{
namespace impl
{
class ScoreReader
{
  public:
    ScoreReader(const core::ScorePartwise &inScorePartwise, DiagnosticsContext diagnostics = {});

    api::ScoreData getScoreData() const;

  private:
    const core::ScorePartwise &myScorePartwise;
    std::span<const core::PartwisePart> myPartSet;
    const core::ScoreHeaderGroup &myHeaderGroup;
    DiagnosticsContext myDiagnostics;

  private:
    mutable std::mutex myMutex;
    mutable api::ScoreData myOutScoreData;
    mutable std::list<api::PartGroupData> myPartGroupStack;

  private:
    using ReconciledPart = std::pair<const core::ScorePart *, const core::PartwisePart *>;
    using ReconciledParts = std::vector<ReconciledPart>;

    const core::PartwisePart *findPartwisePart(const core::ScorePart &scorePart,
                                               std::span<const core::PartwisePart> partwiseParts) const;
    ReconciledParts reconcileParts(const core::ScorePartwise &inScorePartwise) const;
    void handlePartGroup(int partIndex, const core::PartGroup &inPartGroup) const;
    void startPartGroup(int partIndex, const core::PartGroup &inPartGroup) const;
    void stopPartGroup(int partIndex, const core::PartGroup &inPartGroup) const;
    bool groupNumberExistsOnStack(int groupNumber) const;
    api::PartGroupData popGroupFromStack(int groupNumber) const;
    api::PartGroupData popMostRecentGroupFromStack() const;
    int parsePartGroupNumber(const core::PartGroup &inPartGroup) const;
    void scanForSystemInfo() const;
    void scanForPageInfo() const;
    int findMaxDivisionsPerQuarter() const;
};
} // namespace impl
} // namespace mx
