// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_XML_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"

#include <fstream>

using namespace std;
using namespace mx::xml;
    
void streamDoc( const XDocCPtr& xdoc )
{
    xdoc->saveStream( std::cout );
    std::cout << std::endl;
}


TEST( defaultXmlVersion, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    auto xmlVersion = xdoc->getXmlVersion();
    CHECK( xmlVersion == XmlVersion::onePointZero );
}
T_END


TEST( defaultEncoding, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    auto encoding = xdoc->getEncoding();
    CHECK( encoding == DEFAULT_ENCODING );
}
T_END


TEST( defaultGetHasStandaloneAttribute, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    auto hasStandaloneAttribute = xdoc->getHasStandaloneAttribute();
    CHECK( !hasStandaloneAttribute );
}
T_END


TEST( defaultgetIsStandalone, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    auto isStandalone = xdoc->getIsStandalone();
    CHECK( !isStandalone );
}
T_END


TEST( defaultGetHasDoctypeDeclaration, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    auto hasDoctypeDeclaration = xdoc->getHasDoctypeDeclaration();
    CHECK( !hasDoctypeDeclaration );
}
T_END


TEST( defaultgetDoctypeValue, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    std::string expected = "";
    std::string actual = xdoc->getDoctypeValue();
    CHECK_EQUAL( expected, actual )
}
T_END


TEST( defaultContents, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    
    std::stringstream expected;
    expected << R"(<?xml version="1.0" encoding="UTF-8"?>)" << std::endl;
    expected << "<root />" << std::endl;
    std::stringstream actual;
    xdoc->saveStream( actual );
    
    CHECK_EQUAL( expected.str(), actual.str() );
    
}
T_END


TEST( xmlVersionOnePointOne, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    
    std::stringstream documentContents;
    documentContents << R"(<?xml version="1.1"?>)" << std::endl;
    documentContents << R"(<root />)" << std::endl;
    xdoc->loadStream( documentContents );
    std::stringstream actual;
    xdoc->saveStream( actual );
    
    std::stringstream expectedContents;
    expectedContents << R"(<?xml version="1.1" encoding="UTF-8"?>)" << std::endl;
    expectedContents << R"(<root />)" << std::endl;

    CHECK_EQUAL( expectedContents.str(), actual.str() );
    
    auto actualXmlVersion = xdoc->getXmlVersion();
    auto expectedXmlVersion = XmlVersion::onePointOne;
    CHECK( expectedXmlVersion == actualXmlVersion );
}
T_END


TEST( simpleParse1, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    
    std::stringstream input;
    input << R"(<?xml version="1.0"  encoding="UTF-8"?>)" << std::endl;
    input << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 2.0 Partwise//EN"
    "http://www.musicxml.org/dtds/partwise.dtd">)" << std::endl;
    input << R"(<some-tag />)" << std::endl;
    
    xdoc->loadStream( input );
    
    auto expectedEncoding = Encoding::utfEight;
    auto actualEncoding = xdoc->getEncoding();
    CHECK( expectedEncoding == actualEncoding );
    
    auto expectedXmlVersion = XmlVersion::onePointZero;
    auto actualXmlVersion = xdoc->getXmlVersion();
    CHECK( expectedXmlVersion == actualXmlVersion );
    
    auto expectedHasStandalone = false;
    auto actualHasStandalone = xdoc->getHasStandaloneAttribute();
    CHECK( expectedHasStandalone == actualHasStandalone );
    
    auto expectedIsStandalone = false;
    auto actualIsStandalone = xdoc->getIsStandalone();
    CHECK( expectedIsStandalone == actualIsStandalone );
    
    auto expectedHasDoctype = true;
    auto actualHasDoctype = xdoc->getHasDoctypeDeclaration();
    CHECK( expectedHasDoctype == actualHasDoctype );
    
    auto expectedDoctype = std::string{ R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 2.0 Partwise//EN"
        "http://www.musicxml.org/dtds/partwise.dtd">)" };

    auto rootNode = xdoc->getRoot();
    CHECK( rootNode != nullptr );
    CHECK_EQUAL( "some-tag", rootNode->getName() );
}
T_END


TEST( simpleParse_StandaloneNo, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    
    std::stringstream input;
    input << R"(<?xml version="1.0"  encoding="UTF-8" standalone="no"?>)" << std::endl;
    input << R"(<some-tag />)" << std::endl;
    
    xdoc->loadStream( input );
    
    auto expectedEncoding = Encoding::utfEight;
    auto actualEncoding = xdoc->getEncoding();
    CHECK( expectedEncoding == actualEncoding );
    
    auto expectedXmlVersion = XmlVersion::onePointZero;
    auto actualXmlVersion = xdoc->getXmlVersion();
    CHECK( expectedXmlVersion == actualXmlVersion );
    
    auto expectedHasStandalone = true;
    auto actualHasStandalone = xdoc->getHasStandaloneAttribute();
    CHECK( expectedHasStandalone == actualHasStandalone );
    
    auto expectedIsStandalone = false;
    auto actualIsStandalone = xdoc->getIsStandalone();
    CHECK( expectedIsStandalone == actualIsStandalone );
    
    auto expectedHasDoctype = false;
    auto actualHasDoctype = xdoc->getHasDoctypeDeclaration();
    CHECK( expectedHasDoctype == actualHasDoctype );
    
    auto expectedDoctype = std::string{ R"()" };
        
    auto rootNode = xdoc->getRoot();
    CHECK( rootNode != nullptr )
    CHECK_EQUAL( "some-tag", rootNode->getName() );
}
T_END


TEST( simpleParse_StandaloneYes, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    
    std::stringstream input;
    input << R"(<?xml version="1.0"  encoding="UTF-8" standalone="yes"?>)" << std::endl;
    input << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 2.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)" << std::endl;
    input << R"(<some-tag />)" << std::endl;
    
    xdoc->loadStream( input );
    
    auto expectedEncoding = Encoding::utfEight;
    auto actualEncoding = xdoc->getEncoding();
    CHECK( expectedEncoding == actualEncoding );
    
    auto expectedXmlVersion = XmlVersion::onePointZero;
    auto actualXmlVersion = xdoc->getXmlVersion();
    CHECK( expectedXmlVersion == actualXmlVersion );
    
    auto expectedHasStandalone = true;
    auto actualHasStandalone = xdoc->getHasStandaloneAttribute();
    CHECK( expectedHasStandalone == actualHasStandalone );
    
    auto expectedIsStandalone = true;
    auto actualIsStandalone = xdoc->getIsStandalone();
    CHECK( expectedIsStandalone == actualIsStandalone );
    
    auto expectedHasDoctype = true;
    auto actualHasDoctype = xdoc->getHasDoctypeDeclaration();
    CHECK( expectedHasDoctype == actualHasDoctype );
    
    auto expectedDoctype = std::string{ R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 2.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)" };
    
    auto rootNode = xdoc->getRoot();
    CHECK( rootNode != nullptr );
    CHECK_EQUAL( "some-tag", rootNode->getName() );
}
T_END


TEST( simpleParse_EmptyDocParseException, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    
    std::stringstream input;
    CHECK_RAISES( xdoc->loadStream( input ) )
}
T_END


TEST( simpleParse_RootOnly, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    
    std::stringstream input;
    input << "<score-partwise />";
    
    xdoc->loadStream( input );
    
    auto expectedEncoding = DEFAULT_ENCODING;
    auto actualEncoding = xdoc->getEncoding();
    CHECK( expectedEncoding == actualEncoding );
    
    auto expectedXmlVersion = DEFAULT_XML_VERSION;
    auto actualXmlVersion = xdoc->getXmlVersion();
    CHECK( expectedXmlVersion == actualXmlVersion );
    
    auto expectedHasStandalone = false;
    auto actualHasStandalone = xdoc->getHasStandaloneAttribute();
    CHECK( expectedHasStandalone == actualHasStandalone );
    
    auto expectedIsStandalone = false;
    auto actualIsStandalone = xdoc->getIsStandalone();
    CHECK( expectedIsStandalone == actualIsStandalone );
    
    auto expectedHasDoctype = false;
    auto actualHasDoctype = xdoc->getHasDoctypeDeclaration();
    CHECK( expectedHasDoctype == actualHasDoctype );
    
    auto expectedDoctype = std::string{ R"()" };
    
    auto rootNode = xdoc->getRoot();
    CHECK( rootNode != nullptr );
    CHECK_EQUAL( "score-partwise", rootNode->getName() );
    
}
T_END


TEST( simpleParse_RootAndDoctypeOnly, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    xdoc->setDoWriteByteOrderMark( false );
    
    std::stringstream input;
    input << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 2.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)" << std::endl;
    input << "<score-partwise />";
    
    xdoc->loadStream( input );
    
    auto expectedEncoding = DEFAULT_ENCODING;
    auto actualEncoding = xdoc->getEncoding();
    CHECK( expectedEncoding == actualEncoding );
    
    auto expectedXmlVersion = DEFAULT_XML_VERSION;
    auto actualXmlVersion = xdoc->getXmlVersion();
    CHECK( expectedXmlVersion == actualXmlVersion );
    
    auto expectedHasStandalone = false;
    auto actualHasStandalone = xdoc->getHasStandaloneAttribute();
    CHECK( expectedHasStandalone == actualHasStandalone );
    
    auto expectedIsStandalone = false;
    auto actualIsStandalone = xdoc->getIsStandalone();
    CHECK( expectedIsStandalone == actualIsStandalone );
    
    auto expectedHasDoctype = true;
    auto actualHasDoctype = xdoc->getHasDoctypeDeclaration();
    CHECK( expectedHasDoctype == actualHasDoctype );
    
    auto expectedDoctype = std::string{ R"()" };
    
    auto rootNode = xdoc->getRoot();
    CHECK( rootNode != nullptr );
    CHECK_EQUAL( "score-partwise", rootNode->getName() );
}
    T_END;
    
    namespace
    {
        const char * const XML10_UTF8 = R"(<?xml version="1.0" encoding="UTF-8"?>)";
//      const char * const XML10_UTF16 = R"(<?xml version="1.0" encoding="UTF-16"?>)";
        const char * const XML11_UTF8 = R"(<?xml version="1.1" encoding="UTF-8"?>)";
//      const char * const XML11_UTF16 = R"(<?xml version="1.1" encoding="UTF-16"?>)";
        const char * const XML10_UTF8_STANDALONE_YES = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>)";
//      const char * const XML10_UTF16_STANDALONE_YES = R"(<?xml version="1.0" encoding="UTF-16" standalone="yes"?>)";
//      const char * const XML11_UTF8_STANDALONE_YES = R"(<?xml version="1.1" encoding="UTF-8" standalone="yes"?>)";
//      const char * const XML11_UTF16_STANDALONE_YES = R"(<?xml version="1.1" encoding="UTF-16" standalone="yes"?>)";
        const char * const XML10_UTF8_STANDALONE_NO = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
//      const char * const XML10_UTF16_STANDALONE_NO = R"(<?xml version="1.0" encoding="UTF-16" standalone="no"?>)";
//      const char * const XML11_UTF8_STANDALONE_NO = R"(<?xml version="1.1" encoding="UTF-8" standalone="no"?>)";
//      const char * const XML11_UTF16_STANDALONE_NO = R"(<?xml version="1.1" encoding="UTF-16" standalone="no"?>)";
        const char * const DOCTYPE_EMPTY_WSPACE = R"(<!DOCTYPE >)";
        const char * const DOCTYPE_EMPTY_NSPACE = R"(<!DOCTYPE>)";
        const char * const DOCTYPE_VALUE = R"(<!DOCTYPE SomeDoctypeValue>)";
        const char * const ROOT = R"(<empty />)";
    }
    
    
    struct ThreeLineDocString
    {
        const char* const line1;
        const char* const line2;
        const char* const line3;
        ThreeLineDocString( const char* const inLine1, const char* const inLine2, const char* const inLine3 ) : line1( inLine1 ), line2( inLine2 ), line3( inLine3 ) {}
        std::string toString() const { std::stringstream ss; ss << line1 << std::endl << line2 << std::endl << line3 << std::endl; return ss.str(); }
    };
    
    
    struct TwoLineDocString
    {
        const char* const line1;
        const char* const line2;
        TwoLineDocString( const char* const inLine1, const char* const inLine2 ) : line1( inLine1 ), line2( inLine2 ) {}
        std::string toString() const { std::stringstream ss; ss << line1 << std::endl << line2 << std::endl; return ss.str(); }
    };
    
    inline std::string quickDoc(
        const char* const line1,
        const char* const line2,
        const char* const line3 )
    {
        std::stringstream ss;
        ss << line1 << std::endl << line2 << std::endl << line3 << std::endl;
        return ss.str();
    }
    
    TEST( Xml10_Xml11, XDoc )
    {
        ThreeLineDocString xmlString{ XML10_UTF8, DOCTYPE_EMPTY_WSPACE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );

        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setXmlVersion( XmlVersion::onePointOne );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML11_UTF8, DOCTYPE_EMPTY_WSPACE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( Xml11_Xml10, XDoc )
    {
        ThreeLineDocString xmlString{ XML11_UTF8, DOCTYPE_EMPTY_WSPACE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setXmlVersion( XmlVersion::onePointZero );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML10_UTF8, DOCTYPE_EMPTY_WSPACE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( NoDoctype_Doctype, XDoc )
    {
        TwoLineDocString xmlString{ XML10_UTF8, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( ! xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setHasDoctypeDeclaration( true );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML10_UTF8, DOCTYPE_EMPTY_NSPACE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( Doctype_NoDoctype, XDoc )
    {
        ThreeLineDocString xmlString{ XML10_UTF8, DOCTYPE_EMPTY_WSPACE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setHasDoctypeDeclaration( false );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( ! xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        TwoLineDocString expected{ XML10_UTF8, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( NoDoctype_DoctypeWithValue, XDoc )
    {
        TwoLineDocString xmlString{ XML10_UTF8, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( ! xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setHasDoctypeDeclaration( true );
        xdoc->setDoctypeValue( "SomeDoctypeValue" );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML10_UTF8, DOCTYPE_VALUE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( DoctypeWithValue_NoDoctype, XDoc )
    {
        ThreeLineDocString xmlString{ XML10_UTF8, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setHasDoctypeDeclaration( false );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( ! xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        TwoLineDocString expected{ XML10_UTF8, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( HasStandalone_NoStandalone, XDoc )
    {
        ThreeLineDocString xmlString{ XML10_UTF8_STANDALONE_YES, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setHasStandaloneAttribute( false );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML10_UTF8, DOCTYPE_VALUE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( NoStandalone_HasStandalone, XDoc )
    {
        ThreeLineDocString xmlString{ XML10_UTF8, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setHasStandaloneAttribute( true );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML10_UTF8_STANDALONE_NO, DOCTYPE_VALUE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( NoStandalone_setIsStandaloneFalse, XDoc )
    {
        ThreeLineDocString xmlString{ XML10_UTF8, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setIsStandalone( false );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML10_UTF8_STANDALONE_NO, DOCTYPE_VALUE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
    
    TEST( NoStandalone_setIsStandaloneTrue, XDoc )
    {
        ThreeLineDocString xmlString{ XML10_UTF8, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setIsStandalone( true );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML10_UTF8_STANDALONE_YES, DOCTYPE_VALUE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
    
// TODO - support UTF-16 properly
#if 1==0
    TEST( utf16_utf8, XDoc )
    {
        ThreeLineDocString xmlString{ XML10_UTF16, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfSixteen == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setEncoding( Encoding::utfEight );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML10_UTF8, DOCTYPE_VALUE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
#endif

// TODO - support UTF-16 properly
#if 1==0
    TEST( xml11_utf16, XDoc )
    {
        ThreeLineDocString xmlString{ XML11_UTF16, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfSixteen == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setEncoding( Encoding::utfEight );
        xdoc->setIsStandalone( true );
        xdoc->setDoctypeValue( "" );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML11_UTF8_STANDALONE_YES, DOCTYPE_EMPTY_NSPACE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
#endif
    
// TODO - support UTF-16 properly
#if 1==0
    TEST( xml10_utf16_standalone_yes, XDoc )
    {
        TwoLineDocString xmlString{ XML10_UTF16_STANDALONE_YES, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfSixteen == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( xdoc->getIsStandalone() );
        CHECK( ! xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setXmlVersion( XmlVersion::onePointOne );
        xdoc->setIsStandalone( false );
        xdoc->setDoctypeValue( "" );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfSixteen == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        ThreeLineDocString expected{ XML11_UTF16_STANDALONE_NO, DOCTYPE_EMPTY_NSPACE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
#endif

// TODO - support UTF-16 properly
#if 1==0
    TEST( xml11_utf8_standalone_yes, XDoc )
    {
        ThreeLineDocString xmlString{ XML11_UTF8_STANDALONE_YES, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setXmlVersion( XmlVersion::onePointOne );
        xdoc->setEncoding( Encoding::utfSixteen );
        xdoc->setHasStandaloneAttribute( false );
        xdoc->setHasDoctypeDeclaration( false );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfSixteen == xdoc->getEncoding() );
        CHECK( ! xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( ! xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        TwoLineDocString expected{ XML11_UTF16, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
#endif
    
// TODO - support UTF-16 properly
#if 1==0
    TEST( xml11_utf16_standalone_yes, XDoc )
    {
        ThreeLineDocString xmlString{ XML11_UTF16_STANDALONE_YES, DOCTYPE_VALUE, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfSixteen == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setXmlVersion( XmlVersion::onePointZero );
        xdoc->setEncoding( Encoding::utfSixteen );
        xdoc->setHasStandaloneAttribute( false );
        xdoc->setHasDoctypeDeclaration( false );
        xdoc->setHasStandaloneAttribute( false );
        xdoc->setIsStandalone( false );
        
        CHECK( XmlVersion::onePointZero == xdoc->getXmlVersion() );
        CHECK( Encoding::utfSixteen == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( ! xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        TwoLineDocString expected{ XML10_UTF16_STANDALONE_NO, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
#endif
    
// TODO - support UTF-16 properly
#if 1==0
    TEST( xml11_utf16_standalone_no, XDoc )
    {
        TwoLineDocString xmlString{ XML11_UTF16_STANDALONE_NO, ROOT };
        std::istringstream is( xmlString.toString() );
        auto xdoc = XFactory::makeXDoc();
        xdoc->setDoWriteByteOrderMark( false );
        xdoc->loadStream( is );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfSixteen == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( ! xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "", xdoc->getDoctypeValue() );
        
        std::stringstream reserialized;
        xdoc->saveStream( reserialized );
        CHECK_EQUAL( xmlString.toString(), reserialized.str() );
        
        xdoc->setXmlVersion( XmlVersion::onePointOne );
        xdoc->setEncoding( Encoding::utfEight );
        xdoc->setHasStandaloneAttribute( true );
        xdoc->setHasDoctypeDeclaration( true );
        xdoc->setDoctypeValue( "SomeDoctypeValue" );
        xdoc->setHasStandaloneAttribute( true );
        xdoc->setIsStandalone( false );
        
        CHECK( XmlVersion::onePointOne == xdoc->getXmlVersion() );
        CHECK( Encoding::utfEight == xdoc->getEncoding() );
        CHECK( xdoc->getHasStandaloneAttribute() );
        CHECK( ! xdoc->getIsStandalone() );
        CHECK( xdoc->getHasDoctypeDeclaration() );
        CHECK_EQUAL( "SomeDoctypeValue", xdoc->getDoctypeValue() );
        ThreeLineDocString expected{ XML11_UTF8_STANDALONE_NO, DOCTYPE_VALUE, ROOT };
        std::stringstream actual;
        xdoc->saveStream( actual );
        CHECK_EQUAL( expected.toString(), actual.str() );
    }
    T_END;
#endif
    
#endif
