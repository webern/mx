// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DirectionReader.h"
#include "mx/api/WedgeData.h"
#include "mx/core/generated/Accord.h"
#include "mx/core/generated/AccordionRegistration.h"
#include "mx/core/generated/Barre.h"
#include "mx/core/generated/Bass.h"
#include "mx/core/generated/BassStep.h"
#include "mx/core/generated/Beater.h"
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
#include "mx/core/generated/Effect.h"
#include "mx/core/generated/EmptyPrintStyleAlignID.h"
#include "mx/core/generated/Fingering.h"
#include "mx/core/generated/FirstFret.h"
#include "mx/core/generated/Frame.h"
#include "mx/core/generated/FrameNote.h"
#include "mx/core/generated/Fret.h"
#include "mx/core/generated/Glass.h"
#include "mx/core/generated/HarmonyAlter.h"
#include "mx/core/generated/HarmonyChordGroup.h"
#include "mx/core/generated/HarmonyChordGroupChoice.h"
#include "mx/core/generated/HarpPedals.h"
#include "mx/core/generated/Image.h"
#include "mx/core/generated/Inversion.h"
#include "mx/core/generated/Kind.h"
#include "mx/core/generated/KindValue.h"
#include "mx/core/generated/Membrane.h"
#include "mx/core/generated/Metal.h"
#include "mx/core/generated/Metronome.h"
#include "mx/core/generated/Numeral.h"
#include "mx/core/generated/NumeralKey.h"
#include "mx/core/generated/NumeralMode.h"
#include "mx/core/generated/NumeralRoot.h"
#include "mx/core/generated/NumeralValue.h"
#include "mx/core/generated/Octave.h"
#include "mx/core/generated/OctaveShift.h"
#include "mx/core/generated/Offset.h"
#include "mx/core/generated/OnOff.h"
#include "mx/core/generated/OtherDirection.h"
#include "mx/core/generated/OtherText.h"
#include "mx/core/generated/Pedal.h"
#include "mx/core/generated/PedalTuning.h"
#include "mx/core/generated/PedalType.h"
#include "mx/core/generated/Percussion.h"
#include "mx/core/generated/PercussionChoice.h"
#include "mx/core/generated/Pitched.h"
#include "mx/core/generated/PrincipalVoice.h"
#include "mx/core/generated/PrincipalVoiceSymbol.h"
#include "mx/core/generated/Root.h"
#include "mx/core/generated/RootStep.h"
#include "mx/core/generated/Scordatura.h"
#include "mx/core/generated/Segno.h"
#include "mx/core/generated/Semitones.h"
#include "mx/core/generated/Sound.h"
#include "mx/core/generated/StaffDivide.h"
#include "mx/core/generated/StaffDivideSymbol.h"
#include "mx/core/generated/StartStop.h"
#include "mx/core/generated/StartStopContinue.h"
#include "mx/core/generated/Step.h"
#include "mx/core/generated/Stick.h"
#include "mx/core/generated/String.h"
#include "mx/core/generated/StringMute.h"
#include "mx/core/generated/StringNumber.h"
#include "mx/core/generated/StyleText.h"
#include "mx/core/generated/Timpani.h"
#include "mx/core/generated/TuningGroup.h"
#include "mx/core/generated/UpDownStopContinue.h"
#include "mx/core/generated/ValignImage.h"
#include "mx/core/generated/Wedge.h"
#include "mx/core/generated/WedgeType.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/DynamicsReader.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/MetronomeReader.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/SoundFunctions.h"
#include "mx/impl/SpannerFunctions.h"
#include "mx/utility/Round.h"
#include "mx/utility/StringToInt.h"
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
    parseOffset();
    parsePlacement();
    parseSystemRelation();
    parseValues();
    return returnData();
}

mx::api::DirectionData DirectionReader::initializeData()
{
    auto result = api::DirectionData{};
    result.tickTimePosition = myCursor.tickTimePosition;

    if (myDirection)
    {
        result.isStaffValueSpecified = myDirection->staff().has_value();
        result.id = getId(*myDirection);
    }
    else if (myHarmony)
    {
        result.isStaffValueSpecified = myHarmony->staff().has_value();
    }

    return result;
}

void DirectionReader::parseOffset()
{
    // tickTimePosition is left at the anchor (the current musical location); the <offset> is a
    // drawn-position nudge recorded verbatim, not folded into the tick. The writer re-emits it and
    // MeasureWriter still places the direction by its anchor, so an offset that happens to land on
    // a note tick stays distinguishable from a plain direction there.
    if (myDirection)
    {
        if (myDirection->offset().has_value())
        {
            const auto rawVal = myDirection->offset()->value().value().value();
            myOutDirectionData.offset = static_cast<int>(std::ceil(rawVal - 0.5));
        }
    }
    else if (myHarmony)
    {
        if (myHarmony->offset().has_value())
        {
            const auto rawVal = myHarmony->offset()->value().value().value();
            myOutDirectionData.offset = static_cast<int>(std::ceil(rawVal - 0.5));
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

void DirectionReader::parseSystemRelation()
{
    if (myDirection)
    {
        if (myDirection->system().has_value())
        {
            myOutDirectionData.systemRelation = myConverter.convertDirectionSystemRelation(*myDirection->system());
        }
    }
    else if (myHarmony)
    {
        if (myHarmony->system().has_value())
        {
            myOutDirectionData.systemRelation = myConverter.convertDirectionSystemRelation(*myHarmony->system());
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

        // The <direction>'s editorial-voice <voice> assigns the direction to a voice. A value of
        // VALUE_UNSPECIFIED on DirectionData::voice means none was present.
        if (myDirection->editorialVoiceDirection().voice().has_value())
        {
            int parsedVoice = api::VALUE_UNSPECIFIED;
            if (utility::stringToInt(myDirection->editorialVoiceDirection().voice()->c_str(), parsedVoice))
            {
                myOutDirectionData.voice = parsedVoice;
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

mx::api::DirectionData DirectionReader::returnData()
{
    api::DirectionData temp{std::move(myOutDirectionData)};
    myOutDirectionData = api::DirectionData{};
    return temp;
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
        parseWordsRun(directionType);
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
        parseStaffDivide(directionType);
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
        outRehearsal.fontData = getFontData(rehearsal);
        outRehearsal.isColorSpecified = rehearsal.color().has_value();
        if (outRehearsal.isColorSpecified)
        {
            outRehearsal.colorData = getColor(rehearsal);
        }
        if (rehearsal.enclosure().has_value())
        {
            outRehearsal.enclosure = myConverter.convert(*rehearsal.enclosure());
        }
        if (rehearsal.justify().has_value())
        {
            outRehearsal.justify = myConverter.convert(*rehearsal.justify());
        }
        outRehearsal.id = getId(rehearsal);
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outRehearsal)});
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
        outSegno.id = getId(segno);
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outSegno)});
    }
}

void DirectionReader::parseWordsRun(const core::DirectionType &directionType)
{
    // The core `choice` alternative holds this direction-type's ordered (words|symbol)+ run.
    // The whole run becomes one wordsRun DirectionChoice so the interleaving survives.
    const auto &choiceItems = directionType.choice().asChoice();

    std::vector<api::WordsChoice> run;

    for (const auto &item : choiceItems.items())
    {
        if (item.isSymbol())
        {
            const auto &symbolEl = item.asSymbol();
            api::SymbolData outSymbol;
            outSymbol.smufl = symbolEl.value().toString();
            outSymbol.positionData = getPositionData(symbolEl);
            outSymbol.fontData = getFontData(symbolEl);
            if (symbolEl.color().has_value())
            {
                outSymbol.color = getColor(symbolEl);
            }
            if (symbolEl.enclosure().has_value())
            {
                outSymbol.enclosure = myConverter.convert(*symbolEl.enclosure());
            }
            if (symbolEl.justify().has_value())
            {
                outSymbol.justify = myConverter.convert(*symbolEl.justify());
            }
            outSymbol.id = getId(symbolEl);
            run.emplace_back(std::move(outSymbol));
            continue;
        }
        const auto &wordEl = item.asWords();
        api::WordsData outWords;
        outWords.text = wordEl.value();
        outWords.positionData = getPositionData(wordEl);
        outWords.isColorSpecified = wordEl.color().has_value();
        if (outWords.isColorSpecified)
        {
            outWords.colorData = getColor(wordEl);
        }
        outWords.fontData = getFontData(wordEl);
        if (wordEl.enclosure().has_value())
        {
            outWords.enclosure = myConverter.convert(*wordEl.enclosure());
        }
        if (wordEl.justify().has_value())
        {
            outWords.justify = myConverter.convert(*wordEl.justify());
        }
        outWords.id = getId(wordEl);
        run.emplace_back(std::move(outWords));
    }

    if (!run.empty())
    {
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(run)});
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
        outCoda.id = getId(coda);
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outCoda)});
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
    const bool isColorSpecified = wedge.color().has_value();
    const auto colorData = isColorSpecified ? getColor(wedge) : api::ColorData{};

    if (wedge.type().tag() == core::WedgeType::Tag::stop)
    {
        api::WedgeStop stop;
        if (wedge.number().has_value())
        {
            stop.number = api::SpannerNumber(wedge.number()->value());
        }
        if (isSpreadSpecified)
        {
            stop.isSpreadSpecified = true;
            stop.spread = spread;
        }
        stop.positionData = positionData;
        stop.id = getId(wedge);
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(stop)});
        return;
    }
    else
    {
        api::WedgeStart start;
        if (wedge.number().has_value())
        {
            start.number = api::SpannerNumber(wedge.number()->value());
        }
        if (isSpreadSpecified)
        {
            start.isSpreadSpecified = true;
            start.spread = spread;
        }
        start.wedgeType = wedgeType;
        start.positionData = positionData;
        start.lineData = lineData;
        start.isColorSpecified = isColorSpecified;
        start.colorData = colorData;
        start.id = getId(wedge);
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(start)});
    }
}

void DirectionReader::parseDynamics(const core::DirectionType &directionType)
{
    for (const auto &dynamic : directionType.choice().asDynamics().items())
    {
        std::vector<api::MarkData> marks;
        DynamicsReader reader{dynamic, myCursor};
        reader.parseDynamics(marks);
        for (auto &mark : marks)
        {
            mark.tickTimePosition = myOutDirectionData.tickTimePosition;
            myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(mark)});
        }
    }
}

void DirectionReader::parseDashes(const core::DirectionType &directionType)
{
    const auto &dashes = directionType.choice().asDashes();

    if (dashes.type().tag() == core::StartStopContinue::Tag::stop)
    {
        auto stop = impl::getSpannerStop(dashes);
        stop.tickTimePosition = myCursor.tickTimePosition;
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice::dashesStop(std::move(stop)));
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
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice::dashesStart(std::move(start)));
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
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice::bracketStop(std::move(stop)));
        return;
    }
    else if (bracket.type().tag() == core::StartStopContinue::Tag::start)
    {
        api::SpannerStart start;
        start.tickTimePosition = myCursor.tickTimePosition;
        if (impl::checkHasNumber(&bracket))
        {
            start.number = api::SpannerNumber(impl::checkNumber(&bracket));
        }
        start.positionData = this->parsePositionData(bracket);
        start.lineData = makeBracketLineData();
        start.printData = impl::getPrintData(bracket);
        start.id = getId(bracket);
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice::bracketStart(std::move(start)));
        return;
    }
}

api::PedalLineKind pedalKindForTag(core::PedalType::Tag tag)
{
    switch (tag)
    {
    case core::PedalType::Tag::start:
        return api::PedalLineKind::start;
    case core::PedalType::Tag::stop:
        return api::PedalLineKind::stop;
    case core::PedalType::Tag::sostenuto:
        return api::PedalLineKind::sostenuto;
    case core::PedalType::Tag::change:
        return api::PedalLineKind::change;
    case core::PedalType::Tag::continue_:
        return api::PedalLineKind::continueLine;
    case core::PedalType::Tag::discontinue:
        return api::PedalLineKind::discontinue;
    case core::PedalType::Tag::resume:
        return api::PedalLineKind::resume;
    }
    return api::PedalLineKind::unspecified;
}

void DirectionReader::parsePedal(const core::DirectionType &directionType)
{
    const auto &pedal = directionType.choice().asPedal();
    const auto tag = pedal.type().tag();

    const auto placement =
        (myDirection && myDirection->placement().has_value())
            ? (myDirection->placement()->tag() == core::AboveBelow::Tag::above ? api::Placement::above
                                                                               : api::Placement::below)
            : api::Placement::unspecified;

    myOutDirectionData.placement = placement;

    // A pedal line (line="yes"). sostenuto/change/continue/discontinue/resume are inherently
    // line-formatting types (the spec only defines them with line="yes"), so they are treated as
    // line pedals even when the line attribute is omitted. start/stop with line != "yes" are the
    // sign form (Ped./* marks) and fall through to the MarkData path below.
    const bool isLine = (pedal.line().has_value() && pedal.line()->tag() == core::YesNo::Tag::yes) ||
                        tag == core::PedalType::Tag::sostenuto || tag == core::PedalType::Tag::change ||
                        tag == core::PedalType::Tag::continue_ || tag == core::PedalType::Tag::discontinue ||
                        tag == core::PedalType::Tag::resume;

    if (isLine)
    {
        api::PedalLineData pedalData{pedalKindForTag(tag)};
        pedalData.tickTimePosition = myOutDirectionData.tickTimePosition;
        pedalData.positionData = getPositionData(pedal);
        pedalData.positionData.placement = placement;
        pedalData.id = getId(pedal);
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(pedalData)});
        return;
    }

    auto pedalType = api::MarkType::pedal;

    if (tag == core::PedalType::Tag::stop)
    {
        pedalType = api::MarkType::damp;
    }

    auto mark = api::MarkData{placement, pedalType};
    mark.tickTimePosition = myOutDirectionData.tickTimePosition;
    mark.positionData = getPositionData(pedal);
    mark.positionData.placement = placement;
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(mark)});
}

void DirectionReader::parseMetronome(const core::DirectionType &directionType)
{
    const auto &metronome = directionType.choice().asMetronome();
    MetronomeReaderParameters params{metronome};
    MetronomeReader reader{std::move(params)};
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{reader.getTempoData()});
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
        api::OttavaStop stop;
        stop.spannerStop = impl::getSpannerStop(octaveShift);
        stop.spannerStop.tickTimePosition = myCursor.tickTimePosition;

        // The stop's size is not kept: it restates the start's octave shift, and a stop whose size
        // contradicts its start would let the api hold two answers to one question. Only whether
        // the source spelled the attribute out is recorded, so the same spelling is written back.
        stop.writeSize = octaveShift.size().has_value() ? api::Bool::yes : api::Bool::no;
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(stop)});
        return;
    }

    auto ottavaType = api::OttavaType::unspecified;
    int amount = 8;

    if (octaveShift.size().has_value())
    {
        amount = *octaveShift.size();
    }

    // Per the MusicXML spec, octave-shift's type attribute describes the direction the
    // *written* notes are shifted from the true (sounding) pitch: an 8va, which sounds an
    // octave above what is written, is encoded as type="down" (notes are written below true
    // pitch). So type="down" maps to the "up" ottava variants (o8va/o15ma/o22ma) and type="up" maps
    // to the "down" variants (o8vb/o15mb/o22mb).
    bool isUp = octaveShift.type().tag() == core::UpDownStopContinue::Tag::up;

    // MusicXML does not properly constrain the ottava size to valid music notation values. We do
    // so here by interpolating the value into an enum that maps to valid music notation.
    if (!isUp && amount == 22)
    {
        ottavaType = api::OttavaType::o22ma;
    }
    else if (!isUp && amount > 8)
    {
        ottavaType = api::OttavaType::o15ma;
    }
    else if (!isUp)
    {
        ottavaType = api::OttavaType::o8va;
    }
    else if (isUp && amount == 22)
    {
        ottavaType = api::OttavaType::o22mb;
    }
    else if (isUp && amount > 8)
    {
        ottavaType = api::OttavaType::o15mb;
    }
    else if (isUp)
    {
        ottavaType = api::OttavaType::o8vb;
    }

    api::OttavaStart start;
    start.spannerStart = impl::getSpannerStart(octaveShift);
    start.ottavaType = ottavaType;
    const bool isEightLine = ottavaType == api::OttavaType::o8va || ottavaType == api::OttavaType::o8vb;
    start.writeDefaultSize = isEightLine && octaveShift.size().has_value();
    start.spannerStart.tickTimePosition = myCursor.tickTimePosition;
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(start)});
}

void DirectionReader::parseHarpPedals(const core::DirectionType &directionType)
{
    const auto &harpPedals = directionType.choice().asHarpPedals();
    api::HarpPedalsData outHarpPedals;
    for (const auto &pedalTuning : harpPedals.pedalTuning())
    {
        api::HarpPedalTuning outTuning;
        outTuning.step = myConverter.convert(pedalTuning.pedalStep());
        const auto semitonesAndCents =
            Converter::convertToSemitonesAndCents(static_cast<double>(pedalTuning.pedalAlter().value().value()));
        outTuning.alter = semitonesAndCents.first;
        outTuning.cents = semitonesAndCents.second;
        outHarpPedals.pedalTunings.emplace_back(outTuning);
    }
    outHarpPedals.positionData = getPositionData(harpPedals);
    outHarpPedals.fontData = getFontData(harpPedals);
    if (harpPedals.color().has_value())
    {
        outHarpPedals.color = getColor(harpPedals);
    }
    outHarpPedals.id = getId(harpPedals);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outHarpPedals)});
}

void DirectionReader::parseDamp(const core::DirectionType &directionType)
{
    const auto &damp = directionType.choice().asDamp();
    api::DampData outDamp;
    outDamp.positionData = getPositionData(damp);
    outDamp.fontData = getFontData(damp);
    if (damp.color().has_value())
    {
        outDamp.color = getColor(damp);
    }
    outDamp.id = getId(damp);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outDamp)});
}

void DirectionReader::parseDampAll(const core::DirectionType &directionType)
{
    const auto &dampAll = directionType.choice().asDampAll();
    api::DampAllData outDampAll;
    outDampAll.positionData = getPositionData(dampAll);
    outDampAll.fontData = getFontData(dampAll);
    if (dampAll.color().has_value())
    {
        outDampAll.color = getColor(dampAll);
    }
    outDampAll.id = getId(dampAll);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outDampAll)});
}

void DirectionReader::parseEyeglasses(const core::DirectionType &directionType)
{
    const auto &eyeglasses = directionType.choice().asEyeglasses();
    api::EyeglassesData outEyeglasses;
    outEyeglasses.positionData = getPositionData(eyeglasses);
    outEyeglasses.fontData = getFontData(eyeglasses);
    if (eyeglasses.color().has_value())
    {
        outEyeglasses.color = getColor(eyeglasses);
    }
    outEyeglasses.id = getId(eyeglasses);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outEyeglasses)});
}

void DirectionReader::parseStringMute(const core::DirectionType &directionType)
{
    const auto &stringMute = directionType.choice().asStringMute();
    api::StringMuteData outStringMute;
    outStringMute.type =
        stringMute.type().tag() == core::OnOff::Tag::off ? api::StringMuteType::off : api::StringMuteType::on;
    outStringMute.positionData = getPositionData(stringMute);
    outStringMute.fontData = getFontData(stringMute);
    if (stringMute.color().has_value())
    {
        outStringMute.color = getColor(stringMute);
    }
    outStringMute.id = getId(stringMute);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outStringMute)});
}

void DirectionReader::parseStaffDivide(const core::DirectionType &directionType)
{
    const auto &staffDivide = directionType.choice().asStaffDivide();
    api::StaffDivideData outStaffDivide;
    switch (staffDivide.type().tag())
    {
    case core::StaffDivideSymbol::Tag::up:
        outStaffDivide.type = api::StaffDivideType::up;
        break;
    case core::StaffDivideSymbol::Tag::upDown:
        outStaffDivide.type = api::StaffDivideType::upDown;
        break;
    case core::StaffDivideSymbol::Tag::down:
    default:
        outStaffDivide.type = api::StaffDivideType::down;
        break;
    }
    outStaffDivide.positionData = getPositionData(staffDivide);
    outStaffDivide.fontData = getFontData(staffDivide);
    if (staffDivide.color().has_value())
    {
        outStaffDivide.color = getColor(staffDivide);
    }
    outStaffDivide.id = getId(staffDivide);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outStaffDivide)});
}

void DirectionReader::parseScordatura(const core::DirectionType &directionType)
{
    const auto &scordatura = directionType.choice().asScordatura();
    api::ScordaturaData outScordatura;
    for (const auto &accord : scordatura.accord())
    {
        api::AccordData outAccord;
        if (accord.string().has_value())
        {
            outAccord.stringNumber = accord.string()->value();
        }
        outAccord.tuningStep = myConverter.convert(accord.tuning().tuningStep());
        if (accord.tuning().tuningAlter().has_value())
        {
            const auto semitonesAndCents = Converter::convertToSemitonesAndCents(
                static_cast<double>(accord.tuning().tuningAlter()->value().value()));
            outAccord.tuningAlter = semitonesAndCents.first;
            outAccord.tuningCents = semitonesAndCents.second;
        }
        outAccord.tuningOctave = accord.tuning().tuningOctave().value();
        outScordatura.accords.emplace_back(outAccord);
    }
    outScordatura.id = getId(scordatura);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outScordatura)});
}

void DirectionReader::parseImage(const core::DirectionType &directionType)
{
    const auto &image = directionType.choice().asImage();
    api::ImageData outImage;
    outImage.source = image.source();
    outImage.type = image.type();
    if (image.height().has_value())
    {
        outImage.height = static_cast<double>(image.height()->value().value());
    }
    if (image.width().has_value())
    {
        outImage.width = static_cast<double>(image.width()->value().value());
    }
    outImage.positionData = getPositionData(image);
    // <image>'s valign is the valign-image type (no baseline), which the generic position
    // helper cannot read; take it from the element directly.
    if (image.valign().has_value())
    {
        switch (image.valign()->tag())
        {
        case core::ValignImage::Tag::top:
            outImage.positionData.verticalAlignment = api::VerticalAlignment::top;
            break;
        case core::ValignImage::Tag::middle:
            outImage.positionData.verticalAlignment = api::VerticalAlignment::middle;
            break;
        case core::ValignImage::Tag::bottom:
            outImage.positionData.verticalAlignment = api::VerticalAlignment::bottom;
            break;
        default:
            break;
        }
    }
    else
    {
        outImage.positionData.verticalAlignment = api::VerticalAlignment::unspecified;
    }
    outImage.id = getId(image);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outImage)});
}

void DirectionReader::parsePrincipalVoice(const core::DirectionType &directionType)
{
    const auto &principalVoice = directionType.choice().asPrincipalVoice();
    api::PrincipalVoiceData outPrincipalVoice;
    outPrincipalVoice.type = principalVoice.type().tag() == core::StartStop::Tag::stop ? api::PrincipalVoiceType::stop
                                                                                       : api::PrincipalVoiceType::start;
    switch (principalVoice.symbol().tag())
    {
    case core::PrincipalVoiceSymbol::Tag::nebenstimme:
        outPrincipalVoice.symbol = api::PrincipalVoiceSymbol::nebenstimme;
        break;
    case core::PrincipalVoiceSymbol::Tag::plain:
        outPrincipalVoice.symbol = api::PrincipalVoiceSymbol::plain;
        break;
    case core::PrincipalVoiceSymbol::Tag::none:
        outPrincipalVoice.symbol = api::PrincipalVoiceSymbol::none;
        break;
    case core::PrincipalVoiceSymbol::Tag::hauptstimme:
    default:
        outPrincipalVoice.symbol = api::PrincipalVoiceSymbol::hauptstimme;
        break;
    }
    outPrincipalVoice.text = principalVoice.value();
    outPrincipalVoice.positionData = getPositionData(principalVoice);
    outPrincipalVoice.fontData = getFontData(principalVoice);
    if (principalVoice.color().has_value())
    {
        outPrincipalVoice.color = getColor(principalVoice);
    }
    outPrincipalVoice.id = getId(principalVoice);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outPrincipalVoice)});
}

void DirectionReader::parseAccordionRegistration(const core::DirectionType &directionType)
{
    const auto &accordion = directionType.choice().asAccordionRegistration();
    api::AccordionRegistrationData outAccordion;
    outAccordion.high = accordion.accordionHigh();
    if (accordion.accordionMiddle().has_value())
    {
        outAccordion.middle = accordion.accordionMiddle()->value();
    }
    outAccordion.low = accordion.accordionLow();
    outAccordion.positionData = getPositionData(accordion);
    outAccordion.fontData = getFontData(accordion);
    if (accordion.color().has_value())
    {
        outAccordion.color = getColor(accordion);
    }
    outAccordion.id = getId(accordion);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outAccordion)});
}

api::PercussionDataChoice DirectionReader::getPercussionChoice(const core::PercussionChoice &choice) const
{
    using K = core::PercussionChoice::Kind;
    switch (choice.kind())
    {
    case K::glass: {
        api::GlassPercussion glass;
        glass.value = myConverter.convert(choice.asGlass().value());
        if (choice.asGlass().smufl().has_value())
        {
            glass.smufl = choice.asGlass().smufl()->toString();
        }
        return api::PercussionDataChoice{glass};
    }
    case K::metal: {
        api::MetalPercussion metal;
        metal.value = myConverter.convert(choice.asMetal().value());
        if (choice.asMetal().smufl().has_value())
        {
            metal.smufl = choice.asMetal().smufl()->toString();
        }
        return api::PercussionDataChoice{metal};
    }
    case K::wood: {
        api::WoodPercussion wood;
        wood.value = myConverter.convert(choice.asWood().value());
        if (choice.asWood().smufl().has_value())
        {
            wood.smufl = choice.asWood().smufl()->toString();
        }
        return api::PercussionDataChoice{wood};
    }
    case K::pitched: {
        api::PitchedPercussion pitched;
        pitched.value = myConverter.convert(choice.asPitched().value());
        if (choice.asPitched().smufl().has_value())
        {
            pitched.smufl = choice.asPitched().smufl()->toString();
        }
        return api::PercussionDataChoice{pitched};
    }
    case K::membrane: {
        api::MembranePercussion membrane;
        membrane.value = myConverter.convert(choice.asMembrane().value());
        if (choice.asMembrane().smufl().has_value())
        {
            membrane.smufl = choice.asMembrane().smufl()->toString();
        }
        return api::PercussionDataChoice{membrane};
    }
    case K::effect: {
        api::EffectPercussion effect;
        effect.value = myConverter.convert(choice.asEffect().value());
        if (choice.asEffect().smufl().has_value())
        {
            effect.smufl = choice.asEffect().smufl()->toString();
        }
        return api::PercussionDataChoice{effect};
    }
    case K::timpani: {
        api::TimpaniPercussion timpani;
        if (choice.asTimpani().smufl().has_value())
        {
            timpani.smufl = choice.asTimpani().smufl()->toString();
        }
        return api::PercussionDataChoice{timpani};
    }
    case K::beater: {
        api::BeaterPercussion beater;
        beater.value = myConverter.convert(choice.asBeater().value());
        if (choice.asBeater().tip().has_value())
        {
            beater.tip = myConverter.convert(*choice.asBeater().tip());
        }
        return api::PercussionDataChoice{beater};
    }
    case K::stick: {
        api::StickPercussion stick;
        stick.stickType = myConverter.convert(choice.asStick().stickType());
        stick.stickMaterial = myConverter.convert(choice.asStick().stickMaterial());
        if (choice.asStick().tip().has_value())
        {
            stick.tip = myConverter.convert(*choice.asStick().tip());
        }
        if (choice.asStick().parentheses().has_value())
        {
            stick.parentheses = myConverter.convert(*choice.asStick().parentheses());
        }
        if (choice.asStick().dashedCircle().has_value())
        {
            stick.dashedCircle = myConverter.convert(*choice.asStick().dashedCircle());
        }
        return api::PercussionDataChoice{stick};
    }
    case K::stickLocation: {
        return api::PercussionDataChoice{myConverter.convert(choice.asStickLocation())};
    }
    case K::otherPercussion:
    default: {
        api::OtherPercussion other;
        other.text = choice.asOtherPercussion().value();
        if (choice.asOtherPercussion().smufl().has_value())
        {
            other.smufl = choice.asOtherPercussion().smufl()->toString();
        }
        return api::PercussionDataChoice{other};
    }
    }
}

void DirectionReader::parsePercussion(const core::DirectionType &directionType)
{
    const auto &percussionSet = directionType.choice().asPercussion();
    for (const auto &percussion : percussionSet.items())
    {
        api::PercussionData outPercussion;
        outPercussion.choice = getPercussionChoice(percussion.choice());
        if (percussion.enclosure().has_value())
        {
            outPercussion.enclosure = myConverter.convert(*percussion.enclosure());
        }
        outPercussion.positionData = getPositionData(percussion);
        outPercussion.fontData = getFontData(percussion);
        if (percussion.color().has_value())
        {
            outPercussion.color = getColor(percussion);
        }
        outPercussion.id = getId(percussion);
        myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outPercussion)});
    }
}

void DirectionReader::parseOtherDirection(const core::DirectionType &directionType)
{
    const auto &otherDirection = directionType.choice().asOtherDirection();
    api::OtherDirectionData outOtherDirection;
    outOtherDirection.text = otherDirection.value();
    outOtherDirection.printObject = getPrintObject(otherDirection);
    if (otherDirection.smufl().has_value())
    {
        outOtherDirection.smufl = otherDirection.smufl()->toString();
    }
    outOtherDirection.positionData = getPositionData(otherDirection);
    outOtherDirection.fontData = getFontData(otherDirection);
    if (otherDirection.color().has_value())
    {
        outOtherDirection.color = getColor(otherDirection);
    }
    outOtherDirection.id = getId(otherDirection);
    myOutDirectionData.directionTypes.emplace_back(api::DirectionChoice{std::move(outOtherDirection)});
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

    if (kind.stackDegrees().has_value())
    {
        const bool isYes = kind.stackDegrees()->tag() == mx::core::YesNo::Tag::yes;
        chord.stackDegrees = isYes ? api::Bool::yes : api::Bool::no;
    }

    if (kind.parenthesesDegrees().has_value())
    {
        const bool isYes = kind.parenthesesDegrees()->tag() == mx::core::YesNo::Tag::yes;
        chord.parenthesesDegrees = isYes ? api::Bool::yes : api::Bool::no;
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
        if (frame.unplayed().has_value())
        {
            chord.frameData.unplayed = *frame.unplayed();
        }

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
}
} // namespace impl
} // namespace mx
