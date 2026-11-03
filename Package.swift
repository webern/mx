// swift-tools-version: 5.9

// TEMPLATE for the machine-generated release manifest -- the Package.swift that
// exists only at a tagged release commit ("B" in docs/ai/design/release-process.md).
// The _release-target-swift workflow interpolates the three MX_RELEASE
// placeholders (version, url, checksum) and the release-build prep job commits
// the result as B; commit R then restores the source manifest, so every tip of
// main keeps the source-only Package.swift.
//
// KEEP IN SYNC: mxSourceTarget below must stay byte-for-byte identical to the
// target in the repository's Package.swift, so that a release built from source
// (non-Apple hosts, or MX_SOURCE_BUILD=1) is the same build a sibling checkout
// gets. When Package.swift's target changes, change this copy too.

import PackageDescription

// Release: 0.7.1
//
// SwiftPM evaluates this manifest on the consumer's machine, so the selection
// below runs there:
//   - Apple hosts get the published prebuilt XCFramework by default -- no
//     environment setup. Setting MX_SOURCE_BUILD=1 opts a Mac into compiling
//     the tagged release from source instead.
//   - Non-Apple hosts always compile from source; an XCFramework binaryTarget
//     cannot resolve there.

let mxSourceTarget: Target = .target(
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
    ]
)

let mxTarget: Target
#if os(macOS)
if Context.environment["MX_SOURCE_BUILD"] != nil {
    mxTarget = mxSourceTarget
} else {
    mxTarget = .binaryTarget(
        name: "Mx",
        url: "https://github.com/webern/mx/releases/download/v0.7.1/Mx.xcframework.zip",
        checksum: "3edb5c9fb812a76a9943947920adbbc050e77f2e496e238e4a07be54a95e0668"
    )
}
#else
mxTarget = mxSourceTarget
#endif

let package = Package(
    name: "mx",
    products: [
        .library(name: "Mx", targets: ["Mx"]),
    ],
    targets: [mxTarget],
    cxxLanguageStandard: .cxx20
)
