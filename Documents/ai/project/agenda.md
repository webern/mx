# Agenda

This is the project plan for supporting MusicXML 4.0

## Phase 0: Prepare for AI Development

- [x] Create a Makefile
- [ ] fix the line endind issues in markdown files like README.md
- [ ] Get .github CI modernized and working
- [ ] Put a fmt'er in place
- [ ] Put linters in place
- [ ] get a handle on .gitattributes and what should be done with it
- [ ] Put a check of Lints and Fmt's in place
- [ ] See how this now interacts with JetBrains CLion (which seems to create a `cmake-build-debug` directory)
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