// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DefaultsData.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/MarginType.h"
#include "mx/core/generated/PageLayout.h"
#include "mxtest/file/Path.h"

using namespace std;
using namespace mx::api;

inline int loadDoc()
{
    auto &docMngr = DocumentManager::getInstance();
    const auto r = docMngr.createFromFile(std::string{mxtest::getResourcesDirectoryPath()} +
                                          std::string{"/recsuite/Dichterliebe01.xml"});
    return r.ok() ? r.value() : -1;
}

inline void destroyDoc(int documentId)
{
    auto &docMngr = DocumentManager::getInstance();
    docMngr.destroyDocument(documentId);
}

inline ScoreData getScore()
{
    const int documentId = loadDoc();
    const auto r = DocumentManager::getInstance().getData(documentId);
    destroyDoc(documentId);
    return r.ok() ? r.value() : ScoreData{};
}

// Serializes a ScoreData to a stream and parses it back, returning the
// reloaded ScoreData. Any failure along the api error channel fails the
// current test via REQUIRE rather than throwing across the boundary.
inline ScoreData roundTripScore(const ScoreData &input)
{
    auto &docMngr = DocumentManager::getInstance();
    const auto createResult = docMngr.createFromScore(input);
    REQUIRE(createResult.ok());
    const int writeId = createResult.value();
    std::ostringstream oss;
    const auto writeResult = docMngr.writeToStream(writeId, oss);
    REQUIRE(writeResult.ok());
    docMngr.destroyDocument(writeId);
    std::istringstream iss{oss.str()};
    const auto reloadResult = docMngr.createFromStream(iss);
    REQUIRE(reloadResult.ok());
    const int readId = reloadResult.value();
    const auto dataResult = docMngr.getData(readId);
    REQUIRE(dataResult.ok());
    auto output = dataResult.value();
    docMngr.destroyDocument(readId);
    return output;
}

// --- Document handle lifecycle ----------------------------------------------
// The DocumentManager registry contract: a live id yields a document, and
// destroying it makes the id resolve to nullptr. Covered nowhere else.

TEST(createFromFile, DocumentManager)
{
    auto &docMngr = DocumentManager::getInstance();
    const auto createResult = docMngr.createFromFile(std::string{mxtest::getResourcesDirectoryPath()} +
                                                     std::string{"/recsuite/Dichterliebe01.xml"});
    REQUIRE(createResult.ok());
    const int documentId = createResult.value();
    CHECK(documentId > 0);

    auto mxdocPtr = docMngr.getDocument(documentId);
    CHECK(mxdocPtr != nullptr);
    docMngr.destroyDocument(documentId);

    auto shouldBeNull = docMngr.getDocument(documentId);
    CHECK(shouldBeNull == nullptr);
}

T_END

// --- Golden metadata extraction ---------------------------------------------
// Pins the reader against a frozen reference file. The corpus survival tests
// only assert "loads without crashing"; these assert the actual values.

TEST(musicXmlType, DocumentManager)
{
    auto score = getScore();
    CHECK_EQUAL("partwise", score.musicXmlType);
}

T_END

TEST(workTitle, DocumentManager)
{
    auto score = getScore();
    CHECK_EQUAL("Dichterliebe", score.workTitle);
}

T_END

TEST(workNumber, DocumentManager)
{
    auto score = getScore();
    CHECK_EQUAL("Op. 48", score.workNumber);
}

T_END

TEST(movementTitle, DocumentManager)
{
    auto score = getScore();
    CHECK_EQUAL("Im wunderschönen Monat Mai", score.movementTitle);
}

T_END

TEST(movementNumber, DocumentManager)
{
    auto score = getScore();
    CHECK_EQUAL("1", score.movementNumber);
}

T_END

TEST(composerName, DocumentManager)
{
    auto score = getScore();
    CHECK_EQUAL("Robert Schumann", score.composer);
}

T_END

TEST(lyricistName, DocumentManager)
{
    auto score = getScore();
    CHECK_EQUAL("Heinrich Heine", score.lyricist);
}

T_END

TEST(copyright, DocumentManager)
{
    auto score = getScore();
    CHECK_EQUAL("Copyright © 2002 Recordare LLC", score.copyright);
}

T_END

TEST(scalingMillimeters, DocumentManager)
{
    auto score = getScore();
    CHECK_DOUBLES_EQUAL(6.35, score.defaults.scalingMillimeters, MX_API_EQUALITY_EPSILON)
}

T_END

TEST(scalingTenths, DocumentManager)
{
    auto score = getScore();
    CHECK_DOUBLES_EQUAL(40, score.defaults.scalingTenths, MX_API_EQUALITY_EPSILON)
}

T_END

// --- Header / encoding round-trips ------------------------------------------
// createFromScore -> writeToStream -> createFromStream -> getData fidelity for
// the identification and encoding metadata fields.

TEST(RoundTrip_WorkTitle, DocumentManager)
{
    const auto value = std::string{"value"};
    auto input = ScoreData{};
    input.workTitle = value;
    const auto output = roundTripScore(input);
    CHECK_EQUAL(value, output.workTitle);
}

T_END

#define ROUND_TRIP_TEST_SCALAR(scalarType, fieldPath, fieldName, value, nameSuffix)                                    \
    TEST(RoundTrip_##fieldName##_##nameSuffix, DocumentManager)                                                        \
    {                                                                                                                  \
        const auto testValue = scalarType{value};                                                                      \
        auto input = ScoreData{};                                                                                      \
        input.fieldPath = testValue;                                                                                   \
        const auto output = roundTripScore(input);                                                                     \
        CHECK_EQUAL(testValue, output.fieldPath);                                                                      \
    }                                                                                                                  \
    T_END

ROUND_TRIP_TEST_SCALAR(std::string, musicXmlType, musicXmlType, "timewise", 0);
ROUND_TRIP_TEST_SCALAR(std::string, musicXmlType, musicXmlType, "partwise", 1);
ROUND_TRIP_TEST_SCALAR(std::string, workTitle, workTitle, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, workNumber, workNumber, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, movementTitle, movementTitle, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, movementNumber, movementNumber, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, composer, composer, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, lyricist, lyricist, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, copyright, copyright, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, encoding.encoder, encoder, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, encoding.encodingDescription, encodingDescription, "value", 0);
ROUND_TRIP_TEST_SCALAR(int, encoding.encodingDate.year, year, 2016, 0);
ROUND_TRIP_TEST_SCALAR(int, encoding.encodingDate.month, month, 9, 0);
ROUND_TRIP_TEST_SCALAR(int, encoding.encodingDate.day, day, 12, 0);

// --- Page margin coalescing -------------------------------------------------
// Equal odd/even margins collapse to a single <page-margins type="both">;
// unequal margins emit separate odd and even entries. This rule is exercised
// nowhere else.

TEST(Layout_PageMarginsBoth, DocumentManager)
{
    auto score = ScoreData{};
    const long double left = 0.1;
    const long double right = 0.2;
    const long double top = 0.3;
    const long double bottom = 0.4;
    score.defaults.pageLayout.margins.odd = MarginsData{};
    score.defaults.pageLayout.margins.even = MarginsData{};
    score.defaults.pageLayout.margins.odd.value().left = left;
    score.defaults.pageLayout.margins.even.value().left = left;
    score.defaults.pageLayout.margins.odd.value().right = right;
    score.defaults.pageLayout.margins.even.value().right = right;
    score.defaults.pageLayout.margins.odd.value().top = top;
    score.defaults.pageLayout.margins.even.value().top = top;
    score.defaults.pageLayout.margins.odd.value().bottom = bottom;
    score.defaults.pageLayout.margins.even.value().bottom = bottom;
    const auto rDocId = DocumentManager::getInstance().createFromScore(score);
    REQUIRE(rDocId.ok());
    const int docId = rDocId.value();
    auto mxDoc = DocumentManager::getInstance().getDocument(docId);
    REQUIRE(mxDoc != nullptr);
    REQUIRE(mxDoc->isScorePartwise());
    const auto &defaults = mxDoc->asScorePartwise().scoreHeader().defaults();
    REQUIRE(defaults.has_value());
    const auto &pageLayout = defaults->layout().pageLayout();
    REQUIRE(pageLayout.has_value());
    const auto &pageMarginsSpan = pageLayout->pageMargins();
    CHECK_EQUAL(1, pageMarginsSpan.size());
    if (!pageMarginsSpan.empty())
    {
        REQUIRE(pageMarginsSpan[0].type().has_value());
        CHECK(mx::core::MarginType::Tag::both == pageMarginsSpan[0].type()->tag());
    }
    DocumentManager::getInstance().destroyDocument(docId);
}

T_END

TEST(Layout_PageMarginsEvenOdd, DocumentManager)
{
    auto score = ScoreData{};
    const long double left = 0.1;
    const long double right = 0.2;
    const long double top = 0.3;
    const long double bottom = 0.4;
    score.defaults.pageLayout.margins.odd = MarginsData{};
    score.defaults.pageLayout.margins.even = MarginsData{};
    score.defaults.pageLayout.margins.odd.value().left = left + 100.0;
    score.defaults.pageLayout.margins.even.value().left = left;
    score.defaults.pageLayout.margins.odd.value().right = right;
    score.defaults.pageLayout.margins.even.value().right = right;
    score.defaults.pageLayout.margins.odd.value().top = top;
    score.defaults.pageLayout.margins.even.value().top = top;
    score.defaults.pageLayout.margins.odd.value().bottom = bottom;
    score.defaults.pageLayout.margins.even.value().bottom = bottom;
    const auto rDocId = DocumentManager::getInstance().createFromScore(score);
    REQUIRE(rDocId.ok());
    const int docId = rDocId.value();
    auto mxDoc = DocumentManager::getInstance().getDocument(docId);
    REQUIRE(mxDoc != nullptr);
    REQUIRE(mxDoc->isScorePartwise());
    const auto &defaults = mxDoc->asScorePartwise().scoreHeader().defaults();
    REQUIRE(defaults.has_value());
    const auto &pageLayout = defaults->layout().pageLayout();
    REQUIRE(pageLayout.has_value());
    const auto &pageMarginsSpan = pageLayout->pageMargins();
    CHECK_EQUAL(2, pageMarginsSpan.size());
    if (pageMarginsSpan.size() >= 2)
    {
        REQUIRE(pageMarginsSpan[0].type().has_value());
        CHECK(mx::core::MarginType::Tag::odd == pageMarginsSpan[0].type()->tag());
        REQUIRE(pageMarginsSpan[1].type().has_value());
        CHECK(mx::core::MarginType::Tag::even == pageMarginsSpan[1].type()->tag());
    }
    DocumentManager::getInstance().destroyDocument(docId);
}

T_END

// --- Encoding <supports> round-trips ----------------------------------------
// The only coverage of the <supports> element: no other unit test touches it,
// and none of the corpus files that use it are in the api-roundtrip baseline.

TEST(RoundTrip_SupportedItems_elementName, DocumentManager)
{
    const auto testValue0 = std::string{"value0"};
    const auto testValue1 = std::string{"value1"};
    auto expectedStruct0 = SupportedItem{};
    expectedStruct0.elementName = testValue0;
    auto expectedStruct1 = SupportedItem{};
    expectedStruct1.elementName = testValue1;
    auto inputScore = ScoreData{};
    inputScore.encoding.supportedItems.push_back(expectedStruct0);
    inputScore.encoding.supportedItems.push_back(expectedStruct1);
    const auto outputScore = roundTripScore(inputScore);
    REQUIRE(outputScore.encoding.supportedItems.size() >= 2);
    CHECK_EQUAL(testValue0, outputScore.encoding.supportedItems.at(0).elementName);
    CHECK_EQUAL(testValue1, outputScore.encoding.supportedItems.at(1).elementName);
}

T_END

TEST(RoundTrip_SupportedItems_attributeName, DocumentManager)
{
    const auto testValue0 = std::string{"value0"};
    const auto testValue1 = std::string{"value1"};
    auto expectedStruct0 = SupportedItem{};
    expectedStruct0.elementName = "note";
    expectedStruct0.attributeName = testValue0;
    auto expectedStruct1 = SupportedItem{};
    expectedStruct1.elementName = "note";
    expectedStruct1.attributeName = testValue1;
    auto inputScore = ScoreData{};
    inputScore.encoding.supportedItems.push_back(expectedStruct0);
    inputScore.encoding.supportedItems.push_back(expectedStruct1);
    const auto outputScore = roundTripScore(inputScore);
    REQUIRE(outputScore.encoding.supportedItems.size() >= 2);
    CHECK_EQUAL(testValue0, outputScore.encoding.supportedItems.at(0).attributeName);
    CHECK_EQUAL(testValue1, outputScore.encoding.supportedItems.at(1).attributeName);
}

T_END

TEST(RoundTrip_SupportedItems_specificValue, DocumentManager)
{
    const auto testValue0 = std::string{"value0"};
    const auto testValue1 = std::string{"value1"};
    auto expectedStruct0 = SupportedItem{};
    expectedStruct0.elementName = "note";
    expectedStruct0.attributeName = "type";
    expectedStruct0.specificValue = testValue0;
    auto expectedStruct1 = SupportedItem{};
    expectedStruct1.elementName = "note";
    expectedStruct1.attributeName = "type";
    expectedStruct1.specificValue = testValue1;
    auto inputScore = ScoreData{};
    inputScore.encoding.supportedItems.push_back(expectedStruct0);
    inputScore.encoding.supportedItems.push_back(expectedStruct1);
    const auto outputScore = roundTripScore(inputScore);
    REQUIRE(outputScore.encoding.supportedItems.size() >= 2);
    CHECK_EQUAL(testValue0, outputScore.encoding.supportedItems.at(0).specificValue);
    CHECK_EQUAL(testValue1, outputScore.encoding.supportedItems.at(1).specificValue);
}

T_END

TEST(RoundTrip_SupportedItems_software, DocumentManager)
{
    const auto testValue0 = std::string{"value0"};
    const auto testValue1 = std::string{"value1"};
    auto inputScore = ScoreData{};
    inputScore.encoding.software.push_back(testValue0);
    inputScore.encoding.software.push_back(testValue1);
    const auto outputScore = roundTripScore(inputScore);
    REQUIRE(outputScore.encoding.software.size() >= 2);
    CHECK_EQUAL(testValue0, outputScore.encoding.software.at(0));
    CHECK_EQUAL(testValue1, outputScore.encoding.software.at(1));
}

T_END

TEST(RoundTrip_SupportedItems_isSupported, DocumentManager)
{
    const auto testValue0 = true;
    const auto testValue1 = false;
    auto expectedStruct0 = SupportedItem{};
    expectedStruct0.elementName = "note";
    expectedStruct0.isSupported = testValue0;
    auto expectedStruct1 = SupportedItem{};
    expectedStruct1.elementName = "beam";
    expectedStruct1.isSupported = testValue1;
    auto inputScore = ScoreData{};
    inputScore.encoding.supportedItems.push_back(expectedStruct0);
    inputScore.encoding.supportedItems.push_back(expectedStruct1);
    const auto outputScore = roundTripScore(inputScore);
    REQUIRE(outputScore.encoding.supportedItems.size() >= 2);
    CHECK_EQUAL(testValue0, outputScore.encoding.supportedItems.at(0).isSupported);
    CHECK_EQUAL(testValue1, outputScore.encoding.supportedItems.at(1).isSupported);
}

T_END

#endif
