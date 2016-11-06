// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/NoteTest.h"
#include "mxtest/core/NoteChoiceTest.h"
#include "mxtest/core/EditorialVoiceGroupTest.h"
#include "mxtest/core/TimeModificationTest.h"
#include "mxtest/core/NoteheadTextTest.h"
#include "mxtest/core/NotationsTest.h"
#include "mxtest/core/LyricTest.h"
#include "mxtest/core/PlayTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Note )
{
    variant v = variant::one;
	NotePtr object = tgenNote( v );
	stringstream expected;
	tgenNoteExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Note )
{
    variant v = variant::two;
	NotePtr object = tgenNote( v );
	stringstream expected;
	tgenNoteExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Note )
{
    variant v = variant::three;
	NotePtr object = tgenNote( v );
	stringstream expected;
	tgenNoteExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    NotePtr tgenNote( variant v )
    {
        NotePtr o = makeNote();
        switch ( v )
        {
            case variant::one:
            {
                o->setNoteChoice( tgenNoteChoice( v ) );
            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasAttack = true;
                o->getAttributes()->attack = DivisionsValue( 33 );
                o->getAttributes()->hasDynamics = true;
                o->getAttributes()->dynamics = NonNegativeDecimal( 126.99 );
                o->setNoteChoice( tgenNoteChoice( v ) );
                o->getInstrument()->getAttributes()->id = XsIDREF( "I01" );
                o->setHasInstrument( true );
                o->setEditorialVoiceGroup( tgenEditorialVoiceGroup( v ) );
                o->setHasType( true );
                o->getType()->setValue( NoteTypeValue::quarter );
                o->addDot( makeDot() );
                o->addDot( makeDot() );
                o->setHasAccidental( true );
                o->getAccidental()->setValue( AccidentalValue::natural );
                o->setHasTimeModification( true );
                o->setTimeModification( tgenTimeModification( v ) );
                o->addBeam( makeBeam() );
                o->getBeam( o->getBeamSet().cbegin() )->setValue( BeamValue::backwardHook );
                o->addNotations( tgenNotations( v ) );
                o->addLyric( tgenLyric( v ) );
                o->addLyric( tgenLyric( variant::one ) );
                o->setHasPlay( true );
                o->setPlay( tgenPlay( v ) );
            }
                break;
            case variant::three:
            {
                o->setNoteChoice( tgenNoteChoice( v ) );
                o->getAttributes()->hasColor = true;
                o->getAttributes()->color = Color( 1, 2, 3 );
                o->getAttributes()->hasPizzicato = true;
                o->getAttributes()->pizzicato = YesNo::yes;
                o->getInstrument()->getAttributes()->id = XsIDREF( "X987S87F987" );
                o->setHasInstrument( true );
                o->setEditorialVoiceGroup( tgenEditorialVoiceGroup( v ) );
                o->setHasType( true );
                o->getType()->setValue( NoteTypeValue::eighth );
                o->setHasAccidental( true );
                o->getAccidental()->setValue( AccidentalValue::sharpUp );
                o->setHasTimeModification( true );
                o->setTimeModification( tgenTimeModification( v ) );
                o->addNotations( tgenNotations( v ) );
                o->addNotations( tgenNotations( variant::one ) );
                o->addLyric( tgenLyric( v ) );
                o->setHasPlay( true );
                o->setPlay( tgenPlay( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenNoteExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<note>)" );
                tgenNoteChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</note>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<note dynamics="126.99" attack="33">)" );
                tgenNoteChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<instrument id="I01"/>)" );
                tgenEditorialVoiceGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<type>quarter</type>)" );
                streamLine( os, i+1, R"(<dot/>)" );
                streamLine( os, i+1, R"(<dot/>)" );
                streamLine( os, i+1, R"(<accidental>natural</accidental>)" );
                tgenTimeModificationExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<beam>backward hook</beam>)" );
                tgenNotationsExpected( os, i+1,  v );
                os << std::endl;
                tgenLyricExpected( os, i+1,  v );
                os << std::endl;
                tgenLyricExpected( os, i+1,  variant::one );
                os << std::endl;
                tgenPlayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</note>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<note color="#010203" pizzicato="yes">)" );
                tgenNoteChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<instrument id="X987S87F987"/>)" );
                tgenEditorialVoiceGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<type>eighth</type>)" );
                streamLine( os, i+1, R"(<accidental>sharp-up</accidental>)" );
                tgenTimeModificationExpected( os, i+1, v );
                os << std::endl;
                tgenNotationsExpected( os, i+1,  v );
                os << std::endl;
                tgenNotationsExpected( os, i+1,  variant::one );
                os << std::endl;
                tgenLyricExpected( os, i+1,  v );
                os << std::endl;
                tgenPlayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</note>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif

