// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/ElisionSyllabicGroupTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, ElisionSyllabicGroup )
{
    TestMode v = TestMode::one;
	ElisionSyllabicGroupPtr object = tgenElisionSyllabicGroup( v );
	stringstream expected;
	tgenElisionSyllabicGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, ElisionSyllabicGroup )
{
    TestMode v = TestMode::two;
	ElisionSyllabicGroupPtr object = tgenElisionSyllabicGroup( v );
	stringstream expected;
	tgenElisionSyllabicGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, ElisionSyllabicGroup )
{
    TestMode v = TestMode::three;
	ElisionSyllabicGroupPtr object = tgenElisionSyllabicGroup( v );
	stringstream expected;
	tgenElisionSyllabicGroupExpected( expected, 1, v );
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
    ElisionSyllabicGroupPtr tgenElisionSyllabicGroup( TestMode v )
    {
        ElisionSyllabicGroupPtr o = makeElisionSyllabicGroup();
        switch ( v )
        {
            case TestMode::one:
            {
                o->getElision()->setValue( XsString( "A" ) );
            }
                break;
            case TestMode::two:
            {
                o->getElision()->setValue( XsString( "B" ) );
                o->setHasSyllabic( true );
                o->getSyllabic()->setValue( SyllabicEnum::single );
            }
                break;
            case TestMode::three:
            {
                o->getElision()->setValue( XsString( "C" ) );
                o->setHasSyllabic( true );
                o->getSyllabic()->setValue( SyllabicEnum::middle );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenElisionSyllabicGroupExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {

                streamLine( os, i, R"(<elision>A</elision>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<elision>B</elision>)" );
                streamLine( os, i, R"(<syllabic>single</syllabic>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<elision>C</elision>)" );
                streamLine( os, i, R"(<syllabic>middle</syllabic>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
