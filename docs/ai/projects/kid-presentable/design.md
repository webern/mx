# kid-presentable: the mx::api polyglot design

Status: proposed (2026-07-17). A realistic architecture for exposing `mx::api` to languages
other than C++.

## TL;DR

Do not bind the C++ classes. Ship a **document-granularity boundary**: a micro C ABI (~8
functions, hand-written once, frozen forever) that exchanges whole scores as **encoded bytes**
("Score JSON"), plus an **api model document** — a house IDL describing every `mx::api` type —
fed to the existing `gen/` press to generate, per language, an idiomatic native mirror of
`ScoreData` and its codec. C++ keeps the hand-written headers; a generated drift gate locks
them to the model. The engine (mx::core + mx::impl) compiles once — natively or to WASM — and
every language talks to it through the same two calls: `xml → score bytes`, `score bytes → xml`.

## The three facts the design rests on

**1. `mx::api` already has value semantics at document granularity.** `DocumentManager::getData`
returns the entire `ScoreData` by value; `createFromScore` takes it by value. There is no
incremental mutation API, no callbacks, no streaming, no lazy object graph. The functional
surface, seen from outside, is exactly two pure functions plus an error channel:

```
parse:     xml bytes  -> ScoreData | ApiError
serialize: ScoreData  -> xml bytes | ApiError
```

(The document-handle registry exists to bridge to `mx::core` via `getDocument`, which foreign
languages cannot use anyway.) A boundary that copies the whole score per call therefore loses
*nothing* the C++ api doesn't already give up — the C++ api is message passing in disguise.

**2. The value of mx is `mx::impl`, and it cannot be generated.** The doctrine's principles
(tick resolution replacing divisions/backup/forward, chord-tag rules, tie double-encoding,
denormalized effective state) are implemented as tens of thousands of lines of hand-written,
corpus-hardened translation logic. "Polyglot" must mean *binding to* that compiled engine —
native library or WASM — never re-implementing it per language.

**3. The repo already owns a language-agnostic IDL machine.** `gen/` (XSD → IR → Plates →
Mustache press) generates C++, Go, C, and JSON Schema today, with agnosticism structurally
enforced ("adding a target touches no Python"). The question "should the API be hoisted to an
interface definition language?" is really "should gen/ get a second input?" — and the answer
is yes.

## Architecture

```
                      api model (the IDL)            <- hand-owned, docs’ source of truth
                     /       |        \
        [gen press] /        |         \ [gen press]
                   v         v          v
     C++ drift gate    Score JSON     per-language mirrors + codecs
     (locks hand-      Schema         (TypeScript, Swift, Python, ...)
      written api                            |
      headers to the                         | calls
      model)                                 v
                                   +--------------------+
   mx::core + mx::impl + mx::api   |   micro C ABI      |
   compiled once (native / WASM) <-|  mxc_xml_to_score  |
   + generated C++ Score codec     |  mxc_score_to_xml  |
                                   +--------------------+
```

### Layer 1: the micro C ABI (`mxc`)

Hand-written once, ~8 functions, then frozen. Growth in the data model never changes the ABI,
because the data crosses as encoded bytes; only the encoding's schema version moves.

```c
// mxc.h — the entire foreign function surface of mx
typedef struct mxc_buffer   { const uint8_t* data; size_t size; } mxc_buffer;
typedef struct mxc_result   mxc_result;            // opaque

mxc_result* mxc_xml_to_score(const uint8_t* xml, size_t size);   // xml -> Score JSON
mxc_result* mxc_score_to_xml(const uint8_t* score, size_t size); // Score JSON -> xml

bool        mxc_ok(const mxc_result*);
mxc_buffer  mxc_value(const mxc_result*);          // valid until mxc_free
int32_t     mxc_error_code(const mxc_result*);     // mirrors api::ResultCode, generated
mxc_buffer  mxc_error_path(const mxc_result*);     // e.g. /score-partwise/part[1]/...
mxc_buffer  mxc_error_message(const mxc_result*);
void        mxc_free(mxc_result*);

const char* mxc_version(void);                     // library semver
int32_t     mxc_score_schema_version(void);        // Score JSON schema version
```

Deliberate exclusions, all simplifications:

- **No file paths.** Hosts do their own I/O in their own idiom (`fs` in Node, `Data` in Swift);
  WASM has no real filesystem. `ResultCode::ioError` stays host-side.
- **No handles, no singleton.** The two conversion functions are pure and reentrant;
  `DocumentManager`'s registry and `getUniqueId` are C++ conveniences that do not cross.
- **No `getDocument`.** Core access remains a documented C++-only escape hatch.
- **No exceptions, no UB** — the doctrine's failure model already matches what an FFI boundary
  demands; `DocumentManager`'s catch-everything behavior moves down into the shim unchanged.

Every binding technology on the menu — emscripten (`EMSCRIPTEN_KEEPALIVE` + `cwrap`), N-API,
ctypes/cffi, JNA, P/Invoke, cgo, Dart FFI, Swift's C interop — consumes a C header this shape
trivially. This is the C FFI layer the emscripten inquiry correctly identified as necessary,
shrunk to a size where it can be hand-audited.

### Layer 2: the score encoding ("Score JSON")

A canonical JSON encoding of `ScoreData`, defined by a JSON Schema generated from the api
model. Field names are the api's camelCase names; absence conventions map mechanically
(`std::optional` absent → key omitted; `Bool::unspecified` → key omitted; legacy `-1`
sentinels → key omitted, so the wire format is already clean of them; choice types → an object
with a `kind` discriminant). The C++ side of the codec (`ScoreData ⇄ JSON`) is generated
against the public api headers only, and lives with the shim, not inside `mx::api`.

Why JSON first:

- The first requested target is JavaScript. One UTF-8 string across the WASM boundary plus
  `JSON.parse` is the cheapest possible bridge — faster in practice than thousands of
  fine-grained embind calls, and idiomatic on arrival.
- Human-debuggable and diffable; corpus tests can pin golden fixtures; `jq` works.
- Zero dependencies on either side, matching house style (vendored pugixml, hand-rolled
  Mustache press).

The encoding is versioned and can grow a sibling: if profiling ever demands it, a CBOR variant
generated from the same model slots in behind the same ABI (an encoding parameter), changing
nothing else. Start with JSON; measure before adding it.

### Layer 3: the api model (the IDL) and its gen targets

A declarative description of every `mx::api` type: the second input to `gen/`, alongside the
XSD. Same TOML-and-templates diet, same press, same cardinal rule (a new language target adds a
directory, touches no Python). Sketch:

```toml
# api-model: NoteData (excerpt)
[struct.NoteData]
doc = "A note, rest, or chord member ..."
fields = [
  { name = "isRest",           type = "bool",                default = "false" },
  { name = "tieLetRing",       type = "TieLetRing",          optional = true },
  { name = "graceSlash",       type = "Bool" },                       # ternary, unspecified default
  { name = "crossStaffIndex",  type = "int",                 optional = true },
  { name = "tickTimePosition", type = "int",                 default = "0" },
  { name = "beams",            type = "Beam",                repeated = true },
  { name = "userRequestedVoiceNumber", type = "int", legacy-sentinel = "-1" },
]

[choice.TimeChoice]
doc = "Simple (3/4) vs complex (5/8+3/8) time signature ..."
common = [
  { name = "isImplicit", type = "bool", default = "true" },
  { name = "display",    type = "Bool" },
]
alternatives = [
  { kind = "simple",  type = "TimeSignatureData" },
  { kind = "complex", type = "ComplexTimeSignature" },
]

[enum.Beam]
values = ["unspecified", "begin", "end", "extend", "forwardBroken", "backwardBroken"]
```

The model must speak mx's house idioms — this is the decisive argument against off-the-shelf
IDLs: `Bool` ternaries, `unspecified`-first enums, legacy `-1` sentinels, choice classes with
common fields and collapse rules, maps keyed by measure index or voice number, and the
doctrine's musician-facing doc comments (the model carries them; every generated mirror
inherits them).

Generated from the model, each as a gen target directory:

| Target | Output |
|--------|--------|
| `schema` | Score JSON Schema (spec, validation, fixture gate) |
| `cpp-codec` | `ScoreData ⇄ JSON` in C++ (public headers only) + `ResultCode` table for `mxc.h` |
| `cpp-gate` | The drift gate (below) |
| `ts` | TypeScript `interface Score { ... }` etc. + decode/encode (mostly just types) |
| `swift` | `Codable` structs (Score JSON + `Codable` is nearly free) |
| `py`, ... | dataclasses + codec; one directory per language, added as demand appears |

### The C++ headers stay hand-written; a drift gate locks them to the model

Hoisting the *authorship* of the C++ api into the IDL — generating today's 60 headers — is not
required for polyglot and is a poor early move: the headers carry hand-crafted doctrine
comments, choice-class member functions, and constructors with collapse semantics; regenerating
them byte-faithfully is a large migration with no user-visible payoff.

Instead the model is **descriptive first, authoritative always**: a generated translation unit
asserts, for every modeled type, that the C++ type has exactly the modeled fields with the
modeled types and defaults (compile-time member checks plus a default-constructed equality
walk — the `MXAPI_EQUALS` blocks already enumerate every equality-bearing field, and the same
CI gate that catches a missed `MXAPI_EQUALS_MEMBER` line catches a missed model entry). Add a
field to `NoteData` without updating the model and CI fails, exactly like today's gen drift
check. Later — as a separate, deliberate step — the headers can flip to generated output
with zero semantic change, because the gate has kept them isomorphic all along.

Bootstrapping the model is mechanical, not archaeological: the headers are regular
(`MXAPI_EQUALS` blocks enumerate fields; audit/ already parses api enums), so a one-shot
extraction script drafts the model and human review promotes it to source of truth.

### What a foreign user actually sees

The FFI is invisible. The JavaScript answer to rpatters1:

```ts
import { parseMusicXml, toMusicXml, Score } from "@webern/mx"; // wasm inside

const score: Score = parseMusicXml(xmlString);        // throws MxError(code, path, message)
score.workTitle = "Kid Presentable";
const xml: string = toMusicXml(score);
```

`Score` is a plain TypeScript object tree mirroring `ScoreData` — structural typing, spread
operators, `JSON.stringify` for free. Swift gets `Codable` value structs (the existing SPM
package keeps compiling the C++ engine; its public surface becomes the Swift mirror instead of
raw C++ interop). Python gets dataclasses over ctypes. Each wrapper is a thin hand-written
shell (~100 lines: load library/wasm, marshal bytes, raise errors) around generated types.

## Alternatives considered and rejected

- **Direct C++ binding generators (embind, SWIG, cppyy).** This is the path already found
  wanting: `std::optional`/`std::variant`-backed choice classes bind poorly or not at all;
  output is handle-based objects with manual `.delete()` in JS — un-presentable; every language
  gets a different wart set; and the binding surface scales with the whole api (hundreds of
  types) instead of two functions. Fine-grained FFI is also the worst case for WASM call
  overhead, where one big string beats ten thousand small calls.
- **Handle-and-accessor C API** (a C function per field). Thousands of functions, forever
  growing with the model, chatty across every boundary, huge audit surface. The
  document-granularity fact (design fact 1) removes the only reason to want it.
- **Protobuf / FlatBuffers / Cap'n Proto as the IDL.** Free codegen for many languages, but:
  imposes foreign type shapes (its own optionality and union idioms, not mx's), adds a runtime
  dependency to every consumer and to the C++ core against strong house precedent, and its
  generated types are unidiomatic enough that each language would need a wrapper layer anyway —
  at which point it competes with a press target that emits the idiomatic thing directly.
- **WIT / WASM component model.** The right shape (typed values at the boundary) but wasm-only,
  with immature C++ guest tooling. Not the backbone — yet the score-codec boundary is exactly
  a WIT-shaped interface, so if the component model matures, a `wit` gen target maps the same
  model onto it. Nothing in this design blocks that future; most of it prepares for it.
- **Generate full per-language libraries from the XSD** (extend the Go/C toys to production).
  Generates the *types* but not the *judgment* — the impl layer's semantics are the product
  (design fact 2). Rejected as polyglot strategy; the toys remain agnosticism proofs.
- **Rewrite the core in Rust + UniFFI.** Buys the best binding toolchain by discarding the
  working, corpus-hardened engine and generator. Not realistic.
- **Make C the primary api ("hoist the api to C").** Destroys the C++ user experience to serve
  bindings that only need bytes. C here is a substrate, not an api.

## Costs and risks, honestly

- **Whole-tree copies per call.** Identical semantics to today's `getData`; a large orchestral
  score is a few MB of JSON either way. If a real workload proves hot, the CBOR sibling and
  (later) handle-based partial reads are additive, not redesigns.
- **The model is a second thing to maintain.** Mitigated the same way gen/ mitigates schema
  drift: CI gates, fail-loud validation of every model key against the C++ types, and eventual
  header generation collapsing the duplication entirely.
- **Codec correctness.** Defended by the existing harness pattern: for every corpus file,
  `xml → Score JSON` must validate against the schema, and `xml → Score JSON → xml` must pass
  the same DOM comparison the api round-trip already runs. Foreign codecs test against golden
  fixtures generated from the corpus.
- **Doc-comment duplication** between headers and model until the flip. Accepted; the drift
  gate checks shapes, not prose, and prose divergence is a review concern, not a correctness one.

## Suggested sequence

1. This document: design recommendation.
2. Hand-written vertical slice — `mxc.h` + a partial Score JSON codec (score header, parts,
   measures, notes) + an emscripten build + a ten-line JS demo. Proves the boundary and
   answers the emscripten question concretely.
3. The api model + gen targets (`schema`, `cpp-codec`, `cpp-gate`, `ts`), corpus-backed codec
   gate in CI, replacing the hand-written codec.
4. Ship `@webern/mx` (npm, wasm) as the first real binding; Swift mirror next, riding the
   existing SPM package.
