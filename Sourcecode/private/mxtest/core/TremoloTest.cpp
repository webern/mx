// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Tremolo )
{
	std::string indentString( INDENT );
	TremoloMarks value1{ 2 };
	TremoloMarks value2{ 3 };
	Tremolo object1;
	Tremolo object2( value2 );
	TremoloAttributesPtr attributes1 = std::make_shared<TremoloAttributes>();
	TremoloAttributesPtr attributesNull;
	/* set some attribute1 values here */
	//object2.setAttributes( attributes1 );
	object2.getAttributes()->hasPlacement = true;
    object2.getAttributes()->placement = AboveBelow::below;
    std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<tremolo>3</tremolo>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<tremolo placement="below">3</tremolo>)";
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
