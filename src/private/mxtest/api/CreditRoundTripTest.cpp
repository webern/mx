// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

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

TEST(creditRoundTrip, imageValignMiddleWritesMiddle)
{
    // A middle alignment must be written as valign-image's middle, not left at the
    // natural-zero top (#444).
    auto in = makeMinimalScore();
    PageImageData img{};
    img.source = "logo.png";
    img.type = "image/png";
    img.positionData.verticalAlignment = VerticalAlignment::middle;
    in.pageImageItems.push_back(img);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("valign=\"middle\"") != std::string::npos);
    CHECK(xml.find("valign=\"top\"") == std::string::npos);
}

TEST(creditRoundTrip, imageValignBaselineOmitsAttribute)
{
    // valign-image has no baseline value, so it must not be written at all -- and in
    // particular not defaulted to "top" (#444).
    auto in = makeMinimalScore();
    PageImageData img{};
    img.source = "logo.png";
    img.type = "image/png";
    img.positionData.verticalAlignment = VerticalAlignment::baseline;
    in.pageImageItems.push_back(img);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("valign=") == std::string::npos);
}

TEST(creditRoundTrip, imageValignSurvives)
{
    // The reader dropped <credit-image>'s valign attribute, so every alignment came back
    // unspecified (#454).
    for (const auto alignment : {VerticalAlignment::top, VerticalAlignment::middle, VerticalAlignment::bottom})
    {
        auto in = makeMinimalScore();
        PageImageData img{};
        img.source = "logo.png";
        img.type = "image/png";
        img.positionData.verticalAlignment = alignment;
        in.pageImageItems.push_back(img);

        const auto out = mxtest::roundTrip(in);

        REQUIRE(out.pageImageItems.size() == 1);
        CHECK(alignment == out.pageImageItems.at(0).positionData.verticalAlignment);
    }
}

TEST(creditRoundTrip, imageValignAbsentStaysUnspecified)
{
    auto in = makeMinimalScore();
    PageImageData img{};
    img.source = "logo.png";
    img.type = "image/png";
    in.pageImageItems.push_back(img);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("valign=") == std::string::npos);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageImageItems.size() == 1);
    CHECK(VerticalAlignment::unspecified == out.pageImageItems.at(0).positionData.verticalAlignment);
}

TEST(creditRoundTrip, imageValignIsReadFromXml)
{
    const std::string xml = R"(<score-partwise version="3.0">
  <credit>
    <credit-image source="logo.png" type="image/png" valign="middle" />
  </credit>
  <part-list>
    <score-part id="P1">
      <part-name>Flute</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = mxtest::fromXml(xml);

    REQUIRE(score.pageImageItems.size() == 1);
    const auto &img = score.pageImageItems.at(0);
    CHECK_EQUAL("logo.png", img.source);
    CHECK(VerticalAlignment::middle == img.positionData.verticalAlignment);
}

TEST(creditRoundTrip, justifySurvives)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = "Layout options";
    credit.pageNumber = 1;
    credit.justify = HorizontalAlignment::center;
    in.pageTextItems.push_back(credit);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("justify=\"center\"") != std::string::npos);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    const auto &got = out.pageTextItems.at(0);
    CHECK_EQUAL("Layout options", got.text);
    CHECK(HorizontalAlignment::center == got.justify);
}

TEST(creditRoundTrip, justifyAbsentStaysAbsent)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = "no justify here";
    credit.pageNumber = 1;
    in.pageTextItems.push_back(credit);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("justify=") == std::string::npos);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    CHECK(HorizontalAlignment::unspecified == out.pageTextItems.at(0).justify);
}

TEST(creditRoundTrip, enclosureSurvives)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = "Framed title";
    credit.pageNumber = 1;
    credit.enclosure = Enclosure::rectangle;
    in.pageTextItems.push_back(credit);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("enclosure=\"rectangle\"") != std::string::npos);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    const auto &got = out.pageTextItems.at(0);
    CHECK_EQUAL("Framed title", got.text);
    CHECK(Enclosure::rectangle == got.enclosure);
}

TEST(creditRoundTrip, enclosureNoneIsExplicit)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = "Deliberately unframed";
    credit.pageNumber = 1;
    credit.enclosure = Enclosure::none;
    in.pageTextItems.push_back(credit);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("enclosure=\"none\"") != std::string::npos);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    CHECK(Enclosure::none == out.pageTextItems.at(0).enclosure);
}

TEST(creditRoundTrip, enclosureAbsentStaysAbsent)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = "no enclosure here";
    credit.pageNumber = 1;
    in.pageTextItems.push_back(credit);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("enclosure=") == std::string::npos);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    CHECK(Enclosure::unspecified == out.pageTextItems.at(0).enclosure);
}

TEST(creditRoundTrip, continuationsSurvive)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = "Notebooks\n";
    credit.pageNumber = 1;
    credit.fontData.style = FontStyle::italic;
    credit.fontData.sizeType = FontSizeType::point;
    credit.fontData.sizePoint = 14.0;
    credit.justify = HorizontalAlignment::center;

    PageTextSegment catalogue{};
    catalogue.text = "BWV Anhang 114";
    catalogue.fontData.style = FontStyle::normal;
    credit.continuations.push_back(catalogue);

    PageTextSegment last{};
    last.text = "x";
    last.fontData.weight = FontWeight::bold;
    last.enclosure = Enclosure::rectangle;
    last.positionData.defaultX = 12.5;
    last.positionData.isDefaultXSpecified = true;
    last.positionData.defaultY = -30.0;
    last.positionData.isDefaultYSpecified = true;
    last.positionData.horizontalAlignment = HorizontalAlignment::right;
    last.justify = HorizontalAlignment::left;
    credit.continuations.push_back(last);
    in.pageTextItems.push_back(credit);

    const auto xml = mxtest::toXml(in);
    const auto firstWords = xml.find("<credit-words");
    REQUIRE(firstWords != std::string::npos);
    const auto secondWords = xml.find("<credit-words", firstWords + 1);
    REQUIRE(secondWords != std::string::npos);
    const auto thirdWords = xml.find("<credit-words", secondWords + 1);
    REQUIRE(thirdWords != std::string::npos);
    CHECK(xml.find("<credit-words", thirdWords + 1) == std::string::npos);
    // The second run states only its style, so it does not repeat the size of the first.
    const auto secondEnd = xml.find('>', secondWords);
    CHECK(xml.substr(secondWords, secondEnd - secondWords).find("font-size=") == std::string::npos);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    const auto &got = out.pageTextItems.at(0);
    CHECK_EQUAL("Notebooks\n", got.text);
    CHECK(FontStyle::italic == got.fontData.style);
    CHECK(HorizontalAlignment::center == got.justify);
    REQUIRE(got.continuations.size() == 2);

    const auto &gotCatalogue = got.continuations.at(0);
    CHECK_EQUAL("BWV Anhang 114", gotCatalogue.text);
    CHECK(FontStyle::normal == gotCatalogue.fontData.style);
    CHECK(FontSizeType::unspecified == gotCatalogue.fontData.sizeType);
    CHECK(FontWeight::unspecified == gotCatalogue.fontData.weight);
    CHECK(Enclosure::unspecified == gotCatalogue.enclosure);

    const auto &gotLast = got.continuations.at(1);
    CHECK_EQUAL("x", gotLast.text);
    CHECK(FontWeight::bold == gotLast.fontData.weight);
    CHECK(Enclosure::rectangle == gotLast.enclosure);
    CHECK(gotLast.positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(12.5, gotLast.positionData.defaultX, 0.001);
    CHECK(gotLast.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(-30.0, gotLast.positionData.defaultY, 0.001);
    CHECK(HorizontalAlignment::right == gotLast.positionData.horizontalAlignment);
    CHECK(HorizontalAlignment::left == gotLast.justify);

    CHECK(got == in.pageTextItems.at(0));
}

TEST(creditRoundTrip, singleRunHasNoContinuations)
{
    auto in = makeMinimalScore();
    PageTextData credit{};
    credit.text = "Just one run";
    credit.pageNumber = 1;
    in.pageTextItems.push_back(credit);

    const auto xml = mxtest::toXml(in);
    const auto firstWords = xml.find("<credit-words");
    REQUIRE(firstWords != std::string::npos);
    CHECK(xml.find("<credit-words", firstWords + 1) == std::string::npos);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.pageTextItems.size() == 1);
    CHECK(out.pageTextItems.at(0).continuations.empty());
}

TEST(creditRoundTrip, continuationsAreReadFromXml)
{
    const std::string xml = R"(<score-partwise version="3.0">
  <credit page="1">
    <credit-type>title</credit-type>
    <credit-words default-x="300" default-y="1500" font-size="20" justify="center">Minuet
</credit-words>
    <credit-words>in G</credit-words>
    <credit-words font-weight="bold">BWV Anh. 114</credit-words>
  </credit>
  <part-list>
    <score-part id="P1">
      <part-name>Flute</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = mxtest::fromXml(xml);

    REQUIRE(score.pageTextItems.size() == 1);
    const auto &got = score.pageTextItems.at(0);
    CHECK_EQUAL("Minuet\n", got.text);
    CHECK(HorizontalAlignment::center == got.justify);
    REQUIRE(got.creditTypes.size() == 1);
    CHECK_EQUAL("title", got.creditTypes.at(0));

    REQUIRE(got.continuations.size() == 2);
    const auto &second = got.continuations.at(0);
    CHECK_EQUAL("in G", second.text);
    CHECK(FontSizeType::unspecified == second.fontData.sizeType);
    CHECK(FontWeight::unspecified == second.fontData.weight);
    CHECK(HorizontalAlignment::unspecified == second.justify);
    CHECK(Enclosure::unspecified == second.enclosure);
    CHECK(!second.positionData.isDefaultXSpecified);

    const auto &third = got.continuations.at(1);
    CHECK_EQUAL("BWV Anh. 114", third.text);
    CHECK(FontWeight::bold == third.fontData.weight);
}

TEST(creditRoundTrip, continuationsParticipateInEquality)
{
    PageTextData a{};
    a.text = "first";
    PageTextSegment segment{};
    segment.text = "second";
    a.continuations.push_back(segment);

    PageTextData b = a;
    CHECK(a == b);

    b.continuations.at(0).text = "other";
    CHECK(a != b);

    b = a;
    b.continuations.at(0).fontData.style = FontStyle::italic;
    CHECK(a != b);

    b = a;
    b.continuations.at(0).positionData.horizontalAlignment = HorizontalAlignment::right;
    CHECK(a != b);

    b = a;
    b.continuations.at(0).justify = HorizontalAlignment::center;
    CHECK(a != b);

    b = a;
    b.continuations.at(0).enclosure = Enclosure::oval;
    CHECK(a != b);

    b = a;
    b.continuations.push_back(segment);
    CHECK(a != b);
}

TEST(creditRoundTrip, creditSymbolBetweenWordsIsNotASegment)
{
    const std::string xml = R"(<score-partwise version="3.0">
  <credit page="1">
    <credit-words>before</credit-words>
    <credit-symbol>segno</credit-symbol>
    <credit-words>after</credit-words>
  </credit>
  <part-list>
    <score-part id="P1">
      <part-name>Flute</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = mxtest::fromXml(xml);

    REQUIRE(score.pageTextItems.size() == 1);
    const auto &got = score.pageTextItems.at(0);
    CHECK_EQUAL("before", got.text);
    REQUIRE(got.continuations.size() == 1);
    CHECK_EQUAL("after", got.continuations.at(0).text);
}

TEST(creditRoundTrip, wordsAfterALeadingCreditSymbolAreTheText)
{
    const std::string xml = R"(<score-partwise version="3.0">
  <credit page="1">
    <credit-symbol>segno</credit-symbol>
    <credit-words font-weight="bold">after the sign</credit-words>
    <credit-words>more</credit-words>
  </credit>
  <part-list>
    <score-part id="P1">
      <part-name>Flute</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = mxtest::fromXml(xml);

    REQUIRE(score.pageTextItems.size() == 1);
    const auto &got = score.pageTextItems.at(0);
    CHECK_EQUAL("after the sign", got.text);
    CHECK(FontWeight::bold == got.fontData.weight);
    REQUIRE(got.continuations.size() == 1);
    CHECK_EQUAL("more", got.continuations.at(0).text);

    // The symbol is not kept, and the writer does not put an empty <credit-words> in its place.
    const auto written = mxtest::toXml(score);
    const auto firstWords = written.find("<credit-words");
    REQUIRE(firstWords != std::string::npos);
    const auto secondWords = written.find("<credit-words", firstWords + 1);
    REQUIRE(secondWords != std::string::npos);
    CHECK(written.find("<credit-words", secondWords + 1) == std::string::npos);
}

#endif
