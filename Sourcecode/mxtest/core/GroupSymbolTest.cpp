// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, GroupSymbol )
{
	std::string indentString( INDENT );
	GroupSymbolValue value1 = GroupSymbolValue::brace;
	GroupSymbolValue value2 = GroupSymbolValue::bracket;
	GroupSymbol object1;
	GroupSymbol object2( value2 );
	GroupSymbolAttributesPtr attributes1 = std::make_shared<GroupSymbolAttributes>();
	GroupSymbolAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasRelativeX = true;
    attributes1->relativeX = TenthsValue( 2.3 );
    attributes1->hasDefaultY = true;
    attributes1->defaultY = TenthsValue( -9.9 );
    
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<group-symbol>none</group-symbol>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<group-symbol default-y="-9.9" relative-x="2.3">bracket</group-symbol>)";
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
