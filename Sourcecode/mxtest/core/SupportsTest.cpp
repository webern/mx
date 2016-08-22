// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Supports )
{
	std::string indentString( INDENT );
	Supports object1;
	Supports object2;
	SupportsAttributesPtr attributes1 = std::make_shared<SupportsAttributes>();
	SupportsAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasValue = true;
    attributes1->hasAttribute = true;
    attributes1->attribute = XsNMToken{ "A" };
    attributes1->value = XsToken{ "B" };
    attributes1->type = YesNo::yes;
    attributes1->element = { "C" };
	object2.setAttributes( attributes1 );
	object2.setAttributes( attributesNull ); /* should have no affect */
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<supports type="no" element=""/>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<supports type="yes" element="C" attribute="A" value="B"/>)";
	actual = object2_stream.str();
	CHECK_EQUAL( expected, actual )
	std::stringstream o1;	std::stringstream o2;	bool isOneLineOnly = false;
	object1.streamContents( o1, 0, isOneLineOnly );
	object2.streamContents( o2, 0, isOneLineOnly );
	CHECK( isOneLineOnly )
	CHECK_EQUAL( o1.str(), o2.str() )
	CHECK( !object1.hasContents() )
	CHECK( object1.hasAttributes() )
	CHECK( object2.hasAttributes() )
}

#endif
