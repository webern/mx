# mx-project: Session Mode

Read this only when the user's `/project` invocation includes the keyword "session". It layers
structured, multi-session bookkeeping on top of the lightweight default mode described in
`SKILL.md`; it does not replace `AGENTS.md` as the entrypoint.

## Terminology

A project in session mode has:
- sessions: pieces of work conducted within a single agent context.
- milestones: pieces of work that may span many sessions.
- plan: the route to the goal, through milestones.
- state: what the previous session did, and what the next session is expected to do.
- log: the append-only history of what happened in each session.

## Additional Files

Session mode adds a `session/` subdirectory (holding `plan.md` and `state.md`) plus a `log.md`,
alongside `AGENTS.md` and `.prompt`:

### `session/plan.md`

The overarching plan of milestones driving toward the goal.

### `session/state.md`

So the next agent knows exactly where things stand: which milestone/part of the project is active,
what was done in the previous session, the goal/instructions for the next session, and any
gotchas or memories recent sessions surfaced.

### `log.md`

Append-only.
- Starts with a heading: `# {{name}} Log`.
- Each entry has a `## YYYY-MM-DD HH:MM` header (24-hour, machine timezone; get the current date
  and time with `date '+%Y-%m-%d %H:%M'`).
- Entries are plain prose, no bold formatting, concise but complete, remove unnecessary words.
- Always append to the bottom (chronological order).
- Record: what was done, what was decided (and why), what was discovered, what changed direction.
- Do not duplicate the agenda (that's plan/state) or the design's current state (that's the design
  doc) — just the latest facts.

## A Note on Design Docs

A plan may have one or more design docs. A design doc is a static snapshot of the current state of
the design, kept in sync as the design changes through sessions. Historical/rejected reasoning
goes in `log.md` (a short "alternatives considered" section in the design doc is fine).

## Entering Session Mode on a Project That Hasn't Used It Yet

If `session/plan.md`, `session/state.md`, or `log.md` don't exist yet, create them before doing
anything else:
- `session/plan.md`: `# <name> Plan` heading; draft milestones based on discussion with the user.
- `session/state.md`: "Project created. No sessions yet."
- `log.md`: a timestamped creation entry.

## Session Flow

1. Read `AGENTS.md`, then `session/plan.md`, then `session/state.md`. Gather additional context as
   needed.
2. If `session/state.md` is clear enough about what to do this session, proceed. Otherwise check
   with the user.
3. Do the work. Log decisions and pivots to `log.md` as they happen — don't defer to session end.
4. At session end, run the Session End Checklist below.

## Session End Checklist

Complete every item before ending the session:

1. Append a timestamped entry to `log.md` summarizing what was done and decided.
2. Overwrite `session/state.md` with: what was done this session, what the next session should do,
   and any gotchas the next agent needs.
3. Update `AGENTS.md` if file paths or structural context changed.
4. Update `session/plan.md` if milestones shifted.
5. **USER GATE:** Show the user the updated `session/state.md` and the suggested next-session
   prompt. Do not end until the user confirms.
6. Print the next-session invocation: `/project session <name> <prompt>`

## Do Not

- Do not skip reading `AGENTS.md` and `session/state.md` at session start, even if the user
  provides context verbally. The files are authoritative.
- Do not put historical design evolution in design docs. Design docs describe current state only.
  History goes in `log.md`.
- Do not defer `log.md` writes to session end. Log decisions and pivots as they happen during the
  session.
- Do not skip the Session End Checklist. If the session is ending for any reason (user says stop,
  context is filling, work is done), trigger the checklist immediately.
- Do not place context and tracking files (`session/plan.md`, `session/state.md`, `log.md`, design
  docs) outside the `docs/ai/projects/<name>/` directory. Work products belong wherever they
  naturally live.
