// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/DirectionChoice.h"

#include <utility>

namespace mx
{
namespace api
{

DirectionChoice::DirectionChoice() : myValue{TempoData{}}
{
}

DirectionChoice::DirectionChoice(Storage value) : myValue{std::move(value)}
{
}

DirectionChoice::DirectionChoice(TempoData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::tempo)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(MarkData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::mark)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(WedgeStart value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::wedgeStart)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(WedgeStop value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::wedgeStop)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(OttavaStart value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::ottavaStart)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(OttavaStop value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::ottavaStop)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(PedalLineData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::pedal)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(std::vector<WordsChoice> value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::wordsRun)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(SegnoData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::segno)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(CodaData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::coda)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(RehearsalData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::rehearsal)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(DampData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::damp)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(DampAllData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::dampAll)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(EyeglassesData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::eyeglasses)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(StringMuteData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::stringMute)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(StaffDivideData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::staffDivide)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(PrincipalVoiceData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::principalVoice)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(OtherDirectionData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::otherDirection)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(ImageData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::image)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(AccordionRegistrationData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::accordionRegistration)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(HarpPedalsData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::harpPedals)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(ScordaturaData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::scordatura)>, std::move(value)}
{
}

DirectionChoice::DirectionChoice(PercussionData value)
    : myValue{std::in_place_index<static_cast<std::size_t>(Kind::percussion)>, std::move(value)}
{
}

DirectionChoice DirectionChoice::bracketStart(SpannerStart value)
{
    return DirectionChoice{
        Storage{std::in_place_index<static_cast<std::size_t>(Kind::bracketStart)>, std::move(value)}};
}

DirectionChoice DirectionChoice::bracketStop(SpannerStop value)
{
    return DirectionChoice{Storage{std::in_place_index<static_cast<std::size_t>(Kind::bracketStop)>, std::move(value)}};
}

DirectionChoice DirectionChoice::dashesStart(SpannerStart value)
{
    return DirectionChoice{Storage{std::in_place_index<static_cast<std::size_t>(Kind::dashesStart)>, std::move(value)}};
}

DirectionChoice DirectionChoice::dashesStop(SpannerStop value)
{
    return DirectionChoice{Storage{std::in_place_index<static_cast<std::size_t>(Kind::dashesStop)>, std::move(value)}};
}

DirectionChoice::Kind DirectionChoice::kind() const
{
    return static_cast<Kind>(myValue.index());
}

bool DirectionChoice::isTempo() const
{
    return kind() == Kind::tempo;
}

bool DirectionChoice::isMark() const
{
    return kind() == Kind::mark;
}

bool DirectionChoice::isWedgeStart() const
{
    return kind() == Kind::wedgeStart;
}

bool DirectionChoice::isWedgeStop() const
{
    return kind() == Kind::wedgeStop;
}

bool DirectionChoice::isOttavaStart() const
{
    return kind() == Kind::ottavaStart;
}

bool DirectionChoice::isOttavaStop() const
{
    return kind() == Kind::ottavaStop;
}

bool DirectionChoice::isBracketStart() const
{
    return kind() == Kind::bracketStart;
}

bool DirectionChoice::isBracketStop() const
{
    return kind() == Kind::bracketStop;
}

bool DirectionChoice::isDashesStart() const
{
    return kind() == Kind::dashesStart;
}

bool DirectionChoice::isDashesStop() const
{
    return kind() == Kind::dashesStop;
}

bool DirectionChoice::isPedal() const
{
    return kind() == Kind::pedal;
}

bool DirectionChoice::isWordsRun() const
{
    return kind() == Kind::wordsRun;
}

bool DirectionChoice::isSegno() const
{
    return kind() == Kind::segno;
}

bool DirectionChoice::isCoda() const
{
    return kind() == Kind::coda;
}

bool DirectionChoice::isRehearsal() const
{
    return kind() == Kind::rehearsal;
}

bool DirectionChoice::isDamp() const
{
    return kind() == Kind::damp;
}

bool DirectionChoice::isDampAll() const
{
    return kind() == Kind::dampAll;
}

bool DirectionChoice::isEyeglasses() const
{
    return kind() == Kind::eyeglasses;
}

bool DirectionChoice::isStringMute() const
{
    return kind() == Kind::stringMute;
}

bool DirectionChoice::isStaffDivide() const
{
    return kind() == Kind::staffDivide;
}

bool DirectionChoice::isPrincipalVoice() const
{
    return kind() == Kind::principalVoice;
}

bool DirectionChoice::isOtherDirection() const
{
    return kind() == Kind::otherDirection;
}

bool DirectionChoice::isImage() const
{
    return kind() == Kind::image;
}

bool DirectionChoice::isAccordionRegistration() const
{
    return kind() == Kind::accordionRegistration;
}

bool DirectionChoice::isHarpPedals() const
{
    return kind() == Kind::harpPedals;
}

bool DirectionChoice::isScordatura() const
{
    return kind() == Kind::scordatura;
}

bool DirectionChoice::isPercussion() const
{
    return kind() == Kind::percussion;
}

const TempoData DirectionChoice::tempo() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::tempo)>(&myValue))
    {
        return *value;
    }
    return TempoData{};
}

const MarkData DirectionChoice::mark() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::mark)>(&myValue))
    {
        return *value;
    }
    return MarkData{};
}

const WedgeStart DirectionChoice::wedgeStart() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::wedgeStart)>(&myValue))
    {
        return *value;
    }
    return WedgeStart{};
}

const WedgeStop DirectionChoice::wedgeStop() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::wedgeStop)>(&myValue))
    {
        return *value;
    }
    return WedgeStop{};
}

const OttavaStart DirectionChoice::ottavaStart() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::ottavaStart)>(&myValue))
    {
        return *value;
    }
    return OttavaStart{};
}

const OttavaStop DirectionChoice::ottavaStop() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::ottavaStop)>(&myValue))
    {
        return *value;
    }
    return OttavaStop{};
}

const SpannerStart DirectionChoice::bracketStart() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::bracketStart)>(&myValue))
    {
        return *value;
    }
    return SpannerStart{};
}

const SpannerStop DirectionChoice::bracketStop() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::bracketStop)>(&myValue))
    {
        return *value;
    }
    return SpannerStop{};
}

const SpannerStart DirectionChoice::dashesStart() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::dashesStart)>(&myValue))
    {
        return *value;
    }
    return SpannerStart{};
}

const SpannerStop DirectionChoice::dashesStop() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::dashesStop)>(&myValue))
    {
        return *value;
    }
    return SpannerStop{};
}

const PedalLineData DirectionChoice::pedal() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::pedal)>(&myValue))
    {
        return *value;
    }
    return PedalLineData{};
}

const std::vector<WordsChoice> DirectionChoice::wordsRun() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::wordsRun)>(&myValue))
    {
        return *value;
    }
    return std::vector<WordsChoice>{};
}

const SegnoData DirectionChoice::segno() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::segno)>(&myValue))
    {
        return *value;
    }
    return SegnoData{};
}

const CodaData DirectionChoice::coda() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::coda)>(&myValue))
    {
        return *value;
    }
    return CodaData{};
}

const RehearsalData DirectionChoice::rehearsal() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::rehearsal)>(&myValue))
    {
        return *value;
    }
    return RehearsalData{};
}

const DampData DirectionChoice::damp() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::damp)>(&myValue))
    {
        return *value;
    }
    return DampData{};
}

const DampAllData DirectionChoice::dampAll() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::dampAll)>(&myValue))
    {
        return *value;
    }
    return DampAllData{};
}

const EyeglassesData DirectionChoice::eyeglasses() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::eyeglasses)>(&myValue))
    {
        return *value;
    }
    return EyeglassesData{};
}

const StringMuteData DirectionChoice::stringMute() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::stringMute)>(&myValue))
    {
        return *value;
    }
    return StringMuteData{};
}

const StaffDivideData DirectionChoice::staffDivide() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::staffDivide)>(&myValue))
    {
        return *value;
    }
    return StaffDivideData{};
}

const PrincipalVoiceData DirectionChoice::principalVoice() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::principalVoice)>(&myValue))
    {
        return *value;
    }
    return PrincipalVoiceData{};
}

const OtherDirectionData DirectionChoice::otherDirection() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::otherDirection)>(&myValue))
    {
        return *value;
    }
    return OtherDirectionData{};
}

const ImageData DirectionChoice::image() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::image)>(&myValue))
    {
        return *value;
    }
    return ImageData{};
}

const AccordionRegistrationData DirectionChoice::accordionRegistration() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::accordionRegistration)>(&myValue))
    {
        return *value;
    }
    return AccordionRegistrationData{};
}

const HarpPedalsData DirectionChoice::harpPedals() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::harpPedals)>(&myValue))
    {
        return *value;
    }
    return HarpPedalsData{};
}

const ScordaturaData DirectionChoice::scordatura() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::scordatura)>(&myValue))
    {
        return *value;
    }
    return ScordaturaData{};
}

const PercussionData DirectionChoice::percussion() const
{
    if (const auto *value = std::get_if<static_cast<std::size_t>(Kind::percussion)>(&myValue))
    {
        return *value;
    }
    return PercussionData{};
}

bool DirectionChoice::operator==(const DirectionChoice &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
