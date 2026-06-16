---
name: project
description: >
  Create or continue multi-session projects that persist state across agent context windows.
  Use when starting a new project, picking up where the last session left off, or when the user
  says "project", "continue the project", or "start a new project".
argument-hint: "[new] <name> [prompt]"
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

A project is a longer-running piece of work that is larger than a single Agent context can
comprehend. Agents will be conduct a piece of work for the usable life of their context, then they
will be replaced with new Agents to continue the work. Each agent session should be designed to do a
reasonably sized piece of work, then update the project context to prepare the next agent to
continue the work.

## Terminology

A project has:
- sessions (or iterations): pieces of work that can be conducted within a single agent session.
- milestones: a piece of work that can be completed by many agent sessions (or iterations).
- goal: the high-level goal of the project.
- plan: the plan for reaching the goal through milestones.
- current-state: what has been done immediately in the previous session, and what is expected to be
  done in the next session.
- project-log: the log of what agents have done in each session.

## Directory Structure

The project directory is at `docs/ai/projects`. A specific project is as
`docs/ai/projects/{{name}}`. Your entrypoint into a project is `docs/ai/projects/{{name}}/index.md`.
Always start there.

The project directory is for context and coordination only. The actual work of the project may
involve creating or modifying files anywhere in the repository or filesystem - the project directory
is not a sandbox. Keep tracking files, design docs, and other context materials here; keep work
products (code, configs, etc.) wherever they naturally belong.

Projects may add subdirectories and files beyond the standard set below. The `index.md` Index section
should document any bespoke additions so the next agent can find them.

Each new project starts with the following files inside `docs/ai/projects/{{name}}`:
- `.prompt`: a file for the user to craft AI prompts. You should not read this file, the user will
  paste these prompts into sessions. The user may ask you to write the next prompt, in which case
  you should insert the next prompt at the top followed by newlines and `---` to separate it from
  what was in the `.prompt` file previously.
- `index.md`: Entry point to the project directory. Treat it like an AGENTS.md file that appends
  your instruction set beyond this skills instructions.
- `log.md`: append to this frequently to retain a record of what has been done, decisions made,
  pivots, etc.
- `plan.md`: the overarching plan of milestones that will drive us toward the goal.
- `state.md`: what we did last, what program we are following currently, what the next agent should
  do.

Create these 4 files when starting a new project.

## The Standard Files

### `index.md`

Starts with YAML frontmatter for remembering facts like, project start date, pull request numbers,
URLs, etc.

Contains the following sections:
- `## Goal`: the whole point of the project. What are we trying to accomplish overall. Brief, should
  be less than 200 words.
- `## Index`: guide to the files and directory structure of the project (only those details that are
  bespoke and different from the general structure defined by this skill). Mechanical and
  descriptive only, not a narrative.
- `## Instructions`: specific AI instructions for the coding agents to follow when working on the
  project.

As your session creates new files or changes contextual nuances, make sure the `index.md` remains an
accurate and useful landing place for the next agent. Keep it clean, crisp and accurate.

### `log.md`

Dead simple, append-only log. When writing to the log, get the current date and time with

- Starts with a heading: `# {{name}} Log`
- Each entry has a `## YYYY-MM-DD HH:MM` header (24-hour, machine timezone via
  `date '+%Y-%m-%d %H:%M'`)
- Entries are plain prose, no bold formatting, concise but complete, remove unnecessary words
- Always append to the bottom (chronological order)
- Record: what was done, what was decided (and why), what was discovered, what changed direction
- Do not duplicate the agenda (what needs to be done) or the design (current state of the design),
  just the latest facts

You may read the log to gather context on how the project has progressed.

### `state.md`

This is your file to use so that the next agent knows exactly where we are in the project, what we
did in the current session, and what we expect the next agent to do in the subsequent session. Key
information needed in the document:
- which milestone or part of the project are we working on
- what was done in the previous session
- what the goal and instructions are for the next session
- gotchyas or memories from recent sessions that the next agent needs to watch out for

## A Note on Design Docs

A plan may have one or more design docs. A design doc is a static snapshot of the current state of
design. It must not record historical artifacts of the design process, such as "the frobulator no
longer frobulates flux capacitors". That is not a design statement, that is a historical note. A
design statement is only what is true about the design currently, not a record of what changed about
the design. Design docs should be kept in sync as the design changes through sessions.

## Invocation

### `/project new <name> [prompt]`

1. Create `docs/ai/projects/<name>/` with the 4 standard files:
   - `index.md` with frontmatter (`created: YYYY-MM-DD`) and empty Goal, Index, Instructions
     sections.
   - `plan.md` with a `# <name> Plan` heading.
   - `state.md` with initial content: "Project created. No sessions yet."
   - `log.md` with a timestamped creation entry.
2. Verify all 4 files exist with `ls docs/ai/projects/<name>/`.
3. Ask the user to describe the goal. Write it into `index.md ## Goal`.
4. Draft initial milestones into `plan.md` based on the goal discussion.

### `/project <name> [prompt]`

Continue an existing project. Follow the Session Flow below.

## Session Flow

1. Read `index.md`, `plan.md`, then `state.md`. Gather additional context as needed.
2. If `state.md` is clear enough about what to do this session, proceed. Otherwise check with the
   user.
3. Do the work. Write to `log.md` as you go, especially when decisions are being made.
4. At session end, follow the Session End Checklist below.

## Session End Checklist

Complete every item before ending the session:

1. Append a timestamped entry to `log.md` summarizing what was done and decided.
2. Overwrite `state.md` with: what was done this session, what the next session should do, and any
   gotchas the next agent needs.
3. Update `index.md` if file paths or structural context changed.
4. Update `plan.md` if milestones shifted.
5. **USER GATE:** Show the user the updated `state.md` and the suggested next-session prompt. Do not
   end until the user confirms.
6. Print the next-session invocation: `/project <name> <prompt>`

## Do Not

- Do not skip reading `index.md` and `state.md` at session start, even if the user provides context
  verbally. The files are authoritative.
- Do not put historical design evolution in design docs. Design docs describe current state only.
  History goes in `log.md`.
- Do not defer `log.md` writes to session end. Log decisions and pivots as they happen during the
  session.
- Do not skip the Session End Checklist. If the session is ending for any reason (user says stop,
  context is filling, work is done), trigger the checklist immediately.
- Do not place context and tracking files (index.md, plan.md, state.md, log.md, design docs) outside
  the `docs/ai/projects/<name>/` directory. Work products belong wherever they naturally live.
