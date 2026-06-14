// swift-tools-version: 5.9
import PackageDescription

// mx is consumed as a Swift package in one of two modes:
//
//   default            -> `Mx` is a source library compiled from this checkout,
//                         for sibling-checkout development (a consumer using
//                         `.package(path:)`). This is the default, so consumers
//                         need no environment setup -- just a checkout.
//   MX_BINARY_RELEASE  -> `Mx` is the published xcframework binary release. That
//                         pipeline is follow-up work and is not wired up yet, so
//                         this arm fails loudly rather than silently resolving to
//                         a nonexistent artifact.
let useBinaryRelease = Context.environment["MX_BINARY_RELEASE"] != nil

let mxTarget: Target
if useBinaryRelease {
    fatalError(
        "The Mx binary release is not published yet. Build from a source "
            + "checkout (the default); the binary-release arm is follow-up work."
    )
} else {
    // SPM globs every C++ translation unit under `src` (all of which live in
    // `src/private`), minus the Catch2 runner, the test suites, and the example
    // programs -- the only files carrying their own main(). The public surface
    // is the mx::api headers under `src/include`; `src/private` is added to the
    // internal header search path so the model can include `mx/core/...` and
    // `pugixml/...`.
    mxTarget = .target(
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
}

let package = Package(
    name: "mx",
    products: [
        .library(name: "Mx", targets: ["Mx"]),
    ],
    targets: [mxTarget],
    cxxLanguageStandard: .cxx20
)
