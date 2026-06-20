// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Corpus api roundtrip harness.
//
// Runs the full api pipeline (createFromFile → getData → createFromScore →
// writeToStream) over corpus files and compares each output to its input with
// the shared normalize/compare machinery. Two modes:
//
//   regression <dataRoot> <baselineFile>
//       Run only the files listed in <baselineFile> (one data/-relative path
//       per non-comment line). Exit non-zero if any listed file fails. This
//       is the CI gate.
//
//   discovery <dataRoot>
//       Walk the whole corpus (same exclusion rules as corert: skip expected/,
//       testOutput/, generalxml/, smufl/, *.fixup.xml, *.features.xml and the
//       corpus.xml audit artifacts, and *.invalid markers).
//       Print one line per file: PASS|FAIL|SKIP<TAB>relpath<TAB>detail.
//       Exit 0 always. Use to grow the pinned list.

#include "mx/api/DocumentManager.h"
#include "mx/core/Attribution.h"
#include "mxtest/corert/Compare.h"
#include "mxtest/corert/Fixer.h"
#include "pugixml/pugixml.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace
{

bool isExcludedPath(const std::filesystem::path &p)
{
    static const std::vector<std::string> kExcluded = {"expected", "testOutput", "generalxml", "smufl"};
    for (const auto &seg : p)
    {
        for (const auto &ex : kExcluded)
        {
            if (seg.string() == ex)
                return true;
        }
    }
    return false;
}

// mx stamps its provenance <software> into <identification>/<encoding> on every
// api write (core::withStamp via serializeWithAttribution) -- an intended feature.
// Rather than strip it from the output, the round-trip *expects* it: add the same
// stamp to the original (expected) document so both carry it. The comparison then
// asserts the stamp is present and in its schema-correct slot -- a write that drops
// it or misplaces it now fails. Mirrors withStamp: create <identification>/
// <encoding> in schema position if absent, drop any prior mx stamp, then append the
// current one as encoding's last child.
void addMxAttribution(pugi::xml_node scoreRoot)
{
    pugi::xml_node identification = scoreRoot.child("identification");
    if (!identification)
    {
        // identification follows work/movement-*, precedes defaults/credit/part-list.
        pugi::xml_node before;
        for (pugi::xml_node c = scoreRoot.first_child(); c; c = c.next_sibling())
        {
            const std::string_view n{c.name()};
            if (n == "defaults" || n == "credit" || n == "part-list")
            {
                before = c;
                break;
            }
        }
        identification =
            before ? scoreRoot.insert_child_before("identification", before) : scoreRoot.append_child("identification");
    }

    pugi::xml_node encoding = identification.child("encoding");
    if (!encoding)
    {
        // encoding follows creator/rights, precedes source/relation/miscellaneous.
        pugi::xml_node before;
        for (pugi::xml_node c = identification.first_child(); c; c = c.next_sibling())
        {
            const std::string_view n{c.name()};
            if (n == "source" || n == "relation" || n == "miscellaneous")
            {
                before = c;
                break;
            }
        }
        encoding =
            before ? identification.insert_child_before("encoding", before) : identification.append_child("encoding");
    }

    for (pugi::xml_node sw = encoding.child("software"); sw;)
    {
        const pugi::xml_node next = sw.next_sibling("software");
        if (std::string_view{sw.text().get()}.starts_with(mx::core::kMxSoftwareMarker))
        {
            encoding.remove_child(sw);
        }
        sw = next;
    }
    encoding.append_child("software")
        .append_child(pugi::node_pcdata)
        .set_value(mx::core::mxSoftwareAttribution().c_str());
}

bool hasSuffix(const std::string &name, std::string_view suffix)
{
    return name.size() >= suffix.size() && name.compare(name.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool isFixupSidecar(const std::filesystem::path &p)
{
    return hasSuffix(p.filename().string(), ".fixup.xml");
}

// Skip the feature-audit tool's outputs (*.features.xml sidecars and the
// corpus.xml aggregate): they share data/'s .xml extension but are not scores.
bool isAuditArtifact(const std::filesystem::path &p)
{
    const std::string name = p.filename().string();
    return hasSuffix(name, ".features.xml") || name == "corpus.xml";
}

bool hasInvalidMarker(const std::filesystem::path &p)
{
    std::filesystem::path marker = p;
    marker += ".invalid";
    std::error_code ec;
    return std::filesystem::exists(marker, ec);
}

std::vector<std::string> discoverFiles(const std::string &dataRoot)
{
    std::vector<std::string> result;
    namespace fs = std::filesystem;
    const fs::path root(dataRoot);
    if (!fs::exists(root))
        return result;
    for (auto it = fs::recursive_directory_iterator(root); it != fs::recursive_directory_iterator(); ++it)
    {
        const auto &entry = *it;
        if (!entry.is_regular_file())
            continue;
        const fs::path &p = entry.path();
        const fs::path rel = fs::relative(p, root);
        const std::string ext = p.extension().string();
        if (ext != ".xml" && ext != ".musicxml")
            continue;
        if (isExcludedPath(rel) || isFixupSidecar(p) || isAuditArtifact(p) || hasInvalidMarker(p))
            continue;
        result.push_back(p.string());
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<std::string> readBaseline(const std::string &baselineFile)
{
    std::vector<std::string> result;
    std::ifstream f(baselineFile);
    if (!f.is_open())
    {
        std::cerr << "cannot open baseline file: " << baselineFile << std::endl;
        return result;
    }
    std::string line;
    while (std::getline(f, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        result.push_back(line);
    }
    return result;
}

struct RoundtripResult
{
    enum class Status
    {
        pass,
        fail,
        skip,
        loadFail,
        getDataFail,
        createFail
    };
    Status status = Status::fail;
    std::string detail;
};

RoundtripResult runRoundtrip(const std::string &absolutePath)
{
    RoundtripResult r;

    auto &mgr = mx::api::DocumentManager::getInstance();

    // Load via the api
    const auto idResult = mgr.createFromFile(absolutePath);
    if (!idResult.ok())
    {
        r.status = RoundtripResult::Status::loadFail;
        r.detail = idResult.error().message;
        return r;
    }
    const int docId = idResult.value();

    // Get score data
    const auto scoreResult = mgr.getData(docId);
    mgr.destroyDocument(docId);
    if (!scoreResult.ok())
    {
        r.status = RoundtripResult::Status::getDataFail;
        r.detail = scoreResult.error().message;
        return r;
    }

    // Re-create from ScoreData
    const auto id2Result = mgr.createFromScore(scoreResult.value());
    if (!id2Result.ok())
    {
        r.status = RoundtripResult::Status::createFail;
        r.detail = id2Result.error().message;
        return r;
    }
    const int docId2 = id2Result.value();

    // Write to string
    std::ostringstream ss;
    const auto writeResult = mgr.writeToStream(docId2, ss);
    mgr.destroyDocument(docId2);
    if (!writeResult.ok())
    {
        r.status = RoundtripResult::Status::fail;
        r.detail = writeResult.error().message;
        return r;
    }
    const std::string actualXml = ss.str();

    // Load actual XML with pugixml
    pugi::xml_document actualDoc;
    if (!actualDoc.load_string(actualXml.c_str(), pugi::parse_default | pugi::parse_doctype))
    {
        r.status = RoundtripResult::Status::fail;
        r.detail = "failed to parse written output as XML";
        return r;
    }

    // Load expected (original input) with pugixml
    pugi::xml_document expectedDoc;
    if (!expectedDoc.load_file(absolutePath.c_str(), pugi::parse_default | pugi::parse_doctype))
    {
        r.status = RoundtripResult::Status::fail;
        r.detail = "failed to reload expected input";
        return r;
    }

    // Every api write stamps mx's provenance into <identification>/<encoding>
    // (an intended feature). Expect it rather than strip it: add the same stamp to
    // the expected (original) document so both carry it. A write that drops the
    // stamp, or emits it in the wrong place, now fails the comparison.
    addMxAttribution(expectedDoc.document_element());

    // Normalize both and apply fixups to expected
    mxtest::corert::normalizeForComparison(expectedDoc);
    mxtest::corert::normalizeForComparison(actualDoc);
    mxtest::corert::Fixer fixer(absolutePath);
    fixer.applyToExpected(expectedDoc);

    // Compare
    const auto fail = mxtest::corert::compareElements(expectedDoc.document_element(), actualDoc.document_element());
    if (fail.isFailure)
    {
        r.status = RoundtripResult::Status::fail;
        r.detail = fail.detail;
        return r;
    }

    r.status = RoundtripResult::Status::pass;
    return r;
}

std::string statusString(RoundtripResult::Status s)
{
    switch (s)
    {
    case RoundtripResult::Status::pass:
        return "PASS";
    case RoundtripResult::Status::fail:
        return "FAIL";
    case RoundtripResult::Status::skip:
        return "SKIP";
    case RoundtripResult::Status::loadFail:
        return "LOADFAIL";
    case RoundtripResult::Status::getDataFail:
        return "GETDATAFAIL";
    case RoundtripResult::Status::createFail:
        return "CREATEFAIL";
    }
    return "UNKNOWN";
}

// Flatten a corpus-relative path to a single filename component by replacing
// every directory separator with "__". `lysuite/Saltarello.xml` becomes
// `lysuite__Saltarello.xml`. The classifier (audit/classify.py) reverses this.
std::string toFlatName(const std::string &rel)
{
    std::string out;
    out.reserve(rel.size() + 8);
    for (const char c : rel)
    {
        if (c == '/' || c == '\\')
            out += "__";
        else
            out += c;
    }
    return out;
}

// Write the fully-normalized expected (and, when available, actual) documents
// for one non-PASS file into `dumpDir` for offline diff analysis. This re-runs
// just enough of the pipeline to reproduce the documents `compareElements()`
// saw; `runRoundtrip()` is intentionally left untouched (it reports pass/fail
// and never needs to surface the documents). The duplicated pipeline work is
// acceptable: --dump is a developer tool, not a hot path.
//
// FAIL files yield both `<flat>.expected.xml` and `<flat>.actual.xml`.
// LOADFAIL/GETDATAFAIL/CREATEFAIL files have no actual document; they yield the
// expected file plus a `<flat>.status` sidecar recording the exact status code
// (the flat filename alone cannot distinguish the three pipeline-error kinds).
void dumpDocuments(const std::string &absolutePath, const std::string &relPath, const std::string &dumpDir,
                   RoundtripResult::Status status)
{
    namespace fs = std::filesystem;
    const std::string flat = toFlatName(relPath);
    const std::string expectedPath = (fs::path(dumpDir) / (flat + ".expected.xml")).string();
    const std::string actualPath = (fs::path(dumpDir) / (flat + ".actual.xml")).string();

    // Expected side: load the original input, add the mx stamp (every api write
    // emits it, so the comparison expects it), normalize, then apply fixups --
    // the same order runRoundtrip() uses for the expected document.
    pugi::xml_document expectedDoc;
    if (!expectedDoc.load_file(absolutePath.c_str(), pugi::parse_default | pugi::parse_doctype))
    {
        std::cerr << "dump: cannot load expected for " << relPath << "\n";
        return;
    }
    addMxAttribution(expectedDoc.document_element());
    mxtest::corert::normalizeForComparison(expectedDoc);
    mxtest::corert::Fixer fixer(absolutePath);
    fixer.applyToExpected(expectedDoc);
    if (!expectedDoc.save_file(expectedPath.c_str()))
        std::cerr << "dump: failed to write " << expectedPath << "\n";

    // Pipeline errors produced no actual document. Record the status so the
    // classifier can report the precise kind, then stop.
    if (status == RoundtripResult::Status::loadFail || status == RoundtripResult::Status::getDataFail ||
        status == RoundtripResult::Status::createFail)
    {
        const std::string statusPath = (fs::path(dumpDir) / (flat + ".status")).string();
        std::ofstream statusFile(statusPath);
        statusFile << statusString(status) << "\n";
        std::cerr << "dump: no actual for " << relPath << " (" << statusString(status) << ")\n";
        return;
    }

    // Actual side: re-run the api pipeline (load -> getData -> createFromScore
    // -> writeToStream), then normalize. The provenance stamp is kept (the
    // expected side has it added to match). Mirrors runRoundtrip().
    auto &mgr = mx::api::DocumentManager::getInstance();
    const auto idResult = mgr.createFromFile(absolutePath);
    if (!idResult.ok())
    {
        std::cerr << "dump: no actual for " << relPath << " (createFromFile failed)\n";
        return;
    }
    const int docId = idResult.value();
    const auto scoreResult = mgr.getData(docId);
    mgr.destroyDocument(docId);
    if (!scoreResult.ok())
    {
        std::cerr << "dump: no actual for " << relPath << " (getData failed)\n";
        return;
    }
    const auto id2Result = mgr.createFromScore(scoreResult.value());
    if (!id2Result.ok())
    {
        std::cerr << "dump: no actual for " << relPath << " (createFromScore failed)\n";
        return;
    }
    const int docId2 = id2Result.value();
    std::ostringstream ss;
    const auto writeResult = mgr.writeToStream(docId2, ss);
    mgr.destroyDocument(docId2);
    if (!writeResult.ok())
    {
        std::cerr << "dump: no actual for " << relPath << " (writeToStream failed)\n";
        return;
    }

    pugi::xml_document actualDoc;
    if (!actualDoc.load_string(ss.str().c_str(), pugi::parse_default | pugi::parse_doctype))
    {
        std::cerr << "dump: no actual for " << relPath << " (output did not parse)\n";
        return;
    }
    mxtest::corert::normalizeForComparison(actualDoc);
    if (!actualDoc.save_file(actualPath.c_str()))
        std::cerr << "dump: failed to write " << actualPath << "\n";
}

} // namespace

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "usage:\n"
                  << "  " << argv[0] << " regression <dataRoot> <baselineFile>\n"
                  << "  " << argv[0] << " discovery  <dataRoot> [--dump <dir>]\n";
        return 2;
    }

    const std::string mode = argv[1];
    const std::string dataRoot = argv[2];

    if (mode == "regression")
    {
        if (argc < 4)
        {
            std::cerr << "regression mode requires <baselineFile>" << std::endl;
            return 2;
        }
        const std::string baselineFile = argv[3];
        const auto pinnedList = readBaseline(baselineFile);
        if (pinnedList.empty())
        {
            std::cerr << "baseline file is empty or missing" << std::endl;
            return 2;
        }

        int passed = 0;
        int failed = 0;
        for (const auto &rel : pinnedList)
        {
            const std::string absPath = dataRoot + "/" + rel;
            const auto r = runRoundtrip(absPath);
            if (r.status == RoundtripResult::Status::pass)
            {
                ++passed;
                std::cout << "PASS\t" << rel << "\t" << std::endl;
            }
            else
            {
                ++failed;
                std::cout << "FAIL\t" << rel << "\t" << r.detail << std::endl;
            }
        }
        std::cout << "\n" << passed << " passed, " << failed << " failed (of " << pinnedList.size() << " pinned)\n";
        return failed == 0 ? 0 : 1;
    }
    else if (mode == "discovery")
    {
        // discovery <dataRoot> [--dump <dir>]  (flag may precede or follow the root)
        std::string discDataRoot;
        std::optional<std::string> dumpDir;
        for (int i = 2; i < argc; ++i)
        {
            const std::string a = argv[i];
            if (a == "--dump")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--dump requires a directory" << std::endl;
                    return 2;
                }
                dumpDir = argv[++i];
            }
            else if (discDataRoot.empty())
            {
                discDataRoot = a;
            }
            else
            {
                std::cerr << "unexpected argument: " << a << std::endl;
                return 2;
            }
        }
        if (discDataRoot.empty())
        {
            std::cerr << "discovery mode requires <dataRoot>" << std::endl;
            return 2;
        }
        if (dumpDir)
        {
            std::error_code ec;
            std::filesystem::create_directories(*dumpDir, ec);
            if (ec)
            {
                std::cerr << "cannot create dump dir: " << *dumpDir << ": " << ec.message() << std::endl;
                return 2;
            }
        }

        const auto files = discoverFiles(discDataRoot);
        int pass = 0, fail = 0, skip = 0, loadFail = 0, getDataFail = 0, createFail = 0;
        for (const auto &absPath : files)
        {
            namespace fs = std::filesystem;
            const std::string rel = fs::relative(fs::path(absPath), fs::path(discDataRoot)).generic_string();
            const auto r = runRoundtrip(absPath);
            std::cout << statusString(r.status) << "\t" << rel << "\t" << r.detail << "\n";

            // Dump normalized expected/actual for every non-PASS, non-SKIP file.
            if (dumpDir && r.status != RoundtripResult::Status::pass && r.status != RoundtripResult::Status::skip)
            {
                dumpDocuments(absPath, rel, *dumpDir, r.status);
            }
            switch (r.status)
            {
            case RoundtripResult::Status::pass:
                ++pass;
                break;
            case RoundtripResult::Status::fail:
                ++fail;
                break;
            case RoundtripResult::Status::skip:
                ++skip;
                break;
            case RoundtripResult::Status::loadFail:
                ++loadFail;
                break;
            case RoundtripResult::Status::getDataFail:
                ++getDataFail;
                break;
            case RoundtripResult::Status::createFail:
                ++createFail;
                break;
            }
        }
        std::cout << "\ndiscovery: " << pass << " PASS, " << fail << " FAIL, " << skip << " SKIP, " << loadFail
                  << " LOADFAIL, " << getDataFail << " GETDATAFAIL, " << createFail << " CREATEFAIL\n";
        return 0;
    }
    else
    {
        std::cerr << "unknown mode: " << mode << " (expected 'regression' or 'discovery')" << std::endl;
        return 2;
    }
}
