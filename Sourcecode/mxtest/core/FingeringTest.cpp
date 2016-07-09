// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Fingering )
{
	std::string indentString( INDENT );
	XsString value1{ "useyourfingers" };
	XsString value2{ "useyourtoes" };
	Fingering object1;
	Fingering object2( value2 );
	FingeringAttributesPtr attributes1 = std::make_shared<FingeringAttributes>();
	FingeringAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasAlternate = true;
    attributes1->alternate = YesNo::yes;
    attributes1->hasFontFamily = true;
    attributes1->fontFamily = CommaSeparatedText{ "Who, put, the bomp, in the bomp" };
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<fingering></fingering>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<fingering alternate="yes" font-family="Who,put,the bomp,in the bomp">useyourtoes</fingering>)";
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
