// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/ElisionSyllabicGroupTest.h"
#include "mxtest/core/ElisionSyllabicTextGroupTest.h"
#include "mxtest/core/SyllabicTextGroupTest.h"
#include "mxtest/core/LyricTextChoiceTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, LyricTextChoice )
{
    variant v = variant::one;
	LyricTextChoicePtr object = tgenLyricTextChoice( v );
	stringstream expected;
	tgenLyricTextChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, LyricTextChoice )
{
    variant v = variant::two;
	LyricTextChoicePtr object = tgenLyricTextChoice( v );
	stringstream expected;
	tgenLyricTextChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, LyricTextChoice )
{
    variant v = variant::three;
	LyricTextChoicePtr object = tgenLyricTextChoice( v );
	stringstream expected;
	tgenLyricTextChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test04, LyricTextChoice )
{
    variant v = variant::three;
	LyricTextChoicePtr object = tgenLyricTextChoice( v );
    object->setChoice( LyricTextChoice::Choice::extend );
	stringstream expected( R"(<extend/>)" );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 0, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test05, LyricTextChoice )
{
    variant v = variant::three;
	LyricTextChoicePtr object = tgenLyricTextChoice( v );
    object->setChoice( LyricTextChoice::Choice::humming );
	stringstream expected( R"(<humming/>)" );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 0, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}

namespace mxtest
{
    LyricTextChoicePtr tgenLyricTextChoice( variant v )
    {
        LyricTextChoicePtr o = makeLyricTextChoice();
        switch ( v )
        {
            case variant::one:
            {
                o->setChoice( LyricTextChoice::Choice::laughing );
            }
                break;
            case variant::two:
            {
                o->setChoice( LyricTextChoice::Choice::syllabicTextGroup );
                o->setSyllabicTextGroup( tgenSyllabicTextGroup( v ) );
            }
                break;
            case variant::three:
            {
                o->setChoice( LyricTextChoice::Choice::syllabicTextGroup );
                o->setSyllabicTextGroup( tgenSyllabicTextGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenLyricTextChoiceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                
                streamLine( os, i, R"(<laughing/>)", false );
            }
                break;
            case variant::two:
            {
                tgenSyllabicTextGroupExpected( os, i, v );
            }
                break;
            case variant::three:
            {
                tgenSyllabicTextGroupExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
