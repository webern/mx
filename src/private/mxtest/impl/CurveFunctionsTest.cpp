// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/NoteData.h"
#include "mx/core/generated/Slur.h"
#include "mx/core/generated/Tied.h"
#include "mx/core/generated/TiedType.h"
#include "mx/impl/CurveFunctions.h"

using namespace mx;
using namespace mx::impl;

namespace
{
template <typename T> T seed()
{
    using namespace mx::impl;
    using namespace mx;
    T e{};
    e.setBezierOffset(core::Divisions{core::Decimal{1.0}});
    e.setBezierOffset2(core::Divisions{core::Decimal{2.0}});
    e.setDefaultX(core::Tenths{core::Decimal{3.0}});
    e.setDefaultY(core::Tenths{core::Decimal{4.0}});
    e.setRelativeX(core::Tenths{core::Decimal{5.0}});
    e.setRelativeY(core::Tenths{core::Decimal{6.0}});
    e.setColor(core::Color{7, 8, 9, 10});
    e.setDashLength(core::Tenths{core::Decimal{11.0}});
    e.setSpaceLength(core::Tenths{core::Decimal{12.0}});
    e.setNumber(core::NumberLevel{2});
    e.setLineType(core::LineType::wavy());
    e.setPlacement(core::AboveBelow::below());
    e.setOrientation(core::OverUnder::over());
    e.setBezierX(core::Tenths{core::Decimal{13.0}});
    e.setBezierY(core::Tenths{core::Decimal{14.0}});
    e.setBezierX2(core::Tenths{core::Decimal{15.0}});
    e.setBezierY2(core::Tenths{core::Decimal{16.0}});
    return e;
}

api::CurveStart seedStart(api::CurveType t)
{
    api::CurveStart c{t};
    c.numberLevel = 2;
    c.curvePoints.isBezierOffsetSpecified = true;
    c.curvePoints.bezierOffset = 1.0;
    c.curvePoints.isBezierXSpecified = true;
    c.curvePoints.bezierX = 13.0;
    c.curvePoints.isBezierYSpecified = true;
    c.curvePoints.bezierY = 14.0;
    c.curvePoints.positionData.isDefaultXSpecified = true;
    c.curvePoints.positionData.defaultX = 3.0;
    c.curvePoints.positionData.isDefaultYSpecified = true;
    c.curvePoints.positionData.defaultY = 4.0;
    c.curvePoints.positionData.isRelativeXSpecified = true;
    c.curvePoints.positionData.relativeX = 5.0;
    c.curvePoints.positionData.isRelativeYSpecified = true;
    c.curvePoints.positionData.relativeY = 6.0;
    c.isColorSpecified = true;
    c.colorData.red = 8;
    c.colorData.green = 9;
    c.colorData.blue = 10;
    c.colorData.isAlphaSpecified = false;
    c.lineData.lineType = api::LineType::wavy;
    c.lineData.isDashLengthSpecified = true;
    c.lineData.dashLength = 11.0;
    c.lineData.isSpaceLengthSpecified = true;
    c.lineData.spaceLength = 12.0;
    c.curvePoints.positionData.placement = api::Placement::below;
    c.curveOrientation = api::CurveOrientation::overhand;
    return c;
}

api::CurveContinue seedContinue(api::CurveType t)
{
    api::CurveContinue c{t};
    c.numberLevel = 2;
    c.curvePoints.isBezierOffsetSpecified = true;
    c.curvePoints.bezierOffset = 1.0;
    c.curvePoints.isBezierXSpecified = true;
    c.curvePoints.bezierX = 13.0;
    c.curvePoints.isBezierYSpecified = true;
    c.curvePoints.bezierY = 14.0;
    c.curvePoints.positionData.isDefaultXSpecified = true;
    c.curvePoints.positionData.defaultX = 3.0;
    c.curvePoints.positionData.isDefaultYSpecified = true;
    c.curvePoints.positionData.defaultY = 4.0;
    c.curvePoints.positionData.isRelativeXSpecified = true;
    c.curvePoints.positionData.relativeX = 5.0;
    c.curvePoints.positionData.isRelativeYSpecified = true;
    c.curvePoints.positionData.relativeY = 6.0;
    c.curvePoints.positionData.placement = api::Placement::below;
    c.isBezierX2Specified = true;
    c.bezierX2 = 100.0;
    c.isBezierY2Specified = true;
    c.bezierY2 = 101.0;
    c.isBezierOffset2Specified = true;
    c.bezierOffset2 = 102.0;
    return c;
}

api::CurveStop seedStop(api::CurveType t)
{
    api::CurveStop c{t};
    c.numberLevel = 2;
    c.curvePoints.isBezierOffsetSpecified = true;
    c.curvePoints.bezierOffset = 1.0;
    c.curvePoints.isBezierXSpecified = true;
    c.curvePoints.bezierX = 13.0;
    c.curvePoints.isBezierYSpecified = true;
    c.curvePoints.bezierY = 14.0;
    c.curvePoints.positionData.isDefaultXSpecified = true;
    c.curvePoints.positionData.defaultX = 3.0;
    c.curvePoints.positionData.isDefaultYSpecified = true;
    c.curvePoints.positionData.defaultY = 4.0;
    c.curvePoints.positionData.isRelativeXSpecified = true;
    c.curvePoints.positionData.relativeX = 5.0;
    c.curvePoints.positionData.isRelativeYSpecified = true;
    c.curvePoints.positionData.relativeY = 6.0;
    c.curvePoints.positionData.placement = api::Placement::below;
    return c;
}
} // namespace

TEST(parseCurveStart_curveType, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(api::CurveType::tie == c.curveType);
}

T_END

TEST(parseCurveStart_numberLevel, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK_EQUAL(2, c.numberLevel);
}

T_END

TEST(parseCurveStart_curvePoints_bx, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(c.curvePoints.isBezierXSpecified);
    CHECK_DOUBLES_EQUAL(13.0, c.curvePoints.bezierX, 0.01);
}

T_END

TEST(parseCurveStart_curvePoints_by, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(c.curvePoints.isBezierYSpecified);
    CHECK_DOUBLES_EQUAL(14.0, c.curvePoints.bezierY, 0.01);
}

T_END

TEST(parseCurveStart_curvePoints_bo, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(c.curvePoints.isBezierOffsetSpecified);
    CHECK_EQUAL(1, c.curvePoints.bezierOffset);
}

T_END

TEST(parseCurveStart_curvePoints_positionData_x, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(c.curvePoints.positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(3.0, c.curvePoints.positionData.defaultX, 0.01);
}

T_END

TEST(parseCurveStart_curvePoints_positionData_y, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(c.curvePoints.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(4.0, c.curvePoints.positionData.defaultY, 0.01);
}

T_END

TEST(parseCurveStart_curvePoints_positionData_rx, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(c.curvePoints.positionData.isRelativeXSpecified);
    CHECK_DOUBLES_EQUAL(5.0, c.curvePoints.positionData.relativeX, 0.01);
}

T_END

TEST(parseCurveStart_curvePoints_positionData_ry, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(c.curvePoints.positionData.isRelativeYSpecified);
    CHECK_DOUBLES_EQUAL(6.0, c.curvePoints.positionData.relativeY, 0.01);
}

T_END

TEST(parseCurveStart_lineData_dashLength, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    const auto &l = c.lineData;
    CHECK(l.isDashLengthSpecified);
    CHECK_DOUBLES_EQUAL(11.0, l.dashLength, 0.01);
}

T_END

TEST(parseCurveStart_lineData_spaceLength, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    const auto &l = c.lineData;
    CHECK(l.isSpaceLengthSpecified);
    CHECK_DOUBLES_EQUAL(12.0, l.spaceLength, 0.01);
}

T_END

TEST(parseCurveStart_lineData_lineType, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    const auto &l = c.lineData;
    CHECK(api::LineType::wavy == l.lineType);
}

T_END

TEST(parseCurveStart_color, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    const auto &color = c.colorData;
    CHECK(c.isColorSpecified);
    CHECK_EQUAL(8, static_cast<int>(color.red));
    CHECK_EQUAL(9, static_cast<int>(color.green));
    CHECK_EQUAL(10, static_cast<int>(color.blue));
    CHECK(color.isAlphaSpecified);
    CHECK_EQUAL(7, static_cast<int>(color.alpha));
}

T_END

TEST(parseCurveStart_orientation, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart(e);
    CHECK(api::CurveOrientation::overhand == c.curveOrientation);
}

T_END

/////////////////////////////////////////////////////////////////////////////////

TEST(parseCurveContinue_curveType, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(api::CurveType::tie == c.curveType);
}

T_END

TEST(parseCurveContinue_numberLevel, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK_EQUAL(2, c.numberLevel);
}

T_END

TEST(parseCurveContinue_curvePoints_bx, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.curvePoints.isBezierXSpecified);
    CHECK_DOUBLES_EQUAL(13.0, c.curvePoints.bezierX, 0.01);
}

T_END

TEST(parseCurveContinue_curvePoints_by, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.curvePoints.isBezierYSpecified);
    CHECK_DOUBLES_EQUAL(14.0, c.curvePoints.bezierY, 0.01);
}

T_END

TEST(parseCurveContinue_curvePoints_bo, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.curvePoints.isBezierOffsetSpecified);
    CHECK_EQUAL(1, c.curvePoints.bezierOffset);
}

T_END

TEST(parseCurveContinue_curvePoints_bo2, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.isBezierOffset2Specified);
    CHECK_DOUBLES_EQUAL(2.0, c.bezierOffset2, 0.01);
}

T_END

TEST(parseCurveContinue_bx2, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.isBezierX2Specified);
    CHECK_DOUBLES_EQUAL(15.0, c.bezierX2, 0.01);
}

T_END

TEST(parseCurveContinue_by2, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.isBezierY2Specified);
    CHECK_DOUBLES_EQUAL(16.0, c.bezierY2, 0.01);
}

T_END

TEST(parseCurveContinue_curvePoints_positionData_x, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.curvePoints.positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(3.0, c.curvePoints.positionData.defaultX, 0.01);
}

T_END

TEST(parseCurveContinue_curvePoints_positionData_y, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.curvePoints.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(4.0, c.curvePoints.positionData.defaultY, 0.01);
}

T_END

TEST(parseCurveContinue_curvePoints_positionData_rx, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.curvePoints.positionData.isRelativeXSpecified);
    CHECK_DOUBLES_EQUAL(5.0, c.curvePoints.positionData.relativeX, 0.01);
}

T_END

TEST(parseCurveContinue_curvePoints_positionData_ry, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue(e);
    CHECK(c.curvePoints.positionData.isRelativeYSpecified);
    CHECK_DOUBLES_EQUAL(6.0, c.curvePoints.positionData.relativeY, 0.01);
}

T_END

/////////////////////////////////////////////////////////////////////////////////

TEST(parseCurveStop_curveType, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK(api::CurveType::tie == c.curveType);
}

T_END

TEST(parseCurveStop_numberLevel, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK_EQUAL(2, c.numberLevel);
}

T_END

TEST(parseCurveStop_curvePoints_bx, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK(c.curvePoints.isBezierXSpecified);
    CHECK_DOUBLES_EQUAL(13.0, c.curvePoints.bezierX, 0.01);
}

T_END

TEST(parseCurveStop_curvePoints_by, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK(c.curvePoints.isBezierYSpecified);
    CHECK_DOUBLES_EQUAL(14.0, c.curvePoints.bezierY, 0.01);
}

T_END

TEST(parseCurveStop_curvePoints_bo, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK(c.curvePoints.isBezierOffsetSpecified);
    CHECK_EQUAL(1, c.curvePoints.bezierOffset);
}

T_END

TEST(parseCurveStop_curvePoints_positionData_x, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK(c.curvePoints.positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(3.0, c.curvePoints.positionData.defaultX, 0.01);
}

T_END

TEST(parseCurveStop_curvePoints_positionData_y, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK(c.curvePoints.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(4.0, c.curvePoints.positionData.defaultY, 0.01);
}

T_END

TEST(parseCurveStop_curvePoints_positionData_rx, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK(c.curvePoints.positionData.isRelativeXSpecified);
    CHECK_DOUBLES_EQUAL(5.0, c.curvePoints.positionData.relativeX, 0.01);
}

T_END

TEST(parseCurveStop_curvePoints_positionData_ry, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop(e);
    CHECK(c.curvePoints.positionData.isRelativeYSpecified);
    CHECK_DOUBLES_EQUAL(6.0, c.curvePoints.positionData.relativeY, 0.01);
}

T_END

/////////////////////////////////////////////////////////////////////////////////

TEST(writeAttributesFromCurveStart_type, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(core::StartStopContinue::start() == attr.type());
}

T_END

TEST(writeAttributesFromCurveStart_number, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK_EQUAL(2, attr.number()->value());
}

T_END

TEST(writeAttributesFromCurveStart_bezierOffset, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.bezierOffset().has_value());
    CHECK_DOUBLES_EQUAL(1.0, attr.bezierOffset()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_bezierX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.bezierX().has_value());
    CHECK_DOUBLES_EQUAL(13.0, attr.bezierX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_bezierY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.bezierY().has_value());
    CHECK_DOUBLES_EQUAL(14.0, attr.bezierY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_defaultX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.defaultX().has_value());
    CHECK_DOUBLES_EQUAL(3.0, attr.defaultX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_defaultY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(4.0, attr.defaultY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_relativeX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.relativeX().has_value());
    CHECK_DOUBLES_EQUAL(5.0, attr.relativeX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_relativeY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.relativeY().has_value());
    CHECK_DOUBLES_EQUAL(6.0, attr.relativeY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_color, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.color().has_value());
    CHECK_EQUAL(8, static_cast<int>(attr.color()->red()));
    CHECK_EQUAL(9, static_cast<int>(attr.color()->green()));
    CHECK_EQUAL(10, static_cast<int>(attr.color()->blue()));
    CHECK(!attr.color()->alpha().has_value());
}

T_END

TEST(writeAttributesFromCurveStart_lineType, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.lineType().has_value());
    CHECK(core::LineType::wavy() == *attr.lineType());
}

T_END

TEST(writeAttributesFromCurveStart_dashLength, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.dashLength().has_value());
    CHECK_DOUBLES_EQUAL(11.0, attr.dashLength()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_spaceLength, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.spaceLength().has_value());
    CHECK_DOUBLES_EQUAL(12.0, attr.spaceLength()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStart_placement, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.placement().has_value());
    CHECK(core::AboveBelow::below() == *attr.placement());
}

T_END

TEST(writeAttributesFromCurveStart_orientation, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStart(api::CurveType::slur);
    impl::writeAttributesFromCurveStart(c, attr);
    CHECK(attr.orientation().has_value());
    CHECK(core::OverUnder::over() == *attr.orientation());
}

T_END

/////////////////////////////////////////////////////////////////////////////////

TEST(writeAttributesFromCurveContinue_type, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(core::StartStopContinue::continue_() == attr.type());
}

T_END

TEST(writeAttributesFromCurveContinue_number, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK_EQUAL(2, attr.number()->value());
}

T_END

TEST(writeAttributesFromCurveContinue_bezierOffset, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.bezierOffset().has_value());
    CHECK_DOUBLES_EQUAL(1.0, attr.bezierOffset()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_bezierX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.bezierX().has_value());
    CHECK_DOUBLES_EQUAL(13.0, attr.bezierX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_bezierY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.bezierY().has_value());
    CHECK_DOUBLES_EQUAL(14.0, attr.bezierY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_defaultX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.defaultX().has_value());
    CHECK_DOUBLES_EQUAL(3.0, attr.defaultX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_defaultY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(4.0, attr.defaultY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_relativeX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.relativeX().has_value());
    CHECK_DOUBLES_EQUAL(5.0, attr.relativeX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_relativeY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.relativeY().has_value());
    CHECK_DOUBLES_EQUAL(6.0, attr.relativeY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_bezierX2, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.bezierX2().has_value());
    CHECK_DOUBLES_EQUAL(100.0, attr.bezierX2()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_bezierY2, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.bezierY2().has_value());
    CHECK_DOUBLES_EQUAL(101.0, attr.bezierY2()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveContinue_bezierOffset2, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedContinue(api::CurveType::slur);
    impl::writeAttributesFromCurveContinue(c, attr);
    CHECK(attr.bezierOffset2().has_value());
    CHECK_DOUBLES_EQUAL(102.0, attr.bezierOffset2()->value().value(), 0.01);
}

T_END

/////////////////////////////////////////////////////////////////////////////////

TEST(writeAttributesFromCurveStop_type, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK(core::StartStopContinue::stop() == attr.type());
}

T_END

TEST(writeAttributesFromCurveStop_number, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK_EQUAL(2, attr.number()->value());
}

T_END

TEST(writeAttributesFromCurveStop_bezierOffset, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK(attr.bezierOffset().has_value());
    CHECK_DOUBLES_EQUAL(1.0, attr.bezierOffset()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStop_bezierX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK(attr.bezierX().has_value());
    CHECK_DOUBLES_EQUAL(13.0, attr.bezierX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStop_bezierY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK(attr.bezierY().has_value());
    CHECK_DOUBLES_EQUAL(14.0, attr.bezierY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStop_defaultX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK(attr.defaultX().has_value());
    CHECK_DOUBLES_EQUAL(3.0, attr.defaultX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStop_defaultY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK(attr.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(4.0, attr.defaultY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStop_relativeX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK(attr.relativeX().has_value());
    CHECK_DOUBLES_EQUAL(5.0, attr.relativeX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromCurveStop_relativeY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Slur attr;
    auto c = seedStop(api::CurveType::slur);
    impl::writeAttributesFromCurveStop(c, attr);
    CHECK(attr.relativeY().has_value());
    CHECK_DOUBLES_EQUAL(6.0, attr.relativeY()->value().value(), 0.01);
}

T_END

/////////////////////////////////////////////////////////////////////////////////
// laissez-vibrer / let-ring ties (a lone <tied type="let-ring">)

namespace
{
api::TieLetRing seedLetRing()
{
    api::TieLetRing c;
    c.isSpecified = true;
    c.positionData.isDefaultXSpecified = true;
    c.positionData.defaultX = 3.0;
    c.positionData.isDefaultYSpecified = true;
    c.positionData.defaultY = 4.0;
    c.positionData.isRelativeXSpecified = true;
    c.positionData.relativeX = 5.0;
    c.positionData.isRelativeYSpecified = true;
    c.positionData.relativeY = 6.0;
    c.positionData.placement = api::Placement::below;
    c.isColorSpecified = true;
    c.colorData.red = 8;
    c.colorData.green = 9;
    c.colorData.blue = 10;
    c.colorData.isAlphaSpecified = false;
    c.curveOrientation = api::CurveOrientation::overhand;
    return c;
}
} // namespace

TEST(parseTieLetRing_isSpecified, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    auto c = impl::parseTieLetRing(e);
    CHECK(c.isSpecified);
}

T_END

TEST(parseTieLetRing_positionData_x, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    auto c = impl::parseTieLetRing(e);
    CHECK(c.positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(3.0, c.positionData.defaultX, 0.01);
}

T_END

TEST(parseTieLetRing_positionData_y, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    auto c = impl::parseTieLetRing(e);
    CHECK(c.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(4.0, c.positionData.defaultY, 0.01);
}

T_END

TEST(parseTieLetRing_positionData_rx, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    auto c = impl::parseTieLetRing(e);
    CHECK(c.positionData.isRelativeXSpecified);
    CHECK_DOUBLES_EQUAL(5.0, c.positionData.relativeX, 0.01);
}

T_END

TEST(parseTieLetRing_positionData_ry, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    auto c = impl::parseTieLetRing(e);
    CHECK(c.positionData.isRelativeYSpecified);
    CHECK_DOUBLES_EQUAL(6.0, c.positionData.relativeY, 0.01);
}

T_END

TEST(parseTieLetRing_placement, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    auto c = impl::parseTieLetRing(e);
    CHECK(api::Placement::below == c.positionData.placement);
}

T_END

TEST(parseTieLetRing_color, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    auto c = impl::parseTieLetRing(e);
    CHECK(c.isColorSpecified);
    CHECK_EQUAL(8, static_cast<int>(c.colorData.red));
    CHECK_EQUAL(9, static_cast<int>(c.colorData.green));
    CHECK_EQUAL(10, static_cast<int>(c.colorData.blue));
}

T_END

TEST(parseTieLetRing_orientation, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    auto c = impl::parseTieLetRing(e);
    CHECK(api::CurveOrientation::overhand == c.curveOrientation);
}

T_END

// parseCurve must route a let-ring <tied> into NoteData::tieLetRing and leave
// the start/continue/stop curve vectors untouched.
TEST(parseCurve_letRing_routesToTieLetRing, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    e.setType(core::TiedType::letRing());
    api::NoteData nd;
    impl::parseCurve(e, nd);
    CHECK(nd.tieLetRing.isSpecified);
    CHECK_EQUAL(0, static_cast<int>(nd.noteAttachmentData.curveStarts.size()));
    CHECK_EQUAL(0, static_cast<int>(nd.noteAttachmentData.curveStops.size()));
    CHECK_EQUAL(0, static_cast<int>(nd.noteAttachmentData.curveContinuations.size()));
}

T_END

// A regular tie start must still route into the curve vectors and must not be
// mistaken for a let-ring tie.
TEST(parseCurve_start_doesNotSetTieLetRing, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>(); // TiedType defaults to start
    api::NoteData nd;
    impl::parseCurve(e, nd);
    CHECK(!nd.tieLetRing.isSpecified);
    CHECK_EQUAL(1, static_cast<int>(nd.noteAttachmentData.curveStarts.size()));
}

T_END

TEST(writeAttributesFromTieLetRing_type, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Tied attr;
    auto c = seedLetRing();
    impl::writeAttributesFromTieLetRing(c, attr);
    CHECK(core::TiedType::letRing() == attr.type());
}

T_END

TEST(writeAttributesFromTieLetRing_defaultX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Tied attr;
    auto c = seedLetRing();
    impl::writeAttributesFromTieLetRing(c, attr);
    CHECK(attr.defaultX().has_value());
    CHECK_DOUBLES_EQUAL(3.0, attr.defaultX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromTieLetRing_defaultY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Tied attr;
    auto c = seedLetRing();
    impl::writeAttributesFromTieLetRing(c, attr);
    CHECK(attr.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(4.0, attr.defaultY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromTieLetRing_relativeX, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Tied attr;
    auto c = seedLetRing();
    impl::writeAttributesFromTieLetRing(c, attr);
    CHECK(attr.relativeX().has_value());
    CHECK_DOUBLES_EQUAL(5.0, attr.relativeX()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromTieLetRing_relativeY, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Tied attr;
    auto c = seedLetRing();
    impl::writeAttributesFromTieLetRing(c, attr);
    CHECK(attr.relativeY().has_value());
    CHECK_DOUBLES_EQUAL(6.0, attr.relativeY()->value().value(), 0.01);
}

T_END

TEST(writeAttributesFromTieLetRing_placement, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Tied attr;
    auto c = seedLetRing();
    impl::writeAttributesFromTieLetRing(c, attr);
    CHECK(attr.placement().has_value());
    CHECK(core::AboveBelow::below() == *attr.placement());
}

T_END

TEST(writeAttributesFromTieLetRing_color, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Tied attr;
    auto c = seedLetRing();
    impl::writeAttributesFromTieLetRing(c, attr);
    CHECK(attr.color().has_value());
    CHECK_EQUAL(8, static_cast<int>(attr.color()->red()));
    CHECK_EQUAL(9, static_cast<int>(attr.color()->green()));
    CHECK_EQUAL(10, static_cast<int>(attr.color()->blue()));
}

T_END

TEST(writeAttributesFromTieLetRing_orientation, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    core::Tied attr;
    auto c = seedLetRing();
    impl::writeAttributesFromTieLetRing(c, attr);
    CHECK(attr.orientation().has_value());
    CHECK(core::OverUnder::over() == *attr.orientation());
}

T_END

// Round-trip: write a seeded TieLetRing to a core::Tied, then read it back and
// confirm the api values survive.
TEST(tieLetRing_roundTrip, CurveFunctions)
{
    using namespace mx::impl;
    using namespace mx;
    const auto original = seedLetRing();
    core::Tied attr;
    impl::writeAttributesFromTieLetRing(original, attr);
    const auto roundTripped = impl::parseTieLetRing(attr);
    CHECK(original == roundTripped);
}

T_END

#endif
