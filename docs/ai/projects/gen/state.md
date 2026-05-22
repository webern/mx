# gen State

## Milestone

**M3: fix-core-dev** in progress. After iteration 4:
`make test-core-dev` = 1 failed (down from 2); `make test` = 0 failed;
`make test-all` = 1 failed (3027/3028 cases).

## What the previous session did (M3 iteration 4)

Iteration 4 picked the smallest core-roundtrip diff,
`lysuite/ly41e_StaffGroups_InstrumentNames_Linebroken.xml` (10-line
diff). The `<part-name>` text content contained `&#xd;` character
references (literal CR bytes); on round-trip those became `\n`.

Root cause: `mx::core::XsString::toStream` in
`src/private/mx/core/XsString.cpp` escaped only `<`, `>`, `&`. A raw
`\r` written to the output stream is normalized to `\n` by the next
parser pass (pugixml `parse_eol`, per XML 1.0 §2.11). The reader path
was already correct — pugixml expands `&#xd;` into a raw `\r` after
EOL normalization runs, so `myValue` held the right bytes; only the
writer dropped them.

Fix: added a `'\r'` case emitting `"&#xd;"` to the escape switch.
Hand-written type, no regen needed. Committed as `040b2152`.

## One remaining failure

`lysuite/ly22b_Staff_Notestyles.xml` (24-line diff): child count
mismatch inside `measure-style` at
`/score-partwise/part[2]/measure[0]/attributes[2]/measure-style[0]/slash[0]`.
This is the last `make test-core-dev` failure and the last
`make test-all` failure. **Next session target.**

## What the next session should do (M3 iteration 5)

Per `plan.md` M3 session sequence:

1. `rm -rf data/testOutput/*`
2. `make test-core-dev` — record failure count as baseline_core_dev
   (expect 1).
3. `make test` — expect 0.
4. Diff `lysuite_ly22b_Staff_Notestyles.xml.{expected,actual}.xml` in
   `data/testOutput/corert` — it's the only failure. That is
   `test_to_fix`. Report to user with one-paragraph analysis. One test
   per session.
5. Wait for user direction before fixing. This one has a child-count
   mismatch inside `measure-style/slash`, which suggests it could be
   either: a missing optional child the writer is emitting, or an
   optional child the reader is not emitting; check both the XSD shape
   of `measure-style` (sequence vs choice, minOccurs) and the
   generated `MeasureStyle.cpp` / `Slash.cpp` against the input file.
6. After fix: if the fix was in `gen/`, regen `mx/core`; otherwise
   skip regen. Then `make fmt`, `make check`, `make test-core-dev`,
   `make test`, `make test-all` (since the fix will almost certainly
   touch `src/private/mx/core/`).

## Gotchas

- **`.invalid` marker convention**: if a corert failure is caused by
  intentionally invalid MusicXML, the right move is a `{file}.invalid`
  marker, not code. ly22b looks like a real bug (child-count
  mismatch, not a structural validity issue), but confirm by running
  `xmllint --noout --schema /tmp/mx.xsd
  data/lysuite/ly22b_Staff_Notestyles.xml` first.
- **Validating against the MusicXML XSD with xmllint**: the XSD
  imports `http://www.musicxml.org/xsd/xml.xsd` and `xlink.xsd` which
  404 today. Workaround:
  - `curl -sSf -o /tmp/xml.xsd https://www.w3.org/2001/xml.xsd`
  - `curl -sSf -o /tmp/xlink.xsd https://www.w3.org/1999/xlink.xsd`
  - copy `docs/musicxml.xsd` to `/tmp/mx.xsd` and `sed` the two
    `schemaLocation` values to `/tmp/xml.xsd` and `/tmp/xlink.xsd`.
  - then `xmllint --noout --schema /tmp/mx.xsd <file>`.
- **One test per session.** Do not try to fix multiple failures in
  one session even if they share a root cause.
- **Hand-written vs generated.** Files directly under
  `src/private/mx/core/` (e.g. `XsString.cpp`,
  `CommaSeparatedPositiveIntegers.cpp`) are mostly hand-written; files
  under `src/private/mx/core/elements/` are codegen output.
  `gen/generate.py` only references the hand-written ones by header
  path. Bugs in hand-written types are fixed in-place, no regen.
- **Don't touch tests carelessly.** "Never change tests" applies to
  test cases. Test infrastructure (normalization, harness, discovery)
  is fair game when authorized; default to flagging before changing.
- **`make test-all` is the M2 gate; `make test-core-dev` is the M3
  daily driver.** Anything under `src/private/mx/core/` requires
  `test-all` per AGENTS.md.
- **HEAD has a hand-applied UpDownNone backport** in
  `ArpeggiateAttributes.h` that conflicts with a schema-faithful
  regen. Invisible as long as M3 changes don't require regen.
- **`make fmt` runs in Docker** and may time out on first pull. Retry.
