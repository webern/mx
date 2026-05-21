# gen Plan

## Milestone 1: Reverse-engineer codegen (Phase 1)

Produce a generator that, given `docs/musicxml.xsd`, regenerates `mx/core` with minimal unexplained
diff. Exit criteria (from `design/scoring.md`):

- **Gate A** (no-skip): every XSD element, type, attribute, attributeGroup, and group reaches a
  code-emitting path. No skipping.
- **Gate B** (quality): `make fmt && make check && make test-all` passes.
- **Gate C** (classification): 100% of diff lines attributed to a category (RULE/EXC/FIX/ANOMALY).
- **Gate D** (residual): unexplained residual penalty = 0.

Current status: Gate A not met (14 elements skipped). Gate B partially met (make check passes,
make test-all fails). Gates C and D not yet measured against the classification framework.

### Remaining work for Milestone 1

Ordered by expected impact on gates:

1. Attribute group expansion (unblocks Gate A and Gate B)
2. Choice class generation for remaining skipped elements (Gate A)
3. Keyword escaping fixes (Gate B)
4. streamContents pattern refinements (Gate D)
5. Classification of all remaining diff lines (Gate C)
6. Residual reduction to zero (Gate D)

### Open design questions (need /grill-me sessions)

Two grill-me sessions were completed (overrides -> `design/overrides.md`, scoring ->
`design/scoring.md`). Three remain:

- [ ] **CODEGEN_PROGRAM_QUALITY criteria** - how is program quality measured concretely? Static
  analysis tools, manual rubric, or both?
- [ ] **Language constraints** - which languages are permitted for competing agents? (Currently
  Python is the de facto choice.)
- [ ] **Systematic-gap scoring** - how does scoring handle a program that is systematically correct
  for 95% of elements but misses one XSD pattern that appears 300 times? The missing-color FIX
  (45 structs) is a concrete instance of the inverse problem.

## Milestone 2: Improve codegen / modernize C++ (Phase 2)

Improve the generated code from the current MusicXML spec:
- more consistent patterns
- modern C++ (e.g. std::variant to reduce object sizes)
- fewer bespoke alterations

## Milestone 3: Analyze mx/api coverage (Phase 3)

Determine which MusicXML features are inaccessible from `mx/api`.

## Milestone 4: Generate MusicXML 4.0 types (Phase 4)

Replace `docs/musicxml.xsd` with MusicXML 4.0, regenerate. Fix all existing tests. Watch for
backported or bolted-on features like SMuFL that were added with hacks to 3.0/3.1 but are now
first-class in 4.0. Be backward compatible with files that `mx` may have written with those hacks.

## Milestone 5: Surface 4.0 features to mx/api (Phase 5)

Expose new MusicXML 4.0 features through the public API. Prioritize those users will want.

## Milestone 6: PR

Open a PR introducing MusicXML 4.0 support.
