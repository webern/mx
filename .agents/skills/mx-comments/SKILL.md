---
name: mx-comments
description: >
  You must read and use this skill when writing comments in the code and when documenting this
  library.
argument-hint: "<prompt>"
disable-model-invocation: false
user-invocable: true
---
# /mx-comments

Code comments are meant to help a developer or coding agent who is unfamiliar with this codebase
understand how it works.

Audience:
- A musician, a composer, who happens to also like writing code.
- A software developer who happens to also like writing music.

Things to remember:
- Do not leak particulars of a user session, or your LLM context window, into code comments or
  repository writing. Code comments are not an extension of the conversation you had with the user.
- Code comments should not churn every time the code changes. Do not list each item that a function
  touches in its documentation such that you have to add to the list every time you edit the
  function's behavior. Write more generally so that the code comment can outlive future tweaks to
  the function.

Avoid implementation details in the headers of `src/mx/api`. Those are particularly musician-facing
code comments. The more maintainer facing comments go in the deeper layers of the system.

## Please

Use plain grammar and simple sentence structure. Use common vocabulary that a human composer or
software engineer would use in conversation. When the first half of a sentence is enough, don't add
a continuation of the sentence that says the same thing in counterpoint.

Pretend you are humble engineer explaining how something works, or why it is the way that it is,
without trying to look clever. Go out of your way to write with humility and to be easily
understood. You are not here to show how smart you are. Do not increase the cognitive load of those
trying to read your work.

## Banned Words and Structures

- "knob": you invented this and you overuse it. stop.
- Don't say "writes no attribute". That's weird non-human style. Instead say "does not write an
  attribute".
- "load bearing": it's weird, you invented it, and you overuse it.
