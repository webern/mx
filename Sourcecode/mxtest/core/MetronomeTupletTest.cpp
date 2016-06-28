#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, MetronomeTuplet )
{
	MetronomeTuplet object;
	stringstream expected;
	streamLine( expected, 1, R"(<metronome-tuplet type="start">)" );
	streamLine( expected, 2, R"(<actual-notes>1</actual-notes>)" );
	streamLine( expected, 2, R"(<normal-notes>1</normal-notes>)" );
	streamLine( expected, 1, R"(</metronome-tuplet>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, MetronomeTuplet )
{
	MetronomeTuplet object;
    object.getAttributes()->hasShowNumber = true;
    object.getAttributes()->showNumber = ShowTuplet::none;
    object.getAttributes()->hasBracket = true;
    object.getAttributes()->bracket = YesNo::yes;
    object.setHasTimeModificationNormalTypeNormalDot( true );
    object.getTimeModificationNormalTypeNormalDot()->getNormalType()->setValue( NoteTypeValue::breve );
    object.getTimeModificationNormalTypeNormalDot()->addNormalDot( makeNormalDot() );
    object.getTimeModificationNormalTypeNormalDot()->addNormalDot( makeNormalDot() );
    object.getNormalNotes()->setValue( NonNegativeInteger( 2 ) );
    object.getActualNotes()->setValue( NonNegativeInteger( 3 ) );
    stringstream expected;;
	streamLine( expected, 1, R"(<metronome-tuplet type="start" bracket="yes" show-number="none">)" );
	streamLine( expected, 2, R"(<actual-notes>3</actual-notes>)" );
	streamLine( expected, 2, R"(<normal-notes>2</normal-notes>)" );
	streamLine( expected, 2, R"(<normal-type>breve</normal-type>)" );
	streamLine( expected, 2, R"(<normal-dot/>)" );
	streamLine( expected, 2, R"(<normal-dot/>)" );
	streamLine( expected, 1, R"(</metronome-tuplet>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
