// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/DocumentHeaderTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, DocumentHeader )
{
    DocumentType doctype = kDefaultDocumentType;
	stringstream expected( "score-partwise" );
	stringstream actual;
    stream( actual, doctype );
	CHECK_EQUAL( expected.str(), actual.str() )
    
}
TEST( Test02, DocumentHeader )
{
    DocumentType doctype = DocumentType::timewise;
	stringstream expected( "score-timewise" );
	stringstream actual;
    stream( actual, doctype );
	CHECK_EQUAL( expected.str(), actual.str() )
    
}
TEST( Test03, DocumentHeader )
{
    XmlVersion xmlver = kDefaultXmlVersion;
	stringstream expected( "1.0" );
	stringstream actual;
    stream( actual, xmlver );
	CHECK_EQUAL( expected.str(), actual.str() )
    
}
TEST( Test04, DocumentHeader )
{
    DocumentEncoding encoding = kDefaultDocumentEncoding;
	stringstream expected( "UTF-8" );
	stringstream actual;
    stream( actual, encoding );
	CHECK_EQUAL( expected.str(), actual.str() )

}
TEST( Test05, DocumentHeader )
{
    MusicXmlVersion version = kDefaultMusicXmlVersion;
	stringstream expected( "3.0" );
	stringstream actual;
    stream( actual, version );
	CHECK_EQUAL( expected.str(), actual.str() )
    
}
TEST( Test06, DocumentHeader )
{
    DocumentHeader dochead;
	stringstream expected;
    tgenDocumentHeaderExpected( expected, DocumentType::partwise );
    stringstream actual;
    dochead.toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}
TEST( Test07, DocumentHeader )
{
    DocumentHeader dochead;
    dochead.setDocumentType( DocumentType::timewise );
	stringstream expected;
    tgenDocumentHeaderExpected( expected, DocumentType::timewise );
    stringstream actual;
    dochead.toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}

TEST( Test08, DocumentHeader )
{
    DocumentHeaderPtr dochead = makeDocumentHeader();
	stringstream expected;
    tgenDocumentHeaderExpected( expected, DocumentType::partwise );
    stringstream actual;
    dochead->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}
TEST( Test09, DocumentHeader )
{
    DocumentHeaderPtr dochead = makeDocumentHeader();
    dochead->setDocumentType( DocumentType::timewise );
	stringstream expected;
    tgenDocumentHeaderExpected( expected, DocumentType::timewise );
    stringstream actual;
    dochead->toStream( actual );
	CHECK_EQUAL( expected.str(), actual.str() )
}

namespace mxtest
{
    void tgenDocumentHeaderExpected( std::ostream& os, const mx::core::DocumentType doctype )
    {
        
        switch ( doctype )
        {
            case DocumentType::partwise:
            {
                os << R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
                os << std::endl;
                os << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 3.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)";
                
            }
                break;
            case DocumentType::timewise:
            {
                os << R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
                os << std::endl;
                os << R"(<!DOCTYPE score-timewise PUBLIC "-//Recordare//DTD MusicXML 3.0 Timewise//EN" "http://www.musicxml.org/dtds/timewise.dtd">)";
                
            }
                break;
            default:
                break;
        }
    }
}

#endif
