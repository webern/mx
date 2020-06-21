// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/EditorialGroupTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, EditorialGroup )
{
    variant v = variant::one;
	EditorialGroupPtr object = tgenEditorialGroup( v );
	stringstream expected;
	tgenEditorialGroupExpected( expected, 0, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 0, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, EditorialGroup )
{
    variant v = variant::two;
	EditorialGroupPtr object = tgenEditorialGroup( v );
	stringstream expected;
	tgenEditorialGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, EditorialGroup )
{
    variant v = variant::three;
	EditorialGroupPtr object = tgenEditorialGroup( v );
	stringstream expected;
	tgenEditorialGroupExpected( expected, 1, v );
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
    EditorialGroupPtr tgenEditorialGroup( variant v )
    {
        EditorialGroupPtr o = makeEditorialGroup();
        switch ( v )
        {
            case variant::one:
            {
            }
                break;
            case variant::two:
            {
                o->setHasLevel( true );
                o->getLevel()->setValue( XsString( "LevelTwo" ) );
                o->setHasFootnote( true );
                o->getFootnote()->setValue( XsString( "FootNoteTwo" ) );
            }
                break;
            case variant::three:
            {
                o->setHasFootnote( true );
                o->getFootnote()->setValue( XsString( "FootNoteThree" ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenEditorialGroupExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"()", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<footnote>FootNoteTwo</footnote>)" );
                streamLine( os, i, R"(<level>LevelTwo</level>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<footnote>FootNoteThree</footnote>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
