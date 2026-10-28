// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/api/TestHelpers.h"

#include <string>

using namespace mx::api;

namespace mxtest::api::for_part_test
{
// One part, one measure in 4/4 holding two half notes, at 2 ticks per quarter.
ScoreData makeForPartScore()
{
    ScoreData score;
    score.ticksPerQuarter = 2;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.uniqueId = "P1";
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &voice = measure.staves.back().voices[0];
    for (int tick : {0, 4})
    {
        NoteData note;
        note.pitchData.step = Step::c;
        note.durationData.durationName = DurationName::half;
        note.durationData.durationTimeTicks = 4;
        note.tickTimePosition = tick;
        voice.notes.push_back(note);
    }
    return score;
}

ForPartData makeBassClarinet()
{
    ForPartData forPart;
    forPart.chromatic = -14;
    forPart.diatonic = -8;
    forPart.partClef = PartClefData{};
    return forPart;
}

int countOccurrences(const std::string &haystack, const std::string &needle)
{
    int count = 0;
    for (auto pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + 1))
    {
        ++count;
    }
    return count;
}

Diagnostics forPartWriteDiagnostics(const ScoreData &score)
{
    Diagnostics diagnostics;
    REQUIRE(fromScore(score, diagnostics).ok());
    return diagnostics;
}

int countDiagnostics(const Diagnostics &diagnostics, Severity severity, DiagnosticCode code)
{
    int count = 0;
    for (const auto &diagnostic : diagnostics.all())
    {
        if (diagnostic.severity == severity && diagnostic.code == code)
        {
            ++count;
        }
    }
    return count;
}
} // namespace mxtest::api::for_part_test

TEST(forPartRoundTrip, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    auto forPart = makeBassClarinet();
    forPart.id = Id{"bcl"};
    score.parts.front().measures.front().forParts.push_back(forPart);

    const auto out = mxtest::fromXml(mxtest::toXml(score));
    REQUIRE(out.parts.size() == 1);
    const auto &forParts = out.parts.front().measures.front().forParts;
    REQUIRE(forParts.size() == 1);
    CHECK(forPart == forParts.front());
}

T_END;

TEST(forPartWritesConcertScore, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    CHECK(!score.concertScore);
    score.parts.front().measures.front().forParts.push_back(makeBassClarinet());

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<concert-score") != std::string::npos);
    CHECK(xml.find("<part-clef>") != std::string::npos);
    CHECK(xml.find("<octave-change>-1</octave-change>") != std::string::npos);
    CHECK(xml.find("<chromatic>-2</chromatic>") != std::string::npos);
    CHECK(xml.find("<diatonic>-1</diatonic>") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK(out.concertScore);
}

T_END;

TEST(concertScoreWithoutForPart, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    CHECK(mxtest::toXml(score).find("<concert-score") == std::string::npos);

    score.concertScore = true;
    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<concert-score") != std::string::npos);
    CHECK(mxtest::fromXml(xml).concertScore);
}

T_END;

TEST(forPartMidMeasure, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    ForPartData forPart;
    forPart.chromatic = -2;
    forPart.diatonic = -1;
    forPart.tickTimePosition = 4;
    score.parts.front().measures.front().forParts.push_back(forPart);

    const auto xml = mxtest::toXml(score);
    const auto forPartPos = xml.find("<for-part");
    REQUIRE(forPartPos != std::string::npos);
    const auto firstNotePos = xml.find("<note");
    const auto secondNotePos = xml.find("<note", firstNotePos + 1);
    CHECK(firstNotePos < forPartPos);
    CHECK(forPartPos < secondNotePos);

    const auto out = mxtest::fromXml(xml);
    const auto &forParts = out.parts.front().measures.front().forParts;
    REQUIRE(forParts.size() == 1);
    CHECK_EQUAL(4, forParts.front().tickTimePosition);
    CHECK(forPart == forParts.front());
}

T_END;

TEST(forPartAndTransposeAtSameTick, ForPartData)
{
    // MusicXML does not allow <transpose> and <for-part> in the same <attributes>, so a piccolo's
    // octave transposition and its for-part go into separate <attributes> elements.
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    auto &measure = score.parts.front().measures.front();
    measure.transpositions.push_back(TransposeData{12, 7});
    ForPartData forPart;
    forPart.chromatic = 12;
    forPart.diatonic = 7;
    measure.forParts.push_back(forPart);

    const auto xml = mxtest::toXml(score);
    CHECK_EQUAL(2, countOccurrences(xml, "<attributes>"));
    CHECK_EQUAL(1, countOccurrences(xml, "<transpose>"));
    CHECK_EQUAL(1, countOccurrences(xml, "<for-part>"));

    const auto out = mxtest::fromXml(xml);
    const auto &outMeasure = out.parts.front().measures.front();
    REQUIRE(outMeasure.transpositions.size() == 1);
    CHECK_EQUAL(12, outMeasure.transpositions.front().chromatic);
    REQUIRE(outMeasure.forParts.size() == 1);
    CHECK(forPart == outMeasure.forParts.front());
}

T_END;

TEST(forPartStaffNumber, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    auto &measure = score.parts.front().measures.front();
    measure.staves.emplace_back();
    ForPartData forPart;
    forPart.chromatic = -7;
    forPart.diatonic = -4;
    forPart.staffIndex = 1;
    measure.forParts.push_back(forPart);

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<for-part number=\"2\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    const auto &forParts = out.parts.front().measures.front().forParts;
    REQUIRE(forParts.size() == 1);
    CHECK_EQUAL(1, forParts.front().staffIndex);
}

T_END;

TEST(forPartReadDefaults, ForPartData)
{
    // A part-clef without <line> takes the usual line for its sign, and a part-transpose without
    // <diatonic> has it guessed the way <transpose> does.
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <defaults>
    <concert-score/>
  </defaults>
  <part-list>
    <score-part id="P1">
      <part-name>Horn</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <for-part>
          <part-clef>
            <sign>F</sign>
          </part-clef>
          <part-transpose>
            <chromatic>-7</chromatic>
          </part-transpose>
        </for-part>
      </attributes>
      <note>
        <rest measure="yes"/>
        <duration>4</duration>
      </note>
    </measure>
  </part>
</score-partwise>
)";
    const auto out = mxtest::fromXml(xml);
    CHECK(out.concertScore);
    REQUIRE(out.parts.size() == 1);
    const auto &forParts = out.parts.front().measures.front().forParts;
    REQUIRE(forParts.size() == 1);
    const auto &forPart = forParts.front();
    CHECK_EQUAL(-7, forPart.chromatic);
    CHECK_EQUAL(-4, forPart.diatonic);
    CHECK_EQUAL(INDEX_UNSPECIFIED, forPart.staffIndex);
    REQUIRE(forPart.partClef.has_value());
    CHECK(ClefSymbol::f == forPart.partClef->symbol);
    CHECK_EQUAL(4, forPart.partClef->line);
    CHECK_EQUAL(0, forPart.partClef->octaveChange);
}

T_END;

TEST(forPartEquality, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    const auto a = makeBassClarinet();
    auto b = a;
    CHECK(a == b);
    b.partClef->line = 4;
    CHECK(a != b);
    b = a;
    b.tickTimePosition = 1;
    CHECK(a != b);
    b = a;
    b.staffIndex = 0;
    CHECK(a != b);

    MeasureData m1;
    MeasureData m2;
    m1.forParts.push_back(a);
    CHECK(m1 != m2);

    ScoreData s1;
    ScoreData s2;
    s2.concertScore = true;
    CHECK(s1 != s2);
}

T_END;

TEST(forPartImpliesConcertScoreReportsInfo, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    score.parts.front().measures.front().forParts.push_back(makeBassClarinet());

    const auto inferred = forPartWriteDiagnostics(score);
    REQUIRE(1 == countDiagnostics(inferred, Severity::info, DiagnosticCode::valueAdjusted));
    CHECK_EQUAL(0, inferred.all().front().location.partIndex);
    CHECK_EQUAL(0, inferred.all().front().location.measureIndex);

    score.concertScore = true;
    CHECK(forPartWriteDiagnostics(score).all().empty());
}

T_END;

TEST(concertScoreTransposeReportsSpecConflict, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    score.concertScore = true;
    score.parts.front().transposition = TransposeData{-14, -8};

    const auto diagnostics = forPartWriteDiagnostics(score);
    REQUIRE(1 == countDiagnostics(diagnostics, Severity::warning, DiagnosticCode::specConflict));

    // the transposition is still written as given
    const auto out = mxtest::fromXml(mxtest::toXml(score));
    REQUIRE(out.parts.front().transposition.has_value());
    CHECK_EQUAL(-14, out.parts.front().transposition->chromatic);
}

T_END;

TEST(concertScoreInferredTransposeReportsSpecConflict, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    auto &measure = score.parts.front().measures.front();
    measure.transpositions.push_back(TransposeData{-2, -1});
    measure.forParts.push_back(makeBassClarinet());

    const auto diagnostics = forPartWriteDiagnostics(score);
    CHECK_EQUAL(1, countDiagnostics(diagnostics, Severity::warning, DiagnosticCode::specConflict));
}

T_END;

TEST(concertScoreOctaveTransposeIsAllowed, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    score.concertScore = true;
    score.parts.front().transposition = TransposeData{12, 7};
    CHECK(forPartWriteDiagnostics(score).all().empty());
}

T_END;

TEST(transposeWithoutConcertScoreIsAllowed, ForPartData)
{
    using namespace mxtest::api::for_part_test;
    auto score = makeForPartScore();
    score.parts.front().transposition = TransposeData{-2, -1};
    CHECK(forPartWriteDiagnostics(score).all().empty());
}

T_END;

#endif
