// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// fromScore recoveries reported through Diagnostics. Each write produces the same
// document with or without a collection.

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/CurveData.h"
#include "mx/api/Diagnostics.h"
#include "mx/api/MarkData.h"
#include "mx/api/MusicXml.h"
#include "mx/api/ScoreData.h"

#include <sstream>
#include <string>

using namespace mx::api;

inline ScoreData writeDiagnosticsScore(int measureCount)
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
        note.pitchData.step = Step::c;
        note.pitchData.octave = 4;
        note.durationData.durationTimeTicks = 40;
        note.durationData.durationName = DurationName::whole;
    }
    return score;
}

inline NoteData &writeDiagnosticsNote(ScoreData &score, int measureIndex)
{
    return score.parts.front().measures.at(static_cast<size_t>(measureIndex)).staves.front().voices[0].notes.front();
}

inline Diagnostics writeDiagnostics(const ScoreData &score)
{
    Diagnostics diagnostics;
    const auto observed = fromScore(score, diagnostics);
    const auto silent = fromScore(score);
    REQUIRE(observed.ok());
    REQUIRE(silent.ok());

    std::ostringstream observedXml;
    std::ostringstream silentXml;
    REQUIRE(observed.value().writeToStream(observedXml).ok());
    REQUIRE(silent.value().writeToStream(silentXml).ok());
    CHECK_EQUAL(silentXml.str(), observedXml.str());
    return diagnostics;
}

inline void writeDiagnosticsCheck(const Diagnostic &diagnostic, DiagnosticCode code, const std::string &message)
{
    const Severity severity = code == DiagnosticCode::droppedData ? Severity::error : Severity::warning;
    CHECK(severity == diagnostic.severity);
    CHECK(code == diagnostic.code);
    CHECK_EQUAL(message, diagnostic.message);
}

TEST(cleanScoreReportsNothing, WriteDiagnostics)
{
    const auto diagnostics = writeDiagnostics(writeDiagnosticsScore(2));
    CHECK(diagnostics.all().empty());
}

T_END

TEST(scrubbedNoteIdIsReportedAtTheNote, WriteDiagnostics)
{
    auto score = writeDiagnosticsScore(2);
    writeDiagnosticsNote(score, 1).id = Id{"1 note"};

    const auto diagnostics = writeDiagnostics(score);
    REQUIRE(diagnostics.all().size() == 1);
    const auto &diagnostic = diagnostics.all().front();
    writeDiagnosticsCheck(diagnostic, DiagnosticCode::invalidValue, "id \"1 note\" is not a valid id; using \"note\"");
    CHECK_EQUAL(0, diagnostic.location.partIndex);
    CHECK_EQUAL(1, diagnostic.location.measureIndex);
    CHECK_EQUAL(0, diagnostic.location.staffIndex);
    CHECK_EQUAL(0, diagnostic.location.voiceIndex);
    CHECK_EQUAL(0, diagnostic.location.tickTimePosition);
}

T_END

TEST(scrubbedPartIdIsReportedAtThePart, WriteDiagnostics)
{
    auto score = writeDiagnosticsScore(1);
    score.parts.front().uniqueId = "1 P1";

    const auto diagnostics = writeDiagnostics(score);
    REQUIRE(diagnostics.all().size() == 1);
    const auto &diagnostic = diagnostics.all().front();
    writeDiagnosticsCheck(diagnostic, DiagnosticCode::invalidValue, "part id \"1 P1\" is not a valid id; using \"P1\"");
    CHECK_EQUAL(0, diagnostic.location.partIndex);
    CHECK_EQUAL(-1, diagnostic.location.measureIndex);
}

T_END

TEST(outOfRangeOctaveIsClamped, WriteDiagnostics)
{
    auto score = writeDiagnosticsScore(1);
    writeDiagnosticsNote(score, 0).pitchData.octave = 12;

    const auto diagnostics = writeDiagnostics(score);
    REQUIRE(diagnostics.all().size() == 1);
    writeDiagnosticsCheck(diagnostics.all().front(), DiagnosticCode::valueAdjusted,
                          "note octave 12 is out of range; using 9");
}

T_END

TEST(unspecifiedNoteTypeIsDefaulted, WriteDiagnostics)
{
    auto score = writeDiagnosticsScore(1);
    writeDiagnosticsNote(score, 0).durationData.durationName = DurationName::unspecified;

    const auto diagnostics = writeDiagnostics(score);
    REQUIRE(diagnostics.all().size() == 1);
    writeDiagnosticsCheck(diagnostics.all().front(), DiagnosticCode::missingValueDefaulted,
                          "note type is unspecified; using maxima");
}

T_END

TEST(pedalMarkOnANoteIsDropped, WriteDiagnostics)
{
    auto score = writeDiagnosticsScore(1);
    writeDiagnosticsNote(score, 0).noteAttachmentData.marks.emplace_back(MarkType::pedal);

    const auto diagnostics = writeDiagnostics(score);
    REQUIRE(diagnostics.all().size() == 1);
    writeDiagnosticsCheck(diagnostics.all().front(), DiagnosticCode::droppedData,
                          "a pedal mark on a note is not written; write it in a direction");
}

T_END

TEST(creditImageValignBaselineIsDropped, WriteDiagnostics)
{
    auto score = writeDiagnosticsScore(1);
    PageImageData img{};
    img.source = "logo.png";
    img.type = "image/png";
    img.positionData.verticalAlignment = VerticalAlignment::baseline;
    score.pageImageItems.push_back(img);

    const auto diagnostics = writeDiagnostics(score);
    REQUIRE(diagnostics.all().size() == 1);
    writeDiagnosticsCheck(diagnostics.all().front(), DiagnosticCode::droppedData,
                          "a credit-image valign of baseline is not written; an image has no baseline alignment");
}

T_END

TEST(unmatchedSlurIsReportedAtItsNote, WriteDiagnostics)
{
    auto score = writeDiagnosticsScore(2);
    writeDiagnosticsNote(score, 1).noteAttachmentData.curveStarts.emplace_back(CurveType::slur);

    const auto diagnostics = writeDiagnostics(score);
    REQUIRE(diagnostics.all().size() == 1);
    const auto &diagnostic = diagnostics.all().front();
    writeDiagnosticsCheck(diagnostic, DiagnosticCode::unmatchedSpanner, "slur start has no matching stop");
    CHECK_EQUAL(0, diagnostic.location.partIndex);
    CHECK_EQUAL(1, diagnostic.location.measureIndex);
    CHECK_EQUAL(0, diagnostic.location.staffIndex);
    CHECK_EQUAL(0, diagnostic.location.tickTimePosition);
}

T_END

TEST(sameNoteSlurStopAndStartAreMatched, WriteDiagnostics)
{
    auto score = writeDiagnosticsScore(3);
    writeDiagnosticsNote(score, 0).noteAttachmentData.curveStarts.emplace_back(CurveType::slur);
    auto &middle = writeDiagnosticsNote(score, 1).noteAttachmentData;
    middle.curveStarts.emplace_back(CurveType::slur);
    middle.curveStops.emplace_back(CurveType::slur);
    writeDiagnosticsNote(score, 2).noteAttachmentData.curveStops.emplace_back(CurveType::slur);

    const auto diagnostics = writeDiagnostics(score);
    CHECK(diagnostics.all().empty());
}

T_END

#endif
