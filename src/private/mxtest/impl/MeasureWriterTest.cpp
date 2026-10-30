// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DirectionData.h"
#include "mx/api/FiguredBassData.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Clef.h"
#include "mx/core/generated/FullNoteGroup.h"
#include "mx/core/generated/GraceNormalNoteGroup.h"
#include "mx/core/generated/GraceNoteChoice.h"
#include "mx/core/generated/GraceNoteGroup.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/NormalNoteGroup.h"
#include "mx/core/generated/Note.h"
#include "mx/core/generated/NoteChoice.h"
#include "mx/core/generated/StaffDetails.h"
#include "mx/impl/MeasureWriter.h"
#include "mx/impl/ScoreWriter.h"

#include <memory>
#include <string>
#include <vector>

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

// Letters for the kinds of music data in a written measure, in order:
// P print, A attributes, N note, B barline, K backup, F forward, D direction, G figured-bass.
std::string musicDataOrder(const core::PartwiseMeasure &measure)
{
    std::string result;
    for (const auto &mdc : measure.musicData())
    {
        if (mdc.isPrint())
        {
            result += 'P';
        }
        else if (mdc.isAttributes())
        {
            result += 'A';
        }
        else if (mdc.isNote())
        {
            result += 'N';
        }
        else if (mdc.isBarline())
        {
            result += 'B';
        }
        else if (mdc.isBackup())
        {
            result += 'K';
        }
        else if (mdc.isForward())
        {
            result += 'F';
        }
        else if (mdc.isDirection())
        {
            result += 'D';
        }
        else if (mdc.isFiguredBass())
        {
            result += 'G';
        }
        else
        {
            result += '?';
        }
    }
    return result;
}

void addQuarterNote(api::MeasureData &measure, int voiceIndex, int tickTimePosition)
{
    api::NoteData note;
    note.tickTimePosition = tickTimePosition;
    note.durationData.durationTimeTicks = 4;
    measure.staves.at(0).voices[voiceIndex].notes.emplace_back(note);
}

// Adds a quarter note to voice 0 of the given staff.
void addStaffQuarterNote(api::MeasureData &measure, int staffIndex, int tickTimePosition)
{
    api::NoteData note;
    note.tickTimePosition = tickTimePosition;
    note.durationData.durationTimeTicks = 4;
    measure.staves.at(staffIndex).voices[0].notes.emplace_back(note);
}

api::DirectionData makeDynamicDirection(int tickTimePosition)
{
    api::DirectionData direction;
    direction.tickTimePosition = tickTimePosition;
    direction.isStaffValueSpecified = true;
    direction.directionTypes.emplace_back(api::DirectionChoice{api::MarkData{api::StandardDynamic::f}});
    return direction;
}

api::DirectionData makeFiguredBassDirection(int tickTimePosition)
{
    api::DirectionData direction;
    direction.tickTimePosition = tickTimePosition;
    direction.isStaffValueSpecified = true;
    api::FigureData figure;
    figure.figureNumber = "6";
    api::FiguredBassData figuredBass;
    figuredBass.figures.emplace_back(figure);
    direction.figuredBasses.emplace_back(figuredBass);
    return direction;
}

api::BarlineData makeBarline(api::HorizontalAlignment location, int tickTimePosition, api::BarlineType type)
{
    api::BarlineData barline;
    barline.location = location;
    barline.tickTimePosition = tickTimePosition;
    barline.barlineType = type;
    return barline;
}

TestParameters oneStaffParameters()
{
    TestParameters params;
    params.ticksPerQuarter = 4;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    return params;
}

const core::Barline &barlineAt(const core::PartwiseMeasure &measure, int barlineOrdinal)
{
    int count = 0;
    for (const auto &mdc : measure.musicData())
    {
        if (mdc.isBarline())
        {
            if (count == barlineOrdinal)
            {
                return mdc.asBarline();
            }
            ++count;
        }
    }
    static const core::Barline none{};
    return none;
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

TEST(ZeroTicksPerQuarterWritesNoDivisions, MeasureWriter)
{
    // A score with no time resolution (ticksPerQuarter == 0, e.g. a source that declared no
    // <divisions> and has no durations) must not have a bogus <divisions> invented for it.
    mxtest::TestParameters params;
    params.ticksPerQuarter = 0;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    CHECK(partwiseMeasure.musicData().empty());
}

T_END

TEST(ZeroTicksPerQuarterKeepsOtherProperties, MeasureWriter)
{
    // With ticksPerQuarter == 0 the <attributes> element is still written when it has real
    // content (here a clef); only the <divisions> child is suppressed.
    mxtest::TestParameters params;
    params.ticksPerQuarter = 0;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    auto &staff = t.measureData->staves.at(0);
    staff.clefs.emplace_back(api::ClefData{});
    auto &clef = staff.clefs.back();
    clef.symbol = api::ClefSymbol::g;
    clef.line = 2;

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto musicData = partwiseMeasure.musicData();
    auto mdcIter = musicData.begin();
    auto mdcEnd = musicData.end();

    CHECK(mdcIter != mdcEnd);
    CHECK(mdcIter->isAttributes());
    const auto &props = mdcIter->asAttributes();
    CHECK(!props.divisions().has_value());
    CHECK_EQUAL(1, props.clef().size());

    CHECK(++mdcIter == mdcEnd);
}

T_END

TEST(GraceChordThenChordAtSameTick, MeasureWriter)
{
    // Grace notes occupy no ticks, so a grace-note chord shares its tick position with the
    // chord it ornaments. The first note of the main chord must still start a new chord (no
    // <chord/> tag), not be absorbed into the grace chord (#345).
    mxtest::TestParameters params;
    params.ticksPerQuarter = 4;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    auto &staff = t.measureData->staves.at(0);
    auto &voice = staff.voices[0];

    auto makeNote = [](int step, bool isGrace) {
        api::NoteData note{};
        note.isChord = true;
        note.isGrace = isGrace;
        note.tickTimePosition = 0;
        note.pitchData.step = static_cast<api::Step>(step);
        note.pitchData.octave = 4;
        note.durationData.durationName = api::DurationName::quarter;
        note.durationData.durationTimeTicks = isGrace ? 0 : 4;
        return note;
    };

    voice.notes.push_back(makeNote(0, true));
    voice.notes.push_back(makeNote(2, true));
    voice.notes.push_back(makeNote(4, false));
    voice.notes.push_back(makeNote(6, false));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    std::vector<bool> chordTags;
    for (const auto &mdc : partwiseMeasure.musicData())
    {
        if (!mdc.isNote())
        {
            continue;
        }
        const auto &choice = mdc.asNote().choice();
        if (choice.isGraceNoteGroup())
        {
            chordTags.push_back(
                choice.asGraceNoteGroup().graceNoteChoice().asGraceNormalNoteGroup().fullNote().chord());
        }
        else if (choice.isNormalNoteGroup())
        {
            chordTags.push_back(choice.asNormalNoteGroup().fullNote().chord());
        }
    }

    CHECK_EQUAL(4, static_cast<int>(chordTags.size()));
    CHECK(!chordTags.at(0)); // first grace note starts the grace chord
    CHECK(chordTags.at(1));  // second grace note carries <chord/>
    CHECK(!chordTags.at(2)); // first main-chord note must NOT carry <chord/>
    CHECK(chordTags.at(3));  // second main-chord note carries <chord/>
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

TEST(staffDetailsWritesStaffTuning, MeasureWriter)
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    auto &staff = t.measureData->staves.at(0);
    mx::api::StaffTuningData tuning;
    tuning.line = 3;
    tuning.tuningStep = mx::api::Step::d;
    tuning.tuningOctave = 3;
    staff.staffTunings.push_back(tuning);

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
    REQUIRE(1 == details.staffTuning().size());
    CHECK_EQUAL(3, details.staffTuning().front().line().value());
    CHECK(mx::core::Step::Tag::d == details.staffTuning().front().tuning().tuningStep().tag());
    CHECK_EQUAL(3, details.staffTuning().front().tuning().tuningOctave().value());
    CHECK(!details.staffTuning().front().tuning().tuningAlter().has_value());
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

TEST(leftBarlineFollowsPrint, MeasureWriter)
{
    // A left barline comes after the measure's <print>.
    mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
    t.measureData->measureNumbering = api::MeasureNumbering::system;
    t.measureData->barlines.emplace_back(
        mxtest::makeBarline(api::HorizontalAlignment::left, 0, api::BarlineType::heavyLight));
    mxtest::addQuarterNote(*t.measureData, 0, 0);

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    CHECK_EQUAL(std::string{"PBAN"}, mxtest::musicDataOrder(partwiseMeasure));
}

T_END

TEST(leftBarlineWithoutPrintComesFirst, MeasureWriter)
{
    mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
    t.measureData->barlines.emplace_back(
        mxtest::makeBarline(api::HorizontalAlignment::left, 0, api::BarlineType::heavyLight));
    mxtest::addQuarterNote(*t.measureData, 0, 0);

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    CHECK_EQUAL(std::string{"BAN"}, mxtest::musicDataOrder(partwiseMeasure));
}

T_END

TEST(middleBarlineWrittenAtItsTick, MeasureWriter)
{
    mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
    for (int tick = 0; tick < 16; tick += 4)
    {
        mxtest::addQuarterNote(*t.measureData, 0, tick);
    }
    t.measureData->barlines.emplace_back(
        mxtest::makeBarline(api::HorizontalAlignment::center, 8, api::BarlineType::dotted));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    CHECK_EQUAL(std::string{"ANNBNN"}, mxtest::musicDataOrder(partwiseMeasure));

    const auto &barline = mxtest::barlineAt(partwiseMeasure, 0);
    REQUIRE(barline.location().has_value());
    CHECK(barline.location()->tag() == core::RightLeftMiddle::Tag::middle);
    REQUIRE(barline.barStyle().has_value());
    CHECK(barline.barStyle()->value().tag() == core::BarStyle::Tag::dotted);
}

T_END

TEST(middleBarlineAfterForward, MeasureWriter)
{
    // The note at tick 12 follows a <forward> of 8. A barline at the start of the gap comes
    // before the <forward>. A barline at the end of the gap comes after it.
    {
        mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
        mxtest::addQuarterNote(*t.measureData, 0, 0);
        mxtest::addQuarterNote(*t.measureData, 0, 12);
        t.measureData->barlines.emplace_back(
            mxtest::makeBarline(api::HorizontalAlignment::center, 12, api::BarlineType::dotted));

        const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
        CHECK_EQUAL(std::string{"ANFBN"}, mxtest::musicDataOrder(partwiseMeasure));
    }
    {
        mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
        mxtest::addQuarterNote(*t.measureData, 0, 0);
        mxtest::addQuarterNote(*t.measureData, 0, 12);
        t.measureData->barlines.emplace_back(
            mxtest::makeBarline(api::HorizontalAlignment::center, 4, api::BarlineType::dotted));

        const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
        CHECK_EQUAL(std::string{"ANBFN"}, mxtest::musicDataOrder(partwiseMeasure));
    }
}

T_END

TEST(middleBarlineBeforeBackupAndRightBarlineLast, MeasureWriter)
{
    // The middle barline is at the end of voice 0, so it is written before the <backup> into
    // voice 1. The right barline stays at the end of the measure. A middle barline authored at
    // exactly the end of a multi-voice measure is written the same way, after the first voice.
    mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
    mxtest::addQuarterNote(*t.measureData, 0, 0);
    mxtest::addQuarterNote(*t.measureData, 0, 4);
    mxtest::addQuarterNote(*t.measureData, 1, 0);
    t.measureData->barlines.emplace_back(
        mxtest::makeBarline(api::HorizontalAlignment::center, 8, api::BarlineType::dotted));
    t.measureData->barlines.emplace_back(
        mxtest::makeBarline(api::HorizontalAlignment::right, api::TICK_TIME_INFINITY, api::BarlineType::lightHeavy));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    REQUIRE(std::string{"ANNBKNB"} == mxtest::musicDataOrder(partwiseMeasure));
    CHECK(mxtest::barlineAt(partwiseMeasure, 0).location()->tag() == core::RightLeftMiddle::Tag::middle);
    CHECK(mxtest::barlineAt(partwiseMeasure, 1).location()->tag() == core::RightLeftMiddle::Tag::right);
}

T_END

TEST(middleBarlineNotReachedIsWrittenOnceAtEnd, MeasureWriter)
{
    // No note starts at or after the barline's tick, so it is written once, at the end.
    {
        mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
        for (int tick = 0; tick < 16; tick += 4)
        {
            mxtest::addQuarterNote(*t.measureData, 0, tick);
        }
        t.measureData->barlines.emplace_back(
            mxtest::makeBarline(api::HorizontalAlignment::center, 16, api::BarlineType::dotted));

        const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
        REQUIRE(std::string{"ANNNNB"} == mxtest::musicDataOrder(partwiseMeasure));
        CHECK(mxtest::barlineAt(partwiseMeasure, 0).location()->tag() == core::RightLeftMiddle::Tag::middle);
    }
    {
        mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
        t.measureData->barlines.emplace_back(
            mxtest::makeBarline(api::HorizontalAlignment::center, 8, api::BarlineType::dotted));

        const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
        CHECK_EQUAL(std::string{"AB"}, mxtest::musicDataOrder(partwiseMeasure));
    }
}

T_END

TEST(unspecifiedLocationBarlineStaysLast, MeasureWriter)
{
    // Without a location the writer does not guess "middle" from the tick. The barline goes at
    // the end of the measure and is a right barline.
    mxtest::TestItems t = mxtest::setupTestItems(mxtest::oneStaffParameters());
    for (int tick = 0; tick < 16; tick += 4)
    {
        mxtest::addQuarterNote(*t.measureData, 0, tick);
    }
    t.measureData->barlines.emplace_back(
        mxtest::makeBarline(api::HorizontalAlignment::unspecified, 8, api::BarlineType::dotted));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    REQUIRE(std::string{"ANNNNB"} == mxtest::musicDataOrder(partwiseMeasure));
    CHECK(mxtest::barlineAt(partwiseMeasure, 0).location()->tag() == core::RightLeftMiddle::Tag::right);
}

T_END

TEST(leadingDirectionFollowsBackup, MeasureWriter)
{
    // The second staff starts with a dynamic. It is written after the <backup> to the start of
    // the measure, with no <offset> to reach there from the end of the first staff.
    mxtest::TestParameters params = mxtest::oneStaffParameters();
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    for (int staff = 0; staff < 2; ++staff)
    {
        mxtest::addStaffQuarterNote(*t.measureData, staff, 0);
        mxtest::addStaffQuarterNote(*t.measureData, staff, 4);
    }
    t.measureData->staves.at(1).directions.emplace_back(mxtest::makeDynamicDirection(0));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    REQUIRE(std::string{"ANNKDNN"} == mxtest::musicDataOrder(partwiseMeasure));
    CHECK(!partwiseMeasure.musicData()[4].asDirection().offset().has_value());
}

T_END

TEST(leadingDirectionFollowsBackupSingleNoteStaff, MeasureWriter)
{
    // The second staff has a single note, which takes a different path through writeDirections.
    mxtest::TestParameters params = mxtest::oneStaffParameters();
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    mxtest::addStaffQuarterNote(*t.measureData, 0, 0);
    mxtest::addStaffQuarterNote(*t.measureData, 1, 0);
    t.measureData->staves.at(1).directions.emplace_back(mxtest::makeDynamicDirection(0));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    REQUIRE(std::string{"ANKDN"} == mxtest::musicDataOrder(partwiseMeasure));
    CHECK(!partwiseMeasure.musicData()[3].asDirection().offset().has_value());
}

T_END

TEST(leadingFiguredBassFollowsBackup, MeasureWriter)
{
    // A figured-bass is held as a direction, so it follows the same <backup>.
    mxtest::TestParameters params = mxtest::oneStaffParameters();
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    mxtest::addStaffQuarterNote(*t.measureData, 0, 0);
    mxtest::addStaffQuarterNote(*t.measureData, 1, 0);
    t.measureData->staves.at(1).directions.emplace_back(mxtest::makeFiguredBassDirection(0));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    CHECK_EQUAL(std::string{"ANKGN"}, mxtest::musicDataOrder(partwiseMeasure));
}

T_END

TEST(leadingDirectionBeforeLaterFirstNote, MeasureWriter)
{
    // The second staff rests for a beat before its first note. The dynamic is at the start of the
    // measure, so the <backup> goes all the way back to it and a <forward> follows.
    mxtest::TestParameters params = mxtest::oneStaffParameters();
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    mxtest::addStaffQuarterNote(*t.measureData, 0, 0);
    mxtest::addStaffQuarterNote(*t.measureData, 0, 4);
    mxtest::addStaffQuarterNote(*t.measureData, 1, 4);
    t.measureData->staves.at(1).directions.emplace_back(mxtest::makeDynamicDirection(0));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    REQUIRE(std::string{"ANNKDFN"} == mxtest::musicDataOrder(partwiseMeasure));
    CHECK(!partwiseMeasure.musicData()[4].asDirection().offset().has_value());
}

T_END

TEST(directionAtPreviousStaffEndStaysBeforeBackup, MeasureWriter)
{
    // A direction at the tick where the first staff ended may belong to the end of that staff, so
    // it is written there, ahead of the <backup>, with no <offset>.
    mxtest::TestParameters params = mxtest::oneStaffParameters();
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    mxtest::addStaffQuarterNote(*t.measureData, 0, 0);
    mxtest::addStaffQuarterNote(*t.measureData, 0, 4);
    mxtest::addStaffQuarterNote(*t.measureData, 1, 0);
    t.measureData->staves.at(1).directions.emplace_back(mxtest::makeDynamicDirection(8));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    REQUIRE(std::string{"ANNDKN"} == mxtest::musicDataOrder(partwiseMeasure));
    CHECK(!partwiseMeasure.musicData()[3].asDirection().offset().has_value());
}

T_END

TEST(directionAfterFirstNoteFollowsItsNote, MeasureWriter)
{
    // The dynamic is on the second note of the second staff. The <backup> goes to the first note,
    // and the dynamic is written between the two notes.
    mxtest::TestParameters params = mxtest::oneStaffParameters();
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    for (int staff = 0; staff < 2; ++staff)
    {
        mxtest::addStaffQuarterNote(*t.measureData, staff, 0);
        mxtest::addStaffQuarterNote(*t.measureData, staff, 4);
    }
    t.measureData->staves.at(1).directions.emplace_back(mxtest::makeDynamicDirection(4));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    REQUIRE(std::string{"ANNKNDN"} == mxtest::musicDataOrder(partwiseMeasure));
    CHECK(!partwiseMeasure.musicData()[5].asDirection().offset().has_value());
}

T_END

TEST(middleBarlineAtStaffEndStaysBeforeBackup, MeasureWriter)
{
    // The middle barline is at the end of the first staff. It is written before the <backup> to
    // the second staff's leading direction.
    mxtest::TestParameters params = mxtest::oneStaffParameters();
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems(params);
    for (int staff = 0; staff < 2; ++staff)
    {
        mxtest::addStaffQuarterNote(*t.measureData, staff, 0);
        mxtest::addStaffQuarterNote(*t.measureData, staff, 4);
    }
    t.measureData->staves.at(1).directions.emplace_back(mxtest::makeDynamicDirection(0));
    t.measureData->barlines.emplace_back(
        mxtest::makeBarline(api::HorizontalAlignment::center, 8, api::BarlineType::dotted));

    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    CHECK_EQUAL(std::string{"ANNBKDNN"}, mxtest::musicDataOrder(partwiseMeasure));
}

T_END

#endif
