---
name: mx-open-issue
description: >
  Use this skill when the user says open an issue, create an issue, file a new issue, or report a bug.
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
# /mx-open-issue

Use `<prompt>` to understand what the issue is about. Check to see if there is documentation on the
issue available in `docs/ai/api-feature-audit.md` (which might be the case if it was previously
documented by an api-feature-audit).

Use `gh` for GitHub interactions.

Search the open issues for one that it would duplicate. If it would fully or exactly duplicate an
issue, then suggest editing or commenting on the existing issue instead of creating a new one.

Identify related issues and a potential parent/tracking issue.

When writing the issue, the title should be lowercase, e.g. fix missing flerbinator in `mx::impl`.

Look at the labels on GitHub, choose the ones that match the issue best.

Write an issue body. Keep it tight and human-readable, but with enough information to understand
what needs to be done.

Determine from the original prompt whether the user wanted you to YOLO and open or whether the user
wanted to see a draft first.

When ready, create the issue and note the issue number. Make sure the issue body contains references
to related issues and/or PRs (if any are related). If it is a child of another issue, edit the
tracking issue body to add it to the list of tracked issues and also make it a sub-issue of the
tracker.

No self-attribution in the issue body; add the `ai` label instead.
