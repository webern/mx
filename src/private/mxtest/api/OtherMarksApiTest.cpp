// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MarkData.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

#include <algorithm>
#include <string>

using namespace mx::api;

ScoreData otherMarksScoreWithNote()
{
    ScoreData score;
    score.parts.emplace_back();
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    score.parts.back().measures.back().staves.back().voices[0].notes.emplace_back();
    return score;
}

NoteData &otherMarksNote(ScoreData &score)
{
    return score.parts.back().measures.back().staves.back().voices[0].notes.back();
}

TEST(smuflOtherMarksRoundTrip, OtherMarksApi)
{
    auto score = otherMarksScoreWithNote();
    auto &marks = otherMarksNote(score).noteAttachmentData.marks;

    auto addOtherMark = [&](MarkType type, std::string text, std::string smufl) {
        marks.emplace_back(type);
        marks.back().name = std::move(text);
        marks.back().choice = OtherMarkData{std::move(smufl)};
    };

    addOtherMark(MarkType::otherArticulation, "articulation fallback", "articAccentAbove");
    addOtherMark(MarkType::otherTechnical, "technique fallback", "brassMuteClosed");
    addOtherMark(MarkType::otherOrnament, "ornament fallback", "ornamentTurnSlash");

    // An other-dynamics has no dedicated element, so it is a compound of one component.
    marks.emplace_back(CompoundDynamicsData{{OtherDynamicsData{"", std::string{"dynamicZ"}}}});

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("smufl=\"articAccentAbove\"") != std::string::npos);
    CHECK(xml.find("smufl=\"brassMuteClosed\"") != std::string::npos);
    CHECK(xml.find("smufl=\"ornamentTurnSlash\"") != std::string::npos);
    CHECK(xml.find("smufl=\"dynamicZ\"") != std::string::npos);

    const auto out = mxtest::roundTrip(score);
    const auto &outMarks =
        out.parts.back().measures.back().staves.back().voices.at(0).notes.back().noteAttachmentData.marks;
    REQUIRE(outMarks.size() == 4);
    auto smuflFor = [&](MarkType type) {
        const auto it =
            std::find_if(outMarks.begin(), outMarks.end(), [type](const auto &item) { return item.markType == type; });
        return it == outMarks.end() ? std::optional<std::string>{} : it->choice.otherMark().smufl;
    };
    CHECK(smuflFor(MarkType::otherArticulation) == std::optional<std::string>{"articAccentAbove"});
    CHECK(smuflFor(MarkType::otherOrnament) == std::optional<std::string>{"ornamentTurnSlash"});
    CHECK(smuflFor(MarkType::otherTechnical) == std::optional<std::string>{"brassMuteClosed"});

    const auto dynamic = std::find_if(outMarks.begin(), outMarks.end(),
                                      [](const auto &item) { return item.markType == MarkType::dynamics; });
    REQUIRE(dynamic != outMarks.end());
    REQUIRE(dynamic->choice.isCompoundDynamics());
    const auto components = dynamic->choice.compoundDynamics().components;
    REQUIRE(components.size() == 1);
    CHECK(components.front().other().smufl == std::optional<std::string>{"dynamicZ"});
    CHECK(components.front().other().text.empty());
    CHECK(dynamic->name.empty());
}

T_END;

TEST(markChoiceWrongKindFallbacks, OtherMarksApi)
{
    const MarkDataChoice choice;
    CHECK(!choice.otherMark().smufl.has_value());
    CHECK(choice.compoundDynamics().components.empty());
    CHECK(choice.dynamic() == StandardDynamic::p);
    CHECK(choice.otherNotation().type == OtherNotationType::single);

    const DynamicsComponent standard{StandardDynamic::ff};
    CHECK(standard.other() == OtherDynamicsData{});
    const DynamicsComponent other{OtherDynamicsData{"z", std::string{"dynamicZ"}}};
    CHECK(other.standard() == StandardDynamic::p);
}

T_END;

// A lone standard symbol is one mark however it was built, so it is always stored as
// Kind::dynamic. A lone other-dynamics has no dedicated element and stays a compound of one.
TEST(singleStandardDynamicCollapses, OtherMarksApi)
{
    const MarkDataChoice fromCompound{CompoundDynamicsData{{StandardDynamic::ff}}};
    CHECK(fromCompound.kind() == MarkDataChoice::Kind::dynamic);
    CHECK(fromCompound.dynamic() == StandardDynamic::ff);
    CHECK(fromCompound == MarkDataChoice{StandardDynamic::ff});

    const MarkDataChoice loneOther{CompoundDynamicsData{{OtherDynamicsData{"z", std::string{"dynamicZ"}}}}};
    CHECK(loneOther.kind() == MarkDataChoice::Kind::compoundDynamics);
    REQUIRE(loneOther.compoundDynamics().components.size() == 1);

    const MarkDataChoice twoStandard{CompoundDynamicsData{{StandardDynamic::ff, StandardDynamic::p}}};
    CHECK(twoStandard.kind() == MarkDataChoice::Kind::compoundDynamics);

    const MarkData mark{StandardDynamic::ff};
    CHECK(mark.markType == MarkType::dynamics);
    CHECK(mark.choice.dynamic() == StandardDynamic::ff);
}

T_END;

// name spells out the whole mark, the way articulations and fermatas name themselves. The writer
// ignores it -- the symbols that get written are the ones in choice.
TEST(dynamicNameSpellsTheMark, OtherMarksApi)
{
    CHECK_EQUAL("ff", toString(StandardDynamic::ff));
    CHECK_EQUAL("sfzp", toString(StandardDynamic::sfzp));
    CHECK_EQUAL("ff", MarkData{StandardDynamic::ff}.name);
    CHECK_EQUAL("z", toString(DynamicsComponent{OtherDynamicsData{"z", std::string{"dynamicZ"}}}));
    CHECK_EQUAL("ffz",
                toString(CompoundDynamicsData{{StandardDynamic::ff, OtherDynamicsData{"z", std::string{"dynamicZ"}}}}));

    auto score = otherMarksScoreWithNote();
    otherMarksNote(score).noteAttachmentData.marks.emplace_back(StandardDynamic::ffff);

    const auto out = mxtest::roundTrip(score);
    const auto &outMarks =
        out.parts.back().measures.back().staves.back().voices.at(0).notes.back().noteAttachmentData.marks;
    REQUIRE(outMarks.size() == 1);
    CHECK(outMarks.front().choice.dynamic() == StandardDynamic::ffff);
    CHECK_EQUAL("ffff", outMarks.front().name);

    // A lone other-dynamics echoes its text, as it did before the symbol moved into choice.
    auto otherScore = otherMarksScoreWithNote();
    otherMarksNote(otherScore)
        .noteAttachmentData.marks.emplace_back(CompoundDynamicsData{{OtherDynamicsData{"z", std::string{"dynamicZ"}}}});
    const auto otherOut = mxtest::roundTrip(otherScore);
    const auto &otherOutMarks =
        otherOut.parts.back().measures.back().staves.back().voices.at(0).notes.back().noteAttachmentData.marks;
    REQUIRE(otherOutMarks.size() == 1);
    CHECK_EQUAL("z", otherOutMarks.front().name);
    REQUIRE(otherOutMarks.front().choice.isCompoundDynamics());
    CHECK_EQUAL("z", otherOutMarks.front().choice.compoundDynamics().components.front().other().text);

    // The CompoundDynamicsData constructor spells the mark out the same way the reader does, and a
    // lone standard symbol reaches the same mark as the StandardDynamic constructor.
    const MarkData built{CompoundDynamicsData{{StandardDynamic::ff, OtherDynamicsData{"z", std::string{"dynamicZ"}}}}};
    CHECK(built.markType == MarkType::dynamics);
    CHECK_EQUAL("ffz", built.name);
    CHECK(built.choice.isCompoundDynamics());
    CHECK(MarkData{CompoundDynamicsData{{StandardDynamic::ff}}} == MarkData{StandardDynamic::ff});

    // A compound spells out every component in order.
    auto compoundScore = otherMarksScoreWithNote();
    otherMarksNote(compoundScore)
        .noteAttachmentData.marks.emplace_back(
            CompoundDynamicsData{{StandardDynamic::ff, OtherDynamicsData{"z", std::string{"dynamicZ"}}}});
    const auto compoundOut = mxtest::roundTrip(compoundScore);
    const auto &compoundMarks =
        compoundOut.parts.back().measures.back().staves.back().voices.at(0).notes.back().noteAttachmentData.marks;
    REQUIRE(compoundMarks.size() == 1);
    CHECK_EQUAL("ffz", compoundMarks.front().name);
}

T_END;

TEST(compoundDynamicsRoundTrip, OtherMarksApi)
{
    auto score = otherMarksScoreWithNote();
    CompoundDynamicsData compound;
    compound.components.emplace_back(StandardDynamic::ff);
    compound.components.emplace_back(OtherDynamicsData{"z", std::string{"dynamicZ"}});
    otherMarksNote(score).noteAttachmentData.marks.emplace_back(std::move(compound));

    const auto xml = mxtest::toXml(score);
    const auto dynamicsPosition = xml.find("<dynamics");
    const auto ffPosition = xml.find("<ff", dynamicsPosition);
    const auto zPosition = xml.find("<other-dynamics smufl=\"dynamicZ\">z</other-dynamics>", ffPosition);
    REQUIRE(dynamicsPosition != std::string::npos);
    REQUIRE(ffPosition != std::string::npos);
    REQUIRE(zPosition != std::string::npos);
    CHECK(ffPosition < zPosition);

    const auto out = mxtest::roundTrip(score);
    const auto &outMarks =
        out.parts.back().measures.back().staves.back().voices.at(0).notes.back().noteAttachmentData.marks;
    REQUIRE(outMarks.size() == 1);
    CHECK(outMarks.front().markType == MarkType::dynamics);
    REQUIRE(outMarks.front().choice.isCompoundDynamics());
    const auto outCompound = outMarks.front().choice.compoundDynamics();
    REQUIRE(outCompound.components.size() == 2);
    CHECK(outCompound.components.at(0).standard() == StandardDynamic::ff);
    CHECK(outCompound.components.at(1).other().text == "z");
    CHECK(outCompound.components.at(1).other().smufl == std::optional<std::string>{"dynamicZ"});
}

T_END;

TEST(otherNotationRoundTrip, OtherMarksApi)
{
    auto score = otherMarksScoreWithNote();
    auto &mark = otherMarksNote(score).noteAttachmentData.marks.emplace_back(MarkType::otherNotation);
    mark.name = "custom notation";
    mark.positionData.placement = Placement::above;
    mark.printData.printObject = Bool::no;

    OtherNotationMarkData notation;
    notation.type = OtherNotationType::start;
    notation.number = 2;
    notation.smufl = "pluckedSnapPizzicatoAbove";
    notation.id = "notation-id";
    mark.choice = std::move(notation);

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<other-notation") != std::string::npos);
    CHECK(xml.find("type=\"start\"") != std::string::npos);
    CHECK(xml.find("number=\"2\"") != std::string::npos);
    CHECK(xml.find("print-object=\"no\"") != std::string::npos);
    CHECK(xml.find("smufl=\"pluckedSnapPizzicatoAbove\"") != std::string::npos);
    CHECK(xml.find("id=\"notation-id\"") != std::string::npos);

    const auto out = mxtest::roundTrip(score);
    const auto &outMarks =
        out.parts.back().measures.back().staves.back().voices.at(0).notes.back().noteAttachmentData.marks;
    REQUIRE(outMarks.size() == 1);
    CHECK(outMarks.front().markType == MarkType::otherNotation);
    CHECK(outMarks.front().name == "custom notation");
    CHECK(outMarks.front().positionData.placement == Placement::above);
    CHECK(outMarks.front().printData.printObject == Bool::no);
    const auto outNotation = outMarks.front().choice.otherNotation();
    CHECK(outNotation.type == OtherNotationType::start);
    CHECK(outNotation.number == std::optional<int>{2});
    CHECK(outNotation.smufl == std::optional<std::string>{"pluckedSnapPizzicatoAbove"});
    CHECK(outNotation.id == std::optional<std::string>{"notation-id"});
}

T_END;

#endif
