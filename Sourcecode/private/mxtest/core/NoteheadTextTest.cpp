// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/NoteheadTextTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, NoteheadText )
{
    TestMode v = TestMode::one;
	NoteheadTextPtr object = tgenNoteheadText( v );
	stringstream expected;
	tgenNoteheadTextExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, NoteheadText )
{
    TestMode v = TestMode::two;
	NoteheadTextPtr object = tgenNoteheadText( v );
	stringstream expected;
	tgenNoteheadTextExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, NoteheadText )
{
    TestMode v = TestMode::three;
	NoteheadTextPtr object = tgenNoteheadText( v );
	stringstream expected;
	tgenNoteheadTextExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    NoteheadTextPtr tgenNoteheadText( TestMode v )
    {
        NoteheadTextPtr o = makeNoteheadText();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                auto dt = makeNoteheadTextChoice();
                dt->setChoice( NoteheadTextChoice::Choice::displayText );
                dt->getDisplayText()->setValue( XsString( "B" ) );
                o->addNoteheadTextChoice( dt );
                o->removeNoteheadTextChoice( o->getNoteheadTextChoiceSet().cbegin() );
                
                auto at = makeNoteheadTextChoice();
                at->setChoice( NoteheadTextChoice::Choice::accidentalText );
                at->getAccidentalText()->setValue( AccidentalValue::koron );
                o->addNoteheadTextChoice( at );
            }
                break;
            case TestMode::three:
            {
                auto dt = makeNoteheadTextChoice();
                dt->setChoice( NoteheadTextChoice::Choice::displayText );
                dt->getDisplayText()->setValue( XsString( "C" ) );
                dt->getAccidentalText()->setValue( AccidentalValue::koron );
                o->addNoteheadTextChoice( dt );
                o->removeNoteheadTextChoice( o->getNoteheadTextChoiceSet().cbegin() );
                
                auto at = makeNoteheadTextChoice();
                at->setChoice( NoteheadTextChoice::Choice::accidentalText );
                at->getAccidentalText()->setValue( AccidentalValue::sharp );
                o->addNoteheadTextChoice( at );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenNoteheadTextExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<notehead-text>)" );
                streamLine( os, i+1, R"(<display-text></display-text>)" );
                //streamLine( os, i+1, R"(<accidental-text>natural</accidental-text>)" );
                streamLine( os, i, R"(</notehead-text>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<notehead-text>)" );
                streamLine( os, i+1, R"(<display-text>B</display-text>)" );
                streamLine( os, i+1, R"(<accidental-text>koron</accidental-text>)" );
                streamLine( os, i, R"(</notehead-text>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<notehead-text>)" );
                streamLine( os, i+1, R"(<display-text>C</display-text>)" );
                streamLine( os, i+1, R"(<accidental-text>sharp</accidental-text>)" );
                streamLine( os, i, R"(</notehead-text>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
