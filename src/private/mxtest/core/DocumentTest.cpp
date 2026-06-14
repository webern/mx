// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Rejection tests: hand-authored invalid documents must refuse to parse,
// each asserting the right ErrorCode and a meaningful path. Plus the
// Document round-trip surface: version gating, root dispatch,
// root-namespace preservation, and mechanical default-construction probes.

#include "cpul/cpulTestHarness.h"

#include "mx/core/Error.h"
#include "mx/core/Lexical.h"
#include "mx/core/generated/DefaultsProbe.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/Version.h"

#include "pugixml/pugixml.hpp"

#include <string>

using namespace mx::core;

namespace
{

Error parseExpectingError(const char *xml)
{
    pugi::xml_document doc;
    CHECK(doc.load_string(xml));
    auto result = parse(doc);
    CHECK(!result.ok());
    if (result.ok())
    {
        return Error{ErrorCode::invalidDocument, {}, "unexpected success"};
    }
    return result.error();
}

constexpr const char *kMinimalScore = "<score-partwise version=\"3.0\">"
                                      "<part-list><score-part id=\"P1\"><part-name>Music</part-name></score-part>"
                                      "</part-list>"
                                      "<part id=\"P1\"><measure number=\"1\"></measure></part>"
                                      "</score-partwise>";

} // namespace

TEST(RejectsDuplicatePitch, Document)
{
    // A note with two <pitch> elements is the cardinal invalid-document
    // probe: the second pitch cannot be consumed by any later field.
    const Error e = parseExpectingError(
        "<score-partwise><part-list><score-part id=\"P1\"><part-name>M</part-name></score-part></part-list>"
        "<part id=\"P1\"><measure number=\"1\"><note>"
        "<pitch><step>C</step><octave>4</octave></pitch>"
        "<pitch><step>D</step><octave>4</octave></pitch>"
        "<duration>1</duration>"
        "</note></measure></part></score-partwise>");
    CHECK(e.code == ErrorCode::wrongElementOrder);
    CHECK(e.path.find("/score-partwise/part/measure/note") == 0);
}

TEST(RejectsOctaveBeforeStep, Document)
{
    const Error e = parseExpectingError(
        "<score-partwise><part-list><score-part id=\"P1\"><part-name>M</part-name></score-part></part-list>"
        "<part id=\"P1\"><measure number=\"1\"><note>"
        "<pitch><octave>4</octave><step>C</step></pitch>"
        "<duration>1</duration>"
        "</note></measure></part></score-partwise>");
    CHECK(e.code == ErrorCode::wrongElementOrder);
    CHECK(e.path.find("pitch") != std::string::npos);
}

TEST(RejectsUnknownElement, Document)
{
    const Error e = parseExpectingError(
        "<score-partwise><part-list><score-part id=\"P1\"><part-name>M</part-name></score-part></part-list>"
        "<part id=\"P1\"><measure number=\"1\"><banana/></measure></part></score-partwise>");
    CHECK(e.code == ErrorCode::unknownElement);
    CHECK(e.path.find("banana") != std::string::npos);
}

TEST(RejectsUnknownAttribute, Document)
{
    const Error e = parseExpectingError("<score-partwise><part-list><score-part id=\"P1\" banana=\"1\">"
                                        "<part-name>M</part-name></score-part></part-list>"
                                        "<part id=\"P1\"><measure number=\"1\"/></part></score-partwise>");
    CHECK(e.code == ErrorCode::unknownAttribute);
    CHECK(e.path.find("score-part") != std::string::npos);
}

TEST(RejectsMissingRequiredChild, Document)
{
    // part-list requires score-part.
    const Error e = parseExpectingError("<score-partwise><part-list></part-list>"
                                        "<part id=\"P1\"><measure number=\"1\"/></part></score-partwise>");
    CHECK(e.code == ErrorCode::missingRequiredElement);
    CHECK(e.path.find("part-list") != std::string::npos);
}

TEST(RejectsMissingRequiredAttribute, Document)
{
    // score-part requires id, and no import default is configured for it.
    const Error e =
        parseExpectingError("<score-partwise><part-list><score-part><part-name>M</part-name></score-part></part-list>"
                            "<part id=\"P1\"><measure number=\"1\"/></part></score-partwise>");
    CHECK(e.code == ErrorCode::missingRequiredAttribute);
    CHECK(e.path.find("score-part") != std::string::npos);
}

TEST(RejectsNewerVersion, Document)
{
    pugi::xml_document doc;
    CHECK(doc.load_string("<score-partwise version=\"5.0\"/>"));
    auto result = parse(doc);
    CHECK(!result.ok());
    CHECK(result.error().code == ErrorCode::unsupportedVersion);
}

TEST(RejectsUnknownRoot, Document)
{
    const Error e = parseExpectingError("<concerto/>");
    CHECK(e.code == ErrorCode::invalidDocument);
}

TEST(VersionGateBoundary, Document)
{
    // Exactly the supported version is fine; only NEWER rejects.
    CHECK(!musicXmlVersionExceeds(SupportedMusicXMLVersion, SupportedMusicXMLVersion));
    CHECK(musicXmlVersionExceeds("4.1", SupportedMusicXMLVersion));
    CHECK(musicXmlVersionExceeds("5.0", SupportedMusicXMLVersion));
    CHECK(!musicXmlVersionExceeds("3.1", SupportedMusicXMLVersion));
}

TEST(RoundTripMinimalScore, Document)
{
    pugi::xml_document doc;
    CHECK(doc.load_string(kMinimalScore));
    auto parsed = parse(doc);
    CHECK(parsed.ok());
    CHECK(parsed.value().isScorePartwise());
    pugi::xml_document out;
    serialize(parsed.value(), out);
    auto reparsed = parse(out);
    CHECK(reparsed.ok());
    CHECK(parsed.value() == reparsed.value());
}

TEST(RootNamespacePreserved, Document)
{
    pugi::xml_document doc;
    CHECK(doc.load_string("<score-partwise xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"3.0\">"
                          "<part-list><score-part id=\"P1\"><part-name>M</part-name></score-part></part-list>"
                          "<part id=\"P1\"><measure number=\"1\"/></part></score-partwise>"));
    auto parsed = parse(doc);
    CHECK(parsed.ok());
    CHECK_EQUAL(std::size_t{1}, parsed.value().rootNamespaces().size());
    pugi::xml_document out;
    serialize(parsed.value(), out);
    const pugi::xml_attribute ns = out.document_element().attribute("xmlns:xlink");
    CHECK_EQUAL(std::string{"http://www.w3.org/1999/xlink"}, std::string{ns.value()});
}

TEST(DefaultsRoundTripEveryType, Document)
{
    // Mechanical probe: every generated type's natural zero must
    // survive its own strict parser. Throws (and fails the case) if not.
    roundTripDefaults();
    CHECK(true);
}
