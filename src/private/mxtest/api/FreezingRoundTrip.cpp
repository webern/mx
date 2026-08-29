// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/generated/DirectionTypeChoice.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/DynamicsChoice.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/NoteTypeValue.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/file/MxFileRepository.h"
#include "mxtest/file/StupidFileFunctions.h"

#include <algorithm>
#include <sstream>

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
constexpr const char *const freezingFile = "freezing.xml";
constexpr const char *const chordDirFile = "ChordDirectionPlacement.xml";
constexpr const char *const hasVerFile = "HasMusicXmlVersionTrue.xml";
constexpr const char *const noVerFile = "HasMusicXmlVersionFalse.xml";
constexpr const char *const tupletType = "PreserveTimeModificationNormalType.xml";
} // namespace

namespace
{
inline bool writeRoundTrip(std::string inFilename)
{
    const auto nameWithoutExtension = mxtest::MxFileRepository::getNameWithoutExtension(inFilename);
    const auto outDir = mxtest::getTestOutputFullDirectoryPath();
    const auto outBeforeFilepath = outDir + nameWithoutExtension + std::string{"_before.xml"};
    const auto outAfterFilepath = outDir + nameWithoutExtension + std::string{"_after.xml"};
    const auto scoreData = mxtest::MxFileRepository::loadFile(inFilename);
    const auto filePath = mxtest::MxFileRepository::getFullPath(inFilename);
    auto &docMgr = DocumentManager::getInstance();
    const auto rBefore = docMgr.createFromFile(filePath);
    if (!rBefore.ok())
    {
        return false;
    }
    const int docId = rBefore.value();
    docMgr.writeToFile(docId, outBeforeFilepath);
    docMgr.destroyDocument(docId);
    const auto rAfter = docMgr.createFromScore(scoreData);
    if (!rAfter.ok())
    {
        return false;
    }
    const int apiDocId = rAfter.value();
    docMgr.writeToFile(apiDocId, outAfterFilepath);
    docMgr.destroyDocument(apiDocId);
    return docId != apiDocId;
}

/// Holds references into Documents that must be kept alive.
struct TestData
{
    mx::core::DocumentPtr originalDoc;
    mx::core::DocumentPtr savedDoc;
    mx::api::ScoreData originalScoreData;
    mx::api::ScoreData savedScoreData;

    TestData(mx::core::DocumentPtr inOriginalDoc, mx::core::DocumentPtr inSavedDoc,
             mx::api::ScoreData inOriginalScoreData, mx::api::ScoreData inSavedScoreData)
        : originalDoc{std::move(inOriginalDoc)}, savedDoc{std::move(inSavedDoc)},
          originalScoreData{std::move(inOriginalScoreData)}, savedScoreData{std::move(inSavedScoreData)}
    {
    }

    const mx::core::ScorePartwise &originalScore() const
    {
        return originalDoc->asScorePartwise();
    }

    const mx::core::ScorePartwise &savedScore() const
    {
        return savedDoc->asScorePartwise();
    }

    //////////////////////////// original score ///////////////////// saved score ///////////////////
    inline std::pair<std::span<const mx::core::MusicDataChoice>, std::span<const mx::core::MusicDataChoice>>
    getMusicData(int inPartIndex, int inMeasureIndex) const
    {
        const auto oParts = originalScore().part();
        const auto sParts = savedScore().part();
        const auto &oPart = oParts[static_cast<size_t>(inPartIndex)];
        const auto &sPart = sParts[static_cast<size_t>(inPartIndex)];
        const auto oMdc = oPart.measure()[static_cast<size_t>(inMeasureIndex)].musicData();
        const auto sMdc = sPart.measure()[static_cast<size_t>(inMeasureIndex)].musicData();
        return {oMdc, sMdc};
    }
};

inline TestData getTestData(std::string filename)
{
    auto &mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath(filename);
    const auto rOrig = mgr.createFromFile(filePath);
    const int originalId = rOrig.ok() ? rOrig.value() : -1;
    const auto rOrigData = mgr.getData(originalId);
    const auto originalScoreData = rOrigData.ok() ? rOrigData.value() : mx::api::ScoreData{};
    const auto rSaved = mgr.createFromScore(originalScoreData);
    const int savedId = rSaved.ok() ? rSaved.value() : -1;
    const auto rSavedData = mgr.getData(savedId);
    const auto savedScoreData = rSavedData.ok() ? rSavedData.value() : mx::api::ScoreData{};
    auto originalDoc = mgr.getDocument(originalId);
    auto savedDoc = mgr.getDocument(savedId);
    mgr.destroyDocument(originalId);
    mgr.destroyDocument(savedId);
    return TestData{originalDoc, savedDoc, originalScoreData, savedScoreData};
}
} // namespace

TEST(roundTripChordDirFile, Freezing)
{
    const auto success = writeRoundTrip(chordDirFile);
    CHECK(success);
}

T_END

TEST(chordDirectionPlacement, Freezing)
{
    const auto testData = getTestData(chordDirFile);
    const auto musicData = testData.getMusicData(0, 0);
    const auto oNumEntries = musicData.first.size();
    const auto sNumEntries = musicData.second.size();
    CHECK(oNumEntries > 0);
    CHECK_EQUAL(oNumEntries, sNumEntries);

    for (size_t i = 0; i < oNumEntries; ++i)
    {
        const auto &o = musicData.first[i];
        const auto &s = musicData.second[i];
        const auto oKind = o.kind();
        const auto sKind = s.kind();
        std::stringstream ss;
        ss << "After round trip the MusicDataChoice elements are not the same. Currently index (i) == " << i << ".";
        ss << " ( oKind == sKind ) was supposed to return true, but returned false.";
        ss << " with ( " << static_cast<int>(oKind) << " == " << static_cast<int>(sKind) << " )";
        const std::string message = ss.str();
        CHECK_WITH_MESSAGE(oKind == sKind, message);
    }

    CHECK(true);
}

T_END

TEST(roundTripOutput, Freezing)
{
    const auto success = writeRoundTrip(freezingFile);
    CHECK(success);
}

T_END

TEST(roundTripViolaDynamicWrongTime, Freezing)
{
    // in the original file measure number="X7" implicit="yes" width="389"
    // or search for font-family="roundTripViolaDynamicWrongTime"
    auto &mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath(freezingFile);
    const auto rOrigId = mgr.createFromFile(filePath);
    REQUIRE(rOrigId.ok());
    const int originalId = rOrigId.value();
    const auto rOrigData = mgr.getData(originalId);
    REQUIRE(rOrigData.ok());
    const auto originalScoreData = rOrigData.value();
    auto originalDoc = mgr.getDocument(originalId);
    const auto rSavedId = mgr.createFromScore(originalScoreData);
    REQUIRE(rSavedId.ok());
    const int savedId = rSavedId.value();
    const auto rSavedData = mgr.getData(savedId);
    REQUIRE(rSavedData.ok());
    const auto savedScoreData = rSavedData.value();
    auto savedDoc = mgr.getDocument(savedId);
    mgr.destroyDocument(originalId);
    mgr.destroyDocument(savedId);

    const size_t partIndex = 0;
    const size_t measureIndex = 7;

    const auto &originalScore = originalDoc->asScorePartwise();
    const auto &savedScore = savedDoc->asScorePartwise();

    const auto originalMdcSpan = originalScore.part()[partIndex].measure()[measureIndex].musicData();
    auto originalMdcIter = originalMdcSpan.begin();
    const auto originalMdcEnd = originalMdcSpan.end();

    while ((originalMdcIter != originalMdcEnd) && (!originalMdcIter->isNote()))
    {
        ++originalMdcIter;
    }

    // this is the first note in the measure
    CHECK(originalMdcIter != originalMdcEnd);
    CHECK(originalMdcIter->isNote());
    const auto *originalCurrentNote = &originalMdcIter->asNote();
    REQUIRE(originalCurrentNote->choice().isNormalNoteGroup());
    CHECK_DOUBLES_EQUAL(30.0, originalCurrentNote->choice().asNormalNoteGroup().duration().value().value(), 0.0001);
    CHECK(!originalCurrentNote->choice().asNormalNoteGroup().fullNote().chord());

    // this is the second note in the measure
    ++originalMdcIter;
    CHECK(originalMdcIter != originalMdcEnd);
    CHECK(originalMdcIter->isNote());
    originalCurrentNote = &originalMdcIter->asNote();
    REQUIRE(originalCurrentNote->choice().isNormalNoteGroup());
    CHECK_DOUBLES_EQUAL(30.0, originalCurrentNote->choice().asNormalNoteGroup().duration().value().value(), 0.0001);
    CHECK(originalCurrentNote->choice().asNormalNoteGroup().fullNote().chord());

    // this is where we find the first pp dynamic in the measure
    ++originalMdcIter;
    CHECK(originalMdcIter != originalMdcEnd);
    CHECK(originalMdcIter->isDirection());
    const auto &originalDirection = originalMdcIter->asDirection();
    const auto &origDtSpan = originalDirection.directionType();
    CHECK(!origDtSpan.empty());
    CHECK(origDtSpan[0].choice().isDynamics());
    const auto &origDynItems = origDtSpan[0].choice().asDynamics().items();
    CHECK(!origDynItems.empty());
    const auto &origDynChoiceSpan = origDynItems[0].choice();
    CHECK(!origDynChoiceSpan.empty());
    CHECK(mx::core::DynamicsChoice::Kind::pp == origDynChoiceSpan[0].kind());

    const auto &originalDirections =
        originalScoreData.parts.at(partIndex).measures.at(measureIndex).staves.at(0).directions;
    const auto originalDirectionsBegin = std::cbegin(originalDirections);
    const auto originalDirectionsEnd = std::cend(originalDirections);

    const auto findDirectionLambda = [&](const DirectionData &inDirection) {
        if (inDirection.directionTypes.size() == 1 && inDirection.directionTypes.front().isMark())
        {
            if (inDirection.directionTypes.front().mark().choice.dynamic() == StandardDynamic::pp)
            {
                return true;
            }
        }

        return false;
    };

    const auto directionIter = std::find_if(originalDirectionsBegin, originalDirectionsEnd, findDirectionLambda);
    const double tickTimeScaleFactor = 2.0;
    CHECK_DOUBLES_EQUAL(30.0 * tickTimeScaleFactor, directionIter->tickTimePosition, 0.001);

    const auto savedMdcSpan = savedScore.part()[partIndex].measure()[measureIndex].musicData();
    auto savedMdcIter = savedMdcSpan.begin();
    const auto savedMdcEnd = savedMdcSpan.end();

    CHECK(savedMdcIter != savedMdcEnd);
    CHECK(savedMdcIter->isPrint());

    ++savedMdcIter;
    CHECK(savedMdcIter != savedMdcEnd);
    CHECK(savedMdcIter->isNote());
    const auto *savedCurrentNote = &savedMdcIter->asNote();
    REQUIRE(savedCurrentNote->choice().isNormalNoteGroup());
    CHECK_DOUBLES_EQUAL(30.0 * tickTimeScaleFactor,
                        savedCurrentNote->choice().asNormalNoteGroup().duration().value().value(), 0.0001);
    CHECK(!savedCurrentNote->choice().asNormalNoteGroup().fullNote().chord());

    ++savedMdcIter;
    CHECK(savedMdcIter != savedMdcEnd);
    CHECK(savedMdcIter->isNote());
    savedCurrentNote = &savedMdcIter->asNote();
    REQUIRE(savedCurrentNote->choice().isNormalNoteGroup());
    CHECK_DOUBLES_EQUAL(30.0 * tickTimeScaleFactor,
                        savedCurrentNote->choice().asNormalNoteGroup().duration().value().value(), 0.0001);
    CHECK(savedCurrentNote->choice().asNormalNoteGroup().fullNote().chord());

    ++savedMdcIter;
    CHECK(savedMdcIter != savedMdcEnd);
    CHECK(savedMdcIter->isDirection());
    const auto &savedDirection = savedMdcIter->asDirection();
    const auto &savedDtSpan = savedDirection.directionType();
    CHECK(!savedDtSpan.empty());
    CHECK(savedDtSpan[0].choice().isDynamics());
    const auto &savedDynItems = savedDtSpan[0].choice().asDynamics().items();
    CHECK(!savedDynItems.empty());
    const auto &savedDynChoiceSpan = savedDynItems[0].choice();
    CHECK(!savedDynChoiceSpan.empty());
    CHECK(mx::core::DynamicsChoice::Kind::pp == savedDynChoiceSpan[0].kind());
}

T_END

TEST(hasVersion, Freezing)
{
    const auto testData = getTestData(hasVerFile);
    const auto &savedVersion = testData.savedScore().version();
    CHECK(savedVersion.has_value());
    CHECK_EQUAL("3.0", savedVersion.value());
}

T_END

TEST(hasNoVersion, Freezing)
{
    const auto testData = getTestData(noVerFile);
    const auto &savedVersion = testData.savedScore().version();
    CHECK(!savedVersion.has_value());
}

T_END

TEST(missingMusicXMLVersion, Freezing)
{
    auto &mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath(freezingFile);
    const auto rOrigId = mgr.createFromFile(filePath);
    REQUIRE(rOrigId.ok());
    const int originalId = rOrigId.value();
    auto originalDoc = mgr.getDocument(originalId);
    const auto rOrigData = mgr.getData(originalId);
    REQUIRE(rOrigData.ok());
    const auto rSavedId = mgr.createFromScore(rOrigData.value());
    REQUIRE(rSavedId.ok());
    const int savedId = rSavedId.value();
    auto savedDoc = mgr.getDocument(savedId);
    mgr.destroyDocument(originalId);
    mgr.destroyDocument(savedId);

    const bool originalScoreHasVersion = originalDoc->asScorePartwise().version().has_value();
    const bool savedScoreHasVersion = savedDoc->asScorePartwise().version().has_value();
    CHECK(originalScoreHasVersion);
    CHECK(savedScoreHasVersion);
}

TEST(HasDefaultsHasAppearance, Freezing)
{
    auto &mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath(freezingFile);
    const auto rOrigId = mgr.createFromFile(filePath);
    REQUIRE(rOrigId.ok());
    const int originalId = rOrigId.value();
    auto originalDoc = mgr.getDocument(originalId);
    const auto rOrigData = mgr.getData(originalId);
    REQUIRE(rOrigData.ok());
    const auto rSavedId = mgr.createFromScore(rOrigData.value());
    REQUIRE(rSavedId.ok());
    const int savedId = rSavedId.value();
    auto savedDoc = mgr.getDocument(savedId);
    mgr.destroyDocument(originalId);
    mgr.destroyDocument(savedId);

    const auto &origHeader = originalDoc->asScorePartwise().scoreHeader();
    const auto &savedHeader = savedDoc->asScorePartwise().scoreHeader();

    const bool originalHasDefaults = origHeader.defaults().has_value();
    const bool savedHasDefaults = savedHeader.defaults().has_value();
    CHECK(originalHasDefaults == savedHasDefaults);

    if (!originalHasDefaults || !savedHasDefaults)
    {
        return;
    }

    const auto &origDefaults = origHeader.defaults().value();
    const auto &savedDefaults = savedHeader.defaults().value();

    const bool originalHasAppearance = origDefaults.appearance().has_value();
    const bool savedHasAppearance = savedDefaults.appearance().has_value();
    CHECK(originalHasAppearance == savedHasAppearance);

    if (!originalHasAppearance || !savedHasAppearance)
    {
        return;
    }

    const auto &originalAppearance = origDefaults.appearance().value();
    const auto &savedAppearance = savedDefaults.appearance().value();

    for (size_t i = 0; i < originalAppearance.lineWidth().size(); ++i)
    {
        CHECK_DOUBLES_EQUAL(originalAppearance.lineWidth()[i].value().value().value(),
                            savedAppearance.lineWidth()[i].value().value().value(), 0.0001);
        CHECK(originalAppearance.lineWidth()[i].type().value() == savedAppearance.lineWidth()[i].type().value());
    }

    for (size_t i = 0; i < originalAppearance.noteSize().size(); ++i)
    {
        CHECK_DOUBLES_EQUAL(originalAppearance.noteSize()[i].value().value().value(),
                            savedAppearance.noteSize()[i].value().value().value(), 0.0001);
        CHECK(originalAppearance.noteSize()[i].type() == savedAppearance.noteSize()[i].type());
    }

    for (size_t i = 0; i < originalAppearance.distance().size(); ++i)
    {
        CHECK_DOUBLES_EQUAL(originalAppearance.distance()[i].value().value().value(),
                            savedAppearance.distance()[i].value().value().value(), 0.0001);
        CHECK(originalAppearance.distance()[i].type().value() == savedAppearance.distance()[i].type().value());
    }
}

TEST(appearanceLineWidths, Freezing)
{
    auto &mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath(freezingFile);
    const auto rOrigId = mgr.createFromFile(filePath);
    REQUIRE(rOrigId.ok());
    const int originalId = rOrigId.value();
    auto originalDoc = mgr.getDocument(originalId);
    const auto rOrigData = mgr.getData(originalId);
    REQUIRE(rOrigData.ok());
    const auto rSavedId = mgr.createFromScore(rOrigData.value());
    REQUIRE(rSavedId.ok());
    const int savedId = rSavedId.value();
    auto savedDoc = mgr.getDocument(savedId);
    mgr.destroyDocument(originalId);
    mgr.destroyDocument(savedId);

    REQUIRE(originalDoc->asScorePartwise().scoreHeader().defaults().has_value());
    REQUIRE(savedDoc->asScorePartwise().scoreHeader().defaults().has_value());
    REQUIRE(originalDoc->asScorePartwise().scoreHeader().defaults()->appearance().has_value());
    REQUIRE(savedDoc->asScorePartwise().scoreHeader().defaults()->appearance().has_value());

    const auto &originalAppearance = originalDoc->asScorePartwise().scoreHeader().defaults()->appearance().value();
    const auto &savedAppearance = savedDoc->asScorePartwise().scoreHeader().defaults()->appearance().value();

    const auto lineWidthSetSize = savedAppearance.lineWidth().size();
    CHECK(lineWidthSetSize > 0);
    CHECK_EQUAL(originalAppearance.lineWidth().size(), savedAppearance.lineWidth().size());

    for (size_t i = 0; i < lineWidthSetSize; ++i)
    {
        const auto &savedElement = savedAppearance.lineWidth()[i];
        const auto &originalElement = originalAppearance.lineWidth()[i];
        const auto savedLineWidth = savedElement.value().value().value();
        const auto originalLineWidth = originalElement.value().value().value();
        CHECK_DOUBLES_EQUAL(originalLineWidth, savedLineWidth, 0.00001);

        CHECK_EQUAL(originalElement.type().value(), savedElement.type().value());
    }
}

TEST(appearanceNoteSize, Freezing)
{
    auto &mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath(freezingFile);
    const auto rOrigId = mgr.createFromFile(filePath);
    REQUIRE(rOrigId.ok());
    const int originalId = rOrigId.value();
    auto originalDoc = mgr.getDocument(originalId);
    const auto rOrigData = mgr.getData(originalId);
    REQUIRE(rOrigData.ok());
    const auto rSavedId = mgr.createFromScore(rOrigData.value());
    REQUIRE(rSavedId.ok());
    const int savedId = rSavedId.value();
    auto savedDoc = mgr.getDocument(savedId);
    mgr.destroyDocument(originalId);
    mgr.destroyDocument(savedId);

    REQUIRE(originalDoc->asScorePartwise().scoreHeader().defaults().has_value());
    REQUIRE(savedDoc->asScorePartwise().scoreHeader().defaults().has_value());
    REQUIRE(originalDoc->asScorePartwise().scoreHeader().defaults()->appearance().has_value());
    REQUIRE(savedDoc->asScorePartwise().scoreHeader().defaults()->appearance().has_value());

    const auto &originalAppearance = originalDoc->asScorePartwise().scoreHeader().defaults()->appearance().value();
    const auto &savedAppearance = savedDoc->asScorePartwise().scoreHeader().defaults()->appearance().value();

    const auto noteSizeSetSize = savedAppearance.noteSize().size();
    CHECK(noteSizeSetSize > 0);
    CHECK_EQUAL(originalAppearance.noteSize().size(), savedAppearance.noteSize().size());

    for (size_t i = 0; i < noteSizeSetSize; ++i)
    {
        const auto &savedElement = savedAppearance.noteSize()[i];
        const auto &originalElement = originalAppearance.noteSize()[i];
        const auto savedNoteSize = savedElement.value().value().value();
        const auto originalNoteSize = originalElement.value().value().value();
        CHECK_DOUBLES_EQUAL(originalNoteSize, savedNoteSize, 0.00001);

        CHECK(originalElement.type() == savedElement.type());
    }
}

TEST(appearancDistance, Freezing)
{
    auto &mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath(freezingFile);
    const auto rOrigId = mgr.createFromFile(filePath);
    REQUIRE(rOrigId.ok());
    const int originalId = rOrigId.value();
    auto originalDoc = mgr.getDocument(originalId);
    const auto rOrigData = mgr.getData(originalId);
    REQUIRE(rOrigData.ok());
    const auto rSavedId = mgr.createFromScore(rOrigData.value());
    REQUIRE(rSavedId.ok());
    const int savedId = rSavedId.value();
    auto savedDoc = mgr.getDocument(savedId);
    mgr.destroyDocument(originalId);
    mgr.destroyDocument(savedId);

    REQUIRE(originalDoc->asScorePartwise().scoreHeader().defaults().has_value());
    REQUIRE(savedDoc->asScorePartwise().scoreHeader().defaults().has_value());
    REQUIRE(originalDoc->asScorePartwise().scoreHeader().defaults()->appearance().has_value());
    REQUIRE(savedDoc->asScorePartwise().scoreHeader().defaults()->appearance().has_value());

    const auto &originalAppearance = originalDoc->asScorePartwise().scoreHeader().defaults()->appearance().value();
    const auto &savedAppearance = savedDoc->asScorePartwise().scoreHeader().defaults()->appearance().value();

    const auto distanceSetSize = savedAppearance.distance().size();
    CHECK(distanceSetSize > 0);
    CHECK_EQUAL(originalAppearance.distance().size(), savedAppearance.distance().size());

    for (size_t i = 0; i < distanceSetSize; ++i)
    {
        const auto &savedElement = savedAppearance.distance()[i];
        const auto &originalElement = originalAppearance.distance()[i];
        const auto savedDistance = savedElement.value().value().value();
        const auto originalDistance = originalElement.value().value().value();
        CHECK_DOUBLES_EQUAL(originalDistance, savedDistance, 0.00001);

        CHECK_EQUAL(originalElement.type().value(), savedElement.type().value());
    }
}

TEST(checkMissingNormalTypeSimple, Freezing)
{
    const auto testData = getTestData(tupletType);
    const auto musicData = testData.getMusicData(0, 0);

    // The first note's <normal-type>eighth</normal-type> differs from its own <type> and
    // must survive the round trip.
    const auto &mdc1 = musicData.second[1];
    REQUIRE(mdc1.isNote());
    const auto &note1 = mdc1.asNote();
    REQUIRE(note1.timeModification().has_value());
    REQUIRE(note1.timeModification()->group().has_value());
    CHECK(mx::core::NoteTypeValue::Tag::eighth == note1.timeModification()->group()->normalType().tag());

    // The second note has no <normal-type> in the source, and the writer must not invent
    // one (#428).
    const auto &mdc2 = musicData.second[2];
    REQUIRE(mdc2.isNote());
    const auto &note2 = mdc2.asNote();
    REQUIRE(note2.timeModification().has_value());
    CHECK(!note2.timeModification()->group().has_value());
}

TEST(checkMissingNormalType, Freezing)
{
    const auto testData = getTestData(freezingFile);
    const auto partCount = testData.originalScore().part().size();

    for (size_t partIndex = 0; partIndex < partCount; ++partIndex)
    {
        const auto measureCount = testData.originalScore().part()[partIndex].measure().size();

        for (size_t measureIndex = 0; measureIndex < measureCount; ++measureIndex)
        {
            const auto music = testData.getMusicData(static_cast<int>(partIndex), static_cast<int>(measureIndex));

            // Filter notes with time modification from both original and saved
            std::vector<const mx::core::MusicDataChoice *> originalNotes;
            std::vector<const mx::core::MusicDataChoice *> savedNotes;

            for (const auto &mdc : music.first)
            {
                if (mdc.isNote() && mdc.asNote().timeModification().has_value())
                {
                    originalNotes.push_back(&mdc);
                }
            }

            for (const auto &mdc : music.second)
            {
                if (mdc.isNote() && mdc.asNote().timeModification().has_value())
                {
                    savedNotes.push_back(&mdc);
                }
            }

            {
                std::stringstream message1;
                message1 << "( originalNotes.size() == savedNotes.size() ), ";
                message1 << "( " << originalNotes.size() << " == " << savedNotes.size() << " ), ";
                message1 << " partIndex = " << partIndex << ", measureIndex = " << measureIndex;
                CHECK_WITH_MESSAGE(originalNotes.size() == savedNotes.size(), message1.str());
            }

            const size_t numNotes = originalNotes.size();
            for (size_t i = 0; i < numNotes; ++i)
            {
                const auto &savedNote = savedNotes[i]->asNote();
                REQUIRE(savedNote.timeModification().has_value());

                // <normal-type> presence must match the original: present where the source
                // wrote one, absent where it did not (#428).
                const auto &origNote = originalNotes[i]->asNote();
                const bool origHasGroup = origNote.timeModification()->group().has_value();
                const bool savedHasGroup = savedNote.timeModification()->group().has_value();

                {
                    std::stringstream message2;
                    message2 << "( origHasGroup == savedHasGroup ), ";
                    message2 << "( " << origHasGroup << " == " << savedHasGroup << " ), ";
                    message2 << " partIndex = " << partIndex << ", measureIndex = " << measureIndex;
                    CHECK_WITH_MESSAGE(origHasGroup == savedHasGroup, message2.str());
                }

                if (origHasGroup && savedHasGroup)
                {
                    const auto originalType = origNote.timeModification()->group()->normalType();
                    const auto savedType = savedNote.timeModification()->group()->normalType();

                    if (originalType != savedType)
                    {
                        std::stringstream message3;
                        message3 << "( originalType == savedType ), ";
                        message3 << "( " << originalType.toString() << " == " << savedType.toString() << " ), ";
                        message3 << " partIndex = " << partIndex << ", measureIndex = " << measureIndex;
                        CHECK_WITH_MESSAGE(originalType == savedType, message3.str());
                    }
                }
            }
        }
    }
}

#endif
