// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"
#include "mxtest/file/Path.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

TEST(otherArticulation, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();

    note.noteAttachmentData.marks.emplace_back(Placement::unspecified, MarkType::otherArticulation);
    note.noteAttachmentData.marks.back().positionData.isDefaultXSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultX = 333.3;
    note.noteAttachmentData.marks.back().name = "october 2018";

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    auto &opart = oscore.parts.back();
    auto &omeasure = opart.measures.back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];
    auto &onote = ovoice.notes.back();
    auto &oattachments = onote.noteAttachmentData;
    auto &omarks = oattachments.marks;
    auto oIter = omarks.cbegin();

    auto md = *oIter;
    CHECK(md.markType == MarkType::otherArticulation);
    CHECK(md.positionData.isDefaultXSpecified);
    CHECK(!md.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(333.3, md.positionData.defaultX, 0.00001);
    CHECK_EQUAL("october 2018", md.name);
    CHECK(md.positionData.placement == Placement::unspecified);
}

T_END;

TEST(pitchedRestDisplayStepOctave, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 96;
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData rest;
    rest.isRest = true;
    rest.isDisplayStepOctaveSpecified = true;
    rest.pitchData.step = Step::e;
    rest.pitchData.octave = 4;
    rest.durationData.durationName = DurationName::quarter;
    rest.durationData.durationTimeTicks = 96;
    voice.notes.push_back(rest);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const auto xml = ss.str();
    CHECK(xml.find("<display-step>E</display-step>") != std::string::npos);
    CHECK(xml.find("<display-octave>4</display-octave>") != std::string::npos);

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    const auto outScore = rd.value();
    mgr.destroyDocument(docId);

    const auto &outRest = outScore.parts.back().measures.back().staves.back().voices.begin()->second.notes.back();
    CHECK(outRest.isRest);
    CHECK(outRest.isDisplayStepOctaveSpecified);
    CHECK(outRest.pitchData.step == Step::e);
    CHECK_EQUAL(4, outRest.pitchData.octave);
}

TEST(customErrorUnknown, MarkData)
{
    const auto expectedMark = mx::api::MarkType::customErrorUnknown;
    const std::string expectedString = mx::api::markStringCustomErrorUnknown;

    const auto isCustom = mx::api::isMarkCustom(expectedMark);
    CHECK(!isCustom);
    const auto actualString = mx::api::getCustomMarkName(expectedMark);
    CHECK_EQUAL(expectedString, actualString);
    const auto actualMark = mx::api::getMarkTypeFromCustomString(actualString);
    CHECK(expectedMark == actualMark);
}

TEST(customArticulation, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();

    note.noteAttachmentData.marks.emplace_back(Placement::unspecified, MarkType::customAccentTenuto);
    note.noteAttachmentData.marks.back().positionData.isDefaultXSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultX = 333.3;

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    auto &opart = oscore.parts.back();
    auto &omeasure = opart.measures.back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];
    auto &onote = ovoice.notes.back();
    auto &oattachments = onote.noteAttachmentData;
    auto &omarks = oattachments.marks;
    auto oIter = omarks.cbegin();

    auto md = *oIter;
    CHECK(md.markType == MarkType::customAccentTenuto);
    CHECK(md.positionData.isDefaultXSpecified);
    CHECK(!md.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(333.3, md.positionData.defaultX, 0.00001);
    CHECK_EQUAL(mx::api::markStringCustomAccentTenuto, md.name);
    CHECK(md.positionData.placement == Placement::unspecified);
}

T_END;

TEST(otherOrnament, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();

    note.noteAttachmentData.marks.emplace_back(Placement::unspecified, MarkType::otherOrnament);
    note.noteAttachmentData.marks.back().positionData.isDefaultXSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultX = 333.3;
    note.noteAttachmentData.marks.back().name = "**()00))&</>";

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    auto &opart = oscore.parts.back();
    auto &omeasure = opart.measures.back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];
    auto &onote = ovoice.notes.back();
    auto &oattachments = onote.noteAttachmentData;
    auto &omarks = oattachments.marks;
    auto oIter = omarks.cbegin();

    auto md = *oIter;
    CHECK(md.markType == MarkType::otherOrnament);
    CHECK(md.positionData.isDefaultXSpecified);
    CHECK(!md.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(333.3, md.positionData.defaultX, 0.00001);
    CHECK_EQUAL("**()00))&</>", md.name);
    CHECK(md.positionData.placement == Placement::unspecified);
}

T_END;

TEST(technical, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();

    note.noteAttachmentData.marks.emplace_back(Placement::above, MarkType::upBow);
    note.noteAttachmentData.marks.back().positionData.isDefaultXSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultX = 123.0;

    note.noteAttachmentData.marks.emplace_back(Placement::above, MarkType::otherTechnical);
    note.noteAttachmentData.marks.back().positionData.isDefaultYSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultY = -456.0;
    note.noteAttachmentData.marks.back().name = "Bob";

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    auto &opart = oscore.parts.back();
    auto &omeasure = opart.measures.back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];
    auto &onote = ovoice.notes.back();
    auto &oattachments = onote.noteAttachmentData;
    auto &omarks = oattachments.marks;
    auto oIter = omarks.cbegin();

    auto md = *oIter;
    CHECK(md.markType == MarkType::upBow);
    CHECK(md.positionData.isDefaultXSpecified);
    CHECK(!md.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(123.0, md.positionData.defaultX, 0.00001);

    ++oIter;
    md = *oIter;
    CHECK(md.markType == MarkType::otherTechnical);
    CHECK(!md.positionData.isDefaultXSpecified);
    CHECK(md.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(-456.0, md.positionData.defaultY, 0.00001);
    CHECK_EQUAL("Bob", md.name);
}

T_END;

// Issue #185: technical marks with text payloads. <fingering> carries text
// (e.g. "1", "2-3") plus substitution/alternate attributes; <pluck> carries
// text (e.g. "p", "i", "m", "a"). Both must survive an XML round trip.
TEST(technical_fingering_pluck_roundtrip, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();

    // a plain fingering "1"
    note.noteAttachmentData.marks.emplace_back(Placement::above, MarkType::fingering);
    note.noteAttachmentData.marks.back().name = "1";

    // a fingering "2-3" with substitution=yes and alternate=no
    note.noteAttachmentData.marks.emplace_back(Placement::below, MarkType::fingering);
    note.noteAttachmentData.marks.back().name = "2-3";
    note.noteAttachmentData.marks.back().fingeringSubstitution = Bool::yes;
    note.noteAttachmentData.marks.back().fingeringAlternate = Bool::no;

    // a pluck "p"
    note.noteAttachmentData.marks.emplace_back(Placement::above, MarkType::pluck);
    note.noteAttachmentData.marks.back().name = "p";

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    std::istringstream iss{ss.str()};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    const auto oscore = rd.value();
    mgr.destroyDocument(docId);

    const auto &omarks =
        oscore.parts.back().measures.back().staves.back().voices.at(0).notes.back().noteAttachmentData.marks;
    REQUIRE(omarks.size() == 3);

    auto oIter = omarks.cbegin();
    auto md = *oIter;
    CHECK(md.markType == MarkType::fingering);
    CHECK_EQUAL("1", md.name);
    CHECK(md.positionData.placement == Placement::above);
    CHECK(md.fingeringSubstitution == Bool::unspecified);
    CHECK(md.fingeringAlternate == Bool::unspecified);

    ++oIter;
    md = *oIter;
    CHECK(md.markType == MarkType::fingering);
    CHECK_EQUAL("2-3", md.name);
    CHECK(md.positionData.placement == Placement::below);
    CHECK(md.fingeringSubstitution == Bool::yes);
    CHECK(md.fingeringAlternate == Bool::no);

    ++oIter;
    md = *oIter;
    CHECK(md.markType == MarkType::pluck);
    CHECK_EQUAL("p", md.name);
    CHECK(md.positionData.placement == Placement::above);
}

T_END;

TEST(technical_import_file, NoteData)
{
    auto &mgr = DocumentManager::getInstance();
    const auto path = std::string{mxtest::getResourcesDirectoryPath()} + std::string{"/ksuite/k004a_Technical.xml"};
    const auto r = mgr.createFromFile(path);
    REQUIRE(r.ok());
    const int docId = r.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    const auto score = rd.value();
    mgr.destroyDocument(docId);

    const auto &part = score.parts.at(0);

    const auto &fingernailsMark =
        part.measures.at(19).staves.at(0).voices.at(0).notes.at(0).noteAttachmentData.marks.at(0);
    CHECK(fingernailsMark.markType == MarkType::fingernails);
    CHECK_EQUAL("fingernails", fingernailsMark.name);

    const auto &holeMark = part.measures.at(20).staves.at(0).voices.at(0).notes.at(0).noteAttachmentData.marks.at(0);
    CHECK(holeMark.markType == MarkType::hole);
    CHECK_EQUAL("windOpenHole", holeMark.name);

    const auto &arrowMark = part.measures.at(21).staves.at(0).voices.at(0).notes.at(0).noteAttachmentData.marks.at(0);
    CHECK(arrowMark.markType == MarkType::arrow);
    CHECK_EQUAL("arrowOpenUp", arrowMark.name);

    const auto &handbellMark =
        part.measures.at(22).staves.at(0).voices.at(0).notes.at(0).noteAttachmentData.marks.at(0);
    CHECK(handbellMark.markType == MarkType::handbell);
    CHECK_EQUAL("handbellsDamp3", handbellMark.name);
}

T_END;

// Exposes the missing write path: hole, arrow, and handbell are classified as
// isMarkTechnical but NotationsWriter::addTechnical has no branches for them,
// so they silently emit the wrong default element on round-trip. PR $146 Fixup
TEST(technical_hole_arrow_handbell_roundtrip, NoteData)
{
    auto makeScore = [](MarkType markType) {
        ScoreData score;
        score.parts.emplace_back();
        score.ticksPerQuarter = 96;
        auto &part = score.parts.back();
        part.measures.emplace_back();
        auto &measure = part.measures.back();
        measure.staves.emplace_back();
        auto &staff = measure.staves.back();
        auto &voice = staff.voices[0];
        NoteData note;
        note.durationData.durationName = DurationName::quarter;
        note.durationData.durationTimeTicks = 96;
        note.noteAttachmentData.marks.emplace_back(markType);
        voice.notes.push_back(std::move(note));
        return score;
    };

    auto &mgr = DocumentManager::getInstance();

    for (const auto markType : {MarkType::hole, MarkType::arrow, MarkType::handbell})
    {
        const auto r1 = mgr.createFromScore(makeScore(markType));
        REQUIRE(r1.ok());
        auto docId = r1.value();
        std::stringstream ss;
        mgr.writeToStream(docId, ss);
        mgr.destroyDocument(docId);

        std::istringstream iss{ss.str()};
        const auto r2 = mgr.createFromStream(iss);
        REQUIRE(r2.ok());
        docId = r2.value();
        const auto rd = mgr.getData(docId);
        REQUIRE(rd.ok());
        const auto outScore = rd.value();
        mgr.destroyDocument(docId);

        const auto &outMarks = outScore.parts.back()
                                   .measures.back()
                                   .staves.back()
                                   .voices.begin()
                                   ->second.notes.back()
                                   .noteAttachmentData.marks;
        CHECK_EQUAL(1, outMarks.size());
        CHECK(outMarks.front().markType == markType);
    }
}

T_END;

TEST(words, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &directions = staff.directions;

    DirectionData direction;
    WordsData words;
    words.positionData.isDefaultXSpecified = true;
    words.positionData.defaultX = 1.1;
    words.text = "Hello";
    direction.words.push_back(words);
    directions.push_back(direction);

    direction = DirectionData{};
    words = WordsData{};
    words.fontData.style = FontStyle::italic;
    words.text = "One";
    direction.words.push_back(words);
    words = WordsData{};
    words.isColorSpecified = true;
    words.text = "Two";
    direction.words.push_back(words);
    directions.push_back(direction);

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    const auto &opart = oscore.parts.back();
    const auto &omeasure = opart.measures.back();
    const auto &ostaff = omeasure.staves.back();
    const auto &odirections = ostaff.directions;
    CHECK_EQUAL(2, odirections.size());
    const auto &firstDirection = odirections.front();
    const auto &firstWordVec = firstDirection.words;
    CHECK_EQUAL(1, firstWordVec.size());
    const auto &firstWords = firstWordVec.front();
    CHECK_EQUAL("Hello", firstWords.text);
    CHECK(firstWords.positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(1.1, firstWords.positionData.defaultX, 0.0001);

    const auto &secondDirection = odirections.at(1);
    CHECK_EQUAL(2, secondDirection.words.size());
    const auto &wordsOne = secondDirection.words.at(0);
    const auto &wordsTwo = secondDirection.words.at(1);

    CHECK_EQUAL("One", wordsOne.text);
    CHECK(wordsOne.fontData.style == mx::api::FontStyle::italic);

    CHECK_EQUAL("Two", wordsTwo.text);

    // TODO - oops bug opened for this
    //    CHECK( wordsTwo.isColorSpecified );
}

T_END;

TEST(tremolos, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();
    auto &attachments = note.noteAttachmentData;
    auto &marks = attachments.marks;

    MarkData mark{MarkType::tremoloSingleOne};
    marks.emplace_back(mark);
    mark = MarkData{MarkType::tremoloSingleTwo};
    marks.emplace_back(mark);
    mark = MarkData{MarkType::tremoloSingleThree};
    marks.emplace_back(mark);
    mark = MarkData{MarkType::tremoloSingleFour};
    marks.emplace_back(mark);
    mark = MarkData{MarkType::tremoloSingleFive};
    marks.emplace_back(mark);

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    const auto &opart = oscore.parts.back();
    const auto &omeasure = opart.measures.back();
    const auto &ostaff = omeasure.staves.back();
    const auto &ovoice = ostaff.voices.at(0);
    const auto &onote = ovoice.notes.back();
    const auto &omarks = onote.noteAttachmentData.marks;

    for (int i = 1; i <= 5; ++i)
    {
        const auto &markData = omarks.at(static_cast<size_t>(i - 1));
        CHECK_EQUAL(i, numTremoloSlashes(markData.markType));
    }
}

T_END;

TEST(measuredTremolo, NoteData)
{
    ScoreData score;
    score.ticksPerQuarter = 1;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    voice.notes.emplace_back();
    auto &firstNote = voice.notes.back();
    firstNote.durationData.durationName = DurationName::quarter;
    firstNote.durationData.durationTimeTicks = 1;
    firstNote.tickTimePosition = 0;
    MarkData startMark{MarkType::tremoloStart};
    startMark.choice = TremoloMarkData{3};
    firstNote.noteAttachmentData.marks.emplace_back(startMark);

    voice.notes.emplace_back();
    auto &secondNote = voice.notes.back();
    secondNote.durationData.durationName = DurationName::quarter;
    secondNote.durationData.durationTimeTicks = 1;
    secondNote.tickTimePosition = 1;
    MarkData stopMark{MarkType::tremoloStop};
    stopMark.choice = TremoloMarkData{3};
    secondNote.noteAttachmentData.marks.emplace_back(stopMark);

    const auto out = mxtest::roundTrip(score);

    const auto &ovoice = out.parts.back().measures.back().staves.back().voices.at(0);
    REQUIRE(2 == ovoice.notes.size());

    const auto &oFirstMarks = ovoice.notes.at(0).noteAttachmentData.marks;
    REQUIRE(1 == oFirstMarks.size());
    CHECK(MarkType::tremoloStart == oFirstMarks.at(0).markType);
    REQUIRE(oFirstMarks.at(0).choice.isTremolo());
    REQUIRE(oFirstMarks.at(0).choice.tremolo().tremoloMarks.has_value());
    CHECK_EQUAL(3, *oFirstMarks.at(0).choice.tremolo().tremoloMarks);

    const auto &oSecondMarks = ovoice.notes.at(1).noteAttachmentData.marks;
    REQUIRE(1 == oSecondMarks.size());
    CHECK(MarkType::tremoloStop == oSecondMarks.at(0).markType);
    REQUIRE(oSecondMarks.at(0).choice.isTremolo());
    REQUIRE(oSecondMarks.at(0).choice.tremolo().tremoloMarks.has_value());
    CHECK_EQUAL(3, *oSecondMarks.at(0).choice.tremolo().tremoloMarks);
}

T_END;

// Parse the synthetic tremolo file (a <tremolo type="start"> with one slash) and confirm mx::api
// surfaces it as a tremoloStart mark. This pins the core -> api read path for measured tremolos.
TEST(measuredTremoloFromSyntheticFile, NoteData)
{
    const std::string path = mxtest::getResourcesDirectoryPath() + "synthetic/tremolo.3.0.xml";
    auto &docMgr = DocumentManager::getInstance();
    const auto docIdResult = docMgr.createFromFile(path);
    REQUIRE(docIdResult.ok());
    const int docId = docIdResult.value();
    const auto scoreResult = docMgr.getData(docId);
    docMgr.destroyDocument(docId);
    REQUIRE(scoreResult.ok());
    const auto &score = scoreResult.value();

    const auto &marks =
        score.parts.back().measures.back().staves.back().voices.at(0).notes.back().noteAttachmentData.marks;
    REQUIRE(1 == marks.size());
    CHECK(MarkType::tremoloStart == marks.at(0).markType);
    REQUIRE(marks.at(0).choice.isTremolo());
    REQUIRE(marks.at(0).choice.tremolo().tremoloMarks.has_value());
    CHECK_EQUAL(1, *marks.at(0).choice.tremolo().tremoloMarks);
}

T_END;

TEST(miscFields, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();
    note.miscData.push_back("Hello,There");
    note.miscData.push_back("Bones");
    note.miscData.push_back("Bishop");

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    auto &opart = oscore.parts.back();
    auto &omeasure = opart.measures.back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];
    auto &onote = ovoice.notes.back();
    auto &omisc = onote.miscData;
    auto iter = omisc.cbegin();
    const auto end = omisc.cend();

    // assert
    CHECK(iter != end);
    CHECK_EQUAL("Hello_There", *iter);
    ++iter;

    CHECK(iter != end);
    CHECK_EQUAL("Bones", *iter);
    ++iter;

    CHECK(iter != end);
    CHECK_EQUAL("Bishop", *iter);
    ++iter;

    CHECK(iter == end);
}

T_END;

// SlurTieNumberLevel tests: build a score with a note that has a tie/slur using
// mx::api directly, round-trip it, then check that the number is unspecified (absent).

TEST(SlurTieNumberLevelA, NoteData)
{
    // Build a score with a note that has a tie start (no number level specified)
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 1;
    CurveStart curveStart{CurveType::tie};
    note.noteAttachmentData.curveStarts.push_back(curveStart);
    voice.notes.push_back(note);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    const int id = r1.value();
    std::stringstream ss;
    mgr.writeToStream(id, ss);
    mgr.destroyDocument(id);
    std::istringstream iss{ss.str()};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    const int id2 = r2.value();
    const auto rd = mgr.getData(id2);
    REQUIRE(rd.ok());
    const auto scoreData = rd.value();
    mgr.destroyDocument(id2);

    const auto &noteData = scoreData.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.front();
    const auto &cs = noteData.noteAttachmentData.curveStarts.front();
    CHECK(cs.number.isUnspecified());
    CHECK(cs.curveType == mx::api::CurveType::tie);
}

T_END;

TEST(SlurTieNumberLevelB, NoteData)
{
    // Build a score with a note that has a tie continuation (no number level specified)
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 1;
    CurveContinue curveContinue{CurveType::tie};
    note.noteAttachmentData.curveContinuations.push_back(curveContinue);
    voice.notes.push_back(note);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    const int id = r1.value();
    std::stringstream ss;
    mgr.writeToStream(id, ss);
    mgr.destroyDocument(id);
    std::istringstream iss{ss.str()};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    const int id2 = r2.value();
    const auto rd = mgr.getData(id2);
    REQUIRE(rd.ok());
    const auto scoreData = rd.value();
    mgr.destroyDocument(id2);

    const auto &noteData = scoreData.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.front();
    const auto &cc = noteData.noteAttachmentData.curveContinuations.front();
    CHECK(cc.number.isUnspecified());
    CHECK(cc.curveType == mx::api::CurveType::tie);
}

T_END;

TEST(SlurTieNumberLevelC, NoteData)
{
    // Build a score with a note that has a tie stop (no number level specified)
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 1;
    CurveStop curveStop{CurveType::tie};
    note.noteAttachmentData.curveStops.push_back(curveStop);
    voice.notes.push_back(note);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    const int id = r1.value();
    std::stringstream ss;
    mgr.writeToStream(id, ss);
    mgr.destroyDocument(id);
    std::istringstream iss{ss.str()};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    const int id2 = r2.value();
    const auto rd = mgr.getData(id2);
    REQUIRE(rd.ok());
    const auto scoreData = rd.value();
    mgr.destroyDocument(id2);

    const auto &noteData = scoreData.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.front();
    const auto &cs = noteData.noteAttachmentData.curveStops.front();
    CHECK(cs.number.isUnspecified());
    CHECK(cs.curveType == mx::api::CurveType::tie);
}

T_END;

TEST(ornaments, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();

    note.noteAttachmentData.marks.emplace_back(Placement::above, MarkType::trillMark);
    note.noteAttachmentData.marks.back().positionData.isDefaultXSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultX = 123.0;

    note.noteAttachmentData.marks.emplace_back(Placement::above, MarkType::wavyLine);
    note.noteAttachmentData.marks.back().positionData.isDefaultYSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultY = -456.0;

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    auto &opart = oscore.parts.back();
    auto &omeasure = opart.measures.back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];
    auto &onote = ovoice.notes.back();
    auto &oattachments = onote.noteAttachmentData;
    auto &omarks = oattachments.marks;
    auto oIter = omarks.cbegin();

    auto md = *oIter;
    CHECK(md.markType == MarkType::trillMark);
    CHECK(md.positionData.isDefaultXSpecified);
    CHECK(!md.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(123.0, md.positionData.defaultX, 0.00001);

    ++oIter;
    md = *oIter;
    CHECK(md.markType == MarkType::wavyLine);
    CHECK(!md.positionData.isDefaultXSpecified);
    CHECK(md.positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(-456.0, md.positionData.defaultY, 0.00001);
}

T_END;

TEST(pedalStart, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();

    staff.directions.emplace_back();
    auto &direction = staff.directions.back();
    direction.marks.emplace_back(Placement::below, MarkType::pedal);
    direction.tickTimePosition = 7;

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    auto &opart = oscore.parts.back();
    auto &omeasure = opart.measures.back();
    auto &ostaff = omeasure.staves.back();
    auto &odirections = ostaff.directions;
    auto &odirection = odirections.back();
    auto &omark = odirection.marks.back();

    CHECK(omark.markType == MarkType::pedal);
    CHECK_EQUAL(odirection.tickTimePosition, odirection.tickTimePosition);
}

T_END;

TEST(pedalStop, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();

    staff.directions.emplace_back();
    auto &direction = staff.directions.back();
    direction.marks.emplace_back(Placement::below, MarkType::damp);
    direction.tickTimePosition = 70342;

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();

    // get the data after the round trip
    auto &opart = oscore.parts.back();
    auto &omeasure = opart.measures.back();
    auto &ostaff = omeasure.staves.back();
    auto &odirections = ostaff.directions;
    auto &odirection = odirections.back();
    auto &omark = odirection.marks.back();

    CHECK(omark.markType == MarkType::damp);
    CHECK_EQUAL(odirection.tickTimePosition, odirection.tickTimePosition);
}

T_END;

TEST(directionOrder, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 120;
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 120;
    note.tickTimePosition = 0;
    voice.notes.push_back(note);

    note.tickTimePosition = 120;
    note.pitchData.step = Step::d;
    voice.notes.push_back(note);

    note.tickTimePosition = 240;
    note.pitchData.step = Step::e;
    voice.notes.push_back(note);

    note.tickTimePosition = 360;
    note.pitchData.step = Step::e;
    voice.notes.push_back(note);

    DirectionData direction;
    direction.tickTimePosition = 0;
    MarkData mark{Placement::below, MarkType::damp};
    mark.tickTimePosition = direction.tickTimePosition;
    direction.marks.push_back(mark);
    staff.directions.push_back(direction);

    direction.tickTimePosition = 120;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back(direction);

    direction.tickTimePosition = 240;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back(direction);

    direction.tickTimePosition = 360;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back(direction);

    direction.tickTimePosition = 480;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back(direction);

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    const int docId = r1.value();
    auto docPtr = mgr.getDocument(docId);
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);

    REQUIRE(docPtr != nullptr);
    REQUIRE(docPtr->isScorePartwise());
    const auto &partwise = docPtr->asScorePartwise();
    const auto partwiseParts = partwise.part();
    REQUIRE(!partwiseParts.empty());
    const auto &partwisePart = partwiseParts[0];
    const auto partwiseMeasures = partwisePart.measure();
    REQUIRE(!partwiseMeasures.empty());
    const auto &partwiseMeasure = partwiseMeasures[0];
    const auto mdcSpan = partwiseMeasure.musicData();

    size_t idx = 0;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isAttributes());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isDirection());
    CHECK(!mdcSpan[idx].asDirection().offset().has_value());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isNote());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isDirection());
    CHECK(!mdcSpan[idx].asDirection().offset().has_value());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isNote());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isDirection());
    CHECK(!mdcSpan[idx].asDirection().offset().has_value());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isNote());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isDirection());
    CHECK(!mdcSpan[idx].asDirection().offset().has_value());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isNote());

    ++idx;
    REQUIRE(mdcSpan.size() > idx);
    CHECK(mdcSpan[idx].isDirection());
    CHECK(!mdcSpan[idx].asDirection().offset().has_value());
}

T_END;

TEST(directionOrderRoundTrip, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 120;
    auto &part = score.parts.back();
    part.uniqueId = "BONES";
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData note;
    note.userRequestedVoiceNumber = 1;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 120;
    note.tickTimePosition = 0;
    voice.notes.push_back(note);

    note.tickTimePosition = 120;
    note.pitchData.step = Step::d;
    voice.notes.push_back(note);

    note.tickTimePosition = 240;
    note.pitchData.step = Step::e;
    voice.notes.push_back(note);

    note.tickTimePosition = 360;
    note.pitchData.step = Step::e;
    voice.notes.push_back(note);

    auto placement = Placement::above;
    DirectionData direction;
    direction.placement = placement;
    MarkData mark{direction.placement, MarkType::damp};
    direction.tickTimePosition = 0;
    mark.tickTimePosition = direction.tickTimePosition;
    direction.marks.push_back(mark);
    direction.orderedComponents.emplace_back(DirectionComponentKind::mark, 0);
    staff.directions.push_back(direction);

    placement = Placement::unspecified;
    direction.placement = placement;
    direction.marks.back().positionData.placement = placement;
    direction.marks.back().positionData.isDefaultXSpecified = false;
    direction.marks.back().positionData.defaultX = 0.0;
    direction.marks.back().positionData.isDefaultYSpecified = true;
    direction.marks.back().positionData.defaultY = 1.1;
    direction.tickTimePosition = 120;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back(direction);

    placement = Placement::unspecified;
    direction.placement = placement;
    direction.marks.back().positionData.placement = placement;
    direction.marks.back().positionData.isDefaultXSpecified = true;
    direction.marks.back().positionData.defaultX = 2.2;
    direction.marks.back().positionData.isDefaultYSpecified = false;
    direction.marks.back().positionData.defaultY = 0.0;
    direction.tickTimePosition = 240;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back(direction);

    placement = Placement::unspecified;
    direction.placement = placement;
    direction.marks.back().positionData.placement = placement;
    direction.marks.back().positionData.isDefaultXSpecified = true;
    direction.marks.back().positionData.defaultX = 3.3;
    direction.marks.back().positionData.isDefaultYSpecified = true;
    direction.marks.back().positionData.defaultY = 3.3;
    direction.tickTimePosition = 360;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back(direction);

    placement = Placement::unspecified;
    direction.placement = placement;
    direction.marks.back().positionData.isDefaultXSpecified = false;
    direction.marks.back().positionData.defaultX = 0.0;
    direction.marks.back().positionData.isDefaultYSpecified = false;
    direction.marks.back().positionData.defaultY = 0.0;
    direction.marks.back().positionData.placement = placement;
    direction.tickTimePosition = 480;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back(direction);

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();
    // The write side always emits version="4.0"; normalize so version fields
    // do not cause a false mismatch when the original score was built from scratch.
    score.musicXmlVersion = oscore.musicXmlVersion;
    score.declaredMusicXmlVersion = oscore.declaredMusicXmlVersion;
    CHECK(score == oscore);
}

T_END;

TEST(notePositionRoundTrip, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 120;
    auto &part = score.parts.back();
    part.uniqueId = "BISH";
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData note;
    note.userRequestedVoiceNumber = 1;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 120;
    note.tickTimePosition = 0;
    note.positionData.isDefaultXSpecified = true;
    note.positionData.defaultX = 1.0;
    note.positionData.isDefaultYSpecified = true;
    note.positionData.defaultY = -2.0;
    voice.notes.push_back(note);

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    auto oscore = rd.value();
    // The write side always emits version="4.0"; normalize so version fields
    // do not cause a false mismatch when the original score was built from scratch.
    score.musicXmlVersion = oscore.musicXmlVersion;
    score.declaredMusicXmlVersion = oscore.declaredMusicXmlVersion;
    CHECK(score == oscore);
}

T_END;

TEST(noteheadFaUpRoundtrip, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 96;
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 96;
    note.notehead = Notehead::faUp;
    voice.notes.push_back(note);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();

    CHECK(xml.find("fa up") != std::string::npos);

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    const auto outScore = rd.value();
    mgr.destroyDocument(docId);

    const auto &outNote = outScore.parts.back().measures.back().staves.back().voices.at(0).notes.back();
    CHECK(outNote.notehead == Notehead::faUp);
}

T_END;

// #183 - notehead circled (MusicXML 4.0) and the 'other' catch-all were
// missing from noteheadMap, so they read back as Notehead::none (data loss).
TEST(noteheadCircledRoundtrip, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 96;
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 96;
    note.notehead = Notehead::circled;
    voice.notes.push_back(note);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    const auto outScore = rd.value();
    mgr.destroyDocument(docId);

    const auto &outNote = outScore.parts.back().measures.back().staves.back().voices.at(0).notes.back();
    CHECK(outNote.notehead == Notehead::circled);
}

T_END;

TEST(noteheadOtherRoundtrip, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 96;
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 96;
    note.notehead = Notehead::other;
    voice.notes.push_back(note);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    const auto outScore = rd.value();
    mgr.destroyDocument(docId);

    const auto &outNote = outScore.parts.back().measures.back().staves.back().voices.at(0).notes.back();
    CHECK(outNote.notehead == Notehead::other);
}

TEST(printObjectNo, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData note;
    note.isRest = true;
    note.isMeasureRest = true;
    note.durationData.durationTimeTicks = score.ticksPerQuarter * 4;
    note.printData.printObject = Bool::no;
    voice.notes.push_back(note);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    CHECK(xml.find(R"(print-object="no")") != std::string::npos);

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    const auto outScore = rd.value();
    mgr.destroyDocument(docId);

    const auto &outNote = outScore.parts.back().measures.back().staves.back().voices.at(0).notes.back();
    CHECK(outNote.printData.printObject == Bool::no);
}

T_END;

TEST(strongAccentDirection, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];

    NoteData upNote;
    upNote.tickTimePosition = 0;
    upNote.durationData.durationTimeTicks = score.ticksPerQuarter;
    upNote.durationData.durationName = DurationName::quarter;
    upNote.noteAttachmentData.marks.emplace_back(MarkType::strongAccentUp);
    voice.notes.push_back(upNote);

    NoteData downNote;
    downNote.tickTimePosition = score.ticksPerQuarter;
    downNote.durationData.durationTimeTicks = score.ticksPerQuarter;
    downNote.durationData.durationName = DurationName::quarter;
    downNote.noteAttachmentData.marks.emplace_back(MarkType::strongAccentDown);
    voice.notes.push_back(downNote);

    NoteData plainNote;
    plainNote.tickTimePosition = score.ticksPerQuarter * 2;
    plainNote.durationData.durationTimeTicks = score.ticksPerQuarter;
    plainNote.durationData.durationName = DurationName::quarter;
    plainNote.noteAttachmentData.marks.emplace_back(MarkType::strongAccent);
    voice.notes.push_back(plainNote);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    REQUIRE(r1.ok());
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    CHECK(xml.find(R"(<strong-accent type="up" />)") != std::string::npos);
    CHECK(xml.find(R"(<strong-accent type="down" />)") != std::string::npos);
    CHECK(xml.find("<strong-accent />") != std::string::npos);

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    REQUIRE(r2.ok());
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    REQUIRE(rd.ok());
    const auto outScore = rd.value();
    mgr.destroyDocument(docId);

    const auto &outNotes = outScore.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(outNotes.size() == static_cast<size_t>(3));
    CHECK(outNotes.at(0).noteAttachmentData.marks.at(0).markType == MarkType::strongAccentUp);
    CHECK(outNotes.at(1).noteAttachmentData.marks.at(0).markType == MarkType::strongAccentDown);
    CHECK(outNotes.at(2).noteAttachmentData.marks.at(0).markType == MarkType::strongAccent);
}

T_END;

TEST(explicitStaffOnSingleStaffPartRoundTrips, NoteData)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>x</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <note>
        <pitch><step>C</step><octave>4</octave></pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = fromXml(xml);
    const auto &notes = score.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(notes.size() == 1);
    CHECK(notes.at(0).writeStaffNumber == Bool::yes);

    const auto outXml = toXml(score);
    CHECK(outXml.find("<staff>1</staff>") != std::string::npos);
}

T_END;

TEST(omittedStaffOnMultiStaffPartRoundTrips, NoteData)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>x</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <staves>2</staves>
      </attributes>
      <note>
        <pitch><step>C</step><octave>5</octave></pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
      <backup><duration>1</duration></backup>
      <note>
        <pitch><step>C</step><octave>3</octave></pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = fromXml(xml);
    const auto &staves = score.parts.back().measures.back().staves;
    REQUIRE(staves.size() == 2);
    const auto &firstStaffNotes = staves.at(0).voices.at(0).notes;
    const auto &secondStaffNotes = staves.at(1).voices.at(0).notes;
    REQUIRE(firstStaffNotes.size() == 1);
    REQUIRE(secondStaffNotes.size() == 1);
    CHECK(firstStaffNotes.at(0).writeStaffNumber == Bool::no);
    CHECK(secondStaffNotes.at(0).writeStaffNumber == Bool::unspecified);

    const auto outXml = toXml(score);
    CHECK(outXml.find("<staff>1</staff>") == std::string::npos);
    CHECK(outXml.find("<staff>2</staff>") != std::string::npos);
}

T_END;

TEST(writeStaffNumberNoIsIgnoredOffFirstStaff, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    measure.staves.emplace_back();

    NoteData note;
    note.tickTimePosition = 0;
    note.durationData.durationTimeTicks = score.ticksPerQuarter;
    note.durationData.durationName = DurationName::quarter;
    note.writeStaffNumber = Bool::no;
    measure.staves.at(1).voices[0].notes.push_back(note);

    const auto xml = toXml(score);
    CHECK(xml.find("<staff>2</staff>") != std::string::npos);
}

T_END;

TEST(implicitStaffOnSingleStaffPartOmitsElement, NoteData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &voice = measure.staves.back().voices[0];

    NoteData note;
    note.tickTimePosition = 0;
    note.durationData.durationTimeTicks = score.ticksPerQuarter;
    note.durationData.durationName = DurationName::quarter;
    voice.notes.push_back(note);

    const auto xml = toXml(score);
    CHECK(xml.find("<staff>") == std::string::npos);
}

T_END;

#endif
