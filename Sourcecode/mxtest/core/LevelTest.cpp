#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Level )
{
	std::string indentString( INDENT );
	XsString value1{ "Bed" };
	XsString value2{ "Time" };
	Level object1;
	Level object2( value2 );
	LevelAttributesPtr attributes1 = std::make_shared<LevelAttributes>();
	LevelAttributesPtr attributesNull;
	/* set some attribute1 values here */
	object2.setAttributes( attributes1 );
    attributes1->hasParentheses = true;
    attributes1->parentheses = YesNo::no;
    attributes1->hasReference = true;
    attributes1->reference = YesNo::yes;
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<level></level>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<level reference="yes" parentheses="no">Time</level>)";
	actual = object2_stream.str();
	CHECK_EQUAL( expected, actual )
	value1 = object2.getValue();
	object1.setValue( value1 );
	std::stringstream o1;	std::stringstream o2;	bool isOneLineOnly = false;
	object1.streamContents( o1, 0, isOneLineOnly );
	object2.streamContents( o2, 0, isOneLineOnly );
	CHECK( isOneLineOnly )
	CHECK_EQUAL( o1.str(), o2.str() )
}

#endif
