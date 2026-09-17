// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/corert/CoreRoundtripImpl.h"

#include "mx/core/Lexical.h"
#include "mx/core/ParseContext.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/Version.h"
#include "mxtest/corert/Compare.h"
#include "mxtest/corert/Fixer.h"
#include "mxtest/file/PathRoot.h"
#include "mxtest/import/Normalize.h"

#include "pugixml.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string_view>

namespace mxtest
{
namespace corert
{

namespace
{

constexpr const char *const kDataDirName = "data";

const std::string &dataRoot()
{
    static const std::string root = std::string(MX_REPO_ROOT_PATH) + "/" + kDataDirName;
    return root;
}

bool isExcludedPath(const std::filesystem::path &p)
{
    static const std::vector<std::string> excludedSegments = {"expected", "testOutput", "generalxml", "smufl"};
    for (const auto &part : p)
    {
        const std::string seg = part.string();
        for (const auto &ex : excludedSegments)
        {
            if (seg == ex)
            {
                return true;
            }
        }
    }
    return false;
}

bool hasXmlExtension(const std::filesystem::path &p)
{
    const std::string ext = p.extension().string();
    return ext == ".xml" || ext == ".musicxml";
}

bool hasSuffix(const std::string &filename, std::string_view suffix)
{
    if (filename.size() < suffix.size())
    {
        return false;
    }
    return filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool isFixupSidecar(const std::filesystem::path &p)
{
    return hasSuffix(p.filename().string(), ".fixup.xml");
}

// Artifacts written by the feature-audit tool (`python3 -m audit`): the
// per-file `*.features.xml` sidecars and the top-level `corpus.xml` aggregate.
// They live under `data/` with a `.xml` extension but are not MusicXML scores,
// so the round-trip suite must skip them. See audit/discover.py.
bool isAuditArtifact(const std::filesystem::path &p)
{
    const std::string filename = p.filename().string();
    return hasSuffix(filename, ".features.xml") || filename == "corpus.xml";
}

bool hasInvalidMarker(const std::filesystem::path &p)
{
    std::filesystem::path marker = p;
    marker += ".invalid";
    std::error_code ec;
    return std::filesystem::exists(marker, ec);
}

std::string saveToString(const pugi::xml_document &doc)
{
    std::ostringstream ss;
    doc.save(ss, "  ");
    return ss.str();
}

bool declaredVersionExceeds(const pugi::xml_document &doc)
{
    pugi::xml_node root = doc.document_element();
    if (!root)
    {
        return false;
    }
    const std::string_view declared = root.attribute("version").value();
    if (declared.empty())
    {
        return false; // absent means MusicXML 1.0
    }
    return mx::core::musicXmlVersionExceeds(declared, mx::core::SupportedMusicXMLVersion);
}

} // namespace

std::vector<std::string> discoverInputFiles()
{
    std::vector<std::string> result;
    namespace fs = std::filesystem;
    const fs::path root(dataRoot());
    if (!fs::exists(root))
    {
        return result;
    }
    for (auto it = fs::recursive_directory_iterator(root); it != fs::recursive_directory_iterator(); ++it)
    {
        const auto &entry = *it;
        if (!entry.is_regular_file())
        {
            continue;
        }
        const fs::path &p = entry.path();
        const fs::path relative = fs::relative(p, root);
        if (isExcludedPath(relative) || !hasXmlExtension(p) || isFixupSidecar(p) || isAuditArtifact(p) ||
            hasInvalidMarker(p))
        {
            continue;
        }
        result.push_back(p.string());
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::string toTestName(const std::string &absolutePath)
{
    namespace fs = std::filesystem;
    const fs::path root(dataRoot());
    const fs::path abs(absolutePath);
    std::error_code ec;
    const fs::path rel = fs::relative(abs, root, ec);
    if (ec || rel.empty() || rel.native().rfind("..", 0) == 0)
    {
        return absolutePath;
    }
    return rel.generic_string();
}

CoreRoundtripResult runCoreRoundtrip(const std::string &absoluteInputPath)
{
    CoreRoundtripResult r;
    try
    {
        // 1. Load the input (pugixml auto-detects UTF-16/Latin-1).
        pugi::xml_document inputDoc;
        const pugi::xml_parse_result loaded =
            inputDoc.load_file(absoluteInputPath.c_str(), pugi::parse_default | pugi::parse_doctype);
        if (!loaded)
        {
            r.message = std::string{"failed to load: "} + loaded.description();
            return r;
        }

        // 2. Version gate: a document declaring a NEWER MusicXML than the
        //    generated model supports may use types the model cannot
        //    represent; skip, don't fail. (Nothing in the corpus does.)
        if (declaredVersionExceeds(inputDoc))
        {
            r.skipped = true;
            r.message = "declares a MusicXML version newer than the supported " +
                        std::string{mx::core::SupportedMusicXMLVersion};
            return r;
        }

        // 3. Pin the root version to the harness baseline, then parse.
        setRootMusicXmlVersion(inputDoc, kMusicXmlVersionBaseline);
        auto parsed = mx::core::parse(inputDoc, mx::core::ParseContext{});
        if (!parsed)
        {
            r.message = "parse failed [" + parsed.error().path + "] " + parsed.error().message;
            return r;
        }

        // 4. Serialize back out and normalize the actual document.
        pugi::xml_document actualDoc;
        mx::core::serialize(parsed.value(), actualDoc);
        normalizeForComparison(actualDoc);

        // 5. Load a fresh expected from disk, normalize identically, then
        //    apply per-file fixups (mx clamps out-of-bounds values; the
        //    sidecar patches the expected to the clamped form).
        pugi::xml_document expectedDoc;
        if (!expectedDoc.load_file(absoluteInputPath.c_str(), pugi::parse_default | pugi::parse_doctype))
        {
            r.message = "failed to reload expected";
            return r;
        }
        normalizeForComparison(expectedDoc);
        Fixer fixer(absoluteInputPath);
        fixer.applyToExpected(expectedDoc);

        // 6. Depth-first compare.
        const auto fail = compareElements(expectedDoc.document_element(), actualDoc.document_element());
        if (fail.isFailure)
        {
            r.message = fail.detail;
            r.expectedXml = saveToString(expectedDoc);
            r.actualXml = saveToString(actualDoc);
            return r;
        }

        r.ok = true;
        return r;
    }
    catch (const std::exception &e)
    {
        r.message = std::string{"exception: "} + e.what();
        return r;
    }
    catch (...)
    {
        r.message = "unknown exception";
        return r;
    }
}

void writeFailureFiles(const std::string &testName, const std::string &expectedXml, const std::string &actualXml)
{
    namespace fs = std::filesystem;
    const fs::path outDir = fs::path(MX_REPO_ROOT_PATH) / kDataDirName / "testOutput" / "corert";
    std::error_code ec;
    fs::create_directories(outDir, ec);

    std::string flat = testName;
    std::replace(flat.begin(), flat.end(), '/', '_');
    std::replace(flat.begin(), flat.end(), '\\', '_');

    {
        std::ofstream f(outDir / (flat + ".expected.xml"));
        if (f.is_open())
        {
            f << expectedXml;
        }
    }
    {
        std::ofstream f(outDir / (flat + ".actual.xml"));
        if (f.is_open())
        {
            f << actualXml;
        }
    }
}

} // namespace corert
} // namespace mxtest
