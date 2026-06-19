// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DirectionReader.h"
#include "mx/api/WedgeData.h"
#include "mx/core/generated/AccordionRegistration.h"
#include "mx/core/generated/Barre.h"
#include "mx/core/generated/Bass.h"
#include "mx/core/generated/BassStep.h"
#include "mx/core/generated/Bracket.h"
#include "mx/core/generated/Coda.h"
#include "mx/core/generated/Dashes.h"
#include "mx/core/generated/Degree.h"
#include "mx/core/generated/DegreeAlter.h"
#include "mx/core/generated/DegreeType.h"
#include "mx/core/generated/DegreeTypeValue.h"
#include "mx/core/generated/DegreeValue.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/DirectionType.h"
#include "mx/core/generated/DirectionTypeChoice.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/Fingering.h"
#include "mx/core/generated/FirstFret.h"
#include "mx/core/generated/Frame.h"
#include "mx/core/generated/FrameNote.h"
#include "mx/core/generated/Fret.h"
#include "mx/core/generated/HarmonyAlter.h"
#include "mx/core/generated/HarmonyChordGroup.h"
#include "mx/core/generated/HarmonyChordGroupChoice.h"
#include "mx/core/generated/HarpPedals.h"
#include "mx/core/generated/Image.h"
#include "mx/core/generated/Inversion.h"
#include "mx/core/generated/Kind.h"
#include "mx/core/generated/KindValue.h"
#include "mx/core/generated/Metronome.h"
#include "mx/core/generated/Numeral.h"
#include "mx/core/generated/NumeralKey.h"
#include "mx/core/generated/NumeralMode.h"
#include "mx/core/generated/NumeralRoot.h"
#include "mx/core/generated/NumeralValue.h"
#include "mx/core/generated/OctaveShift.h"
#include "mx/core/generated/Offset.h"
#include "mx/core/generated/OtherDirection.h"
#include "mx/core/generated/Pedal.h"
#include "mx/core/generated/PedalType.h"
#include "mx/core/generated/Percussion.h"
#include "mx/core/generated/PrincipalVoice.h"
#include "mx/core/generated/Root.h"
#include "mx/core/generated/RootStep.h"
#include "mx/core/generated/Scordatura.h"
#include "mx/core/generated/Segno.h"
#include "mx/core/generated/Sound.h"
#include "mx/core/generated/StartStop.h"
#include "mx/core/generated/StartStopContinue.h"
#include "mx/core/generated/Step.h"
#include "mx/core/generated/String.h"
#include "mx/core/generated/StringMute.h"
#include "mx/core/generated/StyleText.h"
#include "mx/core/generated/UpDownStopContinue.h"
#include "mx/core/generated/Wedge.h"
#include "mx/core/generated/WedgeType.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/DynamicsReader.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/MetronomeReader.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/SoundFunctions.h"
#include "mx/impl/SpannerFunctions.h"
#include "mx/utility/Round.h"
#include "mx/utility/Unused.h"

namespace mx
{
namespace impl
{
DirectionReader::DirectionReader(const core::Direction &inDirection, Cursor inCursor)
    : myDirection{&inDirection}, myHarmony{nullptr}, myCursor{inCursor}, myConverter{}, myOutDirectionData{}
{
}

DirectionReader::DirectionReader(const core::Harmony &inHarmony, Cursor inCursor)
    : myDirection{nullptr}, myHarmony{&inHarmony}, myCursor{inCursor}, myConverter{}, myOutDirectionData{}
{
}

api::DirectionData DirectionReader::getDirectionData()
{
    myOutDirectionData = initializeData();
    updateTimeForOffset();
    parsePlacement();
    parseValues();

    // need to make sure all the MarkData item times match the DirectionData time
    fixTimes();
    return returnData();
}

mx::api::DirectionData DirectionReader::initializeData()
{
    auto result = api::DirectionData{};
    result.tickTimePosition = myCursor.tickTimePosition;

    if (myDirection)
    {
        result.isStaffValueSpecified = myDirection->staff().has_value();
    }
    else if (myHarmony)
    {
        result.isStaffValueSpecified = myHarmony->staff().has_value();
    }

    return result;
}

void DirectionReader::updateTimeForOffset()
{
    if (myDirection)
    {
        if (myDirection->offset().has_value())
        {
            const auto rawVal = myDirection->offset()->value().value().value();
            const auto offset = static_cast<int>(std::ceil(rawVal - 0.5));
            myOutDirectionData.tickTimePosition += offset;
        }
    }
    else if (myHarmony)
    {
        if (myHarmony->offset().has_value())
        {
            const auto rawVal = myHarmony->offset()->value().value().value();
            const auto offset = static_cast<int>(std::ceil(rawVal - 0.5));
            myOutDirectionData.tickTimePosition += offset;
        }
    }
}

void DirectionReader::parsePlacement()
{
    if (myDirection)
    {
        if (myDirection->placement().has_value())
        {
            myOutDirectionData.placement = myConverter.convert(*myDirection->placement());
        }
    }
    else if (myHarmony)
    {
        if (myHarmony->placement().has_value())
        {
            myOutDirectionData.placement = myConverter.convert(*myHarmony->placement());
        }
    }
}

void DirectionReader::parseValues()
{
    if (myDirection)
    {
        for (const auto &dt : myDirection->directionType())
        {
            parseDirectionType(dt);
        }

        if (myDirection->sound().has_value())
        {
            auto soundData = readSoundData(*myDirection->sound());
            if (soundData.isSpecified())
            {
                myOutDirectionData.isSoundDataSpecified = true;
                myOutDirectionData.soundData = std::move(soundData);
            }
        }
    }
    else if (myHarmony)
    {
        for (const auto &hcg : myHarmony->harmonyChord())
        {
            parseHarmony(*myHarmony, hcg);
        }
    }
}

void DirectionReader::fixTimes()
{
    for (auto &mark : myOutDirectionData.marks)
    {
        mark.tickTimePosition = myOutDirectionData.tickTimePosition;
    }
}

mx::api::DirectionData DirectionReader::returnData()
{
    api::DirectionData temp{std::move(myOutDirectionData)};
    myOutDirectionData = api::DirectionData{};
    return temp;
}

void DirectionReader::appendOrderedComponent(api::DirectionComponentKind kind, int index)
{
    myOutDirectionData.orderedComponents.emplace_back(kind, index);
}

void DirectionReader::parseDirectionType(const core::DirectionType &directionType)
{
    const auto &ch = directionType.choice();
    using K = core::DirectionTypeChoice::Kind;

    switch (ch.kind())
    {
    case K::rehearsal: {
        parseRehearsal(directionType);
        break;
    }
    case K::segno: {
        parseSegno(directionType);
        break;
    }
    case K::choice: {
        parseWords(directionType);
        break;
    }
    case K::coda: {
        parseCoda(directionType);
        break;
    }
    case K::wedge: {
        parseWedge(directionType);
        break;
    }
    case K::dynamics: {
        parseDynamics(directionType);
        break;
    }
    case K::dashes: {
        parseDashes(directionType);
        break;
    }
    case K::bracket: {
        parseBracket(directionType);
        break;
    }
    case K::pedal: {
        parsePedal(directionType);
        break;
    }
    case K::metronome: {
        parseMetronome(directionType);
        break;
    }
    case K::octaveShift: {
        parseOctaveShift(directionType);
        break;
    }
    case K::harpPedals: {
        parseHarpPedals(directionType);
        break;
    }
    case K::damp: {
        parseDamp(directionType);
        break;
    }
    case K::dampAll: {
        parseDampAll(directionType);
        break;
    }
    case K::eyeglasses: {
        parseEyeglasses(directionType);
        break;
    }
    case K::stringMute: {
        parseStringMute(directionType);
        break;
    }
    case K::scordatura: {
        parseScordatura(directionType);
        break;
    }
    case K::image: {
        parseImage(directionType);
        break;
    }
    case K::principalVoice: {
        parsePrincipalVoice(directionType);
        break;
    }
    case K::accordionRegistration: {
        parseAccordionRegistration(directionType);
        break;
    }
    case K::percussion: {
        parsePercussion(directionType);
        break;
    }
    case K::staffDivide: {
        /* unhandled - new in MusicXML 4.0 */
        break;
    }
    case K::otherDirection: {
        parseOtherDirection(directionType);
        break;
    }
    default:
        break;
    }
}

void DirectionReader::parseRehearsal(const core::DirectionType &directionType)
{
    const auto &rehearsalSet = directionType.choice().asRehearsal();

    for (const auto &rehearsal : rehearsalSet.items())
    {
        api::RehearsalData outRehearsal;
        outRehearsal.text = rehearsal.value();
        outRehearsal.positionData = getPositionData(rehearsal);
        outRehearsal.colorData = getColor(rehearsal);
        if (rehearsal.enclosure().has_value())
        {
            switch (rehearsal.enclosure()->tag())
            {
            case core::EnclosureShape::Tag::rectangle:
                outRehearsal.enclosure = api::RehearsalEnclosure::rectangle;
                break;
            case core::EnclosureShape::Tag::square:
                outRehearsal.enclosure = api::RehearsalEnclosure::square;
                break;
            case core::EnclosureShape::Tag::oval:
                outRehearsal.enclosure = api::RehearsalEnclosure::oval;
                break;
            case core::EnclosureShape::Tag::circle:
                outRehearsal.enclosure = api::RehearsalEnclosure::circle;
                break;
            case core::EnclosureShape::Tag::bracket:
                outRehearsal.enclosure = api::RehearsalEnclosure::bracket;
                break;
            case core::EnclosureShape::Tag::triangle:
                outRehearsal.enclosure = api::RehearsalEnclosure::triangle;
                break;
            case core::EnclosureShape::Tag::diamond:
                outRehearsal.enclosure = api::RehearsalEnclosure::diamond;
                break;
            case core::EnclosureShape::Tag::none:
                outRehearsal.enclosure = api::RehearsalEnclosure::none;
                break;
            default:
                break;
            }
        }
        myOutDirectionData.rehearsals.emplace_back(std::move(outRehearsal));
        appendOrderedComponent(api::DirectionComponentKind::rehearsal,
                               static_cast<int>(myOutDirectionData.rehearsals.size()) - 1);
    }
}

void DirectionReader::parseSegno(const core::DirectionType &directionType)
{
    const auto &segnoSet = directionType.choice().asSegno();

    for (const auto &segno : segnoSet.items())
    {
        api::SegnoData outSegno;
        outSegno.positionData = getPositionData(segno);
        outSegno.fontData = getFontData(segno);
        outSegno.isColorSpecified = segno.color().has_value();
        if (outSegno.isColorSpecified)
        {
            outSegno.colorData = getColor(segno);
        }
        if (segno.smufl().has_value())
        {
            outSegno.isSmuflSpecified = true;
            outSegno.smufl = segno.smufl()->toString();
        }
        if (segno.id().has_value())
        {
            outSegno.isIdSpecified = true;
            outSegno.id = segno.id()->value();
        }
        myOutDirectionData.segnos.emplace_back(std::move(outSegno));
        appendOrderedComponent(api::DirectionComponentKind::segno,
                               static_cast<int>(myOutDirectionData.segnos.size()) - 1);
    }
}

void DirectionReader::parseWords(const core::DirectionType &directionType)
{
    // In the new core, "words" is inside the `choice` (DirectionTypeChoiceChoice) alternative.
    // Each element is either words (FormattedTextID) or symbol (FormattedSymbolID).
    const auto &choiceItems = directionType.choice().asChoice();

    for (const auto &item : choiceItems.items())
    {
        if (!item.isWords())
        {
            continue;
        }
        const auto &wordEl = item.asWords();
        api::WordsData outWords;
        outWords.text = wordEl.value();
        outWords.positionData = getPositionData(wordEl);
        outWords.colorData = getColor(wordEl);
        outWords.fontData = getFontData(wordEl);
        if (wordEl.enclosure().has_value())
        {
            switch (wordEl.enclosure()->tag())
            {
            case core::EnclosureShape::Tag::rectangle:
                outWords.enclosure = api::RehearsalEnclosure::rectangle;
                break;
            case core::EnclosureShape::Tag::square:
                outWords.enclosure = api::RehearsalEnclosure::square;
                break;
            case core::EnclosureShape::Tag::oval:
                outWords.enclosure = api::RehearsalEnclosure::oval;
                break;
            case core::EnclosureShape::Tag::circle:
                outWords.enclosure = api::RehearsalEnclosure::circle;
                break;
            case core::EnclosureShape::Tag::bracket:
                outWords.enclosure = api::RehearsalEnclosure::bracket;
                break;
            case core::EnclosureShape::Tag::triangle:
                outWords.enclosure = api::RehearsalEnclosure::triangle;
                break;
            case core::EnclosureShape::Tag::diamond:
                outWords.enclosure = api::RehearsalEnclosure::diamond;
                break;
            case core::EnclosureShape::Tag::none:
                outWords.enclosure = api::RehearsalEnclosure::none;
                break;
            default:
                break;
            }
        }
        myOutDirectionData.words.emplace_back(std::move(outWords));
        appendOrderedComponent(api::DirectionComponentKind::words,
                               static_cast<int>(myOutDirectionData.words.size()) - 1);
    }
}

void DirectionReader::parseCoda(const core::DirectionType &directionType)
{
    const auto &codaSet = directionType.choice().asCoda();

    for (const auto &coda : codaSet.items())
    {
        api::CodaData outCoda;
        outCoda.positionData = getPositionData(coda);
        outCoda.fontData = getFontData(coda);
        outCoda.isColorSpecified = coda.color().has_value();
        if (outCoda.isColorSpecified)
        {
            outCoda.colorData = getColor(coda);
        }
        if (coda.smufl().has_value())
        {
            outCoda.isSmuflSpecified = true;
            outCoda.smufl = coda.smufl()->toString();
        }
        if (coda.id().has_value())
        {
            outCoda.isIdSpecified = true;
            outCoda.id = coda.id()->value();
        }
        myOutDirectionData.codas.emplace_back(std::move(outCoda));
        appendOrderedComponent(api::DirectionComponentKind::coda,
                               static_cast<int>(myOutDirectionData.codas.size()) - 1);
    }
}

void DirectionReader::parseWedge(const core::DirectionType &directionType)
{
    const auto &wedge = directionType.choice().asWedge();
    const auto wedgeType = myConverter.convert(wedge.type());
    const bool isSpreadSpecified = wedge.spread().has_value();
    const double spread = isSpreadSpecified ? static_cast<double>(wedge.spread()->value().value()) : 0.0;
    auto positionData = getPositionData(wedge);
    auto lineData = getLineData(wedge);
    auto colorData = getColor(wedge);

    if (wedge.type().tag() == core::WedgeType::Tag::stop)
    {
        api::WedgeStop stop;
        if (wedge.number().has_value())
        {
            stop.numberLevel = wedge.number()->value();
        }
        if (isSpreadSpecified)
        {
            stop.isSpreadSpecified = true;
            stop.spread = spread;
        }
        stop.positionData = positionData;
        myOutDirectionData.wedgeStops.emplace_back(std::move(stop));
        appendOrderedComponent(api::DirectionComponentKind::wedgeStop,
                               static_cast<int>(myOutDirectionData.wedgeStops.size()) - 1);
        return;
    }
    else
    {
        api::WedgeStart start;
        if (wedge.number().has_value())
        {
            start.numberLevel = wedge.number()->value();
        }
        if (isSpreadSpecified)
        {
            start.isSpreadSpecified = true;
            start.spread = spread;
        }
        start.wedgeType = wedgeType;
        start.positionData = positionData;
        start.lineData = lineData;
        start.colorData = colorData;
        myOutDirectionData.wedgeStarts.emplace_back(std::move(start));
        appendOrderedComponent(api::DirectionComponentKind::wedgeStart,
                               static_cast<int>(myOutDirectionData.wedgeStarts.size()) - 1);
    }
}

void DirectionReader::parseDynamics(const core::DirectionType &directionType)
{
    const auto markCountBefore = myOutDirectionData.marks.size();
    for (const auto &dynamic : directionType.choice().asDynamics().items())
    {
        DynamicsReader reader{dynamic, myCursor};
        reader.parseDynamics(myOutDirectionData.marks);
    }
    for (auto i = markCountBefore; i < myOutDirectionData.marks.size(); ++i)
    {
        appendOrderedComponent(api::DirectionComponentKind::mark, static_cast<int>(i));
    }
}

void DirectionReader::parseDynamic(const core::Dynamics &dynamic)
{
    auto mark = api::MarkData{};
    const auto &choices = dynamic.choice();
    if (choices.empty())
    {
        return;
    }
    const auto &firstChoice = choices.front();
    const auto kind = firstChoice.kind();
    mark.markType = myConverter.convertDynamic(kind);
    if (kind == core::DynamicsChoice::Kind::otherDynamics)
    {
        mark.name = firstChoice.asOtherDynamics().value();
    }
    else
    {
        // use the DynamicsReader helper for consistent name
        DynamicsReader reader{dynamic, myCursor};
        std::vector<api::MarkData> tmp;
        reader.parseDynamics(tmp);
        if (!tmp.empty())
        {
            mark.name = tmp.front().name;
        }
    }
    impl::parseMarkDataAttributes(dynamic, mark);

    auto placement = mark.positionData.placement;

    if (placement == api::Placement::unspecified)
    {
        placement = myOutDirectionData.placement;
    }

    myOutDirectionData.marks.emplace_back(std::move(mark));
    appendOrderedComponent(api::DirectionComponentKind::mark, static_cast<int>(myOutDirectionData.marks.size()) - 1);
}

void DirectionReader::parseDashes(const core::DirectionType &directionType)
{
    const auto &dashes = directionType.choice().asDashes();

    if (dashes.type().tag() == core::StartStopContinue::Tag::stop)
    {
        auto stop = impl::getSpannerStop(dashes);
        stop.tickTimePosition = myCursor.tickTimePosition;
        myOutDirectionData.dashesStops.emplace_back(std::move(stop));
        appendOrderedComponent(api::DirectionComponentKind::dashesStop,
                               static_cast<int>(myOutDirectionData.dashesStops.size()) - 1);
        return;
    }
    else if (dashes.type().tag() == core::StartStopContinue::Tag::start)
    {
        auto start = impl::getSpannerStart(dashes);
        start.tickTimePosition = myCursor.tickTimePosition;
        if (start.lineData.lineType == api::LineType::unspecified)
        {
            start.lineData.lineType = api::LineType::dashed;
        }
        myOutDirectionData.dashesStarts.emplace_back(std::move(start));
        appendOrderedComponent(api::DirectionComponentKind::dashesStart,
                               static_cast<int>(myOutDirectionData.dashesStarts.size()) - 1);
        return;
    }
}

void DirectionReader::parseBracket(const core::DirectionType &directionType)
{
    const auto &bracket = directionType.choice().asBracket();

    const auto makeBracketLineData = [&]() {
        api::LineData lineData{};
        lineData.lineHook = myConverter.convert(bracket.lineEnd());

        if (bracket.lineType().has_value())
        {
            lineData.lineType = myConverter.convert(*bracket.lineType());
        }
        if (bracket.endLength().has_value())
        {
            lineData.isStopLengthSpecified = true;
            lineData.endLength = static_cast<double>(bracket.endLength()->value().value());
        }
        if (bracket.dashLength().has_value())
        {
            lineData.isDashLengthSpecified = true;
            lineData.dashLength = static_cast<double>(bracket.dashLength()->value().value());
        }
        if (bracket.spaceLength().has_value())
        {
            lineData.isSpaceLengthSpecified = true;
            lineData.spaceLength = static_cast<double>(bracket.spaceLength()->value().value());
        }
        return lineData;
    };

    if (bracket.type().tag() == core::StartStopContinue::Tag::stop)
    {
        auto stop = impl::getSpannerStop(bracket);
        stop.tickTimePosition = myCursor.tickTimePosition;
        stop.lineData = makeBracketLineData();
        myOutDirectionData.bracketStops.emplace_back(std::move(stop));
        appendOrderedComponent(api::DirectionComponentKind::bracketStop,
                               static_cast<int>(myOutDirectionData.bracketStops.size()) - 1);
        return;
    }
    else if (bracket.type().tag() == core::StartStopContinue::Tag::start)
    {
        api::SpannerStart start;
        start.tickTimePosition = myCursor.tickTimePosition;
        start.numberLevel = impl::checkNumber(&bracket);
        start.positionData = this->parsePositionData(bracket);
        start.lineData = makeBracketLineData();
        start.printData = impl::getPrintData(bracket);
        myOutDirectionData.bracketStarts.emplace_back(std::move(start));
        appendOrderedComponent(api::DirectionComponentKind::bracketStart,
                               static_cast<int>(myOutDirectionData.bracketStarts.size()) - 1);
        return;
    }
}

void DirectionReader::parsePedal(const core::DirectionType &directionType)
{
    const auto &pedal = directionType.choice().asPedal();

    if (pedal.type().tag() != core::PedalType::Tag::start && pedal.type().tag() != core::PedalType::Tag::stop)
    {
        return;
    }

    const auto placement =
        (myDirection && myDirection->placement().has_value())
            ? (myDirection->placement()->tag() == core::AboveBelow::Tag::above ? api::Placement::above
                                                                               : api::Placement::below)
            : api::Placement::unspecified;

    myOutDirectionData.placement = placement;

    if (pedal.line().has_value() && pedal.line()->tag() == core::YesNo::Tag::yes)
    {
        if (pedal.type().tag() == core::PedalType::Tag::start)
        {
            api::SpannerStart start;
            start.tickTimePosition = myOutDirectionData.tickTimePosition;
            start.positionData = getPositionData(pedal);
            start.positionData.placement = placement;
            start.lineData.lineType = api::LineType::solid;
            start.lineData.lineHook = api::LineHook::none;
            myOutDirectionData.pedalStarts.emplace_back(std::move(start));
            appendOrderedComponent(api::DirectionComponentKind::pedalStart,
                                   static_cast<int>(myOutDirectionData.pedalStarts.size()) - 1);
        }
        else if (pedal.type().tag() == core::PedalType::Tag::stop)
        {
            api::SpannerStop stop;
            stop.tickTimePosition = myOutDirectionData.tickTimePosition;
            stop.positionData = getPositionData(pedal);
            stop.positionData.placement = placement;
            stop.lineData.lineType = api::LineType::solid;
            stop.lineData.lineHook = api::LineHook::down;
            stop.lineData.isStopLengthSpecified = true;
            stop.lineData.endLength = 10.0;
            myOutDirectionData.pedalStops.emplace_back(std::move(stop));
            appendOrderedComponent(api::DirectionComponentKind::pedalStop,
                                   static_cast<int>(myOutDirectionData.pedalStops.size()) - 1);
        }
        return;
    }

    auto pedalType = api::MarkType::pedal;

    if (pedal.type().tag() == core::PedalType::Tag::stop)
    {
        pedalType = api::MarkType::damp;
    }

    auto mark = api::MarkData{placement, pedalType};
    mark.positionData = getPositionData(pedal);
    mark.positionData.placement = placement;
    myOutDirectionData.marks.emplace_back(std::move(mark));
    appendOrderedComponent(api::DirectionComponentKind::mark, static_cast<int>(myOutDirectionData.marks.size()) - 1);
}

void DirectionReader::parseMetronome(const core::DirectionType &directionType)
{
    const auto &metronome = directionType.choice().asMetronome();
    MetronomeReaderParameters params{metronome};
    MetronomeReader reader{std::move(params)};
    myOutDirectionData.tempos.emplace_back(reader.getTempoData());
    appendOrderedComponent(api::DirectionComponentKind::tempo, static_cast<int>(myOutDirectionData.tempos.size()) - 1);
}

void DirectionReader::parseOctaveShift(const core::DirectionType &directionType)
{
    const auto &octaveShift = directionType.choice().asOctaveShift();

    if (octaveShift.type().tag() == core::UpDownStopContinue::Tag::continue_)
    {
        // not supported
        return;
    }

    bool isStop = octaveShift.type().tag() == core::UpDownStopContinue::Tag::stop;
    if (isStop)
    {
        auto stop = impl::getSpannerStop(octaveShift);
        stop.tickTimePosition = myCursor.tickTimePosition;
        myOutDirectionData.ottavaStops.emplace_back(std::move(stop));
        appendOrderedComponent(api::DirectionComponentKind::ottavaStop,
                               static_cast<int>(myOutDirectionData.ottavaStops.size()) - 1);
        return;
    }

    auto ottavaType = api::OttavaType::unspecified;
    int amount = 8;

    if (octaveShift.size().has_value())
    {
        amount = *octaveShift.size();
    }

    bool isUp = octaveShift.type().tag() == core::UpDownStopContinue::Tag::up;

    if (isUp && amount > 8)
    {
        ottavaType = api::OttavaType::o15ma;
    }
    else if (isUp)
    {
        ottavaType = api::OttavaType::o8va;
    }
    else if (!isUp && amount > 8)
    {
        ottavaType = api::OttavaType::o15mb;
    }
    else if (!isUp)
    {
        ottavaType = api::OttavaType::o8vb;
    }

    api::OttavaStart start;
    start.spannerStart = impl::getSpannerStart(octaveShift);
    start.ottavaType = ottavaType;
    start.spannerStart.tickTimePosition = myCursor.tickTimePosition;
    myOutDirectionData.ottavaStarts.emplace_back(std::move(start));
    appendOrderedComponent(api::DirectionComponentKind::ottavaStart,
                           static_cast<int>(myOutDirectionData.ottavaStarts.size()) - 1);
}

void DirectionReader::parseHarpPedals(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseDamp(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseDampAll(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseEyeglasses(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseStringMute(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseScordatura(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseImage(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parsePrincipalVoice(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseAccordionRegistration(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parsePercussion(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseOtherDirection(const core::DirectionType &directionType)
{
    MX_UNUSED(directionType);
}

void DirectionReader::parseHarmony(const core::Harmony &inHarmony, const core::HarmonyChordGroup &inGrp)
{
    mx::api::ChordData chord;
    const auto &choice = inGrp.choice();

    switch (choice.kind())
    {
    case core::HarmonyChordGroupChoice::Kind::root: {
        chord.harmonyChordSource = api::HarmonyChordSource::root;
        const auto &root = choice.asRoot();
        chord.root = myConverter.convert(root.rootStep().value());

        if (root.rootAlter().has_value())
        {
            chord.rootAlter = mx::utility::roundTo<double, int>(root.rootAlter()->value().value().value());
        }
        break;
    }
    case core::HarmonyChordGroupChoice::Kind::numeral: {
        chord.harmonyChordSource = api::HarmonyChordSource::numeral;
        const auto &numeral = choice.asNumeral();
        chord.numeralRoot = numeral.numeralRoot().value().value();

        if (numeral.numeralRoot().text().has_value())
        {
            chord.numeralRootText = *numeral.numeralRoot().text();
        }

        if (numeral.numeralAlter().has_value())
        {
            chord.hasNumeralAlter = true;
            chord.numeralAlter = mx::utility::roundTo<double, int>(numeral.numeralAlter()->value().value().value());
        }

        if (numeral.numeralKey().has_value())
        {
            chord.hasNumeralKey = true;
            const auto &numeralKey = *numeral.numeralKey();
            chord.numeralKeyFifths = numeralKey.numeralFifths().value();

            switch (numeralKey.numeralMode().tag())
            {
            case core::NumeralMode::Tag::major:
                chord.numeralMode = api::NumeralMode::major;
                break;
            case core::NumeralMode::Tag::minor:
                chord.numeralMode = api::NumeralMode::minor;
                break;
            case core::NumeralMode::Tag::naturalMinor:
                chord.numeralMode = api::NumeralMode::naturalMinor;
                break;
            case core::NumeralMode::Tag::melodicMinor:
                chord.numeralMode = api::NumeralMode::melodicMinor;
                break;
            case core::NumeralMode::Tag::harmonicMinor:
                chord.numeralMode = api::NumeralMode::harmonicMinor;
                break;
            default:
                // A core numeral mode we do not model yet: leave numeralMode unspecified rather
                // than guess. No -Wswitch guard exists to flag a newly added core tag.
                break;
            }
        }
        break;
    }
    case core::HarmonyChordGroupChoice::Kind::function: {
        chord.harmonyChordSource = api::HarmonyChordSource::function;
        chord.functionText = choice.asFunction().value();
        break;
    }
    default:
        // A core harmony-chord kind we do not model yet: leave harmonyChordSource at its default
        // (root) with no pitch data rather than silently misreading. No -Wswitch guard exists to
        // flag a newly added core kind.
        break;
    }

    const auto &kind = inGrp.kind();

    chord.chordKind = myConverter.convert(kind.value());

    if (kind.text().has_value())
    {
        chord.text = *kind.text();
    }

    if (kind.useSymbols().has_value())
    {
        if (kind.useSymbols()->tag() == mx::core::YesNo::Tag::yes)
        {
            chord.useSymbols = api::Bool::yes;
        }
        else
        {
            chord.useSymbols = api::Bool::no;
        }
    }

    if (inGrp.bass().has_value())
    {
        const auto &bass = *inGrp.bass();
        chord.bass = myConverter.convert(bass.bassStep().value());

        if (bass.bassAlter().has_value())
        {
            chord.bassAlter = mx::utility::roundTo<double, int>(bass.bassAlter()->value().value().value());
        }
    }

    if (inGrp.inversion().has_value())
    {
        chord.hasInversion = true;
        chord.inversion = inGrp.inversion()->value();
    }

    const auto &degrees = inGrp.degree();

    for (const auto &degree : degrees)
    {
        api::Extension extension;
        bool doAddExtension = true;

        const auto typeVal = degree.degreeType().value();
        const auto alter = mx::utility::roundTo<double, int>(degree.degreeAlter().value().value().value());
        const auto value = degree.degreeValue().value();

        switch (typeVal.tag())
        {
        case core::DegreeTypeValue::Tag::alter:
            extension.extensionType = api::ExtensionType::alter;
            break;
        case core::DegreeTypeValue::Tag::add:
            extension.extensionType = api::ExtensionType::add;
            break;
        case core::DegreeTypeValue::Tag::subtract:
            extension.extensionType = api::ExtensionType::remove;
            break;
        default:
            break;
        }

        if (alter <= -2)
        {
            extension.extensionAlter = api::ExtensionAlter::doubleFlat;
        }
        else if (alter == -1)
        {
            extension.extensionAlter = api::ExtensionAlter::flat;
        }
        else if (alter == 0)
        {
            extension.extensionAlter = api::ExtensionAlter::none;
        }
        else if (alter == 1)
        {
            extension.extensionAlter = api::ExtensionAlter::sharp;
        }
        else if (alter >= 2)
        {
            extension.extensionAlter = api::ExtensionAlter::doubleSharp;
        }

        switch (value)
        {
        case 1:
            extension.extensionNumber = api::ExtensionNumber::first;
            break;
        case 2:
            extension.extensionNumber = api::ExtensionNumber::second;
            break;
        case 3:
            extension.extensionNumber = api::ExtensionNumber::third;
            break;
        case 4:
            extension.extensionNumber = api::ExtensionNumber::fourth;
            break;
        case 5:
            extension.extensionNumber = api::ExtensionNumber::fifth;
            break;
        case 6:
            extension.extensionNumber = api::ExtensionNumber::sixth;
            break;
        case 7:
            extension.extensionNumber = api::ExtensionNumber::seventh;
            break;
        case 9:
            extension.extensionNumber = api::ExtensionNumber::ninth;
            break;
        case 11:
            extension.extensionNumber = api::ExtensionNumber::eleventh;
            break;
        case 13:
            extension.extensionNumber = api::ExtensionNumber::thirteenth;
            break;
        default:
            doAddExtension = false;
            break;
        }

        if (degree.printObject().has_value())
        {
            if (degree.printObject()->tag() == core::YesNo::Tag::yes)
            {
                extension.printObject = api::Bool::yes;
            }
            else
            {
                extension.printObject = api::Bool::no;
            }
        }

        if (doAddExtension)
        {
            chord.extensions.push_back(extension);
        }
    }

    // ProcessingInstructions are not available in the new generated core — skip.

    if (inHarmony.frame().has_value())
    {
        chord.hasFrameData = true;
        const auto &frame = *inHarmony.frame();
        chord.frameData.stringCount = frame.frameStrings();
        chord.frameData.fretCount = frame.frameFrets();

        if (frame.firstFret().has_value())
        {
            chord.frameData.isFirstFretSpecified = true;
            chord.frameData.firstFret = frame.firstFret()->value();
        }

        for (const auto &frameNote : frame.frameNote())
        {
            api::FrameNoteData frameNoteData;
            frameNoteData.stringNumber = frameNote.string().value().value();
            frameNoteData.fretNumber = frameNote.fret().value();

            if (frameNote.fingering().has_value())
            {
                const auto fingeringText = frameNote.fingering()->value();
                try
                {
                    frameNoteData.fingering = std::stoi(fingeringText);
                    frameNoteData.isFingeringSpecified = true;
                }
                catch (...)
                {
                }
            }

            if (frameNote.barre().has_value())
            {
                const auto barreType = frameNote.barre()->type();
                switch (barreType.tag())
                {
                case core::StartStop::Tag::start:
                    frameNoteData.barre = api::FrameBarre::start;
                    break;
                case core::StartStop::Tag::stop:
                    frameNoteData.barre = api::FrameBarre::stop;
                    break;
                }
            }

            chord.frameData.notes.push_back(frameNoteData);
        }
    }

    chord.positionData = getPositionData(inHarmony);

    myOutDirectionData.chords.push_back(chord);
    appendOrderedComponent(api::DirectionComponentKind::chord, static_cast<int>(myOutDirectionData.chords.size()) - 1);
}
} // namespace impl
} // namespace mx
