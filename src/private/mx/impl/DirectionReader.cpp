// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DirectionReader.h"
#include "mx/api/WedgeData.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Barre.h"
#include "mx/core/elements/Bass.h"
#include "mx/core/elements/BassAlter.h"
#include "mx/core/elements/BassStep.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Degree.h"
#include "mx/core/elements/DegreeAlter.h"
#include "mx/core/elements/DegreeType.h"
#include "mx/core/elements/DegreeValue.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/EditorialVoiceDirectionGroup.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/Fingering.h"
#include "mx/core/elements/FirstFret.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Frame.h"
#include "mx/core/elements/FrameFrets.h"
#include "mx/core/elements/FrameNote.h"
#include "mx/core/elements/FrameStrings.h"
#include "mx/core/elements/Fret.h"
#include "mx/core/elements/Function.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/HarmonyChordGroup.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Inversion.h"
#include "mx/core/elements/Kind.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Root.h"
#include "mx/core/elements/RootAlter.h"
#include "mx/core/elements/RootStep.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/String.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"
#include "mx/impl/DynamicsReader.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/MetronomeReader.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/SpannerFunctions.h"
#include "mx/utility/Round.h"

namespace mx
{
namespace impl
{
DirectionReader::DirectionReader(std::shared_ptr<const core::Direction> inDirection, Cursor inCursor)
    : myDirection{inDirection}, myHarmony{nullptr}, myCursor{inCursor}, myConverter{}, myOutDirectionData{}
{
}

DirectionReader::DirectionReader(std::shared_ptr<const core::Harmony> inHarmony, Cursor inCursor)
    : myDirection{nullptr}, myHarmony{inHarmony}, myCursor{inCursor}, myConverter{}, myOutDirectionData{}
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
        result.isStaffValueSpecified = myDirection->getHasStaff();
    }
    else if (myHarmony)
    {
        result.isStaffValueSpecified = myHarmony->getHasStaff();
    }

    return result;
}

void DirectionReader::updateTimeForOffset()
{
    if (myDirection)
    {
        if (myDirection->getHasOffset())
        {
            const auto offset = static_cast<int>(std::ceil(myDirection->getOffset()->getValue().getValue() - 0.5));
            myOutDirectionData.tickTimePosition += offset;
        }
    }
    else if (myHarmony)
    {
        if (myHarmony->getHasOffset())
        {
            const auto offset = static_cast<int>(std::ceil(myHarmony->getOffset()->getValue().getValue() - 0.5));
            myOutDirectionData.tickTimePosition += offset;
        }
    }
}

void DirectionReader::parsePlacement()
{
    if (myDirection)
    {
        if (myDirection->getAttributes()->hasPlacement)
        {
            myOutDirectionData.placement = myConverter.convert(myDirection->getAttributes()->placement);
        }
    }
    else if (myHarmony)
    {
        if (myHarmony->getAttributes()->hasPlacement)
        {
            myOutDirectionData.placement = myConverter.convert(myHarmony->getAttributes()->placement);
        }
    }
}

void DirectionReader::parseValues()
{
    if (myDirection)
    {
        for (const auto &dtPtr : myDirection->getDirectionTypeSet())
        {
            const auto &dt = *dtPtr;
            parseDirectionType(dt);
        }
    }
    else if (myHarmony)
    {
        for (const auto &hPtr : myHarmony->getHarmonyChordGroupSet())
        {
            if (hPtr->getChoice() == core::HarmonyChordGroup::Choice::root)
            {
                parseHarmony(*myHarmony, *hPtr);
            }
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
    switch (directionType.getChoice())
    {
    case core::DirectionType::Choice::rehearsal: {
        parseRehearsal(directionType);
        break;
    }
    case core::DirectionType::Choice::segno: {
        parseSegno(directionType);
        break;
    }
    case core::DirectionType::Choice::words: {
        parseWords(directionType);
        break;
    }
    case core::DirectionType::Choice::coda: {
        parseCoda(directionType);
        break;
    }
    case core::DirectionType::Choice::wedge: {
        parseWedge(directionType);
        break;
    }
    case core::DirectionType::Choice::dynamics: {
        parseDynamics(directionType);
        break;
    }
    case core::DirectionType::Choice::dashes: {
        parseDashes(directionType);
        break;
    }
    case core::DirectionType::Choice::bracket: {
        parseBracket(directionType);
        break;
    }
    case core::DirectionType::Choice::pedal: {
        parsePedal(directionType);
        break;
    }
    case core::DirectionType::Choice::metronome: {
        parseMetronome(directionType);
        break;
    }
    case core::DirectionType::Choice::octaveShift: {
        parseOctaveShift(directionType);
        break;
    }
    case core::DirectionType::Choice::harpPedals: {
        parseHarpPedals(directionType);
        break;
    }
    case core::DirectionType::Choice::damp: {
        parseDamp(directionType);
        break;
    }
    case core::DirectionType::Choice::dampAll: {
        parseDampAll(directionType);
        break;
    }
    case core::DirectionType::Choice::eyeglasses: {
        parseEyeglasses(directionType);
        break;
    }
    case core::DirectionType::Choice::stringMute: {
        parseStringMute(directionType);
        break;
    }
    case core::DirectionType::Choice::scordatura: {
        parseScordatura(directionType);
        break;
    }
    case core::DirectionType::Choice::image: {
        parseImage(directionType);
        break;
    }
    case core::DirectionType::Choice::principalVoice: {
        parsePrincipalVoice(directionType);
        break;
    }
    case core::DirectionType::Choice::accordionRegistration: {
        parseAccordionRegistration(directionType);
        break;
    }
    case core::DirectionType::Choice::percussion: {
        parsePercussion(directionType);
        break;
    }
    case core::DirectionType::Choice::otherDirection: {
        parseOtherDirection(directionType);
        break;
    }
    default:
        break;
    }
}

void DirectionReader::parseRehearsal(const core::DirectionType &directionType)
{
    const auto &rehearsalSet = directionType.getRehearsalSet();

    for (const auto &rehearsalPtr : rehearsalSet)
    {
        api::RehearsalData outRehearsal;
        const auto &attr = *rehearsalPtr->getAttributes();
        outRehearsal.text = rehearsalPtr->getValue().getValue();
        outRehearsal.positionData = getPositionData(attr);
        outRehearsal.colorData = getColor(attr);
        if (attr.hasEnclosure)
        {
            switch (attr.enclosure)
            {
            case core::EnclosureShape::rectangle:
                outRehearsal.enclosure = api::RehearsalEnclosure::rectangle;
                break;
            case core::EnclosureShape::square:
                outRehearsal.enclosure = api::RehearsalEnclosure::square;
                break;
            case core::EnclosureShape::oval:
                outRehearsal.enclosure = api::RehearsalEnclosure::oval;
                break;
            case core::EnclosureShape::circle:
                outRehearsal.enclosure = api::RehearsalEnclosure::circle;
                break;
            case core::EnclosureShape::bracket:
                outRehearsal.enclosure = api::RehearsalEnclosure::bracket;
                break;
            case core::EnclosureShape::triangle:
                outRehearsal.enclosure = api::RehearsalEnclosure::triangle;
                break;
            case core::EnclosureShape::diamond:
                outRehearsal.enclosure = api::RehearsalEnclosure::diamond;
                break;
            case core::EnclosureShape::none:
                outRehearsal.enclosure = api::RehearsalEnclosure::none;
                break;
            }
        }
        myOutDirectionData.rehearsals.emplace_back(std::move(outRehearsal));
        appendOrderedComponent(api::DirectionComponentKind::rehearsal,
                               static_cast<int>(myOutDirectionData.rehearsals.size()) - 1);
    }

    MX_UNUSED(directionType);
}

void DirectionReader::parseSegno(const core::DirectionType &directionType)
{
    const auto &segnoSet = directionType.getSegnoSet();

    for (const auto &segnoPtr : segnoSet)
    {
        api::SegnoData outSegno;
        const auto &attr = *segnoPtr->getAttributes();
        outSegno.positionData = getPositionData(attr);
        outSegno.colorData = getColor(attr);
        myOutDirectionData.segnos.emplace_back(std::move(outSegno));
        appendOrderedComponent(api::DirectionComponentKind::segno,
                               static_cast<int>(myOutDirectionData.segnos.size()) - 1);
    }
}

void DirectionReader::parseWords(const core::DirectionType &directionType)
{
    const auto &wordSet = directionType.getWordsSet();

    for (const auto &wordPtr : wordSet)
    {
        api::WordsData outWords;
        const auto &attr = *wordPtr->getAttributes();
        outWords.text = wordPtr->getValue().getValue();
        outWords.positionData = getPositionData(attr);
        outWords.colorData = getColor(attr);
        outWords.fontData = getFontData(attr);
        if (attr.hasEnclosure)
        {
            switch (attr.enclosure)
            {
            case core::EnclosureShape::rectangle:
                outWords.enclosure = api::RehearsalEnclosure::rectangle;
                break;
            case core::EnclosureShape::square:
                outWords.enclosure = api::RehearsalEnclosure::square;
                break;
            case core::EnclosureShape::oval:
                outWords.enclosure = api::RehearsalEnclosure::oval;
                break;
            case core::EnclosureShape::circle:
                outWords.enclosure = api::RehearsalEnclosure::circle;
                break;
            case core::EnclosureShape::bracket:
                outWords.enclosure = api::RehearsalEnclosure::bracket;
                break;
            case core::EnclosureShape::triangle:
                outWords.enclosure = api::RehearsalEnclosure::triangle;
                break;
            case core::EnclosureShape::diamond:
                outWords.enclosure = api::RehearsalEnclosure::diamond;
                break;
            case core::EnclosureShape::none:
                outWords.enclosure = api::RehearsalEnclosure::none;
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
    const auto &codaSet = directionType.getCodaSet();

    for (const auto &codaPtr : codaSet)
    {
        api::CodaData outCoda;
        const auto &attr = *codaPtr->getAttributes();
        outCoda.positionData = getPositionData(attr);
        outCoda.colorData = getColor(attr);
        myOutDirectionData.codas.emplace_back(std::move(outCoda));
        appendOrderedComponent(api::DirectionComponentKind::coda,
                               static_cast<int>(myOutDirectionData.codas.size()) - 1);
    }
}

void DirectionReader::parseWedge(const core::DirectionType &directionType)
{
    const auto &wedge = *directionType.getWedge();
    const auto &attr = *wedge.getAttributes();
    const auto wedgeType = myConverter.convert(attr.type);
    const bool isSpreadSpecified = attr.hasSpread;
    const long double spread = attr.spread.getValue();
    auto positionData = getPositionData(attr);
    auto lineData = getLineData(attr);
    auto colorData = getColor(attr);

    if (attr.type == core::WedgeType::stop)
    {
        api::WedgeStop stop;
        if (attr.hasNumber)
        {
            stop.numberLevel = attr.number.getValue();
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
        if (attr.hasNumber)
        {
            start.numberLevel = attr.number.getValue();
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
    for (const auto &dynamic : directionType.getDynamicsSet())
    {
        DynamicsReader reader{*dynamic, myCursor};
        reader.parseDynamics(myOutDirectionData.marks);
        // parseDynamic( *dynamic );
    }
    for (auto i = markCountBefore; i < myOutDirectionData.marks.size(); ++i)
    {
        appendOrderedComponent(api::DirectionComponentKind::mark, static_cast<int>(i));
    }
}

void DirectionReader::parseDynamic(const core::Dynamics &dynamic)
{
    auto mark = api::MarkData{};
    const auto valueObject = dynamic.getValue();
    mark.markType = myConverter.convertDynamic(valueObject.getValue());
    mark.name = valueObject.getValueString();
    impl::parseMarkDataAttributes(*dynamic.getAttributes(), mark);

    auto placement = mark.positionData.placement;

    if (placement == api::Placement::unspecified)
    {
        placement = myOutDirectionData.placement;
    }

    mark.name = valueObject.getValueString();
    myOutDirectionData.marks.emplace_back(std::move(mark));
    appendOrderedComponent(api::DirectionComponentKind::mark, static_cast<int>(myOutDirectionData.marks.size()) - 1);
}

void DirectionReader::parseDashes(const core::DirectionType &directionType)
{
    const auto &dashes = *directionType.getDashes();
    const auto &attr = *dashes.getAttributes();

    if (attr.type == core::StartStopContinue::stop)
    {
        auto stop = impl::getSpannerStop(attr);
        stop.tickTimePosition = myCursor.tickTimePosition;
        myOutDirectionData.dashesStops.emplace_back(std::move(stop));
        appendOrderedComponent(api::DirectionComponentKind::dashesStop,
                               static_cast<int>(myOutDirectionData.dashesStops.size()) - 1);
        return;
    }
    else if (attr.type == core::StartStopContinue::start)
    {
        auto start = impl::getSpannerStart(attr);
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
    const auto &bracket = *directionType.getBracket();
    const auto &attr = *bracket.getAttributes();

    const auto makeBracketLineData = [&]() {
        api::LineData lineData{};
        lineData.lineHook = myConverter.convert(attr.lineEnd);

        if (attr.hasLineType)
        {
            lineData.lineType = myConverter.convert(attr.lineType);
        }
        if (attr.hasEndLength)
        {
            lineData.isStopLengthSpecified = true;
            lineData.endLength = attr.endLength.getValue();
        }
        if (attr.hasDashLength)
        {
            lineData.isDashLengthSpecified = true;
            lineData.dashLength = attr.dashLength.getValue();
        }
        if (attr.hasSpaceLength)
        {
            lineData.isSpaceLengthSpecified = true;
            lineData.spaceLength = attr.spaceLength.getValue();
        }
        return lineData;
    };

    if (attr.type == core::StartStopContinue::stop)
    {
        auto stop = impl::getSpannerStop(attr);
        stop.tickTimePosition = myCursor.tickTimePosition;
        stop.lineData = makeBracketLineData();
        myOutDirectionData.bracketStops.emplace_back(std::move(stop));
        appendOrderedComponent(api::DirectionComponentKind::bracketStop,
                               static_cast<int>(myOutDirectionData.bracketStops.size()) - 1);
        return;
    }
    else if (attr.type == core::StartStopContinue::start)
    {
        api::SpannerStart start;
        start.tickTimePosition = myCursor.tickTimePosition;
        start.numberLevel = impl::checkNumber(&attr);
        start.positionData = this->parsePositionData(attr);
        start.lineData = makeBracketLineData();
        start.printData = impl::getPrintData(attr);
        myOutDirectionData.bracketStarts.emplace_back(std::move(start));
        appendOrderedComponent(api::DirectionComponentKind::bracketStart,
                               static_cast<int>(myOutDirectionData.bracketStarts.size()) - 1);
        return;
    }
}

void DirectionReader::parsePedal(const core::DirectionType &directionType)
{
    const auto &pedalPtr = *directionType.getPedal();
    const auto &attr = *pedalPtr.getAttributes();

    if (attr.type != core::StartStopChangeContinue::start && attr.type != core::StartStopChangeContinue::stop)
    {
        return;
    }

    const auto placement =
        myDirection->getAttributes()->hasPlacement
            ? (myDirection->getAttributes()->placement == core::AboveBelow::above ? api::Placement::above
                                                                                  : api::Placement::below)
            : api::Placement::unspecified;

    myOutDirectionData.placement = placement;

    if (attr.hasLine && attr.line == core::YesNo::yes)
    {
        if (attr.type == core::StartStopChangeContinue::start)
        {
            api::SpannerStart start;
            start.tickTimePosition = myOutDirectionData.tickTimePosition;
            start.positionData = getPositionData(attr);
            start.positionData.placement = placement;
            start.lineData.lineType = api::LineType::solid;
            start.lineData.lineHook = api::LineHook::none;
            myOutDirectionData.pedalStarts.emplace_back(std::move(start));
            appendOrderedComponent(api::DirectionComponentKind::pedalStart,
                                   static_cast<int>(myOutDirectionData.pedalStarts.size()) - 1);
        }
        else if (attr.type == core::StartStopChangeContinue::stop)
        {
            api::SpannerStop stop;
            stop.tickTimePosition = myOutDirectionData.tickTimePosition;
            stop.positionData = getPositionData(attr);
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

    if (attr.type == core::StartStopChangeContinue::stop)
    {
        pedalType = api::MarkType::damp;
    }

    auto mark = api::MarkData{placement, pedalType};
    mark.positionData = getPositionData(attr);
    mark.positionData.placement = placement;
    myOutDirectionData.marks.emplace_back(std::move(mark));
    appendOrderedComponent(api::DirectionComponentKind::mark, static_cast<int>(myOutDirectionData.marks.size()) - 1);
}

void DirectionReader::parseMetronome(const core::DirectionType &directionType)
{
    const auto &metronome = *directionType.getMetronome();
    MetronomeReader reader{metronome};
    myOutDirectionData.tempos.emplace_back(reader.getTempoData());
    appendOrderedComponent(api::DirectionComponentKind::tempo, static_cast<int>(myOutDirectionData.tempos.size()) - 1);
}

void DirectionReader::parseOctaveShift(const core::DirectionType &directionType)
{
    const auto &octaveShift = *directionType.getOctaveShift();
    auto &attr = *octaveShift.getAttributes();

    if (attr.type == core::UpDownStopContinue::continue_)
    {
        // not supported
        return;
    }

    bool isStop = attr.type == core::UpDownStopContinue::stop;
    if (isStop)
    {
        auto stop = impl::getSpannerStop(attr);
        stop.tickTimePosition = myCursor.tickTimePosition;
        myOutDirectionData.ottavaStops.emplace_back(std::move(stop));
        appendOrderedComponent(api::DirectionComponentKind::ottavaStop,
                               static_cast<int>(myOutDirectionData.ottavaStops.size()) - 1);
        return;
    }

    auto ottavaType = api::OttavaType::unspecified;
    int amount = 8;

    if (attr.hasSize)
    {
        amount = attr.size.getValue();
    }

    bool isUp = attr.type == core::UpDownStopContinue::up;

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
    start.spannerStart = impl::getSpannerStart(attr);
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
    chord.root = myConverter.convert(inGrp.getRoot()->getRootStep()->getValue());

    if (inGrp.getRoot()->getHasRootAlter())
    {
        chord.rootAlter =
            mx::utility::roundTo<core::DecimalType, int>(inGrp.getRoot()->getRootAlter()->getValue().getValue());
    }

    const auto &kind = *inGrp.getKind();

    chord.chordKind = myConverter.convert(kind.getValue());

    if (kind.getAttributes()->hasText)
    {
        chord.text = kind.getAttributes()->text.getValue();
    }

    if (kind.getAttributes()->hasUseSymbols)
    {
        if (kind.getAttributes()->useSymbols == mx::core::YesNo::yes)
        {
            chord.useSymbols = api::Bool::yes;
        }
        else
        {
            chord.useSymbols = api::Bool::no;
        }
    }

    if (inGrp.getHasBass())
    {
        const auto &bass = *inGrp.getBass();
        chord.bass = myConverter.convert(bass.getBassStep()->getValue());

        if (bass.getHasBassAlter())
        {
            chord.bassAlter = mx::utility::roundTo<core::DecimalType, int>(bass.getBassAlter()->getValue().getValue());
        }
    }

    const auto &degrees = inGrp.getDegreeSet();

    for (const auto &degree : degrees)
    {
        api::Extension extension;
        bool doAddExtension = true;

        const auto type = degree->getDegreeType()->getValue();
        const auto alter =
            mx::utility::roundTo<core::DecimalType, int>(degree->getDegreeAlter()->getValue().getValue());
        const auto value = degree->getDegreeValue()->getValue().getValue();

        switch (type)
        {
        case core::DegreeTypeValue::alter:
            extension.extensionType = api::ExtensionType::alter;
            break;
        case core::DegreeTypeValue::add:
            extension.extensionType = api::ExtensionType::add;
            break;
        case core::DegreeTypeValue::subtract:
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

        if (degree->getAttributes()->hasPrintObject)
        {
            if (degree->getAttributes()->printObject == core::YesNo::yes)
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

    for (const auto &pi : inHarmony.getProcessingInstructions())
    {
        mx::api::MiscData misc;
        misc.name = pi.getName();
        misc.data = pi.getData();
        chord.miscData.push_back(misc);
    }

    if (inHarmony.getHasFrame())
    {
        chord.hasFrameData = true;
        const auto frame = inHarmony.getFrame();
        chord.frameData.stringCount = static_cast<int>(frame->getFrameStrings()->getValue().getValue());
        chord.frameData.fretCount = static_cast<int>(frame->getFrameFrets()->getValue().getValue());

        if (frame->getHasFirstFret())
        {
            chord.frameData.isFirstFretSpecified = true;
            chord.frameData.firstFret = static_cast<int>(frame->getFirstFret()->getValue().getValue());
        }

        for (const auto &frameNotePtr : frame->getFrameNoteSet())
        {
            api::FrameNoteData frameNote;
            frameNote.stringNumber = static_cast<int>(frameNotePtr->getString()->getValue().getValue());
            frameNote.fretNumber = static_cast<int>(frameNotePtr->getFret()->getValue().getValue());

            if (frameNotePtr->getHasFingering())
            {
                const auto fingeringText = frameNotePtr->getFingering()->getValue().getValue();
                try
                {
                    frameNote.fingering = std::stoi(fingeringText);
                    frameNote.isFingeringSpecified = true;
                }
                catch (...)
                {
                }
            }

            if (frameNotePtr->getHasBarre())
            {
                const auto barreType = frameNotePtr->getBarre()->getAttributes()->type;
                switch (barreType)
                {
                case core::StartStop::start:
                    frameNote.barre = api::FrameBarre::start;
                    break;
                case core::StartStop::stop:
                    frameNote.barre = api::FrameBarre::stop;
                    break;
                }
            }

            chord.frameData.notes.push_back(frameNote);
        }
    }

    const auto &attr = *inHarmony.getAttributes();
    chord.positionData = getPositionData(attr);

    myOutDirectionData.chords.push_back(chord);
    appendOrderedComponent(api::DirectionComponentKind::chord, static_cast<int>(myOutDirectionData.chords.size()) - 1);
}
} // namespace impl
} // namespace mx
