// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/OttavaData.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/DirectionType.h"
#include "mx/core/generated/DirectionTypeChoice.h"
#include "mx/core/generated/OctaveShift.h"
#include "mx/impl/DirectionReader.h"

#include <memory>

using namespace mx;
using namespace mx::impl;

TEST(ottavaStart15ma, DirectionReader)
{
    const int tickTimePosition = 150;
    core::Direction dir{};
    core::OctaveShift oct{};
    oct.setType(core::UpDownStopContinue::down());
    oct.setSize(15);
    core::DirectionType dirType{};
    dirType.setChoice(core::DirectionTypeChoice::octaveShift(oct));
    dir.setDirectionType(core::OneOrMore<core::DirectionType>{dirType});
    Cursor cursor{1, 100};
    cursor.tickTimePosition = tickTimePosition;
    DirectionReader reader{dir, cursor};
    auto directionData = reader.getDirectionData();
    CHECK_EQUAL(1, directionData.directionTypes.size());
    CHECK(directionData.directionTypes.front().isOttavaStart());
    const auto ottavaStart = directionData.directionTypes.front().ottavaStart();
    CHECK_EQUAL(tickTimePosition, ottavaStart.spannerStart.tickTimePosition);
    CHECK(api::OttavaType::o15ma == ottavaStart.ottavaType);
}

T_END

TEST(ottavaStart8vaAnd8vb, DirectionReader)
{
    const int tickTimePosition = 199;

    // add an 8vb start
    core::OctaveShift oct1{};
    oct1.setType(core::UpDownStopContinue::up());
    oct1.setSize(8);
    core::DirectionType dirType1{};
    dirType1.setChoice(core::DirectionTypeChoice::octaveShift(oct1));

    // add a phantom "continue" which should be ignored by our code
    core::OctaveShift oct2{};
    oct2.setType(core::UpDownStopContinue::continue_());
    core::DirectionType dirType2{};
    dirType2.setChoice(core::DirectionTypeChoice::octaveShift(oct2));

    // add an 8va but rely on the default 'size'
    core::OctaveShift oct3{};
    oct3.setType(core::UpDownStopContinue::down());
    oct3.setNumber(core::NumberLevel{3});
    core::DirectionType dirType3{};
    dirType3.setChoice(core::DirectionTypeChoice::octaveShift(oct3));

    core::Direction dir{};
    dir.setDirectionType(core::OneOrMore<core::DirectionType>{dirType1});
    dir.addDirectionType(dirType2);
    dir.addDirectionType(dirType3);

    Cursor cursor{1, 111};
    cursor.tickTimePosition = tickTimePosition;
    DirectionReader reader{dir, cursor};
    auto directionData = reader.getDirectionData();

    CHECK_EQUAL(2, directionData.directionTypes.size());
    CHECK(directionData.directionTypes.front().isOttavaStart());
    auto ottavaStart = directionData.directionTypes.front().ottavaStart();

    CHECK_EQUAL(tickTimePosition, ottavaStart.spannerStart.tickTimePosition);
    CHECK(api::OttavaType::o8vb == ottavaStart.ottavaType);
    CHECK(ottavaStart.spannerStart.number.isUnspecified());

    CHECK(directionData.directionTypes.at(1).isOttavaStart());
    ottavaStart = directionData.directionTypes.at(1).ottavaStart();
    CHECK_EQUAL(tickTimePosition, ottavaStart.spannerStart.tickTimePosition);
    CHECK(api::OttavaType::o8va == ottavaStart.ottavaType);
    CHECK(api::SpannerNumber(3) == ottavaStart.spannerStart.number);
}

T_END

TEST(ottavaStop, DirectionReader)
{
    const int tickTimePosition = 150;
    core::OctaveShift oct{};
    oct.setType(core::UpDownStopContinue::stop());
    oct.setSize(15);
    core::DirectionType dirType{};
    dirType.setChoice(core::DirectionTypeChoice::octaveShift(oct));
    core::Direction dir{};
    dir.setDirectionType(core::OneOrMore<core::DirectionType>{dirType});
    Cursor cursor{1, 100};
    cursor.tickTimePosition = tickTimePosition;
    DirectionReader reader{dir, cursor};
    auto directionData = reader.getDirectionData();
    CHECK_EQUAL(1, directionData.directionTypes.size());
    CHECK(directionData.directionTypes.front().isOttavaStop());
    const auto ottavaStop = directionData.directionTypes.front().ottavaStop();
    CHECK_EQUAL(tickTimePosition, ottavaStop.spannerStop.tickTimePosition);
    CHECK(ottavaStop.spannerStop.number.isUnspecified());
    CHECK(ottavaStop.size.has_value());
    CHECK_EQUAL(15, *ottavaStop.size);
}

T_END

#endif
