// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/FullNoteGroupTest.h"
#include "mxtest/core/FullNoteTypeChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, FullNoteGroup )
{
    TestMode v = TestMode::one;
	FullNoteGroupPtr object = tgenFullNoteGroup( v );
	stringstream expected;
	tgenFullNoteGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, FullNoteGroup )
{
    TestMode v = TestMode::two;
	FullNoteGroupPtr object = tgenFullNoteGroup( v );
	stringstream expected;
	tgenFullNoteGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, FullNoteGroup )
{
    TestMode v = TestMode::three;
	FullNoteGroupPtr object = tgenFullNoteGroup( v );
	stringstream expected;
	tgenFullNoteGroupExpected( expected, 1, v );
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
    FullNoteGroupPtr tgenFullNoteGroup( TestMode v )
    {
        FullNoteGroupPtr o = makeFullNoteGroup();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setFullNoteTypeChoice( tgenFullNoteTypeChoice( v ) );
            }
                break;
            case TestMode::three:
            {
                o->setHasChord( true );
                o->setFullNoteTypeChoice( tgenFullNoteTypeChoice( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenFullNoteGroupExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                tgenFullNoteTypeChoiceExpected( os, i, v );
            }
                break;
            case TestMode::two:
            {
                tgenFullNoteTypeChoiceExpected( os, i, v );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<chord/>)" );
                tgenFullNoteTypeChoiceExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
