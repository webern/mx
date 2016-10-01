// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/MeasureFunctions.h"
#include "mx/api/NoteData.h"
#include "mx/core/elements/Alter.h"
#include "mx/core/elements/Backup.h"
#include "mx/core/elements/Barline.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/Bookmark.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/Sign.h"
#include "mx/core/elements/Line.h"
#include "mx/core/elements/ClefOctaveChange.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DisplayOctave.h"
#include "mx/core/elements/DisplayStep.h"
#include "mx/core/elements/DisplayStepOctaveGroup.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/FiguredBass.h"
#include "mx/core/elements/Forward.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/Grouping.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/Link.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Octave.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Print.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Step.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/impl/MxNoteReader.h"
#include "mx/impl/NoteFunctions.h"
#include "mx/impl/TimeFunctions.h"
#include "mx/utility/Throw.h"
#include "mx/api/ClefData.h"
#include "mx/impl/Converter.h"
#include "mx/api/Smufl.h"

#include <string>

namespace mx
{
    namespace impl
    {
        api::Step converCoreStepToApiStep( core::StepEnum coreStep );
        api::Step converCoreStepToApiStep( core::StepEnum coreStep )
        {
            switch ( coreStep )
            {
                case core::StepEnum::c:
                    return api::Step::c;
                case core::StepEnum::d:
                    return api::Step::d;
                case core::StepEnum::e:
                    return api::Step::e;
                case core::StepEnum::f:
                    return api::Step::f;
                case core::StepEnum::g:
                    return api::Step::g;
                case core::StepEnum::a:
                    return api::Step::a;
                case core::StepEnum::b:
                    return api::Step::b;
                default:
                    break;
            }
            MX_BUG;
        }
        
        api::DurationName converCoreDurationEnumToApiDurationEnum( core::NoteTypeValue noteTypeValue );
        api::DurationName converCoreDurationEnumToApiDurationEnum( core::NoteTypeValue noteTypeValue )
        {
            switch ( noteTypeValue )
            {
                case core::NoteTypeValue::oneThousandTwentyFourth: return api::DurationName::dur1024th;
                case core::NoteTypeValue::fiveHundredTwelfth: return api::DurationName::dur512th;
                case core::NoteTypeValue::twoHundredFifthySixth: return api::DurationName::dur256th;
                case core::NoteTypeValue::oneHundredTwentyEighth: return api::DurationName::dur128th;
                case core::NoteTypeValue::sixtyFourth: return api::DurationName::dur64th;
                case core::NoteTypeValue::thirtySecond: return api::DurationName::dur32nd;
                case core::NoteTypeValue::sixteenth: return api::DurationName::dur16th;
                case core::NoteTypeValue::eighth: return api::DurationName::eighth;
                case core::NoteTypeValue::quarter: return api::DurationName::quarter;
                case core::NoteTypeValue::half: return api::DurationName::half;
                case core::NoteTypeValue::whole: return api::DurationName::whole;
                case core::NoteTypeValue::breve: return api::DurationName::breve;
                case core::NoteTypeValue::long_: return api::DurationName::longa;
                case core::NoteTypeValue::maxima: return api::DurationName::maxima;
                default:
                    break;
            }
            return api::DurationName::unspecified;
        }
        
        
        MeasureFunctions::MeasureFunctions( int numStaves, int globalTicksPerQuarter )
        : myCurrentCursor{ numStaves, globalTicksPerQuarter }
        , myPreviousCursor{ numStaves, globalTicksPerQuarter }
        , myStaves{}
        , myAbsoluteNoteIndex{ 0 }
        {
            myCurrentCursor.isFirstMeasureInPart = true;
        }
        
        
        StaffIndexMeasureMap MeasureFunctions::parseMeasure( const core::PartwiseMeasure& inMxMeasure )
        {
            myStaves.clear();
            myCurrentCursor.reset();
            
            if( myCurrentCursor.isFirstMeasureInPart )
            {
                myCurrentCursor.timeSignature.isImplicit = false;
                
            }
            
            myPreviousCursor = myCurrentCursor;
            bool isTimeSignatureFound = false;
            
            const auto& mdcSet = inMxMeasure.getMusicDataGroup()->getMusicDataChoiceSet();
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
                    myCurrentCursor.timeSignature.isImplicit = timeFunc.isTimeSignatureImplicit( myPreviousCursor.timeSignature, myCurrentCursor.timeSignature, myCurrentCursor.isFirstMeasureInPart );
                }
                parseMusicDataChoice( mdc, nextNotePtr );
            }
            
            // assign the correct time signature to all staves
            // and also initialize any staves that were missing
            for( int i = 0; i < myCurrentCursor.getNumStaves(); ++i )
            {
                myStaves[i].timeSignature = myCurrentCursor.timeSignature;
            }
            
            myCurrentCursor.isFirstMeasureInPart = false;
            return myStaves;
        }
        
        
        void MeasureFunctions::parseMusicDataChoice( const core::MusicDataChoice& mdc, const core::NotePtr& nextNotePtr )
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
                    
                    if( myCurrentCursor.position < 0 )
                    {
                        myCurrentCursor.position = 0;
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
        
        
        void MeasureFunctions::parseNote( const core::Note& inMxNote, const core::NotePtr& nextNotePtr )
        {
            bool isNextNoteAChordMemberMakingThisNoteTheStartOfTheChord = false;
            
            if( nextNotePtr )
            {
                MxNoteReader nextNoteReader{ *nextNotePtr };
                isNextNoteAChordMemberMakingThisNoteTheStartOfTheChord = nextNoteReader.getIsChord();
            }
            
            myCurrentCursor.isBackupInProgress = false;
            impl::NoteFunctions noteFunc;
            auto noteData = noteFunc.parseNote( inMxNote, myCurrentCursor );
            
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
                myCurrentCursor.position += noteData.durationTimeTicks;
            }
            
            auto& outMeasure = myStaves[myCurrentCursor.staffIndex];
            noteData.absoluteNoteIndex = myAbsoluteNoteIndex++;
            outMeasure.voices[myCurrentCursor.voiceIndex].notes.emplace_back( std::move( noteData ) );
        }
        
        
        void MeasureFunctions::parseBackup( const core::Backup& inMxBackup )
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
            myCurrentCursor.position -= backupAmount;
        }
        
        
        void MeasureFunctions::parseForward( const core::Forward& inMxForward )
        {
            impl::TimeFunctions timeFunc;
            const int forwardAmount = timeFunc.convertDurationToGlobalTickScale( myCurrentCursor, *inMxForward.getDuration() );
            myCurrentCursor.position += forwardAmount;
        }
        
        
        void MeasureFunctions::parseDirection( const core::Direction& inMxDirection )
        {
            coutItemNotSupported( inMxDirection );
        }
        
        
        void MeasureFunctions::parseProperties( const core::Properties& inMxProperties )
        {
            //coutItemNotSupported( inMxProperties );
            importClefs( inMxProperties.getClefSet() );
        }
        
        
        void MeasureFunctions::parseHarmony( const core::Harmony& inMxHarmony )
        {
            coutItemNotSupported( inMxHarmony );
        }
        
        
        void MeasureFunctions::parseFiguredBass( const core::FiguredBass& inMxFiguredBass )
        {
            coutItemNotSupported( inMxFiguredBass );
        }
        
        
        void MeasureFunctions::parsePrint( const core::Print& inMxPrint )
        {
            coutItemNotSupported( inMxPrint );
        }
        
        
        void MeasureFunctions::parseSound( const core::Sound& inMxSound )
        {
            coutItemNotSupported( inMxSound );
        }
        
        
        void MeasureFunctions::parseBarline( const core::Barline& inMxBarline )
        {
            coutItemNotSupported( inMxBarline );
        }
        
        
        void MeasureFunctions::parseGrouping( const core::Grouping& inMxGrouping )
        {
            coutItemNotSupported( inMxGrouping );
        }
        
        
        void MeasureFunctions::parseLink( const core::Link& inMxLink )
        {
            coutItemNotSupported( inMxLink );
        }
        
        
        void MeasureFunctions::parseBookmark( const core::Bookmark& inMxBookmark )
        {
            coutItemNotSupported( inMxBookmark );
        }
        
        
        void MeasureFunctions::coutItemNotSupported( const core::ElementInterface& element )
        {
            MX_UNUSED(element);
            //std::cout << element.getElementName() << " is not supported" << std::endl;
        }
        
        
        void MeasureFunctions::importClefs( const core::ClefSet& inClefs )
        {
            auto iter = inClefs.cbegin();
            auto endIter = inClefs.cend();
            
            for( ; iter != endIter; ++iter)
            {
                const auto& clef = **iter;
                importClef( clef );
            }
        }
        
        
        void MeasureFunctions::importClef( const core::Clef& inClef )
        {
            api::ClefData clefData;
            clefData.tickPosition = myCurrentCursor.position;
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
            
            if( myCurrentCursor.position == 0 )
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
            
            auto& measure = myStaves[clefData.staffIndex];
            measure.clefs.emplace_back( std::move( clefData ) );
        }
    }
}
