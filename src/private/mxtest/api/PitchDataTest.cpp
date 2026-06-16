// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "pugixml/pugixml.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;
using namespace mx::api;

namespace
{

/// Walk the pugixml tree depth-first to find the first element with the given name.
pugi::xml_node bruteForceFindFirstElement(pugi::xml_node root, const std::string &inElementName)
{
    if (std::string{root.name()} == inElementName)
    {
        return root;
    }

    for (auto child : root.children())
    {
        auto found = bruteForceFindFirstElement(child, inElementName);
        if (!found.empty() && std::string{found.name()} == inElementName)
        {
            return found;
        }
    }

    return pugi::xml_node{};
}

struct Input
{
    Step step;
    int alter;
    double cents;
    Accidental accidental;
};

struct Output
{
    Step step;
    int alter;
    double cents;
    Accidental accidental;
    std::string alterString;
    std::string secondAlterString;
};

Output pitchDataTest(const Input &input)
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
    note.pitchData.step = input.step;
    note.pitchData.accidental = input.accidental;
    note.pitchData.alter = input.alter;
    note.pitchData.cents = input.cents;

    // round trip it through xml
    auto &mgr = DocumentManager::getInstance();
    const auto docIdResult = mgr.createFromScore(score);
    if (!docIdResult.ok())
        return {};
    const int docId = docIdResult.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);

    // check the alter value that was written to xml
    pugi::xml_document xdoc;
    const std::string xml = ss.str();
    xdoc.load_string(xml.c_str());
    auto elem = bruteForceFindFirstElement(xdoc.document_element(), "alter");
    const auto alterString = std::string{elem.text().get()};
    Output output;
    output.alterString = alterString;

    // deserialize back to ScoreData
    std::istringstream iss{xml};
    const auto docId2Result = mgr.createFromStream(iss);
    if (!docId2Result.ok())
        return {};
    const int docId2 = docId2Result.value();
    const auto oscoreResult = mgr.getData(docId2);
    mgr.destroyDocument(docId2);
    if (!oscoreResult.ok())
        return {};
    const auto &oscore = oscoreResult.value();
    const auto &opart = oscore.parts.back();
    const auto &omeasure = opart.measures.back();
    const auto &ostaff = omeasure.staves.back();
    const auto &ovoice = ostaff.voices.at(0);
    const auto &onote = ovoice.notes.back();
    output.step = onote.pitchData.step;
    output.alter = onote.pitchData.alter;
    output.cents = onote.pitchData.cents;
    output.accidental = onote.pitchData.accidental;

    // serialize a second time and check the alter string again
    const auto docId3Result = mgr.createFromScore(score);
    if (!docId3Result.ok())
        return {};
    const int docId3 = docId3Result.value();
    std::stringstream ss2;
    mgr.writeToStream(docId3, ss2);
    mgr.destroyDocument(docId3);

    // check the alter value that was written to xml
    pugi::xml_document xdoc2;
    const std::string xml2 = ss2.str();
    xdoc2.load_string(xml2.c_str());
    auto elem2 = bruteForceFindFirstElement(xdoc2.document_element(), "alter");
    const auto alterString2 = std::string{elem2.text().get()};
    output.secondAlterString = alterString2;
    return output;
}
} // namespace

TEST(ThreeQuarterSharp, PitchData)
{
    auto input = Input{};
    input.step = Step::f;
    input.alter = 1;
    input.cents = 50.0;
    input.accidental = Accidental::threeQuartersSharp;
    const std::string expectedAlterString = "1.5";
    const int expectedAlter = input.alter;
    const double expectedCents = input.cents;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest(input);

    CHECK_EQUAL(expectedAlterString, output.alterString);
    CHECK_EQUAL(expectedAlterString, output.secondAlterString);
    CHECK_EQUAL(expectedAlter, output.alter);
    CHECK_DOUBLES_EQUAL(expectedCents, output.cents, MX_API_EQUALITY_EPSILON);
    CHECK(expectedAccidental == output.accidental);
}

T_END;

TEST(ThreeQuarterFlat, PitchData)
{
    auto input = Input{};
    input.step = Step::b;
    input.alter = -1;
    input.cents = -50.0;
    input.accidental = Accidental::threeQuartersFlat;
    const std::string expectedAlterString = "-1.5";
    const int expectedAlter = input.alter;
    const double expectedCents = input.cents;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest(input);

    CHECK_EQUAL(expectedAlterString, output.alterString);
    CHECK_EQUAL(expectedAlterString, output.secondAlterString);
    CHECK_EQUAL(expectedAlter, output.alter);
    CHECK_DOUBLES_EQUAL(expectedCents, output.cents, MX_API_EQUALITY_EPSILON);
    CHECK(expectedAccidental == output.accidental);
}

T_END;

TEST(AlmostDoubleSharp, PitchData)
{
    auto input = Input{};
    input.step = Step::g;
    input.alter = 1;
    input.cents = 99.999;
    input.accidental = Accidental::doubleSharp;
    const std::string expectedAlterString = "1.99999";
    const int expectedAlter = input.alter;
    const double expectedCents = input.cents;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest(input);

    CHECK_EQUAL(expectedAlterString, output.alterString);
    CHECK_EQUAL(expectedAlterString, output.secondAlterString);
    CHECK_EQUAL(expectedAlter, output.alter);
    CHECK_DOUBLES_EQUAL(expectedCents, output.cents, MX_API_EQUALITY_EPSILON);
    CHECK(expectedAccidental == output.accidental);
}

T_END;

TEST(AlmostDoubleFlat, PitchData)
{
    auto input = Input{};
    input.step = Step::a;
    input.alter = -1;
    input.cents = -99.9999;
    input.accidental = Accidental::flatFlat;
    const std::string expectedAlterString = "-1.999999";
    const int expectedAlter = input.alter;
    const double expectedCents = input.cents;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest(input);

    CHECK_EQUAL(expectedAlterString, output.alterString);
    CHECK_EQUAL(expectedAlterString, output.secondAlterString);
    CHECK_EQUAL(expectedAlter, output.alter);
    CHECK_DOUBLES_EQUAL(expectedCents, output.cents, MX_API_EQUALITY_EPSILON);
    CHECK(expectedAccidental == output.accidental);
}

T_END;

TEST(SharpSharp, PitchData)
{
    // Regression: accidentalMap was missing sharpSharp, so it round-tripped as Accidental::none.
    auto input = Input{};
    input.step = Step::c;
    input.alter = 2;
    input.cents = 0.0;
    input.accidental = Accidental::sharpSharp;
    const std::string expectedAlterString = "2";
    const int expectedAlter = 2;
    const double expectedCents = 0.0;
    const Accidental expectedAccidental = Accidental::sharpSharp;
    const auto output = pitchDataTest(input);

    CHECK_EQUAL(expectedAlterString, output.alterString);
    CHECK_EQUAL(expectedAlterString, output.secondAlterString);
    CHECK_EQUAL(expectedAlter, output.alter);
    CHECK_DOUBLES_EQUAL(expectedCents, output.cents, MX_API_EQUALITY_EPSILON);
    CHECK(expectedAccidental == output.accidental);
}

T_END;

TEST(CrazyEdgeCase1, PitchData)
{
    auto input = Input{};
    input.step = Step::g;
    input.alter = 1;
    input.cents = -123456789;
    input.accidental = Accidental::none;
    const std::string expectedAlterString = "-1234566.89";
    const int expectedAlter = -1234566;
    const double expectedCents = -89.0;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest(input);

    CHECK_EQUAL(expectedAlterString, output.alterString);
    CHECK_EQUAL(expectedAlterString, output.secondAlterString);
    CHECK_EQUAL(expectedAlter, output.alter);
    CHECK_DOUBLES_EQUAL(expectedCents, output.cents, MX_API_EQUALITY_EPSILON * 10);
    CHECK(expectedAccidental == output.accidental);
}

T_END;

TEST(CrazyEdgeCase2, PitchData)
{
    auto input = Input{};
    input.step = Step::e;
    input.alter = 21;
    input.cents = 100.01;
    input.accidental = Accidental::sori;
    const std::string expectedAlterString = "22.0001";
    const int expectedAlter = 22;
    const double expectedCents = 0.01;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest(input);

    CHECK_EQUAL(expectedAlterString, output.alterString);
    CHECK_EQUAL(expectedAlterString, output.secondAlterString);
    CHECK_EQUAL(expectedAlter, output.alter);
    CHECK_DOUBLES_EQUAL(expectedCents, output.cents, MX_API_EQUALITY_EPSILON);
    CHECK(expectedAccidental == output.accidental);
}

T_END;

namespace
{
struct PitchHash
{
    constexpr inline size_t operator()(const mx::api::PitchData &p) const
    {
        size_t h1 = size_t(p.step) << 32;
        size_t h2 = size_t(p.alter);
        return h1 ^ h2;
    }
};
} // namespace

TEST(ConstructorTest, PitchData)
{
    // duplicate the feature request from https://github.com/webern/mx/issues/69
    std::unordered_map<mx::api::PitchData, int, PitchHash> root_to_key_circle{
        {PitchData{Step::f, 0, 4}, -1},
        {PitchData{Step::c, 0, 4}, 0},
        {PitchData{Step::g, 0, 4}, 1},
    };

    PitchData g{Step::g};
    const auto find_iter = root_to_key_circle.find(g);
    REQUIRE(find_iter != std::cend(root_to_key_circle));
    CHECK_EQUAL(1, find_iter->second);
}

T_END;

#endif
