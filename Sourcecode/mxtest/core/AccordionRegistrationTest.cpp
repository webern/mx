// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, AccordionRegistration )
{
	AccordionRegistration object;
	stringstream expected;
	streamLine( expected, 1, R"(<accordion-registration/>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( ! object.hasContents() )
}
TEST( Test02, AccordionRegistration )
{
	AccordionRegistration object;
    object.getAttributes()->hasColor = true;
    object.getAttributes()->hasValign = true;
    object.getAttributes()->valign = Valign::baseline;
    object.setHasAccordionHigh( true );
    object.setHasAccordionMiddle( true );
    object.setHasAccordionLow( true );
    object.getAccordionMiddle()->setValue( AccordionMiddleValue{ 2 } );
	stringstream expected;
	streamLine( expected, 1, R"(<accordion-registration color="#FFFFFF" valign="baseline">)" );
	streamLine( expected, 2, R"(<accordion-high/>)" );
	streamLine( expected, 2, R"(<accordion-middle>2</accordion-middle>)" );
	streamLine( expected, 2, R"(<accordion-low/>)" );
	streamLine( expected, 1, R"(</accordion-registration>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}
#endif
