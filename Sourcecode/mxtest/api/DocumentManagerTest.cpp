// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/file/Path.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/LayoutData.h"
#include "mx/core/Document.h"
#include "mx/core/elements/PageMargins.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/TopMargin.h"
#include "mx/core/elements/BottomMargin.h"


using namespace std;
using namespace mx::api;

inline int loadDoc()
{
    auto& docMngr = DocumentManager::getInstance();
    return docMngr.createFromFile( std::string{ mxtest::RESOURCES_DIRECTORY_PATH } + std::string{ "/recsuite/Dichterliebe01.xml" } );
}

inline int loadActorPreludeDoc()
{
    auto& docMngr = DocumentManager::getInstance();
    return docMngr.createFromFile( std::string{ mxtest::RESOURCES_DIRECTORY_PATH } + std::string{ "/recsuite/ActorPreludeSample.xml" } );
}

inline void destroyDoc( int documentId )
{
    auto& docMngr = DocumentManager::getInstance();
    docMngr.destroyDocument( documentId );
}


inline ScoreData getScore()
{
    auto documentId = loadDoc();
    auto score = DocumentManager::getInstance().getData( documentId );
    destroyDoc( documentId );
    return score;
}

inline ScoreData getActorPreludeScore()
{
    auto documentId = loadActorPreludeDoc();
    auto score = DocumentManager::getInstance().getData( documentId );
    destroyDoc( documentId );
    return score;
}

#if 1==0
TEST( createFromFile, DocumentManager )
{
    auto& docMngr = DocumentManager::getInstance();
    auto documentId = docMngr.createFromFile( std::string{ mxtest::RESOURCES_DIRECTORY_PATH } + std::string{ "/recsuite/Dichterliebe01.xml" } );
    CHECK( documentId > 0 );
    
    auto mxdocPtr = docMngr.getDocument( documentId );
    docMngr.destroyDocument( documentId );
    
    auto shouldBeNull = docMngr.getDocument( documentId );
    CHECK( shouldBeNull == nullptr );
}
T_END


TEST( musicXmlType, DocumentManager )
{
    auto score = getScore();
    CHECK_EQUAL( "partwise", score.musicXmlType );
}
T_END


TEST( workTitle, DocumentManager )
{
    auto score = getScore();
    CHECK_EQUAL( "Dichterliebe", score.workTitle );
}
T_END


TEST( workNumber, DocumentManager )
{
    auto score = getScore();
    CHECK_EQUAL( "Op. 48", score.workNumber );
}
T_END


TEST( movementTitle, DocumentManager )
{
    auto score = getScore();
    CHECK_EQUAL( "Im wunderschönen Monat Mai", score.movementTitle );
}
T_END


TEST( movementNumber, DocumentManager )
{
    auto score = getScore();
    CHECK_EQUAL( "1", score.movementNumber );
}
T_END


TEST( composerName, DocumentManager )
{
    auto score = getScore();
    CHECK_EQUAL( "Robert Schumann", score.composer );
}
T_END


TEST( lyricistName, DocumentManager )
{
    auto score = getScore();
    CHECK_EQUAL( "Heinrich Heine", score.lyricist );
}
T_END


TEST( copyright, DocumentManager )
{
    auto score = getScore();
    CHECK_EQUAL( "Copyright © 2002 Recordare LLC", score.copyright );
}
T_END


TEST( scalingMillimeters, DocumentManager )
{
    auto score = getScore();
    CHECK_DOUBLES_EQUAL( 6.35, score.layout.scalingMillimeters, MX_API_EQUALITY_EPSILON )
}
T_END


TEST( scalingTenths, DocumentManager )
{
    auto score = getScore();
    CHECK_DOUBLES_EQUAL( 40, score.layout.scalingTenths, MX_API_EQUALITY_EPSILON )
}
T_END


TEST( tenthsPerMillimeter, DocumentManager )
{
    auto score = getScore();
    CHECK_DOUBLES_EQUAL( 6.299212598425197, score.layout.tenthsPerMillimeter(), MX_API_EQUALITY_EPSILON )
}
T_END

TEST( tenthsPerInch, DocumentManager )
{
    auto score = getActorPreludeScore();
    CHECK_DOUBLES_EQUAL( 160, score.layout.tenthsPerInch(), MX_API_EQUALITY_EPSILON )
}
T_END


TEST( sillyTest, DocumentManager )
{
    auto score = ScoreData{};
    score.workTitle = "workTitle";
    score.workNumber = "workNumber";
    score.movementTitle = "movementTitle";
    score.movementNumber = "movementNumber";
    score.composer = "The Composer";
    score.lyricist = "The Lyricist";
    score.musicXmlType = "timewise";
    score.encoding.encoder = "The Encoder";
    score.encoding.encodingDescription = "The Encoding Description";
    score.encoding.software.emplace_back( "Software 1" );
    score.encoding.software.emplace_back( "Software 2" );
    score.encoding.encodingDate.year = 2016;
    score.encoding.encodingDate.month = 8;
    score.encoding.encodingDate.day = 30;
    score.copyright = "© 2016 by Matthew James Briggs";
    auto documentId = DocumentManager::getInstance().createFromScore( score );
    DocumentManager::getInstance().writeToFile( documentId, "./sillytest.xml" );
    DocumentManager::getInstance().destroyDocument( documentId );
}
T_END


TEST( RoundTrip_WorkTitle, DocumentManager )
{
    auto value = std::string{ "value" };
    auto score = ScoreData{};
    score.workTitle = value;
    auto documentId = DocumentManager::getInstance().createFromScore( score );
    std::ostringstream oss;
    DocumentManager::getInstance().writeToStream( documentId, oss );
    DocumentManager::getInstance().destroyDocument( documentId );
    std::istringstream iss{ oss.str() };
    documentId = DocumentManager::getInstance().createFromStream( iss );
    auto actual = DocumentManager::getInstance().getData( documentId );
    DocumentManager::getInstance().destroyDocument( documentId );
    CHECK_EQUAL( value, actual.workTitle );
}
T_END


#define ROUND_TRIP_TEST_SCALAR( scalarType, fieldPath, fieldName, value, nameSuffix ) \
TEST( RoundTrip_##structType##_##fieldName##_##nameSuffix, DocumentManager ) \
{ \
    auto testValue = scalarType{ value }; \
    auto expectedStruct = ScoreData{}; \
    auto actualStruct = ScoreData{}; \
    auto& actualValue = actualStruct.fieldPath; \
    auto& expectedValue = expectedStruct.fieldPath; \
    expectedValue = testValue; \
    auto documentId = DocumentManager::getInstance().createFromScore( expectedStruct ); \
    std::ostringstream oss; \
    DocumentManager::getInstance().writeToStream( documentId, oss ); \
    DocumentManager::getInstance().destroyDocument( documentId ); \
    std::istringstream iss{ oss.str() }; \
    documentId = DocumentManager::getInstance().createFromStream( iss ); \
    actualStruct = DocumentManager::getInstance().getData( documentId ); \
    DocumentManager::getInstance().destroyDocument( documentId ); \
    CHECK_EQUAL( expectedValue, actualValue ); \
} \
T_END

ROUND_TRIP_TEST_SCALAR( std::string, musicXmlType, musicXmlType, "timewise", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, musicXmlType, musicXmlType, "partwise", 1 );
ROUND_TRIP_TEST_SCALAR( std::string, workTitle, workTitle, "value", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, workNumber, workNumber, "value", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, movementTitle, movementTitle, "value", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, movementNumber, movementNumber, "value", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, composer, composer, "value", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, lyricist, lyricist, "value", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, copyright, copyright, "value", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, encoding.encoder, encoder, "value", 0 );
ROUND_TRIP_TEST_SCALAR( std::string, encoding.encodingDescription, encodingDescription, "value", 0 );
ROUND_TRIP_TEST_SCALAR( int, encoding.encodingDate.year, year, 2016, 0 );
ROUND_TRIP_TEST_SCALAR( int, encoding.encodingDate.month, month, 9, 0 );
ROUND_TRIP_TEST_SCALAR( int, encoding.encodingDate.day, day, 12, 0 );


//#endif


#define ROUND_TRIP_TEST_SCALAR_DOUBLE( scalarType, fieldPath, fieldName, value, nameSuffix ) \
TEST( RoundTrip_##structType##_##fieldName##_##nameSuffix, DocumentManager ) \
{ \
auto testValue = scalarType{ value }; \
auto expectedStruct = ScoreData{}; \
auto actualStruct = ScoreData{}; \
auto& actualValue = actualStruct.fieldPath; \
auto& expectedValue = expectedStruct.fieldPath; \
expectedValue = testValue; \
auto documentId = DocumentManager::getInstance().createFromScore( expectedStruct ); \
std::ostringstream oss; \
DocumentManager::getInstance().writeToStream( documentId, oss ); \
DocumentManager::getInstance().destroyDocument( documentId ); \
std::istringstream iss{ oss.str() }; \
documentId = DocumentManager::getInstance().createFromStream( iss ); \
actualStruct = DocumentManager::getInstance().getData( documentId ); \
DocumentManager::getInstance().destroyDocument( documentId ); \
CHECK_DOUBLES_EQUAL( expectedValue, actualValue, MX_API_EQUALITY_EPSILON ); \
} \
T_END

using LongDouble = long double;

ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.scalingMillimeters, scalingMillimeters, 5.451, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.scalingTenths, scalingTenths, 5.452, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.oddPageLeftMargin, oddPageLeftMargin, 5.453, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.oddPageRightMargin, oddPageRightMargin, 5.454, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.oddPageTopMargin, oddPageTopMargin, 5.455, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.oddPageBottomMargin, oddPageBottomMargin, 5.456, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.evenPageLeftMargin, evenPageLeftMargin, 5.457, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.evenPageRightMargin, evenPageRightMargin, 5.458, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.evenPageTopMargin, evenPageTopMargin, 5.459, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.evenPageBottomMargin, evenPageBottomMargin, 5.4501, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.systemLeftMargin, systemLeftMargin, 5.4502, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.systemRightMargin, systemRightMargin, 5.4503, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.systemDistance, systemDistance, 5.4504, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.topSystemDistance, topSystemDistance, 5.4505, 0 );
ROUND_TRIP_TEST_SCALAR_DOUBLE( LongDouble, layout.staffDistance, staffDistance, 5.4506, 0 );


TEST( Layout_PageMarginsBoth, DocumentManager )
{
    auto score = ScoreData{};
    const long double left = 0.1;
    const long double right = 0.2;
    const long double top = 0.3;
    const long double bottom = 0.4;
    score.layout.oddPageLeftMargin = left;
    score.layout.evenPageLeftMargin = left;
    score.layout.oddPageRightMargin = right;
    score.layout.evenPageRightMargin = right;
    score.layout.oddPageTopMargin = top;
    score.layout.evenPageTopMargin = top;
    score.layout.oddPageBottomMargin = bottom;
    score.layout.evenPageBottomMargin = bottom;
    auto docId = DocumentManager::getInstance().createFromScore( score );
    auto mxDoc = DocumentManager::getInstance().getDocument( docId );
    const auto& pageMarginsSet = mxDoc->getScorePartwise()->getScoreHeaderGroup()->getDefaults()->getLayoutGroup()->getPageLayout()->getPageMarginsSet();
    CHECK_EQUAL( 1, pageMarginsSet.size() );
    CHECK( mx::core::MarginType::both == (*pageMarginsSet.cbegin())->getAttributes()->type );
}
T_END


TEST( Layout_PageMarginsEvenOdd, DocumentManager )
{
    auto score = ScoreData{};
    const long double left = 0.1;
    const long double right = 0.2;
    const long double top = 0.3;
    const long double bottom = 0.4;
    score.layout.oddPageLeftMargin = left + 100.0;
    score.layout.evenPageLeftMargin = left;
    score.layout.oddPageRightMargin = right;
    score.layout.evenPageRightMargin = right;
    score.layout.oddPageTopMargin = top;
    score.layout.evenPageTopMargin = top;
    score.layout.oddPageBottomMargin = bottom;
    score.layout.evenPageBottomMargin = bottom;
    auto docId = DocumentManager::getInstance().createFromScore( score );
    auto mxDoc = DocumentManager::getInstance().getDocument( docId );
    const auto& pageMarginsSet = mxDoc->getScorePartwise()->getScoreHeaderGroup()->getDefaults()->getLayoutGroup()->getPageLayout()->getPageMarginsSet();
    CHECK_EQUAL( 2, pageMarginsSet.size() );
    auto iter = pageMarginsSet.cbegin();
    CHECK( mx::core::MarginType::odd == (*iter)->getAttributes()->type );
    ++iter;
    CHECK( mx::core::MarginType::even == (*iter)->getAttributes()->type );
}
T_END


TEST( RoundTrip_SupportedItems_elementName, DocumentManager )
{
    auto testValue0 = std::string{ "value0" };
    auto testValue1 = std::string{ "value1" };
    auto expectedStruct0 = SupportedItem{};
    expectedStruct0.elementName = testValue0;
    auto expectedStruct1 = SupportedItem{};
    expectedStruct1.elementName = testValue1;
    auto actualStruct = SupportedItem{};
    auto inputScore = ScoreData{};
    auto outputScore = ScoreData{};
    inputScore.encoding.supportedItems.push_back( expectedStruct0 );    
    inputScore.encoding.supportedItems.push_back( expectedStruct1 );    
    auto documentId = DocumentManager::getInstance().createFromScore( inputScore );
    std::ostringstream oss;
    DocumentManager::getInstance().writeToStream( documentId, oss );
    DocumentManager::getInstance().destroyDocument( documentId );
    std::istringstream iss{ oss.str() };
    documentId = DocumentManager::getInstance().createFromStream( iss );
    outputScore = DocumentManager::getInstance().getData( documentId );
    DocumentManager::getInstance().destroyDocument( documentId );
    auto actualValue = outputScore.encoding.supportedItems.at( 0 ).elementName;
    CHECK_EQUAL( testValue0, actualValue );
    actualValue = outputScore.encoding.supportedItems.at( 1 ).elementName;
    CHECK_EQUAL( testValue1, actualValue );
}
T_END

TEST( RoundTrip_SupportedItems_attributeName, DocumentManager )
{
    auto testValue0 = std::string{ "value0" };
    auto testValue1 = std::string{ "value1" };
    auto expectedStruct0 = SupportedItem{};
    expectedStruct0.attributeName = testValue0;
    auto expectedStruct1 = SupportedItem{};
    expectedStruct1.attributeName = testValue1;
    auto actualStruct = SupportedItem{};
    auto inputScore = ScoreData{};
    auto outputScore = ScoreData{};
    inputScore.encoding.supportedItems.push_back( expectedStruct0 );    
    inputScore.encoding.supportedItems.push_back( expectedStruct1 );    
    auto documentId = DocumentManager::getInstance().createFromScore( inputScore );
    std::ostringstream oss;
    DocumentManager::getInstance().writeToStream( documentId, oss );
    DocumentManager::getInstance().destroyDocument( documentId );
    std::istringstream iss{ oss.str() };
    documentId = DocumentManager::getInstance().createFromStream( iss );
    outputScore = DocumentManager::getInstance().getData( documentId );
    DocumentManager::getInstance().destroyDocument( documentId );
    auto actualValue = outputScore.encoding.supportedItems.at( 0 ).attributeName;
    CHECK_EQUAL( testValue0, actualValue );
    actualValue = outputScore.encoding.supportedItems.at( 1 ).attributeName;
    CHECK_EQUAL( testValue1, actualValue );
}
T_END

TEST( RoundTrip_SupportedItems_specificValue, DocumentManager )
{
    auto testValue0 = std::string{ "value0" };
    auto testValue1 = std::string{ "value1" };
    auto expectedStruct0 = SupportedItem{};
    expectedStruct0.specificValue = testValue0;
    auto expectedStruct1 = SupportedItem{};
    expectedStruct1.specificValue = testValue1;
    auto actualStruct = SupportedItem{};
    auto inputScore = ScoreData{};
    auto outputScore = ScoreData{};
    inputScore.encoding.supportedItems.push_back( expectedStruct0 );    
    inputScore.encoding.supportedItems.push_back( expectedStruct1 );    
    auto documentId = DocumentManager::getInstance().createFromScore( inputScore );
    std::ostringstream oss;
    DocumentManager::getInstance().writeToStream( documentId, oss );
    DocumentManager::getInstance().destroyDocument( documentId );
    std::istringstream iss{ oss.str() };
    documentId = DocumentManager::getInstance().createFromStream( iss );
    outputScore = DocumentManager::getInstance().getData( documentId );
    DocumentManager::getInstance().destroyDocument( documentId );
    auto actualValue = outputScore.encoding.supportedItems.at( 0 ).specificValue;
    CHECK_EQUAL( testValue0, actualValue );
    actualValue = outputScore.encoding.supportedItems.at( 1 ).specificValue;
    CHECK_EQUAL( testValue1, actualValue );
}
T_END

TEST( RoundTrip_SupportedItems_isSupported, DocumentManager )
{
    auto testValue0 = std::string{ "value0" };
    auto testValue1 = std::string{ "value1" };
    auto inputScore = ScoreData{};
    auto outputScore = ScoreData{};
    inputScore.encoding.software.push_back( testValue0 );    
    inputScore.encoding.software.push_back( testValue1 );    
    auto documentId = DocumentManager::getInstance().createFromScore( inputScore );
    std::ostringstream oss;
    DocumentManager::getInstance().writeToStream( documentId, oss );
    DocumentManager::getInstance().destroyDocument( documentId );
    std::istringstream iss{ oss.str() };
    documentId = DocumentManager::getInstance().createFromStream( iss );
    outputScore = DocumentManager::getInstance().getData( documentId );
    DocumentManager::getInstance().destroyDocument( documentId );
    auto actualValue = outputScore.encoding.software.at( 0 );
    CHECK_EQUAL( testValue0, actualValue );
    actualValue = outputScore.encoding.software.at( 1 );
    CHECK_EQUAL( testValue1, actualValue );
}
T_END



TEST( RoundTrip_Software, DocumentManager )
{
    auto testValue0 = true;
    auto testValue1 = false;
    auto expectedStruct0 = SupportedItem{};
    expectedStruct0.isSupported = testValue0;
    auto expectedStruct1 = SupportedItem{};
    expectedStruct1.isSupported = testValue1;
    auto actualStruct = SupportedItem{};
    auto inputScore = ScoreData{};
    auto outputScore = ScoreData{};
    inputScore.encoding.supportedItems.push_back( expectedStruct0 );    
    inputScore.encoding.supportedItems.push_back( expectedStruct1 );    
    auto documentId = DocumentManager::getInstance().createFromScore( inputScore );
    std::ostringstream oss;
    DocumentManager::getInstance().writeToStream( documentId, oss );
    DocumentManager::getInstance().destroyDocument( documentId );
    std::istringstream iss{ oss.str() };
    documentId = DocumentManager::getInstance().createFromStream( iss );
    outputScore = DocumentManager::getInstance().getData( documentId );
    DocumentManager::getInstance().destroyDocument( documentId );
    auto actualValue = outputScore.encoding.supportedItems.at( 0 ).isSupported;
    CHECK_EQUAL( testValue0, actualValue );
    actualValue = outputScore.encoding.supportedItems.at( 1 ).isSupported;
    CHECK_EQUAL( testValue1, actualValue );
}
T_END



TEST( RoundTrip_PageTextData, DocumentManager )
{
    auto testValue0 = PageTextData{};
    testValue0.description = "descrip";
    testValue0.pageNumber = 2;
    testValue0.position.defaultX = -0.1;
    testValue0.position.hasDefaultX = true;
    testValue0.position.defaultY = 108.2;
    testValue0.position.hasDefaultY = true;
    testValue0.position.relativeX = 1.0;
    testValue0.position.hasRelativeX = true;
    testValue0.position.relativeY = 2.0;
    testValue0.position.hasRelativeY = true;
    testValue0.position.horizontalAlignmnet = HorizontalAlignment::left;
    testValue0.position.verticalAlignment = VerticalAlignment::bottom;
    testValue0.position.placement = Placement::above;
    testValue0.text = "my text";

    auto testValue1 = PageTextData{};
    auto inputScore = ScoreData{};
    auto outputScore = ScoreData{};
    
    inputScore.pageTextItems.push_back( testValue0 );
    inputScore.pageTextItems.push_back( testValue1 );    
    auto documentId = DocumentManager::getInstance().createFromScore( inputScore );
    std::ostringstream oss;
    DocumentManager::getInstance().writeToStream( documentId, oss );
    DocumentManager::getInstance().destroyDocument( documentId );
    std::istringstream iss{ oss.str() };
    documentId = DocumentManager::getInstance().createFromStream( iss );
    outputScore = DocumentManager::getInstance().getData( documentId );
    DocumentManager::getInstance().destroyDocument( documentId );
    
    auto actualValue = outputScore.pageTextItems.at( 0 );
    CHECK_EQUAL( testValue0.description, actualValue.description );
    CHECK_EQUAL( testValue0.pageNumber, actualValue.pageNumber );
    CHECK_EQUAL( testValue0.text, actualValue.text );
    CHECK_DOUBLES_EQUAL( testValue0.position.defaultX, actualValue.position.defaultX, MX_API_EQUALITY_EPSILON );
    CHECK_EQUAL( testValue0.position.hasDefaultX, actualValue.position.hasDefaultX );
    CHECK_DOUBLES_EQUAL( testValue0.position.defaultY, actualValue.position.defaultY, MX_API_EQUALITY_EPSILON );
    CHECK_EQUAL( testValue0.position.hasRelativeY, actualValue.position.hasRelativeY );
    CHECK_DOUBLES_EQUAL( testValue0.position.relativeX, actualValue.position.relativeX, MX_API_EQUALITY_EPSILON );
    CHECK_EQUAL( testValue0.position.hasRelativeX, actualValue.position.hasRelativeX );
    CHECK_DOUBLES_EQUAL( testValue0.position.relativeY, actualValue.position.relativeY, MX_API_EQUALITY_EPSILON );
    CHECK_EQUAL( testValue0.position.hasRelativeY, actualValue.position.hasRelativeY );
    
    actualValue = outputScore.pageTextItems.at( 1 );
    CHECK_EQUAL( testValue1.description, actualValue.description );
    CHECK_EQUAL( testValue1.pageNumber, actualValue.pageNumber );
    CHECK_EQUAL( testValue1.text, actualValue.text );
    CHECK_DOUBLES_EQUAL( testValue1.position.defaultX, actualValue.position.defaultX, MX_API_EQUALITY_EPSILON );
    CHECK_EQUAL( testValue1.position.hasDefaultX, actualValue.position.hasDefaultX );
    CHECK_DOUBLES_EQUAL( testValue1.position.defaultY, actualValue.position.defaultY, MX_API_EQUALITY_EPSILON );
    CHECK_EQUAL( testValue1.position.hasRelativeY, actualValue.position.hasRelativeY );
    CHECK_DOUBLES_EQUAL( testValue1.position.relativeX, actualValue.position.relativeX, MX_API_EQUALITY_EPSILON );
    CHECK_EQUAL( testValue1.position.hasRelativeX, actualValue.position.hasRelativeX );
    CHECK_DOUBLES_EQUAL( testValue1.position.relativeY, actualValue.position.relativeY, MX_API_EQUALITY_EPSILON );
    CHECK_EQUAL( testValue1.position.hasRelativeY, actualValue.position.hasRelativeY );

}
T_END

#endif

#endif
