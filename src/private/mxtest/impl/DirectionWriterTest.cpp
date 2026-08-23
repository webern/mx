// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/OttavaData.h"
#include "mx/api/RehearsalData.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/DirectionType.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/OctaveShift.h"
#include "mx/impl/DirectionReader.h"
#include "mx/impl/DirectionWriter.h"
#include "mx/impl/SpannerResolver.h"

#include <memory>

using namespace mx;
using namespace mx::impl;

TEST(ottavaStartStop, DirectionWriter)
{
    Cursor cursor{1, 100};
    cursor.isFirstMeasureInPart = false;
    api::DirectionData directionData;

    // No matching start is visible to the resolver, so this stop takes the documented dangling
    // fallback and writes MusicXML's default size of 8.
    api::OttavaStop stop{};
    stop.spannerStop.number = api::SpannerNumber(2);
    directionData.directionTypes.emplace_back(api::DirectionChoice{stop});

    api::OttavaStart start{};
    start.ottavaType = api::OttavaType::o15mb;
    start.spannerStart.positionData.isDefaultXSpecified = true;
    start.spannerStart.positionData.defaultX = 100.0;
    start.spannerStart.positionData.isDefaultYSpecified = true;
    start.spannerStart.positionData.defaultY = 20.0;
    start.spannerStart.positionData.isRelativeXSpecified = true;
    start.spannerStart.positionData.relativeX = 3.0;
    start.spannerStart.positionData.isRelativeYSpecified = true;
    start.spannerStart.positionData.relativeY = 4.0;
    start.spannerStart.printData.fontData.fontFamily = {"Maestro"};
    start.spannerStart.printData.fontData.style = api::FontStyle::italic;
    start.spannerStart.printData.fontData.weight = api::FontWeight::bold;
    start.spannerStart.printData.fontData.sizeType = api::FontSizeType::point;
    start.spannerStart.printData.fontData.sizePoint = 18.0;
    start.spannerStart.printData.isColorSpecified = true;
    start.spannerStart.printData.color.red = 12;
    start.spannerStart.printData.color.green = 34;
    start.spannerStart.printData.color.blue = 56;
    directionData.directionTypes.emplace_back(api::DirectionChoice{start});

    SpannerResolver spannerResolver;
    DirectionWriter writer{directionData, cursor, spannerResolver};
    const auto mdcSet = writer.getDirectionLikeThings();
    CHECK(mdcSet.front().isDirection());
    const auto &direction = mdcSet.front().asDirection();
    const auto &directionTypes = direction.directionType();

    CHECK_EQUAL(2, directionTypes.size());

    DirectionReader reader{direction, cursor};
    const auto roundTripped = reader.getDirectionData();
    REQUIRE(roundTripped.directionTypes.size() == 2);
    REQUIRE(roundTripped.directionTypes.front().isOttavaStop());
    const auto roundTrippedStop = roundTripped.directionTypes.front().ottavaStop();
    CHECK(api::SpannerNumber(2) == roundTrippedStop.spannerStop.number);
    CHECK(api::Bool::yes == roundTrippedStop.writeSize);
    const auto &stopCore = directionTypes.front().choice().asOctaveShift();
    REQUIRE(stopCore.size().has_value());
    CHECK_EQUAL(8, *stopCore.size());
    REQUIRE(roundTripped.directionTypes.back().isOttavaStart());
    CHECK(start == roundTripped.directionTypes.back().ottavaStart());
}

T_END

TEST(ottava22maAnd22mb, DirectionWriter)
{
    api::DirectionData directionData;

    api::OttavaStart up{};
    up.ottavaType = api::OttavaType::o22ma;
    directionData.directionTypes.emplace_back(api::DirectionChoice{up});

    api::OttavaStart down{};
    down.ottavaType = api::OttavaType::o22mb;
    directionData.directionTypes.emplace_back(api::DirectionChoice{down});

    Cursor cursor{1, 100};
    SpannerResolver spannerResolver;
    DirectionWriter writer{directionData, cursor, spannerResolver};
    const auto mdcSet = writer.getDirectionLikeThings();

    REQUIRE(mdcSet.size() == 1);
    REQUIRE(mdcSet.front().isDirection());
    const auto &directionTypes = mdcSet.front().asDirection().directionType();
    REQUIRE(directionTypes.size() == 2);

    const auto &upCore = directionTypes.front().choice().asOctaveShift();
    REQUIRE(upCore.size().has_value());
    CHECK_EQUAL(22, *upCore.size());
    CHECK(upCore.type().tag() == core::UpDownStopContinue::Tag::down);

    const auto &downCore = directionTypes.back().choice().asOctaveShift();
    REQUIRE(downCore.size().has_value());
    CHECK_EQUAL(22, *downCore.size());
    CHECK(downCore.type().tag() == core::UpDownStopContinue::Tag::up);
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
    segno.positionData.horizontalAlignment = api::HorizontalAlignment::left;
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
    segno.id = "id3";

    api::CodaData coda;
    coda.positionData.isDefaultXSpecified = true;
    coda.positionData.defaultX = 5.0;
    coda.positionData.horizontalAlignment = api::HorizontalAlignment::center;
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
    coda.id = "id7";

    api::DirectionData directionData;
    directionData.directionTypes.emplace_back(api::DirectionChoice{segno});
    directionData.directionTypes.emplace_back(api::DirectionChoice{coda});

    Cursor cursor{1, 100};
    SpannerResolver spannerResolver;
    DirectionWriter writer{directionData, cursor, spannerResolver};
    const auto mdcSet = writer.getDirectionLikeThings();
    REQUIRE(mdcSet.size() >= 1);
    CHECK(mdcSet.front().isDirection());
    const auto &direction = mdcSet.front().asDirection();

    DirectionReader reader{direction, cursor};
    const auto roundTripped = reader.getDirectionData();

    REQUIRE(roundTripped.directionTypes.size() == 2);
    REQUIRE(roundTripped.directionTypes.front().isSegno());
    REQUIRE(roundTripped.directionTypes.back().isCoda());
    CHECK(segno == roundTripped.directionTypes.front().segno());
    CHECK(coda == roundTripped.directionTypes.back().coda());
}

T_END

// Build a RehearsalData carrying the full attribute set (text, position, color, font, enclosure),
// write it with DirectionWriter, read it back with DirectionReader, and confirm every field
// survives the api -> core -> api round trip.
TEST(rehearsalRoundTrip, DirectionWriter)
{
    api::RehearsalData rehearsal;
    rehearsal.text = "A";
    rehearsal.positionData.isDefaultXSpecified = true;
    rehearsal.positionData.defaultX = 10.0;
    rehearsal.positionData.isDefaultYSpecified = true;
    rehearsal.positionData.defaultY = 20.0;
    rehearsal.positionData.isRelativeXSpecified = true;
    rehearsal.positionData.relativeX = 3.0;
    rehearsal.positionData.isRelativeYSpecified = true;
    rehearsal.positionData.relativeY = 4.0;
    rehearsal.positionData.horizontalAlignment = api::HorizontalAlignment::left;
    rehearsal.positionData.verticalAlignment = api::VerticalAlignment::top;
    rehearsal.fontData.fontFamily = {"Maestro"};
    rehearsal.fontData.style = api::FontStyle::italic;
    rehearsal.fontData.weight = api::FontWeight::bold;
    rehearsal.fontData.sizeType = api::FontSizeType::point;
    rehearsal.fontData.sizePoint = 14.0;
    rehearsal.isColorSpecified = true;
    rehearsal.colorData.red = 255;
    rehearsal.colorData.green = 0;
    rehearsal.colorData.blue = 0;
    rehearsal.colorData.isAlphaSpecified = true;
    rehearsal.colorData.alpha = 255;
    rehearsal.enclosure = api::Enclosure::square;

    api::DirectionData directionData;
    directionData.directionTypes.emplace_back(api::DirectionChoice{rehearsal});

    Cursor cursor{1, 100};
    SpannerResolver spannerResolver;
    DirectionWriter writer{directionData, cursor, spannerResolver};
    const auto mdcSet = writer.getDirectionLikeThings();
    REQUIRE(mdcSet.size() >= 1);
    CHECK(mdcSet.front().isDirection());
    const auto &direction = mdcSet.front().asDirection();

    DirectionReader reader{direction, cursor};
    const auto roundTripped = reader.getDirectionData();

    REQUIRE(roundTripped.directionTypes.size() == 1);
    REQUIRE(roundTripped.directionTypes.front().isRehearsal());
    CHECK(rehearsal == roundTripped.directionTypes.front().rehearsal());
}

T_END

// <direction directive="yes"> aligns the direction with the measure's time signature. The
// attribute is tri-state: unspecified does not write anything, and yes/no are written verbatim.
TEST(directiveRoundTrip, DirectionWriter)
{
    api::RehearsalData rehearsal;
    rehearsal.text = "A";

    api::DirectionData directionData;
    directionData.directionTypes.emplace_back(api::DirectionChoice{rehearsal});
    directionData.directive = api::Bool::yes;

    Cursor cursor{1, 100};
    SpannerResolver spannerResolver;
    DirectionWriter writer{directionData, cursor, spannerResolver};
    const auto mdcSet = writer.getDirectionLikeThings();
    REQUIRE(mdcSet.size() >= 1);
    REQUIRE(mdcSet.front().isDirection());
    const auto &direction = mdcSet.front().asDirection();
    REQUIRE(direction.directive().has_value());
    CHECK(core::YesNo::Tag::yes == direction.directive()->tag());

    DirectionReader reader{direction, cursor};
    CHECK(api::Bool::yes == reader.getDirectionData().directive);
}

T_END

// An unspecified directive leaves the attribute off the written <direction> entirely, so a
// document that never mentioned it does not gain one.
TEST(directiveUnspecifiedWritesNoAttribute, DirectionWriter)
{
    api::RehearsalData rehearsal;
    rehearsal.text = "A";

    api::DirectionData directionData;
    directionData.directionTypes.emplace_back(api::DirectionChoice{rehearsal});

    Cursor cursor{1, 100};
    SpannerResolver spannerResolver;
    DirectionWriter writer{directionData, cursor, spannerResolver};
    const auto mdcSet = writer.getDirectionLikeThings();
    REQUIRE(mdcSet.size() >= 1);
    REQUIRE(mdcSet.front().isDirection());
    const auto &direction = mdcSet.front().asDirection();
    CHECK(!direction.directive().has_value());

    DirectionReader reader{direction, cursor};
    CHECK(api::Bool::unspecified == reader.getDirectionData().directive);
}

T_END

#endif
