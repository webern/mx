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
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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
    NoteChoicePtr tgenNoteChoice( variant v )
    {
        NoteChoicePtr o = makeNoteChoice();
        switch ( v )
        {
            case variant::one:
            {
                o->setChoice( NoteChoice::Choice::normal );
                o->setNormalNoteGroup( tgenNormalNoteGroup( v ) );
            }
                break;
            case variant::two:
            {
                o->setChoice( NoteChoice::Choice::grace );
                o->setGraceNoteGroup( tgenGraceNoteGroup( v ) );
            }
                break;
            case variant::three:
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
    void tgenNoteChoiceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                tgenNormalNoteGroupExpected( os, i, v );
            }
                break;
            case variant::two:
            {
                tgenGraceNoteGroupExpected( os, i, v );
            }
                break;
            case variant::three:
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
