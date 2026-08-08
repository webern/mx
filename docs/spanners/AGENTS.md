# Spanners

In this directory we are working on a design, and project, for addressing issue #393, which grew out
of discussion #349, which asks that a spanner be specifiable as a single object instead of a
start-stop pair.

`./the-fail-fast-problem.md` is SUPERSEDED early thinking; its conclusions are rejected. Do not
read it or let it influence the design.

The design document is here `./spanner-design.md` and is governed by the following AI rules (which
are also specified in its frontmatter): the document is in two sections, one for human authorship
(the first part of the document) and one for AI authorship (the latter part of the document).
AI agents MUST:
- ONLY edit the human section when asked
- ONLY edit the human section surgically for error, spelling, and specific user requests
- not wholly overwrite or substantially edit the human section
- comply with the documents frontmatter instructions
