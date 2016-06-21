#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, MetronomeBeam )
{
	std::string indentString( INDENT );
	BeamValue value1 = BeamValue::backwardHook;
	BeamValue value2 = BeamValue::continue_;
	MetronomeBeam object1;
	MetronomeBeam object2( value2 );
	MetronomeBeamAttributesPtr attributes1 = std::make_shared<MetronomeBeamAttributes>();
	MetronomeBeamAttributesPtr attributesNull;
	/* set some attribute1 values here */
	object2.setAttributes( attributes1 );
    attributes1->hasNumber = true;
    attributes1->number = BeamLevel{ 4 };
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<metronome-beam>begin</metronome-beam>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<metronome-beam number="4">continue</metronome-beam>)";
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
