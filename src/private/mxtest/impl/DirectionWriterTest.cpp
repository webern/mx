// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/OttavaData.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/DirectionType.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/OctaveShift.h"
#include "mx/impl/DirectionReader.h"
#include "mx/impl/DirectionWriter.h"

#include <memory>

using namespace mx;
using namespace mx::impl;

TEST(ottavaStartStop, DirectionWriter)
{
    Cursor cursor{1, 100};
    cursor.isFirstMeasureInPart = false;
    api::DirectionData directionData;

    directionData.ottavaStops.emplace_back(api::SpannerStop{});
    auto &stop = directionData.ottavaStops.back();
    stop.numberLevel = 2;

    directionData.ottavaStarts.emplace_back(api::OttavaStart{});
    auto &start = directionData.ottavaStarts.back();
    start.ottavaType = api::OttavaType::o15mb;
    start.spannerStart.positionData.isDefaultXSpecified = true;
    start.spannerStart.positionData.defaultX = 100.0;

    DirectionWriter writer{directionData, cursor};
    const auto mdcSet = writer.getDirectionLikeThings();
    CHECK(mdcSet.front().isDirection());
    const auto &direction = mdcSet.front().asDirection();
    const auto &directionTypes = direction.directionType();

    CHECK_EQUAL(2, directionTypes.size());
}

T_END

// Build a segno and a coda carrying the full empty-print-object-style-align attribute set plus
// smufl and id, write them with DirectionWriter, read them back with DirectionReader, and confirm
// every field survives the api -> core -> api trip.
TEST(segnoAndCodaRoundTrip, DirectionWriter)
{
    api::SegnoData segno;
    segno.positionData.isDefaultXSpecified = true;
    segno.positionData.defaultX = 1.0;
    segno.positionData.isDefaultYSpecified = true;
    segno.positionData.defaultY = 2.0;
    segno.positionData.isRelativeXSpecified = true;
    segno.positionData.relativeX = 3.0;
    segno.positionData.isRelativeYSpecified = true;
    segno.positionData.relativeY = 4.0;
    segno.positionData.horizontalAlignmnet = api::HorizontalAlignment::left;
    segno.positionData.verticalAlignment = api::VerticalAlignment::top;
    segno.fontData.fontFamily = {"Maestro"};
    segno.fontData.style = api::FontStyle::italic;
    segno.fontData.weight = api::FontWeight::bold;
    segno.fontData.sizeType = api::FontSizeType::point;
    segno.fontData.sizePoint = 18.0;
    segno.isColorSpecified = true;
    segno.colorData.red = 0;
    segno.colorData.green = 0;
    segno.colorData.blue = 0;
    segno.colorData.isAlphaSpecified = true;
    segno.colorData.alpha = 255;
    segno.isSmuflSpecified = true;
    segno.smufl = "segno";
    segno.isIdSpecified = true;
    segno.id = "id3";

    api::CodaData coda;
    coda.positionData.isDefaultXSpecified = true;
    coda.positionData.defaultX = 5.0;
    coda.positionData.horizontalAlignmnet = api::HorizontalAlignment::center;
    coda.positionData.verticalAlignment = api::VerticalAlignment::middle;
    coda.fontData.fontFamily = {"Maestro"};
    coda.fontData.style = api::FontStyle::normal;
    coda.fontData.weight = api::FontWeight::normal;
    coda.isColorSpecified = true;
    coda.colorData.red = 12;
    coda.colorData.green = 34;
    coda.colorData.blue = 56;
    coda.isSmuflSpecified = true;
    coda.smufl = "coda";
    coda.isIdSpecified = true;
    coda.id = "id7";

    api::DirectionData directionData;
    directionData.segnos.push_back(segno);
    directionData.codas.push_back(coda);

    Cursor cursor{1, 100};
    DirectionWriter writer{directionData, cursor};
    const auto mdcSet = writer.getDirectionLikeThings();
    REQUIRE(mdcSet.size() >= 1);
    CHECK(mdcSet.front().isDirection());
    const auto &direction = mdcSet.front().asDirection();

    DirectionReader reader{direction, cursor};
    const auto roundTripped = reader.getDirectionData();

    REQUIRE(roundTripped.segnos.size() == 1);
    REQUIRE(roundTripped.codas.size() == 1);
    CHECK(segno == roundTripped.segnos.front());
    CHECK(coda == roundTripped.codas.front());
}

T_END

#endif
