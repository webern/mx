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
//       testOutput/, generalxml/, smufl/, *.fixup.xml, and *.invalid markers).
//       Print one line per file: PASS|FAIL|SKIP<TAB>relpath<TAB>detail.
//       Exit 0 always. Use to grow the pinned list.

#include "mx/api/DocumentManager.h"
#include "mxtest/corert/Compare.h"
#include "mxtest/corert/Fixer.h"
#include "pugixml/pugixml.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
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

bool isFixupSidecar(const std::filesystem::path &p)
{
    const std::string name = p.filename().string();
    constexpr std::string_view kSuffix = ".fixup.xml";
    return name.size() >= kSuffix.size() && name.compare(name.size() - kSuffix.size(), kSuffix.size(), kSuffix) == 0;
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
        if (isExcludedPath(rel) || isFixupSidecar(p) || hasInvalidMarker(p))
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

} // namespace

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "usage:\n"
                  << "  " << argv[0] << " regression <dataRoot> <baselineFile>\n"
                  << "  " << argv[0] << " discovery  <dataRoot>\n";
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
        const auto files = discoverFiles(dataRoot);
        int pass = 0, fail = 0, skip = 0, loadFail = 0, getDataFail = 0, createFail = 0;
        for (const auto &absPath : files)
        {
            namespace fs = std::filesystem;
            const std::string rel = fs::relative(fs::path(absPath), fs::path(dataRoot)).generic_string();
            const auto r = runRoundtrip(absPath);
            std::cout << statusString(r.status) << "\t" << rel << "\t" << r.detail << "\n";
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
