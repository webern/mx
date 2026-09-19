// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/Diagnostics.h"
#include "mx/api/DirectionData.h"
#include "mx/api/MusicXml.h"
#include "mx/api/OttavaData.h"
#include "mx/api/ScoreData.h"
#include "mxtest/file/StupidFileFunctions.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace mx::api;

inline ScoreData diagnosticsScore(int measureCount)
{
    ScoreData score;
    score.ticksPerQuarter = 10;
    auto &part = score.parts.emplace_back();
    part.uniqueId = "P1";
    for (int i = 0; i < measureCount; ++i)
    {
        auto &measure = part.measures.emplace_back();
        auto &staff = measure.staves.emplace_back();
        auto &note = staff.voices[0].notes.emplace_back();
        note.durationData.durationTimeTicks = 10;
        note.durationData.durationName = DurationName::quarter;
    }
    return score;
}

inline void diagnosticsAddOttavaStop(ScoreData &score, int measureIndex, int tick)
{
    DirectionData direction;
    direction.tickTimePosition = tick;
    direction.directionTypes.emplace_back(DirectionChoice{OttavaStop{}});
    score.parts.front()
        .measures.at(static_cast<size_t>(measureIndex))
        .staves.front()
        .directions.emplace_back(direction);
}

inline std::string diagnosticsTimeSignatureXml()
{
    return R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list><score-part id="P1"><part-name>Music</part-name></score-part></part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <time number="3"><beats>3</beats><beat-type>4</beat-type></time>
        <staves>1</staves>
      </attributes>
      <note><rest/><duration>1</duration><voice>1</voice><type>quarter</type></note>
    </measure>
  </part>
</score-partwise>)";
}

inline std::string diagnosticsImportRepairXml()
{
    return R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list><score-part id="P1"><part-name>Music</part-name></score-part></part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <time number="3"><beats>3</beats><beat-type>4</beat-type></time>
        <staves>1</staves>
      </attributes>
      <note><rest/><duration>1</duration><voice>1</voice><type>banana</type></note>
    </measure>
  </part>
</score-partwise>)";
}

inline void diagnosticsCheckRethrowsHandlerFailure(const ApiError &error)
{
    CHECK(ResultCode::internalError == error.code);
    REQUIRE(error.cause);
    bool caughtExpected = false;
    try
    {
        std::rethrow_exception(error.cause);
    }
    catch (const std::runtime_error &cause)
    {
        caughtExpected = true;
        CHECK_EQUAL(std::string{"handler failed"}, std::string{cause.what()});
    }
    CHECK(caughtExpected);
}

TEST(diagnosticsCollectAndInvokeHandlerInOrder, Diagnostics)
{
    auto score = diagnosticsScore(2);
    diagnosticsAddOttavaStop(score, 0, 2);
    diagnosticsAddOttavaStop(score, 1, 7);

    std::vector<int> callbackMeasures;
    Diagnostics diagnostics{[&callbackMeasures](const Diagnostic &diagnostic) {
        callbackMeasures.push_back(diagnostic.location.measureIndex);
    }};
    const auto result = fromScore(score, diagnostics);

    REQUIRE(result.ok());
    REQUIRE(diagnostics.all().size() == 2);
    CHECK_EQUAL(0, diagnostics.all()[0].location.measureIndex);
    CHECK_EQUAL(1, diagnostics.all()[1].location.measureIndex);
    REQUIRE(callbackMeasures.size() == 2);
    CHECK_EQUAL(0, callbackMeasures[0]);
    CHECK_EQUAL(1, callbackMeasures[1]);
    CHECK(diagnostics.hasSeverityOrWorse(Severity::warning));
    CHECK(!diagnostics.hasSeverityOrWorse(Severity::error));
}

T_END

TEST(unmatchedOttavaDiagnosticHasScoreLocationAndKeepsOutput, Diagnostics)
{
    auto score = diagnosticsScore(2);
    diagnosticsAddOttavaStop(score, 1, 7);

    const auto silent = fromScore(score);
    Diagnostics diagnostics;
    const auto observed = fromScore(score, diagnostics);
    REQUIRE(silent.ok());
    REQUIRE(observed.ok());
    REQUIRE(diagnostics.all().size() == 1);

    const auto &diagnostic = diagnostics.all().front();
    CHECK(Severity::warning == diagnostic.severity);
    CHECK(DiagnosticCode::unmatchedSpanner == diagnostic.code);
    CHECK_EQUAL(0, diagnostic.location.partIndex);
    CHECK_EQUAL(1, diagnostic.location.measureIndex);
    CHECK_EQUAL(0, diagnostic.location.staffIndex);
    CHECK_EQUAL(-1, diagnostic.location.voiceIndex);
    CHECK_EQUAL(7, diagnostic.location.tickTimePosition);
    CHECK_EQUAL(std::string{"mx: warning at part=0 measure=1 staff=0 tick=7: "
                            "octave-shift stop has no matching start; using size 8"},
                formatDiagnostic(diagnostic));

    std::ostringstream silentXml;
    std::ostringstream observedXml;
    REQUIRE(silent.value().writeToStream(silentXml).ok());
    REQUIRE(observed.value().writeToStream(observedXml).ok());
    CHECK_EQUAL(silentXml.str(), observedXml.str());
}

T_END

TEST(outOfRangeTimeSignatureStaffReportsAdjustment, Diagnostics)
{
    std::istringstream stream{diagnosticsTimeSignatureXml()};
    auto document = MusicXml::fromStream(stream);
    REQUIRE(document.ok());

    Diagnostics diagnostics;
    const auto score = getScore(document.value(), diagnostics);
    REQUIRE(score.ok());
    REQUIRE(diagnostics.all().size() == 1);
    const auto &diagnostic = diagnostics.all().front();
    CHECK(DiagnosticCode::valueAdjusted == diagnostic.code);
    CHECK_EQUAL(0, diagnostic.location.partIndex);
    CHECK_EQUAL(0, diagnostic.location.measureIndex);
    CHECK_EQUAL(-1, diagnostic.location.staffIndex);
    CHECK(diagnostic.message.find("staff number 3") != std::string::npos);
    CHECK(score.value().parts.front().measures.front().staffTimeSignatures.empty());
    CHECK(!score.value().parts.front().measures.front().timeSignature.isImplicit);

    std::istringstream silentStream{diagnosticsTimeSignatureXml()};
    auto silentDocument = MusicXml::fromStream(silentStream);
    REQUIRE(silentDocument.ok());
    const auto silentScore = getScore(silentDocument.value());
    REQUIRE(silentScore.ok());
    CHECK(silentScore.value() == score.value());
}

T_END

TEST(intoScoreCollectsDiagnostics, Diagnostics)
{
    std::istringstream stream{diagnosticsTimeSignatureXml()};
    auto document = MusicXml::fromStream(stream);
    REQUIRE(document.ok());

    Diagnostics diagnostics;
    const auto score = intoScore(std::move(document).value(), diagnostics);
    REQUIRE(score.ok());
    REQUIRE(diagnostics.all().size() == 1);
    CHECK(DiagnosticCode::valueAdjusted == diagnostics.all().front().code);
}

T_END

TEST(diagnosticsMaySpanSequentialCalls, Diagnostics)
{
    auto score = diagnosticsScore(1);
    diagnosticsAddOttavaStop(score, 0, 0);
    Diagnostics diagnostics;
    REQUIRE(fromScore(score, diagnostics).ok());
    REQUIRE(fromScore(score, diagnostics).ok());
    CHECK_EQUAL(static_cast<size_t>(2), diagnostics.all().size());
}

T_END

TEST(throwingDiagnosticHandlerBecomesInternalError, Diagnostics)
{
    auto score = diagnosticsScore(1);
    diagnosticsAddOttavaStop(score, 0, 0);
    Diagnostics diagnostics{[](const Diagnostic &) { throw std::runtime_error{"handler failed"}; }};

    const auto result = fromScore(score, diagnostics);
    REQUIRE(!result.ok());
    diagnosticsCheckRethrowsHandlerFailure(result.error());
}

T_END

TEST(fromStreamReportsImportRepairs, Diagnostics)
{
    std::istringstream stream{diagnosticsImportRepairXml()};
    Diagnostics diagnostics;
    const auto document = MusicXml::fromStream(stream, diagnostics);
    REQUIRE(document.ok());
    REQUIRE(diagnostics.all().size() == 1);

    const auto &diagnostic = diagnostics.all().front();
    CHECK(Severity::warning == diagnostic.severity);
    CHECK(DiagnosticCode::invalidValue == diagnostic.code);
    CHECK_EQUAL(std::string{"mx: warning at /score-partwise/part/measure/note/type: "
                            "invalid value \"banana\"; using \"1024th\""},
                formatDiagnostic(diagnostic));
}

T_END

TEST(fromFileReportsImportRepairs, Diagnostics)
{
    const std::string path = mxtest::filePath(mxtest::TEST_OUTPUT_DIRECTORY_NAME, "DiagnosticsImportRepair.musicxml");
    {
        std::ofstream file{path};
        file << diagnosticsImportRepairXml();
    }
    Diagnostics diagnostics;
    const auto document = MusicXml::fromFile(path, diagnostics);
    mxtest::deleteFileNoThrow(path);
    REQUIRE(document.ok());
    REQUIRE(diagnostics.all().size() == 1);
    CHECK(DiagnosticCode::invalidValue == diagnostics.all().front().code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note/type"}, diagnostics.all().front().location.xmlPath);
}

T_END

TEST(oneCollectionSpansImportAndTranslation, Diagnostics)
{
    std::istringstream stream{diagnosticsImportRepairXml()};
    Diagnostics diagnostics;
    const auto document = MusicXml::fromStream(stream, diagnostics);
    REQUIRE(document.ok());
    REQUIRE(diagnostics.all().size() == 1);

    const auto score = getScore(document.value(), diagnostics);
    REQUIRE(score.ok());
    REQUIRE(diagnostics.all().size() == 2);
    CHECK(DiagnosticCode::invalidValue == diagnostics.all()[0].code);
    CHECK(DiagnosticCode::valueAdjusted == diagnostics.all()[1].code);
    CHECK_EQUAL(0, diagnostics.all()[1].location.measureIndex);
}

T_END

TEST(importWithoutDiagnosticsReadsTheSameDocument, Diagnostics)
{
    std::istringstream silentStream{diagnosticsImportRepairXml()};
    const auto silent = MusicXml::fromStream(silentStream);
    std::istringstream observedStream{diagnosticsImportRepairXml()};
    Diagnostics diagnostics;
    const auto observed = MusicXml::fromStream(observedStream, diagnostics);
    REQUIRE(silent.ok());
    REQUIRE(observed.ok());
    CHECK(!diagnostics.all().empty());

    std::ostringstream silentXml;
    std::ostringstream observedXml;
    REQUIRE(silent.value().writeToStream(silentXml).ok());
    REQUIRE(observed.value().writeToStream(observedXml).ok());
    CHECK_EQUAL(silentXml.str(), observedXml.str());
}

T_END

TEST(throwingDiagnosticHandlerDuringImportBecomesInternalError, Diagnostics)
{
    std::istringstream stream{diagnosticsImportRepairXml()};
    Diagnostics diagnostics{[](const Diagnostic &) { throw std::runtime_error{"handler failed"}; }};
    const auto result = MusicXml::fromStream(stream, diagnostics);
    REQUIRE(!result.ok());
    diagnosticsCheckRethrowsHandlerFailure(result.error());
}

T_END

TEST(tooManyConcurrentSpannersIsLocatedRefusal, Diagnostics)
{
    auto score = diagnosticsScore(2);
    auto &startDirections = score.parts.front().measures.front().staves.front().directions;
    auto &stopDirections = score.parts.front().measures.back().staves.front().directions;
    for (int i = 0; i < 17; ++i)
    {
        const auto identity = std::string{"ottava-"} + std::to_string(i);
        OttavaStart start;
        start.spannerStart.number = SpannerNumber{identity};
        DirectionData startDirection;
        startDirection.tickTimePosition = 3;
        startDirection.directionTypes.emplace_back(DirectionChoice{start});
        startDirections.emplace_back(std::move(startDirection));

        OttavaStop stop;
        stop.spannerStop.number = SpannerNumber{identity};
        DirectionData stopDirection;
        stopDirection.tickTimePosition = 6;
        stopDirection.directionTypes.emplace_back(DirectionChoice{stop});
        stopDirections.emplace_back(std::move(stopDirection));
    }

    const auto result = fromScore(score);
    REQUIRE(!result.ok());
    CHECK(ResultCode::tooManyElements == result.error().code);
    CHECK_EQUAL(0, result.error().location.partIndex);
    CHECK_EQUAL(0, result.error().location.measureIndex);
    CHECK_EQUAL(0, result.error().location.staffIndex);
    CHECK_EQUAL(3, result.error().location.tickTimePosition);
    CHECK(result.error().message.find("more than 16 spanners") != std::string::npos);
}

T_END

inline std::string diagnosticsIdXml(const std::string &partList, const std::string &partId)
{
    return R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list>)" +
           partList + R"(</part-list>
  <part id=")" +
           partId +
           R"(">
    <measure number="1"><attributes><divisions>1</divisions></attributes></measure>
  </part>
</score-partwise>
)";
}

inline std::string diagnosticsScorePart(const std::string &id)
{
    return "<score-part id=\"" + id + "\"><part-name>Music</part-name></score-part>";
}

TEST(fromStreamReportsDuplicateId, Diagnostics)
{
    std::istringstream stream{diagnosticsIdXml(diagnosticsScorePart("P1") + diagnosticsScorePart("P1"), "P1")};
    Diagnostics diagnostics;
    const auto document = MusicXml::fromStream(stream, diagnostics);
    REQUIRE(document.ok());
    REQUIRE(diagnostics.all().size() == 1);

    const auto &diagnostic = diagnostics.all().front();
    CHECK(Severity::warning == diagnostic.severity);
    CHECK(DiagnosticCode::duplicateId == diagnostic.code);
    CHECK_EQUAL(std::string{"/score-partwise/part-list/score-part[2]"}, diagnostic.location.xmlPath);
}

T_END

TEST(fromStreamReportsDanglingIdReference, Diagnostics)
{
    std::istringstream stream{diagnosticsIdXml(diagnosticsScorePart("P1"), "P9")};
    Diagnostics diagnostics;
    const auto document = MusicXml::fromStream(stream, diagnostics);
    REQUIRE(document.ok());
    REQUIRE(diagnostics.all().size() == 1);

    const auto &diagnostic = diagnostics.all().front();
    CHECK(Severity::warning == diagnostic.severity);
    CHECK(DiagnosticCode::danglingIdReference == diagnostic.code);
    CHECK_EQUAL(std::string{"/score-partwise/part"}, diagnostic.location.xmlPath);
}

T_END

TEST(writeToStreamRenamesDuplicateId, Diagnostics)
{
    // Reading keeps the document as it was written; writing it out cannot,
    // because a duplicate ID is not a MusicXML document.
    std::istringstream stream{diagnosticsIdXml(diagnosticsScorePart("P1") + diagnosticsScorePart("P1"), "P1")};
    const auto document = MusicXml::fromStream(stream);
    REQUIRE(document.ok());

    Diagnostics diagnostics;
    std::ostringstream written;
    REQUIRE(document.value().writeToStream(written, diagnostics).ok());
    REQUIRE(diagnostics.all().size() == 1);
    CHECK(DiagnosticCode::duplicateId == diagnostics.all().front().code);
    CHECK(written.str().find("id=\"P1-2\"") != std::string::npos);
}

T_END

TEST(authoredMidiPartWritesNoIdDiagnostics, Diagnostics)
{
    // A part that brings playback data but no instrument id gets one from the library, and every
    // element that refers to the instrument carries it, so the written document has nothing to
    // repair.
    auto score = diagnosticsScore(1);
    score.parts.front().instrumentData.midiData.name = "Flute";
    score.parts.front().instrumentData.midiData.channel = 1;

    const auto document = fromScore(score);
    REQUIRE(document.ok());

    Diagnostics diagnostics;
    std::ostringstream written;
    REQUIRE(document.value().writeToStream(written, diagnostics).ok());
    CHECK(diagnostics.all().empty());
}

T_END

#endif
