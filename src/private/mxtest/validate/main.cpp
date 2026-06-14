// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Every parsed corpus document is serialized and the output is xmllint-
// validated against the 4.0 XSD -- the mechanical proof that the clamp
// policy emits only valid XML, end to end. This tool does the
// parse+serialize half: it writes each round-tripped document (plus the
// default-constructed Document) to the given output directory; `make
// validate-cpp` runs xmllint over the results.

#include "mx/core/generated/DefaultsProbe.h"
#include "mx/core/generated/Document.h"
#include "mxtest/corert/CoreRoundtripImpl.h"

#include "pugixml/pugixml.hpp"

#include <cstdio>
#include <filesystem>
#include <string>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::fprintf(stderr, "usage: mxtest-validate <output-dir>\n");
        return 2;
    }
    const std::filesystem::path outDir{argv[1]};
    std::filesystem::create_directories(outDir);

    // Mechanical default-construction probes first: every type's
    // natural zero must survive its own strict parser...
    mx::core::roundTripDefaults();
    // ...and the default Document must be schema-valid (xmllint checks it
    // with the corpus outputs below).
    {
        pugi::xml_document doc;
        mx::core::serialize(mx::core::Document{}, doc);
        doc.save_file((outDir / "default-document.xml").string().c_str(), "  ");
    }

    int failures = 0;
    int written = 0;
    for (const auto &path : mxtest::corert::discoverInputFiles())
    {
        pugi::xml_document input;
        if (!input.load_file(path.c_str(), pugi::parse_default | pugi::parse_doctype))
        {
            std::fprintf(stderr, "LOAD FAIL %s\n", path.c_str());
            ++failures;
            continue;
        }
        auto parsed = mx::core::parse(input);
        if (!parsed)
        {
            if (parsed.error().code == mx::core::ErrorCode::unsupportedVersion)
            {
                continue; // version-gated: not this model's document
            }
            std::fprintf(stderr, "PARSE FAIL %s [%s] %s\n", path.c_str(), parsed.error().path.c_str(),
                         parsed.error().message.c_str());
            ++failures;
            continue;
        }
        pugi::xml_document output;
        mx::core::serialize(parsed.value(), output);
        std::string flat = mxtest::corert::toTestName(path);
        for (auto &c : flat)
        {
            if (c == '/' || c == '\\')
            {
                c = '_';
            }
        }
        output.save_file((outDir / flat).string().c_str(), "  ");
        ++written;
    }

    std::printf("mxtest-validate: %d outputs written, %d failures\n", written, failures);
    return failures == 0 ? 0 : 1;
}
