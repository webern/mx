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
    first.extendType = LyricExtendType::start;
    first.positionData.placement = Placement::below;
    first.positionData.horizontalAlignment = HorizontalAlignment::center;
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
    second.extendType = LyricExtendType::stop;
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
    CHECK_EQUAL(std::string{"start"}, std::string{lyric.child("extend").attribute("type").value()});

    auto extendOnly = lyric.next_sibling("lyric");
    CHECK_EQUAL(std::string{"2"}, std::string{extendOnly.attribute("number").value()});
    CHECK_EQUAL(std::string{"no"}, std::string{extendOnly.attribute("print-object").value()});
    CHECK(!extendOnly.child("extend").empty());
    CHECK_EQUAL(std::string{"stop"}, std::string{extendOnly.child("extend").attribute("type").value()});
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
    CHECK(note.lyrics.at(0).extendType == LyricExtendType::start);
    CHECK(note.lyrics.at(0).positionData.placement == Placement::below);
    CHECK(note.lyrics.at(0).positionData.horizontalAlignment == HorizontalAlignment::center);
    CHECK(note.lyrics.at(0).printData.printObject == Bool::yes);
    CHECK(note.lyrics.at(0).printData.isColorSpecified);
    CHECK_EQUAL(1, static_cast<int>(note.lyrics.at(0).printData.color.red));
    CHECK_EQUAL(2, static_cast<int>(note.lyrics.at(0).printData.color.green));
    CHECK_EQUAL(3, static_cast<int>(note.lyrics.at(0).printData.color.blue));
    CHECK_EQUAL(std::string{"Bravura Text"}, note.lyrics.at(0).printData.fontData.fontFamily.at(0));

    CHECK_EQUAL(std::string{"2"}, note.lyrics.at(1).verseNumber);
    CHECK(note.lyrics.at(1).text.empty());
    CHECK(note.lyrics.at(1).hasExtend);
    CHECK(note.lyrics.at(1).extendType == LyricExtendType::stop);
    CHECK(note.lyrics.at(1).printData.printObject == Bool::no);
}

T_END;

TEST(unspecifiedSyllabicIsOmitted, LyricData)
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

    LyricData lyric;
    lyric.text = "plain";
    CHECK(lyric.syllabic == LyricSyllabic::unspecified);
    note.lyrics.emplace_back(lyric);

    const auto xml = mxtest::toXml(score);
    auto xmlNote = mxtest::api::lyric_data_test::firstNote(xml);
    CHECK(xmlNote.child("lyric").child("syllabic").empty());

    const auto out = mxtest::fromXml(xml);
    const auto &outNote = out.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    CHECK(outNote.lyrics.at(0).syllabic == LyricSyllabic::unspecified);
}

T_END;

TEST(elidedSyllablesRoundTripThroughApi, LyricData)
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

    LyricData lyric;
    lyric.text = "str";
    lyric.syllabic = LyricSyllabic::single;

    LyricTextSegment textJoined;
    textJoined.text = "en";
    textJoined.syllabic = LyricSyllabic::single;
    textJoined.elisionText = "\xC2\xA0"; // U+00A0 NBSP
    lyric.continuations.emplace_back(textJoined);

    LyricTextSegment smuflJoined;
    smuflJoined.text = "gth";
    smuflJoined.syllabic = LyricSyllabic::single;
    smuflJoined.elisionSmufl = std::string{"lyricsElisionWide"};
    lyric.continuations.emplace_back(smuflJoined);

    LyricTextSegment bareJoined;
    bareJoined.text = "!";
    bareJoined.syllabic = LyricSyllabic::single;
    // Neither elisionText nor elisionSmufl: writes a bare <elision/>.
    lyric.continuations.emplace_back(bareJoined);

    LyricTextSegment bothJoined;
    bothJoined.text = "?";
    bothJoined.syllabic = LyricSyllabic::single;
    // MusicXML only consults smufl when the text content is empty, but a source can legally set
    // both; round-trip fidelity means neither is dropped.
    bothJoined.elisionText = "\xC2\xA0";
    bothJoined.elisionSmufl = std::string{"lyricsElisionNarrow"};
    lyric.continuations.emplace_back(bothJoined);

    LyricTextSegment otherLyricsGlyphJoined;
    otherLyricsGlyphJoined.text = ".";
    otherLyricsGlyphJoined.syllabic = LyricSyllabic::single;
    // The schema only requires a "lyrics" prefix (pattern lyrics\c+), not one of the three
    // elision-specific names, so an unrelated "lyrics*" glyph name must round-trip too.
    otherLyricsGlyphJoined.elisionSmufl = std::string{"lyricsHyphenBaseline"};
    lyric.continuations.emplace_back(otherLyricsGlyphJoined);

    note.lyrics.emplace_back(lyric);

    const auto xml = mxtest::toXml(score);
    auto xmlNote = mxtest::api::lyric_data_test::firstNote(xml);
    auto xmlLyric = xmlNote.child("lyric");

    CHECK_EQUAL(std::string{"str"}, std::string{xmlLyric.child("text").text().get()});

    auto elisions = xmlLyric.children("elision");
    auto elisionIt = elisions.begin();
    CHECK(elisionIt != elisions.end());
    CHECK_EQUAL(std::string{"\xC2\xA0"}, std::string{elisionIt->text().get()});
    CHECK(std::string{elisionIt->attribute("smufl").value()}.empty());
    ++elisionIt;
    CHECK(elisionIt != elisions.end());
    CHECK(std::string{elisionIt->text().get()}.empty());
    CHECK_EQUAL(std::string{"lyricsElisionWide"}, std::string{elisionIt->attribute("smufl").value()});
    ++elisionIt;
    CHECK(elisionIt != elisions.end());
    CHECK(std::string{elisionIt->text().get()}.empty());
    CHECK(std::string{elisionIt->attribute("smufl").value()}.empty());
    ++elisionIt;
    CHECK(elisionIt != elisions.end());
    CHECK_EQUAL(std::string{"\xC2\xA0"}, std::string{elisionIt->text().get()});
    CHECK_EQUAL(std::string{"lyricsElisionNarrow"}, std::string{elisionIt->attribute("smufl").value()});
    ++elisionIt;
    CHECK(elisionIt != elisions.end());
    CHECK(std::string{elisionIt->text().get()}.empty());
    CHECK_EQUAL(std::string{"lyricsHyphenBaseline"}, std::string{elisionIt->attribute("smufl").value()});

    auto texts = xmlLyric.children("text");
    auto textIt = texts.begin();
    CHECK_EQUAL(std::string{"str"}, std::string{textIt->text().get()});
    ++textIt;
    CHECK(textIt != texts.end());
    CHECK_EQUAL(std::string{"en"}, std::string{textIt->text().get()});
    ++textIt;
    CHECK(textIt != texts.end());
    CHECK_EQUAL(std::string{"gth"}, std::string{textIt->text().get()});
    ++textIt;
    CHECK(textIt != texts.end());
    CHECK_EQUAL(std::string{"!"}, std::string{textIt->text().get()});
    ++textIt;
    CHECK(textIt != texts.end());
    CHECK_EQUAL(std::string{"?"}, std::string{textIt->text().get()});
    ++textIt;
    CHECK(textIt != texts.end());
    CHECK_EQUAL(std::string{"."}, std::string{textIt->text().get()});

    const auto out = mxtest::fromXml(xml);
    const auto &outNote = out.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    const auto &outLyric = outNote.lyrics.at(0);

    CHECK_EQUAL(std::string{"str"}, outLyric.text);
    CHECK(outLyric.syllabic == LyricSyllabic::single);
    CHECK_EQUAL(static_cast<size_t>(5), outLyric.continuations.size());

    CHECK_EQUAL(std::string{"en"}, outLyric.continuations.at(0).text);
    CHECK(outLyric.continuations.at(0).syllabic == LyricSyllabic::single);
    CHECK(outLyric.continuations.at(0).elisionText.has_value());
    CHECK_EQUAL(std::string{"\xC2\xA0"}, *outLyric.continuations.at(0).elisionText);
    CHECK(!outLyric.continuations.at(0).elisionSmufl.has_value());

    CHECK_EQUAL(std::string{"gth"}, outLyric.continuations.at(1).text);
    CHECK(!outLyric.continuations.at(1).elisionText.has_value());
    CHECK(outLyric.continuations.at(1).elisionSmufl.has_value());
    CHECK_EQUAL(std::string{"lyricsElisionWide"}, *outLyric.continuations.at(1).elisionSmufl);

    CHECK_EQUAL(std::string{"!"}, outLyric.continuations.at(2).text);
    CHECK(!outLyric.continuations.at(2).elisionText.has_value());
    CHECK(!outLyric.continuations.at(2).elisionSmufl.has_value());

    // Both set at once must round-trip as both, not collapse to just the winning one.
    CHECK_EQUAL(std::string{"?"}, outLyric.continuations.at(3).text);
    CHECK(outLyric.continuations.at(3).elisionText.has_value());
    CHECK_EQUAL(std::string{"\xC2\xA0"}, *outLyric.continuations.at(3).elisionText);
    CHECK(outLyric.continuations.at(3).elisionSmufl.has_value());
    CHECK_EQUAL(std::string{"lyricsElisionNarrow"}, *outLyric.continuations.at(3).elisionSmufl);

    // A "lyrics*" glyph name unrelated to elision must not be dropped just because it isn't one
    // of the three elision-specific names.
    CHECK_EQUAL(std::string{"."}, outLyric.continuations.at(4).text);
    CHECK(!outLyric.continuations.at(4).elisionText.has_value());
    CHECK(outLyric.continuations.at(4).elisionSmufl.has_value());
    CHECK_EQUAL(std::string{"lyricsHyphenBaseline"}, *outLyric.continuations.at(4).elisionSmufl);
}

T_END;

#endif
