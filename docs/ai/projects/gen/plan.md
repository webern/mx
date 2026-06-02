# gen Plan

## Milestone 1: revgen — reverse-engineer codegen ✅

Complete 2026-05-21. Generator produces every C++ class in `mx/core` from `docs/musicxml.xsd`.
`SKIP_ELEMENTS` and `CHOICE_SKIP` empty. See `log.md`.

## Milestone 2: fix-gen — fix generator bugs surfaced by failing tests ✅

Complete 2026-05-22. `make test-all` 3028/3028. All fixes landed in the generator; workarounds
reverted. One item deferred to M5/M6: HEAD's hand-applied MusicXML 4.0 `UpDownNone` backport vs.
schema-faithful 3.x regen (TODOs in `mx/impl/NotationsWriter.cpp:398`,
`mx/impl/ArpeggiateFunctions.cpp:35`).

## Milestone 3: fix-core-dev — fix bugs surfaced by new core-dev test mode ✅

Complete 2026-05-22 (5 iterations). 31 corert failures triaged into 10 `{file}.invalid` markers and
4 real fixes. The `src/private/mxtest/corert/` harness is now part of `make test-all`. See `log.md`
M3 for per-iteration breakdown.

## Milestone 4: test fixer (clamping-aware comparison) ✅

Complete 2026-05-25. A new `Fixer` patches the expected tree before comparison via per-file
`<file>.fixup.xml` sidecars. Convention documented in `data/README.md`. Design doc at
`src/private/mxtest/corert/Fixer.h`. Closed at 387/387 `test-core-dev` pass and 3065/3065
`test-all`.

## Milestone 5: increase test coverage ✅

Completed 2026-05-30. Added real-world corpus files. Generated 235 synthetic files
(`data/synthetic/`).

## Milestone 6: better-gen — fix garbage

The generator is `~14k` lines of bad Python. Refactor. Use the corert suite as the correctness
oracle.

Sub-milestones:

### 6A_QUAL_GATES: research code quality analysis tools for python. ✅

Delivered 2026-06-01: `make gen-quality` and `make gen-lint`

### 6B_DATA_MODEL:

Consider if we were to generalize this program to support the emission of other forms of code
besides just the C++ mx/core types that it currently generates. Potential targets could include a
Rust library, documentaion in HTML or Markdown, an Erlang library, an alternate specification (for
example conversion to JSON schema). We want to begin a process of generalizing our program to
support other use-cases MusicXML XSD transformation.

Template Context Type: We must consider all the code-generating functions and their input. What is
the common data-shape that all these generators consume? Can it be abstracted away from C++ grammar
and syntax such that a codegenerator for a different grammar and syntax can also consume this common
type?

We should consider a series of transforms:
- parse: we parse the XSD document into an organizational type still closely tied to the XSD
  grammar. I think you've already done this, but let's make sure after this step we don't need to go
  back to the XSD again for anything. I'll refer to this as the xsd-preprocess data type.
  - each XSD construct, especially anonymous ones, need to be given a reproducable ID
  - the reproducable ID should not be sensitive to changes in the specification
  - the common-sense "same" complexType, for example, should get the same ID assignment accross
    MusicXML XSD versions
- contexts: instantiate a complete set of context structs from the xsd-preprocess data that could be
  used, for example, as input into a templating system.
- create a dependency topology order such that leaf node objects can be created first, followed by
  those that directly depend on them, and on up the tree such that we process things in the order
  that, e.g. a build system would.
- idea: not sure if yagni, each generator could be given an oportunity to mutate the state of the
  context so that future generators that depend on the output of this one could receive a signal
  from the dependency

Rework the existing program so that all generating functions now take this context object as its
argument.

### 6C_NEXT_AND_BEYOND

Definition of the next steps is intentionally left TBD depending on the output ob 6B. I don't want
to burdon the LLM with where we are going next.

## Milestone 7: mxml4-types — generate MusicXML 4.1 types

Replace `docs/musicxml.xsd` with MusicXML 4.1, regenerate, fix all existing tests. Watch for
backported / bolted-on features (SMuFL, `UpDown`, …) that were added with hacks to 3.0/3.1 but are
first-class in 4.0. Be backward-compatible with files mx may have written using those hacks. Restore
the `mx/impl` TODOs left from revgen.

## Milestone 8: Surface MusicXML 4.1 features in mx/api

