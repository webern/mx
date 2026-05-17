# Project

This is the directory for supporting MusicXML 4.0.

./agenda.md is the TODO list of work items. ./build-and-ci-design.md is the plan for how Makefile
and CI targets will lint, build, test and fmt the code.

## Note on mx/core/

The files in `src/private/mx/core/` were originally machine-generated from the MusicXML XSD.
The codegen program no longer exists. These files are normal hand-edited source now and must pass
all quality gates. A future codegen rewrite will re-own them. Do not treat them as generated or
read-only.
