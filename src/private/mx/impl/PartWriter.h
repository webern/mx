// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PartData.h"

#include <mutex>
#include <string>

namespace mx
{
namespace core
{
class PartwisePart;
class ScorePart;
class Token;
} // namespace core

namespace impl
{
class DiagnosticsContext;
class ScoreWriter;

class PartWriter
{
  public:
    PartWriter(const api::PartData &inPartData, int inPartIndex, int inTicksPerQuarter,
               const ScoreWriter &inScoreWriter);
    core::ScorePart getScorePart() const;
    core::PartwisePart getPartwisePart() const;

  private:
    const api::PartData &myPartData;
    const int myPartIndex;
    const int myTicksPerQuarter;
    mutable std::mutex myMutex;
    const ScoreWriter &myScoreWriter;

    const DiagnosticsContext &diagnostics() const;
    // Builds an id token from text, reporting text that is not a valid id.
    core::Token writtenToken(const char *name, const std::string &text) const;

  private:
    /// Writes all the measures from myPartData to outPart
    void writeMeasures(core::PartwisePart &outPart) const;

    /// Writes all the measures from inPartData to outPart. We added this function,
    /// in which the part data is passed in, to handle a case when there are zero measures
    /// in myPartData, but we need to force a single measure to exist in the MusicXML to
    /// preserve something about the part.
    void writeMeasures(const mx::api::PartData &inPartData, core::PartwisePart &outPart) const;
};
} // namespace impl
} // namespace mx
