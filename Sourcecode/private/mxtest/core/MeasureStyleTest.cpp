// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, MeasureStyle )
{
	MeasureStyle object;
	stringstream expected;
	streamLine( expected, 1, R"(<measure-style>)" );
	streamLine( expected, 2, R"(<multiple-rest></multiple-rest>)" );
	streamLine( expected, 1, R"(</measure-style>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, MeasureStyle )
{
	MeasureStyle object;
    object.getMeasureStyleChoice()->setChoice( MeasureStyleChoice::Choice::beatRepeat );
    object.getMeasureStyleChoice()->getBeatRepeat()->addSlashDot( makeSlashDot() );
    object.getAttributes()->hasColor = true;
    object.getAttributes()->color = Color{ 76, 58, 201, 43 };
	stringstream expected;
	streamLine( expected, 1, R"(<measure-style color="#4C3AC92B">)" );
	streamLine( expected, 2, R"(<beat-repeat type="start">)" );
	streamLine( expected, 3, R"(<slash-type>eighth</slash-type>)" );
	streamLine( expected, 3, R"(<slash-dot/>)" );
	streamLine( expected, 2, R"(</beat-repeat>)" );
	streamLine( expected, 1, R"(</measure-style>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test03, MeasureStyle )
{
	MeasureStyle object;
	object.getMeasureStyleChoice()->setChoice( MeasureStyleChoice::Choice::slash );
    stringstream expected;
	streamLine( expected, 1, R"(<measure-style>)" );
	streamLine( expected, 2, R"(<slash type="start">)" );
	streamLine( expected, 3, R"(<slash-type>eighth</slash-type>)" );
	streamLine( expected, 2, R"(</slash>)" );
	streamLine( expected, 1, R"(</measure-style>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test04, MeasureStyle )
{
	MeasureStyle object;
	object.getMeasureStyleChoice()->setChoice( MeasureStyleChoice::Choice::measureRepeat );
    stringstream expected;
	streamLine( expected, 1, R"(<measure-style>)" );
	streamLine( expected, 2, R"(<measure-repeat type="start"></measure-repeat>)" );
	streamLine( expected, 1, R"(</measure-style>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
