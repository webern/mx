// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Repeat )
{
	std::string indentString( INDENT );
	Repeat object1;
	Repeat object2;
	RepeatAttributesPtr attributes1 = std::make_shared<RepeatAttributes>();
	RepeatAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasTimes = true;
    attributes1->times = NonNegativeInteger{ 3 };
    attributes1->hasWinged = true;
    attributes1->winged = Winged::doubleCurved;
    attributes1->direction = BackwardForward::forward;
	object2.setAttributes( attributes1 );
	object2.setAttributes( attributesNull ); /* should have no affect */
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<repeat direction="backward"/>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<repeat direction="forward" times="3" winged="double-curved"/>)";
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
