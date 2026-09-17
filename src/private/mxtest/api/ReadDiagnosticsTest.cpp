// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// getScore recoveries reported through Diagnostics. Each read produces the same
// score with or without a collection.

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/Diagnostics.h"
#include "mx/api/MusicXml.h"
#include "mx/api/ScoreData.h"

#include <sstream>
#include <string>

using namespace mx::api;

struct ReadDiagnosticsResult
{
    bool ok = false;
    ScoreData score;
    Diagnostics diagnostics;
};

inline ReadDiagnosticsResult readDiagnostics(const std::string &xml)
{
    ReadDiagnosticsResult out;
    std::istringstream stream{xml};
    auto document = MusicXml::fromStream(stream);
    REQUIRE(document.ok());
    auto score = getScore(document.value(), out.diagnostics);
    out.ok = score.ok();
    if (score.ok())
    {
        out.score = std::move(score).value();
        const auto silent = getScore(document.value());
        REQUIRE(silent.ok());
        CHECK(silent.value() == out.score);
    }
    return out;
}

inline std::string readDiagnosticsPart(const std::string &id, const std::string &measureContent)
{
    return "<part id=\"" + id + "\"><measure number=\"1\">" + measureContent + "</measure></part>";
}

inline std::string readDiagnosticsScorePart(const std::string &id)
{
    return "<score-part id=\"" + id + "\"><part-name>" + id + "</part-name></score-part>";
}

inline std::string readDiagnosticsScore(const std::string &partList, const std::string &parts)
{
    return "<score-partwise version=\"4.0\"><part-list>" + partList + "</part-list>" + parts + "</score-partwise>";
}

inline std::string readDiagnosticsRest()
{
    return "<note><rest/><duration>1</duration></note>";
}

inline void readDiagnosticsCheck(const Diagnostic &diagnostic, DiagnosticCode code, const std::string &message)
{
    const Severity severity = code == DiagnosticCode::droppedData ? Severity::error : Severity::warning;
    CHECK(severity == diagnostic.severity);
    CHECK(code == diagnostic.code);
    CHECK_EQUAL(message, diagnostic.message);
}

TEST(unmatchedPartAndScorePartAreDropped, ReadDiagnostics)
{
    const std::string divisions = "<attributes><divisions>1</divisions></attributes>";
    const auto read =
        readDiagnostics(readDiagnosticsScore(readDiagnosticsScorePart("P1") + readDiagnosticsScorePart("P2"),
                                             readDiagnosticsPart("P1", divisions + readDiagnosticsRest()) +
                                                 readDiagnosticsPart("P3", divisions + readDiagnosticsRest())));
    REQUIRE(read.ok);
    REQUIRE(read.score.parts.size() == 1);
    REQUIRE(read.diagnostics.all().size() == 2);
    readDiagnosticsCheck(read.diagnostics.all()[0], DiagnosticCode::droppedData,
                         "score-part \"P2\" has no part with the same id; it is not read");
    readDiagnosticsCheck(read.diagnostics.all()[1], DiagnosticCode::droppedData,
                         "part \"P3\" matches no score-part; it is not read");
}

T_END

TEST(unbalancedPartGroupsAreReported, ReadDiagnostics)
{
    const auto read = readDiagnostics(readDiagnosticsScore(
        "<part-group type=\"stop\" number=\"2\"/><part-group type=\"start\" number=\"1\"/>" +
            readDiagnosticsScorePart("P1") + "<part-group type=\"start\" number=\"3\"/>" +
            readDiagnosticsScorePart("P2") + "<part-group type=\"stop\" number=\"1\"/>",
        readDiagnosticsPart("P1", readDiagnosticsRest()) + readDiagnosticsPart("P2", readDiagnosticsRest())));
    REQUIRE(read.ok);
    REQUIRE(read.score.partGroups.size() == 1);
    CHECK_EQUAL(0, read.score.partGroups.front().firstPartIndex);
    CHECK_EQUAL(1, read.score.partGroups.front().lastPartIndex);
    REQUIRE(read.diagnostics.all().size() == 2);
    readDiagnosticsCheck(read.diagnostics.all()[0], DiagnosticCode::unmatchedSpanner,
                         "part-group stop number 2 has no matching start; it is ignored");
    readDiagnosticsCheck(read.diagnostics.all()[1], DiagnosticCode::droppedData,
                         "part-group start number 3 has no stop; it is not read");
    CHECK_EQUAL(1, read.diagnostics.all()[1].location.partIndex);
}

T_END

TEST(nonNumericGroupNumberAndClefNumberAboveStavesRead, ReadDiagnostics)
{
    const auto read = readDiagnostics(readDiagnosticsScore(
        "<part-group type=\"start\" number=\"a\"/>" + readDiagnosticsScorePart("P1") +
            "<part-group type=\"stop\" number=\"a\"/>",
        readDiagnosticsPart("P1", "<attributes><divisions>1</divisions><clef number=\"2\"><sign>F</sign>"
                                  "<line>4</line></clef></attributes>" +
                                      readDiagnosticsRest())));
    REQUIRE(read.ok);
    REQUIRE(read.score.partGroups.size() == 1);
    CHECK_EQUAL(NUMBER_LEVEL_UNSPECIFIED, read.score.partGroups.front().number);
    const auto &measure = read.score.parts.front().measures.front();
    REQUIRE(measure.staves.size() == 2);
    CHECK(measure.staves[0].clefs.empty());
    CHECK_EQUAL(static_cast<size_t>(1), measure.staves[1].clefs.size());

    REQUIRE(read.diagnostics.all().size() == 3);
    readDiagnosticsCheck(read.diagnostics.all()[0], DiagnosticCode::invalidValue,
                         "part-group number \"a\" is not a number; reading it as unspecified");
    readDiagnosticsCheck(read.diagnostics.all()[1], DiagnosticCode::invalidValue,
                         "part-group number \"a\" is not a number; reading it as unspecified");
    readDiagnosticsCheck(read.diagnostics.all()[2], DiagnosticCode::valueAdjusted,
                         "staff number 2 is above the staff count 1; reading the part with 2 staves");
    CHECK_EQUAL(0, read.diagnostics.all()[2].location.partIndex);
}

T_END

TEST(outOfRangeStaffAndClampedBackupHaveScorePositions, ReadDiagnostics)
{
    const auto read = readDiagnostics(readDiagnosticsScore(
        readDiagnosticsScorePart("P1"),
        readDiagnosticsPart("P1", "<attributes><divisions>1</divisions></attributes><note><pitch><step>C</step>"
                                  "<octave>4</octave></pitch><duration>1</duration><staff>0</staff></note>"
                                  "<backup><duration>2</duration></backup>" +
                                      readDiagnosticsRest())));
    REQUIRE(read.ok);
    REQUIRE(read.diagnostics.all().size() == 2);
    const auto &staff = read.diagnostics.all()[0];
    readDiagnosticsCheck(staff, DiagnosticCode::valueAdjusted, "note staff number 0 is out of range; using staff 1");
    CHECK_EQUAL(0, staff.location.partIndex);
    CHECK_EQUAL(0, staff.location.measureIndex);
    CHECK_EQUAL(0, staff.location.staffIndex);
    CHECK_EQUAL(0, staff.location.tickTimePosition);
    const auto &backup = read.diagnostics.all()[1];
    readDiagnosticsCheck(backup, DiagnosticCode::valueAdjusted,
                         "backup moves before the start of the measure; using the measure start");
    CHECK_EQUAL(0, backup.location.measureIndex);
    CHECK_EQUAL(0, backup.location.tickTimePosition);
}

T_END

TEST(divisionsBelowOneHalfReadAsOne, ReadDiagnostics)
{
    const auto read = readDiagnostics(readDiagnosticsScore(
        readDiagnosticsScorePart("P1"),
        readDiagnosticsPart("P1", "<attributes><divisions>0.25</divisions></attributes>" + readDiagnosticsRest())));
    REQUIRE(read.ok);
    CHECK_EQUAL(1, read.score.ticksPerQuarter);
    REQUIRE(read.diagnostics.all().size() == 1);
    readDiagnosticsCheck(read.diagnostics.all()[0], DiagnosticCode::valueAdjusted, "divisions 0.25 rounded to 1");
}

T_END

TEST(overflowingDivisionsUseTheLargest, ReadDiagnostics)
{
    const auto part = [](const std::string &id, const std::string &divisions) {
        return readDiagnosticsPart(id, "<attributes><divisions>" + divisions +
                                           "</divisions></attributes><note><rest/><duration>" + divisions +
                                           "</duration></note>");
    };
    const auto read = readDiagnostics(readDiagnosticsScore(
        readDiagnosticsScorePart("P1") + readDiagnosticsScorePart("P2"), part("P1", "65537") + part("P2", "65539")));
    REQUIRE(read.ok);
    CHECK_EQUAL(65539, read.score.ticksPerQuarter);
    REQUIRE(read.diagnostics.all().size() == 1);
    readDiagnosticsCheck(read.diagnostics.all()[0], DiagnosticCode::valueAdjusted,
                         "the least common multiple of the divisions is too large; using the largest, 65539");
}

T_END

TEST(timewisePartMatchingNoScorePartIsDropped, ReadDiagnostics)
{
    const auto read =
        readDiagnostics("<score-timewise version=\"4.0\"><part-list>" + readDiagnosticsScorePart("P1") +
                        "</part-list><measure number=\"1\"><part id=\"P1\">" + readDiagnosticsRest() +
                        "</part><part id=\"P9\">" + readDiagnosticsRest() + "</part></measure></score-timewise>");
    REQUIRE(read.ok);
    REQUIRE(read.diagnostics.all().size() == 1);
    readDiagnosticsCheck(read.diagnostics.all()[0], DiagnosticCode::droppedData,
                         "part \"P9\" matches no score-part; it is not read");
    CHECK_EQUAL(0, read.diagnostics.all()[0].location.measureIndex);
}

T_END

TEST(snappedOctaveShiftAndTremoloAreReported, ReadDiagnostics)
{
    const auto read = readDiagnostics(readDiagnosticsScore(
        readDiagnosticsScorePart("P1"),
        readDiagnosticsPart("P1", "<attributes><divisions>1</divisions></attributes><direction><direction-type>"
                                  "<octave-shift type=\"down\" size=\"10\"/></direction-type></direction>"
                                  "<note><pitch><step>C</step><octave>4</octave></pitch><duration>1</duration>"
                                  "<notations><ornaments><tremolo type=\"single\">0</tremolo></ornaments>"
                                  "</notations></note>")));
    REQUIRE(read.ok);
    REQUIRE(read.diagnostics.all().size() == 2);
    readDiagnosticsCheck(read.diagnostics.all()[0], DiagnosticCode::valueAdjusted,
                         "octave-shift size 10 is not 8, 15 or 22; using 15");
    readDiagnosticsCheck(read.diagnostics.all()[1], DiagnosticCode::valueAdjusted,
                         "tremolo with 0 marks is not supported; using 3");
    CHECK_EQUAL(0, read.diagnostics.all()[1].location.staffIndex);
}

T_END

TEST(cleanDocumentReportsNothing, ReadDiagnostics)
{
    const auto read = readDiagnostics(readDiagnosticsScore(
        "<part-group type=\"start\" number=\"1\"/>" + readDiagnosticsScorePart("P1") + readDiagnosticsScorePart("P2") +
            "<part-group type=\"stop\" number=\"1\"/>",
        readDiagnosticsPart("P1", "<attributes><divisions>2</divisions><staves>2</staves></attributes>"
                                  "<note><rest/><duration>2</duration><staff>2</staff></note>") +
            readDiagnosticsPart("P2", "<attributes><divisions>3</divisions></attributes><note><rest/>"
                                      "<duration>3</duration></note>")));
    REQUIRE(read.ok);
    CHECK(read.diagnostics.all().empty());
}

T_END

#endif
