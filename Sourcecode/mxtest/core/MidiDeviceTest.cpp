#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, MidiDevice )
{
	std::string indentString( INDENT );
	XsString value1{ "Ives" };
	XsString value2{ "Vaughn-Williams" };
	MidiDevice object1;
	MidiDevice object2( value2 );
	MidiDeviceAttributesPtr attributes1 = std::make_shared<MidiDeviceAttributes>();
	MidiDeviceAttributesPtr attributesNull;
	/* set some attribute1 values here */
	object2.setAttributes( attributes1 );
    attributes1->hasId = true;
    attributes1->id = XsIDREF{ "M1" };
    attributes1->hasPort = true;
    attributes1->port = Midi16{ 16 };
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<midi-device></midi-device>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<midi-device port="16" id="M1">Vaughn-Williams</midi-device>)";
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
