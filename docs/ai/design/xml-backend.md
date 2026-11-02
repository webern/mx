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

## The following step: configure-time selection

The next PR exposes the choice as a CMake option, `MX_XML_BACKEND`, with three values:

- `vendored` (default): the pugixml adapter against mx's private `mxpugi` copy. No `pugi::`
  symbols; nothing to install.
- `pugixml`: the pugixml adapter against the consumer's own pugixml (`MX_XML_PUGI_HEADER` set to
  `"pugixml.hpp"`, `MX_XML_PUGI_NAMESPACE` to `pugi`). `mxpugi` is not built.
- `custom`: the consumer names a header that defines `mx::xml::Backend` satisfying `XmlBackend`,
  passed through as `MX_XML_BACKEND_HEADER`. `mxpugi` is not built.

No public header under `src/include` names an XML type in any mode; the choice is invisible to
`mx::api` users.
