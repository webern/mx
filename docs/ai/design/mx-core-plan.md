# mx::core plan: the decision record and definition of done

Status: APPROVED and IMPLEMENTED (2026-06-11; see §7 for the implementation record and the
recorded deviations). This is the binding plan for the generated C++ `mx::core` target. It was
produced by an owner interview on 2026-06-11 that resolved every open conflict between the two
design proposals, `mx-core-gpt-5.0.md` and `mx-core-opus-4.7.md`. Where this document contradicts
either proposal, **this document wins**. Where it is silent, the proposals remain useful reference
material for rendered-shape sketches (the gpt-5.0 doc for API philosophy, the opus-4.7 doc for
plates mechanics, file layout, and dispatch technique).

The reader is assumed to know `plates.md`, `generator-agnosticism.md` (both since folded into
`gen/DESIGN.md`), and `gen/README.md`.
Nothing here amends the generator architecture: C++ is a target directory (`gen/cpp/config.toml`
plus `gen/cpp/templates/`), and the cardinal rule holds — no language knowledge enters the
generator's Python. One exception is explicitly scoped in §2.9: projecting the content tree and
named groups onto the Plates is *neutral schema fact* work that serves all targets.

---

## 1. The non-negotiables (owner's cardinal requirements)

1. **Valid by construction.** It must be impossible to use the public C++ API to produce a
   document that is invalid against the MusicXML 4.0 spec. Invalid states are made
   *unrepresentable* by the type system wherever possible — including bespoke structural
   representations for individual types where the schema's string-with-pattern view hides
   structure (RGB color, dates, comma-separated lists).
2. **Errors in exactly two places, nowhere else.**
   - Rejected parse: `parse(...) -> Result<Document>` is the error boundary.
   - Bounded-cardinality append: `maxOccurs` is enforced; an over-cap append is rejected
     (`Result<void>`). MaxOccurs enforcement is mandatory, not a documented near-miss.
   No other constructor, setter, or factory returns or throws an error. Construction repairs
   deterministically (clamping, normalization) instead of failing.
3. **Modern, safe C++20.** No bare `new`/`delete`, no raw owning pointers, smart pointers where
   indirection is needed at all, Meyers-Effective-Modern-C++ discipline. Raise CMake to C++20
   when this target lands.
4. **Optimization priorities, in order: compile time, runtime memory, runtime speed, binary
   size.** Consequences: light headers / heavy `.cpp` bodies, no template metaprogramming in
   generated code, `std::variant` dispatch by index switch (jump table) rather than `std::visit`
   towers, boxed alternatives only where alternative sizes demand it.

## 2. Decisions

Each decision below was explicitly approved by the owner during the interview.

### 2.1 XML layer: pugixml directly; ezxml retires

Generated code consumes **pugixml** (`pugi::xml_node`, `pugi::xml_document`) directly, the same
way Go consumes etree and C consumes libxml2. ezxml — a virtual-interface wrapper whose every
node access allocates a `shared_ptr` and copies `std::string`s — is retired; its swappability
promise is redundant (target swappability lives in the generator, not inside C++).

- Promote the vendored pugixml (currently buried at
  `src/private/mx/ezxml/src/private/private/pugixml.{hpp,cpp}`, version 1.10) to its own
  directory, e.g. `src/private/pugixml/`, and **refresh it to the current upstream release**
  (1.15 at time of writing). Keep `pugilicense.txt` beside it.
- Domain classes never read or write streams or files. Free functions map model <-> pugixml DOM;
  pugixml owns load/save/encoding (it auto-detects UTF-16/Latin-1, matching what libxml2 does
  for the C target). DOCTYPE handling needs the `pugi::parse_doctype` flag.
- The corert harness, `mxtest/import` normalization utilities (sort attributes, strip decimal
  zeros), and the Fixer are ported to pugixml. Once no client remains, delete
  `src/private/mx/ezxml/`.

### 2.2 Value types: structural unrepresentability first, then clamp

Priority order for representing a constrained value type:

1. **Make invalidity unrepresentable structurally.** Bespoke shapes are expected and welcome:
   - `color` stores numeric RGB(A) components; serialization formats `#RRGGBB`/`#AARRGGBB`. A
     malformed hex string cannot exist.
   - `yyyy-mm-dd` stores year/month/day integers, clamped to valid ranges.
   - `comma-separated-text` stores `std::vector<std::string>`.
   - Prefix-patterned SMuFL glyph-name tokens (`acc\c+` etc.) store the suffix; the prefix is
     structural and emitted by the serializer.
   - The full audit of patterned/structured value types in the 4.0 schema is the first task of
     the values wave (§4, W1): every `StringPlate` with a pattern gets either a structural
     representation or an explicit recorded justification for staying a normalized string.
2. **Clamp/normalize on construction** where a deterministic nearest-valid repair exists.
   Numbers clamp using the plates' `clamp` steps (`NumberPlate.clamp` — already projected data),
   on construction and on `set`. One policy, one spelling, identical to import.
3. **Closed enums need no validation at all**: enum wrapper types whose only constructors are
   the named factories; a `static_cast`-proof invalid value is unrepresentable through the
   public API.
4. Unconstrained strings construct directly.

Default construction is always valid: every type's default is its natural valid zero, recursively
(safe because the complex-type graph is a DAG — by-value composition throughout, no forward
declarations, no shared ownership).

Mechanisms for bespoke shapes, both pre-existing: manifest `[[render.type]] types = ["..."]`
bespoke-template rows (precedent: Go's `yyyy_mm_dd.go.tmpl`), and hand-written runtime files for
schema-independent types (`Decimal`), referenced via `[types]` in `gen/cpp/config.toml`.

### 2.3 Composites model the schema grammar exactly

A flat ordered child list (the Go/C encoding) is rejected for C++: it can represent a `<note>`
with two `<pitch>`es, which violates §1.1. The generated shape follows the resolved content tree:

| content shape | C++ representation |
|---|---|
| pure sequence | named fields in schema order; serializer walks declaration order, so wrong order is unrepresentable |
| one-shot choice | a `std::variant` member — exactly-one by construction |
| named schema group (`full-note`, `music-data`, ...) | a generated shared type (class or variant alias) reused by every referencing composite |
| repeated choice (e.g. measure music-data) | private `std::vector<Variant>` — any order/count of alternatives is schema-valid there, so the vector represents nothing invalid |
| repeated homogeneous element | private `std::vector<T>`, read via `std::span<const T>` |
| bounded repeat (finite maxOccurs > 1, e.g. `beam` <= 8) | private vector + `add...(T) -> Result<void>` that rejects an over-cap append |
| presence-only element | a flag (bool field in a sequence; an `Empty` marker type as a variant alternative) |
| optional anything | `std::optional<T>` |
| derived complex types | non-polymorphic public inheritance (`strategy = inherit`); no virtual anything |

Variant alternatives are by-value by default; box with `std::unique_ptr` **only** where a repeated
heterogeneous choice has large alternatives (measure music-data is the known case — a by-value
`variant<Note, ...>` would size every slot at the largest alternative). Which groups box is target
data (`gen/cpp/config.toml` `[vars]`), not template hardcode. Duplicate variant alternatives
(`<chord>` and `<cue>` both presence-only) dispatch by `std::in_place_index` /
`switch (v.index())` — index, never type.

### 2.4 Import policy: strict names and structure, lenient values, configured repairs

Parsing (the only door for external data) applies:

- **Strict on names**: unknown element or attribute -> error. With version gating, an unknown
  name is a generator gap or garbage input, never data to preserve.
- **Strict on structure**: wrong sequence order, duplicate choice alternatives, missing required
  children, over-cap repeats -> error. Corpus evidence (§3) shows this costs zero files.
- **Lenient on values** (the owner classes these with clamping, not with structure):
  - numbers: lexically-strict parse, then range-clamp (the established fixup-sidecar policy);
  - enums: an unknown literal falls back to the first variant;
  - patterned strings: normalize into the structural representation's nearest valid value.
  Every repair still yields a valid model object — leniency never admits an invalid document.
- **Missing required attributes**: inject a configured default. Config data, not generator
  logic, e.g. in `gen/cpp/config.toml`: `lyric-language`'s `xml:lang` defaults to `"und"` (BCP 47
  "undetermined" — the spec-correct unknown-language code). A missing required attribute with
  *no* configured default is a parse error. The configured-defaults table is the lever if the
  corpus or real-world files ever surface more cases.
- **Version gating**: a root declaring a version newer than the generated
  `SupportedMusicXMLVersion` (`"4.0"`, emitted from `Plates.schema_version`) is rejected with
  `unsupportedVersion`; the harness counts it as a skip.

The user-facing construction API stays strict in the §1.1 sense regardless: leniency is the
parser's door only, and even the parser's repairs produce only valid objects (the two-door
policy).

### 2.5 Errors: one boundary type, internal throw

```cpp
namespace mx::core
{
enum class ErrorCode
{
    unknownElement, unknownAttribute, missingRequiredElement, missingRequiredAttribute,
    wrongElementOrder, tooManyElements, invalidDocument, unsupportedVersion,
};

struct Error
{
    ErrorCode code;
    std::string path;     // e.g. /score-partwise/part[1]/measure[3]/note[2]
    std::string message;
};

template <typename T> class Result;  // small hand-written expected-like; C++20 has no std::expected

Result<Document> parse(const pugi::xml_document& doc);
void serialize(const Document& d, pugi::xml_document& out);
}
```

Internally, generated per-type parse functions **throw** a `ParseError` carrying path context;
the document-level `parse` catches and converts to `Result`. This keeps ~24-arm dispatch bodies
free of per-child propagation boilerplate while the public boundary stays explicit and
exception-free. `serialize` is total for every model object (§1.1 guarantees the object is
valid). Per-type `parse...`/`serialize...` free functions exist (testability) but the documented
public contract is the Document-level pair.

### 2.6 API spelling and layout

- Accessors: `step()` / `setStep(Step)` (modern accessor style — owner's pick). Private members
  `m_step` (`m_` prefix; chosen so the reserved-word *suffix* mangling can never collide with a
  member-suffix convention).
- Types PascalCase, one type per `PascalCase.h`/`.cpp` pair under `src/private/mx/core/generated/`
  (committed, like Go/C output), plus a generated `sources.cmake` (C-target precedent).
- `namespace mx::core`; `#pragma once`; headers include only what they textually need (plates
  `deps` drive the include list); doc comments from the schema.
- Hand-written runtime beside the generated code: `Decimal.{h,cpp}` (lossless decimal: text face
  for round-trip fidelity, numeric face for clamping), `Result.h`/`Error.h`, small parse/format
  helpers (`std::from_chars`-based, locale-free). Generated-only files carry the generated
  banner; hand-written files never contain generated content (regen-safe split: the version
  constant lands in its own generated header).
- Repo `CMakeLists.txt` gains an `mx_core` static library (C++20) that includes `sources.cmake`;
  corert links it. clang-format runs as the manifest's `[render] format` hook.

### 2.7 Corpus actions (evidence in §3)

- **Fix the 5 repo-authored synthetic fixtures** missing required `xlink:href`
  (`data/synthetic/{link.3.0,opus.3.0,part-link.4.0,group-link.4.0,instrument-link.4.0}.xml`) —
  they exist to exercise those elements; marking them invalid would erase that coverage.
- **Add `.fixup.xml` sidecars** for the 4 real-world files missing `xml:lang` on
  `lyric-language`, using a new fixup operation: **add-attribute-to-expected** (the C++ importer
  injects `xml:lang="und"`, so the expected document must gain it too). The Go/C Fixers must
  skip operations they don't implement so their suites stay green untouched.
- **No `.invalid` markers needed**; no files leave the corpus.

### 2.8 Go and C are anemic, and documented as such

The Go and C targets are **non-validating round-trip harness bindings**: they enforce neither
sequence grammar nor choice exclusivity, and their APIs can construct invalid documents (a note
with two pitches is representable). The owner's position, recorded verbatim in spirit: an API
that can construct an invalid document is unacceptable for any product surface; these targets'
only legitimate role is keeping the generator honest about language-agnosticism, and their
current shape is tolerated only in that role.

**Follow-up (approved, scheduled after C++ lands):** retrofit Go and C to the same
grammar-enforcing contract, reusing the §2.9 plates extension. Until then AGENTS.md and the
target READMEs must carry the anemic-API warning.

### 2.9 The one generator change: project content tree and groups onto the Plates

The Plates currently splice groups away and the Go/C templates read only the flat member list.
C++ templates need the resolved content tree (sequence/choice nesting) and named-group identity
(`full-note` as a shared type). Exposing these is **neutral schema fact** projection — the same
test the sounds fold and the clamp steps passed — so extending `gen/plates/` (and, if needed,
`gen/ir/resolve.py`) is legitimate Python work under the cardinal rule. It must be:

- consumed via plate fields any target could read (the JSON Schema target is the forcing
  function: it already wants `content`);
- covered by `gen/tests/test_plates.py`;
- invisible to Go/C output (regen produces a zero diff for them — part of Gate 5).

No C++ branch enters Python. `python3 -m gen plates --check` remains the CI gate.

---

## 3. Corpus evidence (measured 2026-06-11, 829 eligible files)

These facts drove §2.4 and §2.7; re-verify them if the corpus changes.

- **Missing required attributes: 9 files, 4 patterns, zero caused by 4.0 breaking changes.**
  `lyric-language@xml:lang` (4 real-world files; required since 3.0) and `xlink:href` on
  `link`/`opus`/`part-link` (5 synthetic fixtures; required since 3.0 / since the element
  existed).
- **Structural violations: zero.** Full xmllint XSD validation of all 829 files found not one
  wrong-order, unexpected-element, or missing-required-child error. Strict structural parsing
  costs nothing.
- **Value violations: 10 files, all already carrying fixup sidecars.** Mostly numeric range
  (`sound@dynamics="-1.11"`, `midi-channel 0`, `duration 0` — clamp territory); three files hold
  non-numeric garbage (`display-step` of `=`, `accordion-middle` of ``/`test`, empty
  `fret`/`string`) — covered by the enum-first-variant / normalization leniency.
- Expected corert C++ counts: **829 pass / 0 fail / 0 skip** (nothing in the corpus declares a
  version newer than 4.0). Go/C stay at **777 / 0 / 52**.

---

## 4. Implementation waves

All template/config work; each wave ends at a named gate and a commit. Generated output is
committed at every wave. Go/C suites must be green at every wave boundary (agnosticism
regression).

- **W0 — foundations.** Promote + refresh pugixml; hand-written runtime (`Decimal`, `Result`,
  `Error`, parse helpers); CMake `mx_core` skeleton + C++20 bump; the §2.9 plates extension with
  tests; fix the 5 synthetic fixtures.
  *Gate: plates `--check` green, `test_plates`/`test_agnosticism` green, Go/C regen zero-diff
  and suites green.*
- **W1 — value types.** Audit every patterned/structured value type and record its
  representation (§2.2.1); templates for enum/number/string/union + bespoke rows (`color`,
  `yyyy-mm-dd`, `comma-separated-text`, SMuFL names, ...); values smoke test (C-target
  precedent).
  *Gate: all value types emitted, compile, smoke test green, Gate-4 clamp/factory unit tests
  for representative types.*
- **W2 — complex types.** Templates for attrs/value-bodied/sequence/choice/group/derived shapes;
  shape unit tests (sequence order unrepresentability, variant exclusivity, maxOccurs
  rejection, default-construction validity).
  *Gate: full `mx_core` compiles; shape unit tests green; compile time recorded.*
- **W3 — document + corert.** `Document`, version gating, root-namespace preservation, generated
  DOCTYPE/version constants; port harness + `mxtest/import` + Fixer to pugixml and the new API;
  add the add-attribute fixup op (+ Go/C fixers skip unknown ops); the 4 `xml:lang` sidecars.
  *Gate: Gates 1, 2, 3 green (see §5).*
- **W4 — adversarial audit + cleanup.** Run Gate 6; fix findings; delete ezxml and the legacy
  harness pieces; AGENTS.md updates; record final compile-time numbers.
  *Gate: all six gates green; ezxml gone; docs current.*

---

## 5. Definition of done: the six gates

corert green alone is **not** done — it proves round-tripping of valid files, not the cardinal
properties (rejecting invalid input; invalid construction being unrepresentable).

1. **corert C++ green with pinned counts.** 829/0/0 (pass/fail/skip); count drift is failure
   even with zero "fails". Go/C stay 777/0/52.
2. **Emitted documents are schema-valid — permanent CI gate.** Every parsed corpus document is
   serialized and the *output* xmllint-validated against the 4.0 XSD. Even the 10
   value-violating inputs must emit valid XML (proves the clamp policy end-to-end). This is the
   cardinal rule's mechanical proof and it stays in CI, not one-time.
3. **Rejection suite.** Hand-authored invalid documents that parse must refuse — duplicate
   `<pitch>`, `<octave>` before `<step>`, unknown element/attribute, missing required child,
   root version 5.0 — each asserting the right `ErrorCode` and a meaningful path.
4. **Construction-safety tests.** Runtime: clamping on construct/set, maxOccurs append
   rejection, default-construct-every-type-then-serialize-then-xmllint (mechanically
   generatable). Compile-time: must-NOT-compile probes (e.g. constructing an enum wrapper from a
   raw int) verified by a script asserting compilation fails.
5. **Generator hygiene.** `plates --check` green; `test_agnosticism` green; regen-then-
   `git diff --exit-code` (fresh, deterministic, committed output — including zero Go/C diff);
   clang-format hook applied.
6. **Adversarial subagent audit.** At every "done" claim (each wave end and before final merge),
   a **fresh-context** subagent receives only the MusicXML 4.0 XSD, the generated public
   headers, and the instruction: *construct a schema-invalid document using only the public API
   — no casts, no UB, no reaching into privates.* It writes and compiles real programs;
   serialized output is xmllint-validated; any invalid output is a bug against the API design.
   Mirror direction: the subagent crafts invalid input XML it believes parse should reject;
   anything that parses but whose re-serialization xmllint-fails is a leniency hole (findings
   triaged against the documented clamp policy — designed repairs are expected to pass). Fresh
   context is the point: the auditor must not inherit the implementing session's assumptions.

Compile time (priority #1) is measured and recorded at each wave boundary rather than
hard-gated, so regressions are visible without inventing an arbitrary budget.

---

## 6. Follow-ups (recorded so they don't evaporate)

1. **Retrofit Go and C to the grammar-enforcing contract** (§2.8), reusing the §2.9 plates
   extension: Go gets real sequence fields + sum-type-style choices, C gets tagged unions.
   Their corert suites then test the strict contract in three languages.
2. **Decimal contract finalization**: exact trailing-zero/precision policy must round-trip the
   decimal-fields corpus byte-for-byte under the existing normalization; revisit once W3 data is
   in hand.
3. **Streaming serializer** (skip the DOM) only if benchmarks ever show DOM allocation matters —
   not before.

## 7. Implementation record (added at completion, 2026-06-11)

All four waves landed and all six gates are green. What an implementer (or auditor) should
know beyond the decisions above:

- **Corpus actions (§2.7) were already satisfied.** The W0 re-verification (per §3's "re-verify
  if the corpus changes") found the 5 synthetic `xlink:href` fixtures and the 4 `lyric-language`
  `xml:lang` files already fixed in `data/`; exactly the 10 fixup-sidecar value-violation files
  fail xmllint, all clamp-policy territory. No new sidecars and no add-attribute fixup
  operation were needed (the C/Go fixers already skip unknown `<replace>` type flavors). The
  `[import] attribute-defaults` mechanism exists regardless (`lyric-language`'s `xml:lang`
  defaults to `"und"`).
- **The §2.9 projection as built**: `Resolver` gained the field view (trivial single-element
  groups splice inline; structural groups stay referenced; choice-shaped groups like
  `music-data` collapse so reference sites absorb the occurrence); the Plates gained
  `ComplexPlate.fields`, `Plates.groups` (group-class / choice-class plates, including
  hoisted `{owner}-choice` / `{owner}-group` types renameable via `[rename.type]`),
  first-name dispatch sets, nullability, `default_alt` (a nullable choice's natural zero is
  its empty alternative, so a default `<key/>` is valid), `is_base`, and `structural_deps`.
  Go/C/schema output stayed byte-identical throughout (Gate 5 verified at every wave).
- **Boxing knob**: §2.3 suggested `[vars]` for naming which choices box; as built it is a
  manifest `[[render.type]] types = ["music-data-choice"]` row selecting the boxed-choice
  templates -- still config data, not template hardcode, and it reuses the existing bespoke-row
  mechanism instead of inventing a vars convention.
- **Natural-zero refinement**: required opaque-string attributes default to `"P1"` -- the IR
  canonicalizes `xs:ID`/`xs:IDREF` to `token`, and the empty string is not a valid ID, so the
  default-constructed Document could not otherwise pass Gate 2's xmllint check.
- **Gate 4's xmllint clause, as built**: only the two roots are globally declared in the
  schema, so non-root fragments cannot be xmllint-validated in isolation. The mechanical
  probe (`roundTripDefaults`, generated) instead serializes every type's natural zero and
  re-parses it with the strict grammar; the default Document is xmllint-validated with the
  corpus outputs in `make validate-cpp`.
- **Wire-fidelity text faces**: `Decimal`, the comma-separated list types, `time-only`, and
  `ending-number` preserve the exact parsed spelling and re-emit it; numeric/structural
  mutation drops the face and serializes canonically. This is what keeps corert exact-text
  comparisons green without weakening the structural representations.
- **Compile time (priority #1) record**: clean `mx_core`, GCC 13, `-j8` Debug -- W1
  (values only) 14.2s wall / 44s user; W2-W4 (full model, 905+ generated files) ~1m40s wall
  / ~5m35s user.
- **Gate commands**: `make test-core-dev` (Gate 1, pinned 829/0/0), `make validate-cpp`
  (Gate 2, permanent), `make test-cpp-unit` (Gates 3 and 4 runtime), `make probe-cpp`
  (Gate 4 compile-time), `make test-gen` + plates `--check` + regen-zero-diff (Gate 5).
- **Gate 6 (adversarial audit) found and the implementation closed** a systemic lexical hole:
  every `xs:ID`/`xs:IDREF`/`xs:NMTOKEN` attribute had been a raw `std::string` setter that
  accepted spaces, the empty string, and leading digits and wrote them back verbatim. The IR
  had collapsed `xs:ID`/`xs:IDREF` to `token`, hiding the distinction. The fix preserves
  `id`/`idref` as distinct IR primitives (a neutral schema fact, parallel to the already-
  distinct `nmtoken`), adds the neutral `PlateRef.name_token` flag, and gives the C++ target
  hand-written repairing wrappers (`Token` = NCName for id/idref, `NameToken` = the wider
  NMTOKEN). A bare string literal can no longer reach a `setID` (the converting constructor is
  explicit -- probe 7); parse repairs malformed identity tokens as a designed value-leniency
  repair. Go/C/schema map the new primitives to their existing string spellings, so their
  committed output is byte-identical.
- **Known limitation (referential integrity, out of scope by §1.1's "wherever possible").**
  Two intrinsically valid `Token`s with the same value still serialize duplicate `xs:ID`s, and
  a dangling `xs:IDREF` is unconstrained -- both are document-graph properties no per-object
  type can enforce locally, and the audit confirmed `xmllint --schema` does not even check
  IDREF existence. Enforcing them would require a document-level validation pass, which the
  design rejects (the two-error-doors rule). They are the standard XSD-validity-beyond-the-
  type-system carve-out, documented here and in AGENTS.md rather than papered over.
