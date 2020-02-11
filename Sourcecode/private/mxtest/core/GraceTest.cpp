// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Grace )
{
	std::string indentString( INDENT );
	Grace object1;
	Grace object2;
	GraceAttributesPtr attributes1 = std::make_shared<GraceAttributes>();
	GraceAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasMakeTime = true;
    attributes1->makeTime = DivisionsValue{ 0.111 };
    attributes1->hasSlash = true;
    attributes1->slash = YesNo::no;
    attributes1->hasStealTimeFollowing = true;
    attributes1->stealTimeFollowing = Percent{ 0.345 };
    attributes1->hasStealTimePrevious = true;
    attributes1->stealTimePrevious = Percent{ 0.453214 };
	object2.setAttributes( attributes1 );
	object2.setAttributes( attributesNull ); /* should have no affect */
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<grace/>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<grace steal-time-previous="0.453214" steal-time-following="0.345" make-time="0.111" slash="no"/>)";
	actual = object2_stream.str();
	CHECK_EQUAL( expected, actual )
	std::stringstream o1;	std::stringstream o2;	bool isOneLineOnly = false;
	object1.streamContents( o1, 0, isOneLineOnly );
	object2.streamContents( o2, 0, isOneLineOnly );
	CHECK( isOneLineOnly )
	CHECK_EQUAL( o1.str(), o2.str() )
	CHECK( !object1.hasContents() )
	CHECK( !object1.hasAttributes() )
	CHECK( object2.hasAttributes() )
}

#endif
