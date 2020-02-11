// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/TechnicalChoiceTest.h"
#include "mxtest/core/HarmonicTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, TechnicalChoice )
{
    variant v = variant::one;
	TechnicalChoicePtr object = tgenTechnicalChoice( v );
	stringstream expected;
	tgenTechnicalChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, TechnicalChoice )
{
    variant v = variant::two;
	TechnicalChoicePtr object = tgenTechnicalChoice( v );
	stringstream expected;
	tgenTechnicalChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, TechnicalChoice )
{
    variant v = variant::three;
	TechnicalChoicePtr object = tgenTechnicalChoice( v );
	stringstream expected;
	tgenTechnicalChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    TechnicalChoicePtr tgenTechnicalChoice( variant v )
    {
        TechnicalChoicePtr o = makeTechnicalChoice();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setChoice( TechnicalChoice::Choice::string_ );
                o->getString()->setValue( StringNumber( 4 ) );
            }
                break;
            case variant::three:
            {
                o->setChoice( TechnicalChoice::Choice::harmonic );
                o->setHarmonic( tgenHarmonic( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTechnicalChoiceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<up-bow/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<string>4</string>)", false );
            }
                break;
            case variant::three:
            {
                tgenHarmonicExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
