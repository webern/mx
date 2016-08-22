// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/NotationsTest.h"
#include "mxtest/core/NotationsChoiceTest.h"
#include "mxtest/core/TupletTest.h"
#include "mxtest/core/OrnamentsTest.h"
#include "mxtest/core/TechnicalTest.h"
#include "mxtest/core/ArticulationsTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Notations )
{
    variant v = variant::one;
	NotationsPtr object = tgenNotations( v );
	stringstream expected;
	tgenNotationsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Notations )
{
    variant v = variant::two;
	NotationsPtr object = tgenNotations( v );
	stringstream expected;
	tgenNotationsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Notations )
{
    variant v = variant::three;
	NotationsPtr object = tgenNotations( v );
	stringstream expected;
	tgenNotationsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    NotationsPtr tgenNotations( variant v )
    {
        NotationsPtr o = makeNotations();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasPrintObject = true;
                o->getAttributes()->printObject = YesNo::yes;
                
                auto n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::tied );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::slur );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::tuplet );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::glissando );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::slide );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::ornaments );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::technical );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::articulations );
                o->addNotationsChoice( n );
                n.reset();
                
            }
                break;
            case variant::three:
            {
                o->getAttributes()->hasPrintObject = true;
                o->getAttributes()->printObject = YesNo::no;
                
                auto n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::dynamics );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::fermata );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::arpeggiate );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::nonArpeggiate );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::accidentalMark );
                o->addNotationsChoice( n );
                n.reset();
                
                n = tgenNotationsChoice();
                n->setChoice( NotationsChoice::Choice::otherNotation );
                o->addNotationsChoice( n );
                n.reset();
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenNotationsExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<notations/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<notations print-object="yes">)" );
                streamLine( os, i+1, R"(<tied type="start" bezier-offset="2.2"/>)" );
                streamLine( os, i+1, R"(<slur type="start" bezier-offset2="3.1"/>)" );
                tgenTupletExpected( os, i+1, variant::two );
                os << std::endl;
                streamLine( os, i+1, R"(<glissando type="start">Weeee</glissando>)" );
                streamLine( os, i+1, R"(<slide type="start">Geronamo?</slide>)" );
                tgenOrnamentsExpected( os, i+1, variant::three );
                os << std::endl;
                tgenTechnicalExpected( os, i+1, variant::two );
                os << std::endl;
                tgenArticulationsExpected( os, i+1, variant::three );
                os << std::endl;
                streamLine( os, i, R"(</notations>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<notations print-object="no">)" );
                streamLine( os, i+1, R"(<dynamics>)" );
                streamLine( os, i+2, R"(<fz/>)" );
                streamLine( os, i+1, R"(</dynamics>)" );
                streamLine( os, i+1, R"(<fermata>square</fermata>)" );
                streamLine( os, i+1, R"(<arpeggiate number="3"/>)" );
                streamLine( os, i+1, R"(<non-arpeggiate type="bottom"/>)" );
                streamLine( os, i+1, R"(<accidental-mark>flat-flat</accidental-mark>)" );
                streamLine( os, i+1, R"(<other-notation type="start">yo mamma</other-notation>)" );
                streamLine( os, i, R"(</notations>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif

