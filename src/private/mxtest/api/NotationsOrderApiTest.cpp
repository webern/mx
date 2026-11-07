// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// One note can carry a tie, a slur and a tuplet at once: a tied note that ends one slur and begins
// the next, or the first note of a triplet under a slur. MusicXML does not fix the order of these
// inside <notations>, but exporters write ties first, then tuplets, then slurs, and mx writes them
// the same way. Within each kind, a stop that closes a spanner from an earlier note comes before a
// start (#139), and a spanner that starts and stops on one note is written start first (#429).

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mxpugi/mxpugi.hpp"
#include "mxtest/api/TestHelpers.h"

#include <string>
#include <vector>

using namespace mx::api;
using namespace mxtest;

namespace notationsOrderApiTest
{
// A score of one voice holding inNoteCount quarter notes, which the test decorates.
inline ScoreData makeScoreWithNotes(int inNoteCount)
{
    ScoreData score;
    score.parts.emplace_back();
    score.parts.back().uniqueId = "P1";
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    auto &notes = score.parts.back().measures.back().staves.back().voices[0].notes;
    for (int i = 0; i < inNoteCount; ++i)
    {
        auto &note = notes.emplace_back();
        note.pitchData.step = Step::c;
        note.pitchData.octave = 4;
        note.durationData.durationName = DurationName::quarter;
        note.durationData.durationTimeTicks = 1;
        note.tickTimePosition = i;
    }
    return score;
}

inline NoteData &getNote(ScoreData &ioScore, int inNoteIndex)
{
    return ioScore.parts.back().measures.back().staves.back().voices[0].notes.at(inNoteIndex);
}

// The children of one note's <notations>, each as its element name and type attribute, for
// example "tied stop".
inline std::vector<std::string> notationsChildren(const std::string &inXml, int inNoteIndex)
{
    mxpugi::xml_document xdoc;
    if (!xdoc.load_string(inXml.c_str()))
    {
        return {};
    }
    const auto measure = xdoc.document_element().child("part").child("measure");
    auto note = measure.child("note");
    for (int i = 0; i < inNoteIndex; ++i)
    {
        note = note.next_sibling("note");
    }
    std::vector<std::string> result;
    for (const auto &child : note.child("notations").children())
    {
        result.push_back(std::string{child.name()} + " " + child.attribute("type").value());
    }
    return result;
}

inline void checkSequence(const std::vector<std::string> &inExpected, const std::vector<std::string> &inActual)
{
    REQUIRE(inExpected.size() == inActual.size());
    for (std::size_t i = 0; i < inExpected.size(); ++i)
    {
        CHECK_EQUAL(inExpected.at(i), inActual.at(i));
    }
}
} // namespace notationsOrderApiTest

// A tied note that ends one slur, begins another, and begins a triplet. The tie and the first slur
// come from the note before it. The ties are written first, then the tuplet, then the slurs, each
// kind with its stop before its start.
TEST(tiesThenTupletsThenSlurs, NotationsOrder)
{
    using namespace notationsOrderApiTest;
    auto score = makeScoreWithNotes(2);

    auto &first = getNote(score, 0).noteAttachmentData;
    CurveStart firstSlurStart{CurveType::slur};
    firstSlurStart.number = SpannerNumber{1};
    first.curveStarts.push_back(firstSlurStart);
    CurveStart firstTieStart{CurveType::tie};
    firstTieStart.number = SpannerNumber{1};
    first.curveStarts.push_back(firstTieStart);

    auto &note = getNote(score, 1);
    note.durationData.timeModificationActualNotes = 3;
    note.durationData.timeModificationNormalNotes = 2;
    auto &attachments = note.noteAttachmentData;
    CurveStop slurStop{CurveType::slur};
    slurStop.number = SpannerNumber{1};
    attachments.curveStops.push_back(slurStop);
    CurveStop tieStop{CurveType::tie};
    tieStop.number = SpannerNumber{1};
    attachments.curveStops.push_back(tieStop);
    CurveStart slurStart{CurveType::slur};
    slurStart.number = SpannerNumber{1};
    attachments.curveStarts.push_back(slurStart);
    CurveStart tieStart{CurveType::tie};
    tieStart.number = SpannerNumber{1};
    attachments.curveStarts.push_back(tieStart);
    TupletStart tupletStart;
    tupletStart.number = SpannerNumber{1};
    attachments.tupletStarts.push_back(tupletStart);

    checkSequence({"tied stop", "tied start", "tuplet start", "slur stop", "slur start"},
                  notationsChildren(toXml(score), 1));
}

T_END

// A let-ring tie is a tie too, so it is written ahead of a slur that starts on the same note.
TEST(letRingTieBeforeSlur, NotationsOrder)
{
    using namespace notationsOrderApiTest;
    auto score = makeScoreWithNotes(1);
    auto &note = getNote(score, 0);
    note.tieLetRing = TieLetRing{};
    CurveStart slurStart{CurveType::slur};
    slurStart.number = SpannerNumber{1};
    note.noteAttachmentData.curveStarts.push_back(slurStart);

    checkSequence({"tied let-ring", "slur start"}, notationsChildren(toXml(score), 0));
}

T_END

// A tie that starts and stops on one note, the way a let-ring tie was written before MusicXML had a
// let-ring type. It is written start first, even though stops usually come before starts.
TEST(oneNoteTieWritesStartThenStop, NotationsOrder)
{
    using namespace notationsOrderApiTest;
    auto score = makeScoreWithNotes(1);
    auto &attachments = getNote(score, 0).noteAttachmentData;
    attachments.curveStops.push_back(CurveStop{CurveType::tie});
    attachments.curveStarts.push_back(CurveStart{CurveType::tie});

    checkSequence({"tied start", "tied stop"}, notationsChildren(toXml(score), 0));
}

T_END

// A one-note tie, a one-note tuplet and a one-note slur on the same note. Each spanner is written
// start first, and the kinds keep their order: ties, then tuplets, then slurs.
TEST(oneNoteSpansKeepTheKindOrder, NotationsOrder)
{
    using namespace notationsOrderApiTest;
    auto score = makeScoreWithNotes(1);
    auto &note = getNote(score, 0);
    note.durationData.timeModificationActualNotes = 3;
    note.durationData.timeModificationNormalNotes = 2;
    auto &attachments = note.noteAttachmentData;
    CurveStop slurStop{CurveType::slur};
    slurStop.number = SpannerNumber{1};
    attachments.curveStops.push_back(slurStop);
    CurveStop tieStop{CurveType::tie};
    tieStop.number = SpannerNumber{1};
    attachments.curveStops.push_back(tieStop);
    CurveStart slurStart{CurveType::slur};
    slurStart.number = SpannerNumber{1};
    attachments.curveStarts.push_back(slurStart);
    CurveStart tieStart{CurveType::tie};
    tieStart.number = SpannerNumber{1};
    attachments.curveStarts.push_back(tieStart);
    TupletStart tupletStart;
    tupletStart.number = SpannerNumber{1};
    attachments.tupletStarts.push_back(tupletStart);
    TupletStop tupletStop;
    tupletStop.number = SpannerNumber{1};
    attachments.tupletStops.push_back(tupletStop);

    checkSequence({"tied start", "tied stop", "tuplet start", "tuplet stop", "slur start", "slur stop"},
                  notationsChildren(toXml(score), 0));
}

T_END

// A note that ends a slur from the note before it and also holds a one-note slur of its own. The
// slur from before is closed first, then the one-note slur is written start first.
TEST(chainedSlurThenOneNoteSlur, NotationsOrder)
{
    using namespace notationsOrderApiTest;
    auto score = makeScoreWithNotes(2);
    CurveStart firstStart{CurveType::slur};
    firstStart.number = SpannerNumber{1};
    getNote(score, 0).noteAttachmentData.curveStarts.push_back(firstStart);

    auto &attachments = getNote(score, 1).noteAttachmentData;
    CurveStop chainedStop{CurveType::slur};
    chainedStop.number = SpannerNumber{1};
    attachments.curveStops.push_back(chainedStop);
    CurveStop oneNoteStop{CurveType::slur};
    oneNoteStop.number = SpannerNumber{2};
    attachments.curveStops.push_back(oneNoteStop);
    CurveStart oneNoteStart{CurveType::slur};
    oneNoteStart.number = SpannerNumber{2};
    attachments.curveStarts.push_back(oneNoteStart);

    const auto xml = toXml(score);
    checkSequence({"slur stop", "slur start", "slur stop"}, notationsChildren(xml, 1));

    mxpugi::xml_document xdoc;
    REQUIRE(xdoc.load_string(xml.c_str()));
    const auto notations =
        xdoc.document_element().child("part").child("measure").child("note").next_sibling("note").child("notations");
    std::vector<std::string> numbers;
    for (const auto &child : notations.children())
    {
        numbers.push_back(child.attribute("number").value());
    }
    checkSequence({"1", "2", "2"}, numbers);
}

T_END

// A one-note tie read from MusicXML is written back start first.
TEST(oneNoteTieRoundTrips, NotationsOrder)
{
    using namespace notationsOrderApiTest;
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1"><part-name>Music</part-name></score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes><divisions>1</divisions></attributes>
      <note>
        <pitch><step>C</step><octave>4</octave></pitch>
        <duration>1</duration>
        <type>quarter</type>
        <notations>
          <tied type="start"/>
          <tied type="stop"/>
        </notations>
      </note>
    </measure>
  </part>
</score-partwise>
)";
    checkSequence({"tied start", "tied stop"}, notationsChildren(toXml(fromXml(xml)), 0));
}

T_END

#endif
