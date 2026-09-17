// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Import repairs reported through a ParseContext. The lenient parsers still
// produce the same document; the diagnostics say what was changed and where.

#include "cpul/cpulTestHarness.h"

#include "mx/core/ParseContext.h"
#include "mx/core/generated/Document.h"

#include "pugixml.hpp"

#include <string>
#include <utility>
#include <vector>

using namespace mx::core;

struct ImportDiagnosticsParse
{
    bool ok = false;
    Document document;
    std::vector<Diagnostic> diagnostics;
};

inline std::string importDiagnosticsScore(const std::string &defaults, const std::string &measureContent)
{
    return "<score-partwise version=\"4.0\">" + defaults +
           "<part-list><score-part id=\"P1\"><part-name>Music</part-name></score-part></part-list>"
           "<part id=\"P1\"><measure number=\"1\">" +
           measureContent + "</measure></part></score-partwise>";
}

inline std::string importDiagnosticsNote(const std::string &octave, const std::string &type = "")
{
    std::string note = "<note><pitch><step>C</step><octave>" + octave + "</octave></pitch><duration>1</duration>";
    if (!type.empty())
    {
        note += "<type>" + type + "</type>";
    }
    return note + "</note>";
}

inline ImportDiagnosticsParse importDiagnosticsParse(const std::string &xml)
{
    ImportDiagnosticsParse out;
    pugi::xml_document doc;
    CHECK(doc.load_string(xml.c_str()));
    const ParseContext context{[&out](const Diagnostic &diagnostic) { out.diagnostics.push_back(diagnostic); }};
    auto result = parse(doc, context);
    out.ok = result.ok();
    if (result.ok())
    {
        out.document = std::move(result).value();
    }
    return out;
}

TEST(UnknownEnumLiteralFallsBackToFirstVariant, ImportDiagnostics)
{
    const auto parsed = importDiagnosticsParse(importDiagnosticsScore("", importDiagnosticsNote("4", "banana")));
    REQUIRE(parsed.ok);
    REQUIRE(parsed.diagnostics.size() == 1);
    const Diagnostic &diagnostic = parsed.diagnostics.front();
    CHECK(DiagnosticCode::invalidValue == diagnostic.code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note/type"}, diagnostic.path);
    CHECK_EQUAL(std::string{"invalid value \"banana\"; using \"1024th\""}, diagnostic.message);
}

TEST(MalformedAndOutOfRangeNumbersReportDifferentCodes, ImportDiagnostics)
{
    const auto parsed =
        importDiagnosticsParse(importDiagnosticsScore("", importDiagnosticsNote("high") + importDiagnosticsNote("12")));
    REQUIRE(parsed.ok);
    REQUIRE(parsed.diagnostics.size() == 2);

    CHECK(DiagnosticCode::invalidValue == parsed.diagnostics[0].code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note[1]/pitch/octave"}, parsed.diagnostics[0].path);
    CHECK_EQUAL(std::string{"invalid value \"high\"; using \"0\""}, parsed.diagnostics[0].message);

    CHECK(DiagnosticCode::valueAdjusted == parsed.diagnostics[1].code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note[2]/pitch/octave"}, parsed.diagnostics[1].path);
    CHECK_EQUAL(std::string{"value \"12\" adjusted to \"9\""}, parsed.diagnostics[1].message);
}

TEST(InvalidAttributeValueNamesTheAttribute, ImportDiagnostics)
{
    const auto parsed = importDiagnosticsParse(
        importDiagnosticsScore("", "<note default-x=\"left\"><rest/><duration>1</duration></note>"));
    REQUIRE(parsed.ok);
    REQUIRE(parsed.diagnostics.size() == 1);
    const Diagnostic &diagnostic = parsed.diagnostics.front();
    CHECK(DiagnosticCode::invalidValue == diagnostic.code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note"}, diagnostic.path);
    CHECK_EQUAL(std::string{"invalid value \"left\" in attribute \"default-x\"; using \"0\""}, diagnostic.message);
}

TEST(MissingLyricLanguageIsDefaultedAndReported, ImportDiagnostics)
{
    const auto parsed = importDiagnosticsParse(importDiagnosticsScore("<defaults><lyric-language/></defaults>", ""));
    REQUIRE(parsed.ok);
    const auto &defaults = parsed.document.asScorePartwise().scoreHeader().defaults();
    REQUIRE(defaults.has_value());
    REQUIRE(defaults->lyricLanguage().size() == 1);
    CHECK_EQUAL(std::string{"und"}, defaults->lyricLanguage().front().xmlLang());

    REQUIRE(parsed.diagnostics.size() == 1);
    const Diagnostic &diagnostic = parsed.diagnostics.front();
    CHECK(DiagnosticCode::missingValueDefaulted == diagnostic.code);
    CHECK_EQUAL(std::string{"/score-partwise/defaults/lyric-language"}, diagnostic.path);
    CHECK_EQUAL(std::string{"missing attribute \"xml:lang\"; using \"und\""}, diagnostic.message);
}

TEST(RepairsAccumulateInDocumentOrder, ImportDiagnostics)
{
    const auto parsed = importDiagnosticsParse(importDiagnosticsScore(
        "<defaults><lyric-language/></defaults>",
        importDiagnosticsNote("12", "banana") + "<note default-x=\"left\"><rest/><duration>1</duration></note>"));
    REQUIRE(parsed.ok);
    REQUIRE(parsed.diagnostics.size() == 4);
    CHECK(DiagnosticCode::missingValueDefaulted == parsed.diagnostics[0].code);
    CHECK(DiagnosticCode::valueAdjusted == parsed.diagnostics[1].code);
    CHECK(DiagnosticCode::invalidValue == parsed.diagnostics[2].code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note[1]/type"}, parsed.diagnostics[2].path);
    CHECK(DiagnosticCode::invalidValue == parsed.diagnostics[3].code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note[2]"}, parsed.diagnostics[3].path);
}

TEST(RepairedIdWarnsThatItMayDuplicateAnother, ImportDiagnostics)
{
    const auto parsed =
        importDiagnosticsParse(importDiagnosticsScore("", "<note id=\"1\"><rest/><duration>1</duration></note>"));
    REQUIRE(parsed.ok);
    REQUIRE(parsed.diagnostics.size() == 1);
    const Diagnostic &diagnostic = parsed.diagnostics.front();
    CHECK(DiagnosticCode::invalidValue == diagnostic.code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note"}, diagnostic.path);
    CHECK_EQUAL(std::string{"invalid value \"1\" in attribute \"id\"; using \"X\"; "
                            "the repaired ID may duplicate another ID in the document"},
                diagnostic.message);
}

TEST(RepairedReferencesAndNameTokensOmitTheIdWarning, ImportDiagnostics)
{
    const auto parsed = importDiagnosticsParse(
        "<score-partwise version=\"4.0\"><part-list><score-part id=\"P1\"><part-name>Music</part-name></score-part>"
        "</part-list><part id=\"P 1\"><measure number=\"1\"><note><rest/><duration>1</duration>"
        "<lyric number=\"1 2\"><text>la</text></lyric></note></measure></part></score-partwise>");
    REQUIRE(parsed.ok);
    REQUIRE(parsed.diagnostics.size() == 2);
    CHECK(DiagnosticCode::invalidValue == parsed.diagnostics[0].code);
    CHECK_EQUAL(std::string{"/score-partwise/part"}, parsed.diagnostics[0].path);
    CHECK_EQUAL(std::string{"invalid value \"P 1\" in attribute \"id\"; using \"P1\""}, parsed.diagnostics[0].message);
    CHECK(DiagnosticCode::invalidValue == parsed.diagnostics[1].code);
    CHECK_EQUAL(std::string{"/score-partwise/part/measure/note/lyric"}, parsed.diagnostics[1].path);
    CHECK_EQUAL(std::string{"invalid value \"1 2\" in attribute \"number\"; using \"12\""},
                parsed.diagnostics[1].message);
}

TEST(ValidAlternateSpellingsReportNothing, ImportDiagnostics)
{
    const auto parsed = importDiagnosticsParse(importDiagnosticsScore(
        "", "<note id=\" n1 \" default-x=\"+01.50\"><pitch><step>C</step><octave>+04</octave></pitch>"
            "<duration> 1 </duration><type>quarter</type></note>"));
    REQUIRE(parsed.ok);
    CHECK(parsed.diagnostics.empty());
}

TEST(SilentParseProducesTheSameDocument, ImportDiagnostics)
{
    const std::string xml = importDiagnosticsScore("<defaults><lyric-language/></defaults>",
                                                   importDiagnosticsNote("12", "banana") +
                                                       "<note default-x=\"left\"><rest/><duration>1</duration></note>");
    const auto reported = importDiagnosticsParse(xml);
    REQUIRE(reported.ok);
    CHECK(!reported.diagnostics.empty());

    pugi::xml_document doc;
    REQUIRE(doc.load_string(xml.c_str()));
    const auto silent = parse(doc, ParseContext{});
    REQUIRE(silent.ok());
    CHECK(silent.value() == reported.document);
}
