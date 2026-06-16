// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/RoundTrip.h"

using namespace std;
using namespace mx::api;

namespace
{
ScoreData makeMinimalScore()
{
    VoiceData voiceData;
    NoteData n;
    n.tickTimePosition = 0;
    n.pitchData.step = Step::c;
    n.pitchData.octave = 5;
    n.durationData.durationName = DurationName::quarter;
    n.durationData.durationTimeTicks = DEFAULT_TICKS_PER_QUARTER;
    voiceData.notes.push_back(n);
    StaffData staff{};
    staff.voices.emplace(0, voiceData);
    MeasureData m;
    m.staves.push_back(staff);

    PartData pd;
    pd.uniqueId = "P1";
    pd.name = "Flute";
    pd.displayName = "Flute";
    pd.measures.push_back(m);

    ScoreData s;
    // a scaling is required for the <defaults> element to be emitted
    s.defaults.scalingMillimeters = 7.0;
    s.defaults.scalingTenths = 40.0;
    s.parts.push_back(pd);
    return s;
}
} // namespace

TEST(defaultsFontsRoundTrip, wordAndMusicFont)
{
    auto in = makeMinimalScore();

    FontData wordFont{};
    wordFont.fontFamily = {"Times New Roman"};
    wordFont.sizeType = FontSizeType::point;
    wordFont.sizePoint = 10.5;
    wordFont.style = FontStyle::italic;
    wordFont.weight = FontWeight::bold;
    in.defaults.wordFont = wordFont;

    FontData musicFont{};
    musicFont.fontFamily = {"Maestro"};
    musicFont.sizeType = FontSizeType::point;
    musicFont.sizePoint = 20.0;
    in.defaults.musicFont = musicFont;

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.defaults.wordFont.has_value());
    CHECK(wordFont == out.defaults.wordFont.value());

    REQUIRE(out.defaults.musicFont.has_value());
    CHECK(musicFont == out.defaults.musicFont.value());
}

TEST(defaultsFontsRoundTrip, lyricFonts)
{
    auto in = makeMinimalScore();

    LyricFontData lf1{};
    lf1.number = "1";
    lf1.name = "verse";
    lf1.font.fontFamily = {"Arial"};
    lf1.font.sizeType = FontSizeType::point;
    lf1.font.sizePoint = 8.0;
    in.defaults.lyricFonts.push_back(lf1);

    LyricFontData lf2{};
    lf2.number = "2";
    lf2.font.fontFamily = {"Courier"};
    lf2.font.sizeType = FontSizeType::point;
    lf2.font.sizePoint = 9.0;
    in.defaults.lyricFonts.push_back(lf2);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.defaults.lyricFonts.size() == 2);
    CHECK(lf1 == out.defaults.lyricFonts.at(0));
    CHECK(lf2 == out.defaults.lyricFonts.at(1));
}

#endif
