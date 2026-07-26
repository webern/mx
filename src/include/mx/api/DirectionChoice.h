// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/AccordionRegistrationData.h"
#include "mx/api/ApiCommon.h"
#include "mx/api/CodaData.h"
#include "mx/api/DampData.h"
#include "mx/api/EyeglassesData.h"
#include "mx/api/HarpPedalsData.h"
#include "mx/api/ImageData.h"
#include "mx/api/MarkData.h"
#include "mx/api/OtherDirectionData.h"
#include "mx/api/OttavaData.h"
#include "mx/api/PedalLineData.h"
#include "mx/api/PercussionData.h"
#include "mx/api/PrincipalVoiceData.h"
#include "mx/api/RehearsalData.h"
#include "mx/api/ScordaturaData.h"
#include "mx/api/SegnoData.h"
#include "mx/api/SpannerData.h"
#include "mx/api/StaffDivideData.h"
#include "mx/api/StringMuteData.h"
#include "mx/api/TempoData.h"
#include "mx/api/WedgeData.h"
#include "mx/api/WordsChoice.h"

#include <variant>
#include <vector>

namespace mx
{
namespace api
{
// One piece of a direction's content: a tempo mark, a dynamic, one end of a wedge, a run of
// words and symbols, and so on. A MusicXML <direction> holds an ordered sequence of
// <direction-type> elements; DirectionData::directionTypes holds one DirectionChoice per
// direction-type, in that order. Order is meaningful -- consecutive direction-types read as one
// visual sequence (e.g. the words "più" followed by a forte dynamic).
//
// The wordsRun alternative is the one compound case: a single <direction-type> may hold an
// ordered mix of <words> and <symbol> elements ("gliss." followed by an arrow glyph), so the
// run is a vector of WordsChoice items kept together in one alternative. A run of one plain
// WordsData is simply a vector of one.
//
// bracketStart/bracketStop and dashesStart/dashesStop share the SpannerStart/SpannerStop
// payload types, so those four alternatives are built with the static factory functions below
// rather than constructors.
//
// Defaults to tempo, default-constructed.
class DirectionChoice
{
  public:
    enum class Kind
    {
        tempo,
        mark,
        wedgeStart,
        wedgeStop,
        ottavaStart,
        ottavaStop,
        bracketStart,
        bracketStop,
        dashesStart,
        dashesStop,
        pedal,
        wordsRun,
        segno,
        coda,
        rehearsal,
        damp,
        dampAll,
        eyeglasses,
        stringMute,
        staffDivide,
        principalVoice,
        otherDirection,
        image,
        accordionRegistration,
        harpPedals,
        scordatura,
        percussion
    };

    DirectionChoice();

    explicit DirectionChoice(TempoData value);
    explicit DirectionChoice(MarkData value);
    explicit DirectionChoice(WedgeStart value);
    explicit DirectionChoice(WedgeStop value);
    explicit DirectionChoice(OttavaStart value);
    explicit DirectionChoice(OttavaStop value);
    explicit DirectionChoice(PedalLineData value);
    explicit DirectionChoice(std::vector<WordsChoice> value);
    explicit DirectionChoice(SegnoData value);
    explicit DirectionChoice(CodaData value);
    explicit DirectionChoice(RehearsalData value);
    explicit DirectionChoice(DampData value);
    explicit DirectionChoice(DampAllData value);
    explicit DirectionChoice(EyeglassesData value);
    explicit DirectionChoice(StringMuteData value);
    explicit DirectionChoice(StaffDivideData value);
    explicit DirectionChoice(PrincipalVoiceData value);
    explicit DirectionChoice(OtherDirectionData value);
    explicit DirectionChoice(ImageData value);
    explicit DirectionChoice(AccordionRegistrationData value);
    explicit DirectionChoice(HarpPedalsData value);
    explicit DirectionChoice(ScordaturaData value);
    explicit DirectionChoice(PercussionData value);

    // A bracket line's start, MusicXML's <bracket type="start">.
    static DirectionChoice bracketStart(SpannerStart value);

    // A bracket line's stop, MusicXML's <bracket type="stop">.
    static DirectionChoice bracketStop(SpannerStop value);

    // A dashed line's start, MusicXML's <dashes type="start">.
    static DirectionChoice dashesStart(SpannerStart value);

    // A dashed line's stop, MusicXML's <dashes type="stop">.
    static DirectionChoice dashesStop(SpannerStop value);

    Kind kind() const;
    bool isTempo() const;
    bool isMark() const;
    bool isWedgeStart() const;
    bool isWedgeStop() const;
    bool isOttavaStart() const;
    bool isOttavaStop() const;
    bool isBracketStart() const;
    bool isBracketStop() const;
    bool isDashesStart() const;
    bool isDashesStop() const;
    bool isPedal() const;
    bool isWordsRun() const;
    bool isSegno() const;
    bool isCoda() const;
    bool isRehearsal() const;
    bool isDamp() const;
    bool isDampAll() const;
    bool isEyeglasses() const;
    bool isStringMute() const;
    bool isStaffDivide() const;
    bool isPrincipalVoice() const;
    bool isOtherDirection() const;
    bool isImage() const;
    bool isAccordionRegistration() const;
    bool isHarpPedals() const;
    bool isScordatura() const;
    bool isPercussion() const;

    // Each accessor returns a copy of the internally held value. Check the matching is...()
    // first; when this choice holds a different kind, a default constructed value is returned.
    const TempoData tempo() const;
    const MarkData mark() const;
    const WedgeStart wedgeStart() const;
    const WedgeStop wedgeStop() const;
    const OttavaStart ottavaStart() const;
    const OttavaStop ottavaStop() const;
    const SpannerStart bracketStart() const;
    const SpannerStop bracketStop() const;
    const SpannerStart dashesStart() const;
    const SpannerStop dashesStop() const;
    const PedalLineData pedal() const;
    const std::vector<WordsChoice> wordsRun() const;
    const SegnoData segno() const;
    const CodaData coda() const;
    const RehearsalData rehearsal() const;
    const DampData damp() const;
    const DampAllData dampAll() const;
    const EyeglassesData eyeglasses() const;
    const StringMuteData stringMute() const;
    const StaffDivideData staffDivide() const;
    const PrincipalVoiceData principalVoice() const;
    const OtherDirectionData otherDirection() const;
    const ImageData image() const;
    const AccordionRegistrationData accordionRegistration() const;
    const HarpPedalsData harpPedals() const;
    const ScordaturaData scordatura() const;
    const PercussionData percussion() const;

    bool operator==(const DirectionChoice &other) const;

  private:
    // Alternatives are in Kind order; kind() is the variant index. SpannerStart/SpannerStop each
    // appear twice (bracket and dashes), so dispatch is by index, never by type.
    using Storage = std::variant<TempoData, MarkData, WedgeStart, WedgeStop, OttavaStart, OttavaStop, SpannerStart,
                                 SpannerStop, SpannerStart, SpannerStop, PedalLineData, std::vector<WordsChoice>,
                                 SegnoData, CodaData, RehearsalData, DampData, DampAllData, EyeglassesData,
                                 StringMuteData, StaffDivideData, PrincipalVoiceData, OtherDirectionData, ImageData,
                                 AccordionRegistrationData, HarpPedalsData, ScordaturaData, PercussionData>;

    explicit DirectionChoice(Storage value);

    Storage myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(DirectionChoice);

} // namespace api
} // namespace mx
