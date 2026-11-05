// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// One note can carry a tie, a slur and a tuplet at once: a tied note that ends one slur and begins
// the next, or the first note of a triplet under a slur. MusicXML does not fix the order of these
// inside <notations>, but exporters write ties first, then tuplets, then slurs, and mx writes them
// the same way.

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
// A score of one voice holding a single quarter note, which the test decorates.
inline ScoreData makeScoreWithOneNote()
{
    ScoreData score;
    score.parts.emplace_back();
    score.parts.back().uniqueId = "P1";
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    auto &note = score.parts.back().measures.back().staves.back().voices[0].notes.emplace_back();
    note.pitchData.step = Step::c;
    note.pitchData.octave = 4;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 1;
    return score;
}

inline NoteData &getNote(ScoreData &ioScore)
{
    return ioScore.parts.back().measures.back().staves.back().voices[0].notes.back();
}

// The children of the first note's <notations>, each as its element name and type attribute,
// for example "tied stop".
inline std::vector<std::string> notationsChildren(const std::string &inXml)
{
    mxpugi::xml_document xdoc;
    if (!xdoc.load_string(inXml.c_str()))
    {
        return {};
    }
    const auto note = xdoc.document_element().child("part").child("measure").child("note");
    std::vector<std::string> result;
    for (const auto &child : note.child("notations").children())
    {
        result.push_back(std::string{child.name()} + " " + child.attribute("type").value());
    }
    return result;
}
} // namespace notationsOrderApiTest

// A tied note that ends one slur, begins another, and begins a triplet. The ties are written
// first, then the tuplet, then the slurs, each kind with its stop before its start.
TEST(tiesThenTupletsThenSlurs, NotationsOrder)
{
    using namespace notationsOrderApiTest;
    auto score = makeScoreWithOneNote();
    auto &note = getNote(score);
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

    const std::vector<std::string> expected{"tied stop", "tied start", "tuplet start", "slur stop", "slur start"};
    const auto actual = notationsChildren(toXml(score));
    REQUIRE(expected.size() == actual.size());
    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        CHECK_EQUAL(expected.at(i), actual.at(i));
    }
}

T_END

// A let-ring tie is a tie too, so it is written ahead of a slur that starts on the same note.
TEST(letRingTieBeforeSlur, NotationsOrder)
{
    using namespace notationsOrderApiTest;
    auto score = makeScoreWithOneNote();
    auto &note = getNote(score);
    note.tieLetRing = TieLetRing{};
    CurveStart slurStart{CurveType::slur};
    slurStart.number = SpannerNumber{1};
    note.noteAttachmentData.curveStarts.push_back(slurStart);

    const std::vector<std::string> expected{"tied let-ring", "slur start"};
    const auto actual = notationsChildren(toXml(score));
    REQUIRE(expected.size() == actual.size());
    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        CHECK_EQUAL(expected.at(i), actual.at(i));
    }
}

T_END

#endif
