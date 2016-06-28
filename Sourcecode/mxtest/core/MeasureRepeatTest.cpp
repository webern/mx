#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, MeasureRepeat )
{
	std::string indentString( INDENT );
	PositiveIntegerOrEmpty value1{ "" };
	PositiveIntegerOrEmpty value2{ PositiveInteger{ 2 } };
	MeasureRepeat object1;
	MeasureRepeat object2( value2 );
	MeasureRepeatAttributesPtr attributes1 = std::make_shared<MeasureRepeatAttributes>();
	MeasureRepeatAttributesPtr attributesNull;
	/* set some attribute1 values here */
	object2.setAttributes( attributes1 );
    attributes1->hasSlashes = true;
    attributes1->slashes = PositiveInteger{ 2 };
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<measure-repeat type="start"></measure-repeat>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<measure-repeat type="start" slashes="2">2</measure-repeat>)";
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
