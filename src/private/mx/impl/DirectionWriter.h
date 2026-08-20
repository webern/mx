// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/DirectionData.h"
#include "mx/core/generated/EmptyPrintStyleAlignID.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/PercussionChoice.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"
#include "mx/impl/SpannerNumberResolver.h"

#include <set>
#include <vector>

namespace mx
{
namespace impl
{
class DirectionWriter
{
  public:
    // inNumberResolver supplies the resolved 'number' for each wedge, octave
    // shift, bracket, and dashes start/stop (see SpannerNumberResolver); it
    // must outlive this writer.
    DirectionWriter(const api::DirectionData &inDirectionData, const Cursor &inCursor,
                    const SpannerNumberResolver &inNumberResolver);
    std::vector<core::MusicDataChoice> getDirectionLikeThings();

  private:
    void addDirectionType(core::DirectionType directionType, core::Direction &ioDirection);
    std::vector<core::MusicDataChoice> createHarmonyElements(int inOffset);
    std::vector<core::MusicDataChoice> createFiguredBassElements();
    void addMusicDataChoices(const std::vector<core::MusicDataChoice> &inMdcs,
                             std::vector<core::MusicDataChoice> &ioOutputSet);

    // The spanner emitters take inIdentity, the address of the DirectionChoice that holds the
    // spanner: the same address the SpannerNumberResolver saw when it walked the score, so the
    // resolved 'number' can be looked up for the emitted element.
    void emitMark(api::MarkData mark, core::Direction &direction);
    void emitPedal(const api::PedalLineData &pedal, core::Direction &direction);
    void emitWedgeStop(const api::WedgeStop &wedgeStop, const void *inIdentity, core::Direction &direction);
    void emitWedgeStart(const api::WedgeStart &wedgeStart, const void *inIdentity, core::Direction &direction);
    void emitOttavaStop(const api::OttavaStop &ottavaStop, const void *inIdentity, core::Direction &direction);
    void emitOttavaStart(const api::OttavaStart &ottavaStart, const void *inIdentity, core::Direction &direction);
    void emitBracketStart(const api::SpannerStart &item, const void *inIdentity, core::Direction &direction);
    void emitBracketStop(const api::SpannerStop &item, const void *inIdentity, core::Direction &direction);
    void emitDashesStart(const api::SpannerStart &item, const void *inIdentity, core::Direction &direction);
    void emitDashesStop(const api::SpannerStop &item, const void *inIdentity, core::Direction &direction);
    void emitTempo(const api::TempoData &tempo, core::Direction &direction);
    void emitWordsRun(const std::vector<api::WordsChoice> &inRun, core::Direction &direction);
    void emitSegno(const api::SegnoData &item, core::Direction &direction);
    void emitCoda(const api::CodaData &item, core::Direction &direction);
    void emitRehearsal(const api::RehearsalData &item, core::Direction &direction);
    core::EmptyPrintStyleAlignID createEmptyPrintStyleAlign(const api::PositionData &positionData,
                                                            const api::FontData &fontData,
                                                            const std::optional<api::ColorData> &color,
                                                            const std::optional<api::Id> &id);
    void emitDamp(const api::DampData &item, core::Direction &direction);
    void emitDampAll(const api::DampAllData &item, core::Direction &direction);
    void emitEyeglasses(const api::EyeglassesData &item, core::Direction &direction);
    void emitStringMute(const api::StringMuteData &item, core::Direction &direction);
    void emitStaffDivide(const api::StaffDivideData &item, core::Direction &direction);
    void emitPrincipalVoice(const api::PrincipalVoiceData &item, core::Direction &direction);
    void emitOtherDirection(const api::OtherDirectionData &item, core::Direction &direction);
    void emitImage(const api::ImageData &item, core::Direction &direction);
    void emitAccordionRegistration(const api::AccordionRegistrationData &item, core::Direction &direction);
    void emitHarpPedals(const api::HarpPedalsData &item, core::Direction &direction);
    void emitScordatura(const api::ScordaturaData &item, core::Direction &direction);
    core::PercussionChoice createPercussionChoice(const api::PercussionDataChoice &choice);
    void emitPercussion(const api::PercussionData &item, core::Direction &direction);
    void emitDirectionTypes(core::Direction &direction);

  private:
    const api::DirectionData &myDirectionData;
    const Cursor myCursor;
    const SpannerNumberResolver &myNumberResolver;
    const Converter myConverter;
    bool myIsFirstDirectionTypeAdded;
    std::set<api::Placement> myPlacements;
};
} // namespace impl
} // namespace mx
