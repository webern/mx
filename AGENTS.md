# mx

@./README.md

`mx` is a MusicXML C++ library. The product surface is `mx::api`, a simplified and narrowed
interface of MusicXML backed by the strongly-typed `mx::core` model.

## Repository layout

```
mx/
  AGENTS.md             <- you are here
  Makefile              <- top-level build driver
  Dockerfile            <- mx-sdk image with toolchains and dev tools
  CMakeLists.txt        <- C++ project
  data/                 <- MusicXML test corpus (large, see data/README.md)
  docs/ai/design/       <- design docs
  src/                  <- C++ sourcecode root
  src/include/mx/api/   <- the public mx::api headers; mx's public interface
  src/private/          <- C++ source
    pugixml/            <- vendored pugixml 1.15 (XML parser and raw DOM)
    mx/core/            <- mx::core's hand-written runtime (outside of mx/core/generated)
    mx/core/generated/  <- MusicXML XSD C++ typed model (generated, see gen/cpp)
    mx/api/             <- mx::api implementation
    mx/impl/            <- the api to core translation layer
    mx/utility/         <- helpers
    mx/examples/        <- api example programs
    mxtest/core/        <- mx::core unit tests (small)
    mxtest/corert/      <- C++ core roundtrip test (deserializes and serializes the test corpus)
    mxtest/import/      <- TODO: not sure
    mxtest/probe/       <- TODO: not sure
    mxtest/validate/    <- TODO: not sure
    mxtest/api/         <- tests for the mx::api layer
    mxtest/impl/        <- tests for the mx::impl layer
    mxtest/file/        <- file helpers for the mx::api tests (TODO: verify)
    mxtest/control/     <- TODO: not sure: dead code?
    cpul/               <- vendored Catch2 test runner
  gen/                  <- code generator system (see gen/README.md)
    test/go/            <- A toy Go implementation of MusicXML XSD for gen validation
    test/c/             <- A toy C implementation of MusicXML XSD for gen validation
```

## Build system

### Docker (mx-sdk)

Docker is used for tool reproducibility. The `Makefile` has sections that are invoked on the host
and others that are invoked inside the container. The `MX_RUNNING_IN_DOCKER` env var drives this
distinction.

In general, we should strive to make build processes reproducable on developer machines and in CI by
leveraging the `mx-sdk` image and extending it with new tools as they are needed.

### Makefile targets

The `Makefile` serves as the entrypoint of build processes. It calls `cmake` which produces deeper,
generated makefiles in the build directory. You should lean heavily on our top-level `Makefile` and
suggest improvements when it doesn't have what you need.

Run `make help` for the target list.

## The corert (core roundtrip) test

Runs with `make test-core-dev`.

This test suite deserializes and reserializes the test corupus with `mx::core`. It can be compiled
without compiling the `mx::impl` and `mx::api` layes. This provides a mechanism for innovating on
the generated code and templates found in `gen/cpp` without fixing the `mx::impl` layer on every
change to `mx::core`. i.e. you can defer integrating `mx::core` changes with `mx::impl` (and by
extension `mx::api`) until you are ready.

### Flow (same in all three languages)

1. **Discover** eligible `.xml`/`.musicxml` files under `data/`, (excluding certain directories, and
   marker files. See `data/README.md)
   - unparseable files have a sibling file ending with `.invalid` and are skipped.
2. For each file: Load the XML into a DOM, make certain expected alterations, parse it with
   `mx::core`, serialize it back to XML, normalize the output, and compare the two DOMs.
3. Report pass/fail per file.

## Generator (`gen/`)

`gen/` is a Python code generator (`python3 -m gen`) that reads the MusicXML XSD and emits typed
serialization/deserialization libraries. See:
- `gen/AGENTS.md`: your entrypoint
- `gen/DESIGN.md`: how it was designed
- `gen/README.md`: human summary

What you need to know right now is that `gen/cpp` is where our MusicXML types are coming from. Run
{{TODO: put the right Makefile command here}} to regenerate the C++ types.

## Quality gates

Run `make fmt` for a stable version of clang format.
Run `make check` to see if you will pass in CI
Run `make test-core-dev` to run the `corert` tests, especially if you are working in `mx/core`.
Run `make test` to run all the tests. (Slow and heavy on the local machine)

Look at what will run in CI `.github/workflows/ci.yaml` and anticipate issues there when coding
locally.

## Key files to understand

TODO: top ten files in the C++ codebase that an agent will very often need to read. Let's list them
here and what they are.
