#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Interchangeable )
{
	Interchangeable object;
	stringstream expected;
	streamLine( expected, 1, R"(<interchangeable>)" );
	streamLine( expected, 2, R"(<beats></beats>)" );
	streamLine( expected, 2, R"(<beat-type></beat-type>)" );
	streamLine( expected, 1, R"(</interchangeable>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, Interchangeable )
{
	Interchangeable object;
    object.getAttributes()->hasSeparator = true;
    object.getAttributes()->separator = TimeSeparator::horizontal;
    object.getAttributes()->hasSymbol = true;
    object.getAttributes()->symbol = TimeSymbol::cut;
    object.getBeats()->setValue( XsString{ "Weird" } );
    object.getBeatType()->setValue( XsString{ "to be a string" } );
    object.setHasTimeRelation( true );
    object.getTimeRelation()->setValue( TimeRelationEnum::hyphen );
	stringstream expected;
	streamLine( expected, 1, R"(<interchangeable symbol="cut" separator="horizontal">)" );
	streamLine( expected, 2, R"(<time-relation>hyphen</time-relation>)" );
	streamLine( expected, 2, R"(<beats>Weird</beats>)" );
	streamLine( expected, 2, R"(<beat-type>to be a string</beat-type>)" );
	streamLine( expected, 1, R"(</interchangeable>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
