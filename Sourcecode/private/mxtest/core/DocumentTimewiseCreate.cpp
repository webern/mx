// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "DocumentPartwiseCreate.h"
#include "DocumentTimewiseCreate.h"

#include "mx/core/Elements.h"
#include "mx/core/Document.h"
#include <sstream>

using namespace mx::core;

namespace mxtest
{
    mx::core::DocumentPtr createDocumentTimewise()
    {
        auto doc = makeDocument( DocumentChoice::timewise );
        
        /* Set Version Attribute on Score Element */
        auto s = doc->getScoreTimewise();
        s->getAttributes()->hasVersion = true;
        s->getAttributes()->version = XsToken( "3.0" );
        
        /* Create Score Title and Credits */
        auto header = s->getScoreHeaderGroup();
        auto composerCredit = makeCredit();
        composerCredit->getCreditChoice()->setChoice( CreditChoice::Choice::creditWords );
        auto wordsGroup = makeCreditWordsGroup();
        wordsGroup->getCreditWords()->setValue( XsString( "Matthew James Briggs" ) );
        composerCredit->getCreditChoice()->addCreditWordsGroup( wordsGroup );
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
        auto timeSignature = makeTimeSignatureGroup();
        timeSignature->getBeats()->setValue( XsString( beats ) );
        timeSignature->getBeatType()->setValue( XsString( "4" ) );
        time->getTimeChoice()->addTimeSignatureGroup( timeSignature );
        time->getTimeChoice()->removeTimeSignatureGroup( time->getTimeChoice()->getTimeSignatureGroupSet().cbegin() );
        properties->addTime( time );
        auto clef = makeClef();
        clef->getSign()->setValue( ClefSign::g );
        clef->setHasLine( true );
        clef->getLine()->setValue( StaffLine( 2 ) );
        properties->addClef( clef );
        output->addMusicDataChoice( propertiesChoice );
        return output;
    }
 }

#endif
