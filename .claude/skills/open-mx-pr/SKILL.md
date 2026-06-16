---
name: open-mx-pr
description: >
  Use this skill when the user says open a PR, create a pull request, or make a PR.
argument-hint: "<prompt>"
disable-model-invocation: false
user-invocable: true
allowed-tools:  >
  Bash(*),
  Edit,
  EnterWorktree,
  ExitWorktree,
  Read,
  Skill,
  Task,
  WebFetch,
  WebSearch,
  Write
---
# /open-mx-pr

Use `<prompt>` and your context to understand what the PR is about. Check to see if there is
documentation on the issue available in `docs/ai/api-feature-audit.md` (which might be the case if
it was previously documented by an api-feature-audit).

Use `gh` for GitHub interactions.

Search the open issues for one that this PR would close or that it is related to. Make a list of issues
that it closes and issues that is related to. It might also be related to other PRs.

When writing the PR, the title should be lowercase and start with one keyword, e.g.
- build: add cache to make fmt for ci
- chore: clean up stale code comments
- docs: add a new ai skill
- feat: add coda to mx::api support

Look at the labels on GitHub, choose the ones that match the pr best.

Write an pr body. Keep it tight and human-readable, but with enough information to understand what
needs to be done.

Determine from the original prompt whether the user wanted you to YOLO and open or whether the user
wanted to see a draft first.

When ready, create the pr and note the pr number. Make sure the PR body contains references to
issues closed and issues/PRs related (if any).

Do NOT attribute yourself in the PR. No "Written by Claud" or "Codex" or whatever. Nope. Do not
take credit for this. Instead you may add the `ai` label as a flag that a coding agent created the
issue.

Do not use excessive bold and italics in the issue body. Go easy on the keywords since they will add
a lot of backtick formatting as well.

Template/sample:

```markdown
## Summary

`accidentalMap` in `Converter.cpp` was missing the entry for `sharpSharp`. Added a single item to the map so that the
double sharps are now fully supported in `mx::api`.

## Testing

- [x] `SharpSharp_PitchData` fails before the fix, passes after
- [x] All PitchData tests pass (`*_PitchData`: 37 assertions in 8 test cases)
- [x] Full test suite passes (3917 assertions in 219 test cases)

## References

- Closes #123
- Progresses #456
- Fixes a bug introduced in #789
```