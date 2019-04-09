// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, MiscellaneousField )
{
	std::string indentString( INDENT );
	XsString value1{ "Freezing Pieces" };
	XsString value2{ "Escape Sequence" };
	MiscellaneousField object1;
	MiscellaneousField object2( value2 );
	MiscellaneousFieldAttributesPtr attributes1 = std::make_shared<MiscellaneousFieldAttributes>();
	MiscellaneousFieldAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->name = XsToken{ "Title" };
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<miscellaneous-field name=""></miscellaneous-field>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<miscellaneous-field name="Title">Escape Sequence</miscellaneous-field>)";
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
