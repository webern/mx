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
