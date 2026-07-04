// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Clef.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/StaffDetails.h"
#include "mx/impl/MeasureWriter.h"
#include "mx/impl/ScoreWriter.h"

#include <memory>

using namespace mx;
using namespace mx::impl;

// TODO - make real tests

namespace mxtest
{
struct TestItems
{
    std::unique_ptr<api::ScoreData> scoreData;
    std::unique_ptr<ScoreWriter> scoreWriter;
    api::MeasureData *measureData;
    std::unique_ptr<MeasureCursor> measureCursor;
    std::unique_ptr<MeasureWriter> measureWriter;
};

struct TestParameters
{
    int ticksPerQuarter;
    int measureIndex;
    int partIndex;
    int numStaves;
};

TestItems setupTestItems(TestParameters params)
{
    TestItems t;
    t.scoreData = std::make_unique<api::ScoreData>();
    t.scoreData->ticksPerQuarter = params.ticksPerQuarter;
    t.scoreData->parts.resize(params.partIndex + 1);
    int pindex = 0;
    for (auto &p : t.scoreData->parts)
    {
        p.measures.resize(params.measureIndex + 1);
        int mindex = 0;
        for (auto &m : p.measures)
        {
            m.staves.resize(params.numStaves);
            if (pindex == params.partIndex && mindex == params.measureIndex)
            {
                t.measureData = &m;
            }
            ++mindex;
        }
        ++pindex;
    }
    t.scoreWriter = std::make_unique<ScoreWriter>(*t.scoreData);
    t.measureCursor =
        std::make_unique<MeasureCursor>(static_cast<int>(t.measureData->staves.size()), params.ticksPerQuarter);
    t.measureCursor->partIndex = params.partIndex;
    t.measureCursor->measureIndex = params.measureIndex;
    t.measureWriter = std::make_unique<MeasureWriter>(*t.measureData, *t.measureCursor, *t.scoreWriter);
    return t;
}
} // namespace mxtest

TEST(measureNumber, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 100;
    params.measureIndex = 3;
    params.partIndex = 4;
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    const auto expected = std::string{"4"};
    const auto actual = partwiseMeasure.number();
    CHECK_EQUAL(expected, actual);
}

T_END

TEST(measureNumberString, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    t.measureData->number = "hello";
    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    const auto expected = std::string{"hello"};
    const auto actual = partwiseMeasure.number();
    CHECK_EQUAL(expected, actual);
}

T_END

TEST(leftBarlineFirstNoteRequiresForward, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    t.measureData->barlines.emplace_back(api::BarlineData{});
    auto &barline = t.measureData->barlines.front();
    barline.barlineType = api::BarlineType::heavyLight;
    auto &staff = t.measureData->staves.at(0);
    auto &voice = staff.voices[0];
    voice.notes.emplace_back(api::NoteData{});
    auto &note = voice.notes.back();
    note.tickTimePosition = 101;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();
    auto mdcIter = musicData.begin();
    auto mdcEnd = musicData.end();

    CHECK(mdcIter != mdcEnd);

    CHECK(mdcIter->isBarline());

    CHECK(++mdcIter != mdcEnd);
    CHECK(mdcIter->isAttributes());

    CHECK(++mdcIter != mdcEnd);
    CHECK(mdcIter->isForward());

    CHECK(++mdcIter != mdcEnd);
    CHECK(mdcIter->isNote());

    CHECK(++mdcIter == mdcEnd);
}

T_END

TEST(rightBarlineRepeatTimes, MeasureWriter)
{
    // A backward repeat carrying a times count must round-trip that count (#271).
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    t.measureData->barlines.emplace_back(api::BarlineData{});
    auto &barline = t.measureData->barlines.front();
    barline.barlineType = api::BarlineType::lightHeavy;
    barline.location = api::HorizontalAlignment::right;
    barline.tickTimePosition = api::TICK_TIME_INFINITY;
    barline.repeat = true;
    barline.repeatTimes = 5;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();

    const core::Barline *outBarline = nullptr;
    for (const auto &mdc : musicData)
    {
        if (mdc.isBarline())
        {
            outBarline = &mdc.asBarline();
        }
    }

    CHECK(outBarline != nullptr);
    CHECK(outBarline->repeat().has_value());
    CHECK(outBarline->repeat()->times().has_value());
    CHECK_EQUAL(5, *outBarline->repeat()->times());
}

T_END

TEST(PropertiesButNoNotes, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    t.measureData->barlines.emplace_back(api::BarlineData{});
    auto &barline = t.measureData->barlines.front();
    barline.barlineType = api::BarlineType::heavyLight;
    auto &staff = t.measureData->staves.at(0);
    staff.clefs.emplace_back(api::ClefData{});
    auto &clef = staff.clefs.back();
    clef.symbol = api::ClefSymbol::percussion;
    clef.line = 2;
    clef.tickTimePosition = 101;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();
    auto mdcIter = musicData.begin();
    auto mdcEnd = musicData.end();

    CHECK(mdcIter != mdcEnd);

    CHECK(mdcIter->isBarline());

    CHECK(++mdcIter != mdcEnd);
    CHECK(mdcIter->isAttributes());
    const auto &props = mdcIter->asAttributes();
    CHECK(props.divisions().has_value());
    CHECK_DOUBLES_EQUAL(101.0, props.divisions()->value().value(), 0.01);

    CHECK_EQUAL(1, props.clef().size());
    // Single-staff part (numStaves == 1): the clef carries no number; 1 is assumed. The
    // mid-measure write path previously emitted a spurious number="1" (#230).
    CHECK(!props.clef().back().number().has_value());

    CHECK(++mdcIter == mdcEnd);
}

T_END

TEST(MultiStaffClefKeepsNumber, MeasureWriter)
{
    // Counterpart to PropertiesButNoNotes: in a multi-staff part the clef number is required
    // to disambiguate the staff, so it must be emitted (here staff 0 -> number 1).
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    t.measureData->barlines.emplace_back(api::BarlineData{});
    auto &barline = t.measureData->barlines.front();
    barline.barlineType = api::BarlineType::heavyLight;
    auto &staff = t.measureData->staves.at(0);
    staff.clefs.emplace_back(api::ClefData{});
    auto &clef = staff.clefs.back();
    clef.symbol = api::ClefSymbol::percussion;
    clef.line = 2;
    clef.tickTimePosition = 101;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();
    auto mdcIter = musicData.begin();
    auto mdcEnd = musicData.end();

    CHECK(mdcIter != mdcEnd);
    CHECK(mdcIter->isBarline());

    CHECK(++mdcIter != mdcEnd);
    CHECK(mdcIter->isAttributes());
    const auto &props = mdcIter->asAttributes();

    CHECK_EQUAL(1, props.clef().size());
    CHECK(props.clef().back().number().has_value());
    CHECK(1 == props.clef().back().number()->value());
}

T_END

TEST(staffDetailsWritesStaffLines, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    auto &staff = t.measureData->staves.at(0);
    staff.staffLines = 1;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();
    auto mdcIter = musicData.begin();
    const auto mdcEnd = musicData.end();

    CHECK(mdcIter != mdcEnd);
    CHECK(mdcIter->isAttributes());

    const auto &props = mdcIter->asAttributes();
    CHECK_EQUAL(1, props.staffDetails().size());

    const auto &details = props.staffDetails().front();
    CHECK(details.group().has_value());
    CHECK_EQUAL(1, details.group()->staffLines());
    CHECK(!details.number().has_value());
}

T_END

TEST(staffDetailsWritesStaffSize, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    auto &staff = t.measureData->staves.at(0);
    staff.staffSize = 80.5;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();
    auto mdcIter = musicData.begin();
    const auto mdcEnd = musicData.end();

    CHECK(mdcIter != mdcEnd);
    CHECK(mdcIter->isAttributes());

    const auto &props = mdcIter->asAttributes();
    CHECK_EQUAL(1, props.staffDetails().size());

    const auto &details = props.staffDetails().front();
    CHECK(!details.group().has_value());
    CHECK(details.staffSize().has_value());
    CHECK(80.5 == details.staffSize()->value().value().value());
    CHECK(!details.staffSize()->scaling().has_value());
    CHECK(!details.number().has_value());
}

T_END

TEST(staffDetailsWritesStaffSizeScaling, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    auto &staff = t.measureData->staves.at(0);
    staff.staffSize = 80.5;
    staff.staffScaling = 75.5;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();
    auto mdcIter = musicData.begin();
    const auto mdcEnd = musicData.end();

    CHECK(mdcIter != mdcEnd);
    CHECK(mdcIter->isAttributes());

    const auto &props = mdcIter->asAttributes();
    CHECK_EQUAL(1, props.staffDetails().size());

    const auto &details = props.staffDetails().front();
    CHECK(!details.group().has_value());
    CHECK(details.staffSize().has_value());
    CHECK(80.5 == details.staffSize()->value().value().value());
    CHECK(details.staffSize()->scaling().has_value());
    CHECK(75.5 == details.staffSize()->scaling()->value().value());
    CHECK(!details.number().has_value());
}

T_END

TEST(staffDetailsWritesStaffLinesAndStaffSize, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    auto &staff = t.measureData->staves.at(0);
    staff.staffLines = 1;
    staff.staffSize = 80.5;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();
    auto mdcIter = musicData.begin();
    const auto mdcEnd = musicData.end();

    CHECK(mdcIter != mdcEnd);
    CHECK(mdcIter->isAttributes());

    const auto &props = mdcIter->asAttributes();
    CHECK_EQUAL(1, props.staffDetails().size());

    const auto &details = props.staffDetails().front();
    CHECK(details.group().has_value());
    CHECK_EQUAL(1, details.group()->staffLines());
    CHECK(details.staffSize().has_value());
    CHECK(80.5 == details.staffSize()->value().value().value());
    CHECK(!details.staffSize()->scaling().has_value());
    CHECK(!details.number().has_value());
}

T_END

#endif
