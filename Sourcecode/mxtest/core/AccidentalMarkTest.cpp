// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, AccidentalMark )
{
	std::string indentString( INDENT );
	AccidentalValue value1 = AccidentalValue::doubleSharp;
	AccidentalValue value2 = AccidentalValue::koron;
	AccidentalMark object1;
	AccidentalMark object2( value2 );
	AccidentalMarkAttributesPtr attributes1 = std::make_shared<AccidentalMarkAttributes>();
	AccidentalMarkAttributesPtr attributesNull;
	/* set some attribute1 values here */
	attributes1->hasFontWeight = true;
    attributes1->fontWeight = FontWeight::bold;
    object2.setAttributes( attributes1 );
    std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<accidental-mark>natural</accidental-mark>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<accidental-mark font-weight="bold">koron</accidental-mark>)";
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