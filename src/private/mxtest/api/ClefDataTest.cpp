// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/api/TestHelpers.h"

#include <string>

using namespace mx::api;

namespace mxtest::api::clef_data_test
{
ScoreData makeScoreWithClef(Bool printObject)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();

    staff.clefs.emplace_back();
    auto &clef = staff.clefs.back();
    clef.printObject = printObject;

    staff.voices[0].notes.emplace_back();
    return score;
}

ScoreData makeScoreWithAdditionalClef(Bool additional)
{
    ScoreData score = makeScoreWithClef(Bool::unspecified);
    score.parts.front().measures.front().staves.front().clefs.front().additional = additional;
    return score;
}

ScoreData makeScoreWithClefLocation(ClefLocation location)
{
    ScoreData score = makeScoreWithClef(Bool::unspecified);
    score.parts.front().measures.front().staves.front().clefs.front().location = location;
    return score;
}

const ClefData &firstClef(const ScoreData &score)
{
    return score.parts.front().measures.front().staves.front().clefs.front();
}
} // namespace mxtest::api::clef_data_test

TEST(clefPrintObjectNoRoundTrip, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithClef(Bool::no));
    CHECK(xml.find("print-object=\"no\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::no == out.parts.front().measures.front().staves.front().clefs.front().printObject);
}

T_END;

TEST(clefPrintObjectYesRoundTrip, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithClef(Bool::yes));
    CHECK(xml.find("print-object=\"yes\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::yes == out.parts.front().measures.front().staves.front().clefs.front().printObject);
}

T_END;

TEST(clefPrintObjectUnspecifiedIsOmitted, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithClef(Bool::unspecified));
    CHECK(xml.find("print-object=") == std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::unspecified == out.parts.front().measures.front().staves.front().clefs.front().printObject);
}

T_END;

TEST(clefAdditionalYesRoundTrip, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithAdditionalClef(Bool::yes));
    CHECK(xml.find("additional=\"yes\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::yes == out.parts.front().measures.front().staves.front().clefs.front().additional);
}

T_END;

TEST(clefAdditionalNoRoundTrip, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithAdditionalClef(Bool::no));
    CHECK(xml.find("additional=\"no\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::no == out.parts.front().measures.front().staves.front().clefs.front().additional);
}

T_END;

TEST(clefAdditionalUnspecifiedIsOmitted, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithAdditionalClef(Bool::unspecified));
    CHECK(xml.find("additional=") == std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::unspecified == out.parts.front().measures.front().staves.front().clefs.front().additional);
}

T_END;

TEST(clefAfterBarlineYesRoundTrip, ClefData)
{
    using namespace mxtest::api::clef_data_test;
    const auto xml = mxtest::toXml(makeScoreWithClefLocation(ClefLocation::afterBarline));
    CHECK(xml.find("after-barline=\"yes\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(ClefLocation::afterBarline == firstClef(out).location);
}

T_END;

TEST(clefAfterBarlineNoRoundTrip, ClefData)
{
    using namespace mxtest::api::clef_data_test;
    const auto xml = mxtest::toXml(makeScoreWithClefLocation(ClefLocation::beforeBarline));
    CHECK(xml.find("after-barline=\"no\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(ClefLocation::beforeBarline == firstClef(out).location);
}

T_END;

TEST(clefAfterBarlineUnspecifiedIsOmitted, ClefData)
{
    using namespace mxtest::api::clef_data_test;
    const auto xml = mxtest::toXml(makeScoreWithClefLocation(ClefLocation::unspecified));
    CHECK(xml.find("after-barline=") == std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(ClefLocation::unspecified == firstClef(out).location);
}

T_END;

TEST(clefAllFieldsSurviveWrite, ClefData)
{
    using namespace mxtest::api::clef_data_test;
    ScoreData score = makeScoreWithClef(Bool::unspecified);
    auto &clef = score.parts.front().measures.front().staves.front().clefs.front();
    clef.writeStaffNumber = Bool::yes;
    clef.symbol = ClefSymbol::f;
    clef.line = 4;
    clef.isLineSpecified = true;
    clef.octaveChange = -1;
    clef.isOctaveChangeSpecified = true;
    clef.tickTimePosition = 0;
    clef.location = ClefLocation::afterBarline;
    clef.additional = Bool::yes;
    clef.printObject = Bool::no;
    const ClefData expected = clef;

    const auto out = mxtest::fromXml(mxtest::toXml(score));
    const auto &actual = firstClef(out);
    CHECK(expected.writeStaffNumber == actual.writeStaffNumber);
    CHECK(expected.symbol == actual.symbol);
    CHECK_EQUAL(expected.line, actual.line);
    CHECK_EQUAL(expected.isLineSpecified, actual.isLineSpecified);
    CHECK_EQUAL(expected.octaveChange, actual.octaveChange);
    CHECK_EQUAL(expected.isOctaveChangeSpecified, actual.isOctaveChangeSpecified);
    CHECK_EQUAL(expected.tickTimePosition, actual.tickTimePosition);
    CHECK(expected.location == actual.location);
    CHECK(expected.additional == actual.additional);
    CHECK(expected.printObject == actual.printObject);
    CHECK(expected == actual);
}

T_END;

TEST(clefUnspecifiedElementsSurviveWrite, ClefData)
{
    using namespace mxtest::api::clef_data_test;
    ScoreData score = makeScoreWithClef(Bool::unspecified);
    auto &clef = score.parts.front().measures.front().staves.front().clefs.front();
    clef.symbol = ClefSymbol::c;
    clef.isLineSpecified = false;
    clef.line = 3;
    clef.isOctaveChangeSpecified = false;
    clef.octaveChange = 0;
    clef.writeStaffNumber = Bool::no;
    const ClefData expected = clef;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<line>") == std::string::npos);
    CHECK(xml.find("<clef-octave-change>") == std::string::npos);
    CHECK(xml.find("<clef>") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    const auto &actual = firstClef(out);
    CHECK(expected.symbol == actual.symbol);
    CHECK_EQUAL(expected.line, actual.line);
    CHECK_EQUAL(expected.isLineSpecified, actual.isLineSpecified);
    CHECK_EQUAL(expected.octaveChange, actual.octaveChange);
    CHECK_EQUAL(expected.isOctaveChangeSpecified, actual.isOctaveChangeSpecified);

    // writeStaffNumber records only a divergence from the automatic rule. On a single-staff part
    // the automatic rule already omits the number, so no agrees with it and reads back as
    // unspecified. The emitted XML is the same either way.
    CHECK(Bool::unspecified == actual.writeStaffNumber);
}

T_END;

#endif
