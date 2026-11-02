# The XML backend seam (`mx::xml`)

Tracking issue: webern/mx#478.

## The problem

`mx::core` is generated code that reads and writes an XML DOM. Until 0.6 that DOM was pugixml's,
called directly: every generated parser took a `pugi::xml_node`, every serializer appended to one,
and `mx::core::parse` took a `pugi::xml_document`. That was the right call when the typed model was
designed (see `mx-core-plan.md` §2.1: the earlier `ezxml` virtual-interface wrapper allocated a
`shared_ptr` and copied a `std::string` per node access, and retiring it made the core fast). It
left two problems for people who build mx into something larger.

1. **Symbol collisions.** A consumer who already links pugixml got two definitions of every
   `pugi::` symbol, one from their copy and one from mx's vendored copy. Whether the linker
   complained or silently picked one depended on versions and build flags.
2. **Consumers who want another XML library.** Some projects standardize on libxml2, tinyxml2 or a
   platform parser, and do not want a second XML library in the binary at all.

## Step one: the rename (PR #479)

The vendored pugixml moved to `src/private/mxpugi/` and was compiled under the `mxpugi` namespace
(`scripts/vendor_pugixml.py` does the renaming; `make vendor-pugixml VERSION=x.y` refreshes it).
That ended the collisions: mx does not define a `pugi::` symbol, and `make symbol-check` fails CI
if one ever appears. It did nothing for problem 2, and it left `mxpugi::` spelled across the generated
code, the runtime, `mx::api` and the tests.

## Step two: the seam (this document)

Everything on mx's side of the XML library now goes through three value types in
`src/private/mx/xml/Xml.h`:

- `mx::xml::Node` and `mx::xml::Attribute` are trivially copyable handles into a tree. Copying one
  copies the reference, the way copying a pointer does. Each wraps the backend's own handle type
  (`Backend::NodeHandle`, `Backend::AttributeHandle`) and every member is an inline, non-virtual
  one-line forwarder to a static function of `mx::xml::Backend`.
- `mx::xml::Document` owns a `Backend::DocumentHandle`. It loads from a file, a string or a stream,
  saves to a file or a stream, and exposes `root()` (the document node, where the declaration and
  the doctype are appended) and `documentElement()`.

The signatures that matter are now

```cpp
Result<Document> mx::core::parse(const mx::xml::Document &, const ParseContext &);
void mx::core::serialize(const Document &, mx::xml::Document &);
void mx::core::serializeWithAttribution(const Document &, mx::xml::Document &);
void mx::core::checkIds(const mx::xml::Document &, const DiagnosticHandler &);
void mx::core::repairIds(mx::xml::Document &, const DiagnosticHandler &);
```

and every generated `parseX` / `serializeX` takes `mx::xml::Node`.

### What a backend is

`src/private/mx/xml/Concept.h` holds the C++20 concept `mx::xml::XmlBackend<B>`. It is the whole
contract: the three handle types with their requirements (trivially copyable and default
constructible to null for nodes and attributes; default constructible, non-copied and movable for
the document) and the static functions the seam forwards to, each with its return type. `Xml.h`
has `static_assert(XmlBackend<Backend>)`, so a backend that misses something fails to compile with
a message that names the requirement.

The surface is exactly what `mx::core`, `mx::api` and the tests use. Strings come back as
`const char *` owned by the tree, as pugixml returns them, so a name or value lookup never
allocates. Strings go in as `std::string_view`. The seam does not expose the backend's own
handle, so nothing on the mx side can come to depend on one backend by accident. When a member of
the seam loses its last caller, it goes, and so does its line in the concept: a smaller contract is
less for a backend author to write.

### How the backend is chosen

`src/private/mx/xml/Backend.h` includes the header named by `MX_XML_BACKEND_HEADER`, defaulting to
`"mx/xml/pugixml/Backend.h"`. That header must define `struct mx::xml::Backend`.

The pugixml adapter (`src/private/mx/xml/pugixml/Backend.{h,cpp}`) is itself parameterized on two
macros with defaults: `MX_XML_PUGI_HEADER` (`"mxpugi/mxpugi.hpp"`) and `MX_XML_PUGI_NAMESPACE`
(`mxpugi`). A single `namespace pugixml_ns = MX_XML_PUGI_NAMESPACE;` alias inside the adapter keeps
the macro to one place. The adapter does not assume anything particular to the vendored copy; it
compiles against a stock pugixml from 1.9 on (a movable `xml_document`), in pugixml's default char
mode with its STL support enabled.

### Why not a virtual interface, and why not a templated core

A virtual interface (what `ezxml` was) costs a virtual call per node access and, in `ezxml`'s case,
an allocation. The core walks every node and attribute of every file it reads, so that cost is paid
hundreds of thousands of times per score.

A core templated on the backend would be zero-cost but would compile the entire generated model
(hundreds of translation units) once per backend, and would move the whole model into headers. The
core compiles once; that is a property worth keeping.

The seam gets the zero-cost result a different way: the backend is fixed at configure time, so the
forwarders are ordinary inline functions and an optimizing build sees straight through them. The
expected cost is zero in Release. In Debug nothing inlines, so each operation pays one call per
layer (`Node::firstChild` into `Backend::firstChild` into pugixml).

Measured when the seam landed, on the corert suite (842 corpus files), warm build, second run, with
the Docker and CMake overhead included in the wall time:

| Build   | Before the seam | With the seam |
|---------|-----------------|---------------|
| Debug   | 67 s            | 66.8 s        |
| Release | 9.8 s           | 11.5 s        |

The Release numbers were taken on a loaded machine and the after-number also includes the harness
now crossing the seam by text twice per file; the bare Release test binary ran in 6.5 s wall with
three runs within 0.3 s of each other. In the Release `libmx_core.a`, seven symbols are defined in
`mx::xml`: the five stream and file functions of the pugixml adapter plus its two mapping helpers.
Every handle forwarder was inlined away.

### Test strategy

The tests link `mxpugi` for their own DOM work (`Compare`, `Fixer`, `Normalize`, the api round-trip
canonicalizers, `IdSchemaDriftTest` reading the XSD). When a test needs to hand something to
`mx::core` or read what it wrote, it crosses the seam as text: save the mxpugi document to a string,
load the string into an `mx::xml::Document`, call the core, serialize into an `mx::xml::Document`,
save that to a string, load it back into mxpugi. The corert harness does exactly this
(`CoreRoundtripImpl.cpp`, steps 3 and 4). A test that only needs the core's view of a document uses
`mx::xml::Document` directly. Test code uses one or the other and never casts between them, which is
what a consumer who brings a different XML library would have to do.

## Configure-time selection

The choice is the CMake cache variable `MX_XML_BACKEND`, with three values
(`set_property(CACHE ... STRINGS)` lists them; anything else is a configure error). CMake prints
the chosen value as `mx: MX_XML_BACKEND=...`.

- `vendored` (the default, always; never chosen by detecting another pugixml target): the pugixml
  adapter against mx's private `mxpugi` copy. `mx_core` links `mxpugi` and does not get an extra
  definition, so this mode is byte-for-byte the build mx had before the option existed. No
  `pugi::` symbol; nothing to install.
- `pugixml`: the pugixml adapter against the consumer's own pugixml. `MX_PUGIXML_TARGET` (cache
  string, default `pugixml`, which is the target upstream pugixml's CMake defines as an interface
  over `pugixml-static`) names the CMake target; it has to exist when `add_subdirectory(mx)` runs,
  and the configure fails with a `FATAL_ERROR` telling the consumer to create it (FetchContent,
  `find_package`, or `add_subdirectory` on a checkout) otherwise. `mx_core` links that target
  `PUBLIC` and gets the `PUBLIC` compile definitions `MX_XML_PUGI_NAMESPACE=pugi` and
  `MX_XML_PUGI_HEADER="pugixml.hpp"`. `mxpugi` is not linked into `mx_core` or `mx`.
- `custom`: the consumer provides `MX_XML_BACKEND_HEADER` (the include path of a header that
  defines `struct mx::xml::Backend` satisfying `XmlBackend`, written the way it would appear
  between the quotes of an `#include`) and `MX_XML_BACKEND_TARGET` (a CMake target, INTERFACE or
  library, carrying the header's include directory, any sources, and the link to the XML library;
  it exists before `add_subdirectory(mx)`, so a source on it that includes `mx/xml/Concept.h` has
  to carry mx's `src/private` include path itself).
  Both are required; the configure fails naming them otherwise. `mx_core` links the target
  `PUBLIC`, gets the `PUBLIC` compile definition `MX_XML_BACKEND_HEADER="..."`, and does not
  compile `src/private/mx/xml/pugixml/Backend.cpp` (it includes pugixml). The `static_assert` in
  `Xml.h` is the check that the struct satisfies the concept. This mode is a documented contract:
  there is no CI job and no example backend beyond the pugixml one.

The link and the definitions are `PUBLIC` on `mx_core` because `Xml.h` is included by the core's
headers: the `mx` library, the tests and the consumer all have to see the same backend.

In every mode the `mxpugi` target exists and the test executables (`mxtest-core`,
`mxtest-core-dev`, `mxtest`, `mxtest-api-roundtrip`) link it, because the tests do their own DOM
work with it and cross the seam as text (the test strategy above). In the `pugixml` mode a test
translation unit therefore includes both `pugixml.hpp` and `mxpugi.hpp`. The two have distinct
file names, include guards and namespaces, and the `PUGIXML_*` configuration macros are each
behind `#ifndef` in both, so this compiles. In the non-vendored modes `mxpugi` is excluded from
`all`, so a consumer who builds only the `mx` target does not compile a second pugixml (a
default `MX_API=ON` build of everything still does, because `mxtest` links it).

The `pugixml` mode has a gate. `cmake/backend-pugixml/CMakeLists.txt` is a consumer project that
fetches pugixml 1.16 with FetchContent, sets `MX_XML_BACKEND=pugixml`, `MX_CORE_DEV=ON` and
`MX_API=OFF` (the backend lives in `mx_core`; the core suites are what runs), and
adds the repository root as a subdirectory. `make backend-pugixml-test` configures it into
`build/backend-pugixml`, builds it, runs the corert and core unit binaries, and then checks with
`nm -g | c++filt` that `libmx_core.a` in that tree references `pugi::` and does not hold an
`mxpugi::` symbol, which is the proof that the mode took. The CI job `backend: pugixml` runs that target.
`make symbol-check` is the gate for the vendored build only; in the pugixml mode `mx_core`
references `pugi::` on purpose.

The Swift package (`Package.swift`) and the release XCFramework always build the vendored copy;
there is no switch for them. The Emscripten CI job builds vendored too.

No public header under `src/include` names an XML type in any mode; the choice is invisible to
`mx::api` users.
