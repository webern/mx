// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/impl/DirectionWriter.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/api/OttavaData.h"
#include "mx/core/elements/OctaveShift.h"

#include <memory>

using namespace mx;
using namespace mx::impl;

TEST( ottavaStartStop, DirectionWriter )
{
    Cursor cursor{ 1, 100 };
    cursor.isFirstMeasureInPart = false;
    api::DirectionData directionData;
    
    directionData.ottavaStops.emplace_back( api::SpannerStop{} );
    auto& stop = directionData.ottavaStops.back();
    stop.numberLevel = 2;
    
    directionData.ottavaStarts.emplace_back( api::OttavaStart{} );
    auto& start = directionData.ottavaStarts.back();
    start.ottavaType = api::OttavaType::o15mb;
    start.spannerStart.positionData.hasDefaultX = true;
    start.spannerStart.positionData.defaultX = 100.0;
    
    DirectionWriter writer{ directionData, cursor };
    auto direction = writer.getDirection();
    const auto& directionTypes = direction->getDirectionTypeSet();
    
    CHECK_EQUAL( 2, directionTypes.size() );
}
T_END


#endif
