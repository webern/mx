// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DirectionWriter.h"
#include "mx/api/BarlineData.h"
#include "mx/core/generated/Bass.h"
#include "mx/core/generated/BassStep.h"
#include "mx/core/generated/BeatUnitGroup.h"
#include "mx/core/generated/Bracket.h"
#include "mx/core/generated/Dashes.h"
#include "mx/core/generated/Degree.h"
#include "mx/core/generated/DegreeAlter.h"
#include "mx/core/generated/DegreeType.h"
#include "mx/core/generated/DegreeTypeValue.h"
#include "mx/core/generated/DegreeValue.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/DirectionType.h"
#include "mx/core/generated/DirectionTypeChoice.h"
#include "mx/core/generated/DirectionTypeChoiceChoice.h"
#include "mx/core/generated/Divisions.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/Empty.h"
#include "mx/core/generated/FirstFret.h"
#include "mx/core/generated/FormattedTextID.h"
#include "mx/core/generated/Frame.h"
#include "mx/core/generated/FrameNote.h"
#include "mx/core/generated/Fret.h"
#include "mx/core/generated/Harmony.h"
#include "mx/core/generated/HarmonyAlter.h"
#include "mx/core/generated/HarmonyChordGroup.h"
#include "mx/core/generated/HarmonyChordGroupChoice.h"
#include "mx/core/generated/Kind.h"
#include "mx/core/generated/Metronome.h"
#include "mx/core/generated/MetronomeChoice.h"
#include "mx/core/generated/MetronomeChoiceGroup.h"
#include "mx/core/generated/MetronomeChoiceGroupChoice.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/OctaveShift.h"
#include "mx/core/generated/Offset.h"
#include "mx/core/generated/Pedal.h"
#include "mx/core/generated/PedalType.h"
#include "mx/core/generated/PerMinute.h"
#include "mx/core/generated/Root.h"
#include "mx/core/generated/RootStep.h"
#include "mx/core/generated/Semitones.h"
#include "mx/core/generated/Sound.h"
#include "mx/core/generated/StartStop.h"
#include "mx/core/generated/StartStopContinue.h"
#include "mx/core/generated/String.h"
#include "mx/core/generated/StringNumber.h"
#include "mx/core/generated/Wedge.h"
#include "mx/core/generated/WedgeType.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/DynamicsWriter.h"
#include "mx/impl/FontFunctions.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/SoundFunctions.h"
#include "mx/impl/SpannerFunctions.h"
#include "mx/utility/Throw.h"
#include "mx/utility/Unused.h"

namespace mx
{
namespace impl
{
DirectionWriter::DirectionWriter(const api::DirectionData &inDirectionData, const Cursor &inCursor)
    : myDirectionData{inDirectionData}, myCursor{inCursor}, myConverter{}, myPlacements{},
      myIsFirstDirectionTypeAdded{false}
{
}

std::vector<core::MusicDataChoice> DirectionWriter::getDirectionLikeThings()
{
    std::vector<core::MusicDataChoice> output{};
    core::Direction direction{};
    myPlacements.clear();
    myIsFirstDirectionTypeAdded = false;

    if (myDirectionData.placement != api::Placement::unspecified)
    {
        direction.setPlacement(myConverter.convert(myDirectionData.placement));
    }

    if (myDirectionData.isStaffValueSpecified || myCursor.staffIndex != 0)
    {
        direction.setStaff(myCursor.staffIndex + 1);
    }

    int offset = 0;

    if (myDirectionData.tickTimePosition != myCursor.tickTimePosition)
    {
        offset = myDirectionData.tickTimePosition - myCursor.tickTimePosition;
        core::Offset coreOffset{};
        coreOffset.setValue(core::Divisions{core::Decimal{static_cast<double>(offset)}});
        coreOffset.setSound(core::YesNo::yes());
        direction.setOffset(coreOffset);
    }

    for (auto mark : myDirectionData.marks)
    {
        mark.tickTimePosition = myDirectionData.tickTimePosition;

        // TODO - skip marks that aren't of the correct type (i.e. direction marks)
        // if !isDirection( mark ) continue;
        if (isMarkDynamic(mark.markType))
        {
            DynamicsWriter dynamicsWriter{mark, myCursor};
            core::OneOrMore<core::Dynamics> dynamicsSet{dynamicsWriter.getDynamics()};
            core::DirectionType dt{};
            dt.setChoice(core::DirectionTypeChoice::dynamics(dynamicsSet));
            addDirectionType(std::move(dt), direction);
        }

        if (isMarkPedal(mark.markType))
        {
            core::Pedal pedal{};

            if (mark.positionData.placement != api::Placement::unspecified)
            {
                Converter c;
                direction.setPlacement(c.convert(mark.positionData.placement));
            }

            if (mark.markType == api::MarkType::pedal)
            {
                pedal.setType(core::PedalType::start());
            }
            else if (mark.markType == api::MarkType::damp)
            {
                pedal.setType(core::PedalType::stop());
            }

            pedal.setLine(core::YesNo::no());
            pedal.setSign(core::YesNo::yes());
            setAttributesFromPositionData(mark.positionData, pedal);
            core::DirectionType dt{};
            dt.setChoice(core::DirectionTypeChoice::pedal(pedal));
            addDirectionType(std::move(dt), direction);
        }
    }

    for (const auto &pedalStart : myDirectionData.pedalStarts)
    {
        core::Pedal pedal{};
        pedal.setType(core::PedalType::start());
        pedal.setLine(core::YesNo::yes());
        pedal.setSign(core::YesNo::yes());
        setAttributesFromPositionData(pedalStart.positionData, pedal);
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::pedal(pedal));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &pedalStop : myDirectionData.pedalStops)
    {
        core::Pedal pedal{};
        pedal.setType(core::PedalType::stop());
        pedal.setLine(core::YesNo::yes());
        pedal.setSign(core::YesNo::yes());
        setAttributesFromPositionData(pedalStop.positionData, pedal);
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::pedal(pedal));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &wedgeStop : myDirectionData.wedgeStops)
    {
        core::Wedge wedge{};
        wedge.setType(core::WedgeType::stop());

        if (wedgeStop.isSpreadSpecified)
        {
            wedge.setSpread(core::Tenths{core::Decimal{static_cast<double>(wedgeStop.spread)}});
        }
        setAttributesFromPositionData(wedgeStop.positionData, wedge);
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::wedge(wedge));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &wedgeStart : myDirectionData.wedgeStarts)
    {
        core::Wedge wedge{};

        if (wedgeStart.wedgeType != api::WedgeType::unspecified)
        {
            wedge.setType(myConverter.convert(wedgeStart.wedgeType));
        }

        if (wedgeStart.isSpreadSpecified)
        {
            wedge.setSpread(core::Tenths{core::Decimal{static_cast<double>(wedgeStart.spread)}});
        }

        setAttributesFromPositionData(wedgeStart.positionData, wedge);
        setAttributesFromLineData(wedgeStart.lineData, wedge);
        setAttributesFromColorData(wedgeStart.colorData, wedge);
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::wedge(wedge));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &ottavaStop : myDirectionData.ottavaStops)
    {
        core::OctaveShift os{};
        os.setType(core::UpDownStopContinue::stop());
        MX_UNUSED(ottavaStop);
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::octaveShift(os));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &ottavaStart : myDirectionData.ottavaStarts)
    {
        core::OctaveShift os{};
        impl::setAttributesFromLineData(ottavaStart.spannerStart.lineData, os);

        switch (ottavaStart.ottavaType)
        {
        case api::OttavaType::o15ma: {
            os.setType(core::UpDownStopContinue::up());
            os.setSize(15);
            break;
        }
        case api::OttavaType::o15mb: {
            os.setType(core::UpDownStopContinue::down());
            os.setSize(15);
            break;
        }
        case api::OttavaType::o8va: {
            os.setType(core::UpDownStopContinue::up());
            os.setSize(8);
            break;
        }
        case api::OttavaType::o8vb: {
            os.setType(core::UpDownStopContinue::down());
            os.setSize(8);
            break;
        }
        default:
            break;
        }

        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::octaveShift(os));
        addDirectionType(std::move(dt), direction);
    }

    const auto setBracketLineData = [&](const api::LineData &lineData, core::Bracket &bracket) {
        bracket.setLineEnd(lineData.lineHook == api::LineHook::unspecified ? core::LineEnd::none()
                                                                           : myConverter.convert(lineData.lineHook));

        if (lineData.lineType != api::LineType::unspecified)
        {
            bracket.setLineType(myConverter.convert(lineData.lineType));
        }
        if (lineData.isStopLengthSpecified)
        {
            bracket.setEndLength(core::Tenths{core::Decimal{static_cast<double>(lineData.endLength)}});
        }
        if (lineData.isDashLengthSpecified)
        {
            bracket.setDashLength(core::Tenths{core::Decimal{static_cast<double>(lineData.dashLength)}});
        }
        if (lineData.isSpaceLengthSpecified)
        {
            bracket.setSpaceLength(core::Tenths{core::Decimal{static_cast<double>(lineData.spaceLength)}});
        }
    };

    for (const auto &item : myDirectionData.bracketStarts)
    {
        core::Bracket bracket{};
        setAttributesFromSpannerStart(item, bracket);
        bracket.setType(core::StartStopContinue::start());
        setAttributesFromPositionData(item.positionData, bracket);
        setAttributesFromPrintData(item.printData, bracket);
        setBracketLineData(item.lineData, bracket);
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::bracket(bracket));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &item : myDirectionData.bracketStops)
    {
        core::Bracket bracket{};
        setAttributesFromSpannerStop(item, bracket);
        bracket.setType(core::StartStopContinue::stop());
        setBracketLineData(item.lineData, bracket);
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::bracket(bracket));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &item : myDirectionData.dashesStarts)
    {
        core::Dashes dashes{};
        setAttributesFromSpannerStart(item, dashes);
        dashes.setType(core::StartStopContinue::start());
        setAttributesFromPositionData(item.positionData, dashes);
        setAttributesFromPrintData(item.printData, dashes);
        setAttributesFromLineData(item.lineData, dashes);
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::dashes(dashes));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &item : myDirectionData.dashesStops)
    {
        core::Dashes dashes{};
        setAttributesFromSpannerStop(item, dashes);
        dashes.setType(core::StartStopContinue::stop());
        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::dashes(dashes));
        addDirectionType(std::move(dt), direction);
    }

    for (const auto &tempo : myDirectionData.tempos)
    {
        if (tempo.tempoType != api::TempoType::beatsPerMinute)
        {
            MX_THROW("Only api::TempoType::beatsPerMinute is supported, others are not implemented");
        }

        core::BeatUnitGroup beatUnitGroup{};
        Converter converter;
        beatUnitGroup.setBeatUnit(converter.convert(tempo.beatsPerMinute.durationName));

        for (int d = 0; d < tempo.beatsPerMinute.dots; ++d)
        {
            beatUnitGroup.addBeatUnitDot(core::Empty{});
        }

        core::PerMinute pm{};
        pm.setValue(std::to_string(tempo.beatsPerMinute.beatsPerMinute));

        core::MetronomeChoiceGroupChoice mgChoice = core::MetronomeChoiceGroupChoice::perMinute(pm);

        core::MetronomeChoiceGroup mcg{};
        mcg.setBeatUnit(beatUnitGroup);
        mcg.setChoice(mgChoice);

        core::MetronomeChoice metChoice = core::MetronomeChoice::group(mcg);

        core::Metronome metronome{};
        metronome.setChoice(metChoice);

        core::DirectionType dt{};
        dt.setChoice(core::DirectionTypeChoice::metronome(metronome));
        addDirectionType(std::move(dt), direction);
    }

    if (myDirectionData.words.size() > 0)
    {
        bool isFirstWordsAdded = false;
        core::OneOrMore<core::DirectionTypeChoiceChoice> choiceSet{core::DirectionTypeChoiceChoice{}};

        for (const auto &wordsData : myDirectionData.words)
        {
            core::FormattedTextID outWords{};
            outWords.setValue(wordsData.text);
            setAttributesFromPositionData(wordsData.positionData, outWords);
            setAttributesFromFontData(wordsData.fontData, outWords);

            if (wordsData.isColorSpecified)
            {
                // TODO - oops color is missing from words
                setAttributesFromColorData(wordsData.colorData, outWords);
            }

            auto choiceItem = core::DirectionTypeChoiceChoice::words(outWords);

            if (!isFirstWordsAdded)
            {
                isFirstWordsAdded = true;
                choiceSet = core::OneOrMore<core::DirectionTypeChoiceChoice>{choiceItem};
            }
            else
            {
                choiceSet.add(choiceItem);
            }
        }

        if (isFirstWordsAdded)
        {
            core::DirectionType dt{};
            dt.setChoice(core::DirectionTypeChoice::choice(choiceSet));
            addDirectionType(std::move(dt), direction);
        }
    }

    if (myIsFirstDirectionTypeAdded)
    {
        // The direction has other content; attach the <sound> as a child of the <direction>.
        if (myDirectionData.isSoundDataSpecified && myDirectionData.soundData.isSpecified())
        {
            core::Sound sound{};
            writeSoundData(myDirectionData.soundData, sound);
            direction.setSound(std::move(sound));
        }

        output.push_back(core::MusicDataChoice::direction(direction));
    }
    else if (myDirectionData.isSoundDataSpecified && myDirectionData.soundData.isSpecified())
    {
        // The direction has no other content; emit a standalone <sound> element.
        core::Sound sound{};
        writeSoundData(myDirectionData.soundData, sound);

        if (offset != 0)
        {
            core::Offset coreOffset{};
            coreOffset.setValue(core::Divisions{core::Decimal{static_cast<double>(offset)}});
            coreOffset.setSound(core::YesNo::yes());
            sound.setOffset(coreOffset);
        }

        output.push_back(core::MusicDataChoice::sound(std::move(sound)));
    }

    auto harmonyMdcs = createHarmonyElements(offset);
    addMusicDataChoices(harmonyMdcs, output);

    // clear state
    myPlacements.clear();
    myIsFirstDirectionTypeAdded = false;
    return output;
}

void DirectionWriter::addDirectionType(core::DirectionType directionType, core::Direction &ioDirection)
{
    if (!myIsFirstDirectionTypeAdded)
    {
        // Replace the default-constructed direction type (OneOrMore starts with one default item).
        ioDirection.setDirectionType(core::OneOrMore<core::DirectionType>{std::move(directionType)});
        myIsFirstDirectionTypeAdded = true;
    }
    else
    {
        ioDirection.addDirectionType(std::move(directionType));
    }
}

std::vector<core::MusicDataChoice> DirectionWriter::createHarmonyElements(int inOffset)
{
    if (myDirectionData.chords.empty())
    {
        return std::vector<core::MusicDataChoice>{};
    }

    std::vector<core::MusicDataChoice> output;
    core::Harmony harmony{};

    if (inOffset != 0)
    {
        core::Offset coreOffset{};
        coreOffset.setValue(core::Divisions{core::Decimal{static_cast<double>(inOffset)}});
        harmony.setOffset(coreOffset);
    }

    if (myDirectionData.isStaffValueSpecified)
    {
        harmony.setStaff(myCursor.staffIndex + 1);
    }

    const auto &chords = myDirectionData.chords;

    auto chordIter = chords.cbegin();
    const auto chordEnd = chords.cend();

    bool isFirstChord = true;
    bool isFirstHarmonyChordGroup = true;

    for (; chordIter != chordEnd; ++chordIter)
    {
        if (isFirstChord)
        {
            isFirstChord = false;
            setAttributesFromPositionData(chordIter->positionData, harmony);
        }

        core::HarmonyChordGroup grp{};
        auto step = chordIter->root == api::Step::unspecified ? api::Step::c : chordIter->root;

        core::Root root{};
        core::RootStep rootStep{};
        rootStep.setValue(myConverter.convert(step));
        root.setRootStep(rootStep);

        if (chordIter->rootAlter != 0)
        {
            core::HarmonyAlter rootAlter{};
            rootAlter.setValue(core::Semitones{core::Decimal{static_cast<double>(chordIter->rootAlter)}});
            root.setRootAlter(rootAlter);
        }

        grp.setChoice(core::HarmonyChordGroupChoice::root(root));

        if (chordIter->bass != api::Step::unspecified)
        {
            core::Bass bass{};
            core::BassStep bassStep{};
            bassStep.setValue(myConverter.convert(chordIter->bass));
            bass.setBassStep(bassStep);

            if (chordIter->bassAlter != 0)
            {
                core::HarmonyAlter bassAlter{};
                bassAlter.setValue(core::Semitones{core::Decimal{static_cast<double>(chordIter->bassAlter)}});
                bass.setBassAlter(bassAlter);
            }

            grp.setBass(bass);
        }

        const auto k = myConverter.convert(chordIter->chordKind);
        core::Kind kind{};
        kind.setValue(k);

        if (!chordIter->text.empty())
        {
            kind.setText(chordIter->text);
        }

        if (chordIter->useSymbols != api::Bool::unspecified)
        {
            kind.setUseSymbols(chordIter->useSymbols == api::Bool::yes ? core::YesNo::yes() : core::YesNo::no());
        }

        grp.setKind(kind);

        for (const auto &extension : chordIter->extensions)
        {
            core::Degree degree{};
            core::DegreeType degreeType{};
            core::DegreeValue degreeValue{};
            core::DegreeAlter degreeAlter{};

            if (extension.extensionType == api::ExtensionType::add)
            {
                degreeType.setValue(core::DegreeTypeValue::add());
            }
            else if (extension.extensionType == api::ExtensionType::remove)
            {
                degreeType.setValue(core::DegreeTypeValue::subtract());
            }
            else if (extension.extensionType == api::ExtensionType::alter)
            {
                degreeType.setValue(core::DegreeTypeValue::alter());
            }

            switch (extension.extensionNumber)
            {
            case api::ExtensionNumber::first:
                degreeValue.setValue(1);
                break;
            case api::ExtensionNumber::second:
                degreeValue.setValue(2);
                break;
            case api::ExtensionNumber::third:
                degreeValue.setValue(3);
                break;
            case api::ExtensionNumber::fourth:
                degreeValue.setValue(4);
                break;
            case api::ExtensionNumber::fifth:
                degreeValue.setValue(5);
                break;
            case api::ExtensionNumber::sixth:
                degreeValue.setValue(6);
                break;
            case api::ExtensionNumber::seventh:
                degreeValue.setValue(7);
                break;
            case api::ExtensionNumber::ninth:
                degreeValue.setValue(9);
                break;
            case api::ExtensionNumber::eleventh:
                degreeValue.setValue(11);
                break;
            case api::ExtensionNumber::thirteenth:
                degreeValue.setValue(13);
                break;
            default:
                break;
            }

            switch (extension.extensionAlter)
            {
            case api::ExtensionAlter::doubleFlat:
                degreeAlter.setValue(core::Semitones{core::Decimal{-2.0}});
                break;
            case api::ExtensionAlter::doubleSharp:
                degreeAlter.setValue(core::Semitones{core::Decimal{2.0}});
                break;
            case api::ExtensionAlter::flat:
                degreeAlter.setValue(core::Semitones{core::Decimal{-1.0}});
                break;
            case api::ExtensionAlter::sharp:
                degreeAlter.setValue(core::Semitones{core::Decimal{1.0}});
                break;
            case api::ExtensionAlter::none:
                degreeAlter.setValue(core::Semitones{core::Decimal{0.0}});
                break;
            }

            degree.setDegreeType(degreeType);
            degree.setDegreeValue(degreeValue);
            degree.setDegreeAlter(degreeAlter);
            grp.addDegree(degree);
        }

        // Note: ProcessingInstruction is not available in the new core; miscData is skipped.

        if (chordIter->hasFrameData)
        {
            core::Frame frame{};
            frame.setFrameStrings(chordIter->frameData.stringCount);
            frame.setFrameFrets(chordIter->frameData.fretCount);

            if (chordIter->frameData.isFirstFretSpecified)
            {
                core::FirstFret firstFret{};
                firstFret.setValue(chordIter->frameData.firstFret);
                frame.setFirstFret(firstFret);
            }

            for (const auto &noteData : chordIter->frameData.notes)
            {
                core::FrameNote frameNote{};
                core::String strObj{};
                strObj.setValue(core::StringNumber{noteData.stringNumber});
                frameNote.setString(strObj);

                core::Fret fret{};
                fret.setValue(noteData.fretNumber);
                frameNote.setFret(fret);

                if (noteData.isFingeringSpecified)
                {
                    core::Fingering fingering{};
                    fingering.setValue(std::to_string(noteData.fingering));
                    frameNote.setFingering(fingering);
                }

                if (noteData.barre != api::FrameBarre::none)
                {
                    core::Barre barre{};
                    barre.setType(noteData.barre == api::FrameBarre::start ? core::StartStop::start()
                                                                           : core::StartStop::stop());
                    frameNote.setBarre(barre);
                }

                frame.addFrameNote(frameNote);
            }

            harmony.setFrame(frame);
        }

        if (isFirstHarmonyChordGroup)
        {
            isFirstHarmonyChordGroup = false;
            harmony.setHarmonyChord(core::OneOrMore<core::HarmonyChordGroup>{grp});
        }
        else
        {
            harmony.addHarmonyChord(grp);
        }
    }

    if (!chords.empty())
    {
        output.push_back(core::MusicDataChoice::harmony(harmony));
    }

    return output;
}

void DirectionWriter::addMusicDataChoices(const std::vector<core::MusicDataChoice> &inMdcs,
                                          std::vector<core::MusicDataChoice> &ioOutputSet)
{
    for (const auto &mdc : inMdcs)
    {
        ioOutputSet.push_back(mdc);
    }
}
} // namespace impl
} // namespace mx
