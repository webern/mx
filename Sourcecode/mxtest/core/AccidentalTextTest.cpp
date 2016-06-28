#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, AccidentalText )
{
	std::string indentString( INDENT );
	AccidentalValue value1;
	AccidentalValue value2 = AccidentalValue::sharp;
	AccidentalText object1;
	AccidentalText object2( value2 );
	AccidentalTextAttributesPtr attributes1 = std::make_shared<AccidentalTextAttributes>();
	AccidentalTextAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasLang = true;
    attributes1->justify = LeftCenterRight::right;
    attributes1->hasJustify = true;
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = "<accidental-text>natural</accidental-text>";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+"<accidental-text justify=\"right\" xml:lang=\"it\">sharp</accidental-text>";
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