// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/core/elements/BracketAttributes.h"
#include "mx/core/elements/CreditWordsAttributes.h"
#include "mx/core/elements/DirectionAttributes.h"

using namespace mx;
using namespace mx::impl;

// TODO - make real tests

TEST( setAttributesFromPositionDataBracket, PositionFunctions )
{
    core::BracketAttributes attr;
    api::PositionData positionData;
    
    positionData.hasDefaultX = true;
    positionData.defaultX = 0.1;
    positionData.hasDefaultY = true;
    positionData.defaultY = 0.2;
    
    positionData.hasRelativeX = true;
    positionData.relativeX = 0.3;
    positionData.hasRelativeY = true;
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
