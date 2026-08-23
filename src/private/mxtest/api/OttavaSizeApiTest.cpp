// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/OttavaData.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

#include <string>
#include <vector>

using namespace mx::api;

// One <octave-shift> element as it was written, reduced to the two attributes these tests care
// about. An absent size is an empty string.
struct OttavaSizeWrittenShift
{
    std::string type;
    std::string size;
};

// Reads the value of inAttribute from inTag, or an empty string when the attribute is absent.
inline std::string ottavaSizeAttribute(const std::string &inTag, const std::string &inAttribute)
{
    const std::string needle = " " + inAttribute + "=\"";
    const auto valueStart = inTag.find(needle);
    if (valueStart == std::string::npos)
    {
        return {};
    }
    const auto from = valueStart + needle.size();
    const auto to = inTag.find('"', from);
    if (to == std::string::npos)
    {
        return {};
    }
    return inTag.substr(from, to - from);
}

// Every <octave-shift> in the document, in document order.
inline std::vector<OttavaSizeWrittenShift> ottavaSizeWrittenShifts(const std::string &inXml)
{
    std::vector<OttavaSizeWrittenShift> shifts;
    std::string::size_type pos = 0;
    while ((pos = inXml.find("<octave-shift", pos)) != std::string::npos)
    {
        const auto end = inXml.find('>', pos);
        if (end == std::string::npos)
        {
            break;
        }
        const auto tag = inXml.substr(pos, end - pos + 1);
        shifts.push_back(OttavaSizeWrittenShift{ottavaSizeAttribute(tag, "type"), ottavaSizeAttribute(tag, "size")});
        pos = end;
    }
    return shifts;
}

// A part of inMeasureCount measures, each holding one quarter note so the directions have
// somewhere to live.
inline ScoreData ottavaSizeMakeScore(int inMeasureCount)
{
    ScoreData score;
    score.ticksPerQuarter = 10;
    auto &part = score.parts.emplace_back();
    part.uniqueId = "P1";
    for (int i = 0; i < inMeasureCount; ++i)
    {
        auto &measure = part.measures.emplace_back();
        auto &staff = measure.staves.emplace_back();
        auto &note = staff.voices[0].notes.emplace_back();
        note.durationData.durationTimeTicks = 10;
        note.durationData.durationName = DurationName::quarter;
    }
    return score;
}

inline void ottavaSizeAddDirection(ScoreData &ioScore, int inMeasureIndex, const DirectionChoice &inChoice)
{
    DirectionData direction;
    direction.directionTypes.emplace_back(inChoice);
    ioScore.parts.front()
        .measures.at(static_cast<size_t>(inMeasureIndex))
        .staves.front()
        .directions.emplace_back(direction);
}

// Authoring an ottava states its size once, on the start. Every one of the six lines writes the
// matching size onto its stop without the author restating it, and the stop can sit in a later
// measure than its start.
TEST(OttavaStopSizeFollowsStartAcrossMeasures, OttavaSize)
{
    struct Expectation
    {
        OttavaType ottavaType;
        std::string startType;
        std::string size;
    };

    const std::vector<Expectation> expectations = {{OttavaType::o8va, "down", "8"},   {OttavaType::o8vb, "up", "8"},
                                                   {OttavaType::o15ma, "down", "15"}, {OttavaType::o15mb, "up", "15"},
                                                   {OttavaType::o22ma, "down", "22"}, {OttavaType::o22mb, "up", "22"}};

    for (const auto &expectation : expectations)
    {
        auto score = ottavaSizeMakeScore(3);

        OttavaStart start;
        start.ottavaType = expectation.ottavaType;
        ottavaSizeAddDirection(score, 0, DirectionChoice{start});
        ottavaSizeAddDirection(score, 2, DirectionChoice{OttavaStop{}});

        const auto xml = mxtest::toXml(score);
        REQUIRE(!xml.empty());
        const auto shifts = ottavaSizeWrittenShifts(xml);
        REQUIRE(shifts.size() == 2);
        CHECK_EQUAL(expectation.startType, shifts.front().type);
        CHECK_EQUAL(std::string{"stop"}, shifts.back().type);
        CHECK_EQUAL(expectation.size, shifts.back().size);

        // Reading the document back recovers the line, and the stop now records that its source
        // spelled the size out.
        const auto roundTripped = mxtest::roundTrip(score);
        const auto &parts = roundTripped.parts;
        REQUIRE(parts.size() == 1);
        const auto &measures = parts.front().measures;
        REQUIRE(measures.size() == 3);
        const auto &startTypes = measures.front().staves.front().directions.front().directionTypes;
        REQUIRE(startTypes.size() == 1);
        REQUIRE(startTypes.front().isOttavaStart());
        CHECK(expectation.ottavaType == startTypes.front().ottavaStart().ottavaType);
        const auto &stopTypes = measures.back().staves.front().directions.front().directionTypes;
        REQUIRE(stopTypes.size() == 1);
        REQUIRE(stopTypes.front().isOttavaStop());
        CHECK(Bool::yes == stopTypes.front().ottavaStop().writeSize);
    }
}

T_END;

// Two ottavas of different sizes overlap; explicit numbers keep them apart, so each stop takes
// the size of the start that carries its own number rather than the nearest one.
TEST(OttavaStopSizeOverlappingExplicitNumbers, OttavaSize)
{
    auto score = ottavaSizeMakeScore(4);

    OttavaStart outer;
    outer.ottavaType = OttavaType::o15ma;
    outer.spannerStart.number = SpannerNumber(1);
    ottavaSizeAddDirection(score, 0, DirectionChoice{outer});

    OttavaStart inner;
    inner.ottavaType = OttavaType::o8vb;
    inner.spannerStart.number = SpannerNumber(2);
    ottavaSizeAddDirection(score, 1, DirectionChoice{inner});

    OttavaStop innerStop;
    innerStop.spannerStop.number = SpannerNumber(2);
    ottavaSizeAddDirection(score, 2, DirectionChoice{innerStop});

    OttavaStop outerStop;
    outerStop.spannerStop.number = SpannerNumber(1);
    ottavaSizeAddDirection(score, 3, DirectionChoice{outerStop});

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 4);
    CHECK_EQUAL(std::string{"stop"}, shifts.at(2).type);
    CHECK_EQUAL(std::string{"8"}, shifts.at(2).size);
    CHECK_EQUAL(std::string{"stop"}, shifts.at(3).type);
    CHECK_EQUAL(std::string{"15"}, shifts.at(3).size);
}

T_END;

// The same overlap authored with identity spanners, which never serialize their label: the writer
// assigns the MusicXML numbers and each stop still finds its own start.
TEST(OttavaStopSizeOverlappingIdentities, OttavaSize)
{
    auto score = ottavaSizeMakeScore(4);

    OttavaStart outer;
    outer.ottavaType = OttavaType::o22mb;
    outer.spannerStart.number = SpannerNumber(std::string{"outer"});
    ottavaSizeAddDirection(score, 0, DirectionChoice{outer});

    OttavaStart inner;
    inner.ottavaType = OttavaType::o8va;
    inner.spannerStart.number = SpannerNumber(std::string{"inner"});
    ottavaSizeAddDirection(score, 1, DirectionChoice{inner});

    OttavaStop innerStop;
    innerStop.spannerStop.number = SpannerNumber(std::string{"inner"});
    ottavaSizeAddDirection(score, 2, DirectionChoice{innerStop});

    OttavaStop outerStop;
    outerStop.spannerStop.number = SpannerNumber(std::string{"outer"});
    ottavaSizeAddDirection(score, 3, DirectionChoice{outerStop});

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 4);
    CHECK_EQUAL(std::string{"stop"}, shifts.at(2).type);
    CHECK_EQUAL(std::string{"8"}, shifts.at(2).size);
    CHECK_EQUAL(std::string{"stop"}, shifts.at(3).type);
    CHECK_EQUAL(std::string{"22"}, shifts.at(3).size);
}

T_END;

// Two unnumbered ottavas in a row are not concurrent, so each stop closes the start before it.
TEST(OttavaStopSizeSequentialUnnumbered, OttavaSize)
{
    auto score = ottavaSizeMakeScore(4);

    OttavaStart first;
    first.ottavaType = OttavaType::o15mb;
    ottavaSizeAddDirection(score, 0, DirectionChoice{first});
    ottavaSizeAddDirection(score, 1, DirectionChoice{OttavaStop{}});

    OttavaStart second;
    second.ottavaType = OttavaType::o8va;
    ottavaSizeAddDirection(score, 2, DirectionChoice{second});
    ottavaSizeAddDirection(score, 3, DirectionChoice{OttavaStop{}});

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 4);
    CHECK_EQUAL(std::string{"15"}, shifts.at(1).size);
    CHECK_EQUAL(std::string{"8"}, shifts.at(3).size);
}

T_END;

// A stop with no start is an authoring error, not a crash: it writes MusicXML's default size of 8
// and the document still reads back.
TEST(OttavaStopSizeDanglingStopFallsBackToEight, OttavaSize)
{
    auto score = ottavaSizeMakeScore(2);
    ottavaSizeAddDirection(score, 1, DirectionChoice{OttavaStop{}});

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 1);
    CHECK_EQUAL(std::string{"stop"}, shifts.front().type);
    CHECK_EQUAL(std::string{"8"}, shifts.front().size);

    const auto roundTripped = mxtest::roundTrip(score);
    const auto &stopTypes = roundTripped.parts.front().measures.back().staves.front().directions.front().directionTypes;
    REQUIRE(stopTypes.size() == 1);
    CHECK(stopTypes.front().isOttavaStop());
}

T_END;

// writeSize is the one way to suppress the attribute, for a caller who needs the terser spelling.
TEST(OttavaStopWriteSizeNoOmitsTheAttribute, OttavaSize)
{
    auto score = ottavaSizeMakeScore(2);

    OttavaStart start;
    start.ottavaType = OttavaType::o15ma;
    ottavaSizeAddDirection(score, 0, DirectionChoice{start});

    OttavaStop stop;
    stop.writeSize = Bool::no;
    ottavaSizeAddDirection(score, 1, DirectionChoice{stop});

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 2);
    CHECK_EQUAL(std::string{"stop"}, shifts.back().type);
    CHECK(shifts.back().size.empty());

    // The absence survives the trip, because the reader records it.
    const auto roundTripped = mxtest::roundTrip(score);
    const auto &stopTypes = roundTripped.parts.front().measures.back().staves.front().directions.front().directionTypes;
    REQUIRE(stopTypes.size() == 1);
    REQUIRE(stopTypes.front().isOttavaStop());
    CHECK(Bool::no == stopTypes.front().ottavaStop().writeSize);
}

T_END;

// Bool::yes writes the size the start implies -- the same output the unspecified default produces.
TEST(OttavaStopWriteSizeYesMatchesTheDefault, OttavaSize)
{
    auto score = ottavaSizeMakeScore(2);

    OttavaStart start;
    start.ottavaType = OttavaType::o22ma;
    ottavaSizeAddDirection(score, 0, DirectionChoice{start});

    OttavaStop stop;
    stop.writeSize = Bool::yes;
    ottavaSizeAddDirection(score, 1, DirectionChoice{stop});

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 2);
    CHECK_EQUAL(std::string{"22"}, shifts.back().size);
}

T_END;

// A source whose stop size contradicts its start is normalized: the start states the octave shift,
// so the stop is rewritten to agree with it.
TEST(OttavaStopSizeContradictionIsNormalizedToTheStart, OttavaSize)
{
    const std::string sourceXml =
        R"(<?xml version="1.0" encoding="UTF-8"?>)"
        R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 4.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)"
        R"(<score-partwise version="4.0"><part-list><score-part id="P1"><part-name>P</part-name></score-part></part-list>)"
        R"(<part id="P1"><measure number="1"><attributes><divisions>1</divisions></attributes>)"
        R"(<direction><direction-type><octave-shift type="down" size="15"/></direction-type></direction>)"
        R"(<note><pitch><step>C</step><octave>4</octave></pitch><duration>1</duration><type>quarter</type></note>)"
        R"(<direction><direction-type><octave-shift type="stop" size="8"/></direction-type></direction>)"
        R"(</measure></part></score-partwise>)";

    const auto score = mxtest::fromXml(sourceXml);
    REQUIRE(score.parts.size() == 1);

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 2);
    CHECK_EQUAL(std::string{"15"}, shifts.front().size);
    CHECK_EQUAL(std::string{"stop"}, shifts.back().type);
    CHECK_EQUAL(std::string{"15"}, shifts.back().size);
}

T_END;

// MusicXML's schema allows any positive integer in octave-shift/@size, so a file can ask for a
// line music notation has no name for. The api narrows it to the closest line it can draw, which
// is what lets everything downstream assume an ottava is one of the six real ones. A size between
// the 8th and the 15th takes the 15th.
TEST(OttavaSizeBetweenTheLinesIsNarrowedToTheFifteenth, OttavaSize)
{
    const std::string sourceXml =
        R"(<?xml version="1.0" encoding="UTF-8"?>)"
        R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 4.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)"
        R"(<score-partwise version="4.0"><part-list><score-part id="P1"><part-name>P</part-name></score-part></part-list>)"
        R"(<part id="P1"><measure number="1"><attributes><divisions>1</divisions></attributes>)"
        R"(<direction><direction-type><octave-shift type="down" size="11"/></direction-type></direction>)"
        R"(<note><pitch><step>C</step><octave>4</octave></pitch><duration>1</duration><type>quarter</type></note>)"
        R"(<direction><direction-type><octave-shift type="stop" size="11"/></direction-type></direction>)"
        R"(</measure></part></score-partwise>)";

    const auto score = mxtest::fromXml(sourceXml);
    REQUIRE(score.parts.size() == 1);
    const auto &directions = score.parts.front().measures.front().staves.front().directions;
    REQUIRE(directions.size() == 2);
    const auto &startTypes = directions.front().directionTypes;
    REQUIRE(startTypes.size() == 1);
    REQUIRE(startTypes.front().isOttavaStart());
    CHECK(startTypes.front().ottavaStart().ottavaType == OttavaType::o15ma);

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 2);
    CHECK_EQUAL(std::string{"15"}, shifts.front().size);
    CHECK_EQUAL(std::string{"15"}, shifts.back().size);
}

T_END;

// A size past the 22nd asks for more than the widest line we can draw, so it takes that widest
// line. Falling back to the 15th would move the reader further from what the source asked for.
TEST(OttavaSizePastTheTwentySecondIsNarrowedToTheTwentySecond, OttavaSize)
{
    const std::string sourceXml =
        R"(<?xml version="1.0" encoding="UTF-8"?>)"
        R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 4.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)"
        R"(<score-partwise version="4.0"><part-list><score-part id="P1"><part-name>P</part-name></score-part></part-list>)"
        R"(<part id="P1"><measure number="1"><attributes><divisions>1</divisions></attributes>)"
        R"(<direction><direction-type><octave-shift type="down" size="27"/></direction-type></direction>)"
        R"(<note><pitch><step>C</step><octave>4</octave></pitch><duration>1</duration><type>quarter</type></note>)"
        R"(<direction><direction-type><octave-shift type="stop" size="27"/></direction-type></direction>)"
        R"(</measure></part></score-partwise>)";

    const auto score = mxtest::fromXml(sourceXml);
    REQUIRE(score.parts.size() == 1);
    const auto &directions = score.parts.front().measures.front().staves.front().directions;
    REQUIRE(directions.size() == 2);
    const auto &startTypes = directions.front().directionTypes;
    REQUIRE(startTypes.size() == 1);
    REQUIRE(startTypes.front().isOttavaStart());
    CHECK(startTypes.front().ottavaStart().ottavaType == OttavaType::o22ma);

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 2);
    CHECK_EQUAL(std::string{"22"}, shifts.front().size);
    CHECK_EQUAL(std::string{"22"}, shifts.back().size);
}

T_END;

// A size below 8 asks for less than an octave, which an ottava cannot draw either. It becomes a
// plain octave line, and the size the source spelled out stays spelled out.
TEST(OttavaSizeBelowEightIsNarrowedToAnOctave, OttavaSize)
{
    const std::string sourceXml =
        R"(<?xml version="1.0" encoding="UTF-8"?>)"
        R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 4.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)"
        R"(<score-partwise version="4.0"><part-list><score-part id="P1"><part-name>P</part-name></score-part></part-list>)"
        R"(<part id="P1"><measure number="1"><attributes><divisions>1</divisions></attributes>)"
        R"(<direction><direction-type><octave-shift type="up" size="1"/></direction-type></direction>)"
        R"(<note><pitch><step>C</step><octave>4</octave></pitch><duration>1</duration><type>quarter</type></note>)"
        R"(</measure></part></score-partwise>)";

    const auto score = mxtest::fromXml(sourceXml);
    REQUIRE(score.parts.size() == 1);
    const auto &startTypes = score.parts.front().measures.front().staves.front().directions.front().directionTypes;
    REQUIRE(startTypes.size() == 1);
    REQUIRE(startTypes.front().isOttavaStart());
    CHECK(startTypes.front().ottavaStart().ottavaType == OttavaType::o8vb);

    const auto xml = mxtest::toXml(score);
    REQUIRE(!xml.empty());
    const auto shifts = ottavaSizeWrittenShifts(xml);
    REQUIRE(shifts.size() == 1);
    CHECK_EQUAL(std::string{"8"}, shifts.front().size);
}

T_END;

#endif
