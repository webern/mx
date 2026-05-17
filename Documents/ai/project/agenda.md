# Agenda

This is the project plan for supporting MusicXML 4.0

## Phase 0: Prepare for AI Development

- [x] Create a Makefile
- [x] fix the line endind issues in markdown files like README.md
- [x] write build-and-ci-design.md
- [x] implement changes per build-and-ci-design.md
- [ ] Change local dev workflow and CI workflow based on the changed ./build-and-ci-design.md file
- [ ] Push to CI in PR 145 and iteratively refine any mistakes so that the ci yaml works correctly (but we will fix lints and warnings locally)
- [ ] In parallel fix lints and warnings until make fmt && make check && make test-all passes locally
- [ ] push the changes to PR 145 and monitor for passing CI
- [ ] clean up the commits locally such that we have the originally desired commit sequence, i.e. fixup the original
  commits so that they work
- [ ] prompt the user for a force push to PR 145 when commits are clean
- [ ] monitor PR 145 after the force push

- [ ] See how the project now interacts with JetBrains CLion (which seems to create a `cmake-build-debug` directory)
- [ ] Codefy Change Quality Gates in AGENTS.md

## Phase 1: Reverse Engineer Codegen

- [ ] Design a process by which the AI agent can iteratively work on the codegen problem

## Phase 2: Modernize or Improve the C++

Before we codegen 4.0, maybe we should improve what is being generated from the current MusicXML Spec for performance
and for new C++ features, e.g. enum variants etc. Can it be more optimal?

## Phase 3: Analyze which features of MusicXML are inaccessible from mx/api

## Phase 4: Generate MusicXML 4.0 Types

## Phase 5: Surface MusicXML 4.0 Features to mxapi types

## Random Ass TODOs

Not related to the larger project plan:

- [ ] Remove CircleCI Stuff
- [ ] Cleanup the Changelog