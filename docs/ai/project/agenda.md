# Agenda

This is the project plan for supporting MusicXML 4.0

## Phase 0: Prepare for AI Development (DONE)

## Phase 1: Reverse Engineer Codegen

This is a difficult process in which we need to reverse engineer and write a codegen program that,
when we run it using the same/current MusicXML schema, regenerates what we now see in mx/core.

We are not ready to work on MusicXML 4.0 yet. First we want to be able to reliably reproduce the
code we find in mx/core, such that running codegen on the current schema produces the minimal diff
possible to the generated code.

First I want to design the rules and evaluation criteria for a "regenerated" mx/core. When the gen
program runs, it should output it's results on top of mx/core. The diff should be as small as
possible from the generated code, however, no cheating. Everything in musicxml.xsd must result in
generated code output.

When evaluating a small diff, consider the following rubrick. We want to minimize CHANGE_PENALTY.
Change points are assigned, 1 per line (whether addition or deletion), multiplied by the following
factors:
- 1: whitespace anywhere
- 1.5: comment changes in `mx/core`
- 2: non-whitespace code changes in `mx/core/*.cpp` files
- 5: non-whitespace code changes in `mx/core/*.h` files
- 10: changes in `mx/impl` files
- FAIL (infinity): changes in `mx/api` files

These multiply by a diff analysis and lead to a CHANGE_PENALTY score, less is better. If static
analysis of the codegen program reveals that it is skipping parts of MusicXML XSD, then the program
is disqualified, it failed.

Another analysis is conducted on the codegen program itself for readability, human
comprehensability, separation of concerns, extensibility and soundness of design. For example, a
program with a bunch of if/else statements to deal with special conditions would fair worse than a
program that has a well designed mechanism for defining codegen overrides or rule exceptions. TODO:
determine what sort of analysis tools are available for this. Call it CODEGEN_PROGRAM_QUALITY.

Codegen program language: open to discussion and choosing the best tool for the job.

Now consider that more than one agent attempts to solve this codegen problem in parallel producing
different programs. They choose a language, then figure out how to generate mx/core from
MusicXML.xsd keeping the scoring rubric in mind. They must produce a program that generates code and
that passes the mx quality gates. At the end their program and it's output are scored by an
impartial agent.

Once we have a couple of attempts at this, we take the best one and if it can still be improved, we
could repeat the process but this time using agents who are asked to improve the winning program.

What do you think of this plan? Will it work? Would you be able to help me lock down the plan better
with the grill me skill should I choose to move forward?

### Open Questions Requiring Grill-Me Sessions

Each item below needs a `/grill-me` session before Phase 1 agent work begins. Each session should
produce a doc in `docs/ai/project/`.

- [x] **Override mechanism** (`phase1-overrides.md`) - Are hand-coded exceptions to codegen rules
  allowed? If so, what form do they take, and how does CODEGEN_PROGRAM_QUALITY penalize them? This
  resolves the tension between "no cheating" and the known human choices in mx/core.
  OUTPUT=./gen-overrides.md

- [x] **CHANGE_PENALTY threshold** (`phase1-scoring-thresholds.md`) - What numeric score is "good
  enough" to exit Phase 1 and move to Phase 2? Without a pass/fail threshold the phase has no exit
  criterion. OUTPUT=./change-penalty.md
  - Note from `gen-overrides.md`: the override investigation found `mx/core` has legacy
    XSD-infidelity (the "FIX" category), e.g. 45 attribute structs silently omit the `color`
    attribute the XSD declares. A faithful generator must emit it, producing a large but correct
    diff. This session must give faithfulness precedence over raw diff size, or the threshold
    rewards reproducing legacy bugs. See the FIX section and Consequences in `gen-overrides.md`.

- [ ] **CODEGEN_PROGRAM_QUALITY criteria** (`phase1-quality-criteria.md`) - How is program quality
  measured concretely? Static analysis tools, manual rubric, or both? The current plan has a TODO
  here. OUTPUT=./codegen-program-quality.md

- [ ] **Language constraints** (`phase1-language-constraints.md`) - Which languages are permitted
  for competing agents? Leaving this fully open produces incomparable results and risks poor tool
  choices. OUTPUT=./language-constraints.md

- [ ] **Systematic-gap scoring** (`phase1-systematic-gaps.md`) - How does scoring handle a program
  that is systematically correct for 95% of elements but misses one XSD pattern that appears 300
  times? The current rubric has no partial-credit mechanism. OUTPUT=./scoring-gap.md (idea, perhaps
  this can be solved by assigning a very large score for getting something that passes mx quality
  gates without skipping anything in MusicXML.xsd)
  - Note from `gen-overrides.md`: the missing-`color` FIX (45 structs) is a concrete instance of
    the inverse problem - a systematic gap that already exists in `mx/core`, where the *correct*
    generator must add ~300 lines and the rubric as written penalizes it. Use this as the worked
    example for the partial-credit / faithfulness-over-diff mechanism.

## Phase 2: Modernize or Improve the C++

Before we codegen 4.0, maybe we should improve what is being generated from the current MusicXML
Spec for performance and for new C++ features, e.g. enum variants etc. Can it be more optimal?

## Phase 3: Analyze which features of MusicXML are inaccessible from mx/api

## Phase 4: Generate MusicXML 4.0 Types

## Phase 5: Surface MusicXML 4.0 Features to mxapi types

## Random Ass TODOs

Not related to the larger project plan:

- [ ] Remove CircleCI Stuff
- [ ] Cleanup the Changelog
