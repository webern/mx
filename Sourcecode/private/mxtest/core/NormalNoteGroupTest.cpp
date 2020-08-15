// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/NormalNoteGroupTest.h"
#include "mxtest/core/FullNoteGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, NormalNoteGroup )
{
    TestMode v = TestMode::one;
	NormalNoteGroupPtr object = tgenNormalNoteGroup( v );
	stringstream expected;
	tgenNormalNoteGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, NormalNoteGroup )
{
    TestMode v = TestMode::two;
	NormalNoteGroupPtr object = tgenNormalNoteGroup( v );
	stringstream expected;
	tgenNormalNoteGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, NormalNoteGroup )
{
    TestMode v = TestMode::three;
	NormalNoteGroupPtr object = tgenNormalNoteGroup( v );
	stringstream expected;
	tgenNormalNoteGroupExpected( expected, 1, v );
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
    NormalNoteGroupPtr tgenNormalNoteGroup( TestMode v )
    {
        NormalNoteGroupPtr o = makeNormalNoteGroup();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setFullNoteGroup( tgenFullNoteGroup( v ) );
                auto start = makeTie();
                start->getAttributes()->type = StartStop::start;
                auto stop = makeTie();
                stop->getAttributes()->type = StartStop::stop;
                o->addTie( start );
                o->addTie( stop );
                o->addTie( stop ); // has no affect //
                o->getDuration()->setValue( PositiveDivisionsValue( 456 ) );
            }
                break;
            case TestMode::three:
            {
                o->setFullNoteGroup( tgenFullNoteGroup( v ) );
                o->getDuration()->setValue( PositiveDivisionsValue( 654 ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenNormalNoteGroupExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                tgenFullNoteGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<duration>1</duration>)", false );
            }
                break;
            case TestMode::two:
            {
                tgenFullNoteGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<duration>456</duration>)" );
                streamLine( os, i, R"(<tie type="start"/>)" );
                streamLine( os, i, R"(<tie type="stop"/>)", false );
            }
                break;
            case TestMode::three:
            {
                tgenFullNoteGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<duration>654</duration>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
