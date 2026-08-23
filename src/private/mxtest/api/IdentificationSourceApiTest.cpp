// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

// An otherwise empty one-part score, so the only thing under test is the header.
ScoreData identificationSourceMakeScore()
{
    ScoreData score;
    score.parts.emplace_back();
    score.parts.back().uniqueId = "P1";
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    return score;
}

TEST(sourceSurvivesRoundTrip, IdentificationSource)
{
    auto score = identificationSourceMakeScore();
    score.source = "Bach-Gesellschaft Ausgabe, Band 3";

    const auto out = roundTrip(score);
    REQUIRE(out.source.has_value());
    CHECK_EQUAL("Bach-Gesellschaft Ausgabe, Band 3", *out.source);
}

TEST(sourceWritesTheMusicXmlElement, IdentificationSource)
{
    auto score = identificationSourceMakeScore();
    score.source = "Urtext";

    const auto xml = toXml(score);
    CHECK(xml.find("<source>Urtext</source>") != std::string::npos);
}

TEST(anEmptySourceWritesNothing, IdentificationSource)
{
    const auto score = identificationSourceMakeScore();
    const auto xml = toXml(score);
    CHECK(xml.find("<source>") == std::string::npos);

    const auto out = roundTrip(score);
    CHECK(!out.source.has_value());
}

// MusicXML allows <source></source>, which says the score named a source and left it blank.
// That is a different fact from saying nothing at all, so both have to survive the round trip.
TEST(aBlankSourceIsNotTheSameAsNoSource, IdentificationSource)
{
    auto score = identificationSourceMakeScore();
    score.source = "";

    const auto out = roundTrip(score);
    REQUIRE(out.source.has_value());
    CHECK_EQUAL("", *out.source);
}

#endif
