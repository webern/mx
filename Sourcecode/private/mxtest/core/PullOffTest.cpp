// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, PullOff )
{
	std::string indentString( INDENT );
	XsString value1{ "blue" };
	XsString value2{ "red" };
	PullOff object1;
	PullOff object2( value2 );
	PullOffAttributesPtr attributes1 = std::make_shared<PullOffAttributes>();
	PullOffAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasFontSize = true;
    attributes1->fontSize = FontSize{ CssFontSize::xxLarge };
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<pull-off type="start"></pull-off>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<pull-off type="start" font-size="xx-large">red</pull-off>)";
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
