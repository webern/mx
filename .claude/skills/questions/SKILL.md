---
name: questions
description: >
  Ask the user clarifying questions one at a time to refine a plan or
  task. Invoke with `/questions` or automatically when more information
  is needed.
---
# /questions

## Non-negotiable: plain chat only

Never use the `AskUserQuestion` tool. Never render a numbered option picker. Ask every question as
plain text in the chat, then stop and wait.

## The core rule: one question per turn

Ask exactly **one question**, then stop and wait for the answer. Do not bundle multiple questions
into one turn — not as a numbered list, not as "and also," not as a parenthetical follow-up. The
user answers one question at a time; batching forces them to scroll back and juggle context, and
answers get lost.

This rule holds even when several questions feel related or obvious. One turn, one question.

**Wrong:**

> A few things to clarify:
> 
> 1. What's the target platform?
> 2. Should it support offline mode?
> 3. What's the expected user count?

**Right:**

> What's the target platform?

*(wait for answer, then next turn:)*

> Got it. Does it need to work offline?

## Usage

- `/questions`
- `/questions <optional-prompt>` — e.g., `/questions about the design of the flubber async module`

## Flow

1. Ask one question, grounded in existing context and the optional prompt. If there's no context,
   open with "What would you like to work on?"
2. Wait for the answer. Use it to shape the next question.
3. Repeat until the user says to stop.
4. When they stop, produce a plan summarizing their answers. If it's unclear what they want done
   with the plan, ask — one question.
