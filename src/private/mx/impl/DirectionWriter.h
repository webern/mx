// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
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

  private:
    const api::DirectionData &myDirectionData;
    const Cursor myCursor;
    const Converter myConverter;
    bool myIsFirstDirectionTypeAdded;
    std::set<api::Placement> myPlacements;
};
} // namespace impl
} // namespace mx
