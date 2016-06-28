#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/ElisionSyllabicGroupTest.h"
#include "mxtest/core/ElisionSyllabicTextGroupTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, ElisionSyllabicTextGroup )
{
    variant v = variant::one;
	ElisionSyllabicTextGroupPtr object = tgenElisionSyllabicTextGroup( v );
	stringstream expected;
	tgenElisionSyllabicTextGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, ElisionSyllabicTextGroup )
{
    variant v = variant::two;
	ElisionSyllabicTextGroupPtr object = tgenElisionSyllabicTextGroup( v );
	stringstream expected;
	tgenElisionSyllabicTextGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, ElisionSyllabicTextGroup )
{
    variant v = variant::three;
	ElisionSyllabicTextGroupPtr object = tgenElisionSyllabicTextGroup( v );
	stringstream expected;
	tgenElisionSyllabicTextGroupExpected( expected, 1, v );
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
    ElisionSyllabicTextGroupPtr tgenElisionSyllabicTextGroup( variant v )
    {
        ElisionSyllabicTextGroupPtr o = makeElisionSyllabicTextGroup();
        switch ( v )
        {
            case variant::one:
            {
                o->setElisionSyllabicGroup( tgenElisionSyllabicGroup( v ) );
                o->getText()->setValue( XsString( "Brahms" ) );
            }
                break;
            case variant::two:
            {
                o->setElisionSyllabicGroup( tgenElisionSyllabicGroup( v ) );
                o->setHasElisionSyllabicGroup( true );
                o->getText()->setValue( XsString( "Beethoven" ) );
            }
                break;
            case variant::three:
            {
                o->setElisionSyllabicGroup( tgenElisionSyllabicGroup( v ) );
                o->setHasElisionSyllabicGroup( true );
                o->getText()->setValue( XsString( "Berlioz" ) );

            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenElisionSyllabicTextGroupExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                
                streamLine( os, i, R"(<text>Brahms</text>)", false );
            }
                break;
            case variant::two:
            {
                tgenElisionSyllabicGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<text>Beethoven</text>)", false );
            }
                break;
            case variant::three:
            {
                tgenElisionSyllabicGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<text>Berlioz</text>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
