// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model. Never contains
// generated content (regen-safe split, see docs/ai/design/mx-core-plan.md §2.6).

#pragma once

#include <string>
#include <string_view>

namespace pugi
{
class xml_document;
}

namespace mx::core
{
class Document;

/// Prefix that identifies mx's own <software> provenance stamp. A <software>
/// whose text starts with this is mx attribution, not user content: the writer
/// replaces it (so re-serializing never accumulates stamps) and the api read
/// path drops it.
inline constexpr std::string_view kMxSoftwareMarker = "https://github.com/webern/mx";

/// The <software> text mx stamps onto every file it writes, e.g.
/// "https://github.com/webern/mx version=b987bfa16671". The version component
/// is the build's gitSha, carrying "-dirty"/"unknown" in the edge cases.
std::string mxSoftwareAttribution();

/// Serialize, stamping mx's provenance <software> into <encoding> (creating
/// <identification>/<encoding> when absent, replacing any prior mx stamp). The
/// product/api write path goes through here; the generated serialize() stays
/// pure so the roundtrip fidelity tests are unaffected.
void serializeWithAttribution(const Document &d, pugi::xml_document &out);

} // namespace mx::core
