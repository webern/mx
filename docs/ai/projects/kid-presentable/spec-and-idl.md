# spec-and-idl: IDLs and specification languages — a reference survey

A facts-only catalog of interface definition languages, schema languages, and specification
systems that could bear on generating (a) a multi-language plain-data API layer like `mx::api`,
(b) translation/mapping logic like `mx::impl`, and (c) schema-driven XML DOM binding. No
recommendations, no rejections — just what exists.

Compiled 2026-07-20 via a multi-agent research pass: 20 sources fetched, 97 claims extracted,
the top 25 adversarially verified with 3-vote panels. Statements citing a URL trace to that
pass. Rows without a citation come from the tools' own documentation and were not independently
verified here.

## The three generation surfaces

| Surface | What would be generated | Categories that address it |
|---------|------------------------|---------------------------|
| (a) api layer | plain-data types (structs/enums/choices) idiomatic per language | serialization IDLs, FFI IDLs, schema-to-code tools |
| (b) impl layer | translation logic between two typed models | model-transformation / BX languages, mapping tools |
| (c) DOM binding | schema-driven XML parse/serialize against a typed model | XSD data-binding generators, XML-native transform languages |

No single surveyed system covers all three. General-purpose IDLs generate types and wire codecs
(a, sometimes c for their own format); XSD binders generate (c) for one language at a time;
model-to-model translation (b) is the domain of the OMG/MDE and BX tool families plus commercial
mapping products.

## 1. Schema-driven serialization IDLs

Each defines a schema/IDL language and generates types plus encode/decode for its own wire
format. A 2022 benchmark (Viotti & Kinderkhedia, arXiv:2201.03051) covers ASN.1, Avro, Bond,
Cap'n Proto, FlatBuffers, Protocol Buffers, and Thrift, and observes that schema-driven
serialization specifications "typically define custom schema languages" rather than sharing a
standardized one (hedged "typically": Avro's schema language is itself JSON).

| System | Schema language | First-party language targets | Notes |
|--------|----------------|------------------------------|-------|
| Protocol Buffers | `.proto` (proto2/proto3, now "editions") | C++, C#, Dart, Go, Java, Kotlin, Obj-C, PHP, Python, Ruby; JS and Rust via sibling projects (protobuf-javascript, protobuf-es, prost) | `protoc` plugin protocol for custom generators; canonical JSON mapping defined; BSD-3 |
| FlatBuffers | `.fbs` | C, C++, C#, Dart, Go, Java, JS/TS, Kotlin, Lobster, Lua, PHP, Python, Rust, Swift | zero-copy access (no parse step); schema evolution rules; Apache-2.0 |
| Cap'n Proto | `.capnp` | C++ (reference); maintained Rust, Go, Java, Python implementations | zero-copy; includes an RPC system; MIT |
| Apache Thrift | `.thrift` | ~25+ (C++, C#, Erlang, Go, Java, JS/Node, PHP, Python, Ruby, Rust, Swift, …) | IDL + RPC framework combined; Apache-2.0 |
| Apache Avro | JSON schemas or Avro IDL (`.avdl`) | Java (primary), C, C++, C#, Python, Ruby, PHP, Rust, JS | codegen optional (dynamic use possible); built-in schema-resolution/evolution rules; Apache-2.0 |
| ASN.1 | X.680-series ITU-T standard (since 1984) | per-compiler; open-source `asn1c` (C); mature commercial compilers (OSS Nokalva, Objective Systems) cover C/C++/C#/Java/Python | multiple standardized encodings from one schema: BER/DER/PER/OER, **XER (XML)**, JER (JSON); used in X.509, LDAP, 3GPP |
| Microsoft Bond | `.bond` | C++, C#, Python | benchmarked at v9.0.4 in the 2022 paper |

## 2. API/service description languages

Service-shaped rather than document-shaped: they describe operations plus the data types those
operations carry.

- **Smithy** (AWS, Apache-2.0, smithy.io). Protocol-agnostic shape/service model: the same
  definition binds to multiple wire protocols (REST JSON, RPC v2 CBOR, AWS JSON, AWS Query)
  without changing the model. Extensibility is via **traits** — declarative annotations layered
  onto the meta-model. Built-in tooling diffs model revisions for backward compatibility.
  Client SDK generators ship in ten languages (TypeScript, Python, Go, Rust, Kotlin, Swift,
  Java, Scala, Ruby, Dafny). The landing page lists server generation for five (TypeScript,
  Java, Python, Go, Rust), but repo-level status varies: Go server codegen is documented by its
  own README as work-in-progress, and Python server support is implemented via a Rust host
  crate (`aws-smithy-http-server-python`) rather than an independent generator. Tooling is
  JVM-based.
- **TypeSpec** (Microsoft, MIT, typespec.io). A TypeScript-flavored DSL with composition,
  mixins, and templates. Its historical strategy (per a 2023 maintainer discussion,
  github.com/microsoft/typespec/discussions/2392) was to emit OpenAPI 3 and lean on the OpenAPI
  toolchain for downstream codegen; first-party emitters now include OpenAPI 3, JSON Schema,
  and Protobuf, plus client-code emitters (C#, Java, JavaScript/TypeScript, Python) that
  reached 1.0 in 2025 as the Azure SDK generation path.
- **OpenAPI / JSON Schema**. Not IDLs in the classic sense but the largest codegen ecosystems:
  openapi-generator (~50+ client/server languages of varying quality), and per-language JSON
  Schema type generators (section 5).

## 3. FFI / binding-generation IDLs (in-memory, no wire format)

These generate cross-language glue to call a compiled core, converting types at the boundary
rather than serializing them.

- **UniFFI** (Mozilla, MPL-2.0, github.com/mozilla/uniffi-rs). Compiles Rust into a shared
  library and generates foreign bindings. The object model is defined either in a UDL interface
  file or via Rust proc-macros. First-class targets: Kotlin, Swift, Python, Ruby; third-party
  bindings exist for C# and Go (plus community efforts for other languages). Architected so
  external parties can add new language backends. Used in production in Firefox mobile and
  desktop; no 1.0 release yet.
- **Djinni** (dropbox/djinni, archived; now a fork family). A small IDL (enums, flags, records,
  interfaces) generating cross-language type declarations and interface bindings — in-memory
  gluecode passing live object references (e.g. `shared_ptr`), explicitly not a serialization
  or RPC IDL. The cross-language-cpp fork (djinni.xlcpp.dev) targets C++ paired with Java,
  Kotlin, Objective-C, Swift, Python, or C#. The Snapchat fork adds WASM support bridging
  C++ to JavaScript/TypeScript in browsers. Originated at Dropbox (2014), community-maintained
  since 2020.
- **WIT / WASM component model** (WebAssembly/component-model). An IDL describing component
  imports/exports; `worlds` define a component's complete contract; `wit-bindgen` generates
  guest/host bindings directly from `.wit` files. The type system goes beyond plain data:
  `resource` is a first-class abstract type passed only by owned/borrowed handle. Packages are
  namespaced and semver-versioned (`ns:pkg@1.0.0`) with `@since`/`@unstable`/`@deprecated`
  gates. WASM-only by design.
- **Source-derived generators** (the schema is an existing codebase, not an IDL file):
  - SWIG — reads C/C++ headers plus `.i` interface files; ~20 target languages.
  - diplomat (unicode-org) — annotated Rust bridge modules → C, C++, JS/TS, Dart bindings;
    drives ICU4X's multi-language distribution.
  - typeshare (1Password) — annotated Rust types → TypeScript, Kotlin, Swift, Go, Scala type
    mirrors (types only, no calls).
  - cbindgen (Rust → C headers), bindgen (C headers → Rust), cxx (macro-defined Rust⇄C++
    bridge).
- **Older/other interface IDLs**, for completeness: OMG IDL (CORBA; still the type language of
  OMG DDS), ZeroC Slice (C++, C#, Java, JS/TS, PHP, Python, Ruby, Swift, MATLAB; GPL/commercial),
  Fuchsia FIDL (C, C++, Rust, Dart), COM/WinRT MIDL 3.

## 4. XML schema data binding (surface (c))

Generators that read an XSD and emit a typed model plus XML parse/serialize for one language.
A long-standing taxonomy (Bourret, rpbourret.com/xml/XMLDataBinding.htm) splits products into
design-time (codegen from schema) and run-time (reflection-based mapping); it also records that
most products support only a subset of XML Schema — mixed content, wildcards, substitution
groups, key/keyref, and complex-type restriction are the commonly unsupported features.

| Language | Tool | Facts |
|----------|------|-------|
| C++ | CodeSynthesis XSD (codesynthesis.com/projects/xsd/) | XSD → statically-typed C++; two mappings: C++/Tree (classes + parse-to-tree + serialize) and C++/Parser (typed SAX-style callbacks); customizable type mapping; GPLv2 + FLOSS exception, commercial option |
| C/C++ | gSOAP (genivia.com) | `wsdl2h` + `soapcpp2` generate per-type `soap_read_T`/`soap_write_T` roundtrip functions; XSD→native mapping with validation; developed since 2001, used against large schemas (ONVIF, HL7, eBay); GPLv2/commercial |
| Java | JAXB / XJC | XSD → model classes + `ObjectFactory`; unmarshal from stream/URL/DOM/SAX, marshal back; bundled with the JDK through Java 10, removed in Java 11, continued as Jakarta XML Binding |
| Java | Apache XMLBeans | XSD → typed interfaces; retains XML Infoset fidelity (element order, comments) across round-trips; three API surfaces (typed XmlObject, cursor-level XmlCursor, reflective SchemaType); BEA 2003 → Apache → retired 2014 → revived 2018 under POI; v5.3.0 Dec 2024 |
| Java | Eclipse EMF | imports XSD into an Ecore metamodel and generates a Java model with XML persistence; substrate for the MDE tools in section 5 |
| Rust | xsd-parser (github.com/Bergmann89/xsd-parser) | five-stage pipeline (parse → interpret → optimize → generate → render) with a language-agnostic intermediate model; serde-based or direct quick_xml serialization (async-capable); user hooks at multiple stages, can splice in hand-written types; v1.5.0, validated against BPMN 2.0/ONIX/XJustiz; constraint-validation codegen not yet implemented |
| Python | xsdata; generateDS | XSD/WSDL/DTD → dataclasses (pydantic/attrs plugins); generateDS is the older equivalent |
| .NET | xsd.exe; LinqToXsd | XSD → C# classes for XmlSerializer; LinqToXsd gives typed XML programming |
| Go | xgen (xuri/xgen) | XSD parser/generator; README also lists C, Java, Rust, TypeScript output modes |
| JS/TS | — | no actively maintained XSD data-binding generator surfaced in this pass |

In-repo precedent: `gen/` is a house XSD-to-code press (XSD → IR → Plates → Mustache templates)
with targets `gen/cpp` (the product `mx::core` model), `gen/schema` (JSON Schema), and Go/C toy
implementations proving language-agnosticism; adding a target touches no generator Python. See
`gen/DESIGN.md`.

## 5. Schema-to-idiomatic-code tools (JSON-schema family)

- **quicktype** (glideapps/quicktype, Apache-2.0). Generates strongly-typed models and
  serializers from JSON samples, JSON Schema, TypeScript, and GraphQL queries; 25+ target
  languages including C++, Rust, Python, Java, and TypeScript/JS; can infer types from a live
  API URL; extensible with new target-language renderers via its class architecture, with
  community targets solicited; ships as web app, CLI (npm), and library (quicktype-core).
- Per-language JSON Schema generators: typify (Rust, Oxide), datamodel-code-generator (Python),
  NJsonSchema (C#), json-schema-to-typescript (TS).

## 6. Model-transformation and bidirectional-transformation languages (surface (b))

The academic/OMG lineage for generating or specifying the *translation between two models* —
the mx::impl-shaped problem.

- **XML-native transforms**: XSLT 3.0 and XQuery 3.1 (W3C standards; XML→XML/anything;
  processors: Saxon across Java/.NET/C with Python bindings, libxslt for 1.0). Specification
  languages for transformation logic over the DOM itself, unidirectional per stylesheet.
- **QVT** (OMG). Three languages in one spec: Relations (QVT-R, declarative), Core, and
  Operational (QVTo, imperative). QVT-R is the OMG's official standard language for specifying
  bidirectional model transformations (Macedo & Cunha, SoSyM 15:783–810); the same paper
  records that "in part due to ambiguities and omissions in the original semantics, acceptance
  and development of effective tool support have been slow." Implementations: Eclipse QVTo
  (most used), Eclipse QVTd; research tools like Echo (embeds QVT-R into Alloy/SAT with
  least-change update semantics).
- **ATL** (Eclipse). Widely used rule-based transformation language; fundamentally
  unidirectional — bidirectionality exists only as research-layer add-ons (e.g. Echo's
  bidirectionalization of ATL subsets).
- **Triple Graph Grammars (TGG)**: declarative correspondence grammars from which forward,
  backward, and consistency-check transformations are derived (tools: eMoflon, MoTE).
- **Lenses / BX**: the programming-languages branch of bidirectionality. Boomerang
  (bidirectional string transforms via lenses), BiGUL (putback-based), BOTL, Augeas
  (production config-file lenses), Cambria (Ink & Switch; lens-based JSON schema migration,
  research prototype). Survey literature: Stevens, "A Landscape of Bidirectional Model
  Transformations" (GTTSE 2007/LNCS 5235) frames the field as having open research gaps; Hidaka
  et al., "Feature-Based Classification of Bidirectional Transformation Approaches" (SoSyM
  15:907–928, 2016) maps QVT, TGG, and lens-based approaches onto one feature model and notes
  that while several tools include at least partial bidirectionality support, how those
  capabilities relate to each other (and to the classical view-update problem) is not clear.
- **Eclipse Epsilon**: a family of task-specific model languages (ETL transformation, EGL
  generation, EVL validation) on EMF.
- **Commercial mapping codegen**: Altova MapForce — graphical mapping across XML/JSON/DB/EDI
  that generates standalone program code in Java, C#, or C++.

## 7. Allium

The name refers to **juxt/allium** (JUXT Ltd, juxt.github.io/allium): an "LLM-native"
behavioral specification language — minimal syntax for entities and event-driven rules
(when/requires/ensures preconditions and outcomes), authored conversationally with an LLM. It
has no compiler, no runtime, and generates no code, schemas, or bindings; an LLM is the
intended consumer/producer. Scope is deliberately behavioral (rules, triggers, projections,
surfaces) and excludes implementation artifacts such as database schemas and API designs.
Models compose via a `use` keyword resolving immutable references (git SHAs/content hashes).
Self-described as early-stage. It is not a data-schema or API IDL in the protobuf/Thrift/XSD
sense.

## Sources

Primary: smithy.io · github.com/mozilla/uniffi-rs · djinni.xlcpp.dev ·
github.com/WebAssembly/component-model (WIT.md) · arxiv.org/abs/2201.03051 ·
codesynthesis.com/projects/xsd · genivia.com/dev.html · github.com/Bergmann89/xsd-parser ·
github.com/glideapps/quicktype · quicktype.io · github.com/juxt/allium · juxt.github.io/allium ·
Springer: Stevens (10.1007/978-3-540-88643-3_10), Hidaka et al. (10.1007/s10270-014-0450-0),
Macedo & Cunha (10.1007/s10270-014-0437-x).
Secondary: en.wikipedia.org/wiki/Apache_XMLBeans · rpbourret.com/xml/XMLDataBinding.htm ·
thorben-janssen.com (JAXB/XJC) · github.com/microsoft/typespec/discussions/2392.
