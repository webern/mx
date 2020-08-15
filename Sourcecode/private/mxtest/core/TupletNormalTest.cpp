// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/TupletNormalTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, TupletNormal )
{
    TestMode v = TestMode::one;
	TupletNormalPtr object = tgenTupletNormal( v );
	stringstream expected;
	tgenTupletNormalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, TupletNormal )
{
    TestMode v = TestMode::two;
	TupletNormalPtr object = tgenTupletNormal( v );
	stringstream expected;
	tgenTupletNormalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, TupletNormal )
{
    TestMode v = TestMode::three;
	TupletNormalPtr object = tgenTupletNormal( v );
	stringstream expected;
	tgenTupletNormalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    TupletNormalPtr tgenTupletNormal( TestMode v )
    {
        TupletNormalPtr o = makeTupletNormal();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setHasTupletNumber( true );
                o->getTupletNumber()->setValue( NonNegativeInteger( 5 ) );
                o->setHasTupletType( true );
                o->getTupletType()->setValue( NoteTypeValue::eighth );
            }
                break;
            case TestMode::three:
            {
                o->setHasTupletNumber( true );
                o->getTupletNumber()->setValue( NonNegativeInteger( 3 ) );
                o->setHasTupletType( true );
                o->getTupletType()->setValue( NoteTypeValue::quarter );
                o->addTupletDot( makeTupletDot() );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTupletNormalExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<tuplet-normal/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<tuplet-normal>)" );
                streamLine( os, i+1, R"(<tuplet-number>5</tuplet-number>)" );
                streamLine( os, i+1, R"(<tuplet-type>eighth</tuplet-type>)" );
                streamLine( os, i, R"(</tuplet-normal>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<tuplet-normal>)" );
                streamLine( os, i+1, R"(<tuplet-number>3</tuplet-number>)" );
                streamLine( os, i+1, R"(<tuplet-type>quarter</tuplet-type>)" );
                streamLine( os, i+1, R"(<tuplet-dot/>)" );
                streamLine( os, i, R"(</tuplet-normal>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
