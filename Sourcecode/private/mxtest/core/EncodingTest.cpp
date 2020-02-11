// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/EncodingTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

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
	CHECK( ! object->hasContents() )
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

namespace mxtest
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
                auto encodingChoice = makeEncodingChoice();
                encodingChoice->setChoice( EncodingChoice::Choice::software );
                encodingChoice->getSoftware()->setValue( XsString( "My Software!" ) );
                o->addEncodingChoice( encodingChoice );
            }
                break;
            case variant::three:
            {
                auto encodingChoice = makeEncodingChoice();
                encodingChoice->setChoice( EncodingChoice::Choice::software );
                encodingChoice->getSoftware()->setValue( XsString( "Described" ) );
                o->addEncodingChoice( encodingChoice );

                auto anotherChoice = makeEncodingChoice();
                anotherChoice->setChoice( EncodingChoice::Choice::supports );
                o->addEncodingChoice( anotherChoice );
                anotherChoice->getSupports()->getAttributes()->element = XsNMToken( "stupid" );
                anotherChoice->getSupports()->getAttributes()->hasValue = true;
                anotherChoice->getSupports()->getAttributes()->value = XsToken( "TOKEN" );
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
                streamLine( os, i, R"(<encoding/>)", false );
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
                streamLine( os, i+1, R"(<software>Described</software>)" );
                streamLine( os, i+1, R"(<supports type="no" element="stupid" value="TOKEN"/>)" );
                streamLine( os, i, R"(</encoding>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
