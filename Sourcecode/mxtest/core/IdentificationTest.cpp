// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/IdentificationTest.h"
#include "mxtest/core/MiscellaneousTest.h"
#include "mxtest/core/EncodingTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Identification )
{
    variant v = variant::one;
	IdentificationPtr object = tgenIdentification( v );
	stringstream expected;
	tgenIdentificationExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Identification )
{
    variant v = variant::two;
	IdentificationPtr object = tgenIdentification( v );
	stringstream expected;
	tgenIdentificationExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Identification )
{
    variant v = variant::three;
	IdentificationPtr object = tgenIdentification( v );
	stringstream expected;
	tgenIdentificationExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    IdentificationPtr tgenIdentification( variant v )
    {
        IdentificationPtr o = makeIdentification();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                auto creator1 = makeCreator( XsString( "Matthew James Briggs" ) );
                creator1->getAttributes()->hasType = true;
                creator1->getAttributes()->type = XsToken( "composer" );
                o->addCreator( creator1 );
                auto creator2 = makeCreator( XsString( "Bones" ) );
                creator2->getAttributes()->hasType = true;
                creator2->getAttributes()->type = XsToken( "cat" );
                o->addCreator( creator2 );
                o->setHasMiscellaneous( true );
                o->setMiscellaneous( tgenMiscellaneous( v ) );
                o->setHasEncoding( true );
                o->setEncoding( tgenEncoding( v ) );
                auto rights1 = makeRights( XsString( "© 2015" ) );
                rights1->getAttributes()->hasType = true;
                rights1->getAttributes()->type = XsToken( "copyright" );
                o->addRights( rights1 );
                o->setHasSource( true );
                o->getSource()->setValue( XsString( "My Brain" ) );
            }
                break;
            case variant::three:
            {
                auto creator1 = makeCreator( XsString( "world" ) );
                creator1->getAttributes()->hasType = true;
                creator1->getAttributes()->type = XsToken( "hello" );
                o->addCreator( creator1 );
                auto creator2 = makeCreator( XsString( "Bishop" ) );
                creator2->getAttributes()->hasType = true;
                creator2->getAttributes()->type = XsToken( "othercat" );
                o->addCreator( creator2 );
                o->setHasMiscellaneous( true );
                o->setMiscellaneous( tgenMiscellaneous( v ) );
                o->setHasEncoding( true );
                o->setEncoding( tgenEncoding( v ) );
                auto rights1 = makeRights( XsString( "unalienable" ) );
                rights1->getAttributes()->hasType = true;
                rights1->getAttributes()->type = XsToken( "human" );
                o->addRights( rights1 );
                auto rights2 = makeRights( XsString( "yes" ) );
                rights2->getAttributes()->hasType = true;
                rights2->getAttributes()->type = XsToken( "animal" );
                o->addRights( rights2 );
                o->setHasSource( true );
                o->getSource()->setValue( XsString( "Manuscript" ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenIdentificationExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<identification/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<identification>)" );
                streamLine( os, i+1, R"(<creator type="composer">Matthew James Briggs</creator>)" );
                streamLine( os, i+1, R"(<creator type="cat">Bones</creator>)" );
                streamLine( os, i+1, R"(<rights type="copyright">© 2015</rights>)" );
                tgenEncodingExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<source>My Brain</source>)" );
                tgenMiscellaneousExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</identification>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<identification>)" );
                streamLine( os, i+1, R"(<creator type="hello">world</creator>)" );
                streamLine( os, i+1, R"(<creator type="othercat">Bishop</creator>)" );
                streamLine( os, i+1, R"(<rights type="human">unalienable</rights>)" );
                streamLine( os, i+1, R"(<rights type="animal">yes</rights>)" );
                tgenEncodingExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<source>Manuscript</source>)" );
                tgenMiscellaneousExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</identification>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
