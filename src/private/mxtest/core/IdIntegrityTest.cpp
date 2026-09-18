// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Document-wide ID rules: xs:ID values are unique and xs:IDREF values name
// an xs:ID of the right element. Reading reports; writing renames a
// duplicate so the document we emit is valid.

#include "cpul/cpulTestHarness.h"

#include "mx/core/IdIntegrity.h"

#include "pugixml.hpp"

#include <sstream>
#include <string>
#include <vector>

using namespace mx::core;

inline std::string idIntegrityScore(const std::string &partList, const std::string &parts)
{
    return "<score-partwise version=\"4.0\"><part-list>" + partList + "</part-list>" + parts + "</score-partwise>";
}

inline std::string idIntegrityScorePart(const std::string &id)
{
    return "<score-part id=\"" + id + "\"><part-name>Music</part-name></score-part>";
}

inline std::string idIntegrityInstrumentPart(const std::string &partId, const std::string &instrumentId)
{
    return "<score-part id=\"" + partId + "\"><part-name>Music</part-name><score-instrument id=\"" + instrumentId +
           "\"><instrument-name>x</instrument-name></score-instrument></score-part>";
}

inline std::vector<Diagnostic> idIntegrityCheck(const std::string &xml)
{
    pugi::xml_document doc;
    CHECK(doc.load_string(xml.c_str()));
    std::vector<Diagnostic> out;
    checkIds(doc, [&out](const Diagnostic &diagnostic) { out.push_back(diagnostic); });
    return out;
}

struct IdIntegrityRepaired
{
    std::string xml;
    std::vector<Diagnostic> diagnostics;
};

inline IdIntegrityRepaired idIntegrityRepair(const std::string &xml)
{
    pugi::xml_document doc;
    CHECK(doc.load_string(xml.c_str()));
    IdIntegrityRepaired out;
    repairIds(doc, [&out](const Diagnostic &diagnostic) { out.diagnostics.push_back(diagnostic); });
    std::ostringstream stream;
    doc.save(stream, "", pugi::format_raw);
    out.xml = stream.str();
    return out;
}

inline bool idIntegrityContains(const std::string &haystack, const std::string &needle)
{
    return haystack.find(needle) != std::string::npos;
}

TEST(CleanDocumentReportsNothing, IdIntegrity)
{
    const auto diagnostics = idIntegrityCheck(
        idIntegrityScore(idIntegrityScorePart("P1"), "<part id=\"P1\"><measure number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{0}, diagnostics.size());
}

TEST(ReportsDuplicateId, IdIntegrity)
{
    const auto diagnostics = idIntegrityCheck(idIntegrityScore(idIntegrityScorePart("P1") + idIntegrityScorePart("P1"),
                                                               "<part id=\"P1\"><measure number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{1}, diagnostics.size());
    CHECK(diagnostics.at(0).code == DiagnosticCode::duplicateId);
    CHECK_EQUAL(std::string{"/score-partwise/part-list/score-part[2]"}, diagnostics.at(0).path);
    CHECK(idIntegrityContains(diagnostics.at(0).message, "duplicate ID \"P1\""));
    // The message names the element that got there first.
    CHECK(idIntegrityContains(diagnostics.at(0).message, "score-part[1]"));
}

TEST(ReportsDuplicateAcrossElementKinds, IdIntegrity)
{
    // xs:ID is unique across the document, not within one kind of element.
    const auto diagnostics = idIntegrityCheck(
        idIntegrityScore(idIntegrityScorePart("P1"), "<part id=\"P1\"><measure id=\"P1\" number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{1}, diagnostics.size());
    CHECK(diagnostics.at(0).code == DiagnosticCode::duplicateId);
    CHECK(idIntegrityContains(diagnostics.at(0).path, "measure"));
}

TEST(ReportsCollisionMadeByRepair, IdIntegrity)
{
    // "1P1" is not a legal NCName. Token repair drops the leading digit and
    // lands on an id another element already holds, so two ids that differ
    // in the file are one id in the parsed document.
    const auto diagnostics = idIntegrityCheck(idIntegrityScore(idIntegrityScorePart("P1") + idIntegrityScorePart("1P1"),
                                                               "<part id=\"P1\"><measure number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{1}, diagnostics.size());
    CHECK(diagnostics.at(0).code == DiagnosticCode::duplicateId);
    CHECK(idIntegrityContains(diagnostics.at(0).message, "duplicate ID \"P1\""));
}

TEST(ReportsDanglingReference, IdIntegrity)
{
    const auto diagnostics = idIntegrityCheck(
        idIntegrityScore(idIntegrityScorePart("P1"), "<part id=\"P9\"><measure number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{1}, diagnostics.size());
    CHECK(diagnostics.at(0).code == DiagnosticCode::danglingIdReference);
    CHECK_EQUAL(std::string{"/score-partwise/part"}, diagnostics.at(0).path);
    CHECK(idIntegrityContains(diagnostics.at(0).message, "does not match an ID in the document"));
}

TEST(ReportsReferenceToWrongElement, IdIntegrity)
{
    // The id exists, but a <part> has to name a <score-part>. XSD cannot say
    // that, so the schema says it in prose and we check it here.
    const auto diagnostics = idIntegrityCheck(
        idIntegrityScore(idIntegrityInstrumentPart("P1", "I1"), "<part id=\"I1\"><measure number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{1}, diagnostics.size());
    CHECK(diagnostics.at(0).code == DiagnosticCode::danglingIdReference);
    CHECK(idIntegrityContains(diagnostics.at(0).message, "names a <score-instrument>, not a <score-part>"));
}

TEST(ReferenceToRightElementIsQuiet, IdIntegrity)
{
    const auto diagnostics = idIntegrityCheck(idIntegrityScore(
        idIntegrityInstrumentPart("P1", "I1"), "<part id=\"P1\"><measure number=\"1\">"
                                               "<note><instrument id=\"I1\"/><duration>1</duration></note>"
                                               "</measure></part>"));
    CHECK_EQUAL(std::size_t{0}, diagnostics.size());
}

TEST(ReadingChangesNothing, IdIntegrity)
{
    // Renaming on import would break the parse/serialize round trip, so the
    // duplicate survives the read.
    const std::string xml = idIntegrityScore(idIntegrityScorePart("P1") + idIntegrityScorePart("P1"),
                                             "<part id=\"P1\"><measure number=\"1\"/></part>");
    pugi::xml_document doc;
    CHECK(doc.load_string(xml.c_str()));
    checkIds(doc, [](const Diagnostic &) {});
    const pugi::xml_node first = doc.document_element().child("part-list").child("score-part");
    CHECK_EQUAL(std::string{"P1"}, std::string{first.attribute("id").value()});
    CHECK_EQUAL(std::string{"P1"}, std::string{first.next_sibling("score-part").attribute("id").value()});
}

TEST(WriteRenamesDuplicate, IdIntegrity)
{
    const auto repaired = idIntegrityRepair(idIntegrityScore(idIntegrityScorePart("P1") + idIntegrityScorePart("P1"),
                                                             "<part id=\"P1\"><measure number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{1}, repaired.diagnostics.size());
    CHECK(repaired.diagnostics.at(0).code == DiagnosticCode::duplicateId);
    CHECK(idIntegrityContains(repaired.diagnostics.at(0).message, "renamed to \"P1-2\""));
    CHECK(idIntegrityContains(repaired.xml, "<score-part id=\"P1\">"));
    CHECK(idIntegrityContains(repaired.xml, "<score-part id=\"P1-2\">"));
    // The first element keeps the id, so the reference still resolves.
    CHECK(idIntegrityContains(repaired.xml, "<part id=\"P1\">"));
}

TEST(WriteRenameSkipsAnIdUsedLater, IdIntegrity)
{
    const auto repaired = idIntegrityRepair(
        idIntegrityScore(idIntegrityScorePart("P1") + idIntegrityScorePart("P1") + idIntegrityScorePart("P1-2"),
                         "<part id=\"P1\"><measure number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{1}, repaired.diagnostics.size());
    CHECK(idIntegrityContains(repaired.diagnostics.at(0).message, "renamed to \"P1-3\""));
    CHECK(idIntegrityContains(repaired.xml, "<score-part id=\"P1-3\">"));
}

TEST(WriteReportsDanglingReferenceWithoutChangingIt, IdIntegrity)
{
    const auto repaired = idIntegrityRepair(
        idIntegrityScore(idIntegrityScorePart("P1"), "<part id=\"P9\"><measure number=\"1\"/></part>"));
    CHECK_EQUAL(std::size_t{1}, repaired.diagnostics.size());
    CHECK(repaired.diagnostics.at(0).code == DiagnosticCode::danglingIdReference);
    CHECK(idIntegrityContains(repaired.xml, "<part id=\"P9\">"));
}

TEST(WriteRenamesWithoutAHandler, IdIntegrity)
{
    // The rename keeps the output valid, so it happens whether or not
    // anybody is listening.
    const std::string xml = idIntegrityScore(idIntegrityScorePart("P1") + idIntegrityScorePart("P1"),
                                             "<part id=\"P1\"><measure number=\"1\"/></part>");
    pugi::xml_document doc;
    CHECK(doc.load_string(xml.c_str()));
    repairIds(doc, DiagnosticHandler{});
    const pugi::xml_node second =
        doc.document_element().child("part-list").child("score-part").next_sibling("score-part");
    CHECK_EQUAL(std::string{"P1-2"}, std::string{second.attribute("id").value()});
}
