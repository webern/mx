// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Coverage for issue #389: glissando and slide are note-attached start/stop spanners that read and
// write as top-level <notations> children, go through the SpannerNumber identity mechanism, and
// carry printed text, line appearance, and (for slide) playback timing.

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

namespace glissandoApiTest
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
} // namespace glissandoApiTest

TEST(startStopRoundTrip, Glissando)
{
    using namespace glissandoApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    GlissandoStart start{GlissandoType::glissando};
    start.number = SpannerNumber(1);
    start.text = "gliss.";
    start.lineData.lineType = LineType::dashed;
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(start);

    voice.notes.push_back(makeGraceNote(Step::g, 4));
    GlissandoStop stop{GlissandoType::glissando};
    stop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.glissandoStops.push_back(stop);

    const auto roundTripped = roundTrip(score);
    REQUIRE(roundTripped.parts.size() == 1);
    const auto &notes = roundTripped.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;
    REQUIRE(notes.size() == 2);

    REQUIRE(notes.at(0).noteAttachmentData.glissandoStarts.size() == 1);
    const auto &outStart = notes.at(0).noteAttachmentData.glissandoStarts.at(0);
    CHECK(outStart.glissandoType == GlissandoType::glissando);
    CHECK(SpannerNumber(1) == outStart.number);
    CHECK_EQUAL(std::string{"gliss."}, outStart.text);
    CHECK(outStart.lineData.lineType == LineType::dashed);

    REQUIRE(notes.at(1).noteAttachmentData.glissandoStops.size() == 1);
    const auto &outStop = notes.at(1).noteAttachmentData.glissandoStops.at(0);
    CHECK(outStop.glissandoType == GlissandoType::glissando);
    CHECK(SpannerNumber(1) == outStop.number);
}

T_END

TEST(slidePlaybackRoundTrips, Glissando)
{
    using namespace glissandoApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    GlissandoStart start{GlissandoType::slide};
    start.number = SpannerNumber(1);
    start.text = "port.";
    start.accelerate = Bool::yes;
    start.beats = 2.0;
    start.firstBeat = 25.0;
    start.lastBeat = 75.0;
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(start);

    voice.notes.push_back(makeGraceNote(Step::e, 4));
    GlissandoStop stop{GlissandoType::slide};
    stop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.glissandoStops.push_back(stop);

    const auto roundTripped = roundTrip(score);
    const auto &notes = roundTripped.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;

    REQUIRE(notes.at(0).noteAttachmentData.glissandoStarts.size() == 1);
    const auto &outStart = notes.at(0).noteAttachmentData.glissandoStarts.at(0);
    CHECK(outStart.glissandoType == GlissandoType::slide);
    CHECK_EQUAL(std::string{"port."}, outStart.text);
    CHECK(outStart.accelerate == Bool::yes);
    REQUIRE(outStart.beats.has_value());
    CHECK_DOUBLES_EQUAL(2.0, *outStart.beats, 0.00001);
    REQUIRE(outStart.firstBeat.has_value());
    CHECK_DOUBLES_EQUAL(25.0, *outStart.firstBeat, 0.00001);
    REQUIRE(outStart.lastBeat.has_value());
    CHECK_DOUBLES_EQUAL(75.0, *outStart.lastBeat, 0.00001);

    REQUIRE(notes.at(1).noteAttachmentData.glissandoStops.size() == 1);
    CHECK(notes.at(1).noteAttachmentData.glissandoStops.at(0).glissandoType == GlissandoType::slide);
}

T_END

// Requirement 7: <glissando> and <slide> are top-level <notations> children, not nested inside
// <ornaments> (unlike wavy-line).
TEST(writesAsTopLevelNotationsChild, Glissando)
{
    using namespace glissandoApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    GlissandoStart start{GlissandoType::glissando};
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(start);

    const auto xml = toXml(score);
    pugi::xml_document xdoc;
    REQUIRE(xdoc.load_string(xml.c_str()));

    const auto notations = xdoc.document_element().child("part").child("measure").child("note").child("notations");
    REQUIRE(notations);
    const auto firstChild = notations.first_child();
    REQUIRE(firstChild);
    CHECK_EQUAL(std::string{"glissando"}, std::string{firstChild.name()});
}

T_END

// Requirement 8: when a note carries both an incoming stop and an outgoing start for the same
// element, the stop must precede the start in document order (mirrors the #139 rule for slurs).
TEST(stopPrecedesStartOnAChainedNote, Glissando)
{
    using namespace glissandoApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    voice.notes.push_back(makeGraceNote(Step::d, 4));
    voice.notes.push_back(makeGraceNote(Step::e, 4));

    GlissandoStart firstStart{GlissandoType::glissando};
    firstStart.number = SpannerNumber(1);
    voice.notes.at(0).noteAttachmentData.glissandoStarts.push_back(firstStart);

    // The middle note both ends glissando 1 and begins a new glissando -- pushed start-before-stop
    // into the api vectors on purpose, to prove the writer reorders regardless of insertion order.
    GlissandoStart secondStart{GlissandoType::glissando};
    secondStart.number = SpannerNumber(1);
    voice.notes.at(1).noteAttachmentData.glissandoStarts.push_back(secondStart);
    GlissandoStop firstStop{GlissandoType::glissando};
    firstStop.number = SpannerNumber(1);
    voice.notes.at(1).noteAttachmentData.glissandoStops.push_back(firstStop);

    GlissandoStop secondStop{GlissandoType::glissando};
    secondStop.number = SpannerNumber(1);
    voice.notes.at(2).noteAttachmentData.glissandoStops.push_back(secondStop);

    const auto xml = toXml(score);
    pugi::xml_document xdoc;
    REQUIRE(xdoc.load_string(xml.c_str()));

    const auto notes = xdoc.document_element().child("part").child("measure");
    auto noteIter = notes.children("note").begin();
    ++noteIter; // the middle note

    const auto notations = noteIter->child("notations");
    REQUIRE(notations);
    auto glissandoIter = notations.children("glissando").begin();
    REQUIRE(glissandoIter != notations.children("glissando").end());
    CHECK_EQUAL(std::string{"stop"}, std::string{glissandoIter->attribute("type").value()});
    ++glissandoIter;
    REQUIRE(glissandoIter != notations.children("glissando").end());
    CHECK_EQUAL(std::string{"start"}, std::string{glissandoIter->attribute("type").value()});
}

T_END

// Requirement 2: identity spanners draw from the writer-side SpannerResolver, and glissando
// and slide draw from separate pools even when open at the same time.
TEST(identityNumbersAssignedPerElementPool, Glissando)
{
    using namespace glissandoApiTest;
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    // A glissando and a slide open back to back and stay open across the same span: if they
    // shared a number pool the second would be forced to skip the first's number.
    voice.notes.push_back(makeGraceNote(Step::c, 4));
    GlissandoStart glissandoStart{GlissandoType::glissando};
    glissandoStart.number = SpannerNumber("g");
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(glissandoStart);

    voice.notes.push_back(makeGraceNote(Step::e, 4));
    GlissandoStart slideStart{GlissandoType::slide};
    slideStart.number = SpannerNumber("s");
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(slideStart);

    voice.notes.push_back(makeGraceNote(Step::g, 4));
    GlissandoStop glissandoStop{GlissandoType::glissando};
    glissandoStop.number = SpannerNumber("g");
    voice.notes.back().noteAttachmentData.glissandoStops.push_back(glissandoStop);

    voice.notes.push_back(makeGraceNote(Step::b, 4));
    GlissandoStop slideStop{GlissandoType::slide};
    slideStop.number = SpannerNumber("s");
    voice.notes.back().noteAttachmentData.glissandoStops.push_back(slideStop);

    const auto roundTripped = roundTrip(score);
    const auto &notes = roundTripped.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;

    REQUIRE(notes.at(0).noteAttachmentData.glissandoStarts.size() == 1);
    REQUIRE(notes.at(1).noteAttachmentData.glissandoStarts.size() == 1);
    CHECK(SpannerNumber(1) == notes.at(0).noteAttachmentData.glissandoStarts.at(0).number);
    CHECK(SpannerNumber(1) == notes.at(1).noteAttachmentData.glissandoStarts.at(0).number);

    REQUIRE(notes.at(2).noteAttachmentData.glissandoStops.size() == 1);
    REQUIRE(notes.at(3).noteAttachmentData.glissandoStops.size() == 1);
    CHECK(SpannerNumber(1) == notes.at(2).noteAttachmentData.glissandoStops.at(0).number);
    CHECK(SpannerNumber(1) == notes.at(3).noteAttachmentData.glissandoStops.at(0).number);
}

T_END

// Requirement 9: the synthetic fixtures parse and surface pairing plus the shared attributes.
TEST(readsSyntheticGlissandoFixture, Glissando)
{
    using namespace mxtest;
    const auto score = mxtest::MxFileRepository::loadFile("glissando.3.0.xml");
    REQUIRE(score.parts.size() == 1);
    const auto &note = score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    REQUIRE(note.noteAttachmentData.glissandoStarts.size() == 1);
    const auto &start = note.noteAttachmentData.glissandoStarts.at(0);
    CHECK(start.glissandoType == GlissandoType::glissando);
    CHECK(SpannerNumber(1) == start.number);
    CHECK(start.lineData.lineType == LineType::solid);
    CHECK_EQUAL(std::string{"x"}, start.text);
}

T_END

TEST(readsSyntheticSlideFixture, Glissando)
{
    using namespace mxtest;
    const auto score = mxtest::MxFileRepository::loadFile("slide.3.0.xml");
    REQUIRE(score.parts.size() == 1);
    const auto &note = score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    REQUIRE(note.noteAttachmentData.glissandoStarts.size() == 1);
    const auto &start = note.noteAttachmentData.glissandoStarts.at(0);
    CHECK(start.glissandoType == GlissandoType::slide);
    CHECK(start.accelerate == Bool::yes);
    REQUIRE(start.beats.has_value());
    CHECK_DOUBLES_EQUAL(2.0, *start.beats, 0.00001);
    REQUIRE(start.firstBeat.has_value());
    CHECK_DOUBLES_EQUAL(1.0, *start.firstBeat, 0.00001);
    REQUIRE(start.lastBeat.has_value());
    CHECK_DOUBLES_EQUAL(1.0, *start.lastBeat, 0.00001);
}

T_END

#endif
