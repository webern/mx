// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/DirectionData.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

#include <set>
#include <vector>

namespace mx
{
namespace impl
{
class DirectionWriter
{
  public:
    DirectionWriter(const api::DirectionData &inDirectionData, const Cursor &inCursor);
    std::vector<core::MusicDataChoice> getDirectionLikeThings();

  private:
    void addDirectionType(core::DirectionType directionType, core::Direction &ioDirection);
    std::vector<core::MusicDataChoice> createHarmonyElements(int inOffset);
    std::vector<core::MusicDataChoice> createFiguredBassElements();
    void addMusicDataChoices(const std::vector<core::MusicDataChoice> &inMdcs,
                             std::vector<core::MusicDataChoice> &ioOutputSet);

    void emitMark(api::MarkData mark, core::Direction &direction);
    void emitPedalStart(const api::SpannerStart &pedalStart, core::Direction &direction);
    void emitPedalStop(const api::SpannerStop &pedalStop, core::Direction &direction);
    void emitWedgeStop(const api::WedgeStop &wedgeStop, core::Direction &direction);
    void emitWedgeStart(const api::WedgeStart &wedgeStart, core::Direction &direction);
    void emitOttavaStop(const api::SpannerStop &ottavaStop, core::Direction &direction);
    void emitOttavaStart(const api::OttavaStart &ottavaStart, core::Direction &direction);
    void emitBracketStart(const api::SpannerStart &item, core::Direction &direction);
    void emitBracketStop(const api::SpannerStop &item, core::Direction &direction);
    void emitDashesStart(const api::SpannerStart &item, core::Direction &direction);
    void emitDashesStop(const api::SpannerStop &item, core::Direction &direction);
    void emitTempo(const api::TempoData &tempo, core::Direction &direction);
    void emitWords(const std::vector<api::WordsData> &wordsVec, core::Direction &direction);
    void emitSegno(const api::SegnoData &item, core::Direction &direction);
    void emitCoda(const api::CodaData &item, core::Direction &direction);
    void emitRehearsal(const api::RehearsalData &item, core::Direction &direction);
    void emitFixedOrder(core::Direction &direction);
    void emitOrderedComponents(core::Direction &direction);

  private:
    const api::DirectionData &myDirectionData;
    const Cursor myCursor;
    const Converter myConverter;
    bool myIsFirstDirectionTypeAdded;
    std::set<api::Placement> myPlacements;
};
} // namespace impl
} // namespace mx
