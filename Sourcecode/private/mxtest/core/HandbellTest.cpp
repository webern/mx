// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Handbell )
{
	std::string indentString( INDENT );
	HandbellValue value1 = HandbellValue::malletLift;
	HandbellValue value2 = HandbellValue::gyro;
	Handbell object1;
	Handbell object2( value2 );
	HandbellAttributesPtr attributes1 = std::make_shared<HandbellAttributes>();
	HandbellAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasFontStyle = true;
    attributes1->fontStyle = FontStyle::italic;
    attributes1->hasRelativeX = true;
    attributes1->relativeX = TenthsValue{ -0.0001 };
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<handbell>damp</handbell>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<handbell relative-x="-0.0001" font-style="italic">gyro</handbell>)";
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
