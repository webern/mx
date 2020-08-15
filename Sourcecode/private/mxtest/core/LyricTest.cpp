// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/ElisionSyllabicGroupTest.h"
#include "mxtest/core/ElisionSyllabicTextGroupTest.h"
#include "mxtest/core/SyllabicTextGroupTest.h"
#include "mxtest/core/LyricTextChoiceTest.h"
#include "mxtest/core/LyricTest.h"
#include "mxtest/core/EditorialGroupTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Lyric )
{
    TestMode v = TestMode::one;
	LyricPtr object = tgenLyric( v );
	stringstream expected;
	tgenLyricExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Lyric )
{
    TestMode v = TestMode::two;
	LyricPtr object = tgenLyric( v );
	stringstream expected;
	tgenLyricExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Lyric )
{
    TestMode v = TestMode::three;
	LyricPtr object = tgenLyric( v );
	stringstream expected;
	tgenLyricExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    LyricPtr tgenLyric( TestMode v )
    {
        LyricPtr o = makeLyric();
        switch ( v )
        {
            case TestMode::one:
            {
                o->setLyricTextChoice( tgenLyricTextChoice( v ) );
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->hasName = true;
                o->getAttributes()->name = XsToken( "Toker" );
                o->setLyricTextChoice( tgenLyricTextChoice( v ) );
                o->setEditorialGroup( tgenEditorialGroup( v ) );
                o->setHasEndLine( true );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->hasName = true;
                o->getAttributes()->name = XsToken( "YOLO" );
                o->getAttributes()->hasNumber = true;
                o->getAttributes()->number = XsNMToken( "Looser" );
                o->setLyricTextChoice( tgenLyricTextChoice( v ) );
                o->setEditorialGroup( tgenEditorialGroup( v ) );
                o->setHasEndParagraph( true );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenLyricExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<lyric>)" );
                tgenLyricTextChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</lyric>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<lyric name="Toker">)" );
                tgenLyricTextChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<end-line/>)" );
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</lyric>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<lyric number="Looser" name="YOLO">)" );
                tgenLyricTextChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<end-paragraph/>)" );
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</lyric>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
