// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/DefaultsTest.h"
#include "mxtest/core/ScalingTest.h"
#include "mxtest/core/LayoutGroupTest.h"
#include "mxtest/core/AppearanceTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Defaults )
{
    variant v = variant::one;
	DefaultsPtr object = tgenDefaults( v );
	stringstream expected;
	tgenDefaultsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Defaults )
{
    variant v = variant::two;
	DefaultsPtr object = tgenDefaults( v );
	stringstream expected;
	tgenDefaultsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Defaults )
{
    variant v = variant::three;
	DefaultsPtr object = tgenDefaults( v );
	stringstream expected;
	tgenDefaultsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    DefaultsPtr tgenDefaults( variant v )
    {
        DefaultsPtr o = makeDefaults();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasScaling( true );
                o->setScaling( tgenScaling( v ) );
                o->setLayoutGroup( tgenLayoutGroup( v ) );
                o->setHasAppearance( true );
                o->setAppearance( tgenAppearance( v ) );
                o->setHasMusicFont( true );
                o->getMusicFont()->getAttributes()->hasFontFamily = true;
                o->getMusicFont()->getAttributes()->fontFamily = CommaSeparatedText( "one,two,three" );
                auto l1 = makeLyricFont();
                l1->getAttributes()->hasFontWeight = true;
                l1->getAttributes()->fontWeight = FontWeight::bold;
                auto l2 = makeLyricFont();
                l2->getAttributes()->hasFontSize = true;
                l2->getAttributes()->fontSize = FontSize( CssFontSize::large );
                o->addLyricFont( l1 );
                o->addLyricFont( l2 );
            }
                break;
            case variant::three:
            {
                o->setHasScaling( true );
                o->setScaling( tgenScaling( v ) );
                o->setLayoutGroup( tgenLayoutGroup( v ) );
                o->setHasAppearance( true );
                o->setAppearance( tgenAppearance( v ) );
                o->setHasMusicFont( true );
                o->getMusicFont()->getAttributes()->hasFontStyle = true;
                o->getMusicFont()->getAttributes()->fontStyle = FontStyle::italic;
                auto l1 = makeLyricFont();
                l1->getAttributes()->hasName = true;
                l1->getAttributes()->name = XsToken( "Hello Bones" );
                auto l2 = makeLyricFont();
                l2->getAttributes()->hasNumber = true;
                l2->getAttributes()->number = XsToken( "Hello Bish" );
                o->addLyricFont( l1 );
                o->addLyricFont( l2 );
                auto lang1 = makeLyricLanguage();
                lang1->getAttributes()->hasName = true;
                lang1->getAttributes()->name = XsToken( "lang1" );
                o->addLyricLanguage( lang1 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenDefaultsExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<defaults/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<defaults>)" );
                tgenScalingExpected( os, i+1, v ); os << std::endl;
                tgenLayoutGroupExpected( os, i+1, v ); os << std::endl;
                tgenAppearanceExpected( os, i+1, v ); os << std::endl;
                streamLine( os, i+1, R"(<music-font font-family="one,two,three"/>)" );
                streamLine( os, i+1, R"(<lyric-font font-weight="bold"/>)" );
                streamLine( os, i+1, R"(<lyric-font font-size="large"/>)" );
                streamLine( os, i, R"(</defaults>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<defaults>)" );
                tgenScalingExpected( os, i+1, v ); os << std::endl;
                tgenLayoutGroupExpected( os, i+1, v ); os << std::endl;
                tgenAppearanceExpected( os, i+1, v ); os << std::endl;
                streamLine( os, i+1, R"(<music-font font-style="italic"/>)" );
                streamLine( os, i+1, R"(<lyric-font name="Hello Bones"/>)" );
                streamLine( os, i+1, R"(<lyric-font number="Hello Bish"/>)" );
                streamLine( os, i+1, R"(<lyric-language name="lang1" xml:lang="it"/>)" );
                streamLine( os, i, R"(</defaults>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
