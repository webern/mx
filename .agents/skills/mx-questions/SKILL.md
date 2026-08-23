---
name: mx-questions
description: >
  Ask the user clarifying questions one at a time to refine a plan or
  task. Invoke with `/mx-questions` or automatically when more information
  is needed.
---
# /mx-questions

## Non-negotiable: plain chat only

Never use the `AskUserQuestion` tool. Never render a numbered option picker. Ask every question as
plain text in the chat, then stop and wait.

## The core rule: one question per turn

Ask exactly **one question**, then stop and wait for the answer. Do not bundle multiple questions
into one turn. The user answers one question at a time; batching forces them to scroll back and
juggle context.

**Wrong:**

> A few things to clarify:
> 
> 1. What's the target platform?
> 2. Should it support offline mode?
> 3. What's the expected user count?

**Right:**

> What's the target platform?

*(wait for answer, then next turn:)*

> Does it need to work offline?

## Flow

1. Ask one question, grounded in existing context or the optional prompt. If there's no context,
   open with "What would you like to work on?"
2. Wait for the answer. Use it to shape the next question.
3. Repeat until the user says to stop or you have no more questions.
4. When they stop, produce a plan summarizing their answers.
