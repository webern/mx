// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// serializeWithAttribution stamps mx's <software> provenance into <encoding>:
// it creates <identification>/<encoding> when absent, preserves the user's own
// software, and never accumulates stamps across round-trips.

#include "cpul/cpulTestHarness.h"

#include "mx/core/Attribution.h"
#include "mx/core/ParseContext.h"
#include "mx/core/Version.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/Encoding.h"
#include "mx/core/generated/EncodingChoice.h"
#include "mx/core/generated/Identification.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/core/generated/ScorePartwise.h"

#include "mx/xml/Xml.h"

#include <string>
#include <string_view>

using namespace mx::core;

namespace
{

// Count <software> descendants whose text equals the given value.
int countSoftware(const mx::xml::Node &node, std::string_view text)
{
    int count = 0;
    for (mx::xml::Node child : node.children())
    {
        if (std::string_view{child.name()} == "software" && std::string_view{child.text()} == text)
        {
            ++count;
        }
        count += countSoftware(child, text);
    }
    return count;
}

// Count mx stamps regardless of which version they carry (prefix match).
int countMxStamps(const mx::xml::Node &node)
{
    int count = 0;
    for (mx::xml::Node child : node.children())
    {
        if (std::string_view{child.name()} == "software" &&
            std::string_view{child.text()}.starts_with(kMxSoftwareMarker))
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
    mx::xml::Document out;
    serializeWithAttribution(doc, out);

    const std::string expected = mxSoftwareAttribution();
    CHECK(expected == std::string{"https://github.com/webern/mx version="} + gitSha);

    // Stamped at the schema-correct path, exactly once.
    const mx::xml::Node encoding = out.documentElement().child("identification").child("encoding");
    CHECK(std::string_view{encoding.child("software").text()} == expected);
    CHECK(countSoftware(out.documentElement(), expected) == 1);
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

    mx::xml::Document out;
    serializeWithAttribution(doc, out);

    CHECK(countSoftware(out.documentElement(), "Finale") == 1);
    CHECK(countSoftware(out.documentElement(), mxSoftwareAttribution()) == 1);
}

TEST(DoesNotAccumulateAcrossRoundTrips, Attribution)
{
    Document doc;
    mx::xml::Document first;
    serializeWithAttribution(doc, first);

    // Parsing mx's own output, then re-writing, must still yield one stamp.
    const auto reparsed = parse(first, ParseContext{});
    CHECK(reparsed.ok());
    if (reparsed.ok())
    {
        mx::xml::Document second;
        serializeWithAttribution(reparsed.value(), second);
        CHECK(countSoftware(second.documentElement(), mxSoftwareAttribution()) == 1);
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

    mx::xml::Document out;
    serializeWithAttribution(doc, out);

    // The stale stamp is gone, replaced by exactly one current-version stamp.
    CHECK(countSoftware(out.documentElement(), staleStamp) == 0);
    CHECK(countSoftware(out.documentElement(), mxSoftwareAttribution()) == 1);
    CHECK(countMxStamps(out.documentElement()) == 1);
}
