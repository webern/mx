// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/corert/CoreRoundtripImpl.h"

#include "ezxml/XAttribute.h"
#include "ezxml/XAttributeIterator.h"
#include "ezxml/XElementIterator.h"
#include "ezxml/XFactory.h"
#include "mx/core/Document.h"
#include "mxtest/file/PathRoot.h"
#include "mxtest/import/ChangeValues.h"
#include "mxtest/import/SortAttributes.h"

#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace mxtest
{
namespace corert
{

namespace
{

constexpr const char *const kDataDirName = "data";
constexpr const char *const kTestOutputSubdir = "testOutput/corert";

const std::string &dataRoot()
{
    static const std::string root = std::string(MX_REPO_ROOT_PATH) + "/" + kDataDirName;
    return root;
}

// Exclude any path containing a directory segment named one of these.
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

// A file is marked invalid (and therefore unfit for schema-strict round-trip)
// when a sibling file with the same name plus a trailing `.invalid` extension
// exists. See data/README.md for the convention.
bool hasInvalidMarker(const std::filesystem::path &p)
{
    std::filesystem::path marker = p;
    marker += ".invalid";
    std::error_code ec;
    return std::filesystem::exists(marker, ec);
}

// Set the root `version` attribute to mx::core's supported MusicXML version.
// Equivalent in shape to mxtest/import's setRootMusicXmlVersion but without
// the doctype cross-check (which we apply via setDoctype below based on the
// root element name).
// Pinned MusicXML version string for the supported `mx::core` schema.
//
// Ideally this would come from a single `mx::core` constant. Today the core
// namespace declares the version in two conflicting places
// (`DocumentSpec.h::DEFAULT_MUSIC_XML_VERSION` lower-case and
// `DocumentHeader.h::kDefaultMusicXmlVersion` upper-case `ThreePointZero`),
// and including both in one TU is a hard compile error. The api import suite
// already hardcodes `"3.0"` for the same reason (see
// `ImportTestImpl::loadTestFile`); we follow that precedent here. When the
// core-side conflict is resolved (likely as part of the codegen rewrite that
// brings MusicXML 4.0 support), replace this with the canonical constant.
constexpr const char *const kMusicXmlVersionString = "3.0";

void setRootMusicXmlVersion(const ::ezxml::XDocPtr &xdoc)
{
    const auto root = xdoc->getRoot();
    if (!root)
    {
        return;
    }
    for (auto it = root->attributesBegin(); it != root->attributesEnd(); ++it)
    {
        if (it->getName() == "version")
        {
            it->setValue(kMusicXmlVersionString);
            return;
        }
    }
    root->appendAttribute("version")->setValue(kMusicXmlVersionString);
}

void setXmlDeclaration(const ::ezxml::XDocPtr &xdoc)
{
    xdoc->setHasStandaloneAttribute(true);
    xdoc->setIsStandalone(false);
    xdoc->setXmlVersion(::ezxml::XmlVersion::onePointZero);
    const auto encoding = xdoc->getEncoding();
    if (encoding == ::ezxml::Encoding::unknown)
    {
        xdoc->setEncoding(::ezxml::DEFAULT_ENCODING);
    }
}

void setDoctypeFromRoot(const ::ezxml::XDocPtr &xdoc)
{
    const auto root = xdoc->getRoot();
    if (!root)
    {
        return;
    }
    const std::string name = root->getName();
    xdoc->setHasDoctypeDeclaration(true);
    if (name == "score-timewise")
    {
        xdoc->setDoctypeValue(mx::core::DOCTYPE_VALUE_SCORE_TIMEWISE);
    }
    else
    {
        // Default to partwise for anything else (including the common
        // score-partwise root). A non-MusicXML root will be caught upstream
        // by fromXDoc.
        xdoc->setDoctypeValue(mx::core::DOCTYPE_VALUE_SCORE_PARTWISE);
    }
}

// Apply the full normalization pipeline (design §3 Normalization). Must be
// applied to both the expected (input reloaded) and the actual (toXDoc output)
// trees so the comparison sees canonical-against-canonical input.
void normalize(const ::ezxml::XDocPtr &xdoc)
{
    setXmlDeclaration(xdoc);
    setDoctypeFromRoot(xdoc);
    setRootMusicXmlVersion(xdoc);
    mxtest::stripZerosFromDecimalFields(*xdoc);
    mxtest::sortAttributes(*xdoc); // must be last
}

// Format a node path like /score-partwise/part[0]/measure[2]/note[1]/pitch.
std::string nodePath(const std::vector<std::string> &segments)
{
    std::string s;
    for (const auto &seg : segments)
    {
        s += "/";
        s += seg;
    }
    return s.empty() ? "/" : s;
}

struct CompareFailure
{
    bool isFailure = false;
    std::string detail;
};

CompareFailure compareElements(const ::ezxml::XElement &expected, const ::ezxml::XElement &actual,
                               std::vector<std::string> &pathStack)
{
    CompareFailure fail;

    if (expected.getName() != actual.getName())
    {
        std::stringstream ss;
        ss << "element name mismatch at " << nodePath(pathStack) << ": expected '" << expected.getName()
           << "', actual '" << actual.getName() << "'";
        fail.isFailure = true;
        fail.detail = ss.str();
        return fail;
    }

    // Compare text payload (exact, per design §3 open question — start exact).
    if (expected.getValue() != actual.getValue())
    {
        std::stringstream ss;
        ss << "text mismatch at " << nodePath(pathStack) << ": expected '" << expected.getValue() << "', actual '"
           << actual.getValue() << "'";
        fail.isFailure = true;
        fail.detail = ss.str();
        return fail;
    }

    // Compare attributes. They are already sorted by the normalization step.
    auto eAttrIt = expected.attributesBegin();
    const auto eAttrEnd = expected.attributesEnd();
    auto aAttrIt = actual.attributesBegin();
    const auto aAttrEnd = actual.attributesEnd();
    while (eAttrIt != eAttrEnd && aAttrIt != aAttrEnd)
    {
        if (eAttrIt->getName() != aAttrIt->getName() || eAttrIt->getValue() != aAttrIt->getValue())
        {
            std::stringstream ss;
            ss << "attribute mismatch at " << nodePath(pathStack) << "[@" << eAttrIt->getName() << "]: expected '"
               << eAttrIt->getName() << "=" << eAttrIt->getValue() << "', actual '" << aAttrIt->getName() << "="
               << aAttrIt->getValue() << "'";
            fail.isFailure = true;
            fail.detail = ss.str();
            return fail;
        }
        ++eAttrIt;
        ++aAttrIt;
    }
    if (eAttrIt != eAttrEnd || aAttrIt != aAttrEnd)
    {
        std::stringstream ss;
        ss << "attribute count mismatch at " << nodePath(pathStack);
        fail.isFailure = true;
        fail.detail = ss.str();
        return fail;
    }

    // Compare children, depth-first.
    auto eIt = expected.begin();
    const auto eEnd = expected.end();
    auto aIt = actual.begin();
    const auto aEnd = actual.end();
    int childIndex = 0;
    while (eIt != eEnd && aIt != aEnd)
    {
        std::stringstream segSs;
        segSs << eIt->getName() << "[" << childIndex << "]";
        pathStack.push_back(segSs.str());
        const auto childFail = compareElements(*eIt, *aIt, pathStack);
        if (childFail.isFailure)
        {
            return childFail;
        }
        pathStack.pop_back();
        ++eIt;
        ++aIt;
        ++childIndex;
    }
    if (eIt != eEnd || aIt != aEnd)
    {
        std::stringstream ss;
        ss << "child count mismatch at " << nodePath(pathStack);
        fail.isFailure = true;
        fail.detail = ss.str();
        return fail;
    }

    return fail;
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
        // Excludes are checked against the path relative to the data root so
        // an unrelated `expected` segment higher in the filesystem cannot
        // affect classification.
        const fs::path relative = fs::relative(p, root);
        if (isExcludedPath(relative))
        {
            continue;
        }
        if (!hasXmlExtension(p))
        {
            continue;
        }
        if (hasInvalidMarker(p))
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
    // Forward slashes for cross-platform consistency in test names.
    std::string s = rel.generic_string();
    return s;
}

CoreRoundtripResult runCoreRoundtrip(const std::string &absoluteInputPath)
{
    CoreRoundtripResult r;
    try
    {
        // 1. Load the input via ezxml.
        auto inputXDoc = ::ezxml::XFactory::makeXDoc();
        inputXDoc->loadFile(absoluteInputPath);

        // 2. Set input root `version` to the supported MusicXML version. The
        //    declaration and doctype are set later by normalization.
        setRootMusicXmlVersion(inputXDoc);

        // 3. fromXDoc into a mx::core::Document.
        auto mxDoc = mx::core::makeDocument();
        std::stringstream fromMsg;
        const bool ok = mxDoc->fromXDoc(fromMsg, *inputXDoc);
        if (!ok)
        {
            r.ok = false;
            r.message = "fromXDoc returned false: " + fromMsg.str();
            return r;
        }

        // 4. toXDoc back out.
        auto actualXDoc = ::ezxml::XFactory::makeXDoc();
        mxDoc->toXDoc(*actualXDoc);

        // 5. Normalize the actual document.
        normalize(actualXDoc);

        // 6. Load a fresh expected from disk.
        auto expectedXDoc = ::ezxml::XFactory::makeXDoc();
        expectedXDoc->loadFile(absoluteInputPath);
        // Mirror step 2 so the input version is canonical before normalization.
        setRootMusicXmlVersion(expectedXDoc);

        // 7. Normalize the expected document with the same pipeline.
        normalize(expectedXDoc);

        // 8. Depth-first compare.
        std::vector<std::string> pathStack;
        const auto expectedRoot = expectedXDoc->getRoot();
        const auto actualRoot = actualXDoc->getRoot();
        if (!expectedRoot || !actualRoot)
        {
            r.ok = false;
            r.message = "missing root element after normalization";
            return r;
        }
        pathStack.push_back(expectedRoot->getName());
        const auto fail = compareElements(*expectedRoot, *actualRoot, pathStack);

        if (fail.isFailure)
        {
            r.ok = false;
            r.message = fail.detail;
            std::stringstream eSs;
            std::stringstream aSs;
            expectedXDoc->saveStream(eSs);
            actualXDoc->saveStream(aSs);
            r.expectedXml = eSs.str();
            r.actualXml = aSs.str();
            return r;
        }

        r.ok = true;
        return r;
    }
    catch (const std::exception &e)
    {
        r.ok = false;
        r.message = std::string("exception: ") + e.what();
        return r;
    }
    catch (...)
    {
        r.ok = false;
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
    // If create_directories fails, the open below will fail too; we let that
    // surface to the caller via the test simply not having debug files. No
    // throw — the suite is already in a failure state at this point.

    std::string flat = testName;
    std::replace(flat.begin(), flat.end(), '/', '_');
    std::replace(flat.begin(), flat.end(), '\\', '_');

    const fs::path expectedPath = outDir / (flat + ".expected.xml");
    const fs::path actualPath = outDir / (flat + ".actual.xml");
    {
        std::ofstream f(expectedPath);
        if (f.is_open())
        {
            f << expectedXml;
        }
    }
    {
        std::ofstream f(actualPath);
        if (f.is_open())
        {
            f << actualXml;
        }
    }
}

} // namespace corert
} // namespace mxtest
