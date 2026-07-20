---
name: mx-project
description: >
  Create or continue multi-session projects that persist state across agent context windows.
  Use when starting a new project, picking up where the last session left off, or when the user
  says "project", "continue the project", or "start a new project".
argument-hint: "[new] <name> [session] [prompt]"
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
# /project

A project is a longer-running piece of work that outlives a single agent context. Its state lives
in `docs/ai/projects/{{name}}/`.

## Directory

- `docs/ai/projects/{{name}}/AGENTS.md` is the entrypoint. Treat it exactly like a normal AGENTS.md
  file for that directory — read it first, follow what it says.
- `docs/ai/projects/{{name}}/.prompt`: user-owned prompt scratch file. Never read it. If asked to
  write the next prompt, prepend it followed by a `---` separator.
- The directory is for context and coordination only. Work products (code, design docs, etc.) live
  wherever they naturally belong in the repo; projects may add subdirectories/files beyond
  `AGENTS.md` and `.prompt` — document bespoke additions in `AGENTS.md`'s Index section.

## Default mode

This is the default whenever the invocation does not contain the keyword **session**. Keep it
light:

1. Read `AGENTS.md` (create it if this is a new project — see below).
2. Do the work the user asked for.
3. Update `AGENTS.md` only if something durable actually changed (goal, structure, instructions).

No milestones, no log, no state handoff, no end-of-session checklist in this mode.

### New project

`/project new <name> [prompt]`:

1. Create `docs/ai/projects/<name>/AGENTS.md` with frontmatter (`created: YYYY-MM-DD`) and three
   sections: `## Goal`, `## Index`, `## Instructions` (see below).
2. Create an empty `docs/ai/projects/<name>/.prompt`.
3. Ask the user to describe the goal; write it into `## Goal`.

### `AGENTS.md` sections

- `## Goal`: the point of the project, brief (under 200 words).
- `## Index`: guide to bespoke files/subdirectories beyond the standard set (`AGENTS.md`,
  `.prompt`). Mechanical and descriptive, not a narrative.
- `## Instructions`: specific instructions for agents working on the project.

Keep it clean, crisp, and accurate as the project evolves.

## Session mode

Trigger: the user's invocation includes the word **session** (e.g. `/project session <name>`,
"start a session on <name>", "continue the session"). Before doing anything else, read `session.md`
in this skill's directory and follow it instead of Default mode. Session mode layers milestones,
`plan.md`, `state.md`, `log.md`, and a session-end checklist on top of the same `AGENTS.md`
entrypoint — it does not replace it.

## Do Not

- Do not create `plan.md`, `state.md`, or `log.md` unless session mode is active.
- Do not run a session-end checklist, gate on user confirmation, or demand a next-session prompt
  outside session mode.
- Do not place context/tracking files outside `docs/ai/projects/<name>/`. Work products belong
  wherever they naturally live.
