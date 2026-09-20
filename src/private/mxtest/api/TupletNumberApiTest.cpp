// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Tuplets carry a number attribute for the same reason slurs do: to tell two of them apart
// while both are open, which happens when one tuplet is written inside another. An author who
// gives both ends of each tuplet the same identity gets the numbers assigned here, the same way
// the other spanner families are numbered (see SpannerResolver).

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/Diagnostics.h"
#include "mx/api/MusicXml.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/TestHelpers.h"
#include "pugixml.hpp"

#include <string>
#include <utility>
#include <vector>

using namespace mx::api;
using namespace mxtest;

namespace tupletNumberApiTest
{
inline ScoreData makeScore()
{
    ScoreData score;
    score.parts.emplace_back();
    score.parts.back().uniqueId = "P1";
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    return score;
}

inline NoteData &appendNote(ScoreData &ioScore)
{
    auto &voice = ioScore.parts.back().measures.back().staves.back().voices[0];
    auto &note = voice.notes.emplace_back();
    note.pitchData.step = Step::c;
    note.pitchData.octave = 4;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 1;
    note.durationData.timeModificationActualNotes = 3;
    note.durationData.timeModificationNormalNotes = 2;
    note.tickTimePosition = static_cast<int>(voice.notes.size()) - 1;
    return note;
}

inline void addStart(NoteData &ioNote, SpannerNumber inNumber)
{
    TupletStart start;
    start.number = std::move(inNumber);
    ioNote.noteAttachmentData.tupletStarts.push_back(start);
}

inline void addStop(NoteData &ioNote, SpannerNumber inNumber)
{
    TupletStop stop;
    stop.number = std::move(inNumber);
    ioNote.noteAttachmentData.tupletStops.push_back(stop);
}

// Every <tuplet> in the document as a (type, number) pair, in document order. An element with
// no number attribute reports an empty string.
inline std::vector<std::pair<std::string, std::string>> tupletSequence(const std::string &inXml)
{
    std::vector<std::pair<std::string, std::string>> result;
    pugi::xml_document xdoc;
    if (!xdoc.load_string(inXml.c_str()))
    {
        return result;
    }
    for (const auto &note : xdoc.document_element().child("part").child("measure").children("note"))
    {
        for (const auto &notation : note.child("notations").children("tuplet"))
        {
            result.emplace_back(notation.attribute("type").value(), notation.attribute("number").value());
        }
    }
    return result;
}

inline bool hasDiagnostic(const Diagnostics &inDiagnostics, DiagnosticCode inCode, const std::string &inMessage)
{
    for (const auto &diagnostic : inDiagnostics.all())
    {
        if (diagnostic.code == inCode && diagnostic.message == inMessage)
        {
            return true;
        }
    }
    return false;
}
} // namespace tupletNumberApiTest

// A tuplet inside another tuplet: both are open at once, so they need different numbers. The
// outer one is met first and takes 1.
TEST(nestedIdentityTupletsAreNumberedOneAndTwo, TupletNumber)
{
    using namespace tupletNumberApiTest;
    auto score = makeScore();
    addStart(appendNote(score), SpannerNumber{std::string{"outer"}});
    addStart(appendNote(score), SpannerNumber{std::string{"inner"}});
    addStop(appendNote(score), SpannerNumber{std::string{"inner"}});
    addStop(appendNote(score), SpannerNumber{std::string{"outer"}});

    const auto sequence = tupletSequence(toXml(score));
    REQUIRE(4 == sequence.size());
    CHECK_EQUAL(std::string{"start"}, sequence.at(0).first);
    CHECK_EQUAL(std::string{"1"}, sequence.at(0).second);
    CHECK_EQUAL(std::string{"start"}, sequence.at(1).first);
    CHECK_EQUAL(std::string{"2"}, sequence.at(1).second);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(2).first);
    CHECK_EQUAL(std::string{"2"}, sequence.at(2).second);
    CHECK_EQUAL(std::string{"stop"}, sequence.at(3).first);
    CHECK_EQUAL(std::string{"1"}, sequence.at(3).second);
}

T_END

// Two tuplets that follow one another are never open at the same time, so both are number 1.
TEST(sequentialIdentityTupletsBothGetOne, TupletNumber)
{
    using namespace tupletNumberApiTest;
    auto score = makeScore();
    addStart(appendNote(score), SpannerNumber{std::string{"first"}});
    addStop(appendNote(score), SpannerNumber{std::string{"first"}});
    addStart(appendNote(score), SpannerNumber{std::string{"second"}});
    addStop(appendNote(score), SpannerNumber{std::string{"second"}});

    const auto sequence = tupletSequence(toXml(score));
    REQUIRE(4 == sequence.size());
    CHECK_EQUAL(std::string{"1"}, sequence.at(0).second);
    CHECK_EQUAL(std::string{"1"}, sequence.at(1).second);
    CHECK_EQUAL(std::string{"1"}, sequence.at(2).second);
    CHECK_EQUAL(std::string{"1"}, sequence.at(3).second);
}

T_END

// An author who states the number keeps it.
TEST(explicitLevelsAreWrittenAsGiven, TupletNumber)
{
    using namespace tupletNumberApiTest;
    auto score = makeScore();
    addStart(appendNote(score), SpannerNumber{4});
    addStop(appendNote(score), SpannerNumber{4});

    const auto sequence = tupletSequence(toXml(score));
    REQUIRE(2 == sequence.size());
    CHECK_EQUAL(std::string{"4"}, sequence.at(0).second);
    CHECK_EQUAL(std::string{"4"}, sequence.at(1).second);
}

T_END

// A lone tuplet needs no number, and MusicXML reads a missing one as 1.
TEST(unspecifiedNumberOmitsTheAttribute, TupletNumber)
{
    using namespace tupletNumberApiTest;
    auto score = makeScore();
    addStart(appendNote(score), SpannerNumber{});
    addStop(appendNote(score), SpannerNumber{});

    const auto sequence = tupletSequence(toXml(score));
    REQUIRE(2 == sequence.size());
    CHECK_EQUAL(std::string{}, sequence.at(0).second);
    CHECK_EQUAL(std::string{}, sequence.at(1).second);
}

T_END

// Two tuplets open at once with no numbers cannot be told apart by whoever reads the file, so
// the write says so.
TEST(twoUnnumberedTupletsOpenAtOnceIsReported, TupletNumber)
{
    using namespace tupletNumberApiTest;
    auto score = makeScore();
    addStart(appendNote(score), SpannerNumber{});
    addStart(appendNote(score), SpannerNumber{});
    addStop(appendNote(score), SpannerNumber{});
    addStop(appendNote(score), SpannerNumber{});

    Diagnostics diagnostics;
    const auto document = fromScore(score, diagnostics);
    REQUIRE(document.ok());
    CHECK(hasDiagnostic(diagnostics, DiagnosticCode::missingValueDefaulted,
                        "two tuplets without a number are open at once; readers will default both to 1"));

    // one tuplet at a time is the ordinary case and says nothing
    auto ordinary = makeScore();
    addStart(appendNote(ordinary), SpannerNumber{});
    addStop(appendNote(ordinary), SpannerNumber{});
    Diagnostics quiet;
    REQUIRE(fromScore(ordinary, quiet).ok());
    CHECK(quiet.all().empty());
}

T_END

// A tuplet that never closes is reported like any other unfinished spanner.
TEST(unmatchedTupletStartIsReported, TupletNumber)
{
    using namespace tupletNumberApiTest;
    auto score = makeScore();
    addStart(appendNote(score), SpannerNumber{std::string{"never-closed"}});

    Diagnostics diagnostics;
    REQUIRE(fromScore(score, diagnostics).ok());
    CHECK(hasDiagnostic(diagnostics, DiagnosticCode::unmatchedSpanner, "tuplet start has no matching stop"));
}

T_END

#endif
