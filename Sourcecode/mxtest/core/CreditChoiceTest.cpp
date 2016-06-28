#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/CreditChoiceTest.h"
#include "mxtest/core/CreditWordsGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, CreditChoice )
{
    variant v = variant::one;
	CreditChoicePtr object = tgenCreditChoice( v );
	stringstream expected;
	tgenCreditChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, CreditChoice )
{
    variant v = variant::two;
	CreditChoicePtr object = tgenCreditChoice( v );
	stringstream expected;
	tgenCreditChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, CreditChoice )
{
    variant v = variant::three;
	CreditChoicePtr object = tgenCreditChoice( v );
	stringstream expected;
	tgenCreditChoiceExpected( expected, 1, v );
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
    CreditChoicePtr tgenCreditChoice( variant v )
    {
        CreditChoicePtr o = makeCreditChoice();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setChoice( CreditChoice::Choice::creditImage );
                o->getCreditImage()->getAttributes()->type = XsToken( "SDFGJKH" );
                o->getCreditImage()->getAttributes()->source = XsAnyUri( "someimagesource" );
            }
                break;
            case variant::three:
            {
                o->setChoice( CreditChoice::Choice::creditWords );
                o->setCreditWordsGroup( tgenCreditWordsGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenCreditChoiceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                tgenCreditWordsGroupExpected( os, i, v );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<credit-image source="someimagesource" type="SDFGJKH"/>)", false );
            }
                break;
            case variant::three:
            {
                tgenCreditWordsGroupExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
