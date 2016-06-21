#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Transpose )
{
	Transpose object;
	stringstream expected;
	streamLine( expected, 1, R"(<transpose>)" );
	streamLine( expected, 2, R"(<chromatic>0</chromatic>)" );
	streamLine( expected, 1, R"(</transpose>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, Transpose )
{
	Transpose object;
    object.getAttributes()->hasNumber = true;
    object.getAttributes()->number = StaffNumber{ 2 };
    object.setHasDiatonic( true );
    object.getDiatonic()->setValue( Integer( 3 ) );
    object.getChromatic()->setValue( Semitones( 4 ) );
    object.setHasDouble( true );
	stringstream expected;
	streamLine( expected, 1, R"(<transpose number="2">)" );
	streamLine( expected, 2, R"(<diatonic>3</diatonic>)" );
	streamLine( expected, 2, R"(<chromatic>4</chromatic>)" );
	streamLine( expected, 2, R"(<double/>)" );
	streamLine( expected, 1, R"(</transpose>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
