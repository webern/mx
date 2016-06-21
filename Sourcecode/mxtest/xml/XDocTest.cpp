#include "cpul/cpulTestHarness.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"


using namespace std;
using namespace mx::xml;

void streamDoc( const XDocCPtr& xdoc )
{
    xdoc->write( std::cout );
    std::cout << std::endl;
}


TEST( defaultXmlVersion, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    auto xmlVersion = xdoc->getXmlVersion();
    CHECK( xmlVersion == XmlVersion::onePointZero );
}
T_END


TEST( defaultEncoding, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    auto encoding = xdoc->getEncoding();
    CHECK( encoding == DEFAULT_ENCODING );
}
T_END


TEST( defaultGetHasStandaloneAttribute, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    auto hasStandaloneAttribute = xdoc->getHasStandaloneAttribute();
    CHECK( !hasStandaloneAttribute );
}
T_END


TEST( defaultGetIsStandaloneValueYes, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    auto isStandalone = xdoc->getIsStandaloneValueYes();
    CHECK( !isStandalone );
}
T_END


TEST( defaultGetHasDoctypeDeclaration, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    auto hasDoctypeDeclaration = xdoc->getHasDoctypeDeclaration();
    CHECK( !hasDoctypeDeclaration );
}
T_END


TEST( defaultGetDoctypeDeclaration, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    std::string expected = "";
    std::string actual = xdoc->getDoctypeDeclaration();
    CHECK_EQUAL( expected, actual )
}
T_END


TEST( defaultContents, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    
    std::stringstream expected;
    expected << R"(<?xml version="1.0"?>)" << std::endl;
    
    std::stringstream actual;
    xdoc->write( actual );
    
    CHECK_EQUAL( expected.str(), actual.str() );
}
T_END


TEST( xmlVersionOnePointOne, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    
    std::stringstream documentContents;
    documentContents << R"(<?xml version="1.1"?>)" << std::endl;
    documentContents << R"(<root />)" << std::endl;
    xdoc->parse( documentContents );
    std::stringstream actual;
    xdoc->write( actual );
    
    CHECK_EQUAL( documentContents.str(), actual.str() );
    
    auto actualXmlVersion = xdoc->getXmlVersion();
    auto expectedXmlVersion = XmlVersion::onePointOne;
    CHECK( expectedXmlVersion == actualXmlVersion );
}
T_END


TEST( simpleParse1, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    
    std::stringstream input;
    input << R"(<?xml version="1.0"  encoding="UTF-8"?>)" << std::endl;
    input << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 2.0 Partwise//EN"
    "http://www.musicxml.org/dtds/partwise.dtd">)" << std::endl;
    input << R"(<some-tag />)" << std::endl;
    
    xdoc->parse( input );
    
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
    auto actualIsStandalone = xdoc->getIsStandaloneValueYes();
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
    
    std::stringstream input;
    input << R"(<?xml version="1.0"  encoding="UTF-8" standalone="no"?>)" << std::endl;
    input << R"(<some-tag />)" << std::endl;
    
    xdoc->parse( input );
    
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
    auto actualIsStandalone = xdoc->getIsStandaloneValueYes();
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
    
    std::stringstream input;
    input << R"(<?xml version="1.0"  encoding="UTF-8" standalone="yes"?>)" << std::endl;
    input << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 2.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)" << std::endl;
    input << R"(<some-tag />)" << std::endl;
    
    xdoc->parse( input );
    
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
    auto actualIsStandalone = xdoc->getIsStandaloneValueYes();
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
    
    std::stringstream input;
    CHECK_RAISES( xdoc->parse( input ) )
}
T_END


TEST( simpleParse_RootOnly, XDoc )
{
    auto xdoc = XFactory::makeXDoc();
    
    std::stringstream input;
    input << "<score-partwise />";
    
    xdoc->parse( input );
    
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
    auto actualIsStandalone = xdoc->getIsStandaloneValueYes();
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
    
    std::stringstream input;
    input << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 2.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)" << std::endl;
    input << "<score-partwise />";
    
    xdoc->parse( input );
    
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
    auto actualIsStandalone = xdoc->getIsStandaloneValueYes();
    CHECK( expectedIsStandalone == actualIsStandalone );
    
    auto expectedHasDoctype = true;
    auto actualHasDoctype = xdoc->getHasDoctypeDeclaration();
    CHECK( expectedHasDoctype == actualHasDoctype );
    
    auto expectedDoctype = std::string{ R"()" };
    
    auto rootNode = xdoc->getRoot();
    CHECK( rootNode != nullptr );
    CHECK_EQUAL( "score-partwise", rootNode->getName() );
}
T_END
