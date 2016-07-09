// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/MusicDataGroupTest.h"
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

TEST( Test01, MusicDataGroup )
{
    variant v = variant::one;
	MusicDataGroupPtr object = tgenMusicDataGroup( v );
	stringstream expected;
	tgenMusicDataGroupExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, MusicDataGroup )
{
    variant v = variant::two;
	MusicDataGroupPtr object = tgenMusicDataGroup( v );
	stringstream expected;
	tgenMusicDataGroupExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, MusicDataGroup )
{
    variant v = variant::three;
	MusicDataGroupPtr object = tgenMusicDataGroup( v );
	stringstream expected;
	tgenMusicDataGroupExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    MusicDataGroupPtr tgenMusicDataGroup( variant v )
    {
        MusicDataGroupPtr o = makeMusicDataGroup();
        switch ( v )
        {
            case variant::one:
            {

            }
                break;
            case variant::two:
            {
                /* <xs:element name="note" type="note"/>
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
                 <xs:element name="bookmark" type="bookmark"/> */
                auto x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::note );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::forward );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::properties );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::figuredBass );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::sound );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::grouping );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::bookmark );
                o->addMusicDataChoice( x );
                x.reset();
            }
                break;
            case variant::three:
            {
                /* <xs:element name="note" type="note"/>
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
                 <xs:element name="bookmark" type="bookmark"/> */
                auto x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::backup );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::direction );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::harmony );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::print );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::barline );
                o->addMusicDataChoice( x );
                x.reset();
                
                x = tgenMusicDataChoiceAll( v );
                x->setChoice( MusicDataChoice::Choice::link );
                o->addMusicDataChoice( x );
                x.reset();
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenMusicDataGroupExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {

            }
                break;
            case variant::two:
            {
                tgenNoteExpected( os, i, v );
                os << std::endl;
                tgenForwardExpected( os, i, v );
                os << std::endl;
                tgenPropertiesExpected( os, i, v );
                os << std::endl;
                tgenFiguredBassExpected( os, i, v );
                os << std::endl;
                tgenSoundExpected( os, i, v );
                os << std::endl;
                tgenGroupingExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<bookmark id="ID" name="bookmarkStringTwo"/>)", false );
            }
                break;
            case variant::three:
            {
                tgenBackupExpected( os, i, v );
                os << std::endl;
                tgenDirectionExpected( os, i, v );
                os << std::endl;
                tgenHarmonyExpected( os, i, v );
                os << std::endl;
                tgenPrintExpected( os, i, v );
                os << std::endl;
                tgenBarlineExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<link xlink:href="linkStringThree"/>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
