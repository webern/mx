// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/file/MxFileRepository.h"

#include <sstream>

using namespace std;
using namespace mx::api;
using namespace mxtest;

static constexpr const char *const kTimeSignatureApiTestFileName = "testAccidentals1.xml";

// puts the given time signature into a one-part score; extraStaves > 0 adds staves so that per-staff
// time signatures survive the reader's staff-count sanity check
static ScoreData timeSignatureApiTestScore(const TimeChoice &inTimeSignature, int extraStaves = 0)
{
    ScoreData score;
    score.parts.push_back(PartData{});
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.timeSignature = inTimeSignature;
    measure.staves.emplace_back(StaffData{});
    for (int i = 0; i < extraStaves; ++i)
    {
        measure.staves.emplace_back(StaffData{});
    }
    return score;
}

// serializes the score and reads it back through the api
static ScoreData timeSignatureApiTestRoundTrip(const ScoreData &inScore)
{
    auto &docMgr = DocumentManager::getInstance();
    const auto originalIdResult = docMgr.createFromScore(inScore);
    if (!originalIdResult.ok())
    {
        return ScoreData{};
    }
    const int originalId = originalIdResult.value();
    std::stringstream xml;
    docMgr.writeToStream(originalId, xml);
    docMgr.destroyDocument(originalId);
    std::istringstream iss{xml.str()};
    const auto reloadedIdResult = docMgr.createFromStream(iss);
    if (!reloadedIdResult.ok())
    {
        return ScoreData{};
    }
    const int reloadedId = reloadedIdResult.value();
    const auto reloadedScoreResult = docMgr.getData(reloadedId);
    docMgr.destroyDocument(reloadedId);
    if (!reloadedScoreResult.ok())
    {
        return ScoreData{};
    }
    return reloadedScoreResult.value();
}

TEST(implicitCarryover, TimeSignatureApi)
{
    const auto scoreData = mxtest::MxFileRepository::loadFile(kTimeSignatureApiTestFileName);
    CHECK_EQUAL(1, scoreData.parts.size());
    const auto &part = scoreData.parts.front();
    CHECK_EQUAL(2, part.measures.size())
    auto measureIter = part.measures.cbegin();
    auto t = measureIter->timeSignature;

    CHECK(!t.isImplicit);
    REQUIRE(t.isSimple());
    CHECK_EQUAL("3", t.simple().fraction.beats);
    CHECK_EQUAL("4", t.simple().fraction.beatType);
    CHECK(t.simple().symbol == TimeSignatureSymbol::unspecified);

    ++measureIter;
    t = measureIter->timeSignature;
    CHECK(t.isImplicit);
    REQUIRE(t.isSimple());
    CHECK_EQUAL("3", t.simple().fraction.beats);
    CHECK_EQUAL("4", t.simple().fraction.beatType);
}

T_END

TEST(defaultIsSimpleImplicitFourFour, TimeSignatureApi)
{
    TimeChoice t;
    CHECK(t.isImplicit);
    REQUIRE(t.isSimple());
    CHECK(t.kind() == TimeChoice::Kind::simple);
    CHECK_EQUAL("4", t.simple().fraction.beats);
    CHECK_EQUAL("4", t.simple().fraction.beatType);
    CHECK(t.simple().symbol == TimeSignatureSymbol::unspecified);
    CHECK(t.display == Bool::unspecified);
}

T_END

TEST(autoCollapseSimpleEquivalent, TimeSignatureApi)
{
    // a plain single-fraction meter with no decorations collapses to simple
    MeteredTimeSignature plain;
    plain.fractions = {{"3", "4"}};
    auto collapsed = TimeChoice(ComplexTimeSignature(plain));
    REQUIRE(collapsed.isSimple());
    CHECK_EQUAL("3", collapsed.simple().fraction.beats);

    // common/cut collapse too, mapping to the narrow simple symbol
    MeteredTimeSignature common;
    common.symbol = ComplexTimeSymbol::common;
    auto collapsedCommon = TimeChoice(ComplexTimeSignature(common));
    REQUIRE(collapsedCommon.isSimple());
    CHECK(collapsedCommon.simple().symbol == TimeSignatureSymbol::common);
}

T_END

TEST(complexStaysComplex, TimeSignatureApi)
{
    // more than one fraction -> genuinely composite, stays complex
    MeteredTimeSignature composite;
    composite.fractions = {{"2", "4"}, {"3", "8"}};
    CHECK(TimeChoice(ComplexTimeSignature(composite)).isComplex());

    // an unusual symbol (no simple equivalent) keeps it complex
    MeteredTimeSignature single;
    single.fractions = {{"3", "4"}};
    single.symbol = ComplexTimeSymbol::singleNumber;
    CHECK(TimeChoice(ComplexTimeSignature(single)).isComplex());

    // a separator keeps it complex
    MeteredTimeSignature separated;
    separated.fractions = {{"3", "4"}};
    separated.separator = TimeSeparator::diagonal;
    CHECK(TimeChoice(ComplexTimeSignature(separated)).isComplex());

    // an interchangeable alternate keeps it complex
    MeteredTimeSignature dual;
    dual.fractions = {{"3", "4"}};
    InterchangeableTimeSignature alt;
    alt.fractions = {{"6", "8"}};
    dual.interchangeable = alt;
    CHECK(TimeChoice(ComplexTimeSignature(dual)).isComplex());

    // senza-misura is always complex
    CHECK(TimeChoice(ComplexTimeSignature("X")).isComplex());
}

T_END

TEST(equality, TimeSignatureApi)
{
    TimeChoice a;
    TimeChoice b;
    CHECK(a == b);

    // isImplicit is part of equality
    b.isImplicit = false;
    CHECK(a != b);
    b.isImplicit = true;
    CHECK(a == b);

    // simple fraction difference
    a = TimeChoice(TimeSignatureData{"3", "4"});
    CHECK(a != b);
    b = TimeChoice(TimeSignatureData{"3", "4"});
    CHECK(a == b);

    // simple vs complex never compare equal
    MeteredTimeSignature composite;
    composite.fractions = {{"2", "4"}, {"3", "8"}};
    a = TimeChoice(ComplexTimeSignature(composite));
    CHECK(a != b);

    // nested composite second-fraction difference
    MeteredTimeSignature other;
    other.fractions = {{"2", "4"}, {"3", "16"}};
    b = TimeChoice(ComplexTimeSignature(other));
    CHECK(a != b);
    b = TimeChoice(ComplexTimeSignature(composite));
    CHECK(a == b);

    // interchangeable relation-only difference
    MeteredTimeSignature dualA;
    dualA.fractions = {{"3", "4"}};
    InterchangeableTimeSignature altA;
    altA.fractions = {{"6", "8"}};
    altA.relation = TimeRelation::parentheses;
    dualA.interchangeable = altA;
    a = TimeChoice(ComplexTimeSignature(dualA));

    MeteredTimeSignature dualB = dualA;
    dualB.interchangeable->relation = TimeRelation::bracket;
    b = TimeChoice(ComplexTimeSignature(dualB));
    CHECK(a != b);

    // senza-misura glyph difference
    a = TimeChoice(ComplexTimeSignature("X"));
    b = TimeChoice(ComplexTimeSignature(""));
    CHECK(a != b);
    b = TimeChoice(ComplexTimeSignature("X"));
    CHECK(a == b);
}

T_END

TEST(roundTripComposite, TimeSignatureApi)
{
    MeteredTimeSignature composite;
    composite.fractions = {{"2", "4"}, {"3", "8"}};
    auto t = TimeChoice(ComplexTimeSignature(composite));
    t.isImplicit = false;

    const auto reloaded = timeSignatureApiTestRoundTrip(timeSignatureApiTestScore(t));
    REQUIRE(!reloaded.parts.empty());
    const auto &actual = reloaded.parts.at(0).measures.at(0).timeSignature;
    CHECK(!actual.isImplicit);
    REQUIRE(actual.isComplex());
    REQUIRE(actual.complex().isMetered());
    const auto &fractions = actual.complex().metered().fractions;
    REQUIRE(2 == fractions.size());
    CHECK_EQUAL("2", fractions.at(0).beats);
    CHECK_EQUAL("4", fractions.at(0).beatType);
    CHECK_EQUAL("3", fractions.at(1).beats);
    CHECK_EQUAL("8", fractions.at(1).beatType);
}

T_END

TEST(roundTripSenzaMisuraWithGlyph, TimeSignatureApi)
{
    auto t = TimeChoice(ComplexTimeSignature("X"));
    t.isImplicit = false;

    const auto reloaded = timeSignatureApiTestRoundTrip(timeSignatureApiTestScore(t));
    REQUIRE(!reloaded.parts.empty());
    const auto &actual = reloaded.parts.at(0).measures.at(0).timeSignature;
    CHECK(!actual.isImplicit);
    REQUIRE(actual.isComplex());
    REQUIRE(actual.complex().isSenzaMisura());
    CHECK_EQUAL("X", actual.complex().senzaMisura());
}

T_END

TEST(roundTripSenzaMisuraWithoutGlyph, TimeSignatureApi)
{
    auto t = TimeChoice(ComplexTimeSignature(std::string{}));
    t.isImplicit = false;

    const auto reloaded = timeSignatureApiTestRoundTrip(timeSignatureApiTestScore(t));
    REQUIRE(!reloaded.parts.empty());
    const auto &actual = reloaded.parts.at(0).measures.at(0).timeSignature;
    CHECK(!actual.isImplicit);
    REQUIRE(actual.isComplex());
    REQUIRE(actual.complex().isSenzaMisura());
    CHECK_EQUAL("", actual.complex().senzaMisura());
}

T_END

TEST(roundTripInterchangeable, TimeSignatureApi)
{
    MeteredTimeSignature metered;
    metered.fractions = {{"3", "4"}};
    InterchangeableTimeSignature alt;
    alt.fractions = {{"6", "8"}, {"3", "8"}}; // a composite alternate
    alt.relation = TimeRelation::parentheses;
    alt.symbol = ComplexTimeSymbol::cut;
    alt.separator = TimeSeparator::horizontal;
    metered.interchangeable = alt;
    auto t = TimeChoice(ComplexTimeSignature(metered));
    t.isImplicit = false;

    const auto reloaded = timeSignatureApiTestRoundTrip(timeSignatureApiTestScore(t));
    REQUIRE(!reloaded.parts.empty());
    const auto &actual = reloaded.parts.at(0).measures.at(0).timeSignature;
    REQUIRE(actual.isComplex());
    REQUIRE(actual.complex().isMetered());
    const auto &actualMetered = actual.complex().metered();
    CHECK_EQUAL("3", actualMetered.fractions.at(0).beats);
    REQUIRE(actualMetered.interchangeable.has_value());
    CHECK(*actualMetered.interchangeable == alt);
}

T_END

TEST(roundTripWidenedSymbolAndSeparator, TimeSignatureApi)
{
    MeteredTimeSignature metered;
    metered.fractions = {{"3", "4"}};
    metered.symbol = ComplexTimeSymbol::note;
    metered.separator = TimeSeparator::diagonal;
    auto t = TimeChoice(ComplexTimeSignature(metered));
    t.isImplicit = false;

    const auto reloaded = timeSignatureApiTestRoundTrip(timeSignatureApiTestScore(t));
    REQUIRE(!reloaded.parts.empty());
    const auto &actual = reloaded.parts.at(0).measures.at(0).timeSignature;
    REQUIRE(actual.isComplex());
    REQUIRE(actual.complex().isMetered());
    CHECK(actual.complex().metered().symbol == ComplexTimeSymbol::note);
    CHECK(actual.complex().metered().separator == TimeSeparator::diagonal);
}

T_END

TEST(roundTripPerStaff, TimeSignatureApi)
{
    // staff 0 (unscoped default) in 4/4; staff 1 overridden to 3/8
    auto shared = TimeChoice(TimeSignatureData{});
    shared.isImplicit = false;

    auto score = timeSignatureApiTestScore(shared, 1); // two staves

    auto staffTime = TimeChoice(TimeSignatureData{"3", "8"});
    staffTime.isImplicit = false;
    score.parts.at(0).measures.at(0).staffTimeSignatures[1] = staffTime;

    // add a second measure so we can observe per-staff carry-forward
    score.parts.at(0).measures.emplace_back();
    auto &secondMeasure = score.parts.at(0).measures.back();
    secondMeasure.staves.emplace_back(StaffData{});
    secondMeasure.staves.emplace_back(StaffData{});

    const auto reloaded = timeSignatureApiTestRoundTrip(score);
    REQUIRE(!reloaded.parts.empty());
    REQUIRE(2 == reloaded.parts.at(0).measures.size());

    const auto &firstMeasure = reloaded.parts.at(0).measures.at(0);
    CHECK(!firstMeasure.timeSignature.isImplicit);
    CHECK_EQUAL("4", firstMeasure.timeSignature.simple().fraction.beats);
    REQUIRE(1 == firstMeasure.staffTimeSignatures.size());
    REQUIRE(firstMeasure.staffTimeSignatures.count(1) == 1);
    const auto &actualStaffTime = firstMeasure.staffTimeSignatures.at(1);
    CHECK(!actualStaffTime.isImplicit);
    REQUIRE(actualStaffTime.isSimple());
    CHECK_EQUAL("3", actualStaffTime.simple().fraction.beats);
    CHECK_EQUAL("8", actualStaffTime.simple().fraction.beatType);

    // the override carries forward, implicitly, into the next measure
    const auto &carried = reloaded.parts.at(0).measures.at(1);
    CHECK(carried.timeSignature.isImplicit);
    REQUIRE(1 == carried.staffTimeSignatures.size());
    REQUIRE(carried.staffTimeSignatures.count(1) == 1);
    CHECK(carried.staffTimeSignatures.at(1).isImplicit);
    CHECK_EQUAL("3", carried.staffTimeSignatures.at(1).simple().fraction.beats);
}

T_END

#endif
