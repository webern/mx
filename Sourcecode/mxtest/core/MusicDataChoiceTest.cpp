#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/MusicDataChoiceTest.h"
#include "mxtest/core/NoteTest.h"
#include "mxtest/core/BackupTest.h"
#include "mxtest/core/ForwardTest.h"
#include "mxtest/core/DirectionTest.h"
#include "mxtest/core/PropertiesTest.h"
#include "mxtest/core/HarmonyTest.h"
#include "mxtest/core/FiguredBassTest.h"
#include "mxtest/core/PrintTest.h"
#include "mxtest/core/SoundTest.h"
#include "mxtest/core/BarlineTest.h"
#include "mxtest/core/GroupingTest.h"


using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, MusicDataChoice )
{
    variant v = variant::one;
	MusicDataChoicePtr object = tgenMusicDataChoice( v );
	stringstream expected;
	tgenMusicDataChoiceExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, MusicDataChoice )
{
    variant v = variant::two;
	MusicDataChoicePtr object = tgenMusicDataChoice( v );
	stringstream expected;
	tgenMusicDataChoiceExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, MusicDataChoice )
{
    variant v = variant::three;
	MusicDataChoicePtr object = tgenMusicDataChoice( v );
	stringstream expected;
	tgenMusicDataChoiceExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    MusicDataChoicePtr tgenMusicDataChoice( variant v )
    {
        MusicDataChoicePtr o;
        switch ( v )
        {
            case variant::one:
            {
                o = makeMusicDataChoice();
            }
                break;
            case variant::two:
            {
                o = tgenMusicDataChoiceAll( v );
                o->setChoice( MusicDataChoice::Choice::barline );
            }
                break;
            case variant::three:
            {
                o = tgenMusicDataChoiceAll( v );
                o->setChoice( MusicDataChoice::Choice::figuredBass );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenMusicDataChoiceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                tgenNoteExpected( os, i, v );
            }
                break;
            case variant::two:
            {
                tgenBarlineExpected( os, i, v );
            }
                break;
            case variant::three:
            {
                tgenFiguredBassExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
    MusicDataChoicePtr tgenMusicDataChoiceAll( variant v )
    {
        MusicDataChoicePtr o = makeMusicDataChoice();
        /*
         <xs:element name="note" type="note"/>
         <xs:element name="backup" type="backup"/>
         <xs:element name="forward" type="forward"/>
         <xs:element name="direction" type="direction"/>
         <xs:element name="attributes" type="attributes"/>
         <xs:element name="harmony" type="harmony"/>
         <xs:element name="figured-bass" type="figured-bass"/>
         <xs:element name="print" type="print"/>
         <xs:element name="sound" type="sound"/>
         <xs:element name="barline" type="barline"/>
         <xs:element name="grouping" type="grouping"/>
         <xs:element name="link" type="link"/>
         <xs:element name="bookmark" type="bookmark"/>
         */
        o->setNote( tgenNote( v ) );
        o->setBackup( tgenBackup( v ) );
        o->setForward( tgenForward( v ) );
        o->setDirection( tgenDirection( v ) );
        o->setProperties( tgenProperties( v ) );
        o->setHarmony( tgenHarmony( v ) );
        o->setFiguredBass( tgenFiguredBass( v ) );
        o->setPrint( tgenPrint( v ) );
        o->setSound( tgenSound( v ) );
        o->setBarline( tgenBarline( v ) );
        o->setGrouping( tgenGrouping( v ) );
        StringType linkString;
        StringType bookmarkString;
        switch ( v )
        {
            case variant::one:
            {
                linkString = "linkStringOne";
                bookmarkString = "bookmarkStringOne";
            }
                break;
            case variant::two:
            {
                linkString = "linkStringTwo";
                bookmarkString = "bookmarkStringTwo";
            }
                break;
            case variant::three:
            {
                linkString = "linkStringThree";
                bookmarkString = "bookmarkStringThree";
            }
                break;
            default:
                break;
        }
        LinkPtr link = makeLink();
        BookmarkPtr bookmark = makeBookmark();
        link->getAttributes()->href = XlinkHref( linkString );
        bookmark->getAttributes()->hasName = true;
        bookmark->getAttributes()->name = XsToken( bookmarkString );
        o->setLink( link );
        o->setBookmark( bookmark );
        return o;
    }
}

#endif
