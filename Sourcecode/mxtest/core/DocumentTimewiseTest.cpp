// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/DocumentTimewiseTest.h"
#include "mxtest/core/ScoreTimewiseTest.h"
#include "mxtest/core/DocumentHeaderTest.h"


using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, DocumentTimewise )
{
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
	auto object = tgenDocumentTimewise( v );
	stringstream expected;
	tgenDocumentTimewiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}

namespace MxTestHelpers
{
    DocumentPtr tgenDocumentTimewise( variant v )
    {
        DocumentPtr o = makeDocument( DocumentChoice::timewise );
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setScoreTimewise( tgenScoreTimewise( v ) );
            }
                break;
            case variant::three:
            {
                o->setScoreTimewise( tgenScoreTimewise( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenDocumentTimewiseExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                tgenDocumentHeaderExpected( os, DocumentType::timewise );
                os << std::endl;
                tgenScoreTimewiseExpected( os, 0, v );
            }
                break;
            case variant::two:
            {
                tgenDocumentHeaderExpected( os, DocumentType::timewise );
                os << std::endl;
                tgenScoreTimewiseExpected( os, 0, v );
            }
                break;
            case variant::three:
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

