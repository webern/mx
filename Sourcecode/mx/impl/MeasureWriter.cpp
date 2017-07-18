// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/MeasureWriter.h"
#include "mx/core/elements/Accidental.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Backup.h"
#include "mx/core/elements/Barline.h"
#include "mx/core/elements/BarStyle.h"
#include "mx/core/elements/Beam.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Cancel.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/ClefOctaveChange.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Cue.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Directive.h"
#include "mx/core/elements/Divisions.h"
#include "mx/core/elements/Dot.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Ending.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/Fermata.h"
#include "mx/core/elements/Fifths.h"
<<<<<<< 753d3ba8d14c9daba17e9543f6cbc4e3573ea2ae
<<<<<<< caeae2ca63c6721746e558cf2db28844937863ca
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Footnote.h"
=======
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Footnote.h"
>>>>>>> release v0.3.0
=======
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Footnote.h"
>>>>>>> release v0.3.0
#include "mx/core/elements/Forward.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/Grace.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Instrument.h"
#include "mx/core/elements/Instruments.h"
#include "mx/core/elements/Key.h"
#include "mx/core/elements/KeyChoice.h"
#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/elements/LeftMargin.h"
<<<<<<< 753d3ba8d14c9daba17e9543f6cbc4e3573ea2ae
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Level.h"
=======
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Level.h"
>>>>>>> release v0.3.0
#include "mx/core/elements/Line.h"
#include "mx/core/elements/Lyric.h"
#include "mx/core/elements/MeasureLayout.h"
#include "mx/core/elements/MeasureNumbering.h"
#include "mx/core/elements/MeasureStyle.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/Mode.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Notehead.h"
#include "mx/core/elements/NoteheadText.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/elements/PartSymbol.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Play.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Print.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Repeat.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/Sign.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/StaffDetails.h"
#include "mx/core/elements/Staves.h"
#include "mx/core/elements/Stem.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/SystemDistance.h"
#include "mx/core/elements/SystemDividers.h"
#include "mx/core/elements/SystemLayout.h"
#include "mx/core/elements/SystemMargins.h"
#include "mx/core/elements/Tie.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include "mx/core/elements/TopSystemDistance.h"
#include "mx/core/elements/TraditionalKey.h"
#include "mx/core/elements/Transpose.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/WavyLine.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"
#include "mx/impl/Converter.h"
#include "mx/impl/DirectionWriter.h"
#include "mx/impl/NoteWriter.h"
#include "mx/impl/ScoreWriter.h"

#include <limits>

namespace mx
{
    namespace core
    {
        class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
    }

    namespace impl
    {
        MeasureWriter::MeasureWriter( const api::MeasureData& inMeasureData, const MeasureCursor& inCursor, const ScoreWriter& inScoreWriter )
        : myMeasureData{ inMeasureData }
        , myOutMeasure{ nullptr }
        , myPreviousCursor{ inCursor }
        , myScoreWriter{ inScoreWriter }
        , myPropertiesWriter{ nullptr }
        , myConverter{}
        , myBarlinesIter{ inMeasureData.barlines.cbegin() }
        , myBarlinesEnd{ inMeasureData.barlines.cend() }
        , myMeasureKeysIter{ inMeasureData.keys.cbegin() }
        , myMeasureKeysEnd{ inMeasureData.keys.cend() }
        , myHistory{ inCursor }
        {

        }


        core::PartwiseMeasurePtr MeasureWriter::getPartwiseMeasure()
        {
            myOutMeasure = core::makePartwiseMeasure();
            myPropertiesWriter = std::unique_ptr<PropertiesWriter>{ new PropertiesWriter{ myOutMeasure } };
            auto cursor = myHistory.getCursor();
            cursor.reset();
            myHistory = History{ cursor };
            myBarlinesIter = myMeasureData.barlines.cbegin();
            myBarlinesEnd = myMeasureData.barlines.cend();
            myMeasureKeysIter = myMeasureData.keys.cbegin();
            
            writeMeasureGlobals();
            writeStaves();
            myPropertiesWriter->flushBuffer();
            writeBarlines( api::TICK_TIME_INFINITY );
            myPropertiesWriter = nullptr;
            return myOutMeasure;
        }
        
        void MeasureWriter::writeMeasureGlobals()
        {
            writeBarlines( 0 );
            auto& measureAttr = *myOutMeasure->getAttributes();
            
            if( myMeasureData.number.size() > 0 )
            {
                measureAttr.number = core::XsToken{ myMeasureData.number };
            }
            else
            {
                measureAttr.number = core::XsToken{ std::to_string( myHistory.getCursor().measureIndex + 1 ) };
            }
            
            if( myMeasureData.width >= 0.0 )
            {
                measureAttr.hasWidth = true;
                measureAttr.width = core::PositiveDivisionsValue{ myMeasureData.width };
            }
            
            Converter converter;
            
            if( myMeasureData.implicit != api::Bool::unspecified )
            {
                measureAttr.hasImplicit = true;
                measureAttr.implicit = converter.convert( myMeasureData.implicit );
            }
            
            if( myScoreWriter.isStartOfSystem( myHistory.getCursor().measureIndex ) )
            {
                writeSystemInfo();
            }
            
            if( myHistory.getCursor().isFirstMeasureInPart )
            {
                myPropertiesWriter->writeDivisions( myHistory.getCursor().getGlobalTicksPerQuarter() );
                
                if( myMeasureData.staves.size() > 1 )
                {
                    myPropertiesWriter->writeNumStaves( static_cast<int>( myMeasureData.staves.size() ) );
                }
            }
            
            if( !myMeasureData.timeSignature.isImplicit )
            {
                myPropertiesWriter->writeTime( myMeasureData.timeSignature );
            }
            
            int localStaffCounter = 0;
            for( const auto& staff : myMeasureData.staves )
            {
                auto clefIter = staff.clefs.cbegin();
                auto clefEnd = staff.clefs.cend();
                while( clefIter != clefEnd && clefIter->tickTimePosition == 0 )
                {
                    int desiredStaffIndex = -1;
                    if( myHistory.getCursor().getNumStaves() > 1 )
                    {
                        desiredStaffIndex = localStaffCounter;
                    }
                    myPropertiesWriter->writeClef( desiredStaffIndex, *clefIter );
                    ++clefIter;
                }
                ++localStaffCounter;
            }
            
            // TODO - these might end up in the wrong place, figure out how
            // to preserve location if the barline is not at the beginning or
            // the end
            
            while( myMeasureKeysIter != myMeasureKeysEnd && myMeasureKeysIter->tickTimePosition == 0 )
            {
                myPropertiesWriter->writeKey( -1, *myMeasureKeysIter );
                ++myMeasureKeysIter;
            }
        }


        void MeasureWriter::writeSystemInfo()
        {
            auto systemData = myScoreWriter.getSystemData( myHistory.getCursor().measureIndex );
            
            const bool isSystemDataSpecified =
                   systemData.isMarginSpecified
                || systemData.isSystemDistanceSpecified
                || systemData.isTopSystemDistanceSpecified;
            
            if( !isSystemDataSpecified && !myHistory.getCursor().isFirstMeasureInPart )
            {
                return;
            }
            
            auto printMdc = core::makeMusicDataChoice();
            printMdc->setChoice( core::MusicDataChoice::Choice::print );
            auto& print = *printMdc->getPrint();
            print.getAttributes()->hasNewSystem = true;
            print.getAttributes()->newSystem = core::YesNo::yes;
            auto& layoutGroup = *print.getLayoutGroup();
            myOutMeasure->getMusicDataGroup()->addMusicDataChoice( printMdc );
            myHistory.log( "writePrint" );

            if( isSystemDataSpecified )
            {
                layoutGroup.setHasSystemLayout( true );
                auto& systemLayout = *layoutGroup.getSystemLayout();

                if( systemData.isMarginSpecified )
                {
                    systemLayout.setHasSystemMargins( true );
                    auto& margins = *systemLayout.getSystemMargins();
                    margins.getLeftMargin()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.leftMargin ) } );
                    margins.getRightMargin()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.rightMargin ) } );
                }

                if( systemData.isTopSystemDistanceSpecified )
                {
                    systemLayout.setHasTopSystemDistance( true );
                    systemLayout.getTopSystemDistance()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.topSystemDistance ) } );
                }

                if( systemData.isSystemDistanceSpecified )
                {
                    systemLayout.setHasSystemDistance( true );
                    systemLayout.getSystemDistance()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.systemDistance ) } );
                }
           }
        }
        
        
        void MeasureWriter::writeStaves()
        {
            
            myHistory.resetCursorFofStaffIterations();
            myPreviousCursor = myHistory.getCursor();
            for( const auto& staff : myMeasureData.staves )
            {
                writeVoices( staff );
                myHistory.nextStaff();
            }
        }
        
        
        void MeasureWriter::writeVoices( const api::StaffData& inStaff )
        {
            auto clefIter = inStaff.clefs.cbegin();
            const auto clefEnd = inStaff.clefs.cend();
            while( clefIter != clefEnd && clefIter->tickTimePosition == 0 ) { ++clefIter; }
            auto staffKeyIter = inStaff.keys.cbegin();
            auto staffKeyEnd = inStaff.keys.cend();
            auto directionIter = inStaff.directions.cbegin();
            auto directionEnd = inStaff.directions.cend();

            for( const auto& voice : inStaff.voices )
            {
                bool isFirstNote = true;
                int currentChordTickPosition = -1;
                int previousChordTickPosition = -2;
                myHistory.setVoiceIndex( voice.first );
                auto noteIter = voice.second.notes.cbegin();
                auto noteEnd = voice.second.notes.cend();

                if( directionIter != directionEnd )
                {
                    writeDirections( directionIter, directionEnd, noteIter, std::cbegin(voice.second.notes), noteEnd );
                }

                mx::api::NoteData noteForOutsideOfLoop;

                for( ; noteIter != noteEnd; ++noteIter )
                {
                    bool isStartOfChord = false;
                    myHistory.setChord( noteIter->isChord );
                    const auto& apiNote = *noteIter;
                    writeForwardOrBackupIfNeeded( apiNote );
                    
                    if ( noteIter->isChord )
                    {
                        currentChordTickPosition = myHistory.getCursor().tickTimePosition;
                        if( isFirstNote )
                        {
                            isStartOfChord = true;
                        }
                        else if( currentChordTickPosition != previousChordTickPosition )
                        {
                            isStartOfChord = true;
                        }
                        previousChordTickPosition = currentChordTickPosition;
                    }

                    if( myMeasureKeysIter != myMeasureKeysEnd )
                    {
                        if( myMeasureKeysIter->tickTimePosition <= myHistory.getCursor().tickTimePosition )
                        {
                            myPropertiesWriter->writeKey( -1, *myMeasureKeysIter );
                            ++myMeasureKeysIter;
                        }
                    }
                    
                    while( clefIter != clefEnd && clefIter->tickTimePosition <= myHistory.getCursor().tickTimePosition )
                    {
                        myPropertiesWriter->writeClef( myHistory.getCursor().staffIndex, *clefIter );
                        ++clefIter;
                    }
                    
                    for( ; staffKeyIter != staffKeyEnd && staffKeyIter->tickTimePosition <= myHistory.getCursor().tickTimePosition; ++staffKeyIter )
                    {
                        myPropertiesWriter->writeKey( myHistory.getCursor().staffIndex, *staffKeyIter );
                    }

                    myPropertiesWriter->flushBuffer();

                    int minDirectionTime = myHistory.getCursor().tickTimePosition;
                    int maxDirectionTime = minDirectionTime + noteIter->durationData.durationTimeTicks;
                    auto nextNote = noteIter + 1;

                    if( nextNote != noteEnd )
                    {
                        maxDirectionTime = nextNote->tickTimePosition;
                    }

                    writeDirections( directionIter, directionEnd, noteIter, std::cbegin(voice.second.notes), noteEnd );

                    auto mdc = core::makeMusicDataChoice();
                    mdc->setChoice( core::MusicDataChoice::Choice::note );
                    NoteWriter writer{ apiNote, myHistory.getCursor(), myScoreWriter, myPreviousCursor.isChordActive };
                    mdc->setNote( writer.getNote(isStartOfChord) );
                    myOutMeasure->getMusicDataGroup()->addMusicDataChoice( mdc );
                    myHistory.log( "addNote cursorTime " + std::to_string( myHistory.getCursor().tickTimePosition ) + ", noteTime " + std::to_string( apiNote.tickTimePosition ) );
                    advanceCursorIfNeeded( apiNote, noteIter, noteEnd );
                    myPreviousCursor = myHistory.getCursor();
                    isFirstNote = false;

                } // foreach note

                writeDirections( directionIter, directionEnd, std::cend(voice.second.notes), std::cbegin(voice.second.notes), std::cend(voice.second.notes) );

            } // foreach voice

            std::vector<api::NoteData> fakeEmptyNoteVector;
            writeDirections( directionIter, directionEnd, std::cend(fakeEmptyNoteVector), std::cbegin(fakeEmptyNoteVector), std::cend(fakeEmptyNoteVector) );
            
            bool areClefsRemaining = clefIter != clefEnd;
            bool areMeasureKeysRemaining = myMeasureKeysIter != myMeasureKeysEnd;
            bool areStaffKeysRemaining = staffKeyIter != staffKeyEnd;
            
            if( areClefsRemaining || areMeasureKeysRemaining || areStaffKeysRemaining )
            {
                for( ; clefIter != inStaff.clefs.cend(); ++clefIter )
                {
                    MX_ASSERT( clefIter != inStaff.clefs.cend() );
                    api::ClefData clef = *clefIter;
                    myPropertiesWriter->writeClef( myHistory.getCursor().staffIndex, clef );
                }
                
                if( myMeasureKeysIter != myMeasureKeysEnd )
                {
                    if( myMeasureKeysIter->tickTimePosition )
                    {
                        myPropertiesWriter->writeKey( -1, *myMeasureKeysIter );
                        ++myMeasureKeysIter;
                    }
                }
                
                for( ; staffKeyIter != staffKeyEnd; ++staffKeyIter )
                {
                    myPropertiesWriter->writeKey( myHistory.getCursor().staffIndex, *staffKeyIter );
                }
                myPropertiesWriter->flushBuffer();
            }

        } // func writeVoices
        
        
        void MeasureWriter::writeForwardOrBackupIfNeeded( const api::NoteData& currentNote )
        {
            myPropertiesWriter->flushBuffer();

            if( currentNote.tickTimePosition < 0 )
            {
                return;
            }

            const int timeDifference = currentNote.tickTimePosition - myHistory.getCursor().tickTimePosition;
            if( timeDifference < 0 )
            {
                backup( -1 * timeDifference );
            }
            else if ( timeDifference > 0 )
            {
                forward( timeDifference );
            }
        }
        
        
        void MeasureWriter::backup( const int inTicks )
        {
            // ensure we don't go below zero

            const auto currentTime = myHistory.getCursor().tickTimePosition;
            MX_ASSERT( currentTime >= 0 );
            const auto proposedNewTime = currentTime - inTicks;
            int safeTickAmount = inTicks;

            if( proposedNewTime <  0 )
            {
                safeTickAmount += proposedNewTime;
            }

            MX_ASSERT( safeTickAmount >= 0 );

            if( safeTickAmount == 0 )
            {
                return;
            }

            auto backupMdc = core::makeMusicDataChoice();
            backupMdc->setChoice( core::MusicDataChoice::Choice::backup );
            auto backup = backupMdc->getBackup();
            backup->getDuration()->setValue( core::PositiveDivisionsValue{ static_cast<core::DecimalType>( safeTickAmount ) } );
            myOutMeasure->getMusicDataGroup()->addMusicDataChoice( backupMdc );
            myHistory.advanceTickTimePosition( -1 * safeTickAmount, "write backup" );
        }
        
        
        void MeasureWriter::forward( const int ticks )
        {
            auto forwardMdc = core::makeMusicDataChoice();
            forwardMdc->setChoice( core::MusicDataChoice::Choice::forward );
            auto forward = forwardMdc->getForward();
            forward->getDuration()->setValue( core::PositiveDivisionsValue{ static_cast<core::DecimalType>( ticks ) } );
            myOutMeasure->getMusicDataGroup()->addMusicDataChoice( forwardMdc );
            myHistory.advanceTickTimePosition( ticks, "write forward" );
        }


        bool MeasureWriter::isAdvanceNeeded( const api::NoteData& currentNote, const NoteIter& inIter, const NoteIter& inEnd )
        {
            if( currentNote.isChord )
            {
                auto nextNoteIter = inIter + 1;
                if( nextNoteIter == inEnd )
                {
                    return true;
                }
                else
                {
                    const auto nextNoteTime = nextNoteIter->tickTimePosition;
                    if( nextNoteTime != currentNote.tickTimePosition )
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        
        void MeasureWriter::advanceCursorIfNeeded( const api::NoteData& currentNote, const NoteIter& inIter, const NoteIter& inEnd )
        {
            if (isAdvanceNeeded( currentNote, inIter, inEnd ) )
            {
                const auto curTime = std::max( myHistory.getCursor().tickTimePosition, 0 );
                const auto duration = std::max( currentNote.durationData.durationTimeTicks, 0 );
                const auto newTime = curTime + duration;
                myHistory.setTime( newTime, "advance cursor" );
                myPreviousCursor = myHistory.getCursor();
            }
            else
            {
                myHistory.log( "no advance");
            }

        }


        void MeasureWriter::writeDirections( dIter& directionIter, const dIter& directionEnd, const NoteIter& inNoteIter, const NoteIter& inNotesBegin, const NoteIter& inNotesEnd )
        {
            if( directionIter == directionEnd )
            {
                return;
            }

            const bool isNotesEnd = inNoteIter == inNotesEnd;
            const bool isLastNote = !isNotesEnd && ( inNoteIter + 1 == inNotesEnd );
            const bool isFirstNote = !isLastNote && !isNotesEnd && ( inNoteIter == inNotesBegin );
//            const auto previousNote = isFirstNote ? inNoteEnd : ( inNoteIter - 1 );
            const auto nextNote = ( isLastNote || isNotesEnd ) ? inNotesEnd : ( inNoteIter + 1 );

            if( isFirstNote )
            {
                for ( ; directionIter != directionEnd && directionIter->tickTimePosition <= std::max( 0, inNoteIter->tickTimePosition );
                     ++directionIter )
                {
                    writeDirection( *directionIter );
                }

                return;
            }
            else if ( isNotesEnd )
            {
                for ( ; directionIter != directionEnd; ++directionIter )
                {
                    writeDirection( *directionIter );
                }

                return;
            }
            else
            {
                const bool isLastNoteOfChord = !isNotesEnd && !isLastNote && ( inNoteIter->isChord && inNoteIter->tickTimePosition != nextNote->tickTimePosition );
                const bool isChord = !isNotesEnd && inNoteIter->isChord;

                if (isChord && !isLastNoteOfChord)
                {
                    myHistory.log( "note writing any directions because we are in the midst of a chord" );
                    return;
                }

                const int currentNoteEndTime = myHistory.getCursor().tickTimePosition + std::max(0, inNoteIter->durationData.durationTimeTicks);

                for ( ; directionIter != directionEnd && directionIter->tickTimePosition < currentNoteEndTime;
                     ++directionIter )
                {
                    writeDirection( *directionIter );
                }

                return;
            }
        }

        
        void MeasureWriter::writeDirection( const api::DirectionData& inDirectionData )
        {
            if( myPropertiesWriter )
            {
                myPropertiesWriter->flushBuffer();
            }

            //auto directionMdc = core::makeMusicDataChoice();
            //directionMdc->setChoice( core::MusicDataChoice::Choice::direction );
            DirectionWriter directionWriter{ inDirectionData, myHistory.getCursor() };
            auto mdcSet = directionWriter.getDirectionLikeThings();
            for( const auto& mdc : mdcSet )
            {
                myOutMeasure->getMusicDataGroup()->addMusicDataChoice( mdc );
            }
            myHistory.log( "addDirection cursorTime " + std::to_string( myHistory.getCursor().tickTimePosition ) + ", directionTime " + std::to_string( inDirectionData.tickTimePosition ) );
        }


        void MeasureWriter::writeBarlines( int tickTimePosition )
        {
            for( ; myBarlinesIter != myBarlinesEnd && myBarlinesIter->tickTimePosition <= tickTimePosition; ++myBarlinesIter )
            {
                auto mdc = core::makeMusicDataChoice();
                mdc->setChoice( core::MusicDataChoice::Choice::barline );
                auto barlineElement = mdc->getBarline();

                if( myBarlinesIter->barlineType != api::BarlineType::unspecified && myBarlinesIter->barlineType != api::BarlineType::unsupported )
                {
                    barlineElement->setHasBarStyle( true );
                    barlineElement->getBarStyle()->setValue( myConverter.convert( myBarlinesIter->barlineType ) );
                }
<<<<<<< 082227f5ac3f2004cd7884731308aef6d954274d

=======
>>>>>>> Add support for reading and writing of endings
                if( myBarlinesIter->endingType != api::EndingType::none )
                {
                    barlineElement->setHasEnding( true );
                    auto ending = barlineElement->getEnding();
                    ending->getAttributes()->type = myConverter.convert( myBarlinesIter->endingType );

                    if( myBarlinesIter->endingNumber > 0 )
                    {
                        ending->getAttributes()->number = std::to_string( myBarlinesIter->endingNumber );
                    }
                }
<<<<<<< 082227f5ac3f2004cd7884731308aef6d954274d

=======
>>>>>>> Add support for reading and writing of endings
                if( myBarlinesIter->location != api::HorizontalAlignment::unspecified )
                {
                    barlineElement->getAttributes()->hasLocation = true;
                    barlineElement->getAttributes()->location = myConverter.convertBarlinePlacement( myBarlinesIter->location );
                }
<<<<<<< 082227f5ac3f2004cd7884731308aef6d954274d
                else if ( myBarlinesIter->location == api::HorizontalAlignment::left || myBarlinesIter->tickTimePosition == 0 )
                {
                    barlineElement->getAttributes()->hasLocation = true;
                    barlineElement->getAttributes()->location = myConverter.convertBarlinePlacement( mx::api::HorizontalAlignment::left );
                }
                else
                {
                    barlineElement->getAttributes()->hasLocation = true;
                    barlineElement->getAttributes()->location = myConverter.convertBarlinePlacement( mx::api::HorizontalAlignment::right );
                }

                if ( myBarlinesIter->repeat )
                {
                    const auto repeatElement = barlineElement->getRepeat();
                    barlineElement->setHasRepeat(true);

                    if ( myBarlinesIter->location == api::HorizontalAlignment::left || myBarlinesIter->tickTimePosition == 0 )
                    {
                        repeatElement->getAttributes()->direction = mx::core::BackwardForward::forward;
                    }
                    else
                    {
                        repeatElement->getAttributes()->direction = mx::core::BackwardForward::backward;
                    }
                }
=======
>>>>>>> Add support for reading and writing of endings

                myOutMeasure->getMusicDataGroup()->addMusicDataChoice( mdc );
                myHistory.log( "writeBarline" );
            }
        }

        
        void MeasureWriter::History::advanceTickTimePosition( int amount, std::string reason )
        {
            HistoryRecord record;
            record.reason = std::move( reason );
            record.amount = amount;
            record.timeBefore = myCursor.tickTimePosition;
            record.cursorBefore = myCursor;
            myCursor.tickTimePosition += amount;
            record.cursorAfter = myCursor;
            record.timeAfter = myCursor.tickTimePosition;
            historyPush( record );
        }
    }
}
