// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/generated/Bass.h"
#include "mx/core/generated/BassStep.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/Harmony.h"
#include "mx/core/generated/HarmonyChordGroup.h"
#include "mx/core/generated/HarmonyChordGroupChoice.h"
#include "mx/core/generated/Kind.h"
#include "mx/core/generated/KindValue.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/Offset.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PartwisePart.h"
#include "mx/core/generated/Root.h"
#include "mx/core/generated/RootStep.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/core/generated/Step.h"
#include "mxtest/api/ApiChordSimpleScoreData.h"
#include "mxtest/api/TestHelpers.h"
#include "mxtest/file/MxFileRepository.h"
#include <algorithm>
#include <memory>
#include <string>

using namespace std;
using namespace mx::api;
using namespace mx;
using namespace mxtest;

TEST(Save, ChordDataSaveTest)
{
    const auto scoreData = apiChordSimpleScoreData();
    auto &mgr = DocumentManager::getInstance();
    const auto docIdResult = mgr.createFromScore(scoreData);
    REQUIRE(docIdResult.ok());
    const int docId = docIdResult.value();
    const auto documentPtr = mgr.getDocument(docId);
    mgr.destroyDocument(docId);
    REQUIRE(documentPtr != nullptr);
    REQUIRE(documentPtr->isScorePartwise());
    const auto &scorePartwise = documentPtr->asScorePartwise();

    const auto partwiseParts = scorePartwise.part();
    REQUIRE(!partwiseParts.empty());
    const auto &partwisePart = partwiseParts.front();
    const auto partwiseMeasures = partwisePart.measure();

    // Measure 0: 2 music data choices (the note + harmony)
    REQUIRE(partwiseMeasures.size() >= 1);
    const auto &m0 = partwiseMeasures[0];
    CHECK_EQUAL(2, m0.musicData().size());
    REQUIRE(m0.musicData().size() >= 2);
    const auto &mdc0 = m0.musicData().back();
    CHECK(mdc0.isHarmony());
    const auto &harmony0 = mdc0.asHarmony();
    const auto harmonyChords0 = harmony0.harmonyChord();
    REQUIRE(!harmonyChords0.empty());
    const auto &grp0 = harmonyChords0.front();
    REQUIRE(grp0.choice().isRoot());
    const auto &root0 = grp0.choice().asRoot();
    const auto rootStep0 = root0.rootStep().value().tag();
    const bool hasRootAlter0 = root0.rootAlter().has_value();
    const double rootAlter0 = hasRootAlter0 ? root0.rootAlter()->value().value().value() : 0.0;
    const auto kind0 = grp0.kind().value().tag();
    const bool hasText0 = grp0.kind().text().has_value();
    const auto text0 = hasText0 ? grp0.kind().text().value() : std::string{};
    const bool hasBass0 = grp0.bass().has_value();
    const auto bassStep0 = hasBass0 ? grp0.bass()->bassStep().value().tag() : core::Step::Tag::a;
    const bool hasBassAlter0 = hasBass0 && grp0.bass()->bassAlter().has_value();
    const double bassAlter0 = hasBassAlter0 ? grp0.bass()->bassAlter()->value().value().value() : 0.0;
    CHECK(core::Step::Tag::c == rootStep0);
    CHECK(!hasRootAlter0);
    CHECK_DOUBLES_EQUAL(0.0, rootAlter0, 0.01);
    CHECK(core::KindValue::Tag::majorSeventh == kind0);
    CHECK(hasText0);
    CHECK_EQUAL("maj7", text0);
    CHECK(!hasBass0);
    CHECK(!hasBassAlter0);
    CHECK_DOUBLES_EQUAL(0.0, bassAlter0, 0.01);

    // Measure 1: 1 music data choice (harmony only)
    REQUIRE(partwiseMeasures.size() >= 2);
    const auto &m1 = partwiseMeasures[1];
    CHECK_EQUAL(1, m1.musicData().size());
    REQUIRE(!m1.musicData().empty());
    const auto &mdc1 = m1.musicData().back();
    CHECK(mdc1.isHarmony());
    const auto &harmony1 = mdc1.asHarmony();
    const auto harmonyChords1 = harmony1.harmonyChord();
    REQUIRE(!harmonyChords1.empty());
    const auto &grp1 = harmonyChords1.front();
    REQUIRE(grp1.choice().isRoot());
    const auto &root1 = grp1.choice().asRoot();
    const auto rootStep1 = root1.rootStep().value().tag();
    const bool hasRootAlter1 = root1.rootAlter().has_value();
    const double rootAlter1 = hasRootAlter1 ? root1.rootAlter()->value().value().value() : 0.0;
    const auto kind1 = grp1.kind().value().tag();
    const bool hasText1 = grp1.kind().text().has_value();
    const auto text1 = hasText1 ? grp1.kind().text().value() : std::string{};
    const bool hasBass1 = grp1.bass().has_value();
    const auto bassStep1 = hasBass1 ? grp1.bass()->bassStep().value().tag() : core::Step::Tag::a;
    const bool hasBassAlter1 = hasBass1 && grp1.bass()->bassAlter().has_value();
    const double bassAlter1 = hasBassAlter1 ? grp1.bass()->bassAlter()->value().value().value() : 0.0;
    CHECK(core::Step::Tag::d == rootStep1);
    CHECK(!hasRootAlter1);
    CHECK_DOUBLES_EQUAL(0.0, rootAlter1, 0.01);
    CHECK(core::KindValue::Tag::minor == kind1);
    CHECK(hasText1);
    CHECK_EQUAL("m", text1);
    CHECK(hasBass1);
    CHECK(core::Step::Tag::c == bassStep1);
    CHECK(hasBassAlter1);
    CHECK_DOUBLES_EQUAL(1.0, bassAlter1, 0.01);

    // Measure 2
    REQUIRE(partwiseMeasures.size() >= 3);
    const auto &m2 = partwiseMeasures[2];
    CHECK_EQUAL(1, m2.musicData().size());
    REQUIRE(!m2.musicData().empty());
    const auto &mdc2 = m2.musicData().back();
    CHECK(mdc2.isHarmony());
    const auto &harmony2 = mdc2.asHarmony();
    const auto harmonyChords2 = harmony2.harmonyChord();
    REQUIRE(!harmonyChords2.empty());
    const auto &grp2 = harmonyChords2.front();
    REQUIRE(grp2.choice().isRoot());
    const auto &root2 = grp2.choice().asRoot();
    const auto rootStep2 = root2.rootStep().value().tag();
    const bool hasRootAlter2 = root2.rootAlter().has_value();
    const double rootAlter2 = hasRootAlter2 ? root2.rootAlter()->value().value().value() : 0.0;
    const auto kind2 = grp2.kind().value().tag();
    const bool hasText2 = grp2.kind().text().has_value();
    const auto text2 = hasText2 ? grp2.kind().text().value() : std::string{};
    const bool hasBass2 = grp2.bass().has_value();
    const bool hasBassAlter2 = hasBass2 && grp2.bass()->bassAlter().has_value();
    const double bassAlter2 = hasBassAlter2 ? grp2.bass()->bassAlter()->value().value().value() : 0.0;
    CHECK(core::Step::Tag::e == rootStep2);
    CHECK(hasRootAlter2);
    CHECK_DOUBLES_EQUAL(-1.0, rootAlter2, 0.01);
    CHECK(core::KindValue::Tag::dominant == kind2);
    CHECK(hasText2);
    CHECK_EQUAL("7", text2);
    CHECK(!hasBass2);
    CHECK(!hasBassAlter2);
    CHECK_DOUBLES_EQUAL(0.0, bassAlter2, 0.01);

    // Measure 3
    REQUIRE(partwiseMeasures.size() >= 4);
    const auto &m3 = partwiseMeasures[3];
    CHECK_EQUAL(1, m3.musicData().size());
    REQUIRE(!m3.musicData().empty());
    const auto &mdc3 = m3.musicData().back();
    CHECK(mdc3.isHarmony());
    const auto &harmony3 = mdc3.asHarmony();
    const auto harmonyChords3 = harmony3.harmonyChord();
    REQUIRE(!harmonyChords3.empty());
    const auto &grp3 = harmonyChords3.front();
    REQUIRE(grp3.choice().isRoot());
    const auto &root3 = grp3.choice().asRoot();
    const auto rootStep3 = root3.rootStep().value().tag();
    const bool hasRootAlter3 = root3.rootAlter().has_value();
    const double rootAlter3 = hasRootAlter3 ? root3.rootAlter()->value().value().value() : 0.0;
    const auto kind3 = grp3.kind().value().tag();
    const bool hasText3 = grp3.kind().text().has_value();
    const auto text3 = hasText3 ? grp3.kind().text().value() : std::string{};
    const bool hasBass3 = grp3.bass().has_value();
    const bool hasBassAlter3 = hasBass3 && grp3.bass()->bassAlter().has_value();
    const double bassAlter3 = hasBassAlter3 ? grp3.bass()->bassAlter()->value().value().value() : 0.0;
    CHECK(core::Step::Tag::b == rootStep3);
    CHECK(!hasRootAlter3);
    CHECK_DOUBLES_EQUAL(0.0, rootAlter3, 0.01);
    CHECK(core::KindValue::Tag::diminishedSeventh == kind3);
    CHECK(hasText3);
    CHECK_EQUAL("dim7", text3);
    CHECK(!hasBass3);
    CHECK(!hasBassAlter3);
    CHECK_DOUBLES_EQUAL(0.0, bassAlter3, 0.01);

    // Harmony3 has offset
    CHECK(harmony3.offset().has_value());
    const double offsetValue = harmony3.offset()->value().value().value();
    CHECK_DOUBLES_EQUAL(109.0, offsetValue, 0.01);
}

TEST(Load1, ChordDataSaveTest)
{
    const auto scoreData = MxFileRepository::loadFile("chords_simple.xml");
    auto part = &scoreData.parts.front();

    int measureNumber = 1;
    auto measure = &part->measures.at(measureNumber - 1);
    auto staff = &measure->staves.front();
    auto directions = &staff->directions;
    CHECK_EQUAL(1, directions->size());
    auto direction = &directions->at(0);
    auto chords = &direction->chords;
    CHECK_EQUAL(1, chords->size());
    CHECK_EQUAL(0, direction->directionTypes.size());
    auto chord = &chords->at(0);
    CHECK(api::Step::c == chord->root);
    CHECK_EQUAL(0, chord->rootAlter);
    CHECK(ChordKind::majorSeventh == chord->chordKind);
    CHECK_EQUAL("maj7", chord->text);
    CHECK(Step::unspecified == chord->bass);
    CHECK_EQUAL(0, chord->bassAlter);
}

TEST(Load2, ChordDataSaveTest)
{
    const auto scoreData = MxFileRepository::loadFile("chords_simple.xml");
    auto part = &scoreData.parts.front();

    int measureNumber = 2;
    auto measure = &part->measures.at(measureNumber - 1);
    auto staff = &measure->staves.front();
    auto directions = &staff->directions;
    CHECK_EQUAL(1, directions->size());
    auto direction = &directions->at(0);
    auto chords = &direction->chords;
    CHECK_EQUAL(1, chords->size());
    CHECK_EQUAL(0, direction->directionTypes.size());
    auto chord = &chords->at(0);
    CHECK(api::Step::d == chord->root);
    CHECK_EQUAL(0, chord->rootAlter);
    CHECK(ChordKind::minor == chord->chordKind);
    CHECK_EQUAL("m", chord->text);
    CHECK(Step::c == chord->bass);
    CHECK_EQUAL(1, chord->bassAlter);
}

TEST(Load3, ChordDataSaveTest)
{
    const auto scoreData = MxFileRepository::loadFile("chords_simple.xml");
    auto part = &scoreData.parts.front();

    int measureNumber = 3;
    auto measure = &part->measures.at(measureNumber - 1);
    auto staff = &measure->staves.front();
    auto directions = &staff->directions;
    CHECK_EQUAL(1, directions->size());
    auto direction = &directions->at(0);
    auto chords = &direction->chords;
    CHECK_EQUAL(1, chords->size());
    CHECK_EQUAL(0, direction->directionTypes.size());
    auto chord = &chords->at(0);
    CHECK(api::Step::e == chord->root);
    CHECK_EQUAL(-1, chord->rootAlter);
    CHECK(ChordKind::dominant == chord->chordKind);
    CHECK_EQUAL("7", chord->text);
    CHECK(Step::unspecified == chord->bass);
    CHECK_EQUAL(0, chord->bassAlter);
}

TEST(Load4, ChordDataSaveTest)
{
    const auto scoreData = MxFileRepository::loadFile("chords_simple.xml");
    auto part = &scoreData.parts.front();

    int measureNumber = 4;
    auto measure = &part->measures.at(measureNumber - 1);
    auto staff = &measure->staves.front();
    auto directions = &staff->directions;
    CHECK_EQUAL(1, directions->size());
    auto direction = &directions->at(0);
    auto chords = &direction->chords;
    CHECK_EQUAL(1, chords->size());
    CHECK_EQUAL(0, direction->directionTypes.size());
    auto chord = &chords->at(0);
    CHECK(api::Step::b == chord->root);
    CHECK_EQUAL(0, chord->rootAlter);
    CHECK(ChordKind::diminishedSeventh == chord->chordKind);
    CHECK_EQUAL("dim7", chord->text);
    CHECK(Step::unspecified == chord->bass);
    CHECK_EQUAL(0, chord->bassAlter);
}

TEST(SaveProcessingInstructions, ChordDataSaveTest)
{
    ScoreData scoreData{};
    scoreData.parts.emplace_back();
    auto part = &scoreData.parts.back();
    part->measures.emplace_back();
    auto measure = &part->measures.back();
    measure->staves.emplace_back();
    auto staff = &measure->staves.back();
    staff->directions.emplace_back();
    auto direction = &staff->directions.back();
    direction->chords.emplace_back();
    auto chord = &direction->chords.back();
    chord->root = Step::b;
    chord->miscData.emplace_back(MiscData{"test", "123"});
    const auto xml = toXml(scoreData);
    auto outScore = fromXml(xml);
    part = nullptr;
    measure = nullptr;
    staff = nullptr;
    chord = nullptr;
    part = &outScore.parts.back();
    measure = &part->measures.back();
    staff = &measure->staves.back();
    direction = &staff->directions.back();
    chord = &direction->chords.back();
    // §11 adjudication #7: ChordData::miscData was stored via XML ProcessingInstructions
    // in the old core; the new generated core is strictly typed from the MusicXML XSD and
    // does not support ProcessingInstructions. miscData does not survive serialization.
    REQUIRE(0 == chord->miscData.size());
    CHECK(chord->root == Step::b);
}

TEST(SavePositionData, ChordDataSaveTest)
{
    ScoreData scoreData{};
    scoreData.parts.emplace_back();
    auto part = &scoreData.parts.back();
    part->measures.emplace_back();
    auto measure = &part->measures.back();
    measure->staves.emplace_back();
    auto staff = &measure->staves.back();
    staff->directions.emplace_back();
    auto direction = &staff->directions.back();
    direction->chords.emplace_back();
    auto chord = &direction->chords.back();
    chord->root = Step::b;
    chord->positionData.isDefaultXSpecified = true;
    chord->positionData.defaultX = 123.0;
    chord->positionData.isDefaultYSpecified = true;
    chord->positionData.defaultY = 456.0;

    const auto xml = toXml(scoreData);
    auto outScore = fromXml(xml);

    part = nullptr;
    measure = nullptr;
    staff = nullptr;
    chord = nullptr;

    part = &outScore.parts.back();
    measure = &part->measures.back();
    staff = &measure->staves.back();
    direction = &staff->directions.back();
    chord = &direction->chords.back();

    CHECK(chord->positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(123.0, chord->positionData.defaultX, 0.001);

    CHECK(chord->positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(456.0, chord->positionData.defaultY, 0.001);
}

#endif
