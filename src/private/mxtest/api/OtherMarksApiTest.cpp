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
    addOtherMark(MarkType::otherDynamics, "", "dynamicZ");

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
    CHECK(smuflFor(MarkType::otherDynamics) == std::optional<std::string>{"dynamicZ"});
    const auto dynamic = std::find_if(outMarks.begin(), outMarks.end(),
                                      [](const auto &item) { return item.markType == MarkType::otherDynamics; });
    REQUIRE(dynamic != outMarks.end());
    CHECK(dynamic->name.empty());
}

T_END;

TEST(markChoiceWrongKindFallbacks, OtherMarksApi)
{
    const MarkDataChoice choice;
    CHECK(!choice.otherMark().smufl.has_value());
    CHECK(choice.compoundDynamics().components.empty());
    CHECK(choice.otherNotation().type == OtherNotationType::single);

    const DynamicsComponent standard{StandardDynamic::ff};
    CHECK(standard.other() == OtherDynamicsData{});
    const DynamicsComponent other{OtherDynamicsData{"z", std::string{"dynamicZ"}}};
    CHECK(other.standard() == StandardDynamic::p);
}

T_END;

TEST(compoundDynamicsRoundTrip, OtherMarksApi)
{
    auto score = otherMarksScoreWithNote();
    auto &mark = otherMarksNote(score).noteAttachmentData.marks.emplace_back(MarkType::compoundDynamics);

    CompoundDynamicsData compound;
    compound.components.emplace_back(StandardDynamic::ff);
    compound.components.emplace_back(OtherDynamicsData{"z", std::string{"dynamicZ"}});
    mark.choice = std::move(compound);

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
    CHECK(outMarks.front().markType == MarkType::compoundDynamics);
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
