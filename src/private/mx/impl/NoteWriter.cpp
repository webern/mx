// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NoteWriter.h"
#include "mx/core/NameToken.h"
#include "mx/core/generated/Accidental.h"
#include "mx/core/generated/BeamLevel.h"
#include "mx/core/generated/CueNoteGroup.h"
#include "mx/core/generated/DisplayStepOctaveGroup.h"
#include "mx/core/generated/Extend.h"
#include "mx/core/generated/FormattedText.h"
#include "mx/core/generated/FullNoteGroupChoice.h"
#include "mx/core/generated/GraceNormalNoteGroup.h"
#include "mx/core/generated/GraceNoteChoice.h"
#include "mx/core/generated/GraceNoteGroup.h"
#include "mx/core/generated/LyricChoice.h"
#include "mx/core/generated/LyricTextGroup.h"
#include "mx/core/generated/NormalNoteGroup.h"
#include "mx/core/generated/Pitch.h"
#include "mx/core/generated/Rest.h"
#include "mx/core/generated/Syllabic.h"
#include "mx/core/generated/TextElementData.h"
#include "mx/core/generated/Tied.h"
#include "mx/core/generated/TimeModification.h"
#include "mx/core/generated/TimeModificationGroup.h"
#include "mx/core/generated/Unpitched.h"
#include "mx/impl/NotationsWriter.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/impl/WriteRefusal.h"
#include "mx/utility/Throw.h"

#include <cmath>

namespace mx
{
namespace impl
{
core::Syllabic convertLyricSyllabicForNoteWriter(api::LyricSyllabic value)
{
    switch (value)
    {
    case api::LyricSyllabic::single:
        return core::Syllabic::single();
    case api::LyricSyllabic::begin:
        return core::Syllabic::begin();
    case api::LyricSyllabic::end:
        return core::Syllabic::end();
    case api::LyricSyllabic::middle:
        return core::Syllabic::middle();
    }

    return core::Syllabic::single();
}

NoteWriter::NoteWriter(const api::NoteData &inNoteData, const MeasureCursor &inCursor, const ScoreWriter &inScoreWriter,
                       bool isPreviousNoteAChordMember, const std::vector<mx::api::NoteData> &inSiblingNotes,
                       int inNoteIndex, int inNumVoices)
    : myNoteData{inNoteData}, myCursor{inCursor}, myScoreWriter{inScoreWriter}, myConverter{},
      myIsPreviousNoteAChordMember{isPreviousNoteAChordMember}, mySiblingNotes{inSiblingNotes},
      myNoteIndex{inNoteIndex}, myNumVoices{inNumVoices}, myOutNote{}, myOutFullNoteGroup{}, myOutTies{},
      myOutTieNotationsChoices{}
{
}

core::Note NoteWriter::getNote(bool isStartOfChord) const
{
    myOutNote = core::Note{};
    myOutFullNoteGroup = core::FullNoteGroup{};
    myOutTies.clear();
    myOutTieNotationsChoices.clear();

    setNoteChoiceAndFullNoteGroup(isStartOfChord);
    setFullNoteTypeChoice();
    assembleNoteChoice();
    setStaffAndVoice();
    setDurationNameAndDots();
    setNotehead();
    setStemDirection();
    setMiscData();
    NotationsWriter notationsWriter{myNoteData, myCursor, myScoreWriter};
    impl::setAttributesFromPositionData(myNoteData.positionData, myOutNote);

    // The tie <notations> come first (as in the old writer, where they were
    // created during setNoteChoiceAndFullNoteGroup).
    if (!myOutTieNotationsChoices.empty())
    {
        core::Notations tieNotations;
        for (auto &choice : myOutTieNotationsChoices)
        {
            tieNotations.addChoice(std::move(choice));
        }
        myOutNote.addNotations(std::move(tieNotations));
    }

    auto notations = notationsWriter.getNotations();
    if (notations.choice().size() > 0)
    {
        myOutNote.addNotations(std::move(notations));
    }

    if (myNoteData.pitchData.accidental != api::Accidental::none)
    {
        core::Accidental accidental;
        accidental.setValue(myConverter.convert(myNoteData.pitchData.accidental));
        myOutNote.setAccidental(std::move(accidental));
    }

    auto beamIndex = 0;
    for (const auto &beam : myNoteData.beams)
    {
        core::Beam mxBeam;
        mxBeam.setNumber(core::BeamLevel{beamIndex + 1});
        mxBeam.setValue(myConverter.convert(beam));
        const auto added = myOutNote.addBeam(std::move(mxBeam));
        if (!added)
        {
            // Refuse, don't drop: the core caps
            // beams at 8; silently discarding the ninth would lose data.
            throw WriteRefusal{api::ApiError{api::ResultCode::tooManyElements, added.error().path,
                                             "NoteWriter: " + added.error().message}};
        }
        ++beamIndex;
    }

    if (myNoteData.durationData.timeModificationNormalNotes > 0 &&
        myNoteData.durationData.timeModificationActualNotes > 0 &&
        (myNoteData.durationData.timeModificationNormalNotes > 1 ||
         myNoteData.durationData.timeModificationActualNotes > 1))
    {
        core::TimeModification timeMod;
        timeMod.setActualNotes(myNoteData.durationData.timeModificationActualNotes);
        timeMod.setNormalNotes(myNoteData.durationData.timeModificationNormalNotes);

        // find the tuplet start note and TupletStart object
        bool isTupletStartFound = false;
        int tupletStartIndex = myNoteIndex;
        mx::api::NoteData tupletStartNote{};
        mx::api::TupletStart tupletStart{};

        for (; tupletStartIndex >= 0 && mySiblingNotes.size() > 0; --tupletStartIndex)
        {
            const auto &siblingNote = mySiblingNotes.at(static_cast<size_t>(tupletStartIndex));
            if (siblingNote.noteAttachmentData.tupletStarts.size() == 1)
            {
                isTupletStartFound = true;
                tupletStartNote = siblingNote;
                tupletStart = siblingNote.noteAttachmentData.tupletStarts.at(0);
                break;
            }
        }

        if (!isTupletStartFound)
        {
            MX_DEBUG_THROW("tupletStart was not found");
        }

        // find the tuplet stop note and TupletStop object
        bool isTupletStopFound = false;
        int tupletStopIndex = tupletStartIndex;
        mx::api::NoteData tupletStopNote{};
        mx::api::TupletStop tupletStop{};

        if (isTupletStartFound)
        {
            for (; tupletStopIndex < static_cast<int>(mySiblingNotes.size()); ++tupletStopIndex)
            {
                const auto &siblingNote = mySiblingNotes.at(static_cast<size_t>(tupletStopIndex));
                if (siblingNote.noteAttachmentData.tupletStops.size() == 1)
                {
                    isTupletStopFound = true;
                    tupletStopNote = siblingNote;
                    tupletStop = siblingNote.noteAttachmentData.tupletStops.at(0);
                    break;
                }
            }
        }

        if (!isTupletStopFound)
        {
            MX_DEBUG_THROW("tupletStop was not found");
        }

        // calculate the distance between the two
        if (isTupletStartFound && isTupletStopFound)
        {
            const auto tickTimeDistance =
                (tupletStopNote.tickTimePosition + tupletStopNote.durationData.durationTimeTicks) -
                tupletStartNote.tickTimePosition;

            if (tickTimeDistance > 0 && tupletStart.normalNumber != 0 && tupletStart.actualNumber != 0)
            {
                // calculate the tuplet normal type and dots based on the distance between start and stop and the ratio
                const double normalLength =
                    static_cast<double>(tickTimeDistance) / static_cast<double>(tupletStart.normalNumber);

                mx::api::DurationName normalName = mx::api::DurationName::unspecified;
                int normalDots = 0;
                const bool isNormalNameAndDotsFound = this->findNormalNameAndDots(normalName, normalDots, normalLength);
                if (isNormalNameAndDotsFound)
                {
                    core::TimeModificationGroup group;
                    group.setNormalType(myConverter.convert(normalName));

                    std::vector<core::Empty> normalDotsVec;
                    for (int i = 0; i < normalDots; ++i)
                    {
                        normalDotsVec.emplace_back();
                    }
                    group.setNormalDot(std::move(normalDotsVec));
                    timeMod.setGroup(std::move(group));
                }
                else
                {
                    MX_DEBUG_THROW("this->findNormalNameAndDots could not find what it was looking for. This probably "
                                   "means that the file has a badly specified tuplet.");
                }
            }
            else
            {
                MX_DEBUG_THROW("one of these things was not true ( tickTimeDistance > 0 && tupletStart.normalNumber != "
                               "0 && tupletStart.actualNumber != 0 )");
            }
        }
        else
        {
            MX_DEBUG_THROW("one of these things was not true ( isTupletStartFound && isTupletStopFound )");
        }

        myOutNote.setTimeModification(std::move(timeMod));

        // TODO - decide what happens if the user entered specific tuplet type in the
        // duration data, possibly remove those fields from duration data.
    }

    setLyrics();

    return myOutNote;
}

// Records a tie (for the note choice) and the matching tied notation. Old
// behavior preserved: a single <notations> element collects the tied
// choices, stop before start when both are present.
void NoteWriter::addTie(bool isStart) const
{
    core::Tie tie;
    tie.setType(isStart ? core::StartStop::start() : core::StartStop::stop());
    myOutTies.push_back(std::move(tie));

    core::Tied tied;
    tied.setType(isStart ? core::TiedType::start() : core::TiedType::stop());
    myOutTieNotationsChoices.push_back(core::NotationsChoice::tied(std::move(tied)));
}

void NoteWriter::setNoteChoiceAndFullNoteGroup(bool isStartOfChord) const
{
    myOutFullNoteGroup.setChord(myCursor.isChordActive && myIsPreviousNoteAChordMember && !isStartOfChord);

    switch (myNoteData.noteType)
    {
    case api::NoteType::cue: {
        break;
    }
    case api::NoteType::grace: {
        if (myNoteData.isTieStop)
        {
            addTie(false);
        }

        if (myNoteData.isTieStart)
        {
            addTie(true);
        }

        break;
    }
    case api::NoteType::normal: {
        if (myNoteData.isTieStop)
        {
            addTie(false);
        }

        if (myNoteData.isTieStart)
        {
            addTie(true);
        }

        break;
    }
    default:
        break;
    }
}

// Installs the assembled full-note group, duration, and ties into the note's
// choice. (The old writer mutated co-allocated members through shared
// pointers; under value semantics the alternative is built and installed.)
void NoteWriter::assembleNoteChoice() const
{
    const auto duration =
        core::PositiveDivisions{core::Decimal{static_cast<double>(myNoteData.durationData.durationTimeTicks)}};

    switch (myNoteData.noteType)
    {
    case api::NoteType::cue: {
        core::CueNoteGroup choiceObj;
        choiceObj.setFullNote(myOutFullNoteGroup);
        choiceObj.setDuration(duration);
        myOutNote.setChoice(core::NoteChoice::cueNoteGroup(std::move(choiceObj)));
        break;
    }
    case api::NoteType::grace: {
        core::GraceNormalNoteGroup inner;
        inner.setFullNote(myOutFullNoteGroup);
        for (const auto &tie : myOutTies)
        {
            const auto added = inner.addTie(tie);
            if (!added)
            {
                throw WriteRefusal{api::ApiError{api::ResultCode::tooManyElements, added.error().path,
                                                 "NoteWriter: " + added.error().message}};
            }
        }
        core::GraceNoteGroup choiceObj;
        choiceObj.setGraceNoteChoice(core::GraceNoteChoice::graceNormalNoteGroup(std::move(inner)));
        myOutNote.setChoice(core::NoteChoice::graceNoteGroup(std::move(choiceObj)));
        break;
    }
    case api::NoteType::normal:
    default: {
        core::NormalNoteGroup choiceObj;
        choiceObj.setFullNote(myOutFullNoteGroup);
        choiceObj.setDuration(duration);
        for (const auto &tie : myOutTies)
        {
            const auto added = choiceObj.addTie(tie);
            if (!added)
            {
                throw WriteRefusal{api::ApiError{api::ResultCode::tooManyElements, added.error().path,
                                                 "NoteWriter: " + added.error().message}};
            }
        }
        myOutNote.setChoice(core::NoteChoice::normalNoteGroup(std::move(choiceObj)));
        break;
    }
    }
}

void NoteWriter::setFullNoteTypeChoice() const
{
    if (myNoteData.isRest)
    {
        core::Rest rest;
        if (myNoteData.isDisplayStepOctaveSpecified)
        {
            core::DisplayStepOctaveGroup pitch;
            pitch.setDisplayStep(myConverter.convert(myNoteData.pitchData.step));
            pitch.setDisplayOctave(core::Octave{myNoteData.pitchData.octave});
            rest.setDisplayStepOctave(std::move(pitch));
        }

        if (myNoteData.isMeasureRest)
        {
            rest.setMeasure(core::YesNo::yes());
        }

        myOutFullNoteGroup.setChoice(core::FullNoteGroupChoice::rest(std::move(rest)));
    }
    else if (myNoteData.isUnpitched)
    {
        core::Unpitched unpitched;
        if (myNoteData.isDisplayStepOctaveSpecified)
        {
            core::DisplayStepOctaveGroup pitch;
            pitch.setDisplayStep(myConverter.convert(myNoteData.pitchData.step));
            pitch.setDisplayOctave(core::Octave{myNoteData.pitchData.octave});
            unpitched.setDisplayStepOctave(std::move(pitch));
        }

        myOutFullNoteGroup.setChoice(core::FullNoteGroupChoice::unpitched(std::move(unpitched)));
    }
    else
    {
        core::Pitch pitch;
        pitch.setStep(myConverter.convert(myNoteData.pitchData.step));
        if (myNoteData.pitchData.alter != 0 || myNoteData.pitchData.cents != 0.0)
        {
            const auto alter = Converter::convertToAlter(myNoteData.pitchData.alter, myNoteData.pitchData.cents);
            pitch.setAlter(core::Semitones{core::Decimal{alter}});
        }
        pitch.setOctave(core::Octave{myNoteData.pitchData.octave});
        myOutFullNoteGroup.setChoice(core::FullNoteGroupChoice::pitch(std::move(pitch)));
    }
}

void NoteWriter::setStaffAndVoice() const
{
    if (myCursor.getNumStaves() > 1 && myCursor.staffIndex >= 0)
    {
        myOutNote.setStaff(myCursor.staffIndex + 1);
    }

    const bool sourceHadVoice = myNoteData.userRequestedVoiceNumber != -1;
    const bool isNonDefaultVoice = myCursor.voiceIndex > 0;
    const bool isMultiVoiceStaff = myNumVoices > 1;
    if (myCursor.voiceIndex >= 0 && (sourceHadVoice || isNonDefaultVoice || isMultiVoiceStaff))
    {
        auto editorialVoice = myOutNote.editorialVoice();
        editorialVoice.setVoice(std::to_string(myCursor.voiceIndex + 1));
        myOutNote.setEditorialVoice(std::move(editorialVoice));
    }
}

void NoteWriter::setDurationNameAndDots() const
{
    const bool isMeasureRest = myNoteData.isRest && myNoteData.isMeasureRest;
    if (myNoteData.durationData.isDurationNameSpecified && !isMeasureRest)
    {
        core::NoteType noteType;
        noteType.setValue(myConverter.convert(myNoteData.durationData.durationName));
        myOutNote.setType(std::move(noteType));
    }

    for (int d = 0; d < static_cast<int>(myNoteData.durationData.durationDots); ++d)
    {
        myOutNote.addDot(core::EmptyPlacement{});
    }
}

void NoteWriter::setNotehead() const
{
    if (myNoteData.notehead != mx::api::Notehead::normal)
    {
        core::Notehead notehead;
        notehead.setValue(myConverter.convert(myNoteData.notehead));
        myOutNote.setNotehead(std::move(notehead));
    }
}

void NoteWriter::setStemDirection() const
{
    if (myNoteData.stem == api::Stem::unspecified)
    {
        return;
    }

    core::Stem stem;
    stem.setValue(myConverter.convert(myNoteData.stem));
    impl::setAttributesFromPositionData(myNoteData.stemPositionData, stem);
    myOutNote.setStem(std::move(stem));
}

void NoteWriter::setLyrics() const
{
    for (const auto &lyricData : myNoteData.lyrics)
    {
        core::Lyric lyric;
        if (!lyricData.verseNumber.empty())
        {
            lyric.setNumber(core::NameToken{lyricData.verseNumber});
        }

        if (!lyricData.verseName.empty())
        {
            lyric.setName(lyricData.verseName);
        }

        impl::setAttributesFromPositionData(lyricData.positionData, lyric);
        if (lyricData.positionData.horizontalAlignmnet != api::HorizontalAlignment::unspecified)
        {
            lyric.setJustify(myConverter.convert(lyricData.positionData.horizontalAlignmnet));
        }

        if (lyricData.printData.printObject != api::Bool::unspecified)
        {
            lyric.setPrintObject(myConverter.convert(lyricData.printData.printObject));
        }
        if (lyricData.printData.isColorSpecified)
        {
            setAttributesFromColorData(lyricData.printData.color, lyric);
        }

        if (lyricData.text.empty() && lyricData.hasExtend)
        {
            lyric.setChoice(core::LyricChoice::extend(core::Extend{}));
        }
        else
        {
            core::TextElementData text;
            text.setValue(lyricData.text);
            setAttributesFromFontData(lyricData.printData.fontData, text);

            core::LyricTextGroup textGroup;
            textGroup.setSyllabic(convertLyricSyllabicForNoteWriter(lyricData.syllabic));
            textGroup.setText(std::move(text));
            if (lyricData.hasExtend)
            {
                textGroup.setExtend(core::Extend{});
            }

            lyric.setChoice(core::LyricChoice::lyricTextGroup(std::move(textGroup)));
        }

        myOutNote.addLyric(std::move(lyric));
    }
}

void NoteWriter::setMiscData() const
{
    if (myNoteData.miscData.size() == 0)
    {
        return;
    }

    const std::string comma = ",";
    const std::string underscore = "_";

    std::vector<std::string> miscItems;
    bool isFirst = true;
    for (auto s : myNoteData.miscData)
    {
        std::string::size_type position = 0;
        while ((position = s.find(comma, position)) != std::string::npos)
        {
            s.replace(position, comma.size(), underscore);
            position++;
        }

        if (isFirst)
        {
            isFirst = false;
            miscItems.push_back(std::string{"##misc-data##"} + s);
        }
        else
        {
            miscItems.push_back(s);
        }
    }

    core::FormattedText footnote;
    footnote.setFontFamily(core::FontFamily{std::move(miscItems)});

    auto editorialVoice = myOutNote.editorialVoice();
    editorialVoice.setFootnote(std::move(footnote));
    myOutNote.setEditorialVoice(std::move(editorialVoice));
}

bool NoteWriter::findNormalNameAndDots(mx::api::DurationName &ioName, int &ioDots, double inTickLength) const
{
    const auto equals = [&](double a, double b) { return std::abs(a - b) < 0.0001; };

    const auto isMatch = [&](double durQuarters, int numDots, mx::api::DurationName name) {
        if (equals(mx::api::applyDots(durQuarters * static_cast<double>(myCursor.ticksPerQuarter), numDots),
                   inTickLength))
        {
            ioName = name;
            ioDots = numDots;
            return true;
        }

        return false;
    };

    for (int dots = 0; dots < 4; ++dots)
    {

        if (isMatch(mx::api::DUR_QUARTERS_VALUE_QUARTER, dots, mx::api::DurationName::quarter))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_EIGHTH, dots, mx::api::DurationName::eighth))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_16TH, dots, mx::api::DurationName::dur16th))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_MAXIMA, dots, mx::api::DurationName::maxima))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_LONGA, dots, mx::api::DurationName::longa))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_BREVE, dots, mx::api::DurationName::breve))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_WHOLE, dots, mx::api::DurationName::whole))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_HALF, dots, mx::api::DurationName::half))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_32ND, dots, mx::api::DurationName::dur32nd))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_64TH, dots, mx::api::DurationName::dur64th))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_128TH, dots, mx::api::DurationName::dur128th))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_256TH, dots, mx::api::DurationName::dur256th))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_512TH, dots, mx::api::DurationName::dur512th))
        {
            return true;
        }
        else if (isMatch(mx::api::DUR_QUARTERS_VALUE_1024TH, dots, mx::api::DurationName::dur1024th))
        {
            return true;
        }
    }

    return false;
}
} // namespace impl
} // namespace mx
