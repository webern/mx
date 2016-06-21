#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, WithBar )
{
	std::string indentString( INDENT );
	XsString value1{ "Lang" };
	XsString value2{ "Adams" };
	WithBar object1;
	WithBar object2( value2 );
	WithBarAttributesPtr attributes1 = std::make_shared<WithBarAttributes>();
	WithBarAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasDefaultY = true;
    attributes1->defaultY = TenthsValue{ 0.391 };
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<with-bar></with-bar>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<with-bar default-y="0.391">Adams</with-bar>)";
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
