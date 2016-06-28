#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/PercussionTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Percussion )
{
    variant v = variant::one;
	PercussionPtr object = tgenPercussion( v );
	stringstream expected;
	tgenPercussionExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Percussion )
{
    variant v = variant::two;
	PercussionPtr object = tgenPercussion( v );
	stringstream expected;
	tgenPercussionExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Percussion )
{
    variant v = variant::three;
	PercussionPtr object = tgenPercussion( v );
	stringstream expected;
	tgenPercussionExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    PercussionPtr tgenPercussion( variant v )
    {
        PercussionPtr o = makePercussion();
        switch ( v )
        {
            case variant::one:
            {
                ;
            }
                break;
            case variant::two:
            {
                o->setPercussionChoice( tgenPercussionChoice( v ) );
                o->getAttributes()->hasValign = true;
                o->getAttributes()->valign = Valign::baseline;
            }
                break;
            case variant::three:
            {
                o->setPercussionChoice( tgenPercussionChoice( v ) );
                o->getAttributes()->hasHalign = true;
                o->getAttributes()->halign = LeftCenterRight::center;
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPercussionExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<percussion>)" );
                streamLine( os, i+1, R"(<glass>wind chimes</glass>)" );
                streamLine( os, i, R"(</percussion>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<percussion valign="baseline">)" );
                streamLine( os, i+1, R"(<wood>claves</wood>)" );
                streamLine( os, i, R"(</percussion>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<percussion halign="center">)" );
                streamLine( os, i+1, R"(<other-percussion>Hello</other-percussion>)" );
                streamLine( os, i, R"(</percussion>)", false );
            }
                break;
            default:
                break;
        }
    }
    PercussionChoicePtr tgenPercussionChoice( variant v )
    {
        PercussionChoicePtr o = makePercussionChoice();
        switch ( v )
        {
            case variant::one:
            {
                o->setChoice( PercussionChoice::Choice::stickType );
            }
                break;
            case variant::two:
            {
                o->setChoice( PercussionChoice::Choice::wood );
            }
                break;
            case variant::three:
            {
                o->setChoice( PercussionChoice::Choice::otherPercussion );
                o->getOtherPercussion()->setValue( XsString("Hello" ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
}

#endif
