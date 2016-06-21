#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Relation )
{
	std::string indentString( INDENT );
	XsString value1{ "suliban" };
	XsString value2{ "denobulan" };
	Relation object1;
	Relation object2( value2 );
	RelationAttributesPtr attributes1 = std::make_shared<RelationAttributes>();
	RelationAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasType = true;
    attributes1->type = XsToken{ "Some Token" };
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<relation></relation>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<relation type="Some Token">denobulan</relation>)";
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
