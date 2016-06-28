#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/OrnamentsTest.h"
#include "mxtest/core/OrnamentsChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Ornaments )
{
    variant v = variant::one;
	OrnamentsPtr object = tgenOrnaments( v );
	stringstream expected;
	tgenOrnamentsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Ornaments )
{
    variant v = variant::two;
	OrnamentsPtr object = tgenOrnaments( v );
	stringstream expected;
	tgenOrnamentsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Ornaments )
{
    variant v = variant::three;
	OrnamentsPtr object = tgenOrnaments( v );
	stringstream expected;
	tgenOrnamentsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    OrnamentsPtr tgenOrnaments( variant v )
    {
        OrnamentsPtr o = makeOrnaments();
        switch ( v )
        {
            case variant::one:
            {
                auto choice = tgenOrnamentsChoice( v );
                o->setOrnamentsChoice( choice );
            }
                break;
            case variant::two:
            {
                auto choice = tgenOrnamentsChoice( v );
                o->setOrnamentsChoice( choice );
                auto mark1 = makeAccidentalMark();
                mark1->setValue( AccidentalValue::doubleSharp );
                o->addAccidentalMark( mark1 );
            }
                break;
            case variant::three:
            {
                auto choice = tgenOrnamentsChoice( v );
                o->setOrnamentsChoice( choice );
                auto mark1 = makeAccidentalMark();
                mark1->setValue( AccidentalValue::natural );
                o->addAccidentalMark( mark1 );
                auto mark2 = makeAccidentalMark();
                mark2->setValue( AccidentalValue::flat );
                o->addAccidentalMark( mark2 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenOrnamentsExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<ornaments>)" );
                tgenOrnamentsChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</ornaments>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<ornaments>)" );
                tgenOrnamentsChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<accidental-mark>double-sharp</accidental-mark>)" );
                streamLine( os, i, R"(</ornaments>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<ornaments>)" );
                tgenOrnamentsChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<accidental-mark>natural</accidental-mark>)" );
                streamLine( os, i+1, R"(<accidental-mark>flat</accidental-mark>)" );
                streamLine( os, i, R"(</ornaments>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif

