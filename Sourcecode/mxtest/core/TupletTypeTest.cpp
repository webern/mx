#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, TupletType )
{
	std::string indentString( INDENT );
	NoteTypeValue value1 = NoteTypeValue::quarter;
	NoteTypeValue value2 = NoteTypeValue::sixtyFourth;
	TupletType object1;
	TupletType object2( value2 );
	TupletTypeAttributesPtr attributes1 = std::make_shared<TupletTypeAttributes>();
	TupletTypeAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasFontSize = true;
    attributes1->hasFontWeight = true;
    attributes1->hasFontStyle = true;
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<tuplet-type>eighth</tuplet-type>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<tuplet-type font-style="normal" font-size="medium" font-weight="normal">64th</tuplet-type>)";
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
