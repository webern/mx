// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/DocumentPartwiseTest.h"
#include "mxtest/core/ScorePartwiseTest.h"
#include "mxtest/core/DocumentHeaderTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, DocumentPartwise )
{
    TestMode v = TestMode::one;
	DocumentPtr object = tgenDocumentPartwise( v );
	stringstream expected;
	tgenDocumentPartwiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}
TEST( Test02, DocumentPartwise )
{
    TestMode v = TestMode::two;
	DocumentPtr object = tgenDocumentPartwise( v );
	stringstream expected;
	tgenDocumentPartwiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}
TEST( Test03, DocumentPartwise )
{
    TestMode v = TestMode::three;
	DocumentPtr object = tgenDocumentPartwise( v );
	stringstream expected;
	tgenDocumentPartwiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}

namespace mxtest
{
    DocumentPtr tgenDocumentPartwise( TestMode v )
    {
        DocumentPtr o = makeDocument();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setScorePartwise( tgenScorePartwise( v ) );
            }
                break;
            case TestMode::three:
            {
                o->setScorePartwise( tgenScorePartwise( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenDocumentPartwiseExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                tgenDocumentHeaderExpected( os, DocumentType::partwise );
                os << std::endl;
                tgenScorePartwiseExpected( os, 0, v );
            }
                break;
            case TestMode::two:
            {
                tgenDocumentHeaderExpected( os, DocumentType::partwise );
                os << std::endl;
                tgenScorePartwiseExpected( os, 0, v );
            }
                break;
            case TestMode::three:
            {
                tgenDocumentHeaderExpected( os, DocumentType::partwise );
                os << std::endl;
                tgenScorePartwiseExpected( os, 0, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
