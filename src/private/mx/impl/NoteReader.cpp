// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NoteReader.h"
#include "mx/core/generated/BeamLevel.h"
#include "mx/core/generated/ElisionSyllabicGroup.h"
#include "mx/core/generated/LyricChoice.h"
#include "mx/core/generated/LyricSyllableGroup.h"
#include "mx/core/generated/LyricTextGroup.h"
#include "mx/core/generated/Notations.h"
#include "mx/core/generated/NotationsChoice.h"
#include "mx/core/generated/SmuflLyricsGlyphName.h"
#include "mx/core/generated/StartStopContinue.h"
#include "mx/core/generated/Syllabic.h"
#include "mx/core/generated/TextElementData.h"
#include "mx/core/generated/Tied.h"
#include "mx/impl/FontFunctions.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/utility/StringToInt.h"
#include "mx/utility/Throw.h"

#include "Converter.h"
#include "mx/api/PitchData.h"
#include <map>

namespace mx
{
namespace impl
{
namespace
{
api::LyricSyllabic convertLyricSyllabic(core::Syllabic value)
{
    switch (value.tag())
    {
    case core::Syllabic::Tag::single:
        return api::LyricSyllabic::single;
    case core::Syllabic::Tag::begin:
        return api::LyricSyllabic::begin;
    case core::Syllabic::Tag::end:
        return api::LyricSyllabic::end;
    case core::Syllabic::Tag::middle:
        return api::LyricSyllabic::middle;
    }

    return api::LyricSyllabic::single;
}

api::PositionData getLyricPositionData(const core::Lyric &inLyric)
{
    api::PositionData outPositionData;

    if (inLyric.defaultX().has_value())
    {
        outPositionData.isDefaultXSpecified = true;
        outPositionData.defaultX = inLyric.defaultX()->value().value();
    }

    if (inLyric.defaultY().has_value())
    {
        outPositionData.isDefaultYSpecified = true;
        outPositionData.defaultY = inLyric.defaultY()->value().value();
    }

    if (inLyric.relativeX().has_value())
    {
        outPositionData.isRelativeXSpecified = true;
        outPositionData.relativeX = inLyric.relativeX()->value().value();
    }

    if (inLyric.relativeY().has_value())
    {
        outPositionData.isRelativeYSpecified = true;
        outPositionData.relativeY = inLyric.relativeY()->value().value();
    }

    Converter converter;

    if (inLyric.placement().has_value())
    {
        outPositionData.placement = converter.convert(*inLyric.placement());
    }

    if (inLyric.justify().has_value())
    {
        outPositionData.horizontalAlignment = converter.convert(*inLyric.justify());
    }

    return outPositionData;
}

api::PrintData getLyricPrintData(const core::Lyric &inLyric, const core::TextElementData *textElement)
{
    api::PrintData outPrintData;
    outPrintData.printObject = getPrintObject(inLyric);

    if (inLyric.color().has_value())
    {
        outPrintData.isColorSpecified = true;
        outPrintData.color = getColor(inLyric);
    }

    if (textElement)
    {
        outPrintData.fontData = getFontData(*textElement);
    }

    return outPrintData;
}

// Appends the runs after the first <text>: one api::LyricTextSegment per <elision>-joined
// <syllabic>?/<text> pair, preserving each run's own syllabic value and elision content.
std::vector<api::LyricTextSegment> getLyricContinuations(const core::LyricTextGroup &inGroup)
{
    std::vector<api::LyricTextSegment> result;

    for (const auto &group : inGroup.lyricSyllableGroup())
    {
        api::LyricTextSegment segment;
        segment.text = group.text().value();

        if (group.elisionSyllabicGroup().has_value())
        {
            const auto &elisionSyllabicGroup = *group.elisionSyllabicGroup();
            const auto &elision = elisionSyllabicGroup.elision();
            if (!elision.value().empty())
            {
                segment.elisionText = elision.value();
            }
            // Preserved independently of elisionText for round-trip fidelity, even though
            // MusicXML only consults smufl when the text content is empty.
            if (elision.smufl().has_value())
            {
                segment.elisionSmufl = elision.smufl()->toString();
            }
            if (elisionSyllabicGroup.syllabic().has_value())
            {
                segment.syllabic = convertLyricSyllabic(*elisionSyllabicGroup.syllabic());
            }
        }

        result.emplace_back(std::move(segment));
    }

    return result;
}
} // namespace

NoteReader::NoteReader(const core::Note &mxNote)
    : myNote(mxNote), myNoteChoice(myNote.choice()), myFullNoteGroup(findFullNoteGroup(myNoteChoice)),
      myIsNormal(false), myIsGrace(false), myIsCue(false), myIsRest(false), myIsChord(false), myIsMeasureRest(false),
      myIsUnpitched(false), myIsPitch(false), myIsDisplayStepOctaveSpecified(false), myDurationValue(0.0),
      myStep(core::Step::c()), myAlter(0), myCents(0.0), myOctave(4), myStaffNumber(0), myIsStaffSpecified(false),
      myVoiceNumber(0), myVoiceLabel{}, myNoteheadValue(core::NoteheadValue::normal()), myNoteheadFilled{},
      myNoteheadSmufl{}, myDurationType(core::NoteTypeValue::maxima()), myIsDurationTypeSpecified(false), myNumDots(0),
      myBeams(), myTimeModificationActualNotes(-1), myTimeModificationNormalNotes(-1),
      myTimeModificationNormalType(core::NoteTypeValue::maxima()), myTimeModificationNormalTypeDots(0),
      myIsTimeModificationNormalTypeSpecified(false), myHasAccidental(false),
      myAccidental(core::AccidentalValue::natural()), myIsAccidentalParenthetical(false),
      myIsAccidentalCautionary{false}, myIsAccidentalEditorial{false}, myIsAccidentalBracketed{false},
      myIsStemSpecified{false}, myStem{}, myIsGraceSlashSpecified{false}, myGraceSlash{}, myIsTieStart{false},
      myIsTieStop{false}, myHasLyric{false}
{
    setNormalGraceCueItems();
    setRestPitchUnpitchedItems();
    setChord();
    setStaffNumber();
    setVoiceNumber();
    setNoteheadItems();
    setDurationType();
    setNumDots();
    setBeams();
    setTimeModification();
    setAccidental();
    setStem();
    setLyric();
}

const core::FullNoteGroup &NoteReader::findFullNoteGroup(const core::NoteChoice &noteChoice) const
{
    switch (noteChoice.kind())
    {
    case core::NoteChoice::Kind::normalNoteGroup: {
        return noteChoice.asNormalNoteGroup().fullNote();
    }
    case core::NoteChoice::Kind::graceNoteGroup: {
        const auto &graceChoice = noteChoice.asGraceNoteGroup().graceNoteChoice();
        if (graceChoice.isGraceCueNoteGroup())
        {
            return graceChoice.asGraceCueNoteGroup().fullNote();
        }
        return graceChoice.asGraceNormalNoteGroup().fullNote();
    }
    case core::NoteChoice::Kind::cueNoteGroup: {
        return noteChoice.asCueNoteGroup().fullNote();
    }
    default:
        break;
    }

    MX_BUG;
}

void NoteReader::setNormalGraceCueItems()
{
    switch (myNoteChoice.kind())
    {
    case core::NoteChoice::Kind::normalNoteGroup: {
        myIsNormal = true;
        const auto &noteGuts = myNoteChoice.asNormalNoteGroup();
        myDurationValue = noteGuts.duration().value().value();
        setTie(noteGuts.tie());
        break;
    }
    case core::NoteChoice::Kind::graceNoteGroup: {
        myIsGrace = true;
        const auto &noteGuts = myNoteChoice.asGraceNoteGroup();
        myDurationValue = 0;
        if (noteGuts.grace().slash().has_value())
        {
            myIsGraceSlashSpecified = true;
            myGraceSlash = *noteGuts.grace().slash();
        }
        if (noteGuts.graceNoteChoice().isGraceNormalNoteGroup())
        {
            setTie(noteGuts.graceNoteChoice().asGraceNormalNoteGroup().tie());
        }
        else
        {
            // <grace/> + <cue/>: a grace note inside a cue passage. The
            // grace-cue group has no <tie> slot in the schema, so the tie can
            // only be stated as a <tied> notation.
            myIsCue = true;
            setTieFromNotations();
        }
        break;
    }
    case core::NoteChoice::Kind::cueNoteGroup: {
        myIsCue = true;
        const auto &noteGuts = myNoteChoice.asCueNoteGroup();
        myDurationValue = noteGuts.duration().value().value();
        // A cue note is silent and has no <tie> slot either; same as grace-cue.
        setTieFromNotations();
        break;
    }
    default:
        MX_THROW("invalid NoteChoice::Kind value");
    }
}

void NoteReader::setRestPitchUnpitchedItems()
{
    const auto &fullNoteTypeChoice = myFullNoteGroup.choice();

    switch (fullNoteTypeChoice.kind())
    {
    case core::FullNoteGroupChoice::Kind::rest: {
        myIsRest = true;
        const auto &rest = fullNoteTypeChoice.asRest();

        if (rest.measure().has_value() && (*rest.measure() == core::YesNo::yes()))
        {
            myIsMeasureRest = true;
        }

        if (rest.displayStepOctave().has_value())
        {
            const auto &stepOctave = *rest.displayStepOctave();
            myIsDisplayStepOctaveSpecified = true;
            myStep = stepOctave.displayStep();
            myOctave = stepOctave.displayOctave().value();
        }

        myAlter = 0;
        break;
    }
    case core::FullNoteGroupChoice::Kind::unpitched: {
        myIsUnpitched = true;
        const auto &unpitched = fullNoteTypeChoice.asUnpitched();

        if (unpitched.displayStepOctave().has_value())
        {
            const auto &stepOctave = *unpitched.displayStepOctave();
            myIsDisplayStepOctaveSpecified = true;
            myStep = stepOctave.displayStep();
            myOctave = stepOctave.displayOctave().value();
        }

        myAlter = 0;
        break;
    }
    case core::FullNoteGroupChoice::Kind::pitch: {
        myIsPitch = true;
        const auto &pitch = fullNoteTypeChoice.asPitch();
        myStep = pitch.step();
        myOctave = pitch.octave().value();
        if (pitch.alter().has_value())
        {
            const auto semitonesAndCents =
                mx::impl::Converter::convertToSemitonesAndCents(pitch.alter()->value().value());
            myAlter = semitonesAndCents.first;
            myCents = semitonesAndCents.second;
        }
        break;
    }

    default:
        MX_THROW("invalid FullNoteGroupChoice::Kind value");
    }
}

void NoteReader::setChord()
{
    myIsChord = myFullNoteGroup.chord();
}

void NoteReader::setStaffNumber()
{
    myIsStaffSpecified = myNote.staff().has_value();
    if (myIsStaffSpecified)
    {
        myStaffNumber = *myNote.staff();
    }
}

void NoteReader::setVoiceNumber()
{
    myVoiceNumber = api::VALUE_UNSPECIFIED;
    myVoiceLabel.clear();

    if (!myNote.editorialVoice().voice().has_value())
    {
        return;
    }

    myVoiceLabel = *myNote.editorialVoice().voice();
    utility::stringToInt(myVoiceLabel.c_str(), myVoiceNumber);
}

void NoteReader::setNoteheadItems()
{
    if (!myNote.notehead().has_value())
    {
        return;
    }

    const auto &notehead = *myNote.notehead();
    myNoteheadValue = notehead.value();
    myNoteheadFilled = notehead.filled();

    if (notehead.smufl().has_value())
    {
        myNoteheadSmufl = notehead.smufl()->toString();
    }
}

void NoteReader::setDurationType()
{
    if (myNote.type().has_value())
    {
        myIsDurationTypeSpecified = true;
        myDurationType = myNote.type()->value();
    }
}

void NoteReader::setNumDots()
{
    myNumDots = static_cast<int>(myNote.dot().size());
}

void NoteReader::setBeams()
{
    const auto mxBeamSet = myNote.beam();
    std::map<int, core::BeamValue> mapOfBeams;
    int calculatedBeamIndex = 0;

    for (const auto &mxBeam : mxBeamSet)
    {
        const int userBeamIndex = mxBeam.number().has_value() ? mxBeam.number()->value() : core::BeamLevel{}.value();
        int useThisBeamIndex = calculatedBeamIndex;

        // if the userBeamIndex seems valid we will use it
        // by doing this we support the possibility that
        // the musicxml file could have beams listed in the
        // 'wrong' order, i.e. 3,4,1,2
        if (mxBeam.number().has_value())
        {
            if (mapOfBeams.find(userBeamIndex) == mapOfBeams.cend())
            {
                useThisBeamIndex = userBeamIndex;
            }
        }

        mapOfBeams[useThisBeamIndex] = mxBeam.value();
        ++calculatedBeamIndex;
    }

    // if the musicxml document has not provided all of the needed
    // beams then this will not be using the docs illogical numbers
    for (const auto &beamVal : mapOfBeams)
    {
        myBeams.push_back(beamVal.second);
    }
}

void NoteReader::setTimeModification()
{
    if (!myNote.timeModification().has_value())
    {
        // 1/1 is the api's "no time modification" sentinel; NoteWriter skips writing 1/1, so
        // a literal <time-modification>1/1 in a source file will not round-trip.
        myTimeModificationActualNotes = 1;
        myTimeModificationNormalNotes = 1;
        return;
    }

    const auto &mxTimeMod = *myNote.timeModification();
    myTimeModificationActualNotes = mxTimeMod.actualNotes();
    myTimeModificationNormalNotes = mxTimeMod.normalNotes();

    if (mxTimeMod.group().has_value())
    {
        const auto &grp = *mxTimeMod.group();
        myTimeModificationNormalType = grp.normalType();
        myTimeModificationNormalTypeDots = static_cast<int>(grp.normalDot().size());
        myIsTimeModificationNormalTypeSpecified = true;
    }
    else
    {
        myTimeModificationNormalType = myDurationType;
        myTimeModificationNormalTypeDots = 0;
    }
}

void NoteReader::setAccidental()
{
    myIsAccidentalParenthetical = false;
    myHasAccidental = myNote.accidental().has_value();

    if (myHasAccidental)
    {
        const auto &accidental = *myNote.accidental();
        myAccidental = accidental.value();

        if (accidental.parentheses().has_value() && *accidental.parentheses() == core::YesNo::yes())
        {
            myIsAccidentalParenthetical = true;
        }

        if (accidental.cautionary().has_value() && *accidental.cautionary() == core::YesNo::yes())
        {
            myIsAccidentalCautionary = true;
        }

        if (accidental.editorial().has_value() && *accidental.editorial() == core::YesNo::yes())
        {
            myIsAccidentalEditorial = true;
        }

        if (accidental.bracket().has_value() && *accidental.bracket() == core::YesNo::yes())
        {
            myIsAccidentalBracketed = true;
        }
    }
}

void NoteReader::setStem()
{
    if (myNote.stem().has_value())
    {
        myIsStemSpecified = true;
        myStem = myNote.stem()->value();
    }
    else
    {
        myIsStemSpecified = false;
    }
}

void NoteReader::setTie(std::span<const core::Tie> tieSet)
{
    for (const auto &tie : tieSet)
    {
        if (tie.type() == core::StartStop::start())
        {
            myIsTieStart = true;
        }
        else if (tie.type() == core::StartStop::stop())
        {
            myIsTieStop = true;
        }
    }
}

// Cue and grace-cue notes have no <tie> slot in the schema, so their tie can
// only be stated as a <tied> notation. Reads the start/stop flags from there.
// (A lone <tied type="let-ring"> is a different thing and is read elsewhere,
// into NoteData::tieLetRing.)
void NoteReader::setTieFromNotations()
{
    for (const auto &notations : myNote.notations())
    {
        for (const auto &notationsChoice : notations.choice())
        {
            if (notationsChoice.kind() != core::NotationsChoice::Kind::tied)
            {
                continue;
            }

            const auto type = notationsChoice.asTied().type();

            if (type == core::TiedType::start())
            {
                myIsTieStart = true;
            }
            else if (type == core::TiedType::stop())
            {
                myIsTieStop = true;
            }
        }
    }
}

void NoteReader::setLyric()
{
    const auto lyricSet = myNote.lyric();
    auto iter = lyricSet.begin();
    const auto iterEnd = lyricSet.end();
    for (; iter != iterEnd; ++iter)
    {
        const auto &lyric = *iter;
        const auto &textChoice = lyric.choice();

        api::LyricData lyricData;
        lyricData.positionData = getLyricPositionData(lyric);
        lyricData.id = getId(lyric);

        if (lyric.number().has_value())
        {
            lyricData.verseNumber = lyric.number()->value();
        }

        if (lyric.name().has_value())
        {
            lyricData.verseName = *lyric.name();
        }

        switch (textChoice.kind())
        {
        case core::LyricChoice::Kind::lyricTextGroup: {
            const auto &textGroup = textChoice.asLyricTextGroup();

            lyricData.text = textGroup.text().value();
            lyricData.printData = getLyricPrintData(lyric, &textGroup.text());

            lyricData.syllabic = textGroup.syllabic().has_value() ? convertLyricSyllabic(*textGroup.syllabic())
                                                                  : api::LyricSyllabic::unspecified;
            lyricData.continuations = getLyricContinuations(textGroup);
            if (textGroup.extend().has_value())
            {
                lyricData.hasExtend = true;
                if (textGroup.extend()->type().has_value())
                {
                    switch (textGroup.extend()->type()->tag())
                    {
                    case core::StartStopContinue::Tag::start:
                        lyricData.extendType = api::LyricExtendType::start;
                        break;
                    case core::StartStopContinue::Tag::continue_:
                        lyricData.extendType = api::LyricExtendType::continue_;
                        break;
                    case core::StartStopContinue::Tag::stop:
                        lyricData.extendType = api::LyricExtendType::stop;
                        break;
                    }
                }
            }
            myLyrics.emplace_back(lyricData);
            myHasLyric = true;
            break;
        }

        case core::LyricChoice::Kind::extend:
            lyricData.hasExtend = true;
            if (textChoice.asExtend().type().has_value())
            {
                switch (textChoice.asExtend().type()->tag())
                {
                case core::StartStopContinue::Tag::start:
                    lyricData.extendType = api::LyricExtendType::start;
                    break;
                case core::StartStopContinue::Tag::continue_:
                    lyricData.extendType = api::LyricExtendType::continue_;
                    break;
                case core::StartStopContinue::Tag::stop:
                    lyricData.extendType = api::LyricExtendType::stop;
                    break;
                }
            }
            lyricData.printData = getLyricPrintData(lyric, nullptr);
            myLyrics.emplace_back(lyricData);
            myHasLyric = true;
            break;

        case core::LyricChoice::Kind::laughing:
        case core::LyricChoice::Kind::humming: {
            // laughing/humming lyrics are not modeled in mx::api; the whole <lyric> is
            // intentionally dropped (not just the choice element).
            break;
        }
        }
    }
}
} // namespace impl
} // namespace mx
