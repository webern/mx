// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/SystemDividersTest.h"
#include "mxtest/core/PlayTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, SystemDividers )
{
    TestMode v = TestMode::one;
	SystemDividersPtr object = tgenSystemDividers( v );
	stringstream expected;
	tgenSystemDividersExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, SystemDividers )
{
    TestMode v = TestMode::two;
	SystemDividersPtr object = tgenSystemDividers( v );
	stringstream expected;
	tgenSystemDividersExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, SystemDividers )
{
    TestMode v = TestMode::three;
	SystemDividersPtr object = tgenSystemDividers( v );
	stringstream expected;
	tgenSystemDividersExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    SystemDividersPtr tgenSystemDividers( TestMode v )
    {
        SystemDividersPtr o = makeSystemDividers();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getLeftDivider()->getAttributes()->hasHalign = true;
                o->getRightDivider()->getAttributes()->hasFontSize = true;
            }
                break;
            case TestMode::three:
            {
                o->getLeftDivider()->getAttributes()->hasDefaultY = true;
                o->getRightDivider()->getAttributes()->hasRelativeX = true;
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenSystemDividersExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<system-dividers>)" );
                streamLine( os, i+1, R"(<left-divider/>)" );
                streamLine( os, i+1, R"(<right-divider/>)" );
                streamLine( os, i, R"(</system-dividers>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<system-dividers>)" );
                streamLine( os, i+1, R"(<left-divider halign="center"/>)" );
                streamLine( os, i+1, R"(<right-divider font-size="medium"/>)" );
                streamLine( os, i, R"(</system-dividers>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<system-dividers>)" );
                streamLine( os, i+1, R"(<left-divider default-y="0"/>)" );
                streamLine( os, i+1, R"(<right-divider relative-x="0"/>)" );
                streamLine( os, i, R"(</system-dividers>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
