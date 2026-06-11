# Generator agnosticism: removing language knowledge from the generator

Status: design. This document specifies the redesign that removes all target-language knowledge
from the generator's Python code. It supersedes the emit-stage portions of
[`plates.md`](plates.md) (the Plates layer itself -- sections 1-8 there -- survives intact; what
changes is what a "template" is and where language facts live).

## 1. The cardinal rule

**The generator is language agnostic. Adding a new language target must not require edits to the
generator's Python files.**

The rule has a letter and a spirit. The letter: `git diff --name-only` for the change that adds a
new target touches no `*.py` under `gen/` (outside the new target's own directory). The spirit:
the Python pipeline must be a closed machine -- schema in, files out -- that is *incapable* of
expressing a language-specific decision, so that language knowledge has nowhere to live except in
the target's own directory. Go and C were built first precisely to force this generality; the
current implementation failed the test, and the C++ target would have failed it a third time.

A corollary worth stating: under this rule, the generator has no concept of "Go" or "C" at all.
There is no language registry, no language name in config, no per-language defaults. A target is a
directory of data and templates; the generator cannot tell which language it is emitting.

## 2. What violates the rule today

| Where | What it encodes | Lines |
|---|---|---|
| `gen/emit/go/` (6 modules) | Go grammar end to end: declarations, parse/serialize bodies, the runtime source as a Python string, import lists, string-literal quoting, the gofmt subprocess | ~970 |
| `gen/emit/c/` (7 modules) | C grammar end to end: header/impl frames, include guards, the calling-convention module (ownership rules per plate kind), the runtime source, memory management | ~1,360 |
| `gen/plates/languages.py` | Per-language type maps, reserved-word lists, doc-comment styles, variant scopes -- data tables keyed by language name | ~125 |
| `gen/emit/__init__.py` | `BACKENDS`: a language-name -> Python-module registry | -- |
| `gen/config.py` | `[target] language` (selects the above), and prescribed keys (`namespace`, `prefix`) that exist only because specific languages need them | -- |

Roughly 2,500 lines of Python that are *about* Go and C. Everything else in the pipeline --
the XSD parser, the IR and Resolver, the naming machinery (`gen/names.py`), the Plates projection,
the collision gate, the writer -- is already neutral, and the review rounds that pushed decisions
"into the plates" (final identifiers, clamp policy, union tags, effective cardinality) made the
*data* model genuinely language-free. The failure is confined to one question the plates design
left unanswered: **what is a template?** The design said "templates are dumb renderers" and the
implementation answered "a Python module per language." Every subsequent fix improved where
*decisions* live but left the *renderers* as per-language programs.

## 3. The redesign in one paragraph

A target becomes a **pack**: a directory containing `config.toml` and a `templates/` directory,
and nothing else the generator needs. Everything `languages.py` held becomes required config data.
Everything `gen/emit/<lang>/` held becomes template files in a deliberately minimal, logic-less
template language, rendered by one generic engine (the **press**, completing the plates metaphor:
plates carry every decision; the press inks and prints them). A render **manifest** in the config
declares which template renders which plate shapes into which output paths, so file layout --
including C's header/impl pairs -- is pack data, not Python. The generator's Python is then a
closed set: parse, lower, project, render, write. The proof obligation is concrete: port Go and C
to packs with byte-identical generated output, delete `gen/emit/go`, `gen/emit/c`, and
`languages.py`, then add a third target (the JSON Schema emitter that has been this project's
forcing function since the plates design) as a pure pack, and let CI assert that change touched no
Python.

```
gen/test/c/                     <- a target pack
  config.toml                   <- inputs, projection settings, vars, render manifest
  templates/
    enum.h.tmpl  enum.c.tmpl    <- one template per shape (the original design principle,
    composite.h.tmpl ...           now literally one FILE per shape)
    runtime.h.tmpl ...          <- support files: a template with no tags is a static file
    member-parse.tmpl ...       <- partials shared by this pack's templates
  src/                          <- the hand-written corert harness (target code, not generator)
  mx/                           <- generated output (committed)
```

## 4. Config: the projection contract vs. freeform vars

The PR review asked the right question about `[target] namespace`: is the config schema
prescribed, and is that not itself language-specific? The answer is a split, with a litmus test.

**Prescribed keys are the projection contract**: every key the generator itself consumes must be
definable in projection terms, without reference to any language. These survive, because the
plates' work -- casing, renaming, sanitizing, collision-gating, strategy selection -- is real and
neutral:

- `[naming]` conventions, acronyms, `[rename.*]`, `[reserved] words` -- already neutral.
- `[target] symbol-prefix` (today `prefix`): "prepended to every type identifier and composed
  constant before sanitization." Neutral semantics; it must stay in the projection (not become a
  template variable) because the collision gate certifies the *final* identifiers -- moving
  composition into templates is exactly the regression the first review round fixed.
- `[target] variant-scope = "bare" | "composed"`: how constants are scoped, today seeded per
  language in Python. Becomes explicit config.
- `[target] inheritance = true | false`: selects the derived strategy. Already neutral.
- `[types]`: the primitive -> spelling map, today defaulted per language. Becomes **required** for
  any pack whose templates emit typed code (a pack that omits it gets primitive names passed
  through, which is what a neutral target wants).
- `[reserved] words`: today extends per-language defaults. Becomes the **whole** list; packs own
  their keyword lists. Two small additions let packs protect their template-synthesized names
  generically: `members = [...]` (member identifiers the pack's templates reserve, e.g. Go's
  `Children`) and `type-suffixes = [...]` (compositions like `Child` that the templates append to
  type identifiers), both fed to the existing collision gate.
- `[docs] wrap`: the plates pre-wrap doc text into lines (`doc_lines`); comment *syntax* moves
  into template text, so `[docs] style` and the `DocStyle` machinery are deleted.

**Everything else is freeform.** A `[vars]` table of string key-values passes through to templates
verbatim (`{{target.vars.namespace}}`, `{{target.vars.package}}`, `{{target.vars.anything}}`).
`namespace` stops being generator schema; it becomes a variable that the Go pack's templates
happen to consume as a package name and the C++ pack's as a namespace. `target.foo = "bar"` is
exactly as legal as either. The litmus test for any future key: *if you cannot define it without
naming a language, it is a var, not a key.*

**Deleted outright**: `[target] language` (nothing selects on it anymore), `[layout]` entirely
(partition, file-prefix, file-convention -- subsumed by the manifest, section 6), and
`languages.py` with all its tables.

## 5. The press: a Mustache engine with three documented deviations

`gen/press/` renders template files against a context built from the plates. **The template
language is Mustache** -- the interpolation/sections/inverted-sections/partials core of the
published spec, nothing invented -- so the load-bearing commitment is to a frozen, logic-less
*language*, and the engine behind it is swappable (section 9 records why we implement it ourselves
and the trigger for reversing that). Mustache's poverty is the feature: **if a template cannot
express something, the plates must carry it** -- which keeps decisions in the projection, where
they are dumpable, diffable, and collision-gated.

The implemented subset, derived by walking every construct the current Python backends emit:

- **Variables**: `{{ident}}`, dotted paths `{{name.snake}}`, `{{type_ref.ident}}`,
  `{{target.vars.prefix}}`.
- **Sections**: `{{#members}}...{{/members}}` iterates lists (the cursor becomes the context) and
  gates on truthiness for scalars/objects; `{{^x}}...{{/x}}` inverts.
- **Partials**: `{{> member-parse}}`, resolved within the pack's `templates/` directory, with
  spec-conformant call-site indentation (essential for readable generated code) and recursion
  permitted with a depth limit (a schema-shaped target walking `content` trees needs it).
- **Whitespace discipline**: the spec's standalone-line rules, so templates can be indented
  readably without leaking blank lines.

Three deliberate deviations from spec semantics, each because code generation is not HTML:

1. **Missing keys are a render error**, with `template:line` in the message. The spec mandates
   silent empty output -- the worst possible failure mode for a generator (a typo'd `{{indent}}`
   emits nothing, and a pack with no compiler behind it, like JSON Schema, never finds out). This
   project's ethos is fail-loud; the engine follows it.
2. **No HTML escaping**: `{{x}}` interpolates verbatim (the spec's `{{{x}}}` everywhere would be
   noise; there is no HTML here to protect).
3. **No lambdas** (the spec's one escape hatch into logic). Closed.

Conformance to everything else is *tested*: the press runs against the official Mustache spec test
suite (the published YAML cases for interpolation, sections, inverted sections, and partials),
asserting agreement everywhere except the three deviations above -- the spec authors' edge-case
coverage, especially the fiddly whitespace rules, without their code.

Everything the engine does **not** do -- expressions, comparisons, arithmetic, filters, string
manipulation, casing, assignment -- stays not done; that is Mustache's constitution, not ours to
amend. In particular there is no equality test; dispatch happens three other ways:

1. **By manifest**: each template entry declares which plate strategies it renders (section 6),
   so per-shape dispatch never appears inside a template -- restoring "one template per shape" as
   one *file* per shape.
2. **By discriminant expansion**: the context builder (neutral, mechanical) expands every closed
   enumerated field into boolean companions -- `kind: "enum"` yields `is_enum`; `cardinality:
   "vector"` yields `is_vector`; `category: "primitive"` yields `is_primitive` -- and exposes the
   member list pre-split (`attributes`, `elements`, `value`) using the filters the plates already
   define. Templates branch with plain sections: `{{#type_ref.is_complex}}...{{/type_ref.is_complex}}`.
3. **By injected context, not engine extensions**: loop metadata arrives as fields the context
   builder adds to every list item (`is_first`, `is_last`, `index0`) -- this is what expresses
   `if`/`else if` chains and separator joins -- and every wire-string leaf gets a quoted companion
   (`wire` -> `wire_q`): a double-quoted, backslash-escaped literal using the JSON repertoire with
   non-ASCII as `\uXXXX`, a subset valid verbatim in C, C++, Go, Java, JavaScript, and Rust.
   Keeping both OUT of the engine keeps the template syntax pure Mustache (so the engine stays
   swappable) and keeps the one acknowledged compromise -- that quoted-literal escaping encodes a
   language *family* -- in the neutral context layer, where a future non-C-family target would
   extend it (section 10).

Two small, neutral additions to the plates feed this (the only model changes the redesign needs):

- `PlateRef` gains the referenced type's `name` bundle and `kind` (plate kind, or the primitive's
  family). Today the Python backends look these up via `plates.plate(wire)` to compose calls like
  `mx_{{snake}}_parse(...)` and to choose ownership idioms; a logic-less template cannot perform
  lookups, so the materialized tree denormalizes them (it is materialized precisely so templates
  get random access without computation).
- Each plate gains `deps`: its dependency references with name bundles, replacing
  `FileSpec.includes` so include/import lines become template text composed from data
  (`{{#deps}}#include "mx_{{name.snake}}.h"{{/deps}}`).

Worked example -- today's `gen/emit/c/complexes.py` attribute loop, as template text:

```
    for (xmlAttrPtr a = el->properties; a; a = a->next) {
        {{> attr-name}}
{{#attributes}}
        {{#is_first}}if{{/is_first}}{{^is_first}}}} else if{{/is_first}} (strcmp(aname, {{name.wire_q}}) == 0) {
            m->has_{{ident}} = true;
            m->{{ident}} = {{> attr-parse-expr}};
{{/attributes}}
        {{#attributes}}} else {{{/attributes}}{{^attributes}}{{{/attributes}}
            {{target.vars.fn_prefix}}error_set("unknown attribute \"%s\" on <%s>", aname, (const char *)el->name);
            ...
```

Everything language-shaped (C's `strcmp`, `->`, `has_` prefixes, the error idiom) is pack content;
everything decided (idents, wire names, which members are attributes) is plate data. The press
contributes iteration and the `@first` chain mechanics, nothing more.

## 6. The render manifest: file layout as pack data

`config.toml` declares what gets rendered where. Two entry kinds:

```toml
[render]
dir = "templates"

# Per-type entries: rendered once per plate whose strategy matches.
[[render.type]]
strategies = ["enum-class"]
template   = "enum.h.tmpl"
output     = "mx_{snake}.h"        # casing placeholders from the plate's Name

[[render.type]]
strategies = ["enum-class"]
template   = "enum.c.tmpl"
output     = "mx_{snake}.c"

[[render.type]]
strategies = ["composite-class", "value-class", "flag", "attrs-class", "flatten"]
template   = "complex.h.tmpl"      # or one entry per strategy; the pack chooses its granularity
output     = "mx_{snake}.h"

# Once entries: rendered once per target, against the whole Plates context.
[[render.once]]
template = "runtime.c.tmpl"
output   = "mx_runtime.c"

[[render.once]]
template = "sources.cmake.tmpl"    # receives `outputs`: every path the manifest produced
output   = "sources.cmake"
```

This mechanism absorbs, generically, several things that were Python:

- **C's header/impl pairs**: two entries per strategy. The "one FileId, two files" wart in
  `plates.md` dissolves -- file multiplicity is just manifest rows.
- **Partitioning**: per-type entries *are* `per-type` partition; a pack with only `once` entries
  *is* `single` partition (the JSON Schema pack: one entry, one template, one output). `[layout]`
  dies.
- **File naming**: `output` patterns with casing placeholders (`{snake}`, `{pascal}`, ...) replace
  `file-prefix`/`file-convention` and plate file stems. The generator expands every pattern for
  every matching plate and runs the existing case-insensitive uniqueness check over the full
  expansion -- the file-collision gate survives, now over real paths, including collisions between
  type outputs and `once` outputs (which retires the backend "reserved stem" guards).
- **Support files**: the runtime sources stop being Python string constants and become templates
  (mostly static text; `{{plates.schema_version}}` and `{{target.vars.fn_prefix}}` are the only
  tags the current runtimes need). The completeness check every manifest gets for free: every
  plate must be matched by at least one entry, or none if the pack declares it renders only a
  subset (a `strategies = []` is an error; an explicitly empty manifest is one too).
- **Formatting**: the gofmt pass becomes an optional, generic post-render hook --
  `[render] format = ["gofmt", "-w", "{dir}"]` -- run against the scratch render directory before
  the writer's write-if-changed diff, preserving idempotence. The command is pack data; the
  generator knows only "run this, fail loud if it fails or is absent."

The writer (`gen/emit/writer.py`) is already neutral and survives unchanged: marker-gated pruning,
foreign-file safety, idempotence.

## 7. What remains in Python, and why that is allowed

The closed set, each definable without naming any language: `gen/xsd` (schema parsing), `gen/ir`
(lowering + Resolver), `gen/names.py` (tokenizer, casing registry, sanitizer -- string mechanics),
`gen/plates` (projection driven entirely by IR + config; `languages.py` deleted), `gen/press`
(template engine + context builder + manifest expansion), the writer, and the CLI. The litmus test
for every future line of generator Python: *could this be wrong for a language we have not heard
of?* If yes, it belongs in a pack.

Explicitly **outside** the rule's scope: the corert harnesses (`gen/test/go/corert/`,
`gen/test/c/src/`), smoke tests, CMakeLists, go.mod. These are hand-written programs that *consume*
generated code, exactly like a downstream user; they are target code, not generator code. The rule
governs the machine, not the things the machine's output links against.

## 8. Migration plan

Each phase lands green (all suites pass) and pushed; phases 3-4 carry a hard parity gate:
regenerate and `git diff --exit-code` over the committed `mx/` output -- the port is proven by
byte-identical generation before the Python it replaces is deleted.

1. **The press.** Engine + context builder + manifest expansion + format hook. Tests: the
   official Mustache spec suite for the implemented subset (minus the three documented
   deviations), plus unit tests for the deviations themselves (fail-loud missing keys, identity
   interpolation, no lambdas) and for the context builder's injections (discriminant expansion,
   loop metadata, `_q` companions). No target changes.
2. **Config absorbs `languages.py`.** `[types]`/`[reserved]` become explicit in all three configs;
   `variant-scope` explicit; `[vars]` introduced; `doc_lines` on plates; `PlateRef.name`/`kind`
   and plate `deps` added. Generated output must not change (these are data motions). Delete
   `languages.py`.
3. **Port the C pack.** Translate `gen/emit/c/*.py` into `gen/test/c/templates/` + manifest.
   Byte-parity gate, corert green, valgrind clean. Delete `gen/emit/c/`.
4. **Port the Go pack.** Same, with the format hook carrying gofmt. Byte-parity gate, corert
   green. Delete `gen/emit/go/`, the `BACKENDS` registry, and `[target] language`.
5. **Prove the rule.** Add the JSON Schema pack (`gen/schema/`: config.toml + one template) -- the
   neutral target the plates design used as its forcing function, now actually built. Its
   round-trip check: validate a corpus sample against the emitted schema. Add the CI assertion
   that the pack's commit touches no `*.py`, and a structural test that `gen/` imports cleanly
   with no module or table naming a language.
6. **Docs.** Update `plates.md` section 11 (supersession note), `gen/README.md` (pack anatomy,
   press spec), `AGENTS.md` (the cardinal rule, stated as such).

Then, and only then, the C++ target begins -- as a pack, written without touching Python, which is
the entire point.

## 9. Alternatives considered and rejected

- **Per-target Python plugins** (each pack ships a `backend.py` the generator loads dynamically).
  Satisfies the letter of the rule -- no edits to the generator's files -- and would be the
  cheapest migration (move the existing modules into the packs). Rejected on the spirit: the
  language knowledge would still be Python programs, just relocated; the C++ backend would again
  be two thousand lines of imperative emission; and nothing would force decisions into the plates,
  because a plugin can compute anything. The review's instruction was that the bespoke backends
  "should not exist," not that they should move.
- **Jinja2** (or any expressive template engine, vendored or as a dependency). Mature, excellent
  diagnostics, configurable strictness -- and expressive is the problem: filters, macros,
  arbitrary expressions, and `set` would let the Go backend be reconstituted *inside* template
  files, hiding naming logic where no structural gate can see it; only review discipline would
  stand between the packs and that, and this redesign exists because structure beats discipline.
  It also adds a pip/vendored dependency tree to a deliberately dependency-free Python side.
- **An existing Mustache library** (chevron, pystache) -- the serious alternative, since it shares
  the language's logic-less constitution and would spare us the parser. Weighed and declined, as a
  close call, on four counts: (1) the spec mandates *silent empty output for missing keys*, which
  is disqualifying for a generator and not configurable in chevron (pystache has a strict option
  but is effectively unmaintained; both last released years ago); (2) spec HTML-escaping and weak
  error locations mean we would patch a vendored copy in three places and own the result anyway --
  owning ~400 written-and-spec-tested lines beats owning ~500 vendored lines plus patches by a
  thin margin; (3) the repo's Python side has a deliberate no-dependencies precedent (the
  hand-written XSD parser, tomllib); (4) conformance risk -- the real argument FOR a library -- is
  neutralized by running the official Mustache spec test suite against the press (section 5).
  Because template syntax is pure Mustache, this decision is cheaply reversible: **if during phase
  1 the press exceeds ~600 lines or cannot pass the spec suite, the pre-committed fallback is to
  vendor chevron and patch strictness/escaping/diagnostics** -- with zero template changes.
- **AST-based emitters** (build a language-neutral syntax tree, print per language). A second
  language-shaped abstraction to design, with the per-language printers landing right back in
  Python. Wrong direction entirely.
- **Keeping `languages.py` as "just data."** It is data, but data keyed by language name inside
  the generator is still the generator knowing languages; every new target edits it. Config is the
  same data in the right place.

## 10. Risks and open questions

- **Template debuggability.** Generated-code bugs become template bugs; the press must report
  `template:line` in every error, and a `python3 -m gen render --config C --type note` debugging
  command (render one plate through its matching templates to stdout) should land with phase 1.
- **The quoted-literal compromise** (section 5). The `_q` companions encode one escape family
  (C/C++/Go/Java/JS/Rust-compatible) in the neutral context layer. Revisit trigger: a target whose
  string literals are outside that family (e.g. single-quote-only syntaxes); the extension point
  is the context builder, not the engine.
- **Parity discipline.** The ports in phases 3-4 will be tedious precisely because parity is
  byte-exact; resist "improving" generated output mid-port. Cleanups come after deletion, as
  ordinary template edits.
- **Synthetic-name gating.** `[reserved] members` / `type-suffixes` (section 4) covers the known
  cases (Go's `Children` field, `Child` struct suffix). The residual risk -- a pack composing an
  identifier shape the gate cannot model -- is bounded by the compiler catching it in committed
  output.
- **Cross-pack template sharing.** Go and C templates will rhyme (the same walk, two grammars).
  No sharing mechanism in v1: a pack is self-contained, and duplication across packs is the
  acceptable cost of packs being independently ownable. Revisit only if a third *code* pack makes
  the rhyme painful -- and note the C++ pack is likely to diverge more than it rhymes (sum types,
  references, exceptions).
- **The inherit-chain guard.** The Go backend's loud rejection of derivation chains with children
  in multiple members was backend Python; as a pack, that knowledge has no generic home. Position:
  drop it. The plates dump makes chain shapes visible, no MusicXML schema has the shape, and the
  committed-output compile is the backstop. If it ever bites, the neutral fact ("N chain members
  carry element members") can become plate data a template renders into a `#error`.
- **Engine creep.** The contract is "the template language is Mustache": the press neither adds
  syntax nor restores the spec's lambdas, and the spec test suite pins it there. The review
  question for any proposed press or context-builder feature: "does this let a template make a
  decision the plates should own?" If yes, the answer is no.
