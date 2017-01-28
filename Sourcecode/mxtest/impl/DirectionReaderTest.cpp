// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/impl/DirectionReader.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/api/OttavaData.h"
#include "mx/core/elements/OctaveShift.h"

#include <memory>

using namespace mx;
using namespace mx::impl;

TEST( ottavaStart15mb, DirectionReader )
{
    const int tickTimePosition = 150;
    auto dir = core::makeDirection();
    CHECK_EQUAL( 1, dir->getDirectionTypeSet().size() );
    auto dirType = dir->getDirectionTypeSet().front();
    dirType->setChoice( core::DirectionType::Choice::octaveShift );
    auto oct = dirType->getOctaveShift();
    auto attr = oct->getAttributes();
    attr->type = core::UpDownStopContinue::down;
    attr->hasSize = true;
    attr->size = core::PositiveInteger{ 15 };
    Cursor cursor{ 1, 100 };
    cursor.tickTimePosition = tickTimePosition;
    DirectionReader reader{ *dir, cursor };
    auto directionData = reader.getDirectionData();
    CHECK_EQUAL( 1, directionData.ottavaStarts.size() );
    const auto& ottavaStart = directionData.ottavaStarts.front();
    CHECK_EQUAL( tickTimePosition, ottavaStart.spannerStart.tickTimePosition );
    CHECK( api::OttavaType::o15mb == ottavaStart.ottavaType );
}
T_END


TEST( ottavaStart8vaAnd8vb, DirectionReader )
{
    const int tickTimePosition = 199;
    
    // add an 8va start
    auto dir = core::makeDirection();
    CHECK_EQUAL( 1, dir->getDirectionTypeSet().size() );
    auto dirType = dir->getDirectionTypeSet().front();
    dirType->setChoice( core::DirectionType::Choice::octaveShift );
    auto oct = dirType->getOctaveShift();
    auto attr = oct->getAttributes();
    attr->type = core::UpDownStopContinue::up;
    attr->hasSize = true;
    attr->size = core::PositiveInteger{ 8 };

    // add a phantom "continue" which should be ignored by our code
    dirType = core::makeDirectionType();
    dirType->setChoice( core::DirectionType::Choice::octaveShift );
    dir->addDirectionType( dirType );
    oct = dirType->getOctaveShift();
    attr = oct->getAttributes();
    attr->type = core::UpDownStopContinue::continue_;

    // add an 8vb but rely on the default 'size'
    dirType = core::makeDirectionType();
    dirType->setChoice( core::DirectionType::Choice::octaveShift );
    dir->addDirectionType( dirType );
    oct = dirType->getOctaveShift();
    attr = oct->getAttributes();
    attr->type = core::UpDownStopContinue::down;
    attr->hasNumber = true;
    attr->number = core::NumberLevel{ 3 };

    Cursor cursor{ 1, 111 };
    cursor.tickTimePosition = tickTimePosition;
    DirectionReader reader{ *dir, cursor };
    auto directionData = reader.getDirectionData();
    
    CHECK_EQUAL( 2, directionData.ottavaStarts.size() );
    auto ottavaStart = directionData.ottavaStarts.front();
    
    CHECK_EQUAL( tickTimePosition, ottavaStart.spannerStart.tickTimePosition );
    CHECK( api::OttavaType::o8va == ottavaStart.ottavaType );
    CHECK_EQUAL( -1, ottavaStart.spannerStart.numberLevel );
    
    ottavaStart = directionData.ottavaStarts.at( 1 );
    CHECK_EQUAL( tickTimePosition, ottavaStart.spannerStart.tickTimePosition );
    CHECK( api::OttavaType::o8vb == ottavaStart.ottavaType );
    CHECK_EQUAL( 3, ottavaStart.spannerStart.numberLevel );
}
T_END


TEST( ottavaStop, DirectionReader )
{
    const int tickTimePosition = 150;
    auto dir = core::makeDirection();
    CHECK_EQUAL( 1, dir->getDirectionTypeSet().size() );
    auto dirType = dir->getDirectionTypeSet().front();
    dirType->setChoice( core::DirectionType::Choice::octaveShift );
    auto oct = dirType->getOctaveShift();
    auto attr = oct->getAttributes();
    attr->type = core::UpDownStopContinue::stop;
    attr->hasSize = true;
    attr->size = core::PositiveInteger{ 15 };
    Cursor cursor{ 1, 100 };
    cursor.tickTimePosition = tickTimePosition;
    DirectionReader reader{ *dir, cursor };
    auto directionData = reader.getDirectionData();
    CHECK_EQUAL( 1, directionData.ottavaStops.size() );
    const auto& ottavaStop = directionData.ottavaStops.front();
    CHECK_EQUAL( tickTimePosition, ottavaStop.tickTimePosition );
    CHECK_EQUAL( -1, ottavaStop.numberLevel );
}
T_END


#endif
