// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <string>
#include <vector>

namespace mxtest
{
namespace corert
{

// Result of running the core roundtrip flow on a single file.
struct CoreRoundtripResult
{
    bool ok = false;
    bool skipped = false; // declares a MusicXML version newer than supported
    std::string message;
    // Populated on failure (and only on failure) so the caller can diff:
    // the corresponding documents serialized after normalization.
    std::string expectedXml;
    std::string actualXml;
};

// Discover all eligible MusicXML inputs under `data/` (see AGENTS.md "The
// corert test"): regular `.xml`/`.musicxml` files, excluding the expected/
// testOutput/generalxml/smufl directories, `*.fixup.xml` sidecars,
// `*.features.xml` audit sidecars, the `corpus.xml` audit aggregate, and
// files with a `.invalid` sibling marker. Returns absolute paths, sorted.
std::vector<std::string> discoverInputFiles();

// Convert an absolute path under `data/` into the relative test name used
// as the test case name (e.g. `lysuite/ly01a_Pitches_Pitches.xml`).
std::string toTestName(const std::string &absolutePath);

// Run the per-file core roundtrip flow: load via mxpugi; pin the root
// version attribute to the harness baseline; hand the text to an
// mx::xml::Document and mx::core::parse it; serialize back into an
// mx::xml::Document and reload its text in mxpugi; normalize both the
// actual and a fresh-loaded expected; apply fixups to the expected;
// depth-first compare. Does not throw.
CoreRoundtripResult runCoreRoundtrip(const std::string &absoluteInputPath);

// Write a pair of `.expected.xml` / `.actual.xml` debug files to
// `data/testOutput/corert/` (flattened name). Both strings are the
// already-normalized XML payloads.
void writeFailureFiles(const std::string &testName, const std::string &expectedXml, const std::string &actualXml);

} // namespace corert
} // namespace mxtest
