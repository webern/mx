// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Attribution.h"

#include "mx/core/Version.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/Encoding.h"
#include "mx/core/generated/EncodingChoice.h"
#include "mx/core/generated/Identification.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/core/generated/ScoreTimewise.h"

#include "pugixml/pugixml.hpp"

#include <utility>
#include <vector>

namespace mx::core
{

namespace
{

// Rebuild the score header's <encoding> with exactly one mx stamp: drop any
// prior mx stamp (matched by prefix) and append the current one. Preserves the
// user's own software entries and every other encoding child. Works on either
// root because both expose scoreHeader()/setScoreHeader().
template <typename Score> Score withStamp(Score score)
{
    ScoreHeaderGroup header = score.scoreHeader();
    Identification identification = header.identification().value_or(Identification{});
    Encoding encoding = identification.encoding().value_or(Encoding{});

    std::vector<EncodingChoice> choice;
    for (const auto &existing : encoding.choice())
    {
        const bool isMxStamp =
            existing.isSoftware() && std::string_view{existing.asSoftware()}.starts_with(kMxSoftwareMarker);
        if (!isMxStamp)
        {
            choice.push_back(existing);
        }
    }
    choice.push_back(EncodingChoice::software(mxSoftwareAttribution()));
    encoding.setChoice(std::move(choice));

    identification.setEncoding(std::move(encoding));
    header.setIdentification(std::move(identification));
    score.setScoreHeader(std::move(header));
    return score;
}

} // namespace

std::string mxSoftwareAttribution()
{
    return std::string{kMxSoftwareMarker} + " version=" + gitSha;
}

void serializeWithAttribution(const Document &d, pugi::xml_document &out)
{
    // Stamp the typed model and let the generated serializer place
    // <identification>/<encoding> in their schema-correct slots for free.
    Document stamped = d;
    if (stamped.isScorePartwise())
    {
        stamped.setRoot(Document::Root{withStamp(stamped.asScorePartwise())});
    }
    else
    {
        stamped.setRoot(Document::Root{withStamp(stamped.asScoreTimewise())});
    }
    serialize(stamped, out);
}

} // namespace mx::core
