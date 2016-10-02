// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MeasureData.h"
#include "mx/impl/Cursor.h"

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
        
        struct MeasureReaderParameters
        {
            // number of staves in the part, i.e.
            // the max <staff>x</staff> or max
            // <staves>x</staves> found in the part
            int numStaves;
            
            // ticks per quarter to be used for all
            // measures throughout the entire score
            // must be calculated at the top and
            // past down to the import routines
            int globalTicksPerQuarter;
            
            // is this the first measure in the part
            bool isFirstMeasure;
            int measureIndex;
            
            // is this the first part in the score
            bool isFirstPart;
            
            MeasureReaderParameters()
            : numStaves{ 0 }
            , globalTicksPerQuarter{ 1 }
            , isFirstMeasure{ false }
            , measureIndex( -1 )
            , isFirstPart{ false }
            {
                
            }
        };
        
    	class MeasureReader
    	{
    	public:
    		MeasureReader( const core::PartwiseMeasure& inPartwiseMeasureRef, const MeasureReaderParameters& params );

    		api::MeasureData getMeasureData() const;

    	private:
            mutable std::mutex myMutex;
            const core::PartwiseMeasure& myPartwiseMeasure;
            const int myNumStaves;
            const int myGlobalTicksPerQuarter;
            const bool myIsFirstMeasure;
            const bool myIsFirstPart;
            const int myMeasureIndex;
            
            mutable api::MeasureData myOutMeasureData;
            mutable Cursor myCurrentCursor;
            mutable Cursor myPreviousCursor;
            
            
        private:
            void addStavesToOutMeasure() const;
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
    	};
    }
}
