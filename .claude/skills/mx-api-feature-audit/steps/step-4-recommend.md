# Step 4: Prioritized recommendations

Compare `data/api.features.xml` (view 3) against `data/corpus.xml` (views 1 and 2) and write a
recommendation report. Save it to `docs/ai/api-feature-audit.md` (overwrite the prior run) and also
summarize the headline findings to the user.

## Priority order

Recommend in this exact order -- it reflects severity, not convenience:

1. BUG FIXES: enums missing members. Every `status="missing-members"` enum from Step 2. These are
   silent data loss, the worst failure mode. Rank within this group by whether a wild file triggers
   the missing value (urgent) over synthetic-only (still a real bug). For each: name the core enum,
   the missing member(s), the api enum, the translation site (`file:line`), and the fallback the
   value currently lands on.

2. HIGH-USE GAPS: features heavily used in the wild that `mx::api` does not support. Take
   `support="none"` (or `partial` with an unsupported, common attribute) features and rank by
   `wild-files` from `data/corpus.xml`. A `support="none"` element with a high `wild-files` count is
   the strongest feature case.

3. ROUND-TRIP CLOSERS: small gaps that would make a wild corpus file fully round-trippable. Look for
   a wild file whose only unsupported features are one or two small items -- closing them flips the
   whole file to round-trippable. Use the per-file `*.features.xml` sidecars intersected against
   `data/api.features.xml` to spot these. These are high value-per-effort.

## What each recommendation must contain

- The MusicXML feature (element/attribute/enum member) and the concrete evidence: `wild-files` count
  and one or two example files from `data/corpus.xml`.
- The exact code locus to change (api enum/type header, the `mx::impl` Converter table or
  reader/writer). For a bug, that is usually a one-line addition to an enum plus a Converter table
  row; say so.
- Severity (bug vs gap) and rough effort.

## Hand off to implementation

Do not implement here. The `add-mx-api-feature` skill is the implementation path -- enum bug fixes
and small attribute gaps both fit its `mx::api` + `mx::impl` two-sided flow (core almost always
already has the value). Point the reader to it and, if asked, kick off the top-priority item there.

## Keep it honest

- Distinguish "confirmed" from "suspected" findings -- if Step 3 left an attribute's round-trip
  status uncertain, label it suspected.
- Do not pad the gap list with synthetic-only obscurities; the api is a subset on purpose. The value
  of this report is the short list of things that actually matter: real bugs and common-in-the-wild
  gaps.
