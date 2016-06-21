#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/PageLayoutTest.h"
#include "mxtest/core/PageMarginsTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, PageLayout )
{
    variant v = variant::one;
	PageLayoutPtr object = tgenPageLayout( v );
	stringstream expected;
	tgenPageLayoutExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, PageLayout )
{
    variant v = variant::two;
	PageLayoutPtr object = tgenPageLayout( v );
	stringstream expected;
	tgenPageLayoutExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, PageLayout )
{
    variant v = variant::three;
	PageLayoutPtr object = tgenPageLayout( v );
	stringstream expected;
	tgenPageLayoutExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    PageLayoutPtr tgenPageLayout( variant v )
    {
        PageLayoutPtr o = makePageLayout();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getPageHeight()->setValue( TenthsValue( 2534 ) );
                o->getPageWidth()->setValue( TenthsValue( 4352 ) );
                o->addPageMargins( tgenPageMargins( variant::one ) );
                o->addPageMargins( tgenPageMargins( variant::two ) );
            }
                break;
            case variant::three:
            {
                o->getPageHeight()->setValue( TenthsValue( 3524 ) );
                o->getPageWidth()->setValue( TenthsValue( 3241 ) );
                o->addPageMargins( tgenPageMargins( variant::three ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPageLayoutExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<page-layout>)" );
                streamLine( os, i+1, R"(<page-height>0</page-height>)" );
                streamLine( os, i+1, R"(<page-width>0</page-width>)" );
                streamLine( os, i, R"(</page-layout>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<page-layout>)" );
                streamLine( os, i+1, R"(<page-height>2534</page-height>)" );
                streamLine( os, i+1, R"(<page-width>4352</page-width>)" );
                tgenPageMarginsExpected( os, i+1, variant::one );
                os << std::endl;
                tgenPageMarginsExpected( os, i+1, variant::two );
                os << std::endl;
                streamLine( os, i, R"(</page-layout>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<page-layout>)" );
                streamLine( os, i+1, R"(<page-height>3524</page-height>)" );
                streamLine( os, i+1, R"(<page-width>3241</page-width>)" );
                tgenPageMarginsExpected( os, i+1, variant::three );
                os << std::endl;
                streamLine( os, i, R"(</page-layout>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
