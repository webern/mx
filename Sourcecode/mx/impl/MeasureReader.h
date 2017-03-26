// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MeasureData.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MeasureCursor.h"

#include <mutex>

namespace mx
{
    namespace core
    {
        class ElementInterface;
        class PartwiseMeasure;
        class MusicDataChoice;
        class Note;
        using NotePtr = std::shared_ptr<Note>;
        class Backup;
        class Forward;
        class Direction;
        class Properties;
        class Harmony;
        class FiguredBass;
        class Print;
        class Sound;
        class Barline;
        class Grouping;
        class Link;
        class Bookmark;
        class Clef;
        using ClefPtr = std::shared_ptr<Clef>;
        using ClefSet = std::vector<ClefPtr>;
    }
    
    namespace impl
    {

    	class MeasureReader
    	{
    	public:
    		MeasureReader( const core::PartwiseMeasure& inPartwiseMeasureRef, const MeasureCursor& cursor, const MeasureCursor& previousMeasureCursor );

    		api::MeasureData getMeasureData() const;

    	private:
            mutable std::mutex myMutex;
            const core::PartwiseMeasure& myPartwiseMeasure;
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

        private:
            void addStavesToOutMeasure() const;
            void parseTimeSignature() const;
            void parseMusicDataChoice( const core::MusicDataChoice& mdc, const core::NotePtr& nextNotePtr ) const;
            void parseNote( const core::Note& inMxNote, const core::NotePtr& nextNotePtr ) const;
            void parseBackup( const core::Backup& inMxBackup ) const;
            void parseForward( const core::Forward& inMxForward ) const;
            void parseDirection( const core::Direction& inMxDirection ) const;
            void parseProperties( const core::Properties& inMxProperties ) const;
            void parseHarmony( const core::Harmony& inMxHarmony ) const;
            void parseFiguredBass( const core::FiguredBass& inMxFiguredBass ) const;
            void parsePrint( const core::Print& inMxPrint ) const;
            void parseSound( const core::Sound& inMxSound ) const;
            void parseBarline( const core::Barline& inMxBarline ) const;
            void parseGrouping( const core::Grouping& inMxGrouping ) const;
            void parseLink( const core::Link& inMxLink ) const;
            void parseBookmark( const core::Bookmark& inMxBookmark ) const;
            void coutItemNotSupported( const core::ElementInterface& element ) const;
            void importClefs( const core::ClefSet& inClefs ) const;
            void importClef( const core::Clef& inClef ) const;
            void insertNoteData( api::NoteData&& noteData, int staff, int voice ) const;
            void insertClef( api::ClefData&& clefData, int staff ) const;
            void consolidateVoicesForAllStaves() const;
            void takeUserRequestedVoiceNumbers( api::StaffData& staff ) const;
            void collapseVoicesAutomatically( api::StaffData& staff ) const;
            bool isUserRequestedVoiceNumberConsistent( const api::VoiceData& voiceData ) const;
            bool isUserRequestedVoiceNumberConsistentAccrossAllVoices( const api::StaffData& staff ) const;
            int getUserRequestedVoiceNumber( const api::VoiceData& voiceData ) const;
            void advanceTickTimePosition( int amount, std::string reason ) const;
    	};
    }
}
