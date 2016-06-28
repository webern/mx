#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mx/core/DocumentPartwise.h"
#include "mx/core/Elements.h"
#include "mx/core/DocumentTimewise.h"
#include "mxtest/core/DocumentPartwiseCreate.h"
#include "mxtest/core/DocumentTimewiseCreate.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace mx::core;
using namespace std;

namespace MxTest
{
    inline MusicDataChoicePtr makeSixteenthNote( const StepEnum step, int octave, BeamValue beamval )
    {
        auto noteData = makeMusicDataChoice();
        noteData->setChoice( MusicDataChoice::Choice::note );
        noteData->getNote()->getNoteChoice()->setChoice( NoteChoice::Choice::normal );
        noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::pitch );
        noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->setValue( step );
        noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getOctave()->setValue( OctaveValue( octave ) );
        noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getDuration()->setValue( PositiveDivisionsValue( 1 ) );
        noteData->getNote()->getType()->setValue( NoteTypeValue::sixteenth );
        noteData->getNote()->setHasType( true );
        auto beam = makeBeam();
        beam->setValue( beamval );
        noteData->getNote()->addBeam( beam );
        noteData->getNote()->addBeam( beam );
        return noteData;
    }
    
    
    inline ClefPtr makeTrebleClef()
    {
        auto clef = makeClef();
        clef->getSign()->setValue( ClefSign::g );
        clef->setHasLine( true );
        clef->getLine()->setValue( StaffLine( 2 ) );
        return clef;
    }
    
    
    inline ClefPtr makeBassClef()
    {
        auto clef = makeClef();
        clef->getSign()->setValue( ClefSign::f );
        clef->setHasLine( true );
        clef->getLine()->setValue( StaffLine( 4 ) );
        return clef;
    }
    
    
    inline TimePtr makeFourFourTime()
    {
        auto time = makeTime();
        time->getTimeChoice()->setChoice( TimeChoice::Choice::timeSignature );
        time->getTimeChoice()->getTimeSignature()->getBeats()->setValue( XsString( "4" ) );
        time->getTimeChoice()->getTimeSignature()->getBeatType()->setValue( XsString( "4" ) );
        return time;
    }
    
    
    inline MusicDataChoicePtr makeMeasureProperties()
    {
        auto propertiesChoice = makeMusicDataChoice();
        propertiesChoice->setChoice( MusicDataChoice::Choice::properties );
        auto properties = propertiesChoice->getProperties();
        properties->setHasDivisions( true );
        properties->getDivisions()->setValue( PositiveDivisionsValue( 4 ) );
        properties->addKey( makeKey() );
        properties->addTime( makeFourFourTime() );
        properties->addClef( makeTrebleClef() );
        return propertiesChoice;
    }
    
    
    DocumentPartwisePtr makeDocStub()
    {
        auto doc = makeDocumentPartwise();
        auto s = doc->getScorePartwise();
        s->getAttributes()->hasVersion = true;
        s->getAttributes()->version = XsToken( "3.0" );
        auto header = s->getScoreHeaderGroup();
        header->getPartList()->getScorePart()->getAttributes()->id = XsID( "P1" );
        header->getPartList()->getScorePart()->getPartName()->setValue( XsString( "Music" ) );
        IdentificationPtr ident = makeIdentification();
        auto composer = makeCreator();
        composer->getAttributes()->hasType = true;
        composer->getAttributes()->type = XsToken( "composer" );
        composer->setValue( XsString( "Matthew James Briggs" ) );
        ident->addCreator( composer );
        header->setHasIdentification( true );
        header->setIdentification( ident );
        ident->getEncoding()->setChoice( Encoding::Choice::software );
        ident->getEncoding()->getSoftware()->setValue( XsString("MxSample" ) );
        ident->setHasEncoding( true );
        header->setHasWork( true );
        header->getWork()->setHasWorkTitle( true );
        header->getWork()->getWorkTitle()->setValue( XsString( "In the Beginning" ) );
        auto part = *( s->getPartwisePartSet().cbegin() );
        part->getAttributes()->id = XsIDREF( "P1" );
        auto measure = *( part->getPartwiseMeasureSet().cbegin() );
        measure->getAttributes()->number = XsToken( "1" );
        return doc;
    }
    
    
    inline std::string helloMusicXml()
    {
        auto doc = makeDocumentPartwise();
        auto s = doc->getScorePartwise();
        s->getAttributes()->hasVersion = true;
        s->getAttributes()->version = XsToken( "3.0" );
        auto header = s->getScoreHeaderGroup();
        header->getPartList()->getScorePart()->getAttributes()->id = XsID( "P1" );
        header->getPartList()->getScorePart()->getPartName()->setValue( XsString( "Music" ) );
        auto part = *( s->getPartwisePartSet().cbegin() );
        part->getAttributes()->id = XsIDREF( "P1" );
        auto measure = *( part->getPartwiseMeasureSet().cbegin() );
        measure->getAttributes()->number = XsToken( "1" );
        auto propertiesChoice = makeMusicDataChoice();
        propertiesChoice->setChoice( MusicDataChoice::Choice::properties );
        auto properties = propertiesChoice->getProperties();
        properties->setHasDivisions( true );
        properties->getDivisions()->setValue( PositiveDivisionsValue( 1 ) );
        properties->addKey( makeKey() );
        auto time = makeTime();
        time->getTimeChoice()->setChoice( TimeChoice::Choice::timeSignature );
        time->getTimeChoice()->getTimeSignature()->getBeats()->setValue( XsString( "4" ) );
        time->getTimeChoice()->getTimeSignature()->getBeatType()->setValue( XsString( "4" ) );
        properties->addTime( time );
        auto clef = makeClef();
        clef->getSign()->setValue( ClefSign::g );
        clef->setHasLine( true );
        clef->getLine()->setValue( StaffLine( 2 ) );
        properties->addClef( clef );
        measure->getMusicDataGroup()->addMusicDataChoice( propertiesChoice );
        auto noteData = makeMusicDataChoice();
        noteData->setChoice( MusicDataChoice::Choice::note );
        noteData->getNote()->getNoteChoice()->setChoice( NoteChoice::Choice::normal );
        noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::pitch );
        noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->setValue( StepEnum::c );
        noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getOctave()->setValue( OctaveValue( 4 ) );
        noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getDuration()->setValue( PositiveDivisionsValue( 4 ) );
        noteData->getNote()->getType()->setValue( NoteTypeValue::whole );
        measure->getMusicDataGroup()->addMusicDataChoice( noteData );
        auto twDoc = DocumentTimewise::convert( doc );
        auto pwDoc = DocumentPartwise::convert( twDoc );
        std::stringstream output;
        pwDoc->toStream( output );
        return output.str();
    }
    
}

#endif
