#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, PartSymbol )
{
	std::string indentString( INDENT );
	GroupSymbolValue value1 = GroupSymbolValue::brace;
	GroupSymbolValue value2 = GroupSymbolValue::square;
	PartSymbol object1;
	PartSymbol object2( value2 );
	PartSymbolAttributesPtr attributes1 = std::make_shared<PartSymbolAttributes>();
	PartSymbolAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasTopStaff = true;
    attributes1->topStaff = StaffNumber{ 3 };
    attributes1->hasBottomStaff = true;
    attributes1->bottomStaff = StaffNumber{ 15 };
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<part-symbol>none</part-symbol>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<part-symbol top-staff="3" bottom-staff="15">square</part-symbol>)";
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
