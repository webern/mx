#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/EditorialVoiceGroupTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, EditorialVoiceGroup )
{
    variant v = variant::one;
	EditorialVoiceGroupPtr object = tgenEditorialVoiceGroup( v );
	stringstream expected;
	tgenEditorialVoiceGroupExpected( expected, 0, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 0, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, EditorialVoiceGroup )
{
    variant v = variant::two;
	EditorialVoiceGroupPtr object = tgenEditorialVoiceGroup( v );
	stringstream expected;
	tgenEditorialVoiceGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, EditorialVoiceGroup )
{
    variant v = variant::three;
	EditorialVoiceGroupPtr object = tgenEditorialVoiceGroup( v );
	stringstream expected;
	tgenEditorialVoiceGroupExpected( expected, 1, v );
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
    EditorialVoiceGroupPtr tgenEditorialVoiceGroup( variant v )
    {
        EditorialVoiceGroupPtr o = makeEditorialVoiceGroup();
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
                o->setHasVoice( true );
                o->getVoice()->setValue( XsString( "123" ) );
            }
                break;
            case variant::three:
            {
                o->setHasFootnote( true );
                o->getFootnote()->setValue( XsString( "FootNoteThree" ) );
                o->setHasVoice( true );
                o->getVoice()->setValue( XsString( "456" ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenEditorialVoiceGroupExpected( std::ostream& os, int i, variant v )
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
                streamLine( os, i, R"(<level>LevelTwo</level>)" );
                streamLine( os, i, R"(<voice>123</voice>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<footnote>FootNoteThree</footnote>)" );
                streamLine( os, i, R"(<voice>456</voice>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
