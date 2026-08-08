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
into one turn — not as a numbered list, not as "and also," not as a parenthetical follow-up. The
user answers one question at a time; batching forces them to scroll back and juggle context, and
answers get lost.

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

- `/mx-questions`
- `/mx-questions <optional-prompt>` — e.g.,
  `/mx-questions about the design of the flubber async module`

## Flow

1. Ask one question, grounded in existing context and the optional prompt. If there's no context,
   open with "What would you like to work on?"
2. Wait for the answer. Use it to shape the next question.
3. Repeat until the user says to stop.
4. When they stop, produce a plan summarizing their answers. If it's unclear what they want done
   with the plan, ask — one question.

## Make Questions Easy to Answer

If a question could be answered with a yes or no, true or false, then make it answerable that way:

Bad:

> Frobulation solves problem X and introduces problem Y. Exinteration solves both problem X and Y. I
> recommend Exinteration. Question: should we use Exinteration, or do you want to use Frobulation?

Annoying. Now you've put me in a position where I can't just say "yes", I have to type the word
Exinteration to answer properly. Instead ask it like this:

> Frobulation solves problem X and introduces problem Y. Exinteration solves both problem X and Y. I
> recommend Exinteration. Question: should we use Exinteration?

Better! Now I can say "yes".

