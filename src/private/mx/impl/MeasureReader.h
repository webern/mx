// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MeasureData.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MeasureCursor.h"

#include <map>
#include <mutex>

namespace mx
{
namespace core
{
class PartwiseMeasure;
class MusicDataChoice;
class Note;
class Backup;
class Forward;
class Direction;
class Attributes;
class Harmony;
class FiguredBass;
class Print;
class Sound;
class Barline;
class Grouping;
class Link;
class Bookmark;
class Clef;
} // namespace core

namespace impl
{

class MeasureReader
{
  public:
    MeasureReader(const core::PartwiseMeasure &inPartwiseMeasureRef, const MeasureCursor &cursor,
                  const MeasureCursor &previousMeasureCursor);

    std::pair<api::MeasureData, std::optional<api::TransposeData>> getMeasureData() const;
    impl::MeasureCursor getCursor() const;

  private:
    mutable std::mutex myMutex;
    const core::PartwiseMeasure &myPartwiseMeasure;
    const Converter myConverter;

    mutable api::MeasureData myOutMeasureData;
    mutable MeasureCursor myCurrentCursor;
    mutable MeasureCursor myPreviousMeasureCursor;

    class HistoryRecord
    {
      public:
        std::string reason;
        int amount;
        int timeBefore;
        int timeAfter;
        MeasureCursor cursorBefore;
        MeasureCursor cursorAfter;
    };

    mutable std::vector<HistoryRecord> myHistory;

    // cross-staff bookkeeping (issue #289): for each note that belongs to a level-1 beam
    // group spanning more than one <staff> value, the staff bucket that should contain it,
    // keyed by the note's index in the measure's musicData sequence; built by
    // scanForCrossStaffBeamGroups before the parse loop runs
    mutable std::map<size_t, int> myCrossStaffHomes;

    // the staff bucket that received the previous note; <chord> members follow it so a
    // chord cannot be split across staff buckets; -1 means "no previous note"
    mutable int myPreviousNoteBucketStaffIndex;

  private:
    void addStavesToOutMeasure() const;
    void parseTimeSignature() const;

    /// Parses the contents of the measure and writes data to myOutMeasureData. In mx::api,
    /// the first transposition at the zero position of the first measure is considered a
    /// property of the part (not the measure). Since we don't have access to the part in
    /// this function (as far as I can tell), the transpose info is returned up the call
    /// stack if measure index is zero and time index is zero.
    std::optional<api::TransposeData> parseMusicDataChoice(const core::MusicDataChoice &mdc,
                                                           const core::Note *nextNotePtr, size_t inMdcIndex) const;

    void parseNote(const core::Note &inMxNote, const core::Note *nextNotePtr, size_t inMdcIndex) const;
    void scanForCrossStaffBeamGroups() const;
    void parseBackup(const core::Backup &inMxBackup) const;
    void parseForward(const core::Forward &inMxForward) const;
    void parseDirection(const core::Direction &inDirection) const;

    /// Parses the measure 'attributes' (i.e. the `<attributes>` element) and writes data to
    /// myOutMeasureData. In mx::api, the first transposition at the zero position of the
    /// first measure is considered a property of the part (not the measure). Since we don't
    /// have access to the part in this function (as far as I can tell), the transpose info
    /// is returned up the call stack if measure index is zero and time index is zero.
    std::optional<api::TransposeData> parseAttributes(const core::Attributes &inMxAttributes) const;

    void parseHarmony(const core::Harmony &inHarmony) const;
    void parseFiguredBass(const core::FiguredBass &inMxFiguredBass, const core::Note *nextNotePtr) const;
    void parsePrint(const core::Print &inMxPrint) const;
    void parseSound(const core::Sound &inMxSound) const;
    void parseBarline(const core::Barline &inMxBarline) const;
    void parseGrouping(const core::Grouping &inMxGrouping) const;
    void parseLink(const core::Link &inMxLink) const;
    void parseBookmark(const core::Bookmark &inMxBookmark) const;
    void importStaffDetails(const core::Attributes &inMxAttributes) const;
    void importClefs(std::span<const core::Clef> inClefs) const;
    void importClef(const core::Clef &inClef) const;
    void insertNoteData(api::NoteData &&noteData, int staff, int voice) const;
    void insertClef(api::ClefData &&clefData, int staff) const;
    void consolidateVoicesForAllStaves() const;
    void takeUserRequestedVoiceNumbers(api::StaffData &staff) const;
    void collapseVoicesAutomatically(api::StaffData &staff) const;
    bool isUserRequestedVoiceNumberConsistent(const api::VoiceData &voiceData) const;
    bool isUserRequestedVoiceNumberConsistentAccrossAllVoices(const api::StaffData &staff) const;
    int getUserRequestedVoiceNumber(const api::VoiceData &voiceData) const;
    void advanceTickTimePosition(int amount, std::string reason) const;
};
} // namespace impl
} // namespace mx
