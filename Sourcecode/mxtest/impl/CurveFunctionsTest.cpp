// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/impl/CurveFunctions.h"
#include "mx/core/elements/Slur.h"
#include "mx/core/elements/Tied.h"

using namespace mx;
using namespace mx::impl;

namespace
{
    template<typename T>
    std::shared_ptr<T> seed()
    {
        using namespace mx::impl;
        using namespace mx;
        auto e = std::make_shared<T>();
        auto& attr = *e->getAttributes();
        attr.type = core::StartStopContinue::start;
        attr.bezierOffset.setValue( 1.0 );
        attr.hasBezierOffset = true;
        attr.bezierOffset2.setValue( 2.0 );
        attr.hasBezierOffset2 = true;
        attr.defaultX.setValue( 3.0 );
        attr.hasDefaultX = true;
        attr.defaultY.setValue( 4.0 );
        attr.hasDefaultY = true;
        attr.relativeX.setValue( 5.0 );
        attr.hasRelativeX = true;
        attr.relativeY.setValue( 6.0 );
        attr.hasRelativeY = true;
        attr.color = core::Color{ 7, 8, 9, 10 };
        attr.hasColor = true;
        attr.dashLength.setValue( 11.0 );
        attr.hasDashLength = true;
        attr.spaceLength.setValue( 12.0 );
        attr.hasSpaceLength = true;
        attr.number.setValue( 2 );
        attr.hasNumber = true;
        attr.lineType = core::LineType::wavy;
        attr.hasLineType = true;
        attr.placement = core::AboveBelow::below;
        attr.hasPlacement = true;
        attr.orientation = core::OverUnder::over;
        attr.hasOrientation = true;
        attr.bezierX.setValue( 13.0 );
        attr.hasBezierX = true;
        attr.bezierY.setValue( 14.0 );
        attr.hasBezierY = true;
        attr.bezierX2.setValue( 15.0 );
        attr.hasBezierX2 = true;
        attr.bezierY2.setValue( 16.0 );
        attr.hasBezierY2 = true;
        return e;
    }
    
    api::CurveStart seedStart( api::CurveType t )
    {
        api::CurveStart c{ t };
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
    
    api::CurveContinue seedContinue( api::CurveType t )
    {
        api::CurveContinue c{ t };
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
    
    api::CurveStop seedStop( api::CurveType t )
    {
        api::CurveStop c{ t };
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
}



TEST( parseCurveStart_curveType, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( api::CurveType::tie == c.curveType );
}
T_END


TEST( parseCurveStart_numberLevel, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK_EQUAL( 2, c.numberLevel );
}
T_END


TEST( parseCurveStart_curvePoints_bx, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( c.curvePoints.isBezierXSpecified );
    CHECK_DOUBLES_EQUAL( 13.0, c.curvePoints.bezierX, 0.01 );
}
T_END


TEST( parseCurveStart_curvePoints_by, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( c.curvePoints.isBezierYSpecified );
    CHECK_DOUBLES_EQUAL( 14.0, c.curvePoints.bezierY, 0.01 );
}
T_END


TEST( parseCurveStart_curvePoints_bo, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( c.curvePoints.isBezierOffsetSpecified );
    CHECK_EQUAL( 1, c.curvePoints.bezierOffset );
}
T_END


TEST( parseCurveStart_curvePoints_positionData_x, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( c.curvePoints.positionData.isDefaultXSpecified );
    CHECK_DOUBLES_EQUAL( 3.0, c.curvePoints.positionData.defaultX, 0.01 );
}
T_END


TEST( parseCurveStart_curvePoints_positionData_y, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( c.curvePoints.positionData.isDefaultYSpecified );
    CHECK_DOUBLES_EQUAL( 4.0, c.curvePoints.positionData.defaultY, 0.01 );
}
T_END


TEST( parseCurveStart_curvePoints_positionData_rx, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( c.curvePoints.positionData.isRelativeXSpecified );
    CHECK_DOUBLES_EQUAL( 5.0, c.curvePoints.positionData.relativeX, 0.01 );
}
T_END


TEST( parseCurveStart_curvePoints_positionData_ry, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( c.curvePoints.positionData.isRelativeYSpecified );
    CHECK_DOUBLES_EQUAL( 6.0, c.curvePoints.positionData.relativeY, 0.01 );
}
T_END


TEST( parseCurveStart_lineData_dashLength, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    const auto& l = c.lineData;
    CHECK( l.isDashLengthSpecified );
    CHECK_DOUBLES_EQUAL( 11.0, l.dashLength, 0.01 );
}
T_END


TEST( parseCurveStart_lineData_spaceLength, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    const auto& l = c.lineData;
    CHECK( l.isSpaceLengthSpecified );
    CHECK_DOUBLES_EQUAL( 12.0, l.spaceLength, 0.01 );
}
T_END


TEST( parseCurveStart_lineData_lineType, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    const auto& l = c.lineData;
    CHECK( api::LineType::wavy == l.lineType );
}
T_END


TEST( parseCurveStart_color, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    const auto& color = c.colorData;
    CHECK( c.isColorSpecified );
    CHECK_EQUAL( 8, static_cast<int>( color.red ) );
    CHECK_EQUAL( 9, static_cast<int>( color.green ) );
    CHECK_EQUAL( 10, static_cast<int>( color.blue ) );
    CHECK( color.isAlphaSpecified );
    CHECK_EQUAL( 7, static_cast<int>( color.alpha ) );
}
T_END


TEST( parseCurveStart_orientation, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStart( *e );
    CHECK( api::CurveOrientation::overhand == c.curveOrientation );
}
T_END


/////////////////////////////////////////////////////////////////////////////////


TEST( parseCurveContinue_curveType, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( api::CurveType::tie == c.curveType );
}
T_END


TEST( parseCurveContinue_numberLevel, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK_EQUAL( 2, c.numberLevel );
}
T_END


TEST( parseCurveContinue_curvePoints_bx, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.curvePoints.isBezierXSpecified );
    CHECK_DOUBLES_EQUAL( 13.0, c.curvePoints.bezierX, 0.01 );
}
T_END


TEST( parseCurveContinue_curvePoints_by, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.curvePoints.isBezierYSpecified );
    CHECK_DOUBLES_EQUAL( 14.0, c.curvePoints.bezierY, 0.01 );
}
T_END


TEST( parseCurveContinue_curvePoints_bo, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.curvePoints.isBezierOffsetSpecified );
    CHECK_EQUAL( 1, c.curvePoints.bezierOffset );
}
T_END


TEST( parseCurveContinue_curvePoints_bo2, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.isBezierOffset2Specified );
    CHECK_DOUBLES_EQUAL( 2.0, c.bezierOffset2, 0.01 );
}
T_END


TEST( parseCurveContinue_bx2, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.isBezierX2Specified );
    CHECK_DOUBLES_EQUAL( 15.0, c.bezierX2, 0.01 );
}
T_END


TEST( parseCurveContinue_by2, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.isBezierY2Specified );
    CHECK_DOUBLES_EQUAL( 16.0, c.bezierY2, 0.01 );
}
T_END



TEST( parseCurveContinue_curvePoints_positionData_x, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.curvePoints.positionData.isDefaultXSpecified );
    CHECK_DOUBLES_EQUAL( 3.0, c.curvePoints.positionData.defaultX, 0.01 );
}
T_END


TEST( parseCurveContinue_curvePoints_positionData_y, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.curvePoints.positionData.isDefaultYSpecified );
    CHECK_DOUBLES_EQUAL( 4.0, c.curvePoints.positionData.defaultY, 0.01 );
}
T_END


TEST( parseCurveContinue_curvePoints_positionData_rx, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.curvePoints.positionData.isRelativeXSpecified );
    CHECK_DOUBLES_EQUAL( 5.0, c.curvePoints.positionData.relativeX, 0.01 );
}
T_END


TEST( parseCurveContinue_curvePoints_positionData_ry, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveContinue( *e );
    CHECK( c.curvePoints.positionData.isRelativeYSpecified );
    CHECK_DOUBLES_EQUAL( 6.0, c.curvePoints.positionData.relativeY, 0.01 );
}
T_END


/////////////////////////////////////////////////////////////////////////////////


TEST( parseCurveStop_curveType, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK( api::CurveType::tie == c.curveType );
}
T_END


TEST( parseCurveStop_numberLevel, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK_EQUAL( 2, c.numberLevel );
}
T_END


TEST( parseCurveStop_curvePoints_bx, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK( c.curvePoints.isBezierXSpecified );
    CHECK_DOUBLES_EQUAL( 13.0, c.curvePoints.bezierX, 0.01 );
}
T_END


TEST( parseCurveStop_curvePoints_by, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK( c.curvePoints.isBezierYSpecified );
    CHECK_DOUBLES_EQUAL( 14.0, c.curvePoints.bezierY, 0.01 );
}
T_END


TEST( parseCurveStop_curvePoints_bo, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK( c.curvePoints.isBezierOffsetSpecified );
    CHECK_EQUAL( 1, c.curvePoints.bezierOffset );
}
T_END


TEST( parseCurveStop_curvePoints_positionData_x, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK( c.curvePoints.positionData.isDefaultXSpecified );
    CHECK_DOUBLES_EQUAL( 3.0, c.curvePoints.positionData.defaultX, 0.01 );
}
T_END


TEST( parseCurveStop_curvePoints_positionData_y, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK( c.curvePoints.positionData.isDefaultYSpecified );
    CHECK_DOUBLES_EQUAL( 4.0, c.curvePoints.positionData.defaultY, 0.01 );
}
T_END


TEST( parseCurveStop_curvePoints_positionData_rx, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK( c.curvePoints.positionData.isRelativeXSpecified );
    CHECK_DOUBLES_EQUAL( 5.0, c.curvePoints.positionData.relativeX, 0.01 );
}
T_END


TEST( parseCurveStop_curvePoints_positionData_ry, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    auto e = seed<core::Tied>();
    auto c = impl::parseCurveStop( *e );
    CHECK( c.curvePoints.positionData.isRelativeYSpecified );
    CHECK_DOUBLES_EQUAL( 6.0, c.curvePoints.positionData.relativeY, 0.01 );
}
T_END


/////////////////////////////////////////////////////////////////////////////////


TEST( writeAttributesFromCurveStart_type, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( core::StartStopContinue::start == attr.type );
}
T_END

TEST( writeAttributesFromCurveStart_number, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK_EQUAL( 2, attr.number.getValue() );
}
T_END


TEST( writeAttributesFromCurveStart_bezierOffset, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasBezierOffset );
    CHECK_DOUBLES_EQUAL( 1.0, attr.bezierOffset.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_bezierX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasBezierX );
    CHECK_DOUBLES_EQUAL( 13.0, attr.bezierX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_bezierY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasBezierY );
    CHECK_DOUBLES_EQUAL( 14.0, attr.bezierY.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_defaultX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasDefaultX );
    CHECK_DOUBLES_EQUAL( 3.0, attr.defaultX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_defaultY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasDefaultY );
    CHECK_DOUBLES_EQUAL( 4.0, attr.defaultY.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_relativeX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasRelativeX );
    CHECK_DOUBLES_EQUAL( 5.0, attr.relativeX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_relativeY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasRelativeY );
    CHECK_DOUBLES_EQUAL( 6.0, attr.relativeY.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_color, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasColor );
    CHECK_EQUAL( 8, static_cast<int>( attr.color.getRed() ) );
    CHECK_EQUAL( 9, static_cast<int>( attr.color.getGreen() ) );
    CHECK_EQUAL( 10, static_cast<int>( attr.color.getBlue() ) );
    CHECK( core::Color::ColorType::RGB == attr.color.getColorType() );
}
T_END


TEST( writeAttributesFromCurveStart_lineType, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasLineType );
    CHECK( core::LineType::wavy == attr.lineType );
}
T_END


TEST( writeAttributesFromCurveStart_dashLength, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasDashLength );
    CHECK_DOUBLES_EQUAL( 11.0, attr.dashLength.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_spaceLength, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasSpaceLength );
    CHECK_DOUBLES_EQUAL( 12.0, attr.spaceLength.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStart_placement, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasPlacement );
    CHECK( core::AboveBelow::below == attr.placement );
}
T_END


TEST( writeAttributesFromCurveStart_orientation, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStart( api::CurveType::slur );
    impl::writeAttributesFromCurveStart( c, attr );
    CHECK( attr.hasOrientation );
    CHECK( core::OverUnder::over == attr.orientation );
}
T_END


/////////////////////////////////////////////////////////////////////////////////


TEST( writeAttributesFromCurveContinue_type, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( core::StartStopContinue::continue_ == attr.type );
}
T_END


TEST( writeAttributesFromCurveContinue_number, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK_EQUAL( 2, attr.number.getValue() );
}
T_END


TEST( writeAttributesFromCurveContinue_bezierOffset, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasBezierOffset );
    CHECK_DOUBLES_EQUAL( 1.0, attr.bezierOffset.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_bezierX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasBezierX );
    CHECK_DOUBLES_EQUAL( 13.0, attr.bezierX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_bezierY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasBezierY );
    CHECK_DOUBLES_EQUAL( 14.0, attr.bezierY.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_defaultX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasDefaultX );
    CHECK_DOUBLES_EQUAL( 3.0, attr.defaultX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_defaultY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasDefaultY );
    CHECK_DOUBLES_EQUAL( 4.0, attr.defaultY.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_relativeX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasRelativeX );
    CHECK_DOUBLES_EQUAL( 5.0, attr.relativeX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_relativeY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasRelativeY );
    CHECK_DOUBLES_EQUAL( 6.0, attr.relativeY.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_bezierX2, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasBezierX2 );
    CHECK_DOUBLES_EQUAL( 100.0, attr.bezierX2.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_bezierY2, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasBezierY2 );
    CHECK_DOUBLES_EQUAL( 101.0, attr.bezierY2.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveContinue_bezierOffset2, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedContinue( api::CurveType::slur );
    impl::writeAttributesFromCurveContinue( c, attr );
    CHECK( attr.hasBezierOffset2 );
    CHECK_DOUBLES_EQUAL( 102.0, attr.bezierOffset2.getValue(), 0.01 );
}
T_END


/////////////////////////////////////////////////////////////////////////////////


TEST( writeAttributesFromCurveStop_type, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK( core::StartStopContinue::stop == attr.type );
}
T_END


TEST( writeAttributesFromCurveStop_number, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK_EQUAL( 2, attr.number.getValue() );
}
T_END


TEST( writeAttributesFromCurveStop_bezierOffset, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK( attr.hasBezierOffset );
    CHECK_DOUBLES_EQUAL( 1.0, attr.bezierOffset.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStop_bezierX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK( attr.hasBezierX );
    CHECK_DOUBLES_EQUAL( 13.0, attr.bezierX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStop_bezierY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK( attr.hasBezierY );
    CHECK_DOUBLES_EQUAL( 14.0, attr.bezierY.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStop_defaultX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK( attr.hasDefaultX );
    CHECK_DOUBLES_EQUAL( 3.0, attr.defaultX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStop_defaultY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK( attr.hasDefaultY );
    CHECK_DOUBLES_EQUAL( 4.0, attr.defaultY.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStop_relativeX, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK( attr.hasRelativeX );
    CHECK_DOUBLES_EQUAL( 5.0, attr.relativeX.getValue(), 0.01 );
}
T_END


TEST( writeAttributesFromCurveStop_relativeY, CurveFunctions )
{
    using namespace mx::impl;
    using namespace mx;
    core::SlurAttributes attr;
    auto c = seedStop( api::CurveType::slur );
    impl::writeAttributesFromCurveStop( c, attr );
    CHECK( attr.hasRelativeY );
    CHECK_DOUBLES_EQUAL( 6.0, attr.relativeY.getValue(), 0.01 );
}
T_END


#endif
