#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, Kind )
{
	std::string indentString( INDENT );
	KindValue value1 = KindValue::augmented;
	KindValue value2 = KindValue::minorSeventh;
	Kind object1;
	Kind object2( value2 );
	KindAttributesPtr attributes1 = std::make_shared<KindAttributes>();
	KindAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasStackDegrees = true;
    attributes1->stackDegrees = YesNo::yes;
    attributes1->hasFontFamily = true;
    attributes1->fontFamily = CommaSeparatedText( "  Hello  , World" );
	object2.setAttributes( attributes1 );
    object2.setAttributes( attributesNull );
    CHECK( object2.getAttributes() )
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = "<kind>major</kind>";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+"<kind stack-degrees=\"yes\" font-family=\"Hello,World\">minor-seventh</kind>";
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
