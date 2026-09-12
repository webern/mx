// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DefaultsData.h"
#include "mx/api/MusicXml.h"
#include "mx/core/Attribution.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/MarginType.h"
#include "mx/core/generated/PageLayout.h"
#include "mxtest/file/Path.h"

#include <new>
#include <sstream>

using namespace std;
using namespace mx::api;

inline ScoreData loadDichterliebe()
{
    auto docResult = MusicXml::fromFile(std::string{mxtest::getResourcesDirectoryPath()} +
                                        std::string{"/recsuite/Dichterliebe01.xml"});
    if (!docResult.ok())
        return ScoreData{};
    const auto scoreResult = getScore(std::move(docResult).value());
    if (!scoreResult.ok())
        return ScoreData{};
    return scoreResult.value();
}

// Serializes a ScoreData to a stream and parses it back, returning the
// reloaded ScoreData. Any failure along the api error channel fails the
// current test via REQUIRE rather than throwing across the boundary.
inline ScoreData roundTripScore(const ScoreData &input)
{
    auto createResult = fromScore(input);
    REQUIRE(createResult.ok());
    std::ostringstream oss;
    const auto writeResult = std::move(createResult).value().writeToStream(oss);
    REQUIRE(writeResult.ok());
    std::istringstream iss{oss.str()};
    auto reloadResult = MusicXml::fromStream(iss);
    REQUIRE(reloadResult.ok());
    const auto dataResult = getScore(std::move(reloadResult).value());
    REQUIRE(dataResult.ok());
    return dataResult.value();
}

// --- RAII ownership ---------------------------------------------------------
// A MusicXml owns its document. It cannot be copied (clone makes a deep
// copy), it moves, and a moved-from document is a valid, empty document.

TEST(moveTransfersOwnership, MusicXml)
{
    auto docResult = MusicXml::fromFile(std::string{mxtest::getResourcesDirectoryPath()} +
                                        std::string{"/recsuite/Dichterliebe01.xml"});
    REQUIRE(docResult.ok());
    MusicXml doc = std::move(docResult).value();

    // moving transfers the document to the destination
    MusicXml other = std::move(doc);
    const auto scoreResult = getScore(other);
    REQUIRE(scoreResult.ok());
    CHECK_EQUAL("Dichterliebe", scoreResult.value().workTitle);

    // the moved-from document is a valid, empty document, safe to read and write
    const auto movedScoreResult = getScore(doc);
    REQUIRE(movedScoreResult.ok());
    CHECK(movedScoreResult.value().workTitle.empty());
    std::stringstream ss;
    const auto movedWriteResult = doc.writeToStream(ss);
    REQUIRE(movedWriteResult.ok());
    CHECK(!ss.str().empty());
}

T_END

TEST(clone, MusicXml)
{
    auto input = ScoreData{};
    input.workTitle = "CloneTest";
    auto docResult = fromScore(input);
    REQUIRE(docResult.ok());
    const auto original = std::move(docResult).value();

    const auto cloned = original.clone();

    // the copy and the original are both usable and hold the same score
    const auto a = getScore(original);
    const auto b = getScore(cloned);
    REQUIRE(a.ok());
    REQUIRE(b.ok());
    CHECK_EQUAL("CloneTest", a.value().workTitle);
    CHECK_EQUAL("CloneTest", b.value().workTitle);

    // the copy is deep: writing it does not disturb the original
    auto edited = b.value();
    edited.workTitle = "Edited";
    auto rewritten = fromScore(edited);
    REQUIRE(rewritten.ok());
    std::stringstream ss;
    std::move(rewritten).value().writeToStream(ss);
    const auto reread = getScore(original);
    REQUIRE(reread.ok());
    CHECK_EQUAL("CloneTest", reread.value().workTitle);
}

T_END

// intoScore takes the document by value: the underlying tree is freed when
// the function returns, and the call site must spell std::move.
TEST(intoScoreConsumesDocument, MusicXml)
{
    auto docResult = MusicXml::fromFile(std::string{mxtest::getResourcesDirectoryPath()} +
                                        std::string{"/recsuite/Dichterliebe01.xml"});
    REQUIRE(docResult.ok());
    const auto scoreResult = intoScore(std::move(docResult).value());
    REQUIRE(scoreResult.ok());
    CHECK_EQUAL("Dichterliebe", scoreResult.value().workTitle);
}

T_END

// --- Golden metadata extraction ---------------------------------------------
// Pins the reader against a frozen reference file. The corpus survival tests
// only assert "loads without crashing"; these assert the actual values.

TEST(musicXmlType, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_EQUAL("partwise", score.musicXmlType);
}

T_END

TEST(workTitle, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_EQUAL("Dichterliebe", score.workTitle);
}

T_END

TEST(workNumber, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_EQUAL("Op. 48", score.workNumber);
}

T_END

TEST(movementTitle, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_EQUAL("Im wunderschönen Monat Mai", score.movementTitle);
}

T_END

TEST(movementNumber, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_EQUAL("1", score.movementNumber);
}

T_END

TEST(composerName, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_EQUAL("Robert Schumann", score.composer);
}

T_END

TEST(lyricistName, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_EQUAL("Heinrich Heine", score.lyricist);
}

T_END

TEST(copyright, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_EQUAL("Copyright © 2002 Recordare LLC", score.copyright);
}

T_END

TEST(RoundTrip_copyrightType_defaultIsCopyright, MusicXml)
{
    auto input = ScoreData{};
    input.copyright = "Public Domain";
    std::stringstream ss;
    auto createResult = fromScore(input);
    REQUIRE(createResult.ok());
    std::move(createResult).value().writeToStream(ss);
    CHECK(ss.str().find(R"(<rights type="copyright">Public Domain</rights>)") != std::string::npos);

    const auto output = roundTripScore(input);
    REQUIRE(output.copyrightType.has_value());
    CHECK_EQUAL(std::string{"copyright"}, *output.copyrightType);
}

T_END

// The reader only recognizes a <rights> typed "copyright" (or untyped) as the
// source of ScoreData::copyright; any other type value is out of scope for
// this simplified field, so only the write side is asserted here.
TEST(WriteHonorsExplicitCopyrightType, MusicXml)
{
    auto input = ScoreData{};
    input.copyright = "All rights reserved";
    input.copyrightType = std::string{"mechanical"};
    std::stringstream ss;
    auto createResult = fromScore(input);
    REQUIRE(createResult.ok());
    std::move(createResult).value().writeToStream(ss);
    CHECK(ss.str().find(R"(<rights type="mechanical">All rights reserved</rights>)") != std::string::npos);
}

T_END

TEST(RoundTrip_copyrightType_unsetOmitsAttribute, MusicXml)
{
    auto input = ScoreData{};
    input.copyright = "Public Domain";
    input.copyrightType = std::nullopt;
    std::stringstream ss;
    auto createResult = fromScore(input);
    REQUIRE(createResult.ok());
    std::move(createResult).value().writeToStream(ss);
    CHECK(ss.str().find(R"(<rights>Public Domain</rights>)") != std::string::npos);

    const auto output = roundTripScore(input);
    CHECK(!output.copyrightType.has_value());
}

T_END

TEST(scalingMillimeters, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_DOUBLES_EQUAL(6.35, score.defaults.scalingMillimeters, MX_API_EQUALITY_EPSILON)
}

T_END

TEST(scalingTenths, MusicXml)
{
    auto score = loadDichterliebe();
    CHECK_DOUBLES_EQUAL(40, score.defaults.scalingTenths, MX_API_EQUALITY_EPSILON)
}

T_END

// --- Header / encoding round-trips ------------------------------------------
// fromScore -> writeToStream -> fromStream -> getScore fidelity for the
// identification and encoding metadata fields.

TEST(RoundTrip_WorkTitle, MusicXml)
{
    const auto value = std::string{"value"};
    auto input = ScoreData{};
    input.workTitle = value;
    const auto output = roundTripScore(input);
    CHECK_EQUAL(value, output.workTitle);
}

T_END

#define ROUND_TRIP_TEST_SCALAR(scalarType, fieldPath, fieldName, value, nameSuffix)                                    \
    TEST(RoundTrip_##fieldName##_##nameSuffix, MusicXml)                                                               \
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
ROUND_TRIP_TEST_SCALAR(std::string, arranger, arranger, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, publisher, publisher, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, copyright, copyright, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, encoding.encoder, encoder, "value", 0);
ROUND_TRIP_TEST_SCALAR(std::string, encoding.encodingDescription, encodingDescription, "value", 0);
ROUND_TRIP_TEST_SCALAR(int, encoding.encodingDate.year, year, 2016, 0);
ROUND_TRIP_TEST_SCALAR(int, encoding.encodingDate.month, month, 9, 0);
ROUND_TRIP_TEST_SCALAR(int, encoding.encodingDate.day, day, 12, 0);

// The four <creator> types mx::api models are independent slots. The arranger used to be read
// into ScoreData::lyricist and rewritten as type="lyricist", so a file carrying both lost the
// lyricist; the publisher was dropped on the way out entirely.

TEST(RoundTrip_AllCreatorTypes, MusicXml)
{
    auto input = ScoreData{};
    input.composer = "MetaComposer";
    input.lyricist = "MetaLyricist";
    input.arranger = "MetaArranger";
    input.publisher = "MetaPublisher";

    std::ostringstream oss;
    auto createResult = fromScore(input);
    REQUIRE(createResult.ok());
    const auto writeResult = std::move(createResult).value().writeToStream(oss);
    REQUIRE(writeResult.ok());

    const auto xml = oss.str();
    CHECK(xml.find("<creator type=\"composer\">MetaComposer</creator>") != std::string::npos);
    CHECK(xml.find("<creator type=\"lyricist\">MetaLyricist</creator>") != std::string::npos);
    CHECK(xml.find("<creator type=\"arranger\">MetaArranger</creator>") != std::string::npos);
    CHECK(xml.find("<creator type=\"publisher\">MetaPublisher</creator>") != std::string::npos);

    const auto output = roundTripScore(input);
    CHECK_EQUAL("MetaComposer", output.composer);
    CHECK_EQUAL("MetaLyricist", output.lyricist);
    CHECK_EQUAL("MetaArranger", output.arranger);
    CHECK_EQUAL("MetaPublisher", output.publisher);
}

T_END

// Reading the same shape from a real file: musuite/testMetaData.xml carries an arranger, a
// composer and a lyricist, plus creator types mx::api does not model, which must not disturb
// the ones it does.

TEST(ReadAllCreatorTypes, MusicXml)
{
    auto docResult =
        MusicXml::fromFile(std::string{mxtest::getResourcesDirectoryPath()} + std::string{"/musuite/testMetaData.xml"});
    REQUIRE(docResult.ok());
    const auto dataResult = getScore(std::move(docResult).value());
    REQUIRE(dataResult.ok());
    const auto score = dataResult.value();

    CHECK_EQUAL("MetaComposer", score.composer);
    CHECK_EQUAL("MetaLyricist", score.lyricist);
    CHECK_EQUAL("MetaArranger", score.arranger);
    CHECK_EQUAL("", score.publisher);
}

T_END

// --- Page margin coalescing -------------------------------------------------
// Equal odd/even margins collapse to a single <page-margins type="both">;
// unequal margins emit separate odd and even entries. This rule is exercised
// nowhere else.

TEST(Layout_PageMarginsBoth, MusicXml)
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
    auto docResult = fromScore(score);
    REQUIRE(docResult.ok());
    const MusicXml doc = std::move(docResult).value();
    const auto &mxDoc = doc.getCoreDocument();
    REQUIRE(mxDoc.isScorePartwise());
    const auto &defaults = mxDoc.asScorePartwise().scoreHeader().defaults();
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
}

T_END

TEST(Layout_PageMarginsEvenOdd, MusicXml)
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
    auto docResult = fromScore(score);
    REQUIRE(docResult.ok());
    const MusicXml doc = std::move(docResult).value();
    const auto &mxDoc = doc.getCoreDocument();
    REQUIRE(mxDoc.isScorePartwise());
    const auto &defaults = mxDoc.asScorePartwise().scoreHeader().defaults();
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
}

T_END

// --- Encoding <supports> round-trips ----------------------------------------
// The only coverage of the <supports> element: no other unit test touches it,
// and none of the corpus files that use it are in the api-roundtrip baseline.

TEST(RoundTrip_SupportedItems_elementName, MusicXml)
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

TEST(RoundTrip_SupportedItems_attributeName, MusicXml)
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

TEST(RoundTrip_SupportedItems_specificValue, MusicXml)
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

TEST(RoundTrip_SupportedItems_software, MusicXml)
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

TEST(RoundTrip_SupportedItems_isSupported, MusicXml)
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

// --- writeMxVersion: suppressing mx's provenance stamp ----------------------
// mx stamps its own <software> (text begins with kMxSoftwareMarker) into
// <encoding> on every api write. EncodingData::writeMxVersion defaults true --
// even for files parsed without the stamp -- but the caller may set it false to
// suppress mx's node while still emitting the user's own <software>.

// Serialize a ScoreData to a string via the api write path (no reload).
inline std::string writeScoreToString(const ScoreData &input)
{
    auto createResult = fromScore(input);
    REQUIRE(createResult.ok());
    std::ostringstream oss;
    const auto writeResult = std::move(createResult).value().writeToStream(oss);
    REQUIRE(writeResult.ok());
    return oss.str();
}

TEST(writeMxVersion_defaultsTrueAndStamps, MusicXml)
{
    // Parsed from a real file that never carried mx's stamp; the flag still
    // defaults true, so the written output gains the stamp.
    ScoreData score = loadDichterliebe();
    CHECK(score.encoding.writeMxVersion);
    const std::string xml = writeScoreToString(score);
    CHECK(xml.find(std::string{mx::core::kMxSoftwareMarker}) != std::string::npos);
}

T_END

TEST(writeMxVersion_offSuppressesStamp, MusicXml)
{
    // Turn the stamp off after parsing: the written output must not contain it.
    ScoreData score = loadDichterliebe();
    score.encoding.writeMxVersion = false;
    const std::string xml = writeScoreToString(score);
    CHECK(xml.find(std::string{mx::core::kMxSoftwareMarker}) == std::string::npos);
}

T_END

// --- Error reporting -------------------------------------------------------
// The api never lets an exception through; it returns errors. These pin what
// an error carries: the reason, the place in the score, and, for caught
// exceptions, the exception itself.

// A ninth beam is more than the core model holds, so fromScore refuses it.
// The refusal was discovered while walking the score, so it says which note
// was the problem, not just that a limit exists somewhere.
TEST(tooManyElementsCarriesThePlaceInTheScore, MusicXml)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &note = measure.staves.back().voices[0].notes.emplace_back();
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 4;
    for (int i = 0; i < 9; ++i)
    {
        note.beams.push_back(Beam::extend);
    }

    const auto result = fromScore(score);
    REQUIRE(!result.ok());
    const auto &error = result.error();
    CHECK(ResultCode::tooManyElements == error.code);
    CHECK_EQUAL(0, error.location.partIndex);
    CHECK_EQUAL(0, error.location.measureIndex);
    CHECK_EQUAL(0, error.location.staffIndex);
    CHECK_EQUAL(0, error.location.voiceIndex);
    CHECK_EQUAL(0, error.location.tickTimePosition);
    CHECK(error.message.find("at most 8 beam occurrences") != std::string::npos);
    CHECK(formatError(error).find("at part=0 measure=0 staff=0 voice=0 tick=0") != std::string::npos);

    // a refusal is a choice mx made, not a caught exception
    CHECK(!error.cause);
}

T_END

// A stream that fails mid-read the way a broken file or connection might.
// Pugixml asks a stream where it is and how long it is before reading, so
// the stream must look seekable; every read then throws.
class ThrowingStreambuf : public std::streambuf
{
  public:
    explicit ThrowingStreambuf(std::exception_ptr inToThrow) : myToThrow{inToThrow}, myPosition{0}
    {
    }

  protected:
    pos_type seekoff(off_type offset, std::ios_base::seekdir direction, std::ios_base::openmode) override
    {
        if (direction == std::ios_base::beg)
        {
            myPosition = offset;
        }
        else if (direction == std::ios_base::end)
        {
            myPosition = size() + offset;
        }
        else
        {
            myPosition += offset;
        }
        return pos_type{myPosition};
    }

    pos_type seekpos(pos_type position, std::ios_base::openmode) override
    {
        myPosition = position;
        return pos_type{myPosition};
    }

    std::streamsize xsgetn(char_type *, std::streamsize) override
    {
        std::rethrow_exception(myToThrow);
    }

    int_type underflow() override
    {
        std::rethrow_exception(myToThrow);
    }

  private:
    std::streamsize size() const
    {
        return 16;
    }

    std::exception_ptr myToThrow;
    std::streampos myPosition;
};

// A std::bad_alloc that reaches the boundary is reported as outOfMemory.
// Real exhaustion is not practical to cause in a test; what matters is the
// mapping, and a bad_alloc thrown mid-parse exercises it exactly.
TEST(outOfMemoryIsReportedNotThrown, MusicXml)
{
    ThrowingStreambuf buf{std::make_exception_ptr(std::bad_alloc{})};
    std::istream stream{&buf};
    // std::istream catches exceptions thrown by its streambuf, sets badbit,
    // and only rethrows if badbit is in the exception mask
    stream.exceptions(std::ios_base::badbit);
    const auto result = MusicXml::fromStream(stream);

    REQUIRE(!result.ok());
    CHECK_EQUAL(ResultCode::outOfMemory, result.error().code);

    // the exception itself came through, not just a message about it
    REQUIRE(result.error().cause);
    bool caughtBadAlloc = false;
    try
    {
        std::rethrow_exception(result.error().cause);
    }
    catch (const std::bad_alloc &)
    {
        caughtBadAlloc = true;
    }
    CHECK(caughtBadAlloc);
}

T_END

// A stream that throws while being read is an unexpected exception: the
// boundary keeps it out of the caller's face, reports it as internalError,
// and keeps the exception in `cause` for whoever wants to look closer.
TEST(internalErrorKeepsTheException, MusicXml)
{
    ThrowingStreambuf buf{std::make_exception_ptr(std::runtime_error{"the stream failed mid-read"})};
    std::istream stream{&buf};
    stream.exceptions(std::ios_base::badbit);
    const auto result = MusicXml::fromStream(stream);

    REQUIRE(!result.ok());
    CHECK_EQUAL(ResultCode::internalError, result.error().code);

    REQUIRE(result.error().cause);
    bool caughtTheStreamsException = false;
    std::string what;
    try
    {
        std::rethrow_exception(result.error().cause);
    }
    catch (const std::exception &e)
    {
        caughtTheStreamsException = true;
        what = e.what();
    }
    CHECK(caughtTheStreamsException);
    CHECK(!what.empty());
}

T_END

#endif
