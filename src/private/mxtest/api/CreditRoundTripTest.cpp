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
    s.parts.push_back(pd);
    return s;
}
} // namespace

TEST(creditRoundTrip, multipleCreditTypes)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = "Symphony No. 1";
    credit.pageNumber = 1;
    credit.creditTypes = {"title", "subtitle"};
    in.pageTextItems.push_back(credit);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    const auto &got = out.pageTextItems.at(0);
    CHECK_EQUAL("Symphony No. 1", got.text);
    REQUIRE(got.creditTypes.size() == 2);
    CHECK_EQUAL("title", got.creditTypes.at(0));
    CHECK_EQUAL("subtitle", got.creditTypes.at(1));
    // legacy description mirrors the first credit-type
    CHECK_EQUAL("title", got.description);
}

TEST(creditRoundTrip, noWordsCreditSurvives)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = ""; // metadata-only credit: no <credit-words>
    credit.pageNumber = 2;
    credit.creditTypes = {"page number"};
    in.pageTextItems.push_back(credit);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    const auto &got = out.pageTextItems.at(0);
    CHECK(got.text.empty());
    CHECK_EQUAL(2, got.pageNumber);
    REQUIRE(got.creditTypes.size() == 1);
    CHECK_EQUAL("page number", got.creditTypes.at(0));
}

TEST(creditRoundTrip, creditImage)
{
    auto in = makeMinimalScore();
    PageImageData img{};
    img.source = "logo.png";
    img.type = "image/png";
    img.width = 200.0;
    img.height = 100.0;
    img.pageNumber = 1;
    img.positionData.isDefaultXSpecified = true;
    img.positionData.defaultX = 50.0;
    img.positionData.isDefaultYSpecified = true;
    img.positionData.defaultY = 60.0;
    in.pageImageItems.push_back(img);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageImageItems.size() == 1);
    const auto &got = out.pageImageItems.at(0);
    CHECK_EQUAL("logo.png", got.source);
    CHECK_EQUAL("image/png", got.type);
    CHECK_EQUAL(200.0, got.width);
    CHECK_EQUAL(100.0, got.height);
    CHECK_EQUAL(1, got.pageNumber);
    CHECK(got.positionData.isDefaultXSpecified);
    CHECK_EQUAL(50.0, got.positionData.defaultX);
    CHECK(got.positionData.isDefaultYSpecified);
    CHECK_EQUAL(60.0, got.positionData.defaultY);
}

#endif
