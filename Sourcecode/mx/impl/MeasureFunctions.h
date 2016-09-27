// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/MeasureData.h"
#include "mx/api/TimeSignatureData.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/impl/Cursor.h"
#include "mx/utility/Throw.h"

#include <map>

namespace mx
{
    namespace core
    {
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
        using StaffIndexMeasureMap = std::map<int, api::MeasureData>;
        class MeasureFunctions
        {
        public:
            MeasureFunctions( int numStaves, int globalTicksPerQuarter );
            ~MeasureFunctions() = default;
            MeasureFunctions( const MeasureFunctions& ) = default;
            MeasureFunctions( MeasureFunctions&& ) = default;
            MeasureFunctions& operator=( const MeasureFunctions& ) = default;
            MeasureFunctions& operator=( MeasureFunctions&& ) = default;
            
            StaffIndexMeasureMap parseMeasure( const core::PartwiseMeasure& inMxMeasure );
            
        private:
            impl::Cursor myCurrentCursor;
            impl::Cursor myPreviousCursor;
            StaffIndexMeasureMap myStaves;
        
        private:
            void parseMusicDataChoice( const core::MusicDataChoice& mdc, const core::NotePtr& nextNotePtr );
            void parseNote( const core::Note& inMxNote, const core::NotePtr& nextNotePtr );
            void parseBackup( const core::Backup& inMxBackup );
            void parseForward( const core::Forward& inMxForward );
            void parseDirection( const core::Direction& inMxDirection );
            void parseProperties( const core::Properties& inMxProperties );
            void parseHarmony( const core::Harmony& inMxHarmony );
            void parseFiguredBass( const core::FiguredBass& inMxFiguredBass );
            void parsePrint( const core::Print& inMxPrint );
            void parseSound( const core::Sound& inMxSound );
            void parseBarline( const core::Barline& inMxBarline );
            void parseGrouping( const core::Grouping& inMxGrouping );
            void parseLink( const core::Link& inMxLink );
            void parseBookmark( const core::Bookmark& inMxBookmark );
            void coutItemNotSupported( const core::ElementInterface& element );
            void importClefs( const core::ClefSet& inClefs );
            void importClef( const core::Clef& inClef );
        };
    }
}
