#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/MetronomeTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Metronome )
{
	Metronome object;
	stringstream expected;
	streamLine( expected, 1, R"(<metronome>)" );
	streamLine( expected, 2, R"(<beat-unit>eighth</beat-unit>)" );
	streamLine( expected, 2, R"(<per-minute></per-minute>)" );
	streamLine( expected, 1, R"(</metronome>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, Metronome )
{
	MetronomePtr object = tgenMetronome( variant::one );
	stringstream expected;
	streamLine( expected, 1, R"(<metronome font-family="Bone,and,Bish" color="#7BC909">)" );
	streamLine( expected, 2, R"(<beat-unit>16th</beat-unit>)" );
	streamLine( expected, 2, R"(<per-minute font-size="xx-large">104</per-minute>)" );
	streamLine( expected, 1, R"(</metronome>)", false );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Metronome )
{
	MetronomePtr object = tgenMetronome( variant::two );
	stringstream expected;
	streamLine( expected, 1, R"(<metronome halign="center" justify="center" parentheses="yes">)" );
	streamLine( expected, 2, R"(<metronome-note>)" );
	streamLine( expected, 3, R"(<metronome-type>64th</metronome-type>)" );
	streamLine( expected, 3, R"(<metronome-dot/>)" );
	streamLine( expected, 3, R"(<metronome-beam>begin</metronome-beam>)" );
	streamLine( expected, 3, R"(<metronome-tuplet type="start">)" );
	streamLine( expected, 4, R"(<actual-notes>33</actual-notes>)" );
	streamLine( expected, 4, R"(<normal-notes>24</normal-notes>)" );
	streamLine( expected, 3, R"(</metronome-tuplet>)" );
	streamLine( expected, 2, R"(</metronome-note>)" );
	streamLine( expected, 2, R"(<metronome-note>)" );
	streamLine( expected, 3, R"(<metronome-type>32nd</metronome-type>)" );
	streamLine( expected, 3, R"(<metronome-dot/>)" );
	streamLine( expected, 3, R"(<metronome-dot/>)" );
	streamLine( expected, 3, R"(<metronome-beam>begin</metronome-beam>)" );
	streamLine( expected, 3, R"(<metronome-beam>begin</metronome-beam>)" );
	streamLine( expected, 2, R"(</metronome-note>)" );
	streamLine( expected, 2, R"(<metronome-relation>equals</metronome-relation>)" );
	streamLine( expected, 2, R"(<metronome-note>)" );
	streamLine( expected, 3, R"(<metronome-type>64th</metronome-type>)" );
	streamLine( expected, 3, R"(<metronome-tuplet type="start">)" );
	streamLine( expected, 4, R"(<actual-notes>10</actual-notes>)" );
	streamLine( expected, 4, R"(<normal-notes>11</normal-notes>)" );
	streamLine( expected, 4, R"(<normal-type>eighth</normal-type>)" );
	streamLine( expected, 4, R"(<normal-dot/>)" );
	streamLine( expected, 3, R"(</metronome-tuplet>)" );
	streamLine( expected, 2, R"(</metronome-note>)" );
	streamLine( expected, 1, R"(</metronome>)", false );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test04, Metronome )
{
	MetronomePtr object = tgenMetronome( variant::three );
	stringstream expected;
	streamLine( expected, 1, R"(<metronome default-x="13.2">)" );
    streamLine( expected, 2, R"(<beat-unit>quarter</beat-unit>)" );
    streamLine( expected, 2, R"(<beat-unit-dot/>)" );
    streamLine( expected, 2, R"(<beat-unit>32nd</beat-unit>)" );
    streamLine( expected, 2, R"(<beat-unit-dot/>)" );
    streamLine( expected, 2, R"(<beat-unit-dot/>)" );
    streamLine( expected, 1, R"(</metronome>)", false );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    BeatUnitGroupPtr tgenBeatUnitGroup( variant v )
    {
        BeatUnitGroupPtr o = makeBeatUnitGroup();
        switch ( v )
        {
            case variant::one:
            {
                o->getBeatUnit()->setValue( NoteTypeValue::sixteenth );
            }
                break;
            case variant::two:
            {
                o->getBeatUnit()->setValue( NoteTypeValue::quarter );
                o->addBeatUnitDot( makeBeatUnitDot() );
            }
                break;
            case variant::three:
            {
                o->getBeatUnit()->setValue( NoteTypeValue::thirtySecond );
                o->addBeatUnitDot( makeBeatUnitDot() );
                o->addBeatUnitDot( makeBeatUnitDot() );
            }
                break;
            default:
                break;
        }
        return o;
    }
    MetronomeRelationGroupPtr tgenMetronomeRelationGroup( variant v )
    {
        MetronomeRelationGroupPtr o = makeMetronomeRelationGroup();
        switch ( v )
        {
            case variant::one:
            {
                MetronomeRelationPtr mr = makeMetronomeRelation();
                mr->setValue( XsString( "120" ) );
                o->setMetronomeRelation( mr );
                MetronomeNotePtr mn = makeMetronomeNote();
                mn->setHasMetronomeTuplet( false );
                mn->getMetronomeType()->setValue( NoteTypeValue::whole );
                o->setMetronomeNote( mn );
            }
                break;
            case variant::two:
            {
                MetronomeRelationPtr mr = makeMetronomeRelation();
                mr->setValue( XsString( "96" ) );
                o->setMetronomeRelation( mr );
                MetronomeNotePtr mn = makeMetronomeNote();
                mn->setHasMetronomeTuplet( true );
                mn->getMetronomeType()->setValue( NoteTypeValue::sixtyFourth );
                MetronomeTupletPtr mtup = makeMetronomeTuplet();
                mtup->setHasTimeModificationNormalTypeNormalDot( true );
                mtup->getActualNotes()->setValue( NonNegativeInteger( 10 ) );
                mtup->getNormalNotes()->setValue( NonNegativeInteger( 11 ) );
                mtup->getTimeModificationNormalTypeNormalDot()->addNormalDot( makeNormalDot() );
                mn->setMetronomeTuplet( mtup );
                o->setMetronomeNote( mn );
            }
                break;
            case variant::three:
            {
                MetronomeRelationPtr mr = makeMetronomeRelation();
                mr->setValue( XsString( "30" ) );
                o->setMetronomeRelation( mr );
                MetronomeNotePtr mn = makeMetronomeNote();
                mn->setHasMetronomeTuplet( true );
                mn->getMetronomeType()->setValue( NoteTypeValue::sixtyFourth );
                MetronomeTupletPtr mtup = makeMetronomeTuplet();
                mtup->setHasTimeModificationNormalTypeNormalDot( true );
                mtup->getActualNotes()->setValue( NonNegativeInteger( 12 ) );
                mtup->getNormalNotes()->setValue( NonNegativeInteger( 14 ) );
                mtup->getTimeModificationNormalTypeNormalDot()->addNormalDot( makeNormalDot() );
                mtup->getTimeModificationNormalTypeNormalDot()->addNormalDot( makeNormalDot() );
                mn->setMetronomeTuplet( mtup );
                o->setMetronomeNote( mn );
            }
                break;
            default:
                break;
        }
        return o;
    }
    NoteRelationNotePtr tgenNoteRelationNote( variant v )
    {
        NoteRelationNotePtr o = makeNoteRelationNote();
        switch ( v )
        {
            case variant::one:
            {
                MetronomeNotePtr n = makeMetronomeNote();
                n->setHasMetronomeTuplet( true );
                n->getMetronomeType()->setValue( NoteTypeValue::half );
                n->getMetronomeTuplet()->getActualNotes()->setValue( NonNegativeInteger( 3 ) );
                n->getMetronomeTuplet()->getNormalNotes()->setValue( NonNegativeInteger( 4 ) );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeDot( makeMetronomeDot() );
                o->addMetronomeNote( n );
                n = makeMetronomeNote();
                n->setHasMetronomeTuplet( false );
                n->getMetronomeType()->setValue( NoteTypeValue::half );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeDot( makeMetronomeDot() );
                n->addMetronomeDot( makeMetronomeDot() );
                o->addMetronomeNote( n );
                o->removeMetronomeNote( o->getMetronomeNoteSet().cbegin() );
                o->setHasMetronomeRelationGroup( true );
                o->setMetronomeRelationGroup( tgenMetronomeRelationGroup( variant::three ) );
            }
                break;
            case variant::two:
            {
                MetronomeNotePtr n = makeMetronomeNote();
                n->setHasMetronomeTuplet( true );
                n->getMetronomeType()->setValue( NoteTypeValue::sixtyFourth );
                n->getMetronomeTuplet()->getActualNotes()->setValue( NonNegativeInteger( 33 ) );
                n->getMetronomeTuplet()->getNormalNotes()->setValue( NonNegativeInteger( 24 ) );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeDot( makeMetronomeDot() );
                o->addMetronomeNote( n );
                n = makeMetronomeNote();
                n->setHasMetronomeTuplet( false );
                n->getMetronomeType()->setValue( NoteTypeValue::thirtySecond );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeDot( makeMetronomeDot() );
                n->addMetronomeDot( makeMetronomeDot() );
                o->addMetronomeNote( n );
                o->removeMetronomeNote( o->getMetronomeNoteSet().cbegin() );
                o->setHasMetronomeRelationGroup( true );
                o->setMetronomeRelationGroup( tgenMetronomeRelationGroup( variant::two ) );
            }
                break;
            case variant::three:
            {
                MetronomeNotePtr n = makeMetronomeNote();
                n->setHasMetronomeTuplet( false );
                n->getMetronomeType()->setValue( NoteTypeValue::whole );
                n->getMetronomeTuplet()->getActualNotes()->setValue( NonNegativeInteger( 13 ) );
                n->getMetronomeTuplet()->getNormalNotes()->setValue( NonNegativeInteger( 11 ) );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeDot( makeMetronomeDot() );
                o->addMetronomeNote( n );
                n = makeMetronomeNote();
                n->setHasMetronomeTuplet( false );
                n->getMetronomeType()->setValue( NoteTypeValue::sixteenth );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeBeam( makeMetronomeBeam() );
                n->addMetronomeDot( makeMetronomeDot() );
                n->addMetronomeDot( makeMetronomeDot() );
                o->addMetronomeNote( n );
                o->removeMetronomeNote( o->getMetronomeNoteSet().cbegin() );
                o->setHasMetronomeRelationGroup( true );
                o->setMetronomeRelationGroup( tgenMetronomeRelationGroup( variant::one ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    PerMinuteOrBeatUnitChoicePtr tgenPerMinuteOrBeatUnitChoice( variant v )
    {
        PerMinuteOrBeatUnitChoicePtr o = makePerMinuteOrBeatUnitChoice();
        switch ( v )
        {
            case variant::one:
            {
                o->setChoice( PerMinuteOrBeatUnitChoice::Choice::beatUnitGroup );
                o->setBeatUnitGroup( tgenBeatUnitGroup( v ) );
            }
                break;
            case variant::two:
            {
                o->setChoice( PerMinuteOrBeatUnitChoice::Choice::perMinute );
                o->getPerMinute()->setValue( XsString( "104" ) );
                o->getPerMinute()->getAttributes()->hasFontSize = true;
                o->getPerMinute()->getAttributes()->fontSize = FontSize( CssFontSize::xxLarge );
            }
                break;
            case variant::three:
            {
                o->setChoice( PerMinuteOrBeatUnitChoice::Choice::beatUnitGroup );
                o->setChoice( PerMinuteOrBeatUnitChoice::Choice::beatUnitGroup );
                o->setBeatUnitGroup( tgenBeatUnitGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    BeatUnitPerPtr tgenBeatUnitPer( variant v )
    {
        BeatUnitPerPtr o = makeBeatUnitPer();
        switch ( v )
        {
            case variant::one:
            {
                o->setBeatUnitGroup( tgenBeatUnitGroup( variant::two ) );
                o->setPerMinuteOtBeatUnitChoice( tgenPerMinuteOrBeatUnitChoice( variant::three ) );
            }
                break;
            case variant::two:
            {
                o->setBeatUnitGroup( tgenBeatUnitGroup( variant::three ) );
                o->setPerMinuteOtBeatUnitChoice( tgenPerMinuteOrBeatUnitChoice( variant::one ) );
            }
                break;
            case variant::three:
            {
                o->setBeatUnitGroup( tgenBeatUnitGroup( variant::one ) );
                o->setPerMinuteOtBeatUnitChoice( tgenPerMinuteOrBeatUnitChoice( variant::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    BeatUnitPerOrNoteRelationNoteChoicePtr tgenBeatUnitPerOrNoteRelationNoteChoice( variant v )
    {
        BeatUnitPerOrNoteRelationNoteChoicePtr o = makeBeatUnitPerOrNoteRelationNoteChoice();
        switch ( v )
        {
            case variant::one:
            {
                o->setChoice( BeatUnitPerOrNoteRelationNoteChoice::Choice::beatUnitPer );
                o->setBeatUnitPer( tgenBeatUnitPer( variant::three ) );
            }
                break;
            case variant::two:
            {
                o->setChoice( BeatUnitPerOrNoteRelationNoteChoice::Choice::noteRelationNote );
                o->setNoteRelationNote( tgenNoteRelationNote( variant::two ) );
            }
                break;
            case variant::three:
            {
                o->setChoice( BeatUnitPerOrNoteRelationNoteChoice::Choice::beatUnitPer );
                o->setBeatUnitPer( tgenBeatUnitPer( variant::one ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    MetronomeAttributesPtr tgenMetronomeAttributesPtr( variant v )
    {
        MetronomeAttributesPtr o = std::make_shared<MetronomeAttributes>();
        switch ( v )
        {
            case variant::one:
            {
                o->hasColor = true;
                o->color = Color( 123, 201, 9 );
                o->hasFontFamily = true;
                o->fontFamily = CommaSeparatedText( "Bone,and,Bish" );
            }
                break;
            case variant::two:
            {
                o->hasHalign = true;
                o->hasJustify = true;
                o->hasParentheses = true;
                o->parentheses = YesNo::yes;
            }
                break;
            case variant::three:
            {
                o->hasDefaultX = true;
                o->defaultX = TenthsValue{ 13.2 };
            }
                break;
            default:
                break;
        }
        return o;
    }
    MetronomePtr tgenMetronome( variant v )
    {
        MetronomePtr o = makeMetronome();
        switch ( v )
        {
            case variant::one:
            {
                o->setAttributes( tgenMetronomeAttributesPtr( v ) );
                o->setBeatUnitPerOrNoteRelationNoteChoice( tgenBeatUnitPerOrNoteRelationNoteChoice( v ) );
            }
                break;
            case variant::two:
            {
                o->setAttributes( tgenMetronomeAttributesPtr( v ) );
                o->setBeatUnitPerOrNoteRelationNoteChoice( tgenBeatUnitPerOrNoteRelationNoteChoice( v ) );
            }
                break;
            case variant::three:
            {
                o->setAttributes( tgenMetronomeAttributesPtr( v ) );
                o->setBeatUnitPerOrNoteRelationNoteChoice( tgenBeatUnitPerOrNoteRelationNoteChoice( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
}

#endif
