# CHANGE_PENALTY Threshold and Phase 1 Exit Criterion

What numeric score is "good enough" to exit Phase 1? A generous numeric "good enough" is the wrong
instrument. The exit criterion is a conjunction of gates; the numeric penalty's job is to be driven
to zero on the *unexplained residual*, not to define a tolerance budget. This doc depends on the
classification taxonomy established in `overrides.md` (RULE / EXC / SUBSTRATE / FIX / ANOMALY).

## Context

Two facts shape every decision here, both inherited from `overrides.md`:

- **4.0 codegen is a one-time run.** Phase 1 reproduces the current `mx/core` from the current
  schema so the generator can be trusted before it is pointed at 4.0. The pass/fail bar exists to
  certify that trust, not to protect a recurring build.
- **`mx/core` contains legacy XSD-infidelity (the FIX category).** A faithful generator must emit
  code that differs from the checked-in source. The exit criterion must score that difference as
  *correct*, or it certifies a generator that reproduces bugs.

## The flaw in CHANGE_PENALTY as written

The agenda computes penalty over the raw diff between regenerated output and checked-in `mx/core`: 1
point per added or deleted line, multiplied (1 whitespace, 1.5 comment, 2 `core/*.cpp`, 5
`core/*.h`, 10 `mx/impl`, FAIL for `mx/api`). Lower is better.

Worked example, verified in code. `docs/musicxml.xsd` reaches the `color` attribute via
`print-style` for 71 complexTypes that have an attributes struct. 26 emit the field; 45 omit it.
`src/private/mx/core/elements/AccidentalAttributes.h` is a confirmed omission - it has no `color`
member, while `ArrowAttributes.h` and 25 others do. A faithful generator emits `color` in all 71.
That is roughly 45 added `.h` lines at the 5x multiplier (`~225 points`) plus the matching `.cpp`
accessor and stream lines at 2x, for one FIX.

So the raw-diff rubric ranks the *unfaithful* generator - the one that hardcodes "skip color in
these 45 structs" - strictly above the faithful one. That hardcoded skip is precisely the cheating
the agenda forbids ("no cheating", "Everything in musicxml.xsd must result in generated code
output"). Any single numeric "good enough" threshold over the raw diff has this inversion built in.
Picking a number does not fix it; it sets the price of the bug.

A second, structural flaw: the metric counts a modified line as delete + add (2 points) and a moved
block as N deletes + N adds. A single wrong ordering rule applied by an otherwise correct generator
reorders entire files and accrues penalty proportional to file size for what is one root-cause
defect. The raw metric punishes one mistake thousands of times.

## The mechanism: penalty is computed over the unexplained residual

CHANGE_PENALTY must not be computed over the raw diff. It is computed over the **residual** - the
diff that remains after every differing line is attributed to a category from `overrides.md`.
The generator already takes a declarative overrides input; that same manifest is the ledger that
attributes diff lines:

- **RULE-correct**: lines that match checked-in `mx/core`. Zero diff, zero penalty.
- **FIX**: lines that differ because the generator correctly emits XSD content `mx/core` never
  emitted. Declared in the manifest, each entry validated against `docs/musicxml.xsd` by an
  analyzer. Scored **zero**. A FIX whose declared XSD justification does not check out is not a FIX
  \- it falls through to residual.
- **EXC**: lines produced by a justified slot reproducing a deliberate human choice. Scored zero
  when the slot output matches the checked-in source; any delta is residual.
- **SUBSTRATE**: not generation output; out of diff scope by construction (namespace-derived,
  analyzer-enforced per `overrides.md`).
- **ANOMALY**: each anomaly carries a recorded decision (reproduce or correct). The resulting diff
  is scored zero *because the decision is recorded and justified*, not because it is small. An
  anomaly with no recorded decision falls to residual.
- **Residual**: every diff line with no valid classification. This, and only this, is what
  CHANGE_PENALTY scores, using the agenda's multiplier table.

This converts the metric from "size of the diff" to "size of the *unexplained* diff" and is the
concrete faithfulness-over-diff-size mechanism the agenda asks both this session and
`scoring-gap.md` to supply. FIX no longer competes with cheating: emitting `color` in 45 structs is
a declared, XSD-validated FIX worth zero; *not* emitting it is now the defect, because the missing
lines become unexplained residual against the faithful target.

## The exit criterion: four gates, not one number

Phase 1 exits when a generator passes all four. The first three are binary. The fourth carries the
number, and the number is small by design.

**Gate A - No-skip (binary, already in the agenda).** Static analysis of the generator confirms
every `xs:element`, `xs:simpleType`, `xs:complexType`, `xs:attribute`, `xs:attributeGroup`, and
`xs:group` in `docs/musicxml.xsd` reaches a code-emitting path. Skipping disqualifies. The
scoring-gap note's "very large score for not skipping" is best realized as this hard gate, not as
points; a score, however large, is a budget, and a budget is a cheating surface.

**Gate B - Quality gates (binary).** The regenerated tree passes
`make fmt && make check && make test-all` (test-all because the change touches
`src/private/mx/core/`). Non-negotiable and independent of any diff measurement.

**Gate C - Classification completeness (binary).** 100% of diff lines are attributed to a category
above. An unclassified diff line is a defect, not a tolerated cost. This gate is what makes Gate D
safe: with no unclassified lines permitted, there is no budget to spend on unjustified deviation
spread thin across 1182 files.

**Gate D - Residual target (numeric).** The residual penalty, computed with the agenda's multipliers
over only the unexplained residual, must be **zero**, with a single published, enumerated allowance
list for irreducible noise (see below). There is no generous "good enough" band. A nonzero tolerance
is itself the cheating surface this whole design removes: spread across a 113k-line tree it silently
funds exactly the legacy-bug reproduction the agenda warns about.

### Why zero, and why an enumerated allowance instead of a band

A tolerance band answers "how much unjustified wrongness do we accept?" The correct answer for a
one-time generator that becomes the new hand-maintained source is none - every nonzero diff line is
either explained (classified, zero penalty) or a defect (fix it or justify it). The allowance list
is not a band; it is a closed, named set of noise sources agreed in advance, each of which is then
itself zero:

- Formatting is normalized away. `make fmt` applies the same pinned clang-format-18 Microsoft style
  to both the regenerated output and the checked-in tree (the tree was reformatted in commit
  662c7ff9). Whitespace residual after `make fmt` is expected to be 0; if it is not, that is a
  generator defect, not noise.
- The license banner (`// MusicXML Class Library` / copyright / MIT, verified as the first three
  lines of `AccidentalAttributes.h`) is RULE-derivable and reproduced verbatim. There is no
  historical "generated by" banner to reconstruct.
- Any other claimed-irreducible source must be added to this list by name with justification before
  it is allowed, and the goal remains to drive it to a RULE that produces zero.

If the first reverse-engineering attempt cannot reach zero residual, that is expected and is handled
by the ranking metric below - it does not lower Gate D. Phase 1 iterates (the agenda's "repeat the
process improving the winning program") until Gate D is met. Gate D is the exit bar; it is not the
within-iteration progress signal.

## Two distinct uses the agenda conflates

The agenda uses one metric for two jobs. They need different treatment.

1. **Ranking competing generators** ("scored by an impartial agent ... take the best one"). Rank by
   residual penalty (Gates A-C as prerequisites to be ranked at all). Ranking must use
   residual-after-classification, never raw diff - otherwise the cheater that reproduces the
   missing-`color` bug out-ranks the faithful generator, and the competition selects the wrong
   winner to carry into the improvement round. Among generators with equal residual, rank by **fewer
   distinct root-cause defects**, then by raw line-penalty as the final tiebreaker.

2. **Exiting Phase 1** (the pass/fail this doc resolves). Gates A-D. This is not a ranking; it is a
   certification that the winner is trustworthy to point at 4.0.

The unit for ranking and for "is this defect-free" is the **distinct root-cause defect**, not the
line. One wrong ordering rule that reorders 40 files is one defect, not thousands of points.
Line-weighted penalty is retained only as telemetry while iterating and as the last tiebreaker.
Counting a systematic gap that recurs 300 times as one defect rather than 300x penalty is the
partial-credit problem owned by `scoring-gap.md`; this doc establishes the shared foundation
(residual-after-classification, defect-as-unit) and explicitly defers the recurrence-counting
mechanism there. The two docs must not diverge on the foundation.

## Multiplier table review

The agenda's table is mostly sound for scoring residual. Three adjustments:

- **`mx/impl` at 10x is too lenient as a mere cost.** A core-only generator that produces any
  `mx/impl` diff has changed a generated core API signature the conversion layer consumes (added
  fields cannot break `mx/impl`; renamed or removed ones can). Treat any `mx/impl` residual as a
  **Gate C / Gate D failure requiring explicit justification**, not a payable 10x cost. `mx/api` =
  FAIL is correct and stays.
- **Modify = 2 points and move = 2N points stand**, but only for residual. Because residual excludes
  classified lines and the unit of judgment is the defect, the double-count no longer punishes one
  mistake thousands of times - it inflates the telemetry number for a single defect, which is
  acceptable for a tiebreaker.
- **`.h` 5x vs `.cpp` 2x vs comment 1.5x vs whitespace 1x** are reasonable interface-weighted values
  and need no change for residual scoring.

## Recommendation summary

1. Compute CHANGE_PENALTY over the **unexplained residual**, never the raw diff. Classified lines
   (FIX, EXC, SUBSTRATE, recorded ANOMALY) score zero.
2. Phase 1 exits on the conjunction of **Gate A** (no-skip, disqualifying), **Gate B**
   (`make fmt && make check && make test-all`), **Gate C** (100% diff lines classified), and **Gate
   D** (residual penalty = 0 modulo a closed, named allowance list).
3. Replace the "good enough numeric band" question with "residual = 0"; a tolerance band is a
   cheating surface on a 113k-line tree.
4. Use residual penalty to **rank** competing generators; use the four gates to **exit**. Make the
   unit of both ranking and exit the distinct root-cause defect, with line-penalty as telemetry and
   final tiebreaker only.
5. Promote `mx/impl` residual from a 10x cost to a justify-or-fail gate condition.

## Boundary with the other open questions

- `scoring-gap.md` (systematic-gap / partial credit): owns counting an N-times-recurring gap as one
  defect. Inherits this doc's residual-after-classification foundation and defect-as-unit; must not
  redefine them.
- `codegen-program-quality.md`: `CODEGEN_PROGRAM_QUALITY` judges the override mechanism's design
  (per `overrides.md`), independent of CHANGE_PENALTY. The two scores are orthogonal; a
  generator can pass Gate D with a poorly designed override mechanism and still fail Phase 1 on
  quality. Note the cross-dependency, do not merge the metrics.
- `language-constraints.md`: unaffected by this doc.

## Method and evidence

- Missing-`color` FIX verified directly: `grep` confirms no `color` member in
  `src/private/mx/core/elements/AccidentalAttributes.h`; `Color color` present in
  `ArrowAttributes.h` and 25 siblings. Counts (71 reach `color`, 26 emit, 45 omit) are from
  `overrides.md`, reproducible via the probes in `gen/`.
- Tree scale: `src/private/mx/core` is 1182 files and ~113k lines; the 5x `.h` multiplier over 45
  FIX additions (~225 points for one FIX) is the concrete inversion that forces
  residual-after-classification rather than a tuned threshold.
- Formatting normalization assumption rests on `build-and-ci-design.md` (pinned clang-format-18,
  Microsoft style, applied to `mx/core`) and the reformat in commit 662c7ff9; Gate D treats
  post-`make fmt` whitespace residual as a defect, not noise, on that basis.

## Deferred to Phase 1 implementation

- The analyzer that validates each declared FIX against `docs/musicxml.xsd` (the mechanism that
  makes FIX-scored-zero non-gameable) is a build task, not a design question.
- The exact diff tooling that attributes lines to manifest entries is an implementation choice; the
  requirement is per-line attribution with no unclassified lines, not a specific tool.
- ANOMALY decisions (reproduce vs correct) are recorded per `overrides.md`'s ANOMALY table; this
  doc only requires that the decision exists and is justified for the line to score zero.
