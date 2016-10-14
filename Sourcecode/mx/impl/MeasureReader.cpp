// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/MeasureReader.h"
#include "mx/api/ClefData.h"
#include "mx/api/KeyData.h"
#include "mx/api/NoteData.h"
#include "mx/api/Smufl.h"
#include "mx/core/elements/Alter.h"
#include "mx/core/elements/Backup.h"
#include "mx/core/elements/Barline.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/Bookmark.h"
#include "mx/core/elements/Cancel.h"
#include "mx/core/elements/Cancel.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/ClefOctaveChange.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DisplayOctave.h"
#include "mx/core/elements/DisplayStep.h"
#include "mx/core/elements/DisplayStepOctaveGroup.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Fifths.h"
#include "mx/core/elements/Fifths.h"
#include "mx/core/elements/FiguredBass.h"
#include "mx/core/elements/Forward.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/Grouping.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/Key.h"
#include "mx/core/elements/KeyAccidental.h"
#include "mx/core/elements/KeyAlter.h"
#include "mx/core/elements/KeyChoice.h"
#include "mx/core/elements/KeyOctave.h"
#include "mx/core/elements/KeyStep.h"
#include "mx/core/elements/Line.h"
#include "mx/core/elements/Link.h"
#include "mx/core/elements/MeasureStyle.h"
#include "mx/core/elements/MeasureStyleChoice.h"
#include "mx/core/elements/Mode.h"
#include "mx/core/elements/Mode.h"
#include "mx/core/elements/MultipleRest.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/NonTraditionalKey.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Octave.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Print.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Sign.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Step.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include "mx/core/elements/TraditionalKey.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/impl/Converter.h"
#include "mx/impl/DirectionReader.h"
#include "mx/impl/NoteFunctions.h"
#include "mx/impl/NoteReader.h"
#include "mx/impl/TimeFunctions.h"
#include "mx/utility/Throw.h"

namespace mx
{
    namespace impl
    {
        MeasureReader::MeasureReader( const core::PartwiseMeasure& inPartwiseMeasureRef, const MeasureReaderParameters& params )
        : myMutex{}
        , myPartwiseMeasure{ inPartwiseMeasureRef }
        , myNumStaves{ params.numStaves }
        , myGlobalTicksPerQuarter{ params.globalTicksPerQuarter }
        , myIsFirstMeasure{ params.isFirstMeasure }
        , myIsFirstPart{ params.isFirstPart }
        , myOutMeasureData{}
        , myCurrentCursor{ params.numStaves, params.globalTicksPerQuarter }
        , myPreviousCursor{ params.numStaves, params.globalTicksPerQuarter }
        , myMeasureIndex( params.measureIndex )
        {
            
        }
        
        void MeasureReader::addStavesToOutMeasure() const
        {
            for( int i = 0; i < myNumStaves; ++ i )
            {
                myOutMeasureData.staves.emplace_back( api::StaffData{} );
            }
        }

        api::MeasureData MeasureReader::getMeasureData() const
        {
            // lock due to the use of a mutable in const function
            std::lock_guard<std::mutex> lock(myMutex);
            myOutMeasureData = api::MeasureData{};
            const auto& attr = *myPartwiseMeasure.getAttributes();
            myOutMeasureData.number = attr.number.getValue();
            
            if( myOutMeasureData.number == std::to_string( myMeasureIndex + 1 ) )
            {
                myOutMeasureData.number = "";
            }
            
            if( attr.hasWidth )
            {
                myOutMeasureData.width = attr.width.getValue();
            }
            
            if( attr.hasImplicit )
            {
                myOutMeasureData.implicit = attr.implicit == core::YesNo::yes ? api::Bool::yes : api::Bool::no;
            }
            
            addStavesToOutMeasure();
            
            bool isTimeSignatureFound = false;
            
            const auto& mdcSet = myPartwiseMeasure.getMusicDataGroup()->getMusicDataChoiceSet();
            auto iter = mdcSet.cbegin();
            const auto endIter = mdcSet.cend();
            
            for( ; iter != endIter; ++iter )
            {
                const auto& mdc = **iter;
                
                // incredibly, we need to know if the note following this one has a 'chord' tag
                // otherwise we don't know whether or not the current note if part of a chord,
                // which we need to know so that we can choose whether or not to increment the
                // current tick position in myCurrentCursor
                auto peekAheadAtNextNoteIter = iter + 1;
                
                while( mdc.getChoice() == core::MusicDataChoice::Choice::note
                       && peekAheadAtNextNoteIter != endIter
                       && (*peekAheadAtNextNoteIter)->getChoice() != core::MusicDataChoice::Choice::note )
                {
                    ++peekAheadAtNextNoteIter;
                }
                
                core::NotePtr nextNotePtr = nullptr;
                if( peekAheadAtNextNoteIter != endIter )
                {
                    nextNotePtr = (*peekAheadAtNextNoteIter)->getNote();
                }
                
                if( !isTimeSignatureFound )
                {
                    impl::TimeFunctions timeFunc;
                    isTimeSignatureFound = timeFunc.findAndFillTimeSignature( mdc, myCurrentCursor.timeSignature );
                    bool implicitTime = true;
                    if( myCurrentCursor.isFirstMeasureInPart )
                    {
                        implicitTime = false;
                    }
                    else
                    {
                        implicitTime = timeFunc.isTimeSignatureImplicit( myPreviousCursor.timeSignature, myCurrentCursor.timeSignature, myCurrentCursor.isFirstMeasureInPart );
                    }
                    myCurrentCursor.timeSignature.isImplicit = implicitTime;
                }
                parseMusicDataChoice( mdc, nextNotePtr );
            }
            
            myCurrentCursor.isFirstMeasureInPart = false;
            
            // move the data to a temp then return the temp
            auto temp = api::MeasureData{ std::move(myOutMeasureData) };
            myOutMeasureData = api::MeasureData{};
            return temp;
        }
        
        
        void MeasureReader::parseMusicDataChoice( const core::MusicDataChoice& mdc, const core::NotePtr& nextNotePtr ) const
        {
            switch ( mdc.getChoice() )
            {
                case core::MusicDataChoice::Choice::note:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseNote( *mdc.getNote(), nextNotePtr );
                    break;
                }
                case core::MusicDataChoice::Choice::backup:
                {
                    parseBackup( *mdc.getBackup() );
                    
                    if( myCurrentCursor.tickTimePosition < 0 )
                    {
                        myCurrentCursor.tickTimePosition = 0;
                        // TODO - log or inform the client that the file is erroneous
                    }
                    break;
                }
                case core::MusicDataChoice::Choice::forward:
                {
                    
                    myCurrentCursor.isBackupInProgress = false;
                    parseForward( *mdc.getForward() );
                    break;
                }
                case core::MusicDataChoice::Choice::direction:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseDirection( *mdc.getDirection() );
                    break;
                }
                case core::MusicDataChoice::Choice::properties:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseProperties( *mdc.getProperties() );
                    break;
                }
                case core::MusicDataChoice::Choice::harmony:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseHarmony( *mdc.getHarmony() );
                    break;
                }
                case core::MusicDataChoice::Choice::figuredBass:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseFiguredBass( *mdc.getFiguredBass() );
                    break;
                }
                case core::MusicDataChoice::Choice::print:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parsePrint( *mdc.getPrint() );
                    break;
                }
                case core::MusicDataChoice::Choice::sound:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseSound( *mdc.getSound() );
                    break;
                }
                case core::MusicDataChoice::Choice::barline:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseBarline( *mdc.getBarline() );
                    break;
                }
                case core::MusicDataChoice::Choice::grouping:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseGrouping( *mdc.getGrouping() );
                    break;
                }
                case core::MusicDataChoice::Choice::link:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseLink( *mdc.getLink() );
                    break;
                }
                case core::MusicDataChoice::Choice::bookmark:
                {
                    myCurrentCursor.isBackupInProgress = false;
                    parseBookmark( *mdc.getBookmark() );
                    break;
                }
                default:
                {
                    MX_THROW( "unsupported MusicDataChoice::Choice value" );
                }
            }
        }
        
        
        void MeasureReader::parseNote( const core::Note& inMxNote, const core::NotePtr& nextNotePtr ) const
        {
            MX_UNUSED( inMxNote );
            MX_UNUSED( nextNotePtr );
            
            bool isNextNoteAChordMemberMakingThisNoteTheStartOfTheChord = false;
            
            if( nextNotePtr )
            {
                NoteReader nextNoteReader{ *nextNotePtr };
                isNextNoteAChordMemberMakingThisNoteTheStartOfTheChord = nextNoteReader.getIsChord();
            }
            
            myCurrentCursor.isBackupInProgress = false;
            impl::NoteFunctions noteFunc{ inMxNote, myCurrentCursor};
            auto noteData = noteFunc.parseNote();
            
            if( noteData.staffIndex >= 0 )
            {
                myCurrentCursor.staffIndex = noteData.staffIndex;
            }
            else
            {
                myCurrentCursor.staffIndex = 0;
            }
            
            if( !noteData.isChord && !isNextNoteAChordMemberMakingThisNoteTheStartOfTheChord)
            {
                myCurrentCursor.tickTimePosition += noteData.durationData.durationTimeTicks;
            }
            
            insertNoteData( std::move(noteData), myCurrentCursor.staffIndex, myCurrentCursor.voiceIndex );
        }
        
        
        void MeasureReader::parseBackup( const core::Backup& inMxBackup ) const
        {
            if( ! ( myCurrentCursor.isBackupInProgress ) )
            {
                ++myCurrentCursor.voiceIndex;
            }
            else
            {
                MX_THROW( "multiple backups in a row" ); // TODO - remove this debugging check
            }
            myCurrentCursor.isBackupInProgress = true;
            impl::TimeFunctions timeFunc;
            const int backupAmount = timeFunc.convertDurationToGlobalTickScale( myCurrentCursor, *inMxBackup.getDuration() );
            myCurrentCursor.tickTimePosition -= backupAmount;
        }
        
        
        void MeasureReader::parseForward( const core::Forward& inMxForward ) const
        {
            impl::TimeFunctions timeFunc;
            const int forwardAmount = timeFunc.convertDurationToGlobalTickScale( myCurrentCursor, *inMxForward.getDuration() );
            myCurrentCursor.tickTimePosition += forwardAmount;
        }
        
        
        void MeasureReader::parseDirection( const core::Direction& inMxDirection ) const
        {
            
            DirectionReader reader{ inMxDirection, myCurrentCursor };
            reader.getDirectionData( myOutMeasureData );
        }
        
        
        void MeasureReader::parseProperties( const core::Properties& inMxProperties ) const
        {
            // TODO - continue work on measure numbering and style etc
            for( const auto& measureStylePtr : inMxProperties.getMeasureStyleSet() )
            {
                const auto& measureStyle = *measureStylePtr;
                if( measureStyle.getMeasureStyleChoice()->getChoice() == core::MeasureStyleChoice::Choice::multipleRest )
                {
                    const auto& mr = *measureStyle.getMeasureStyleChoice()->getMultipleRest();
                    if( !mr.getValue().getIsEmpty() )
                    {
                        const auto value = mr.getValue().getValueNumber().getValue();
                        myOutMeasureData.multiMeasureRest = value;
                    }
                }
            }
            
            for( const auto& keyPtr : inMxProperties.getKeySet() )
            {
                
                const auto& key = *keyPtr;
                const auto& attr = *key.getAttributes();
                const auto keyChoiceObj = *key.getKeyChoice();
                const auto keyType = keyChoiceObj.getChoice();

                if( keyType == core::KeyChoice::Choice::nonTraditionalKey )
                {
                    // TODO - support non-traditional keys
                    continue;
                }
                
                api::KeyData keyData;
                // TODO - do position attributes
                const auto& traditionalKey = *keyChoiceObj.getTraditionalKey();
                
                if( attr.hasNumber )
                {
                    keyData.staffIndex = attr.number.getValue() - 1;
                    if( keyData.staffIndex > myNumStaves -1 )
                    {
                        keyData.staffIndex = -1;
                    }
                }
                
                keyData.fifths = traditionalKey.getFifths()->getValue().getValue();
                if( traditionalKey.getHasCancel() )
                {
                    keyData.cancel = traditionalKey.getCancel()->getValue().getValue();
                }
                
                if( traditionalKey.getHasMode() )
                {
                    // TODO - support all modes, not just major/minor
                    const auto coreMode = traditionalKey.getMode()->getValue().getValue();
                    if( coreMode == core::ModeEnum::major )
                    {
                        keyData.mode = api::KeyMode::major;
                    }
                    else if( coreMode == core::ModeEnum::minor )
                    {
                        keyData.mode = api::KeyMode::minor;
                    }
                    else if( coreMode == core::ModeEnum::minor )
                    {
                        keyData.mode = api::KeyMode::unsupported;
                    }
                }
                keyData.tickTimePosition = myCurrentCursor.tickTimePosition;
                
                myOutMeasureData.keys.emplace_back( std::move( keyData ) );
            }
            importClefs( inMxProperties.getClefSet() );
        }
        
        
        void MeasureReader::parseHarmony( const core::Harmony& inMxHarmony ) const
        {
            coutItemNotSupported( inMxHarmony );
        }
        
        
        void MeasureReader::parseFiguredBass( const core::FiguredBass& inMxFiguredBass ) const
        {
            coutItemNotSupported( inMxFiguredBass );
        }
        
        
        void MeasureReader::parsePrint( const core::Print& inMxPrint ) const
        {
            coutItemNotSupported( inMxPrint );
        }
        
        
        void MeasureReader::parseSound( const core::Sound& inMxSound ) const
        {
            coutItemNotSupported( inMxSound );
        }
        
        
        void MeasureReader::parseBarline( const core::Barline& inMxBarline ) const
        {
            coutItemNotSupported( inMxBarline );
        }
        
        
        void MeasureReader::parseGrouping( const core::Grouping& inMxGrouping ) const
        {
            coutItemNotSupported( inMxGrouping );
        }
        
        
        void MeasureReader::parseLink( const core::Link& inMxLink ) const
        {
            coutItemNotSupported( inMxLink );
        }
        
        
        void MeasureReader::parseBookmark( const core::Bookmark& inMxBookmark ) const
        {
            coutItemNotSupported( inMxBookmark );
        }
        
        
        void MeasureReader::coutItemNotSupported( const core::ElementInterface& element ) const
        {
            MX_UNUSED(element);
            //std::cout << element.getElementName() << " is not supported" << std::endl;
        }
        
        
        void MeasureReader::importClefs( const core::ClefSet& inClefs ) const
        {
            auto iter = inClefs.cbegin();
            auto endIter = inClefs.cend();
            
            for( ; iter != endIter; ++iter)
            {
                const auto& clef = **iter;
                importClef( clef );
            }
        }
        
        
        void MeasureReader::importClef( const core::Clef& inClef ) const
        {
            api::ClefData clefData;
            clefData.tickTimePosition = myCurrentCursor.tickTimePosition;
            auto converter = Converter{};
            clefData.symbol = converter.convert( inClef.getSign()->getValue() );
            
            if( inClef.getHasLine() )
            {
                clefData.line = inClef.getLine()->getValue().getValue();
            }
            else
            {
                switch (clefData.symbol)
                {
                    case api::ClefSymbol::g:
                        clefData.line = 2;
                        break;
                    case api::ClefSymbol::f:
                        clefData.line = 4;
                        break;
                    case api::ClefSymbol::c:
                        clefData.line = 3;
                        break;
                    case api::ClefSymbol::jianpu:
                        clefData.line = 1;
                        break;
                    case api::ClefSymbol::tab:
                        clefData.line = 1;
                        break;
                    case api::ClefSymbol::percussion:
                        clefData.line = 3;
                        break;
                    default:
                        clefData.line = 3;
                        break;
                }
            }
            
            if( inClef.getHasClefOctaveChange() )
            {
                clefData.octaveChange = inClef.getClefOctaveChange()->getValue().getValue();
            }
            else
            {
                clefData.octaveChange = 0;
            }
            
            const auto& attr = *inClef.getAttributes();
            
            if( attr.hasNumber )
            {
                clefData.staffIndex = attr.number.getValue() - 1;
            }
            else
            {
                clefData.staffIndex = 0;
            }
            
            if( myCurrentCursor.tickTimePosition == 0 )
            {
                if( attr.hasAfterBarline )
                {
                    if( attr.afterBarline == core::YesNo::yes )
                    {
                        clefData.location = api::ClefLocation::afterBarline;
                    }
                    else
                    {
                        clefData.location = api::ClefLocation::beforeBarline;
                    }
                }
                else
                {
                    clefData.location = api::ClefLocation::unspecified;
                }
            }
            else
            {
                clefData.location = api::ClefLocation::midMeasure;
            }
            insertClef( std::move( clefData ), clefData.staffIndex );
        }
    
        
        void MeasureReader::insertNoteData( api::NoteData&& noteData, int staff, int voice ) const
        {
            MX_ASSERT( staff >= 0 );
            MX_ASSERT( static_cast<size_t>( staff ) < myOutMeasureData.staves.size() );
            auto& staffRef = myOutMeasureData.staves.at( static_cast<size_t>( staff ) );
            
            if( staffRef.voices.find( voice ) == staffRef.voices.cend() )
            {
                staffRef.voices[voice] = api::VoiceData{};
            }
            auto& voiceRef = staffRef.voices[voice];
            voiceRef.notes.emplace_back( std::move( noteData ) );
        }
        
        
        void MeasureReader::insertClef( api::ClefData&& clefData, int staff ) const
        {
            MX_ASSERT( staff >= 0 );
            MX_ASSERT( static_cast<size_t>( staff ) < myOutMeasureData.staves.size() );
            auto& staffRef = myOutMeasureData.staves.at( static_cast<size_t>( staff ) );
            staffRef.clefs.emplace_back( std::move( clefData ) );
        }
    }
}
