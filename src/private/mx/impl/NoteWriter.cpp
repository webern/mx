// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NoteWriter.h"
#include "mx/core/Decimal.h"
#include "mx/core/NameToken.h"
#include "mx/core/generated/Accidental.h"
#include "mx/core/generated/BeamLevel.h"
#include "mx/core/generated/CueNoteGroup.h"
#include "mx/core/generated/DisplayStepOctaveGroup.h"
#include "mx/core/generated/Elision.h"
#include "mx/core/generated/ElisionSyllabicGroup.h"
#include "mx/core/generated/Extend.h"
#include "mx/core/generated/FormattedText.h"
#include "mx/core/generated/FullNoteGroupChoice.h"
#include "mx/core/generated/GraceCueNoteGroup.h"
#include "mx/core/generated/GraceNormalNoteGroup.h"
#include "mx/core/generated/GraceNoteChoice.h"
#include "mx/core/generated/GraceNoteGroup.h"
#include "mx/core/generated/LyricChoice.h"
#include "mx/core/generated/LyricSyllableGroup.h"
#include "mx/core/generated/LyricTextGroup.h"
#include "mx/core/generated/NonNegativeDecimal.h"
#include "mx/core/generated/NormalNoteGroup.h"
#include "mx/core/generated/Pitch.h"
#include "mx/core/generated/Rest.h"
#include "mx/core/generated/SmuflGlyphName.h"
#include "mx/core/generated/SmuflLyricsGlyphName.h"
#include "mx/core/generated/Syllabic.h"
#include "mx/core/generated/TextElementData.h"
#include "mx/core/generated/Tied.h"
#include "mx/core/generated/TimeModification.h"
#include "mx/core/generated/TimeModificationGroup.h"
#include "mx/core/generated/Unpitched.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/NotationsWriter.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/impl/WriteRefusal.h"

#include <algorithm>

namespace mx
{
namespace impl
{
core::Syllabic convertLyricSyllabicForNoteWriter(api::LyricSyllabic value)
{
    switch (value)
    {
    case api::LyricSyllabic::unspecified:
        break;
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
                       int inNumVoices, const std::string &inVoiceLabel)
    : myNoteData{inNoteData}, myCursor{inCursor}, myScoreWriter{inScoreWriter}, myConverter{},
      myIsPreviousNoteAChordMember{isPreviousNoteAChordMember}, mySiblingNotes{inSiblingNotes},
      myNumVoices{inNumVoices}, myVoiceLabel{inVoiceLabel}, myOutNote{}, myOutFullNoteGroup{}, myOutTies{},
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
    impl::setId(myNoteData.id, myOutNote);
    NotationsWriter notationsWriter{myNoteData, myCursor, myScoreWriter};
    impl::setAttributesFromPositionData(myNoteData.positionData, myOutNote);
    if (myNoteData.printData.printObject != api::Bool::unspecified)
    {
        myOutNote.setPrintObject(myConverter.convert(myNoteData.printData.printObject));
    }

    if (myNoteData.velocityStart.has_value())
    {
        myOutNote.setDynamics(core::NonNegativeDecimal{core::Decimal{*myNoteData.velocityStart}});
    }

    if (myNoteData.velocityStop.has_value())
    {
        myOutNote.setEndDynamics(core::NonNegativeDecimal{core::Decimal{*myNoteData.velocityStop}});
    }

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
        if (myNoteData.pitchData.isAccidentalParenthetical)
        {
            accidental.setParentheses(core::YesNo::yes());
        }
        if (myNoteData.pitchData.isAccidentalCautionary)
        {
            accidental.setCautionary(core::YesNo::yes());
        }
        if (myNoteData.pitchData.isAccidentalEditorial)
        {
            accidental.setEditorial(core::YesNo::yes());
        }
        if (myNoteData.pitchData.isAccidentalBracketed)
        {
            accidental.setBracket(core::YesNo::yes());
        }
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

    // 1/1 is NoteReader's "no time modification" sentinel -- writing it back would fabricate
    // a <time-modification> on every plain note, so it is skipped here.
    if (myNoteData.durationData.timeModificationNormalNotes > 0 &&
        myNoteData.durationData.timeModificationActualNotes > 0 &&
        (myNoteData.durationData.timeModificationNormalNotes > 1 ||
         myNoteData.durationData.timeModificationActualNotes > 1))
    {
        core::TimeModification timeMod;
        timeMod.setActualNotes(myNoteData.durationData.timeModificationActualNotes);
        timeMod.setNormalNotes(myNoteData.durationData.timeModificationNormalNotes);

        // <normal-type> is the author's statement, taken straight from DurationData. When it
        // is unspecified it is omitted, and MusicXML reads the absence as the note's own
        // <type> (#428).
        if (myNoteData.durationData.timeModificationNormalType != api::DurationName::unspecified)
        {
            core::TimeModificationGroup group;
            group.setNormalType(myConverter.convert(myNoteData.durationData.timeModificationNormalType));

            std::vector<core::Empty> normalDots;
            for (int i = 0; i < myNoteData.durationData.timeModificationNormalTypeDots; ++i)
            {
                normalDots.emplace_back();
            }
            group.setNormalDot(std::move(normalDots));
            timeMod.setGroup(std::move(group));
        }

        myOutNote.setTimeModification(std::move(timeMod));
    }

    setLyrics();

    return myOutNote;
}

// True when the note's curve vectors already carry a tie curve in the given
// direction. Those curves are written by NotationsWriter with their full
// attributes, so the bare <tied> synthesized here would be a duplicate.
bool NoteWriter::hasTieCurve(bool isStart) const
{
    if (isStart)
    {
        const auto &curves = myNoteData.noteAttachmentData.curveStarts;
        return std::any_of(curves.cbegin(), curves.cend(),
                           [](const api::CurveStart &curve) { return curve.curveType == api::CurveType::tie; });
    }

    const auto &curves = myNoteData.noteAttachmentData.curveStops;
    return std::any_of(curves.cbegin(), curves.cend(),
                       [](const api::CurveStop &curve) { return curve.curveType == api::CurveType::tie; });
}

// Records a tie in both of MusicXML's encodings: the sound-level <tie> (for the
// note choice) and the matching <tied> notation. A single <notations> element
// collects the tied choices, stop before start when both are present.
void NoteWriter::addTie(bool isStart) const
{
    // <tie> lives inside the note choice, and the schema gives it a slot in only
    // two of the four note flavors: normal and grace-normal. Cue and grace-cue
    // notes are silent, so a sound-level tie is meaningless on them.
    if (!myNoteData.isCue)
    {
        core::Tie tie;
        tie.setType(isStart ? core::StartStop::start() : core::StartStop::stop());
        myOutTies.push_back(std::move(tie));
    }

    // <tied> is a notation and <notations> sits outside the note choice, so it
    // is legal on all four flavors -- cue and grace-cue notes included.
    if (!hasTieCurve(isStart))
    {
        core::Tied tied;
        tied.setType(isStart ? core::TiedType::start() : core::TiedType::stop());
        myOutTieNotationsChoices.push_back(core::NotationsChoice::tied(std::move(tied)));
    }
}

void NoteWriter::setNoteChoiceAndFullNoteGroup(bool isStartOfChord) const
{
    myOutFullNoteGroup.setChord(myCursor.isChordActive && myIsPreviousNoteAChordMember && !isStartOfChord);

    if (myNoteData.isTieStop)
    {
        addTie(false);
    }

    if (myNoteData.isTieStart)
    {
        addTie(true);
    }
}

// Installs the assembled full-note group, duration, and ties into the note's
// choice. (The old writer mutated co-allocated members through shared
// pointers; under value semantics the alternative is built and installed.)
void NoteWriter::assembleNoteChoice() const
{
    const auto duration =
        core::PositiveDivisions{core::Decimal{static_cast<double>(myNoteData.durationData.durationTimeTicks)}};

    if (myNoteData.isGrace)
    {
        // Grace notes have no wire <duration>; durationTimeTicks is ignored.
        core::GraceNoteGroup choiceObj;
        if (myNoteData.graceSlash != api::Bool::unspecified)
        {
            core::Grace grace;
            grace.setSlash(myConverter.convert(myNoteData.graceSlash));
            choiceObj.setGrace(std::move(grace));
        }
        if (myNoteData.isCue)
        {
            // <grace/> + <cue/>: the grace-cue group has no <tie> slot, so
            // myOutTies is empty here; the tie survives as a <tied> notation.
            core::GraceCueNoteGroup inner;
            inner.setFullNote(myOutFullNoteGroup);
            choiceObj.setGraceNoteChoice(core::GraceNoteChoice::graceCueNoteGroup(std::move(inner)));
        }
        else
        {
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
            choiceObj.setGraceNoteChoice(core::GraceNoteChoice::graceNormalNoteGroup(std::move(inner)));
        }
        myOutNote.setChoice(core::NoteChoice::graceNoteGroup(std::move(choiceObj)));
    }
    else if (myNoteData.isCue)
    {
        core::CueNoteGroup choiceObj;
        choiceObj.setFullNote(myOutFullNoteGroup);
        choiceObj.setDuration(duration);
        myOutNote.setChoice(core::NoteChoice::cueNoteGroup(std::move(choiceObj)));
    }
    else
    {
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
    const bool isCrossStaff = myNoteData.crossStaffIndex.has_value() && *myNoteData.crossStaffIndex >= 0;

    if (isCrossStaff)
    {
        // cross-staff note: the containing staff governs voice membership and stream
        // position, but the displayed staff comes from the override (NoteData.h)
        myOutNote.setStaff(*myNoteData.crossStaffIndex + 1);
    }
    else if (myCursor.staffIndex >= 0)
    {
        // Auto rule: <staff> is structurally required on a multi-staff part and implied (1) on a
        // single-staff part. writeStaffNumber forces the decision either way (round-trip
        // fidelity), except that no cannot suppress <staff> off the first staff - MusicXML would
        // read the omission as staff 1.
        bool includeStaff = myCursor.getNumStaves() > 1;
        if (myNoteData.writeStaffNumber == api::Bool::yes)
        {
            includeStaff = true;
        }
        else if (myNoteData.writeStaffNumber == api::Bool::no && myCursor.staffIndex == 0)
        {
            includeStaff = false;
        }
        if (includeStaff)
        {
            myOutNote.setStaff(myCursor.staffIndex + 1);
        }
    }

    // VoiceData::label decides what goes in the <voice> element, replacing the voice's number
    // when the number cannot express its name. Which notes get the element is otherwise
    // unchanged, so a source that left <voice> off a chord member still does. The exception is a
    // voice where no note carried a number: there the label is the only thing asking for the
    // element, so it is what puts one there.
    const bool hasVoiceLabel = !myVoiceLabel.empty();
    const bool sourceHadVoice = myNoteData.userRequestedVoiceNumber != api::VALUE_UNSPECIFIED;
    const bool isNonDefaultVoice = myCursor.voiceIndex > 0;
    const bool isMultiVoiceStaff = myNumVoices > 1;
    const bool isVoiceNumbered = std::any_of(mySiblingNotes.cbegin(), mySiblingNotes.cend(), [](const auto &note) {
        return note.userRequestedVoiceNumber != api::VALUE_UNSPECIFIED;
    });
    const bool labelAsksForVoice = hasVoiceLabel && !isVoiceNumbered;

    if (myCursor.voiceIndex >= 0 && (sourceHadVoice || isNonDefaultVoice || isMultiVoiceStaff || labelAsksForVoice))
    {
        auto editorialVoice = myOutNote.editorialVoice();
        editorialVoice.setVoice(hasVoiceLabel ? myVoiceLabel : std::to_string(myCursor.voiceIndex + 1));
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
    const bool isFilledSpecified = myNoteData.noteheadFilled != api::Bool::unspecified;
    // An empty glyph name names no glyph, so it is treated the same as no smufl at all.
    const bool isSmuflSpecified = myNoteData.noteheadSmufl.has_value() && !myNoteData.noteheadSmufl->empty();

    // <notehead> stays out of the file for a plain notehead, but filled or smufl still needs
    // the element even when the value itself is 'normal'.
    if (myNoteData.notehead == api::Notehead::normal && !isFilledSpecified && !isSmuflSpecified)
    {
        return;
    }

    core::Notehead notehead;
    notehead.setValue(myConverter.convert(myNoteData.notehead));

    if (isFilledSpecified)
    {
        notehead.setFilled(myConverter.convert(myNoteData.noteheadFilled));
    }

    if (isSmuflSpecified)
    {
        notehead.setSmufl(core::SmuflGlyphName{*myNoteData.noteheadSmufl});
    }

    myOutNote.setNotehead(std::move(notehead));
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
        impl::setId(lyricData.id, lyric);
        if (lyricData.positionData.horizontalAlignment != api::HorizontalAlignment::unspecified)
        {
            lyric.setJustify(myConverter.convert(lyricData.positionData.horizontalAlignment));
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
            core::Extend extend;
            switch (lyricData.extendType)
            {
            case api::LyricExtendType::unspecified:
                break;
            case api::LyricExtendType::start:
                extend.setType(core::StartStopContinue::start());
                break;
            case api::LyricExtendType::continue_:
                extend.setType(core::StartStopContinue::continue_());
                break;
            case api::LyricExtendType::stop:
                extend.setType(core::StartStopContinue::stop());
                break;
            }
            lyric.setChoice(core::LyricChoice::extend(std::move(extend)));
        }
        else
        {
            core::TextElementData text;
            text.setValue(lyricData.text);
            setAttributesFromFontData(lyricData.printData.fontData, text);

            core::LyricTextGroup textGroup;
            if (lyricData.syllabic != api::LyricSyllabic::unspecified)
            {
                textGroup.setSyllabic(convertLyricSyllabicForNoteWriter(lyricData.syllabic));
            }
            textGroup.setText(std::move(text));
            for (const auto &segment : lyricData.continuations)
            {
                core::Elision elision;
                if (segment.elisionText.has_value())
                {
                    elision.setValue(*segment.elisionText);
                }
                if (segment.elisionSmufl.has_value())
                {
                    elision.setSmufl(core::SmuflLyricsGlyphName::parse(*segment.elisionSmufl));
                }

                core::ElisionSyllabicGroup elisionSyllabicGroup;
                elisionSyllabicGroup.setElision(std::move(elision));
                if (segment.syllabic != api::LyricSyllabic::unspecified)
                {
                    elisionSyllabicGroup.setSyllabic(convertLyricSyllabicForNoteWriter(segment.syllabic));
                }

                core::TextElementData segmentText;
                segmentText.setValue(segment.text);
                setAttributesFromFontData(lyricData.printData.fontData, segmentText);

                core::LyricSyllableGroup syllableGroup;
                syllableGroup.setElisionSyllabicGroup(std::move(elisionSyllabicGroup));
                syllableGroup.setText(std::move(segmentText));
                textGroup.addLyricSyllableGroup(std::move(syllableGroup));
            }
            if (lyricData.hasExtend)
            {
                core::Extend extend;
                switch (lyricData.extendType)
                {
                case api::LyricExtendType::unspecified:
                    break;
                case api::LyricExtendType::start:
                    extend.setType(core::StartStopContinue::start());
                    break;
                case api::LyricExtendType::continue_:
                    extend.setType(core::StartStopContinue::continue_());
                    break;
                case api::LyricExtendType::stop:
                    extend.setType(core::StartStopContinue::stop());
                    break;
                }
                textGroup.setExtend(std::move(extend));
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
        // Comma is the item separator in the miscellaneous-field wire encoding, so commas
        // inside user misc-data are irreversibly replaced with underscores.
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
} // namespace impl
} // namespace mx
