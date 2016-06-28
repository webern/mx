#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, MetronomeNote )
{
	MetronomeNote object;
	stringstream expected;
	streamLine( expected, 1, R"(<metronome-note>)" );
	streamLine( expected, 2, R"(<metronome-type>eighth</metronome-type>)" );
	streamLine( expected, 1, R"(</metronome-note>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, MetronomeNote )
{
    auto tm = makeTimeModificationNormalTypeNormalDot();
    tm->getNormalType()->setValue( NoteTypeValue::quarter );
    tm->addNormalDot( makeNormalDot() );
    auto mt = makeMetronomeTuplet();
    mt->setHasTimeModificationNormalTypeNormalDot( true );
    mt->setTimeModificationNormalTypeNormalDot( tm );
    mt->getNormalNotes()->setValue( NonNegativeInteger( 3 ) );
    mt->getActualNotes()->setValue( NonNegativeInteger( 4 ) );
	MetronomeNote object;
    object.setHasMetronomeTuplet( true );
    object.setMetronomeTuplet( mt );
    mt.reset();
    object.getMetronomeType()->setValue( NoteTypeValue::oneHundredTwentyEighth );
    object.addMetronomeBeam( makeMetronomeBeam() );
    object.addMetronomeBeam( makeMetronomeBeam() );
    stringstream expected;
	streamLine( expected, 1, R"(<metronome-note>)" );
	streamLine( expected, 2, R"(<metronome-type>128th</metronome-type>)" );
	streamLine( expected, 2, R"(<metronome-beam>begin</metronome-beam>)" );
	streamLine( expected, 2, R"(<metronome-beam>begin</metronome-beam>)" );
	streamLine( expected, 2, R"(<metronome-tuplet type="start">)" );
	streamLine( expected, 3, R"(<actual-notes>4</actual-notes>)" );
	streamLine( expected, 3, R"(<normal-notes>3</normal-notes>)" );
	streamLine( expected, 3, R"(<normal-type>quarter</normal-type>)" );
	streamLine( expected, 3, R"(<normal-dot/>)" );
	streamLine( expected, 2, R"(</metronome-tuplet>)" );
	streamLine( expected, 1, R"(</metronome-note>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
