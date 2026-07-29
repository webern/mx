// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DirectionWriter.h"
#include "mx/api/BarlineData.h"
#include "mx/core/generated/Accord.h"
#include "mx/core/generated/AccordionMiddle.h"
#include "mx/core/generated/AccordionRegistration.h"
#include "mx/core/generated/Bass.h"
#include "mx/core/generated/BassStep.h"
#include "mx/core/generated/BeamLevel.h"
#include "mx/core/generated/BeatUnitGroup.h"
#include "mx/core/generated/BeatUnitTied.h"
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
#include "mx/core/generated/DirectionTypeChoiceChoice.h"
#include "mx/core/generated/Divisions.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/EditorialVoiceDirectionGroup.h"
#include "mx/core/generated/Effect.h"
#include "mx/core/generated/Empty.h"
#include "mx/core/generated/EmptyPrintStyleAlignID.h"
#include "mx/core/generated/Figure.h"
#include "mx/core/generated/FiguredBass.h"
#include "mx/core/generated/FirstFret.h"
#include "mx/core/generated/FormattedTextID.h"
#include "mx/core/generated/Frame.h"
#include "mx/core/generated/FrameNote.h"
#include "mx/core/generated/Fret.h"
#include "mx/core/generated/Glass.h"
#include "mx/core/generated/Harmony.h"
#include "mx/core/generated/HarmonyAlter.h"
#include "mx/core/generated/HarmonyChordGroup.h"
#include "mx/core/generated/HarmonyChordGroupChoice.h"
#include "mx/core/generated/HarpPedals.h"
#include "mx/core/generated/Image.h"
#include "mx/core/generated/Inversion.h"
#include "mx/core/generated/Kind.h"
#include "mx/core/generated/Membrane.h"
#include "mx/core/generated/Metal.h"
#include "mx/core/generated/Metronome.h"
#include "mx/core/generated/MetronomeBeam.h"
#include "mx/core/generated/MetronomeChoice.h"
#include "mx/core/generated/MetronomeChoiceGroup.h"
#include "mx/core/generated/MetronomeChoiceGroup2.h"
#include "mx/core/generated/MetronomeChoiceGroup2Group.h"
#include "mx/core/generated/MetronomeChoiceGroupChoice.h"
#include "mx/core/generated/MetronomeChoiceGroupChoiceGroup.h"
#include "mx/core/generated/MetronomeNote.h"
#include "mx/core/generated/MetronomeTied.h"
#include "mx/core/generated/MetronomeTuplet.h"
#include "mx/core/generated/MusicDataChoice.h"
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
#include "mx/core/generated/PerMinute.h"
#include "mx/core/generated/Percussion.h"
#include "mx/core/generated/PercussionChoice.h"
#include "mx/core/generated/Pitched.h"
#include "mx/core/generated/PositiveDivisions.h"
#include "mx/core/generated/PrincipalVoice.h"
#include "mx/core/generated/PrincipalVoiceSymbol.h"
#include "mx/core/generated/Root.h"
#include "mx/core/generated/RootStep.h"
#include "mx/core/generated/Scordatura.h"
#include "mx/core/generated/Segno.h"
#include "mx/core/generated/Semitones.h"
#include "mx/core/generated/ShowTuplet.h"
#include "mx/core/generated/SmuflGlyphName.h"
#include "mx/core/generated/SmuflPictogramGlyphName.h"
#include "mx/core/generated/Sound.h"
#include "mx/core/generated/StaffDivide.h"
#include "mx/core/generated/StaffDivideSymbol.h"
#include "mx/core/generated/StartStop.h"
#include "mx/core/generated/StartStopContinue.h"
#include "mx/core/generated/Stick.h"
#include "mx/core/generated/String.h"
#include "mx/core/generated/StringMute.h"
#include "mx/core/generated/StringNumber.h"
#include "mx/core/generated/StyleText.h"
#include "mx/core/generated/TimeModificationGroup.h"
#include "mx/core/generated/Timpani.h"
#include "mx/core/generated/TuningGroup.h"
#include "mx/core/generated/ValignImage.h"
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

namespace mx
{
namespace impl
{

static void applyBracketLineData(const api::LineData &lineData, core::Bracket &bracket, const Converter &converter)
{
    bracket.setLineEnd(lineData.lineHook == api::LineHook::unspecified ? core::LineEnd::none()
                                                                       : converter.convert(lineData.lineHook));

    if (lineData.lineType != api::LineType::unspecified)
    {
        bracket.setLineType(converter.convert(lineData.lineType));
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
}

DirectionWriter::DirectionWriter(const api::DirectionData &inDirectionData, const Cursor &inCursor,
                                 const SpannerNumberResolver &inNumberResolver)
    : myDirectionData{inDirectionData}, myCursor{inCursor}, myNumberResolver{inNumberResolver}, myConverter{},
      myPlacements{}, myIsFirstDirectionTypeAdded{false}
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

    if (myDirectionData.offset.has_value())
    {
        // The source carried an explicit <offset>: a drawn-position nudge measured from the
        // direction's anchor (tickTimePosition). Re-emit it verbatim, with no sound attribute -- an
        // <offset> moves only where the direction is drawn, so playback stays at the anchor.
        offset = *myDirectionData.offset;
        if (offset != 0)
        {
            core::Offset coreOffset{};
            coreOffset.setValue(core::Divisions{core::Decimal{static_cast<double>(offset)}});
            direction.setOffset(coreOffset);
        }
    }
    else if (myDirectionData.tickTimePosition != myCursor.tickTimePosition)
    {
        // No modeled offset, but the direction is anchored off the cursor tick: synthesize an
        // <offset> so the serialized location matches tickTimePosition, and carry playback with it
        // (sound="yes") since here the offset stands in for the note-stream position itself.
        offset = myDirectionData.tickTimePosition - myCursor.tickTimePosition;
        core::Offset coreOffset{};
        coreOffset.setValue(core::Divisions{core::Decimal{static_cast<double>(offset)}});
        coreOffset.setSound(core::YesNo::yes());
        direction.setOffset(coreOffset);
    }

    // Assign the direction to a voice via the editorial-voice <voice>. Only emitted when this
    // direction also writes direction-type content (guarded by myIsFirstDirectionTypeAdded below),
    // so a voice carried in from a figured-bass or harmony element never produces an empty
    // <direction>.
    if (myDirectionData.voice >= 0)
    {
        core::EditorialVoiceDirectionGroup editorialVoice{};
        editorialVoice.setVoice(std::to_string(myDirectionData.voice));
        direction.setEditorialVoiceDirection(std::move(editorialVoice));
    }

    emitDirectionTypes(direction);

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

    auto figuredBassMdcs = createFiguredBassElements();
    addMusicDataChoices(figuredBassMdcs, output);

    // clear state
    myPlacements.clear();
    myIsFirstDirectionTypeAdded = false;
    return output;
}

void DirectionWriter::emitMark(api::MarkData mark, core::Direction &direction)
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

core::PedalType corePedalType(api::PedalLineKind kind)
{
    switch (kind)
    {
    case api::PedalLineKind::start:
        return core::PedalType::start();
    case api::PedalLineKind::stop:
        return core::PedalType::stop();
    case api::PedalLineKind::sostenuto:
        return core::PedalType::sostenuto();
    case api::PedalLineKind::change:
        return core::PedalType::change();
    case api::PedalLineKind::continueLine:
        return core::PedalType::continue_();
    case api::PedalLineKind::discontinue:
        return core::PedalType::discontinue();
    case api::PedalLineKind::resume:
        return core::PedalType::resume();
    case api::PedalLineKind::unspecified:
        break;
    }
    return core::PedalType::start();
}

void DirectionWriter::emitPedal(const api::PedalLineData &item, core::Direction &direction)
{
    // An unspecified kind describes no pedal event; emit nothing rather than a guessed default.
    if (item.kind == api::PedalLineKind::unspecified)
    {
        return;
    }

    core::Pedal pedal{};
    pedal.setType(corePedalType(item.kind));
    pedal.setLine(core::YesNo::yes());
    setAttributesFromPositionData(item.positionData, pedal);
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::pedal(pedal));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitWedgeStop(const api::WedgeStop &wedgeStop, const void *inIdentity, core::Direction &direction)
{
    core::Wedge wedge{};
    wedge.setType(core::WedgeType::stop());

    const auto number = myNumberResolver.emittedNumber(wedgeStop.number, inIdentity);
    if (number.has_value())
    {
        wedge.setNumber(core::NumberLevel{*number});
    }

    if (wedgeStop.isSpreadSpecified)
    {
        wedge.setSpread(core::Tenths{core::Decimal{static_cast<double>(wedgeStop.spread)}});
    }
    setAttributesFromPositionData(wedgeStop.positionData, wedge);
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::wedge(wedge));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitWedgeStart(const api::WedgeStart &wedgeStart, const void *inIdentity,
                                     core::Direction &direction)
{
    core::Wedge wedge{};

    if (wedgeStart.wedgeType != api::WedgeType::unspecified)
    {
        wedge.setType(myConverter.convert(wedgeStart.wedgeType));
    }

    const auto number = myNumberResolver.emittedNumber(wedgeStart.number, inIdentity);
    if (number.has_value())
    {
        wedge.setNumber(core::NumberLevel{*number});
    }

    if (wedgeStart.isSpreadSpecified)
    {
        wedge.setSpread(core::Tenths{core::Decimal{static_cast<double>(wedgeStart.spread)}});
    }

    setAttributesFromPositionData(wedgeStart.positionData, wedge);
    setAttributesFromLineData(wedgeStart.lineData, wedge);
    if (wedgeStart.isColorSpecified)
    {
        setAttributesFromColorData(wedgeStart.colorData, wedge);
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::wedge(wedge));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitOttavaStop(const api::OttavaStop &ottavaStop, const void *inIdentity,
                                     core::Direction &direction)
{
    core::OctaveShift os{};
    setAttributesFromSpannerStop(ottavaStop.spannerStop, os,
                                 myNumberResolver.emittedNumber(ottavaStop.spannerStop.number, inIdentity));
    os.setType(core::UpDownStopContinue::stop());
    os.setSize(ottavaStop.size);
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::octaveShift(os));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitOttavaStart(const api::OttavaStart &ottavaStart, const void *inIdentity,
                                      core::Direction &direction)
{
    core::OctaveShift os{};
    impl::setAttributesFromPositionData(ottavaStart.spannerStart.positionData, os);
    impl::setAttributesFromPrintData(ottavaStart.spannerStart.printData, os);
    impl::setAttributesFromLineData(ottavaStart.spannerStart.lineData, os);

    const auto number = myNumberResolver.emittedNumber(ottavaStart.spannerStart.number, inIdentity);
    if (number.has_value())
    {
        os.setNumber(core::NumberLevel{*number});
    }

    int sizeValue = 8;

    switch (ottavaStart.ottavaType)
    {
    case api::OttavaType::o15ma: {
        os.setType(core::UpDownStopContinue::down());
        sizeValue = 15;
        break;
    }
    case api::OttavaType::o15mb: {
        os.setType(core::UpDownStopContinue::up());
        sizeValue = 15;
        break;
    }
    case api::OttavaType::o22ma: {
        os.setType(core::UpDownStopContinue::down());
        sizeValue = 22;
        break;
    }
    case api::OttavaType::o22mb: {
        os.setType(core::UpDownStopContinue::up());
        sizeValue = 22;
        break;
    }
    case api::OttavaType::o8va: {
        os.setType(core::UpDownStopContinue::down());
        sizeValue = 8;
        break;
    }
    case api::OttavaType::o8vb: {
        os.setType(core::UpDownStopContinue::up());
        sizeValue = 8;
        break;
    }
    default:
        break;
    }

    // size follows from ottavaType; non-8va lines need an explicit size to encode their shift, while
    // the redundant default size="8" is emitted only when writeDefaultSize is set (the source spelled
    // it out).
    if (sizeValue != 8 || ottavaStart.writeDefaultSize)
    {
        os.setSize(sizeValue);
    }

    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::octaveShift(os));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitBracketStart(const api::SpannerStart &item, const void *inIdentity,
                                       core::Direction &direction)
{
    core::Bracket bracket{};
    setAttributesFromSpannerStart(item, bracket, myNumberResolver.emittedNumber(item.number, inIdentity));
    bracket.setType(core::StartStopContinue::start());
    setAttributesFromPositionData(item.positionData, bracket);
    setAttributesFromPrintData(item.printData, bracket);
    applyBracketLineData(item.lineData, bracket, myConverter);
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::bracket(bracket));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitBracketStop(const api::SpannerStop &item, const void *inIdentity, core::Direction &direction)
{
    core::Bracket bracket{};
    setAttributesFromSpannerStop(item, bracket, myNumberResolver.emittedNumber(item.number, inIdentity));
    bracket.setType(core::StartStopContinue::stop());
    applyBracketLineData(item.lineData, bracket, myConverter);
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::bracket(bracket));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitDashesStart(const api::SpannerStart &item, const void *inIdentity, core::Direction &direction)
{
    core::Dashes dashes{};
    setAttributesFromSpannerStart(item, dashes, myNumberResolver.emittedNumber(item.number, inIdentity));
    dashes.setType(core::StartStopContinue::start());
    setAttributesFromPositionData(item.positionData, dashes);
    setAttributesFromPrintData(item.printData, dashes);
    setAttributesFromLineData(item.lineData, dashes);
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::dashes(dashes));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitDashesStop(const api::SpannerStop &item, const void *inIdentity, core::Direction &direction)
{
    core::Dashes dashes{};
    setAttributesFromSpannerStop(item, dashes, myNumberResolver.emittedNumber(item.number, inIdentity));
    dashes.setType(core::StartStopContinue::stop());
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::dashes(dashes));
    addDirectionType(std::move(dt), direction);
}

static core::BeatUnitGroup makeBeatUnitGroup(const Converter &converter, api::DurationName durationName, int dots)
{
    core::BeatUnitGroup beatUnitGroup{};
    beatUnitGroup.setBeatUnit(converter.convert(durationName));
    for (int d = 0; d < dots; ++d)
    {
        beatUnitGroup.addBeatUnitDot(core::Empty{});
    }
    return beatUnitGroup;
}

static core::BeatUnitTied makeBeatUnitTied(const Converter &converter, const api::BeatUnit &beatUnit)
{
    core::BeatUnitTied tied{};
    tied.setBeatUnit(makeBeatUnitGroup(converter, beatUnit.type, beatUnit.dots));
    return tied;
}

static core::MetronomeNote makeMetronomeNote(const Converter &converter, const api::MetronomeNoteData &note)
{
    core::MetronomeNote out{};
    out.setMetronomeType(converter.convert(note.metronomeType));
    for (int d = 0; d < note.dots; ++d)
    {
        out.addMetronomeDot(core::Empty{});
    }
    for (const auto &beam : note.beams)
    {
        core::MetronomeBeam coreBeam{};
        coreBeam.setValue(converter.convert(beam.value));
        if (beam.number.has_value())
        {
            coreBeam.setNumber(core::BeamLevel{*beam.number});
        }
        out.addMetronomeBeam(coreBeam);
    }
    if (note.tie.has_value())
    {
        core::MetronomeTied tied{};
        tied.setType(*note.tie == api::MetronomeTieType::start ? core::StartStop::start() : core::StartStop::stop());
        out.setMetronomeTied(tied);
    }
    if (note.tuplet.has_value())
    {
        const auto &tuplet = *note.tuplet;
        core::MetronomeTuplet coreTuplet{};
        coreTuplet.setActualNotes(tuplet.actualNotes);
        coreTuplet.setNormalNotes(tuplet.normalNotes);
        if (tuplet.normalType != api::DurationName::unspecified)
        {
            core::TimeModificationGroup group{};
            group.setNormalType(converter.convert(tuplet.normalType));
            for (int d = 0; d < tuplet.normalDots; ++d)
            {
                group.addNormalDot(core::Empty{});
            }
            coreTuplet.setGroup(group);
        }
        coreTuplet.setType(tuplet.type == api::MetronomeTupletType::start ? core::StartStop::start()
                                                                          : core::StartStop::stop());
        if (tuplet.bracket != api::Bool::unspecified)
        {
            coreTuplet.setBracket(converter.convert(tuplet.bracket));
        }
        switch (tuplet.showNumber)
        {
        case api::MetronomeShowNumber::actual:
            coreTuplet.setShowNumber(core::ShowTuplet::actual());
            break;
        case api::MetronomeShowNumber::both:
            coreTuplet.setShowNumber(core::ShowTuplet::both());
            break;
        case api::MetronomeShowNumber::none:
            coreTuplet.setShowNumber(core::ShowTuplet::none());
            break;
        case api::MetronomeShowNumber::unspecified:
            break;
        }
        out.setMetronomeTuplet(coreTuplet);
    }
    return out;
}

static core::OneOrMore<core::MetronomeNote> makeMetronomeNotes(const Converter &converter,
                                                               const std::vector<api::MetronomeNoteData> &notes)
{
    core::OneOrMore<core::MetronomeNote> out{makeMetronomeNote(converter, notes.front())};
    for (std::size_t i = 1; i < notes.size(); ++i)
    {
        out.add(makeMetronomeNote(converter, notes.at(i)));
    }
    return out;
}

void DirectionWriter::emitTempo(const api::TempoData &tempo, core::Direction &direction)
{
    const auto kind = tempo.choice.kind();
    core::Metronome metronome{};

    if (kind == api::TempoChoice::Kind::beatsPerMinute)
    {
        // beat-unit (+dots, +tied continuations) followed by a per-minute string, kept verbatim
        // from the source (per-minute is xs:string: "120", "ca. 76", a range, ...).
        const auto bpm = tempo.choice.beatsPerMinute();
        // Content-guard: a default/empty tempo has no beat-unit; skip rather than emit an empty
        // <metronome>. Nothing throws; the direction simply carries no tempo.
        if (bpm.durationName == api::DurationName::unspecified)
        {
            return;
        }
        core::PerMinute pm{};
        pm.setValue(bpm.beatsPerMinute);

        core::MetronomeChoiceGroup mcg{};
        mcg.setBeatUnit(makeBeatUnitGroup(myConverter, bpm.durationName, bpm.dots));
        for (const auto &tied : bpm.tiedBeatUnits)
        {
            mcg.addBeatUnitTied(makeBeatUnitTied(myConverter, tied));
        }
        mcg.setChoice(core::MetronomeChoiceGroupChoice::perMinute(pm));
        metronome.setChoice(core::MetronomeChoice::group(mcg));
    }
    else if (kind == api::TempoChoice::Kind::metricModulation)
    {
        // Metric modulation: two beat-units, e.g. <beat-unit>quarter</beat-unit>
        // = <beat-unit>half</beat-unit>. The second beat-unit is the 'group' alternative.
        const auto mm = tempo.choice.metricModulation();
        if (mm.leftDurationName == api::DurationName::unspecified)
        {
            return;
        }
        core::MetronomeChoiceGroupChoiceGroup rightBeatUnitHolder{};
        rightBeatUnitHolder.setBeatUnit(makeBeatUnitGroup(myConverter, mm.rightDurationName, mm.rightDots));
        for (const auto &tied : mm.rightTiedBeatUnits)
        {
            rightBeatUnitHolder.addBeatUnitTied(makeBeatUnitTied(myConverter, tied));
        }

        core::MetronomeChoiceGroup mcg{};
        mcg.setBeatUnit(makeBeatUnitGroup(myConverter, mm.leftDurationName, mm.leftDots));
        for (const auto &tied : mm.leftTiedBeatUnits)
        {
            mcg.addBeatUnitTied(makeBeatUnitTied(myConverter, tied));
        }
        mcg.setChoice(core::MetronomeChoiceGroupChoice::group(rightBeatUnitHolder));
        metronome.setChoice(core::MetronomeChoice::group(mcg));
    }
    else
    {
        // Note-relation form: one or more metronome-note figures, optionally followed by a
        // relation symbol and a second group of figures.
        const auto noteRelation = tempo.choice.noteRelation();
        if (noteRelation.notes.empty())
        {
            return;
        }
        core::MetronomeChoiceGroup2 group2{};
        group2.setMetronomeArrows(noteRelation.arrows);
        group2.setMetronomeNote(makeMetronomeNotes(myConverter, noteRelation.notes));
        if (noteRelation.relation.has_value() && !noteRelation.relation->notes.empty())
        {
            core::MetronomeChoiceGroup2Group relationGroup{};
            relationGroup.setMetronomeRelation(noteRelation.relation->symbol);
            relationGroup.setMetronomeNote(makeMetronomeNotes(myConverter, noteRelation.relation->notes));
            group2.setGroup(relationGroup);
        }
        metronome.setChoice(core::MetronomeChoice::group2(group2));
    }

    // print-style-align (default-x/y, relative-x/y, font, color, halign, valign) + justify +
    // print-object + parentheses + id.
    setAttributesFromPositionData(tempo.positionData, metronome);
    setAttributesFromFontData(tempo.fontData, metronome);
    if (tempo.color.has_value())
    {
        setAttributesFromColorData(*tempo.color, metronome);
    }
    if (tempo.id.has_value())
    {
        metronome.setID(core::Token{*tempo.id});
    }
    if (tempo.justify != api::HorizontalAlignment::unspecified)
    {
        metronome.setJustify(myConverter.convert(tempo.justify));
    }
    if (tempo.printObject != api::Bool::unspecified)
    {
        metronome.setPrintObject(myConverter.convert(tempo.printObject));
    }
    if (tempo.isParenthetical != api::Bool::unspecified)
    {
        metronome.setParentheses(myConverter.convert(tempo.isParenthetical));
    }

    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::metronome(metronome));
    addDirectionType(std::move(dt), direction);
}

// Maps the api enclosure enum shared by rehearsals, words, and symbols to the core
// enclosure-shape attribute; nullopt (for unspecified) emits no attribute.
static std::optional<core::EnclosureShape> directionWriterEnclosure(api::RehearsalEnclosure enclosure)
{
    switch (enclosure)
    {
    case api::RehearsalEnclosure::rectangle:
        return core::EnclosureShape::rectangle();
    case api::RehearsalEnclosure::square:
        return core::EnclosureShape::square();
    case api::RehearsalEnclosure::oval:
        return core::EnclosureShape::oval();
    case api::RehearsalEnclosure::circle:
        return core::EnclosureShape::circle();
    case api::RehearsalEnclosure::bracket:
        return core::EnclosureShape::bracket();
    case api::RehearsalEnclosure::triangle:
        return core::EnclosureShape::triangle();
    case api::RehearsalEnclosure::diamond:
        return core::EnclosureShape::diamond();
    case api::RehearsalEnclosure::none:
        return core::EnclosureShape::none();
    case api::RehearsalEnclosure::unspecified:
    default:
        return std::nullopt;
    }
}

void DirectionWriter::emitWordsRun(const std::vector<api::WordsChoice> &inRun, core::Direction &direction)
{
    // An empty run cannot be expressed (the schema requires at least one words or symbol) and is
    // not written.
    if (inRun.empty())
    {
        return;
    }

    bool isFirstItemAdded = false;
    core::OneOrMore<core::DirectionTypeChoiceChoice> choiceSet{core::DirectionTypeChoiceChoice{}};

    for (const auto &item : inRun)
    {
        core::DirectionTypeChoiceChoice choiceItem{};

        if (item.isSymbol())
        {
            const auto symbolData = item.symbol();
            core::FormattedSymbolID outSymbol{};
            outSymbol.setValue(core::SmuflGlyphName{symbolData.smufl});
            setAttributesFromPositionData(symbolData.positionData, outSymbol);
            setAttributesFromFontData(symbolData.fontData, outSymbol);
            if (symbolData.color.has_value())
            {
                setAttributesFromColorData(*symbolData.color, outSymbol);
            }
            outSymbol.setEnclosure(directionWriterEnclosure(symbolData.enclosure));
            choiceItem = core::DirectionTypeChoiceChoice::symbol(std::move(outSymbol));
        }
        else
        {
            const auto wordsData = item.words();
            core::FormattedTextID outWords{};
            outWords.setValue(wordsData.text);
            setAttributesFromPositionData(wordsData.positionData, outWords);
            setAttributesFromFontData(wordsData.fontData, outWords);
            if (wordsData.isColorSpecified)
            {
                setAttributesFromColorData(wordsData.colorData, outWords);
            }
            outWords.setEnclosure(directionWriterEnclosure(wordsData.enclosure));
            choiceItem = core::DirectionTypeChoiceChoice::words(std::move(outWords));
        }

        if (!isFirstItemAdded)
        {
            isFirstItemAdded = true;
            choiceSet = core::OneOrMore<core::DirectionTypeChoiceChoice>{std::move(choiceItem)};
        }
        else
        {
            choiceSet.add(std::move(choiceItem));
        }
    }

    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::choice(choiceSet));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitSegno(const api::SegnoData &item, core::Direction &direction)
{
    core::Segno segno{};
    setAttributesFromPositionData(item.positionData, segno);
    setAttributesFromFontData(item.fontData, segno);
    if (item.isColorSpecified)
    {
        setAttributesFromColorData(item.colorData, segno);
    }
    if (item.isSmuflSpecified)
    {
        segno.setSmufl(core::SmuflSegnoGlyphName::parse(item.smufl));
    }
    if (item.isIdSpecified)
    {
        segno.setID(core::Token{item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::segno(core::OneOrMore<core::Segno>{std::move(segno)}));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitCoda(const api::CodaData &item, core::Direction &direction)
{
    core::Coda coda{};
    setAttributesFromPositionData(item.positionData, coda);
    setAttributesFromFontData(item.fontData, coda);
    if (item.isColorSpecified)
    {
        setAttributesFromColorData(item.colorData, coda);
    }
    if (item.isSmuflSpecified)
    {
        coda.setSmufl(core::SmuflCodaGlyphName::parse(item.smufl));
    }
    if (item.isIdSpecified)
    {
        coda.setID(core::Token{item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::coda(core::OneOrMore<core::Coda>{std::move(coda)}));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitRehearsal(const api::RehearsalData &item, core::Direction &direction)
{
    core::FormattedTextID rehearsal{};
    rehearsal.setValue(item.text);
    setAttributesFromPositionData(item.positionData, rehearsal);
    setAttributesFromFontData(item.fontData, rehearsal);
    if (item.isColorSpecified)
    {
        setAttributesFromColorData(item.colorData, rehearsal);
    }
    rehearsal.setEnclosure(directionWriterEnclosure(item.enclosure));
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::rehearsal(core::OneOrMore<core::FormattedTextID>{std::move(rehearsal)}));
    addDirectionType(std::move(dt), direction);
}

core::EmptyPrintStyleAlignID DirectionWriter::createEmptyPrintStyleAlign(const api::PositionData &positionData,
                                                                         const api::FontData &fontData,
                                                                         const std::optional<api::ColorData> &color,
                                                                         const std::optional<std::string> &id)
{
    core::EmptyPrintStyleAlignID element{};
    setAttributesFromPositionData(positionData, element);
    setAttributesFromFontData(fontData, element);
    if (color.has_value())
    {
        setAttributesFromColorData(*color, element);
    }
    if (id.has_value())
    {
        element.setID(core::Token{*id});
    }
    return element;
}

void DirectionWriter::emitDamp(const api::DampData &item, core::Direction &direction)
{
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::damp(
        createEmptyPrintStyleAlign(item.positionData, item.fontData, item.color, item.id)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitDampAll(const api::DampAllData &item, core::Direction &direction)
{
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::dampAll(
        createEmptyPrintStyleAlign(item.positionData, item.fontData, item.color, item.id)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitEyeglasses(const api::EyeglassesData &item, core::Direction &direction)
{
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::eyeglasses(
        createEmptyPrintStyleAlign(item.positionData, item.fontData, item.color, item.id)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitStringMute(const api::StringMuteData &item, core::Direction &direction)
{
    core::StringMute stringMute{};
    stringMute.setType(item.type == api::StringMuteType::off ? core::OnOff::off() : core::OnOff::on());
    setAttributesFromPositionData(item.positionData, stringMute);
    setAttributesFromFontData(item.fontData, stringMute);
    if (item.color.has_value())
    {
        setAttributesFromColorData(*item.color, stringMute);
    }
    if (item.id.has_value())
    {
        stringMute.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::stringMute(std::move(stringMute)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitStaffDivide(const api::StaffDivideData &item, core::Direction &direction)
{
    core::StaffDivide staffDivide{};
    switch (item.type)
    {
    case api::StaffDivideType::up:
        staffDivide.setType(core::StaffDivideSymbol::up());
        break;
    case api::StaffDivideType::upDown:
        staffDivide.setType(core::StaffDivideSymbol::upDown());
        break;
    case api::StaffDivideType::down:
        staffDivide.setType(core::StaffDivideSymbol::down());
        break;
    }
    setAttributesFromPositionData(item.positionData, staffDivide);
    setAttributesFromFontData(item.fontData, staffDivide);
    if (item.color.has_value())
    {
        setAttributesFromColorData(*item.color, staffDivide);
    }
    if (item.id.has_value())
    {
        staffDivide.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::staffDivide(std::move(staffDivide)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitPrincipalVoice(const api::PrincipalVoiceData &item, core::Direction &direction)
{
    core::PrincipalVoice principalVoice{};
    principalVoice.setType(item.type == api::PrincipalVoiceType::stop ? core::StartStop::stop()
                                                                      : core::StartStop::start());
    switch (item.symbol)
    {
    case api::PrincipalVoiceSymbol::nebenstimme:
        principalVoice.setSymbol(core::PrincipalVoiceSymbol::nebenstimme());
        break;
    case api::PrincipalVoiceSymbol::plain:
        principalVoice.setSymbol(core::PrincipalVoiceSymbol::plain());
        break;
    case api::PrincipalVoiceSymbol::none:
        principalVoice.setSymbol(core::PrincipalVoiceSymbol::none());
        break;
    case api::PrincipalVoiceSymbol::hauptstimme:
        principalVoice.setSymbol(core::PrincipalVoiceSymbol::hauptstimme());
        break;
    }
    principalVoice.setValue(item.text);
    setAttributesFromPositionData(item.positionData, principalVoice);
    setAttributesFromFontData(item.fontData, principalVoice);
    if (item.color.has_value())
    {
        setAttributesFromColorData(*item.color, principalVoice);
    }
    if (item.id.has_value())
    {
        principalVoice.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::principalVoice(std::move(principalVoice)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitOtherDirection(const api::OtherDirectionData &item, core::Direction &direction)
{
    core::OtherDirection otherDirection{};
    otherDirection.setValue(item.text);
    if (item.printObject != api::Bool::unspecified)
    {
        otherDirection.setPrintObject(myConverter.convert(item.printObject));
    }
    if (item.smufl.has_value())
    {
        otherDirection.setSmufl(core::SmuflGlyphName{*item.smufl});
    }
    setAttributesFromPositionData(item.positionData, otherDirection);
    setAttributesFromFontData(item.fontData, otherDirection);
    if (item.color.has_value())
    {
        setAttributesFromColorData(*item.color, otherDirection);
    }
    if (item.id.has_value())
    {
        otherDirection.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::otherDirection(std::move(otherDirection)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitImage(const api::ImageData &item, core::Direction &direction)
{
    core::Image image{};
    image.setSource(item.source);
    image.setType(item.type);
    if (item.height.has_value())
    {
        image.setHeight(core::Tenths{core::Decimal{*item.height}});
    }
    if (item.width.has_value())
    {
        image.setWidth(core::Tenths{core::Decimal{*item.width}});
    }
    setAttributesFromPositionData(item.positionData, image);
    // <image>'s valign is the valign-image type (no baseline), which the generic position
    // helper cannot write; set it on the element directly. A baseline value cannot be
    // expressed on an image and is not written.
    switch (item.positionData.verticalAlignment)
    {
    case api::VerticalAlignment::top:
        image.setValign(core::ValignImage::top());
        break;
    case api::VerticalAlignment::middle:
        image.setValign(core::ValignImage::middle());
        break;
    case api::VerticalAlignment::bottom:
        image.setValign(core::ValignImage::bottom());
        break;
    case api::VerticalAlignment::baseline:
    case api::VerticalAlignment::unspecified:
    default:
        break;
    }
    if (item.id.has_value())
    {
        image.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::image(std::move(image)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitAccordionRegistration(const api::AccordionRegistrationData &item, core::Direction &direction)
{
    core::AccordionRegistration accordion{};
    accordion.setAccordionHigh(item.high);
    if (item.middle.has_value())
    {
        accordion.setAccordionMiddle(core::AccordionMiddle{*item.middle});
    }
    accordion.setAccordionLow(item.low);
    setAttributesFromPositionData(item.positionData, accordion);
    setAttributesFromFontData(item.fontData, accordion);
    if (item.color.has_value())
    {
        setAttributesFromColorData(*item.color, accordion);
    }
    if (item.id.has_value())
    {
        accordion.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::accordionRegistration(std::move(accordion)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitHarpPedals(const api::HarpPedalsData &item, core::Direction &direction)
{
    // MusicXML requires at least one pedal-tuning; a diagram with none cannot be expressed
    // and is not written.
    if (item.pedalTunings.empty())
    {
        return;
    }
    core::HarpPedals harpPedals{};
    bool isFirstTuningAdded = false;
    for (const auto &tuning : item.pedalTunings)
    {
        core::PedalTuning pedalTuning{};
        pedalTuning.setPedalStep(myConverter.convert(tuning.step));
        pedalTuning.setPedalAlter(
            core::Semitones{core::Decimal{Converter::convertToAlter(tuning.alter, tuning.cents)}});
        if (!isFirstTuningAdded)
        {
            harpPedals.setPedalTuning(core::OneOrMore<core::PedalTuning>{std::move(pedalTuning)});
            isFirstTuningAdded = true;
        }
        else
        {
            harpPedals.addPedalTuning(std::move(pedalTuning));
        }
    }
    setAttributesFromPositionData(item.positionData, harpPedals);
    setAttributesFromFontData(item.fontData, harpPedals);
    if (item.color.has_value())
    {
        setAttributesFromColorData(*item.color, harpPedals);
    }
    if (item.id.has_value())
    {
        harpPedals.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::harpPedals(std::move(harpPedals)));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitScordatura(const api::ScordaturaData &item, core::Direction &direction)
{
    // MusicXML requires at least one accord; a scordatura with none cannot be expressed and
    // is not written.
    if (item.accords.empty())
    {
        return;
    }
    core::Scordatura scordatura{};
    bool isFirstAccordAdded = false;
    for (const auto &accordData : item.accords)
    {
        core::Accord accord{};
        if (accordData.stringNumber.has_value())
        {
            accord.setString(core::StringNumber{*accordData.stringNumber});
        }
        core::TuningGroup tuning{};
        tuning.setTuningStep(myConverter.convert(accordData.tuningStep));
        if (accordData.tuningAlter != 0 || accordData.tuningCents != 0.0)
        {
            tuning.setTuningAlter(core::Semitones{
                core::Decimal{Converter::convertToAlter(accordData.tuningAlter, accordData.tuningCents)}});
        }
        tuning.setTuningOctave(core::Octave{accordData.tuningOctave});
        accord.setTuning(std::move(tuning));
        if (!isFirstAccordAdded)
        {
            scordatura.setAccord(core::OneOrMore<core::Accord>{std::move(accord)});
            isFirstAccordAdded = true;
        }
        else
        {
            scordatura.addAccord(std::move(accord));
        }
    }
    if (item.id.has_value())
    {
        scordatura.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::scordatura(std::move(scordatura)));
    addDirectionType(std::move(dt), direction);
}

core::PercussionChoice DirectionWriter::createPercussionChoice(const api::PercussionDataChoice &choice)
{
    using Kind = api::PercussionDataChoice::Kind;
    switch (choice.kind())
    {
    case Kind::metal: {
        core::Metal metal{};
        metal.setValue(myConverter.convert(choice.metal().value));
        if (choice.metal().smufl.has_value())
        {
            metal.setSmufl(core::SmuflPictogramGlyphName::parse(*choice.metal().smufl));
        }
        return core::PercussionChoice::metal(std::move(metal));
    }
    case Kind::wood: {
        core::Wood wood{};
        wood.setValue(myConverter.convert(choice.wood().value));
        if (choice.wood().smufl.has_value())
        {
            wood.setSmufl(core::SmuflPictogramGlyphName::parse(*choice.wood().smufl));
        }
        return core::PercussionChoice::wood(std::move(wood));
    }
    case Kind::pitched: {
        core::Pitched pitched{};
        pitched.setValue(myConverter.convert(choice.pitched().value));
        if (choice.pitched().smufl.has_value())
        {
            pitched.setSmufl(core::SmuflPictogramGlyphName::parse(*choice.pitched().smufl));
        }
        return core::PercussionChoice::pitched(std::move(pitched));
    }
    case Kind::membrane: {
        core::Membrane membrane{};
        membrane.setValue(myConverter.convert(choice.membrane().value));
        if (choice.membrane().smufl.has_value())
        {
            membrane.setSmufl(core::SmuflPictogramGlyphName::parse(*choice.membrane().smufl));
        }
        return core::PercussionChoice::membrane(std::move(membrane));
    }
    case Kind::effect: {
        core::Effect effect{};
        effect.setValue(myConverter.convert(choice.effect().value));
        if (choice.effect().smufl.has_value())
        {
            effect.setSmufl(core::SmuflPictogramGlyphName::parse(*choice.effect().smufl));
        }
        return core::PercussionChoice::effect(std::move(effect));
    }
    case Kind::timpani: {
        core::Timpani timpani{};
        if (choice.timpani().smufl.has_value())
        {
            timpani.setSmufl(core::SmuflPictogramGlyphName::parse(*choice.timpani().smufl));
        }
        return core::PercussionChoice::timpani(std::move(timpani));
    }
    case Kind::beater: {
        core::Beater beater{};
        beater.setValue(myConverter.convert(choice.beater().value));
        if (choice.beater().tip != api::TipDirection::unspecified)
        {
            beater.setTip(myConverter.convert(choice.beater().tip));
        }
        return core::PercussionChoice::beater(std::move(beater));
    }
    case Kind::stick: {
        core::Stick stick{};
        stick.setStickType(myConverter.convert(choice.stick().stickType));
        stick.setStickMaterial(myConverter.convert(choice.stick().stickMaterial));
        if (choice.stick().tip != api::TipDirection::unspecified)
        {
            stick.setTip(myConverter.convert(choice.stick().tip));
        }
        if (choice.stick().parentheses != api::Bool::unspecified)
        {
            stick.setParentheses(myConverter.convert(choice.stick().parentheses));
        }
        if (choice.stick().dashedCircle != api::Bool::unspecified)
        {
            stick.setDashedCircle(myConverter.convert(choice.stick().dashedCircle));
        }
        return core::PercussionChoice::stick(std::move(stick));
    }
    case Kind::stickLocation: {
        return core::PercussionChoice::stickLocation(myConverter.convert(choice.stickLocation()));
    }
    case Kind::otherPercussion: {
        core::OtherText other{};
        other.setValue(choice.otherPercussion().text);
        if (choice.otherPercussion().smufl.has_value())
        {
            other.setSmufl(core::SmuflGlyphName{*choice.otherPercussion().smufl});
        }
        return core::PercussionChoice::otherPercussion(std::move(other));
    }
    case Kind::glass:
    default: {
        core::Glass glass{};
        glass.setValue(myConverter.convert(choice.glass().value));
        if (choice.glass().smufl.has_value())
        {
            glass.setSmufl(core::SmuflPictogramGlyphName::parse(*choice.glass().smufl));
        }
        return core::PercussionChoice::glass(std::move(glass));
    }
    }
}

void DirectionWriter::emitPercussion(const api::PercussionData &item, core::Direction &direction)
{
    core::Percussion percussion{};
    percussion.setChoice(createPercussionChoice(item.choice));
    if (item.enclosure != api::PercussionEnclosure::unspecified)
    {
        percussion.setEnclosure(myConverter.convert(item.enclosure));
    }
    setAttributesFromPositionData(item.positionData, percussion);
    setAttributesFromFontData(item.fontData, percussion);
    if (item.color.has_value())
    {
        setAttributesFromColorData(*item.color, percussion);
    }
    if (item.id.has_value())
    {
        percussion.setID(core::Token{*item.id});
    }
    core::DirectionType dt{};
    dt.setChoice(core::DirectionTypeChoice::percussion(core::OneOrMore<core::Percussion>{std::move(percussion)}));
    addDirectionType(std::move(dt), direction);
}

void DirectionWriter::emitDirectionTypes(core::Direction &direction)
{
    for (const auto &choice : myDirectionData.directionTypes)
    {
        switch (choice.kind())
        {
        case api::DirectionChoice::Kind::tempo:
            emitTempo(choice.tempo(), direction);
            break;

        case api::DirectionChoice::Kind::mark:
            emitMark(choice.mark(), direction);
            break;

        case api::DirectionChoice::Kind::wedgeStart:
            emitWedgeStart(choice.wedgeStart(), &choice, direction);
            break;

        case api::DirectionChoice::Kind::wedgeStop:
            emitWedgeStop(choice.wedgeStop(), &choice, direction);
            break;

        case api::DirectionChoice::Kind::ottavaStart:
            emitOttavaStart(choice.ottavaStart(), &choice, direction);
            break;

        case api::DirectionChoice::Kind::ottavaStop:
            emitOttavaStop(choice.ottavaStop(), &choice, direction);
            break;

        case api::DirectionChoice::Kind::bracketStart:
            emitBracketStart(choice.bracketStart(), &choice, direction);
            break;

        case api::DirectionChoice::Kind::bracketStop:
            emitBracketStop(choice.bracketStop(), &choice, direction);
            break;

        case api::DirectionChoice::Kind::dashesStart:
            emitDashesStart(choice.dashesStart(), &choice, direction);
            break;

        case api::DirectionChoice::Kind::dashesStop:
            emitDashesStop(choice.dashesStop(), &choice, direction);
            break;

        case api::DirectionChoice::Kind::pedal:
            emitPedal(choice.pedal(), direction);
            break;

        case api::DirectionChoice::Kind::wordsRun:
            emitWordsRun(choice.wordsRun(), direction);
            break;

        case api::DirectionChoice::Kind::segno:
            emitSegno(choice.segno(), direction);
            break;

        case api::DirectionChoice::Kind::coda:
            emitCoda(choice.coda(), direction);
            break;

        case api::DirectionChoice::Kind::rehearsal:
            emitRehearsal(choice.rehearsal(), direction);
            break;

        case api::DirectionChoice::Kind::damp:
            emitDamp(choice.damp(), direction);
            break;

        case api::DirectionChoice::Kind::dampAll:
            emitDampAll(choice.dampAll(), direction);
            break;

        case api::DirectionChoice::Kind::eyeglasses:
            emitEyeglasses(choice.eyeglasses(), direction);
            break;

        case api::DirectionChoice::Kind::stringMute:
            emitStringMute(choice.stringMute(), direction);
            break;

        case api::DirectionChoice::Kind::staffDivide:
            emitStaffDivide(choice.staffDivide(), direction);
            break;

        case api::DirectionChoice::Kind::principalVoice:
            emitPrincipalVoice(choice.principalVoice(), direction);
            break;

        case api::DirectionChoice::Kind::otherDirection:
            emitOtherDirection(choice.otherDirection(), direction);
            break;

        case api::DirectionChoice::Kind::image:
            emitImage(choice.image(), direction);
            break;

        case api::DirectionChoice::Kind::accordionRegistration:
            emitAccordionRegistration(choice.accordionRegistration(), direction);
            break;

        case api::DirectionChoice::Kind::harpPedals:
            emitHarpPedals(choice.harpPedals(), direction);
            break;

        case api::DirectionChoice::Kind::scordatura:
            emitScordatura(choice.scordatura(), direction);
            break;

        case api::DirectionChoice::Kind::percussion:
            emitPercussion(choice.percussion(), direction);
            break;
        }
    }
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

        switch (chordIter->harmonyChordSource)
        {
        case api::HarmonyChordSource::numeral: {
            core::Numeral numeral{};
            core::NumeralRoot numeralRoot{};
            numeralRoot.setValue(core::NumeralValue{chordIter->numeralRoot});

            if (!chordIter->numeralRootText.empty())
            {
                numeralRoot.setText(chordIter->numeralRootText);
            }

            numeral.setNumeralRoot(numeralRoot);

            if (chordIter->hasNumeralAlter)
            {
                core::HarmonyAlter numeralAlter{};
                numeralAlter.setValue(core::Semitones{core::Decimal{static_cast<double>(chordIter->numeralAlter)}});
                numeral.setNumeralAlter(numeralAlter);
            }

            if (chordIter->hasNumeralKey)
            {
                core::NumeralKey numeralKey{};
                numeralKey.setNumeralFifths(core::Fifths{chordIter->numeralKeyFifths});

                switch (chordIter->numeralMode)
                {
                case api::NumeralMode::minor:
                    numeralKey.setNumeralMode(core::NumeralMode::minor());
                    break;
                case api::NumeralMode::naturalMinor:
                    numeralKey.setNumeralMode(core::NumeralMode::naturalMinor());
                    break;
                case api::NumeralMode::melodicMinor:
                    numeralKey.setNumeralMode(core::NumeralMode::melodicMinor());
                    break;
                case api::NumeralMode::harmonicMinor:
                    numeralKey.setNumeralMode(core::NumeralMode::harmonicMinor());
                    break;
                case api::NumeralMode::major:
                case api::NumeralMode::unspecified:
                default:
                    // numeral-mode is required inside numeral-key, so it cannot be omitted. An
                    // 'unspecified' mode here means the caller set hasNumeralKey without a concrete
                    // mode (a contradiction); fall back to major rather than drop the numeral-key.
                    numeralKey.setNumeralMode(core::NumeralMode::major());
                    break;
                }

                numeral.setNumeralKey(numeralKey);
            }

            grp.setChoice(core::HarmonyChordGroupChoice::numeral(numeral));
            break;
        }
        case api::HarmonyChordSource::function: {
            core::StyleText function{};
            function.setValue(chordIter->functionText);
            grp.setChoice(core::HarmonyChordGroupChoice::function(function));
            break;
        }
        case api::HarmonyChordSource::root:
        default: {
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
            break;
        }
        }

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

        if (chordIter->hasInversion)
        {
            core::Inversion inversion{};
            inversion.setValue(chordIter->inversion);
            grp.setInversion(inversion);
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

        if (chordIter->stackDegrees != api::Bool::unspecified)
        {
            const bool isYes = chordIter->stackDegrees == api::Bool::yes;
            kind.setStackDegrees(isYes ? core::YesNo::yes() : core::YesNo::no());
        }

        if (chordIter->parenthesesDegrees != api::Bool::unspecified)
        {
            const bool isYes = chordIter->parenthesesDegrees == api::Bool::yes;
            kind.setParenthesesDegrees(isYes ? core::YesNo::yes() : core::YesNo::no());
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

std::vector<core::MusicDataChoice> DirectionWriter::createFiguredBassElements()
{
    std::vector<core::MusicDataChoice> output;

    for (const auto &figuredBassData : myDirectionData.figuredBasses)
    {
        // The core figure list is never-empty (OneOrMore), so a figured-bass with zero figures
        // would still serialize one fabricated empty <figure/>. Skip it: an empty figures list
        // means "no figured-bass", and round-tripping must not invent content.
        if (figuredBassData.figures.empty())
        {
            continue;
        }

        core::FiguredBass figuredBass{};

        bool isFirstFigure = true;

        for (const auto &figureData : figuredBassData.figures)
        {
            core::Figure figure{};

            if (!figureData.prefix.empty())
            {
                core::StyleText prefix{};
                prefix.setValue(figureData.prefix);
                figure.setPrefix(prefix);
            }

            if (!figureData.figureNumber.empty())
            {
                core::StyleText figureNumber{};
                figureNumber.setValue(figureData.figureNumber);
                figure.setFigureNumber(figureNumber);
            }

            if (!figureData.suffix.empty())
            {
                core::StyleText suffix{};
                suffix.setValue(figureData.suffix);
                figure.setSuffix(suffix);
            }

            if (isFirstFigure)
            {
                isFirstFigure = false;
                figuredBass.setFigure(core::OneOrMore<core::Figure>{figure});
            }
            else
            {
                figuredBass.addFigure(figure);
            }
        }

        if (figuredBassData.parentheses != api::Bool::unspecified)
        {
            figuredBass.setParentheses(figuredBassData.parentheses == api::Bool::yes ? core::YesNo::yes()
                                                                                     : core::YesNo::no());
        }

        if (figuredBassData.durationTimeTicks >= 0)
        {
            figuredBass.setDuration(
                core::PositiveDivisions{core::Decimal{static_cast<double>(figuredBassData.durationTimeTicks)}});
        }

        output.push_back(core::MusicDataChoice::figuredBass(figuredBass));
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
