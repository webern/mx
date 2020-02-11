// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Accidental )
{
	std::string indentString( INDENT );
	AccidentalValue value1 = AccidentalValue::flat4;
	AccidentalValue value2 = AccidentalValue::quarterSharp;
	Accidental object1;
	Accidental object2( value2 );
	AccidentalAttributesPtr attributes1 = std::make_shared<AccidentalAttributes>();
	AccidentalAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasBracket = true;
    attributes1->bracket = YesNo::no;
    attributes1->hasEditorial = true;
    attributes1->editorial = YesNo::yes;
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<accidental>natural</accidental>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<accidental editorial="yes" bracket="no">quarter-sharp</accidental>)";
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