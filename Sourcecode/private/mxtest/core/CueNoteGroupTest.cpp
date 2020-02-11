// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/CueNoteGroupTest.h"
#include "mxtest/core/FullNoteGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, CueNoteGroup )
{
    variant v = variant::one;
	CueNoteGroupPtr object = tgenCueNoteGroup( v );
	stringstream expected;
	tgenCueNoteGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, CueNoteGroup )
{
    variant v = variant::two;
	CueNoteGroupPtr object = tgenCueNoteGroup( v );
	stringstream expected;
	tgenCueNoteGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, CueNoteGroup )
{
    variant v = variant::three;
	CueNoteGroupPtr object = tgenCueNoteGroup( v );
	stringstream expected;
	tgenCueNoteGroupExpected( expected, 1, v );
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
    CueNoteGroupPtr tgenCueNoteGroup( variant v )
    {
        CueNoteGroupPtr o = makeCueNoteGroup();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setFullNoteGroup( tgenFullNoteGroup( v ) );
                o->setDuration( makeDuration( PositiveDivisionsValue( 123 ) ) );
                
            }
                break;
            case variant::three:
            {
                o->setFullNoteGroup( tgenFullNoteGroup( v ) );
                
                o->setDuration( makeDuration( PositiveDivisionsValue( 321 ) ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenCueNoteGroupExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<cue/>)" );
                tgenFullNoteGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<duration>1</duration>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<cue/>)" );
                tgenFullNoteGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<duration>123</duration>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<cue/>)" );
                tgenFullNoteGroupExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<duration>321</duration>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
