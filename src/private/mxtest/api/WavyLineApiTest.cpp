// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Coverage for issue #389: a wavy line is a start/continue/stop spanner that lives inside a
// note's <ornaments>, is numbered like any other spanner, and carries a SMuFL glyph override and
// trill-sound playback on its start.

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"
#include "mxtest/file/MxFileRepository.h"
#include "pugixml.hpp"

using namespace mx::api;
using namespace mxtest;

namespace wavyLineApiTest
{
inline NoteData makeGraceNote(Step inStep, int inOctave)
{
    NoteData note;
    note.isGrace = true;
    note.pitchData.step = inStep;
    note.pitchData.octave = inOctave;
    note.durationData.durationName = DurationName::eighth;
    note.durationData.durationTimeTicks = 0;
    return note;
}
} // namespace wavyLineApiTest

TEST(startStopRoundTrip, WavyLine)
{
    using namespace wavyLineApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    WavyLineStart start;
    start.number = SpannerNumber(1);
    start.positionData.placement = Placement::above;
    start.smufl = "wiggleTrill";
    start.startNote = WavyLineStartNote::upper;
    start.trillStep = WavyLineTrillStep::whole;
    start.twoNoteTurn = WavyLineTwoNoteTurn::whole;
    start.accelerate = Bool::yes;
    start.beats = 2.0;
    start.secondBeat = 25.0;
    start.lastBeat = 75.0;
    voice.notes.back().noteAttachmentData.wavyLineStarts.push_back(start);

    voice.notes.push_back(makeGraceNote(Step::g, 4));
    WavyLineStop stop;
    stop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStops.push_back(stop);

    const auto roundTripped = roundTrip(score);
    REQUIRE(roundTripped.parts.size() == 1);
    const auto &notes = roundTripped.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;
    REQUIRE(notes.size() == 2);

    REQUIRE(notes.at(0).noteAttachmentData.wavyLineStarts.size() == 1);
    const auto &outStart = notes.at(0).noteAttachmentData.wavyLineStarts.at(0);
    CHECK(SpannerNumber(1) == outStart.number);
    CHECK(outStart.positionData.placement == Placement::above);
    REQUIRE(outStart.smufl.has_value());
    CHECK_EQUAL(std::string{"wiggleTrill"}, *outStart.smufl);
    CHECK(outStart.startNote == WavyLineStartNote::upper);
    CHECK(outStart.trillStep == WavyLineTrillStep::whole);
    CHECK(outStart.twoNoteTurn == WavyLineTwoNoteTurn::whole);
    CHECK(outStart.accelerate == Bool::yes);
    REQUIRE(outStart.beats.has_value());
    CHECK_DOUBLES_EQUAL(2.0, *outStart.beats, 0.00001);
    REQUIRE(outStart.secondBeat.has_value());
    CHECK_DOUBLES_EQUAL(25.0, *outStart.secondBeat, 0.00001);
    REQUIRE(outStart.lastBeat.has_value());
    CHECK_DOUBLES_EQUAL(75.0, *outStart.lastBeat, 0.00001);

    REQUIRE(notes.at(1).noteAttachmentData.wavyLineStops.size() == 1);
    CHECK(SpannerNumber(1) == notes.at(1).noteAttachmentData.wavyLineStops.at(0).number);
}

T_END

// Requirement 1: two wavy lines on one note -- a stop for the incoming trill extension and a
// start for a new one -- must be expressible and keep score order (stop before start).
TEST(twoWavyLinesOnOneNote, WavyLine)
{
    using namespace wavyLineApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    WavyLineStart firstStart;
    firstStart.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStarts.push_back(firstStart);

    voice.notes.push_back(makeGraceNote(Step::d, 4));
    // Pushed start-before-stop on purpose, to prove the writer reorders regardless of insertion
    // order (mirrors the #139 stop-before-start rule).
    WavyLineStart secondStart;
    secondStart.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStarts.push_back(secondStart);
    WavyLineStop firstStop;
    firstStop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStops.push_back(firstStop);

    voice.notes.push_back(makeGraceNote(Step::e, 4));
    WavyLineStop secondStop;
    secondStop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStops.push_back(secondStop);

    const auto roundTripped = roundTrip(score);
    const auto &notes = roundTripped.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;
    CHECK(notes.at(0).noteAttachmentData.wavyLineStarts.size() == 1);
    CHECK(notes.at(1).noteAttachmentData.wavyLineStarts.size() == 1);
    CHECK(notes.at(1).noteAttachmentData.wavyLineStops.size() == 1);
    CHECK(notes.at(2).noteAttachmentData.wavyLineStops.size() == 1);

    const auto xml = toXml(score);
    pugi::xml_document xdoc;
    REQUIRE(xdoc.load_string(xml.c_str()));
    auto noteIter = xdoc.document_element().child("part").child("measure").children("note").begin();
    ++noteIter; // the middle note, carrying both a stop and a start

    const auto ornaments = noteIter->child("notations").child("ornaments");
    REQUIRE(ornaments);
    auto wavyIter = ornaments.children("wavy-line").begin();
    REQUIRE(wavyIter != ornaments.children("wavy-line").end());
    CHECK_EQUAL(std::string{"stop"}, std::string{wavyIter->attribute("type").value()});
    ++wavyIter;
    REQUIRE(wavyIter != ornaments.children("wavy-line").end());
    CHECK_EQUAL(std::string{"start"}, std::string{wavyIter->attribute("type").value()});
}

T_END

// A continuation point (for a wavy line crossing a system/page break) round-trips its number and
// position.
TEST(continuationRoundTrips, WavyLine)
{
    using namespace wavyLineApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    WavyLineStart start;
    start.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStarts.push_back(start);

    voice.notes.push_back(makeGraceNote(Step::d, 4));
    WavyLineContinue wavyLineContinue;
    wavyLineContinue.number = SpannerNumber(1);
    wavyLineContinue.isColorSpecified = true;
    wavyLineContinue.colorData.red = 0xAA;
    wavyLineContinue.colorData.green = 0xBB;
    wavyLineContinue.colorData.blue = 0xCC;
    voice.notes.back().noteAttachmentData.wavyLineContinuations.push_back(wavyLineContinue);

    voice.notes.push_back(makeGraceNote(Step::e, 4));
    WavyLineStop stop;
    stop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStops.push_back(stop);

    const auto roundTripped = roundTrip(score);
    const auto &notes = roundTripped.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;
    REQUIRE(notes.at(1).noteAttachmentData.wavyLineContinuations.size() == 1);
    const auto &outContinue = notes.at(1).noteAttachmentData.wavyLineContinuations.at(0);
    CHECK(SpannerNumber(1) == outContinue.number);
    CHECK(outContinue.isColorSpecified);
    CHECK_EQUAL(0xAA, static_cast<int>(outContinue.colorData.red));
    CHECK_EQUAL(0xBB, static_cast<int>(outContinue.colorData.green));
    CHECK_EQUAL(0xCC, static_cast<int>(outContinue.colorData.blue));
}

T_END

// A wavy line lives inside <ornaments>, unlike glissando/slide which are top-level <notations>
// children.
TEST(writesInsideOrnaments, WavyLine)
{
    using namespace wavyLineApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    WavyLineStart start;
    voice.notes.back().noteAttachmentData.wavyLineStarts.push_back(start);

    const auto xml = toXml(score);
    pugi::xml_document xdoc;
    REQUIRE(xdoc.load_string(xml.c_str()));
    const auto wavyLine = xdoc.document_element()
                              .child("part")
                              .child("measure")
                              .child("note")
                              .child("notations")
                              .child("ornaments")
                              .child("wavy-line");
    REQUIRE(wavyLine);
}

T_END

// Requirement 9: the synthetic fixtures parse and surface pairing plus the shared attributes.
TEST(readsSyntheticWavyLineFixture, WavyLine)
{
    const auto score = mxtest::MxFileRepository::loadFile("wavy-line.3.0.xml");
    REQUIRE(score.parts.size() == 1);
    const auto &note = score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    REQUIRE(note.noteAttachmentData.wavyLineStarts.size() == 1);
    const auto &start = note.noteAttachmentData.wavyLineStarts.at(0);
    CHECK(SpannerNumber(1) == start.number);
    CHECK(start.positionData.placement == Placement::above);
    CHECK(start.startNote == WavyLineStartNote::upper);
    CHECK(start.trillStep == WavyLineTrillStep::whole);
    CHECK(start.twoNoteTurn == WavyLineTwoNoteTurn::whole);
    CHECK(start.accelerate == Bool::yes);
    REQUIRE(start.beats.has_value());
    CHECK_DOUBLES_EQUAL(2.0, *start.beats, 0.00001);
}

T_END

TEST(readsSyntheticWavyLineFixtureWithSmufl, WavyLine)
{
    const auto score = mxtest::MxFileRepository::loadFile("wavy-line.4.0.xml");
    REQUIRE(score.parts.size() == 1);
    const auto &note = score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    REQUIRE(note.noteAttachmentData.wavyLineStarts.size() == 1);
    REQUIRE(note.noteAttachmentData.wavyLineStarts.at(0).smufl.has_value());
    CHECK_EQUAL(std::string{"wiggleTrill"}, *note.noteAttachmentData.wavyLineStarts.at(0).smufl);
}

T_END

#endif
