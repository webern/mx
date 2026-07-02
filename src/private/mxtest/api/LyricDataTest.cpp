// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/TestHelpers.h"
#include "pugixml.hpp"

#include <string>

using namespace mx::api;

namespace mxtest
{
namespace api
{
namespace lyric_data_test
{
mx::api::ScoreData makeScoreWithLyrics()
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();
    note.durationData.durationTimeTicks = 4;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.isDurationNameSpecified = true;

    LyricData first;
    first.text = "Hel";
    first.verseNumber = "1";
    first.verseName = "verse";
    first.syllabic = LyricSyllabic::begin;
    first.hasExtend = true;
    first.positionData.placement = Placement::below;
    first.positionData.horizontalAlignmnet = HorizontalAlignment::center;
    first.positionData.isDefaultYSpecified = true;
    first.positionData.defaultY = -40.0;
    first.printData.printObject = Bool::yes;
    first.printData.isColorSpecified = true;
    first.printData.color.red = 1;
    first.printData.color.green = 2;
    first.printData.color.blue = 3;
    first.printData.fontData.fontFamily = {"Bravura Text"};
    note.lyrics.emplace_back(first);

    LyricData second;
    second.verseNumber = "2";
    second.hasExtend = true;
    second.printData.printObject = Bool::no;
    note.lyrics.emplace_back(second);

    return score;
}

pugi::xml_node firstNote(const std::string &xml)
{
    static pugi::xml_document doc;
    doc.reset();
    doc.load_string(xml.c_str());
    return doc.child("score-partwise").child("part").child("measure").child("note");
}
} // namespace lyric_data_test
} // namespace api
} // namespace mxtest

TEST(lyricsWriteToMusicXml, LyricData)
{
    const auto xml = mxtest::toXml(mxtest::api::lyric_data_test::makeScoreWithLyrics());
    auto note = mxtest::api::lyric_data_test::firstNote(xml);
    auto lyric = note.child("lyric");

    CHECK_EQUAL(std::string{"1"}, std::string{lyric.attribute("number").value()});
    CHECK_EQUAL(std::string{"verse"}, std::string{lyric.attribute("name").value()});
    CHECK_EQUAL(std::string{"below"}, std::string{lyric.attribute("placement").value()});
    CHECK_EQUAL(std::string{"center"}, std::string{lyric.attribute("justify").value()});
    CHECK_EQUAL(std::string{"yes"}, std::string{lyric.attribute("print-object").value()});
    CHECK_EQUAL(std::string{"#010203"}, std::string{lyric.attribute("color").value()});
    CHECK_EQUAL(std::string{"-40"}, std::string{lyric.attribute("default-y").value()});
    CHECK_EQUAL(std::string{"begin"}, std::string{lyric.child("syllabic").text().get()});
    CHECK_EQUAL(std::string{"Hel"}, std::string{lyric.child("text").text().get()});
    CHECK_EQUAL(std::string{"Bravura Text"}, std::string{lyric.child("text").attribute("font-family").value()});
    CHECK(!lyric.child("extend").empty());

    auto extendOnly = lyric.next_sibling("lyric");
    CHECK_EQUAL(std::string{"2"}, std::string{extendOnly.attribute("number").value()});
    CHECK_EQUAL(std::string{"no"}, std::string{extendOnly.attribute("print-object").value()});
    CHECK(!extendOnly.child("extend").empty());
    CHECK(extendOnly.child("text").empty());
}

T_END;

TEST(lyricsRoundTripThroughApi, LyricData)
{
    const auto out = mxtest::fromXml(mxtest::toXml(mxtest::api::lyric_data_test::makeScoreWithLyrics()));
    const auto &note = out.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);

    CHECK_EQUAL(static_cast<size_t>(2), note.lyrics.size());
    CHECK_EQUAL(std::string{"Hel"}, note.lyrics.at(0).text);
    CHECK_EQUAL(std::string{"1"}, note.lyrics.at(0).verseNumber);
    CHECK_EQUAL(std::string{"verse"}, note.lyrics.at(0).verseName);
    CHECK(note.lyrics.at(0).syllabic == LyricSyllabic::begin);
    CHECK(note.lyrics.at(0).hasExtend);
    CHECK(note.lyrics.at(0).positionData.placement == Placement::below);
    CHECK(note.lyrics.at(0).positionData.horizontalAlignmnet == HorizontalAlignment::center);
    CHECK(note.lyrics.at(0).printData.printObject == Bool::yes);
    CHECK(note.lyrics.at(0).printData.isColorSpecified);
    CHECK_EQUAL(1, static_cast<int>(note.lyrics.at(0).printData.color.red));
    CHECK_EQUAL(2, static_cast<int>(note.lyrics.at(0).printData.color.green));
    CHECK_EQUAL(3, static_cast<int>(note.lyrics.at(0).printData.color.blue));
    CHECK_EQUAL(std::string{"Bravura Text"}, note.lyrics.at(0).printData.fontData.fontFamily.at(0));

    CHECK_EQUAL(std::string{"2"}, note.lyrics.at(1).verseNumber);
    CHECK(note.lyrics.at(1).text.empty());
    CHECK(note.lyrics.at(1).hasExtend);
    CHECK(note.lyrics.at(1).printData.printObject == Bool::no);
}

T_END;

#endif
