// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/core/elements/BracketAttributes.h"
#include "mx/core/elements/CreditWordsAttributes.h"
#include "mx/core/elements/DirectionAttributes.h"

using namespace mx;
using namespace mx::impl;

TEST( setAttributesFromPositionDataBracket, PositionFunctions )
{
    core::BracketAttributes attr;
    api::PositionData positionData;
    
    positionData.isDefaultXSpecified = true;
    positionData.defaultX = 0.1;
    positionData.isDefaultYSpecified = true;
    positionData.defaultY = 0.2;
    
    positionData.isRelativeXSpecified = true;
    positionData.relativeX = 0.3;
    positionData.isRelativeYSpecified = true;
    positionData.relativeY = 0.4;
    
    impl::setAttributesFromPositionData( positionData, attr );
    
    CHECK( attr.hasDefaultX );
    CHECK_DOUBLES_EQUAL( 0.1, attr.defaultX.getValue(), 0.0001 );
    
    CHECK( attr.hasDefaultY );
    CHECK_DOUBLES_EQUAL( 0.2, attr.defaultY.getValue(), 0.0001 );
    
    CHECK( attr.hasRelativeX );
    CHECK_DOUBLES_EQUAL( 0.3, attr.relativeX.getValue(), 0.0001 );
    
    CHECK( attr.hasRelativeY );
    CHECK_DOUBLES_EQUAL( 0.4, attr.relativeY.getValue(), 0.0001 );
}
T_END


class TotallyBogusClassToTestSetAttributesFromPositionData {};


TEST( setAttributesFromPositionDataFakeClass, PositionFunctions )
{
    TotallyBogusClassToTestSetAttributesFromPositionData fake;
    api::PositionData positionData;
    impl::setAttributesFromPositionData( positionData, fake );
    
    // only check that it compiles
    CHECK( true );
}
T_END


TEST( setAttributesFromPositionDataDirectionAttributes, PositionFunctions )
{
    core::DirectionAttributes attr;
    api::PositionData positionData;
    
    positionData.placement = api::Placement::below;
    
    impl::setAttributesFromPositionData( positionData, attr );
    
    CHECK( attr.hasPlacement );
    CHECK( attr.placement == core::AboveBelow::below );
}
T_END

#endif
