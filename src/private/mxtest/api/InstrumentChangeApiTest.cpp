// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/api/TestHelpers.h"

#include <sstream>
#include <string>

using namespace mx::api;

namespace mxtest::api::instrument_change_test
{
// A flute part of two measures, each one whole note, at 1 tick per quarter.
ScoreData makeFluteScore()
{
    ScoreData score;
    score.ticksPerQuarter = 1;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.uniqueId = "P1";
    part.name = "Flute";
    part.instrumentData.uniqueId = "P1-I1";
    part.instrumentData.name = "Flute";
    part.instrumentData.soundID = SoundID::windFlutesFlute;
    for (int i = 0; i < 2; ++i)
    {
        part.measures.emplace_back();
        auto &measure = part.measures.back();
        measure.staves.emplace_back();
        NoteData note;
        note.pitchData.step = Step::c;
        note.durationData.durationName = DurationName::whole;
        note.durationData.durationTimeTicks = 4;
        measure.staves.back().voices[0].notes.push_back(note);
    }
    return score;
}

InstrumentChangeData makePiccolo()
{
    InstrumentChangeData change;
    change.soundID = SoundID::windFlutesFlutePiccolo;
    change.soloOrEnsemble = SoloOrEnsemble::solo;
    change.virtualLibrary = "Library";
    change.virtualName = "Piccolo";
    return change;
}

// Adds a sound-only direction carrying the change at the start of the given measure.
void addInstrumentChange(ScoreData &score, int measureIndex, const InstrumentChangeData &change)
{
    DirectionData direction;
    direction.isSoundDataSpecified = true;
    direction.soundData.instrumentChange = change;
    score.parts.front().measures.at(measureIndex).staves.front().directions.push_back(direction);
}

std::optional<InstrumentChangeData> findInstrumentChange(const ScoreData &score, int measureIndex)
{
    for (const auto &direction : score.parts.front().measures.at(measureIndex).staves.front().directions)
    {
        if (direction.isSoundDataSpecified && direction.soundData.instrumentChange.has_value())
        {
            return direction.soundData.instrumentChange;
        }
    }
    return std::nullopt;
}

ScoreData readWithDiagnostics(const std::string &xml, Diagnostics &diagnostics)
{
    std::istringstream stream{xml};
    auto document = MusicXml::fromStream(stream);
    REQUIRE(document.ok());
    auto score = intoScore(std::move(document).value(), diagnostics);
    REQUIRE(score.ok());
    return score.value();
}

// A part with two score-instruments: a change for the second one is outside the api's
// one-instrument model.
std::string twoInstrumentXml()
{
    return R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1">
      <part-name>Flute</part-name>
      <score-instrument id="P1-I1">
        <instrument-name>Flute</instrument-name>
      </score-instrument>
      <score-instrument id="P1-I2">
        <instrument-name>Piccolo</instrument-name>
      </score-instrument>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <sound>
        <instrument-change id="P1-I2">
          <instrument-sound>wind.flutes.flute.piccolo</instrument-sound>
        </instrument-change>
      </sound>
      <sound>
        <instrument-change id="P1-I1">
          <instrument-sound>wind.flutes.flute</instrument-sound>
        </instrument-change>
      </sound>
      <note>
        <rest measure="yes"/>
        <duration>4</duration>
      </note>
    </measure>
  </part>
</score-partwise>
)";
}
} // namespace mxtest::api::instrument_change_test

TEST(instrumentChangeRoundTrip, InstrumentChangeData)
{
    using namespace mxtest::api::instrument_change_test;
    auto score = makeFluteScore();
    addInstrumentChange(score, 1, makePiccolo());

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<instrument-change id=\"P1-I1\">") != std::string::npos);
    CHECK(xml.find("<instrument-sound>wind.flutes.flute.piccolo</instrument-sound>") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK(!findInstrumentChange(out, 0).has_value());
    const auto change = findInstrumentChange(out, 1);
    REQUIRE(change.has_value());
    CHECK(makePiccolo() == *change);
    CHECK(SoundID::windFlutesFlute == out.parts.front().instrumentData.soundID);
}

T_END;

TEST(instrumentChangeRefersToSynthesizedInstrument, InstrumentChangeData)
{
    // A part that sets nothing about its instrument still gets a score-instrument for the change
    // to refer to.
    using namespace mxtest::api::instrument_change_test;
    auto score = makeFluteScore();
    score.parts.front().instrumentData = InstrumentData{};
    addInstrumentChange(score, 1, makePiccolo());

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<score-instrument id=\"ID1000000\">") != std::string::npos);
    CHECK(xml.find("<instrument-change id=\"ID1000000\">") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    const auto change = findInstrumentChange(out, 1);
    REQUIRE(change.has_value());
    CHECK(makePiccolo() == *change);
}

T_END;

TEST(instrumentChangeWithoutChangeWritesNoScoreInstrument, InstrumentChangeData)
{
    auto score = mxtest::api::instrument_change_test::makeFluteScore();
    score.parts.front().instrumentData = InstrumentData{};
    CHECK(mxtest::toXml(score).find("<score-instrument") == std::string::npos);
}

T_END;

TEST(instrumentChangeInDirection, InstrumentChangeData)
{
    // A change on a direction with words is written inside that direction's <sound>.
    using namespace mxtest::api::instrument_change_test;
    auto score = makeFluteScore();
    DirectionData direction;
    WordsData words;
    words.text = "to Piccolo";
    direction.directionTypes.emplace_back(DirectionChoice{std::vector<WordsChoice>{WordsChoice{words}}});
    direction.isSoundDataSpecified = true;
    direction.soundData.instrumentChange = makePiccolo();
    score.parts.front().measures.at(1).staves.front().directions.push_back(direction);

    const auto xml = mxtest::toXml(score);
    const auto directionPos = xml.find("<direction");
    const auto changePos = xml.find("<instrument-change");
    REQUIRE(directionPos != std::string::npos);
    REQUIRE(changePos != std::string::npos);
    CHECK(directionPos < changePos);
    CHECK(changePos < xml.find("</direction>"));

    const auto change = findInstrumentChange(mxtest::fromXml(xml), 1);
    REQUIRE(change.has_value());
    CHECK(makePiccolo() == *change);
}

T_END;

TEST(instrumentChangeForOtherInstrumentIsDropped, InstrumentChangeData)
{
    using namespace mxtest::api::instrument_change_test;
    Diagnostics diagnostics;
    const auto score = readWithDiagnostics(twoInstrumentXml(), diagnostics);

    int changes = 0;
    for (const auto &direction : score.parts.front().measures.front().staves.front().directions)
    {
        if (direction.soundData.instrumentChange.has_value())
        {
            ++changes;
            CHECK(SoundID::windFlutesFlute == direction.soundData.instrumentChange->soundID);
        }
    }
    CHECK_EQUAL(1, changes);

    int dropped = 0;
    for (const auto &diagnostic : diagnostics.all())
    {
        if (diagnostic.severity == Severity::warning && diagnostic.code == DiagnosticCode::droppedData)
        {
            ++dropped;
        }
    }
    CHECK_EQUAL(1, dropped);
}

T_END;

TEST(instrumentChangeEquality, InstrumentChangeData)
{
    using namespace mxtest::api::instrument_change_test;
    const auto a = makePiccolo();
    auto b = a;
    CHECK(a == b);
    b.virtualName = "Flute";
    CHECK(a != b);

    SoundData sound;
    CHECK(!sound.isSpecified());
    sound.instrumentChange = InstrumentChangeData{};
    CHECK(sound.isSpecified());
    CHECK(sound != SoundData{});
}

T_END;

TEST(partNameDisplayRoundTrip, NameDisplayData)
{
    using namespace mxtest::api::instrument_change_test;
    auto score = makeFluteScore();
    auto &measure = score.parts.front().measures.at(1);
    NameDisplayData name;
    name.text = "Piccolo";
    measure.partNameDisplay = name;
    NameDisplayData abbreviation;
    abbreviation.text = "Picc.";
    abbreviation.printData.printObject = Bool::no;
    measure.partAbbreviationDisplay = abbreviation;
    measure.measureNumbering = MeasureNumbering::system;

    const auto xml = mxtest::toXml(score);
    const auto printPos = xml.find("<print");
    REQUIRE(printPos != std::string::npos);
    CHECK(xml.find("<print", printPos + 1) == std::string::npos);
    CHECK(xml.find("<part-abbreviation-display print-object=\"no\">") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    const auto &outMeasure = out.parts.front().measures.at(1);
    REQUIRE(outMeasure.partNameDisplay.has_value());
    CHECK(name == *outMeasure.partNameDisplay);
    REQUIRE(outMeasure.partAbbreviationDisplay.has_value());
    CHECK(abbreviation == *outMeasure.partAbbreviationDisplay);
    CHECK(MeasureNumbering::system == outMeasure.measureNumbering);
    CHECK(!out.parts.front().measures.at(0).partNameDisplay.has_value());
}

T_END;

TEST(partNameDisplayWithNewSystem, NameDisplayData)
{
    // The name display shares the <print> that starts the new system.
    using namespace mxtest::api::instrument_change_test;
    auto score = makeFluteScore();
    SystemData system;
    system.newSystem = Bool::yes;
    score.layout[1].system = system;
    NameDisplayData name;
    name.text = "Piccolo";
    score.parts.front().measures.at(1).partNameDisplay = name;

    const auto xml = mxtest::toXml(score);
    const auto printPos = xml.find("<print new-system=\"yes\">");
    REQUIRE(printPos != std::string::npos);
    CHECK(xml.find("<print", printPos + 1) == std::string::npos);
    CHECK(printPos < xml.find("<part-name-display>", printPos));

    const auto out = mxtest::fromXml(xml);
    REQUIRE(out.parts.front().measures.at(1).partNameDisplay.has_value());
    CHECK(name == *out.parts.front().measures.at(1).partNameDisplay);
}

T_END;

TEST(nameDisplayEquality, NameDisplayData)
{
    NameDisplayData a;
    a.text = "Piccolo";
    auto b = a;
    CHECK(a == b);
    b.printData.printObject = Bool::no;
    CHECK(a != b);

    MeasureData m1;
    MeasureData m2;
    m1.partNameDisplay = a;
    CHECK(m1 != m2);
    m2.partNameDisplay = a;
    CHECK(m1 == m2);
    m2.partAbbreviationDisplay = a;
    CHECK(m1 != m2);
}

T_END;

#endif
