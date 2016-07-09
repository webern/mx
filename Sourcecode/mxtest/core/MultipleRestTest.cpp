// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, MultipleRest )
{
	std::string indentString( INDENT );
	PositiveIntegerOrEmpty value1( PositiveInteger( 1 ) );
	PositiveIntegerOrEmpty value2( PositiveInteger( 2 ) );
	MultipleRest object1;
	MultipleRest object2( value2 );
	MultipleRestAttributesPtr attributes1 = std::make_shared<MultipleRestAttributes>();
	MultipleRestAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasUseSymbols = true;
    attributes1->useSymbols = YesNo::no;
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = "<multiple-rest></multiple-rest>";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+"<multiple-rest use-symbols=\"no\">2</multiple-rest>";
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
