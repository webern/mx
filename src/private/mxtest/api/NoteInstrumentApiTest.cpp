// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

#include <string>

using namespace mx::api;

InstrumentData noteInstrumentMake(const std::string &id, const std::string &name, int unpitched)
{
    InstrumentData instrument;
    instrument.uniqueId = id;
    instrument.name = name;
    instrument.midiData.channel = 10;
    instrument.midiData.program = 1;
    instrument.midiData.unpitched = unpitched;
    return instrument;
}

// A drum kit part of one measure with three instruments and one quarter note for each.
ScoreData noteInstrumentDrumKitScore()
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    auto &part = score.parts.emplace_back();
    part.uniqueId = "P1";
    part.name = "Drum Kit";

    part.instrumentData = noteInstrumentMake("P1-I36", "Bass Drum", 36);
    part.instrumentData.abbreviation = "B.D.";
    part.instrumentData.midiData.isVolumeSpecified = true;
    part.instrumentData.midiData.volume = 78.5;

    auto snare = noteInstrumentMake("P1-I39", "Snare", 39);
    snare.abbreviation = "S.D.";
    snare.midiData.isPanSpecified = true;
    snare.midiData.pan = -20.5;
    part.additionalInstruments.push_back(snare);

    auto hiHat = noteInstrumentMake("P1-I43", "Hi-Hat", 43);
    hiHat.midiData.isElevationSpecified = true;
    hiHat.midiData.elevation = 10.5;
    hiHat.midiData.device = "Kit";
    hiHat.midiData.devicePort = 2;
    part.additionalInstruments.push_back(hiHat);

    auto &measure = part.measures.emplace_back();
    auto &staff = measure.staves.emplace_back();
    const std::vector<std::string> ids{"P1-I36", "P1-I39", "P1-I43"};
    for (int i = 0; i < 3; ++i)
    {
        auto &note = staff.voices[0].notes.emplace_back();
        note.isUnpitched = true;
        note.pitchData.step = Step::f;
        note.pitchData.octave = 4;
        note.durationData.durationName = DurationName::quarter;
        note.durationData.durationTimeTicks = 4;
        note.tickTimePosition = 4 * i;
        note.instruments.push_back(ids.at(static_cast<size_t>(i)));
    }
    return score;
}

const NoteData &noteInstrumentNote(const ScoreData &score, size_t index)
{
    return score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(index);
}

TEST(drumKitWriteThenRead, NoteInstrument)
{
    const auto score = noteInstrumentDrumKitScore();
    const auto out = mxtest::roundTrip(score);
    REQUIRE(out.parts.size() == 1);
    const auto &part = out.parts.front();

    CHECK(score.parts.front().instrumentData == part.instrumentData);
    REQUIRE(part.additionalInstruments.size() == 2);
    CHECK(score.parts.front().additionalInstruments.at(0) == part.additionalInstruments.at(0));

    // An additional instrument's device is linked to it by default, and reads back as linked.
    auto expectedHiHat = score.parts.front().additionalInstruments.at(1);
    expectedHiHat.midiData.writeDeviceId = Bool::yes;
    CHECK(expectedHiHat == part.additionalInstruments.at(1));

    CHECK(std::vector<std::string>{"P1-I36"} == noteInstrumentNote(out, 0).instruments);
    CHECK(std::vector<std::string>{"P1-I39"} == noteInstrumentNote(out, 1).instruments);
    CHECK(std::vector<std::string>{"P1-I43"} == noteInstrumentNote(out, 2).instruments);
}

T_END;

TEST(drumKitWritesEveryInstrument, NoteInstrument)
{
    const auto xml = mxtest::toXml(noteInstrumentDrumKitScore());
    CHECK(xml.find("<score-instrument id=\"P1-I36\">") != std::string::npos);
    CHECK(xml.find("<score-instrument id=\"P1-I39\">") != std::string::npos);
    CHECK(xml.find("<score-instrument id=\"P1-I43\">") != std::string::npos);
    CHECK(xml.find("<midi-instrument id=\"P1-I39\">") != std::string::npos);
    CHECK(xml.find("<midi-device port=\"2\" id=\"P1-I43\">Kit</midi-device>") != std::string::npos);
    CHECK(xml.find("<midi-unpitched>43</midi-unpitched>") != std::string::npos);
    CHECK(xml.find("<instrument id=\"P1-I36\" />") != std::string::npos);
    CHECK(xml.find("<instrument id=\"P1-I39\" />") != std::string::npos);
    CHECK(xml.find("<instrument id=\"P1-I43\" />") != std::string::npos);
    CHECK(xml.find("<score-instrument id=\"P1-I36\">") < xml.find("<score-instrument id=\"P1-I39\">"));
}

T_END;

std::string noteInstrumentDrumKitXml()
{
    return R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1">
      <part-name>Drum Kit</part-name>
      <score-instrument id="P1-I36">
        <instrument-name>Bass Drum</instrument-name>
      </score-instrument>
      <score-instrument id="P1-I39">
        <instrument-name>Snare</instrument-name>
        <instrument-abbreviation>S.D.</instrument-abbreviation>
      </score-instrument>
      <score-instrument id="P1-I43">
        <instrument-name>Hi-Hat</instrument-name>
      </score-instrument>
      <midi-instrument id="P1-I36">
        <midi-channel>10</midi-channel>
        <midi-unpitched>36</midi-unpitched>
      </midi-instrument>
      <midi-instrument id="P1-I39">
        <midi-channel>11</midi-channel>
        <midi-program>2</midi-program>
        <midi-unpitched>39</midi-unpitched>
        <volume>60.5</volume>
      </midi-instrument>
      <midi-device>Kit</midi-device>
      <midi-instrument id="P1-I43">
        <midi-channel>12</midi-channel>
        <midi-unpitched>43</midi-unpitched>
      </midi-instrument>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <note>
        <unpitched>
          <display-step>F</display-step>
          <display-octave>4</display-octave>
        </unpitched>
        <duration>1</duration>
        <instrument id="P1-I36"/>
        <type>quarter</type>
      </note>
      <note>
        <unpitched>
          <display-step>C</display-step>
          <display-octave>5</display-octave>
        </unpitched>
        <duration>1</duration>
        <instrument id="P1-I39"/>
        <type>quarter</type>
      </note>
      <note>
        <unpitched>
          <display-step>G</display-step>
          <display-octave>5</display-octave>
        </unpitched>
        <duration>2</duration>
        <instrument id="P1-I43"/>
        <type>half</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";
}

TEST(drumKitReadFromXml, NoteInstrument)
{
    const auto score = mxtest::fromXml(noteInstrumentDrumKitXml());
    REQUIRE(score.parts.size() == 1);
    const auto &part = score.parts.front();

    CHECK_EQUAL("P1-I36", part.instrumentData.uniqueId);
    CHECK_EQUAL("Bass Drum", part.instrumentData.name);
    CHECK_EQUAL(10, part.instrumentData.midiData.channel);
    CHECK_EQUAL(36, part.instrumentData.midiData.unpitched);
    CHECK(Bool::unspecified == part.instrumentData.midiData.writeDeviceId);

    REQUIRE(part.additionalInstruments.size() == 2);
    const auto &snare = part.additionalInstruments.at(0);
    CHECK_EQUAL("P1-I39", snare.uniqueId);
    CHECK_EQUAL("Snare", snare.name);
    CHECK_EQUAL("S.D.", snare.abbreviation);
    CHECK_EQUAL(11, snare.midiData.channel);
    CHECK_EQUAL(2, snare.midiData.program);
    CHECK_EQUAL(39, snare.midiData.unpitched);
    CHECK(snare.midiData.isVolumeSpecified);
    CHECK_DOUBLES_EQUAL(60.5, snare.midiData.volume, 0.0001);
    CHECK(snare.midiData.device.empty());

    const auto &hiHat = part.additionalInstruments.at(1);
    CHECK_EQUAL("P1-I43", hiHat.uniqueId);
    CHECK_EQUAL("Hi-Hat", hiHat.name);
    CHECK_EQUAL(12, hiHat.midiData.channel);
    CHECK_EQUAL(43, hiHat.midiData.unpitched);
    CHECK_EQUAL("Kit", hiHat.midiData.device);
    CHECK(Bool::no == hiHat.midiData.writeDeviceId);

    CHECK(std::vector<std::string>{"P1-I36"} == noteInstrumentNote(score, 0).instruments);
    CHECK(std::vector<std::string>{"P1-I39"} == noteInstrumentNote(score, 1).instruments);
    CHECK(std::vector<std::string>{"P1-I43"} == noteInstrumentNote(score, 2).instruments);
}

T_END;

TEST(unlinkedDeviceStaysUnlinked, NoteInstrument)
{
    const auto xml = mxtest::toXml(mxtest::fromXml(noteInstrumentDrumKitXml()));
    CHECK(xml.find("<midi-device>Kit</midi-device>") != std::string::npos);
    CHECK(xml.find("<midi-device id=") == std::string::npos);
}

T_END;

TEST(noteNamesTwoInstruments, NoteInstrument)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    auto &part = score.parts.emplace_back();
    part.uniqueId = "P1";
    part.name = "Clarinet 1 & 2";
    part.instrumentData.uniqueId = "P1-I1";
    part.instrumentData.name = "Clarinet 1";
    auto &second = part.additionalInstruments.emplace_back();
    second.uniqueId = "P1-I2";
    second.name = "Clarinet 2";
    auto &note = part.measures.emplace_back().staves.emplace_back().voices[0].notes.emplace_back();
    note.pitchData.step = Step::g;
    note.pitchData.octave = 4;
    note.durationData.durationName = DurationName::whole;
    note.durationData.durationTimeTicks = 16;
    note.instruments = {"P1-I1", "P1-I2"};

    const auto xml = mxtest::toXml(score);
    const auto first = xml.find("<instrument id=\"P1-I1\" />");
    const auto other = xml.find("<instrument id=\"P1-I2\" />");
    REQUIRE(first != std::string::npos);
    REQUIRE(other != std::string::npos);
    CHECK(first < other);

    const auto out = mxtest::fromXml(xml);
    REQUIRE(out.parts.size() == 1);
    CHECK(std::vector<std::string>({"P1-I1", "P1-I2"}) == noteInstrumentNote(out, 0).instruments);
    REQUIRE(out.parts.front().additionalInstruments.size() == 1);
    CHECK_EQUAL("Clarinet 2", out.parts.front().additionalInstruments.front().name);
}

T_END;

TEST(defaultWritesNoInstrument, NoteInstrument)
{
    auto score = noteInstrumentDrumKitScore();
    auto &part = score.parts.front();
    part.additionalInstruments.clear();
    for (auto &note : part.measures.front().staves.front().voices[0].notes)
    {
        note.instruments.clear();
    }

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<instrument ") == std::string::npos);
    CHECK(xml.find("<score-instrument id=\"P1-I36\">") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK(noteInstrumentNote(out, 0).instruments.empty());
    CHECK(out.parts.front().additionalInstruments.empty());
}

T_END;

TEST(noteNamesPartsOnlyInstrument, NoteInstrument)
{
    // A note may name the part's instrument even when the part has no other; the id is kept.
    auto score = noteInstrumentDrumKitScore();
    auto &part = score.parts.front();
    part.additionalInstruments.clear();
    part.instrumentData = InstrumentData{};
    part.instrumentData.uniqueId = "P1-I7";
    auto &notes = part.measures.front().staves.front().voices[0].notes;
    notes.resize(1);
    notes.front().instruments = {"P1-I7"};

    // The part's score-instrument is written for the note to refer to, even though it sets only
    // its id.
    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<score-instrument id=\"P1-I7\">") != std::string::npos);
    CHECK(xml.find("<instrument id=\"P1-I7\" />") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL("P1-I7", out.parts.front().instrumentData.uniqueId);
    CHECK(std::vector<std::string>{"P1-I7"} == noteInstrumentNote(out, 0).instruments);
}

T_END;

TEST(additionalInstrumentWithoutIdGetsOne, NoteInstrument)
{
    auto score = noteInstrumentDrumKitScore();
    auto &part = score.parts.front();
    part.additionalInstruments.at(0).uniqueId.clear();
    part.measures.front().staves.front().voices[0].notes.at(1).instruments.clear();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<score-instrument id=\"ID1000000-1\">") != std::string::npos);
    CHECK(xml.find("<midi-instrument id=\"ID1000000-1\">") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    REQUIRE(out.parts.front().additionalInstruments.size() == 2);
    CHECK_EQUAL("ID1000000-1", out.parts.front().additionalInstruments.at(0).uniqueId);
    CHECK_EQUAL("Snare", out.parts.front().additionalInstruments.at(0).name);
}

T_END;

TEST(noteInstrumentsEquality, NoteInstrument)
{
    NoteData a;
    a.instruments = {"P1-I1"};
    auto b = a;
    CHECK(a == b);
    b.instruments = {"P1-I2"};
    CHECK(a != b);

    PartData c;
    c.additionalInstruments.push_back(noteInstrumentMake("P1-I2", "Snare", 38));
    auto d = c;
    CHECK(c == d);
    d.additionalInstruments.front().midiData.unpitched = 40;
    CHECK(c != d);
}

T_END;

#endif
