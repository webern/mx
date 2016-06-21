#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_CORE_TESTS
#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Clef )
{
	Clef object;
    stringstream expected;
	streamLine( expected, 1, R"(<clef>)" );
	streamLine( expected, 2, R"(<sign>G</sign>)" );
	streamLine( expected, 1, R"(</clef>)", false );
	stringstream actual;
    // object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}

TEST( Test02, Clef )
{
	Clef object;
    object.getAttributes()->hasAdditional = true;
    object.getAttributes()->additional = YesNo::yes;
    object.getAttributes()->hasAfterBarline = true;
    object.getAttributes()->afterBarline = YesNo::yes;
    object.setHasClefOctaveChange( true );
    object.getClefOctaveChange()->setValue( Integer { 1 } );
    object.setHasLine( true );
    object.getLine()->setValue( StaffLine( 3 ) );
	stringstream expected;
	streamLine( expected, 1, R"(<clef additional="yes" after-barline="yes">)" );
	streamLine( expected, 2, R"(<sign>G</sign>)" );
	streamLine( expected, 2, R"(<line>3</line>)" );
	streamLine( expected, 2, R"(<clef-octave-change>1</clef-octave-change>)" );
	streamLine( expected, 1, R"(</clef>)", false );
	stringstream actual;
    // object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
