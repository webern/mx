// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Coverage for issue #429: a spanner whose start and stop sit on the same note must be written
// start-first. Finale produces this shape routinely -- it can only anchor a glissando or slide
// line to a notehead, so a line drawn toward a rest starts and stops on one note -- and a nested
// tuplet can cover exactly one note of its outer tuplet. A note that merely chains two spanners
// (one ends where the next begins) keeps the stop-before-start order of #139; that case is
// covered in GlissandoApiTest.

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"
#include "pugixml.hpp"

#include <string>
#include <utility>
#include <vector>

using namespace mx::api;
using namespace mxtest;

namespace singleNoteSpannerApiTest
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

inline TupletStart makeTripletStart(int inNumberLevel)
{
    TupletStart start;
    start.numberLevel = inNumberLevel;
    start.actualNumber = 3;
    start.actualDurationName = DurationName::quarter;
    start.normalNumber = 2;
    start.normalDurationName = DurationName::quarter;
    return start;
}

inline ScoreData makeScoreWithOneVoice()
{
    ScoreData score;
    score.parts.emplace_back();
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    return score;
}

inline VoiceData &getVoice(ScoreData &ioScore)
{
    return ioScore.parts.back().measures.back().staves.back().voices[0];
}

inline void collectSpanner(const pugi::xml_node &inNode, const std::string &inElementName,
                           std::vector<std::pair<std::string, std::string>> &ioResult)
{
    if (std::string{inNode.name()} == inElementName)
    {
        ioResult.emplace_back(inNode.attribute("type").value(), inNode.attribute("number").value());
    }
    for (const auto &child : inNode.children())
    {
        collectSpanner(child, inElementName, ioResult);
    }
}

// The (type, number) attribute pairs of every element named inElementName in the note's
// <notations>, in document order, looking through <ornaments> and other wrappers. An element
// without a number attribute reports an empty string.
inline std::vector<std::pair<std::string, std::string>> spannerSequence(const std::string &inXml, int inNoteIndex,
                                                                        const std::string &inElementName)
{
    pugi::xml_document xdoc;
    if (!xdoc.load_string(inXml.c_str()))
    {
        return {};
    }
    const auto measure = xdoc.document_element().child("part").child("measure");
    auto noteIter = measure.children("note").begin();
    for (int i = 0; i < inNoteIndex; ++i)
    {
        ++noteIter;
    }
    std::vector<std::pair<std::string, std::string>> result;
    for (const auto &notationsChild : noteIter->child("notations").children())
    {
        collectSpanner(notationsChild, inElementName, result);
    }
    return result;
}
} // namespace singleNoteSpannerApiTest

// A nested tuplet whose inner tuplet covers exactly one note -- the last note of the outer
// tuplet. That note carries the outer stop and the whole inner tuplet, and the inner start must
// come before the inner stop, with the outer stop after both.
TEST(nestedTupletOnFinalNoteWritesInnerStartFirst, SingleNoteSpanner)
{
    using namespace singleNoteSpannerApiTest;
    auto score = makeScoreWithOneVoice();
    auto &voice = getVoice(score);

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    voice.notes.back().noteAttachmentData.tupletStarts.push_back(makeTripletStart(1));

    voice.notes.push_back(makeGraceNote(Step::d, 4));

    voice.notes.push_back(makeGraceNote(Step::e, 4));
    TupletStop outerStop;
    outerStop.numberLevel = 1;
    voice.notes.back().noteAttachmentData.tupletStops.push_back(outerStop);
    voice.notes.back().noteAttachmentData.tupletStarts.push_back(makeTripletStart(2));
    TupletStop innerStop;
    innerStop.numberLevel = 2;
    voice.notes.back().noteAttachmentData.tupletStops.push_back(innerStop);

    const auto sequence = spannerSequence(toXml(score), 2, "tuplet");
    REQUIRE(sequence.size() == 3);
    CHECK_EQUAL(std::string{"start"}, sequence.at(0).first);
    CHECK_EQUAL(std::string{"2"}, sequence.at(0).second);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(1).first);
    CHECK_EQUAL(std::string{"2"}, sequence.at(1).second);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(2).first);
    CHECK_EQUAL(std::string{"1"}, sequence.at(2).second);
}

T_END

// The mirror image: the inner single-note tuplet sits on the first note of the outer tuplet, so
// the outer start comes first and the inner pair follows.
TEST(nestedTupletOnFirstNoteWritesOuterStartFirst, SingleNoteSpanner)
{
    using namespace singleNoteSpannerApiTest;
    auto score = makeScoreWithOneVoice();
    auto &voice = getVoice(score);

    voice.notes.push_back(makeGraceNote(Step::c, 4));
    voice.notes.back().noteAttachmentData.tupletStarts.push_back(makeTripletStart(1));
    voice.notes.back().noteAttachmentData.tupletStarts.push_back(makeTripletStart(2));
    TupletStop innerStop;
    innerStop.numberLevel = 2;
    voice.notes.back().noteAttachmentData.tupletStops.push_back(innerStop);

    voice.notes.push_back(makeGraceNote(Step::d, 4));

    voice.notes.push_back(makeGraceNote(Step::e, 4));
    TupletStop outerStop;
    outerStop.numberLevel = 1;
    voice.notes.back().noteAttachmentData.tupletStops.push_back(outerStop);

    const auto sequence = spannerSequence(toXml(score), 0, "tuplet");
    REQUIRE(sequence.size() == 3);
    CHECK_EQUAL(std::string{"start"}, sequence.at(0).first);
    CHECK_EQUAL(std::string{"1"}, sequence.at(0).second);
    CHECK_EQUAL(std::string{"start"}, sequence.at(1).first);
    CHECK_EQUAL(std::string{"2"}, sequence.at(1).second);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(2).first);
    CHECK_EQUAL(std::string{"2"}, sequence.at(2).second);
}

T_END

// A one-note tuplet on a plain, un-nested note: start then stop.
TEST(oneNoteTupletWritesStartThenStop, SingleNoteSpanner)
{
    using namespace singleNoteSpannerApiTest;
    auto score = makeScoreWithOneVoice();
    auto &voice = getVoice(score);

    voice.notes.push_back(makeGraceNote(Step::b, 4));
    voice.notes.back().noteAttachmentData.tupletStarts.push_back(makeTripletStart(1));
    TupletStop stop;
    stop.numberLevel = 1;
    voice.notes.back().noteAttachmentData.tupletStops.push_back(stop);

    const auto sequence = spannerSequence(toXml(score), 0, "tuplet");
    REQUIRE(sequence.size() == 2);
    CHECK_EQUAL(std::string{"start"}, sequence.at(0).first);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(1).first);
}

T_END

// A glissando drawn from a note toward a rest: both ends sit on the note, with an explicit
// number, and the start must be written first.
TEST(glissandoToRestWritesStartThenStop, SingleNoteSpanner)
{
    using namespace singleNoteSpannerApiTest;
    auto score = makeScoreWithOneVoice();
    auto &voice = getVoice(score);

    voice.notes.push_back(makeGraceNote(Step::f, 4));
    GlissandoStart start{GlissandoType::glissando};
    start.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(start);
    GlissandoStop stop{GlissandoType::glissando};
    stop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.glissandoStops.push_back(stop);

    voice.notes.push_back(makeGraceNote(Step::f, 4));

    const auto sequence = spannerSequence(toXml(score), 0, "glissando");
    REQUIRE(sequence.size() == 2);
    CHECK_EQUAL(std::string{"start"}, sequence.at(0).first);
    CHECK_EQUAL(std::string{"1"}, sequence.at(0).second);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(1).first);
    CHECK_EQUAL(std::string{"1"}, sequence.at(1).second);
}

T_END

// The same shape as a slide, and with identity numbers instead of explicit ones.
TEST(slideToRestWritesStartThenStop, SingleNoteSpanner)
{
    using namespace singleNoteSpannerApiTest;
    auto score = makeScoreWithOneVoice();
    auto &voice = getVoice(score);

    voice.notes.push_back(makeGraceNote(Step::f, 4));
    GlissandoStart start{GlissandoType::slide};
    start.number = SpannerNumber("s");
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(start);
    GlissandoStop stop{GlissandoType::slide};
    stop.number = SpannerNumber("s");
    voice.notes.back().noteAttachmentData.glissandoStops.push_back(stop);

    const auto sequence = spannerSequence(toXml(score), 0, "slide");
    REQUIRE(sequence.size() == 2);
    CHECK_EQUAL(std::string{"start"}, sequence.at(0).first);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(1).first);
    CHECK_EQUAL(sequence.at(0).second, sequence.at(1).second);
}

T_END

// A single-note glissando with no number at all still comes out start-first. With the attribute
// absent a reader falls back to number 1 for pairing, so the order is all it has to go on.
TEST(unnumberedGlissandoWritesStartThenStop, SingleNoteSpanner)
{
    using namespace singleNoteSpannerApiTest;
    auto score = makeScoreWithOneVoice();
    auto &voice = getVoice(score);

    voice.notes.push_back(makeGraceNote(Step::f, 4));
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(GlissandoStart{GlissandoType::glissando});
    voice.notes.back().noteAttachmentData.glissandoStops.push_back(GlissandoStop{GlissandoType::glissando});

    const auto sequence = spannerSequence(toXml(score), 0, "glissando");
    REQUIRE(sequence.size() == 2);
    CHECK_EQUAL(std::string{"start"}, sequence.at(0).first);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(1).first);
}

T_END

// A wavy line that starts and stops on one note: both ends live inside <ornaments>, and the stop
// follows its start instead of being written before the mark-derived ornaments.
TEST(wavyLineOnOneNoteWritesStartThenStop, SingleNoteSpanner)
{
    using namespace singleNoteSpannerApiTest;
    auto score = makeScoreWithOneVoice();
    auto &voice = getVoice(score);

    voice.notes.push_back(makeGraceNote(Step::a, 4));
    WavyLineStart start;
    start.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStarts.push_back(start);
    WavyLineStop stop;
    stop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.wavyLineStops.push_back(stop);

    const auto sequence = spannerSequence(toXml(score), 0, "wavy-line");
    REQUIRE(sequence.size() == 2);
    CHECK_EQUAL(std::string{"start"}, sequence.at(0).first);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(1).first);
}

T_END

// The reader accepts what the writer now produces: a single-note glissando survives a round trip
// with both ends still on the one note.
TEST(glissandoToRestRoundTrips, SingleNoteSpanner)
{
    using namespace singleNoteSpannerApiTest;
    auto score = makeScoreWithOneVoice();
    auto &voice = getVoice(score);

    voice.notes.push_back(makeGraceNote(Step::f, 4));
    GlissandoStart start{GlissandoType::glissando};
    start.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.glissandoStarts.push_back(start);
    GlissandoStop stop{GlissandoType::glissando};
    stop.number = SpannerNumber(1);
    voice.notes.back().noteAttachmentData.glissandoStops.push_back(stop);

    const auto roundTripped = roundTrip(score);
    REQUIRE(roundTripped.parts.size() == 1);
    const auto &notes = roundTripped.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;
    REQUIRE(notes.size() == 1);
    CHECK(notes.at(0).noteAttachmentData.glissandoStarts.size() == 1);
    CHECK(notes.at(0).noteAttachmentData.glissandoStops.size() == 1);
}

T_END

#endif
