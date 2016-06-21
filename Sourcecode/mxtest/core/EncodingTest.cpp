#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/EncodingTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Encoding )
{
    variant v = variant::one;
	EncodingPtr object = tgenEncoding( v );
	stringstream expected;
	tgenEncodingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Encoding )
{
    variant v = variant::two;
	EncodingPtr object = tgenEncoding( v );
	stringstream expected;
	tgenEncodingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Encoding )
{
    variant v = variant::three;
	EncodingPtr object = tgenEncoding( v );
	stringstream expected;
	tgenEncodingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    EncodingPtr tgenEncoding( variant v )
    {
        EncodingPtr o = makeEncoding();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setChoice( Encoding::Choice::software );
                o->getSoftware()->setValue( XsString( "My Software!" ) );
            }
                break;
            case variant::three:
            {
                o->setChoice( Encoding::Choice::encodingDescription );
                o->getEncodingDescription()->setValue( XsString( "Described" ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenEncodingExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<encoding>)" );
                streamLine( os, i+1, R"(<encoding-date>1900-01-01</encoding-date>)" );
                streamLine( os, i, R"(</encoding>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<encoding>)" );
                streamLine( os, i+1, R"(<software>My Software!</software>)" );
                streamLine( os, i, R"(</encoding>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<encoding>)" );
                streamLine( os, i+1, R"(<encoding-description>Described</encoding-description>)" );
                streamLine( os, i, R"(</encoding>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
