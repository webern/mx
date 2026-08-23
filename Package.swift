// swift-tools-version: 5.9
import PackageDescription

// This is mx's SOURCE manifest: `Mx` compiles from this checkout, so sibling
// development (a consumer using `.package(path:)`) needs no environment setup,
// and non-Apple hosts can build. Every tip of main carries this manifest.
//
// Released versions resolve differently: a consumer depending on
// `from: "X.Y.Z"` checks out the tagged release commit, which carries a
// machine-generated binary-default manifest (an XCFramework binaryTarget)
// interpolated from .github/release/Package.swift.template by the
// release-build workflow. That manifest exists only at tagged release commits,
// never at a tip of main. See docs/ai/design/release-process.md.
//
// KEEP IN SYNC: the target below must stay byte-for-byte identical to
// mxSourceTarget in .github/release/Package.swift.template, so a release built
// from source is the same build a sibling checkout gets.

// SPM globs every C++ translation unit under `src` (all of which live in
// `src/private`), minus the Catch2 runner, the test suites, and the example
// programs -- the only files carrying their own main(). The public surface
// is the mx::api headers under `src/include`; `src/private` is added to the
// internal header search path so the model can include `mx/core/...`; and
// `src/private/pugixml` is added so sources can use the canonical
// `#include "pugixml.hpp"` form that the CMake build also exposes.
let mxTarget: Target = .target(
    name: "Mx",
    path: "src",
    exclude: [
        "private/cpul",
        "private/mxtest",
        "private/mx/examples",
    ],
    publicHeadersPath: "include",
    cxxSettings: [
        .headerSearchPath("private"),
        .headerSearchPath("private/pugixml"),
    ]
)

let package = Package(
    name: "mx",
    products: [
        .library(name: "Mx", targets: ["Mx"]),
    ],
    targets: [mxTarget],
    cxxLanguageStandard: .cxx20
)
