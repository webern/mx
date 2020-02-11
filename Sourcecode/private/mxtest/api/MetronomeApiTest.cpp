// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/testFramework.h"
#include "mxtest/api/RoundTrip.h"
#include "mx/api/DocumentManager.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;


TEST( roundTripBpm, MetronomeApi )
{
    const auto expectedDurationName = DurationName::dur16th;
    const int expectedDots = 1;
    const int expectedBeatsPerMinute = 123;
    const int expectedTickTimePosition = 77;
    
    ScoreData expectedScoreData;
    expectedScoreData.ticksPerQuarter = 100;
    expectedScoreData.parts.emplace_back();
    auto& expectedPart = expectedScoreData.parts.back();
    expectedPart.measures.emplace_back();
    auto& expectedMeasure = expectedPart.measures.back();
    expectedMeasure.staves.emplace_back();
    auto& expectedStaff = expectedMeasure.staves.back();
    expectedStaff.directions.emplace_back();
    auto& expectedDirection = expectedStaff.directions.back();
    expectedDirection.tempos.emplace_back();
    expectedDirection.tickTimePosition = expectedTickTimePosition;
    auto& expectedTempo = expectedDirection.tempos.back();
    expectedTempo.tempoType = TempoType::beatsPerMinute;
    auto& expectedBpm = expectedTempo.beatsPerMinute;
    expectedBpm.durationName = expectedDurationName;
    expectedBpm.dots = expectedDots;
    expectedBpm.beatsPerMinute = expectedBeatsPerMinute;
    
    auto actualScoreData = roundTrip( expectedScoreData );
    
    auto& actualPart = actualScoreData.parts.back();
    auto& actualMeasure = actualPart.measures.back();
    auto& actualStaff = actualMeasure.staves.back();
    auto& actualDirection = actualStaff.directions.back();
    auto& actualTempo = actualDirection.tempos.back();
    actualTempo.tempoType = TempoType::beatsPerMinute;
    auto& actualBpm = actualTempo.beatsPerMinute;
    
    CHECK_EQUAL( expectedBeatsPerMinute, actualBpm.beatsPerMinute );
    CHECK_EQUAL( expectedDots, actualBpm.dots );
    CHECK( expectedDurationName == actualBpm.durationName );
    CHECK_EQUAL( expectedTickTimePosition, actualDirection.tickTimePosition );
}
T_END

#endif
