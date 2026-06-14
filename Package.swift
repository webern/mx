// swift-tools-version: 5.9
import PackageDescription

// mx is consumed as a Swift package in one of two modes, selected by the
// MX_LOCAL_CHECKOUT environment variable. The name mirrors the repo's existing
// local-vs-Docker build switch (MX_RUNNING_IN_DOCKER) and reads as "consume mx
// from a local checkout":
//
//   set   -> `Mx` is a source library compiled from this checkout, for
//            sibling-checkout development (a consumer using `.package(path:)`).
//   unset -> `Mx` is the published xcframework binary release. That pipeline is
//            follow-up work and is not wired up yet, so this arm fails loudly
//            rather than silently resolving to a nonexistent artifact.
let useLocalCheckout = Context.environment["MX_LOCAL_CHECKOUT"] != nil

let mxTarget: Target
if useLocalCheckout {
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
} else {
    fatalError(
        "The Mx binary release is not published yet. Build with "
            + "MX_LOCAL_CHECKOUT=1 against a sibling mx checkout; the "
            + "binary-release arm is follow-up work."
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
