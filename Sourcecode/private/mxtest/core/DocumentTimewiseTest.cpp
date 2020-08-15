// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/DocumentTimewiseTest.h"
#include "mxtest/core/ScoreTimewiseTest.h"
#include "mxtest/core/DocumentHeaderTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, DocumentTimewise )
{
    TestMode v = TestMode::one;
	auto object = tgenDocumentTimewise( v );
	stringstream expected;
	tgenDocumentTimewiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}
TEST( Test02, DocumentTimewise )
{
    TestMode v = TestMode::two;
	auto object = tgenDocumentTimewise( v );
	stringstream expected;
	tgenDocumentTimewiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}
TEST( Test03, DocumentTimewise )
{
    TestMode v = TestMode::three;
	auto object = tgenDocumentTimewise( v );
	stringstream expected;
	tgenDocumentTimewiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}

namespace mxtest
{
    DocumentPtr tgenDocumentTimewise( TestMode v )
    {
        DocumentPtr o = makeDocument( DocumentChoice::timewise );
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setScoreTimewise( tgenScoreTimewise( v ) );
            }
                break;
            case TestMode::three:
            {
                o->setScoreTimewise( tgenScoreTimewise( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenDocumentTimewiseExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                tgenDocumentHeaderExpected( os, DocumentType::timewise );
                os << std::endl;
                tgenScoreTimewiseExpected( os, 0, v );
            }
                break;
            case TestMode::two:
            {
                tgenDocumentHeaderExpected( os, DocumentType::timewise );
                os << std::endl;
                tgenScoreTimewiseExpected( os, 0, v );
            }
                break;
            case TestMode::three:
            {
                tgenDocumentHeaderExpected( os, DocumentType::timewise );
                os << std::endl;
                tgenScoreTimewiseExpected( os, 0, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif

