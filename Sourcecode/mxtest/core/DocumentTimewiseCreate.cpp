#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "DocumentPartwiseCreate.h"
#include "DocumentTimewiseCreate.h"

#include "mx/core/Strings.h"
#include "mx/core/Elements.h"
#include "mx/core/DocumentTimewise.h"
#include <sstream>

using namespace mx::core;

namespace MxTestHelpers
{
    mx::core::DocumentTimewisePtr createDocumentTimewise()
    {
        auto doc = makeDocumentTimewise();
        
        /* Set Version Attribute on Score Element */
        auto s = doc->getScoreTimewise();
        s->getAttributes()->hasVersion = true;
        s->getAttributes()->version = XsToken( "3.0" );
        
        /* Create Score Title and Credits */
        auto header = s->getScoreHeaderGroup();
        auto composerCredit = makeCredit();
        composerCredit->getCreditChoice()->setChoice( CreditChoice::Choice::creditWords );
        composerCredit->getCreditChoice()->getCreditWordsGroup()->getCreditWords()->setValue( XsString( "Matthew James Briggs" ) );
        auto creditType = makeCreditType();
        creditType->setValue( XsString( "composer" ) );
        composerCredit->addCreditType( creditType );
        header->addCredit( composerCredit );
        header->setHasWork( true );
        header->getWork()->setHasWorkTitle( true );
        header->getWork()->getWorkTitle()->setValue( XsString( "Simple Measures" ) );
        
        /* Create Score Header First Part */
        header->getPartList()->getScorePart()->getAttributes()->id = XsID( "PARTONE" );
        header->getPartList()->getScorePart()->getPartName()->setValue( XsString( "Part One" ) );
        
        /* Create Score Header Second Part */
        auto p2 = makeScorePart();
        p2->getAttributes()->id = XsID( "A2" );
        p2->getPartName()->setValue( XsString( "Part Two" ) );
        auto p2g = makePartGroupOrScorePart();
        p2g->setChoice( PartGroupOrScorePart::Choice::scorePart );
        p2g->setScorePart( p2 );
        header->getPartList()->addPartGroupOrScorePart( p2g );
        
        /* Create Score Header Third Part */
        auto p3 = makeScorePart();
        p3->getAttributes()->id = XsID( "P3" );
        p3->getPartName()->setValue( XsString( "Part Three" ) );
        auto p3g = makePartGroupOrScorePart();
        p3g->setChoice( PartGroupOrScorePart::Choice::scorePart );
        p3g->setScorePart( p3 );
        header->getPartList()->addPartGroupOrScorePart( p3g );
        
        /* Create Three Timewise Measure Stubs */
        // first measure stub already exists
        s->addTimewiseMeasure( makeTimewiseMeasure() ); // two
        s->addTimewiseMeasure( makeTimewiseMeasure() ); // three
        
        auto iter = s->getTimewiseMeasureSet().cbegin();
        auto measure1 = *iter;
        auto measure2 = *( ++iter );
        auto measure3 = *( ++iter );
        
        measure1->getAttributes()->number = XsToken( "1" );
        setTimewiseMeasureProperties( measure1, // measure pointer
                                  1,    // measure number
                                  1,    // divisions
                                  4,    // beats
                                  4 );  // beat type
        setTimewiseMeasureProperties( measure2, // measure pointer
                                  2,    // measure number
                                  1,    // divisions
                                  4,    // beats
                                  4 );  // beat type
        setTimewiseMeasureProperties( measure3, // measure pointer
                                  3,    // measure number
                                  1,    // divisions
                                  2,    // beats
                                  4 );  // beat type
        
        /* Add Three Parts to Measure 1 */
        auto m1p1 = *( measure1->getTimewisePartSet().cbegin() );
        m1p1->getAttributes()->id = XsIDREF( "PARTONE" );
        m1p1->setMusicDataGroup( getConstructedMusicDataGroup( "4" ) );
        addP1M1Data( m1p1->getMusicDataGroup() );
        
        auto m1p2 = makeTimewisePart();
        m1p2->getAttributes()->id = XsIDREF( "A2" );
        m1p2->setMusicDataGroup( getConstructedMusicDataGroup( "4" ) );
        addP2M1Data( m1p2->getMusicDataGroup() );
        measure1->addTimewisePart( m1p2 );
        
        auto m1p3 = makeTimewisePart();
        m1p3->getAttributes()->id = XsIDREF( "P3" );
        m1p3->setMusicDataGroup( getConstructedMusicDataGroup( "4" ) );
        addP3M1Data( m1p3->getMusicDataGroup() );
        measure1->addTimewisePart( m1p3 );
        
        /* Add Three Parts to Measure 2 */
        auto m2p1 = *( measure2->getTimewisePartSet().cbegin() );
        m2p1->getAttributes()->id = XsIDREF( "PARTONE" );
        m2p1->setMusicDataGroup( getConstructedMusicDataGroup( "4" ) );
        addP1M2Data( m2p1->getMusicDataGroup() );
        
        auto m2p2 = makeTimewisePart();
        m2p2->getAttributes()->id = XsIDREF( "A2" );
        m2p2->setMusicDataGroup( getConstructedMusicDataGroup( "4" ) );
        addP2M2Data( m2p2->getMusicDataGroup() );
        measure2->addTimewisePart( m2p2 );
        
        auto m2p3 = makeTimewisePart();
        m2p3->getAttributes()->id = XsIDREF( "P3" );
        m2p3->setMusicDataGroup( getConstructedMusicDataGroup( "4" ) );
        addP3M2Data( m2p3->getMusicDataGroup() );
        measure2->addTimewisePart( m2p3 );
        
        /* Add Three Parts to Measure 3 */
        auto m3p1 = *( measure3->getTimewisePartSet().cbegin() );
        m3p1->getAttributes()->id = XsIDREF( "PARTONE" );
        m3p1->setMusicDataGroup( getConstructedMusicDataGroup( "2" ) );
        addP1M3Data( m3p1->getMusicDataGroup() );
        
        auto m3p2 = makeTimewisePart();
        m3p2->getAttributes()->id = XsIDREF( "A2" );
        m3p2->setMusicDataGroup( getConstructedMusicDataGroup( "2" ) );
        addP2M3Data( m3p2->getMusicDataGroup() );
        measure3->addTimewisePart( m3p2 );
        
        auto m3p3 = makeTimewisePart();
        m3p3->getAttributes()->id = XsIDREF( "P3" );
        m3p3->setMusicDataGroup( getConstructedMusicDataGroup( "2" ) );
        addP3M3Data( m3p3->getMusicDataGroup() );
        measure3->addTimewisePart( m3p3 );
        
        return doc;
    }

    void setTimewiseMeasureProperties( TimewiseMeasurePtr& measure,
                                      int measureNumber,
                                      int divisions,
                                      int beats,
                                      int beatType )
    {
        std::stringstream measureNumberSstr;
        measureNumberSstr << measureNumber;
        std::stringstream beatSstr;
        beatSstr << beats;
        std::stringstream beatTypeSstr;
        beatTypeSstr << beatType;
        measure->getAttributes()->number = XsToken( measureNumberSstr.str() );
        
        
    }

    MusicDataGroupPtr getConstructedMusicDataGroup( const std::string& beats )
    {
        MusicDataGroupPtr output = makeMusicDataGroup();
        auto propertiesChoice = makeMusicDataChoice();
        propertiesChoice->setChoice( MusicDataChoice::Choice::properties );
        auto properties = propertiesChoice->getProperties();
        properties->setHasDivisions( true );
        properties->getDivisions()->setValue( PositiveDivisionsValue( 1 ) );
        properties->addKey( makeKey() );
        auto time = makeTime();
        time->getTimeChoice()->setChoice( TimeChoice::Choice::timeSignature );
        time->getTimeChoice()->getTimeSignature()->getBeats()->setValue( XsString( beats ) );
        time->getTimeChoice()->getTimeSignature()->getBeatType()->setValue( XsString( "4" ) );
        properties->addTime( time );
        auto clef = makeClef();
        clef->getSign()->setValue( ClefSign::g );
        clef->setHasLine( true );
        clef->getLine()->setValue( StaffLine( 2 ) );
        properties->addClef( clef );
        output->addMusicDataChoice( propertiesChoice );
        return output;
    }
    /*
    MusicDataChoicePtr makeNote( mx::t::StepEnum step,
                                       int octave,
                                       mx::t::NoteTypeValue duration,
                                       int divisions )
    {
        auto m1p1_noteData = makeMusicDataChoice();
        m1p1_noteData->setChoice( MusicDataChoice::Choice::note );
        m1p1_noteData->getNote()->getNoteChoice()->setChoice( NoteChoice::Choice::normal );
        m1p1_noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::pitch );
        m1p1_noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->setValue( step );
        m1p1_noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getOctave()->setValue( OctaveValue( octave ) );
        m1p1_noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getDuration()->setValue( PositiveDivisionsValue( divisions ) );
        m1p1_noteData->getNote()->getType()->setValue( duration );
        return m1p1_noteData;
    }
    
    void addP1M1Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto m1p1_noteData = makeNote( StepEnum::c,          // step
                                      4,                    // octave
                                      NoteTypeValue::whole, // duration
                                      4 );                  // divisions
        musicDataGroup->addMusicDataChoice( m1p1_noteData );
    }
    void addP1M2Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto m2p1_noteData = makeNote( StepEnum::d,          // step
                                      4,                    // octave
                                      NoteTypeValue::whole, // duration
                                      4 );                  // divisions
        musicDataGroup->addMusicDataChoice( m2p1_noteData );
    }
    void addP1M3Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto m3p1_noteData = makeNote( StepEnum::e,          // step
                                      4,                    // octave
                                      NoteTypeValue::half, // duration
                                      2 );                  // divisions
        musicDataGroup->addMusicDataChoice( m3p1_noteData );
    }
    void addP2M1Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto m1p2_noteData = makeNote( StepEnum::c,          // step
                                      5,                    // octave
                                      NoteTypeValue::quarter, // duration
                                      1 );                  // divisions
        musicDataGroup->addMusicDataChoice( m1p2_noteData );
        m1p2_noteData = makeNote( StepEnum::b,          // step
                                 4,                    // octave
                                 NoteTypeValue::quarter, // duration
                                 1 );                  // divisions
        musicDataGroup->addMusicDataChoice( m1p2_noteData );
        m1p2_noteData = makeNote( StepEnum::a,          // step
                                 4,                    // octave
                                 NoteTypeValue::quarter, // duration
                                 1 );                  // divisions
        musicDataGroup->addMusicDataChoice( m1p2_noteData );
        m1p2_noteData = makeNote( StepEnum::g,          // step
                                 4,                    // octave
                                 NoteTypeValue::quarter, // duration
                                 1 );                  // divisions
        musicDataGroup->addMusicDataChoice( m1p2_noteData );
    }
    void addP2M2Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto notedata = makeNote( StepEnum::a,          // step
                                 4,                    // octave
                                 NoteTypeValue::whole, // duration
                                 4 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
    }
    void addP2M3Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto notedata = makeNote( StepEnum::a,          // step
                                 4,                    // octave
                                 NoteTypeValue::quarter, // duration
                                 1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
        notedata = makeNote( StepEnum::f,          // step
                            4,                    // octave
                            NoteTypeValue::quarter, // duration
                            1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
    }
    void addP3M1Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto notedata = makeNote( StepEnum::a,          // step
                                 3,                    // octave
                                 NoteTypeValue::quarter, // duration
                                 1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
        notedata = makeNote( StepEnum::f,          // step
                            3,                    // octave
                            NoteTypeValue::quarter, // duration
                            1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
        notedata = makeNote( StepEnum::g,          // step
                            3,                    // octave
                            NoteTypeValue::quarter, // duration
                            1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
        notedata = makeNote( StepEnum::a,          // step
                            3,                    // octave
                            NoteTypeValue::quarter, // duration
                            1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
    }
    void addP3M2Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto notedata = makeNote( StepEnum::b,          // step
                                 3,                    // octave
                                 NoteTypeValue::quarter, // duration
                                 1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
        notedata = makeNote( StepEnum::c,          // step
                            4,                    // octave
                            NoteTypeValue::quarter, // duration
                            1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
        notedata = makeNote( StepEnum::b,          // step
                            3,                    // octave
                            NoteTypeValue::quarter, // duration
                            1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
        notedata = makeNote( StepEnum::a,          // step
                            3,                    // octave
                            NoteTypeValue::quarter, // duration
                            1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
    }
    void addP3M3Data( const MusicDataGroupPtr& musicDataGroup )
    {
        auto notedata = makeNote( StepEnum::g,          // step
                                 3,                    // octave
                                 NoteTypeValue::quarter, // duration
                                 1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
        notedata = makeNote( StepEnum::f,          // step
                            3,                    // octave
                            NoteTypeValue::quarter, // duration
                            1 );                  // divisions
        musicDataGroup->addMusicDataChoice( notedata );
    }
     */
}

#endif
