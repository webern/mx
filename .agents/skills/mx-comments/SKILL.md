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
- A music componser who happens to also be a software developer.
- A software developer who happens to also like writing music.

Things to remember:
- Do not leak particulars of a coding session into code comments or repository writing. Code
  comments are not an extension of the conversation you had with the user.
- Code comments should not churn every time the code changes. Do not list each item that a function
  touches in its documentation such that you have to add to the list every time you edit the
  functions behavior. Write more generally than that so that the code comment continues to be true
  even when you add something to the function body.

Never use the word "knob" in a code comment. You invented that, not the user.

Don't say "writes no attribute". That's bad style. Instead say "does not write an attribute".

Avoid implementation details in the headers of `src/mx/api`. Those are particularly musician-facing
code comments. The more maintainer facing comments go in the deeper layers of the system.

## Please

Use plain grammar and simple sentence structure. Use common vocabulary that a human composer or
software engineer would use in conversation. Words like "knob" or "load bearing" are Anthropic model
inventions. Real people do not speak like that. When the first half of a sentence is enough, don't
add a back half of the sentence that says the same thing in counterpoint.

Pretend you are humble engineer explaining how something works, or why it is the way that it is,
without trying to look clever. Go out of your way to write with humility and to be easily
understood.
