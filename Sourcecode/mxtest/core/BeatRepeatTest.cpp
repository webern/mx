// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, BeatRepeat )
{
	BeatRepeat object;
	stringstream expected;
	streamLine( expected, 1, R"(<beat-repeat type="start">)" );
	streamLine( expected, 2, R"(<slash-type>eighth</slash-type>)" );
	streamLine( expected, 1, R"(</beat-repeat>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, BeatRepeat )
{
	BeatRepeat object;
    object.getAttributes()->type = StartStop::stop;
    object.getAttributes()->hasUseDots = true;
    object.getAttributes()->useDots = YesNo::yes;
    object.getAttributes()->hasSlashes = true;
    object.addSlashDot( makeSlashDot() );
	object.addSlashDot( makeSlashDot() );
	stringstream expected;
	streamLine( expected, 1, R"(<beat-repeat type="stop" slashes="1" use-dots="yes">)" );
	streamLine( expected, 2, R"(<slash-type>eighth</slash-type>)" );
	streamLine( expected, 2, R"(<slash-dot/>)" );
	streamLine( expected, 2, R"(<slash-dot/>)" );
	streamLine( expected, 1, R"(</beat-repeat>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif