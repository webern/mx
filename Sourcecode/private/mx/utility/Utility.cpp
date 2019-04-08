// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/utility/Utility.h"
#include "mx/utility/UtilityImpl.h"
#include <string>
#include <sstream>

using namespace mx::core;

namespace mx
{
	namespace utility
	{
        void setTitle(
            const MxDoc& doc,
            const StringType& title )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            header->setHasWork( true );
            header->getWork()->setHasWorkTitle( true );
            header->getWork()->getWorkTitle()->setValue( XsString( title ) );
        }
        
        
        StringType getTitle( const MxDoc& doc )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            if( !header->getHasWork() || !header->getWork()->getHasWorkTitle() )
            {
                return "";
            }
            return header->getWork()->getWorkTitle()->getValue().getValue();
        }
        
        
        void setMovementTitle(
            const MxDoc& doc,
            const StringType& title )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            header->setHasMovementTitle( true );
            header->getMovementTitle()->setValue( XsString( title ) );
        }
        
        
        StringType getMovementTitle( const MxDoc& doc )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            if( !header->getHasMovementTitle() )
            {
                return "";
            }
            return header->getMovementTitle()->getValue().getValue();
        }
        
        
        void setMovementNumber(
            const MxDoc& doc,
            const int value )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            header->setHasMovementNumber( true );
            header->getMovementNumber()->setValue( XsString( std::to_string( value ) ) );
        }
        
        
        int getMovementNumber( const MxDoc& doc )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            if( !header->getHasMovementNumber() )
            {
                return 0;
            }
            std::stringstream ss( header->getMovementNumber()->getValue().getValue() );
            int number = 0;
            ss >> number;
            return number;
        }
        
        
        void setComposer(
            const MxDoc& doc,
            const StringType& value )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            auto composerCreator = makeCreator();
            composerCreator->getAttributes()->hasType = true;
            composerCreator->getAttributes()->type = XsToken( "composer" );
            composerCreator->setValue( XsString( value ) );
            header->setHasIdentification( true );
            header->getIdentification()->addCreator( composerCreator );
        }
        
        
        StringType getComposer( const MxDoc& doc )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            auto creatorSet = header->getIdentification()->getCreatorSet();
            for( auto it = creatorSet.cbegin(); it != creatorSet.cend(); ++it )
            {
                if ( (*it)->getAttributes()->type.getValue() == "composer" )
                {
                    return (*it)->getValue().getValue();
                }
            }
            return "";
        }
        
        
        void setCopyright(
            const MxDoc& doc,
            const StringType& value )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            auto valueObject = makeRights();
            valueObject->getAttributes()->hasType = true;
            valueObject->getAttributes()->type = XsToken( "copyright" );
            valueObject->setValue( XsString( value ) );
            header->setHasIdentification( true );
            header->getIdentification()->addRights( valueObject );
        }
        
        
        StringType getCopyright( const MxDoc& doc )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            auto valueSet = header->getIdentification()->getRightsSet();
            for( auto it = valueSet.cbegin(); it != valueSet.cend(); ++it )
            {
                if ( (*it)->getAttributes()->type.getValue() == "copyright" )
                {
                    return (*it)->getValue().getValue();
                }
            }
            return "";
        }
        
        
        void setSoftware(
            const MxDoc& doc,
            const StringType& value )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            header->setHasIdentification( true );
            header->getIdentification()->setHasEncoding( true );
            auto encodingChoice = makeEncodingChoice();
            encodingChoice->setChoice( EncodingChoice::Choice::software );
            encodingChoice->getSoftware()->setValue( XsString( value ) );
            header->getIdentification()->getEncoding()->addEncodingChoice( encodingChoice );
        }
        
        
        StringType getSoftware( const MxDoc& doc )
        {
            auto header = doc->getScorePartwise()->getScoreHeaderGroup();
            if( !header->getHasIdentification() || !header->getIdentification()->getHasEncoding() )
            {
                return "";
            }
            auto encoding = header->getIdentification()->getEncoding();
            for( auto e : encoding->getEncodingChoiceSet() )
            {
                if( e->getChoice() == EncodingChoice::Choice::software )
                {
                    return e->getSoftware()->getValue().getValue();
                }
            }
            return "";
        }
        
        void startPartGroup(
            const MxDoc& doc,
            const PartGroupParams& params )
        {
            auto headerPartList = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList();
            auto pg = makePartGroupStartTag( params );
            
            if( isPartListInitialized( doc ) )
            {
                auto pgosp = makePartGroupOrScorePart();
                pgosp->setChoice( PartGroupOrScorePart::Choice::partGroup );
                pgosp->setPartGroup( pg );
                doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->addPartGroupOrScorePart( pgosp );
            }
            else
            {
                headerPartList->addPartGroup( pg );
            }
        }
        
        
        void stopPartGroup(
            const MxDoc& doc,
            int number )
        {
            auto partHeaderList = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList();
            auto pg = makePartGroup();
            pg->getAttributes()->type = StartStop::stop;
            
            if( number >= 0 )
            {
                pg->getAttributes()->hasNumber = true;
                pg->getAttributes()->number = XsToken( std::to_string( number ) );
            }
            
            auto pgosp = makePartGroupOrScorePart();
            pgosp->setChoice( PartGroupOrScorePart::Choice::partGroup );
            pgosp->setPartGroup( pg );
            doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->addPartGroupOrScorePart( pgosp );
        }
        
        
        void stopPartGroup( const MxDoc& doc )
        {
            stopPartGroup( doc, -1 );
        }
        
        
        PartwisePartPtr addPart( const MxDoc& doc, const PartParams& params )
        {
            if( ! isPartListInitialized( doc ) )
            {
                return addInitialPartToDocument( doc, params );
            }
            else
            {
                return addSubsequentPartToDocument( doc, params );
            }
        }
        
        
        PartwiseMeasurePtr createMeasure( int measureNumber )
        {
            auto measure = makePartwiseMeasure();
            measure->getAttributes()->number = std::to_string( measureNumber );
            auto musicDataGroup = measure->getMusicDataGroup();
            return measure;
        }
        
        
        void addTimeSignatureGroup( const PartwiseMeasurePtr& measure, int top, int bottom )
        {
            auto measureProperties = createOrRetrieveMeasureProperties( measure );
            auto time = makeTime();
            auto timeChoice = time->getTimeChoice();
            timeChoice->setChoice( TimeChoice::Choice::timeSignature );
            auto timeSignature = makeTimeSignatureGroup();
            timeSignature->getBeats()->setValue( XsString( std::to_string( top ) ) );
            timeSignature->getBeatType()->setValue( XsString( std::to_string( bottom ) ) );
            timeChoice->addTimeSignatureGroup( timeSignature );
            timeChoice->removeTimeSignatureGroup( timeChoice->getTimeSignatureGroupSet().cbegin() );
            measureProperties->getProperties()->addTime( time );
        }
        
        
        void addStaveCount( const PartwiseMeasurePtr& measure, int staveCount )
        {
            auto measureProperties = createOrRetrieveMeasureProperties( measure );
            measureProperties->getProperties()->setHasStaves( true );
            measureProperties->getProperties()->getStaves()->setValue( NonNegativeInteger( staveCount ) );
        }
        
        
        void addDivisions( const PartwiseMeasurePtr& measure, int divisions )
        {
            auto measureProperties = createOrRetrieveMeasureProperties( measure );
            measureProperties->getProperties()->setHasDivisions( true );
            measureProperties->getProperties()->getDivisions()->setValue( PositiveDivisionsValue( divisions ) );
        }
        
        
        int getDivisions( const PartwiseMeasurePtr& measure )
        {
            auto collection = measure->getMusicDataGroup()->getMusicDataChoiceSet();
            auto iter = collection.cbegin();
            auto endIter = collection.cend();
            for ( ; iter != endIter; ++iter )
            {
                auto choice = (*iter)->getChoice();
                if ( choice == MusicDataChoice::Choice::properties )
                {
                    auto props = (*iter)->getProperties();
                    if( props->getHasDivisions() )
                    {
                        return static_cast<int>( props->getDivisions()->getValue().getValue() );
                    }
                }
            }
            return -1;
        }
        
        
        void addKey( const PartwiseMeasurePtr& measure )
        {
            auto mdg = measure->getMusicDataGroup();
            auto mdc = makeMusicDataChoice();
            mdc->setChoice( MusicDataChoice::Choice::properties );
            mdg->addMusicDataChoice( mdc );
        }
        
        
        MusicDataChoicePtr createNote( const NoteParams& params )
        {
            auto musicDataChoice = createEmptyNote( params.noteChoice );
            setNoteDurationType( musicDataChoice, params.durationType );
            setNoteDurationInt( musicDataChoice, params.duration );
            setNoteDurationDots( musicDataChoice, params.durationDots );
            setNoteBeams( musicDataChoice, params.beams );
            setNoteStaffNumber( musicDataChoice, params.staffNumber );
            setNoteVoiceNumber( musicDataChoice, params.voiceNumber );
            if( params.isRest )
            {
                setNoteIsRest( musicDataChoice );
                return musicDataChoice;
            }
            if( params.isChord )
            {
                setNoteIsChord( musicDataChoice );
            }
            setNoteStep( musicDataChoice, params.step );
            setNoteAlter( musicDataChoice, params.alter );
            setNoteOctave( musicDataChoice, params.octave );
            if ( params.stemDirection != UpDown::unspecified )
            {
                setStemDirection( musicDataChoice, params.stemDirection );
            }
            if( params.showAccidental )
            {
                setNoteAccidental( musicDataChoice, params.alter );
            }
            return musicDataChoice;
        }
        
    } // namespace utility

} // namespace mx
