#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/HarmonicTypeChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, HarmonicTypeChoice )
{
    variant v = variant::one;
	HarmonicTypeChoicePtr object = tgenHarmonicTypeChoice( v );
	stringstream expected;
	tgenHarmonicTypeChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, HarmonicTypeChoice )
{
    variant v = variant::two;
	HarmonicTypeChoicePtr object = tgenHarmonicTypeChoice( v );
	stringstream expected;
	tgenHarmonicTypeChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, HarmonicTypeChoice )
{
    variant v = variant::three;
	HarmonicTypeChoicePtr object = tgenHarmonicTypeChoice( v );
	stringstream expected;
	tgenHarmonicTypeChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    HarmonicTypeChoicePtr tgenHarmonicTypeChoice( variant v )
    {
        HarmonicTypeChoicePtr o = makeHarmonicTypeChoice();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setChoice( HarmonicTypeChoice::Choice::artificial );
            }
                break;
            case variant::three:
            {
                o->setChoice( HarmonicTypeChoice::Choice::natural );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenHarmonicTypeChoiceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<natural/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<artificial/>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<natural/>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
