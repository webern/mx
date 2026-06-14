// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// serializeWithAttribution stamps mx's <software> provenance into <encoding>:
// it creates <identification>/<encoding> when absent, preserves the user's own
// software, and never accumulates stamps across round-trips.

#include "cpul/cpulTestHarness.h"

#include "mx/core/Attribution.h"
#include "mx/core/Version.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/Encoding.h"
#include "mx/core/generated/EncodingChoice.h"
#include "mx/core/generated/Identification.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/core/generated/ScorePartwise.h"

#include "pugixml/pugixml.hpp"

#include <string>
#include <string_view>

using namespace mx::core;

namespace
{

// Count <software> descendants whose text equals the given value.
int countSoftware(const pugi::xml_node &node, std::string_view text)
{
    int count = 0;
    for (pugi::xml_node child : node.children())
    {
        if (std::string_view{child.name()} == "software" && std::string_view{child.text().get()} == text)
        {
            ++count;
        }
        count += countSoftware(child, text);
    }
    return count;
}

// Count mx stamps regardless of which version they carry (prefix match).
int countMxStamps(const pugi::xml_node &node)
{
    int count = 0;
    for (pugi::xml_node child : node.children())
    {
        if (std::string_view{child.name()} == "software" &&
            std::string_view{child.text().get()}.starts_with(kMxSoftwareMarker))
        {
            ++count;
        }
        count += countMxStamps(child);
    }
    return count;
}

} // namespace

TEST(StampsIntoEmptyDocument, Attribution)
{
    Document doc; // default ScorePartwise, no <identification>
    pugi::xml_document out;
    serializeWithAttribution(doc, out);

    const std::string expected = mxSoftwareAttribution();
    CHECK(expected == std::string{"https://github.com/webern/mx version="} + gitSha);

    // Stamped at the schema-correct path, exactly once.
    const pugi::xml_node encoding = out.document_element().child("identification").child("encoding");
    CHECK(std::string_view{encoding.child("software").text().get()} == expected);
    CHECK(countSoftware(out.document_element(), expected) == 1);
}

TEST(PreservesUserSoftware, Attribution)
{
    Document doc;
    ScorePartwise score = doc.asScorePartwise();
    ScoreHeaderGroup header = score.scoreHeader();
    Identification identification;
    Encoding encoding;
    encoding.addChoice(EncodingChoice::software("Finale"));
    identification.setEncoding(encoding);
    header.setIdentification(identification);
    score.setScoreHeader(header);
    doc.setRoot(Document::Root{score});

    pugi::xml_document out;
    serializeWithAttribution(doc, out);

    CHECK(countSoftware(out.document_element(), "Finale") == 1);
    CHECK(countSoftware(out.document_element(), mxSoftwareAttribution()) == 1);
}

TEST(DoesNotAccumulateAcrossRoundTrips, Attribution)
{
    Document doc;
    pugi::xml_document first;
    serializeWithAttribution(doc, first);

    // Parsing mx's own output, then re-writing, must still yield one stamp.
    const auto reparsed = parse(first);
    CHECK(reparsed.ok());
    if (reparsed.ok())
    {
        pugi::xml_document second;
        serializeWithAttribution(reparsed.value(), second);
        CHECK(countSoftware(second.document_element(), mxSoftwareAttribution()) == 1);
    }
}

TEST(ReplacesAStampFromADifferentVersion, Attribution)
{
    // A file written by some other mx version carries a different-sha stamp.
    Document doc;
    ScorePartwise score = doc.asScorePartwise();
    ScoreHeaderGroup header = score.scoreHeader();
    Identification identification;
    Encoding encoding;
    const std::string staleStamp = "https://github.com/webern/mx version=0000staleomx0";
    encoding.addChoice(EncodingChoice::software(staleStamp));
    identification.setEncoding(encoding);
    header.setIdentification(identification);
    score.setScoreHeader(header);
    doc.setRoot(Document::Root{score});

    pugi::xml_document out;
    serializeWithAttribution(doc, out);

    // The stale stamp is gone, replaced by exactly one current-version stamp.
    CHECK(countSoftware(out.document_element(), staleStamp) == 0);
    CHECK(countSoftware(out.document_element(), mxSoftwareAttribution()) == 1);
    CHECK(countMxStamps(out.document_element()) == 1);
}
