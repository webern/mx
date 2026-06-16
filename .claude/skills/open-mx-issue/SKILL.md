---
name: open-mx-issue
description: >
  Use this skill when the user says open an issue, create an issue, or file a new issue.
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
# /open-mx-issue

Use `<prompt>` to understand what the issue is about. Check to see if there is documentation on the
issue available in `docs/ai/api-feature-audit.md` (which might be the case if it was previously
documented by an api-feature-audit).

Use `gh` for GitHub interactions.

Research the issue locally, make sure you understand it.

Search the open issues for one that it would duplicate. If it would fully or exactly duplicate an
issue, then suggest editing or commenting on the existing issue instead of creating a new one.

Figure out which issues are related. Figure out if there is an issue that would serve as a good
parent for this new one to be a child of.

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

Do NOT attribute yourself in the issue. No "Written by Claud" or "Codex" or whatever. Nope. Do not
take credit for this. Instead you may add the `ai` label as a flag that a coding agent created the
issue.
