// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/NoteChoiceTest.h"
#include "mxtest/core/FullNoteGroupTest.h"
#include "mxtest/core/GraceNoteGroupTest.h"
#include "mxtest/core/CueNoteGroupTest.h"
#include "mxtest/core/NormalNoteGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, NoteChoice )
{
    TestMode v = TestMode::one;
	NoteChoicePtr object = tgenNoteChoice( v );
	stringstream expected;
	tgenNoteChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, NoteChoice )
{
    TestMode v = TestMode::two;
	NoteChoicePtr object = tgenNoteChoice( v );
	stringstream expected;
	tgenNoteChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, NoteChoice )
{
    TestMode v = TestMode::three;
	NoteChoicePtr object = tgenNoteChoice( v );
	stringstream expected;
	tgenNoteChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    NoteChoicePtr tgenNoteChoice( TestMode v )
    {
        NoteChoicePtr o = makeNoteChoice();
        switch ( v )
        {
            case TestMode::one:
            {
                o->setChoice( NoteChoice::Choice::normal );
                o->setNormalNoteGroup( tgenNormalNoteGroup( v ) );
            }
                break;
            case TestMode::two:
            {
                o->setChoice( NoteChoice::Choice::grace );
                o->setGraceNoteGroup( tgenGraceNoteGroup( v ) );
            }
                break;
            case TestMode::three:
            {
                o->setChoice( NoteChoice::Choice::cue );
                o->setCueNoteGroup( tgenCueNoteGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenNoteChoiceExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                tgenNormalNoteGroupExpected( os, i, v );
            }
                break;
            case TestMode::two:
            {
                tgenGraceNoteGroupExpected( os, i, v );
            }
                break;
            case TestMode::three:
            {
                tgenCueNoteGroupExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
