// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/GraceNoteGroupTest.h"
#include "mxtest/core/FullNoteGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, GraceNoteGroup )
{
    TestMode v = TestMode::one;
	GraceNoteGroupPtr object = tgenGraceNoteGroup( v );
	stringstream expected;
	tgenGraceNoteGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, GraceNoteGroup )
{
    TestMode v = TestMode::two;
	GraceNoteGroupPtr object = tgenGraceNoteGroup( v );
	stringstream expected;
	tgenGraceNoteGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, GraceNoteGroup )
{
    TestMode v = TestMode::three;
	GraceNoteGroupPtr object = tgenGraceNoteGroup( v );
	stringstream expected;
	tgenGraceNoteGroupExpected( expected, 1, v );
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
    GraceNoteGroupPtr tgenGraceNoteGroup( TestMode v )
    {
        GraceNoteGroupPtr o = makeGraceNoteGroup();
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
                
            }
                break;
            case TestMode::three:
            {
                o->setFullNoteGroup( tgenFullNoteGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenGraceNoteGroupExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<grace/>)" );
                tgenFullNoteGroupExpected( os, i, v );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<grace/>)" );
                tgenFullNoteGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<tie type="start"/>)" );
                streamLine( os, i, R"(<tie type="stop"/>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<grace/>)" );
                tgenFullNoteGroupExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
