// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/TimewisePartTest.h"
#include "mxtest/core/MusicDataGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, TimewisePart )
{
    variant v = variant::one;
	TimewisePartPtr object = tgenTimewisePart( v );
	stringstream expected;
	tgenTimewisePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, TimewisePart )
{
    variant v = variant::two;
	TimewisePartPtr object = tgenTimewisePart( v );
	stringstream expected;
	tgenTimewisePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, TimewisePart )
{
    variant v = variant::three;
	TimewisePartPtr object = tgenTimewisePart( v );
	stringstream expected;
	tgenTimewisePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    TimewisePartPtr tgenTimewisePart( variant v )
    {
        TimewisePartPtr o = makeTimewisePart();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getAttributes()->id = XsID( "IDTWO" );
                o->setMusicDataGroup( tgenMusicDataGroup( v ) );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->id = XsID( "ID300" );
                o->setMusicDataGroup( tgenMusicDataGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTimewisePartExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<part id="ID"/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<part id="IDTWO">)" );
                tgenMusicDataGroupExpected( os, i+1,  v );
                os << std::endl;
                streamLine( os, i, R"(</part>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<part id="ID300">)" );
                tgenMusicDataGroupExpected( os, i+1,  v );
                os << std::endl;
                streamLine( os, i, R"(</part>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
