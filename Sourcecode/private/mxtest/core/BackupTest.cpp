// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/BackupTest.h"
#include "mxtest/core/EditorialGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Backup )
{
    TestMode v = TestMode::one;
	BackupPtr object = tgenBackup( v );
	stringstream expected;
	tgenBackupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Backup )
{
    TestMode v = TestMode::two;
	BackupPtr object = tgenBackup( v );
	stringstream expected;
	tgenBackupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Backup )
{
    TestMode v = TestMode::three;
	BackupPtr object = tgenBackup( v );
	stringstream expected;
	tgenBackupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    BackupPtr tgenBackup( TestMode v )
    {
        BackupPtr o = makeBackup();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getDuration()->setValue( PositiveDivisionsValue( 31 ) );
            }
                break;
            case TestMode::three:
            {
                o->getDuration()->setValue( PositiveDivisionsValue( 32 ) );
                o->setEditorialGroup( tgenEditorialGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenBackupExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<backup>)" );
                streamLine( os, i+1, R"(<duration>1</duration>)" );
                streamLine( os, i, R"(</backup>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<backup>)" );
                streamLine( os, i+1, R"(<duration>31</duration>)" );
                streamLine( os, i, R"(</backup>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<backup>)" );
                streamLine( os, i+1, R"(<duration>32</duration>)" );
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</backup>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
