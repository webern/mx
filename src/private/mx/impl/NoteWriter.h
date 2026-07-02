// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/NoteData.h"
#include "mx/core/generated/FullNoteGroup.h"
#include "mx/core/generated/Notations.h"
#include "mx/core/generated/NotationsChoice.h"
#include "mx/core/generated/Note.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MeasureCursor.h"

#include <vector>

namespace mx
{
namespace impl
{
class ScoreWriter;

class NoteWriter
{
  public:
    NoteWriter(const api::NoteData &inNoteData, const MeasureCursor &inCursor, const ScoreWriter &inScoreWriter,
               bool isPreviousNoteAChordMember, const std::vector<mx::api::NoteData> &inSiblingNotes, int inNoteIndex,
               int inNumVoices);

    core::Note getNote(bool isStartOfChord) const;

  private:
    const api::NoteData &myNoteData;
    const impl::MeasureCursor &myCursor;
    const impl::ScoreWriter &myScoreWriter;
    const Converter myConverter;
    const bool myIsPreviousNoteAChordMember;
    const std::vector<mx::api::NoteData> &mySiblingNotes;
    const int myNoteIndex;
    const int myNumVoices;
    mutable core::Note myOutNote;
    mutable core::FullNoteGroup myOutFullNoteGroup;
    mutable std::vector<core::Tie> myOutTies;
    mutable std::vector<core::NotationsChoice> myOutTieNotationsChoices;

  private:
    void addTie(bool isStart) const;
    void setNoteChoiceAndFullNoteGroup(bool isStartOfChord) const;
    void assembleNoteChoice() const;
    void setFullNoteTypeChoice() const;
    void setStaffAndVoice() const;
    void setDurationNameAndDots() const;
    void setNotehead() const;
    void setStemDirection() const;
    void setLyrics() const;
    void setMiscData() const;
    bool findNormalNameAndDots(mx::api::DurationName &ioName, int &ioDots, double inTickLength) const;
};
} // namespace impl
} // namespace mx
