// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Elements.h"
#include <sstream>

using namespace mx::core;

TEST( Test01, DegreeAlter )
{
	std::string indentString( INDENT );
	Semitones value1 = Semitones{ 1 };
	Semitones value2 = Semitones{ -2 };
	DegreeAlter object1;
	DegreeAlter object2( value2 );
	DegreeAlterAttributesPtr attributes1 = std::make_shared<DegreeAlterAttributes>();
	DegreeAlterAttributesPtr attributesNull;
	/* set some attribute1 values here */
    attributes1->hasPlusMinus = true;
	object2.setAttributes( attributes1 );
	std::stringstream default_constructed;
	object1.toStream( default_constructed, 0 );
	std::stringstream object2_stream;
	object2.toStream( object2_stream, 2 );
	std::string expected = R"(<degree-alter>0</degree-alter>)";
	std::string actual = default_constructed.str();
	CHECK_EQUAL( expected, actual )
	expected = indentString+indentString+R"(<degree-alter plus-minus="no">-2</degree-alter>)";
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
