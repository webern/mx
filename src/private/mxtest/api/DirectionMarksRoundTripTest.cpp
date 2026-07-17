// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"

#include <sstream>

using namespace std;
using namespace mx::api;

namespace
{
// Round-trips a DirectionData through the full serialize -> deserialize path and
// returns the directions read back from the first staff. Used by the direction-type
// data-loss regression tests below (#324).
std::vector<DirectionData> roundTripDirectionData(const DirectionData &inDirectionData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.directions.push_back(inDirectionData);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    if (!r1.ok())
        return {};
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    if (!r2.ok())
        return {};
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    mgr.destroyDocument(docId);
    if (!rd.ok())
        return {};
    const auto &oscore = rd.value();

    return oscore.parts.back().measures.back().staves.back().directions;
}
} // namespace

TEST(Damp, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.damps.emplace_back();
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    CHECK_EQUAL(1, static_cast<int>(directions.front().damps.size()));
}

T_END;

TEST(DampAll, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.dampAlls.emplace_back();
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    CHECK_EQUAL(1, static_cast<int>(directions.front().dampAlls.size()));
}

T_END;

TEST(Eyeglasses, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.eyeglasses.emplace_back();
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    CHECK_EQUAL(1, static_cast<int>(directions.front().eyeglasses.size()));
}

T_END;

TEST(StringMute, DirectionMarksRoundTrip)
{
    DirectionData direction;
    StringMuteData stringMute;
    stringMute.type = StringMuteType::off;
    direction.stringMutes.push_back(stringMute);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().stringMutes.size() == 1);
    CHECK(directions.front().stringMutes.front().type == StringMuteType::off);
}

T_END;

TEST(StaffDivide, DirectionMarksRoundTrip)
{
    DirectionData direction;
    StaffDivideData staffDivide;
    staffDivide.type = StaffDivideType::upDown;
    direction.staffDivides.push_back(staffDivide);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().staffDivides.size() == 1);
    CHECK(directions.front().staffDivides.front().type == StaffDivideType::upDown);
}

T_END;

TEST(PrincipalVoice, DirectionMarksRoundTrip)
{
    DirectionData direction;
    PrincipalVoiceData principalVoice;
    principalVoice.type = PrincipalVoiceType::start;
    principalVoice.symbol = PrincipalVoiceSymbol::nebenstimme;
    principalVoice.text = "N";
    direction.principalVoices.push_back(principalVoice);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().principalVoices.size() == 1);
    const auto &out = directions.front().principalVoices.front();
    CHECK(out.type == PrincipalVoiceType::start);
    CHECK(out.symbol == PrincipalVoiceSymbol::nebenstimme);
    CHECK_EQUAL("N", out.text);
}

T_END;

TEST(OtherDirection, DirectionMarksRoundTrip)
{
    DirectionData direction;
    OtherDirectionData otherDirection;
    otherDirection.text = "con sordino misterioso";
    otherDirection.printObject = Bool::no;
    otherDirection.smufl = "luteFingeringRHThumb";
    direction.otherDirections.push_back(otherDirection);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().otherDirections.size() == 1);
    const auto &out = directions.front().otherDirections.front();
    CHECK_EQUAL("con sordino misterioso", out.text);
    CHECK(out.printObject == Bool::no);
    REQUIRE(out.smufl.has_value());
    CHECK_EQUAL("luteFingeringRHThumb", *out.smufl);
}

T_END;

TEST(Image, DirectionMarksRoundTrip)
{
    DirectionData direction;
    ImageData image;
    image.source = "logo.png";
    image.type = "image/png";
    image.height = 40.0;
    image.width = 80.0;
    image.positionData.verticalAlignment = VerticalAlignment::middle;
    direction.images.push_back(image);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().images.size() == 1);
    const auto &out = directions.front().images.front();
    CHECK_EQUAL("logo.png", out.source);
    CHECK_EQUAL("image/png", out.type);
    REQUIRE(out.height.has_value());
    CHECK_DOUBLES_EQUAL(40.0, *out.height, 0.0001);
    REQUIRE(out.width.has_value());
    CHECK_DOUBLES_EQUAL(80.0, *out.width, 0.0001);
    CHECK(out.positionData.verticalAlignment == VerticalAlignment::middle);
}

T_END;

TEST(AccordionRegistration, DirectionMarksRoundTrip)
{
    DirectionData direction;
    AccordionRegistrationData accordion;
    accordion.high = true;
    accordion.middle = 2;
    accordion.low = true;
    direction.accordionRegistrations.push_back(accordion);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().accordionRegistrations.size() == 1);
    const auto &out = directions.front().accordionRegistrations.front();
    CHECK(out.high);
    REQUIRE(out.middle.has_value());
    CHECK_EQUAL(2, *out.middle);
    CHECK(out.low);
}

T_END;

TEST(AccordionRegistrationEmpty, DirectionMarksRoundTrip)
{
    // A registration with nothing engaged is legal and draws the empty diagram.
    DirectionData direction;
    direction.accordionRegistrations.emplace_back();
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().accordionRegistrations.size() == 1);
    const auto &out = directions.front().accordionRegistrations.front();
    CHECK(!out.high);
    CHECK(!out.middle.has_value());
    CHECK(!out.low);
}

T_END;

TEST(HarpPedals, DirectionMarksRoundTrip)
{
    DirectionData direction;
    HarpPedalsData harpPedals;
    harpPedals.pedalTunings.emplace_back(Step::d, 0);
    harpPedals.pedalTunings.emplace_back(Step::c, -1);
    harpPedals.pedalTunings.emplace_back(Step::b, 1);
    direction.harpPedals.push_back(harpPedals);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().harpPedals.size() == 1);
    const auto &out = directions.front().harpPedals.front();
    REQUIRE(out.pedalTunings.size() == 3);
    CHECK(out.pedalTunings.at(0).step == Step::d);
    CHECK_EQUAL(0, out.pedalTunings.at(0).alter);
    CHECK(out.pedalTunings.at(1).step == Step::c);
    CHECK_EQUAL(-1, out.pedalTunings.at(1).alter);
    CHECK(out.pedalTunings.at(2).step == Step::b);
    CHECK_EQUAL(1, out.pedalTunings.at(2).alter);
}

T_END;

TEST(Scordatura, DirectionMarksRoundTrip)
{
    DirectionData direction;
    ScordaturaData scordatura;
    AccordData accord;
    accord.stringNumber = 6;
    accord.tuningStep = Step::d;
    accord.tuningOctave = 2;
    scordatura.accords.push_back(accord);
    direction.scordaturas.push_back(scordatura);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().scordaturas.size() == 1);
    const auto &out = directions.front().scordaturas.front();
    REQUIRE(out.accords.size() == 1);
    REQUIRE(out.accords.front().stringNumber.has_value());
    CHECK_EQUAL(6, *out.accords.front().stringNumber);
    CHECK(out.accords.front().tuningStep == Step::d);
    CHECK_EQUAL(0, out.accords.front().tuningAlter);
    CHECK_EQUAL(2, out.accords.front().tuningOctave);
}

T_END;

TEST(ScordaturaNoStringNumbers, DirectionMarksRoundTrip)
{
    // When stringNumber is absent the accords run through the strings in order.
    DirectionData direction;
    ScordaturaData scordatura;
    AccordData accord;
    accord.tuningStep = Step::a;
    accord.tuningAlter = -1;
    accord.tuningOctave = 3;
    scordatura.accords.push_back(accord);
    direction.scordaturas.push_back(scordatura);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().scordaturas.size() == 1);
    const auto &out = directions.front().scordaturas.front();
    REQUIRE(out.accords.size() == 1);
    CHECK(!out.accords.front().stringNumber.has_value());
    CHECK(out.accords.front().tuningStep == Step::a);
    CHECK_EQUAL(-1, out.accords.front().tuningAlter);
    CHECK_EQUAL(3, out.accords.front().tuningOctave);
}

T_END;

TEST(DampFormatting, DirectionMarksRoundTrip)
{
    DirectionData direction;
    DampData damp;
    damp.positionData.isDefaultXSpecified = true;
    damp.positionData.defaultX = 5.0;
    damp.id = "damp-1";
    direction.damps.push_back(damp);
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().damps.size() == 1);
    const auto &outDamp = directions.front().damps.front();
    CHECK(outDamp.positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(5.0, outDamp.positionData.defaultX, 0.0001);
    REQUIRE(outDamp.id.has_value());
    CHECK_EQUAL("damp-1", *outDamp.id);
}

T_END;

#endif
