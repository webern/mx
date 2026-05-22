# gen Log

Append chronologically, oldest on top.

## M1: revgen (2026-05-18 — 2026-05-21, 40 iterations)

Reverse-engineered the codegen. Iteratively shrank `SKIP_ELEMENTS` and `CHOICE_SKIP` to empty.
Closed 2026-05-21 with the generator producing every C++ class in `mx/core`. Tests failing — the
commit `d4f25ee6` "src: issues caused by revgen" hand-edits non-generated consumers to keep
the build working; those hand-edits were the input to M2.

## M2: fix-gen (2026-05-21 — 2026-05-22)

Triaged `d4f25ee6` into 6 root-cause issues (A–F), then triaged 62 (later 129 after a clean
build) `make test-all` failures into clusters. Fixed iteratively, ending at 0 failures.

### Triage of `d4f25ee6` hand-edits

- **A — UpDownNone collapsed to UpDown** (WEIRD / Low): Hand-applied MusicXML 4.0 backport
  in original `mx/core` overwritten by schema-faithful 3.x regen. Deferred to M5; TODO
  comments left in `mx/impl`.
- **B — `hasLong` escaped to `hasLong_`** (BUG / Low): Keyword-escape over-applied to the
  has-prefix flag. Fix: `has_flag_name` strips trailing underscore added by `camel()` for
  keywords.
- **C — MetronomeTuplet group flattened** (BUG / High): Regen flattened
  `TimeModificationNormalTypeNormalDot` into `MetronomeTuplet`, making `<normal-type>`
  unconditional. Fix: added `EXTENSION_OPTIONAL_GROUP_RENAME` (per-extending-type override) +
  `SUPPRESS_GROUP_SUFFIX` + `WRAPPER_AS_ELEMENT_SYNTH_GROUPS` so the wrapper survives as a
  sub-element rather than being inlined.
- **D — `setPerMinuteOtBeatUnitChoice` typo** (BENIGN): Historical typo in original; accepted
  small test edit, no generator change.
- **E/F — Missing `importGroup` overloads for traditional-/non-traditional-key** (BUG /
  Medium): Fix path option 1: emit `Key::importTraditionalKey` / `Key::importNonTraditionalKey`
  as private member functions via new `TREE_ELEMENT_CONFIG["key"] = {"parent_imports_choice_groups": True}`.
  Reverted hand-edits in `FromXElement.{h,cpp}`.

### Failing-test clusters

Original triage scheme R1–R7 was rewritten mid-M2 as the situation evolved. Final shape:

- **R1 — direction-family parents emit `MX_FROM_XELEMENT_UNUSED`** (19 tests): split into
  R1a (ArrowGroup needs real `fromXElementImpl` body — added "arrow" to
  `GROUPS_WITH_REAL_FROM_X_ELEMENT`) and R1b (Direction parent bespoke — added
  `_emit_direction_family` handler driven by `model.complex_types["direction-type"].content_tree.branches`).
- **R5 — required-set seeding** (10 tests including HarpPedals SIGSEGV): added structural
  rule in `generate_element_cpp` for `min_occurs>=1, max_occurs!=1, not is_group` children:
  ctor pre-seeds default, `hasContents()` returns true, `remove*` gates on `size>1`,
  `clear*Set` re-seeds.
- **R3 — group-class `streamContents` inter-child `endl`** (~10 tests): rewrote
  `generate_group_cpp` separator logic — required-after-required: unconditional endl before;
  required-after-optional: optional's block emits; index-0 optional: endl after.
- **R2 — choice-class spurious leading `endl`** (~7 tests): gated on
  `config.get("skip_parent")` in `generate_choice_class_cpp` so only `direction-type`-style
  wrapper choices keep the leading endl.
- **R4 — attribute default initializers** (bulk fix, ~22 tests): seeded 17 entries in
  `ATTR_DEFAULT_OVERRIDE` (lang="it" defaults, lineEnd=down, number="1", etc.). Continued
  in i5 with `CHILD_INIT_VALUE_OVERRIDE` for `Scaling`/`StaffDetails`.
- **hasContents direction cluster** (6 tests): Rest/Unpitched gated optional groups on
  `myHas`; MeasureLayout/NoteheadText/Slash/BeatRepeat needed
  `ELEMENT_HAS_CONTENTS_ALWAYS_TRUE` / `CHILD_MIN_OCCURS_OVERRIDE` / notehead-text
  `seed_choice_set`.
- **group/tree-group isFirst separator** (i4/i6, ~30 tests): `generate_group_cpp` and
  `generate_tree_group_cpp` were emitting unconditional `endl` before each set-based child
  even when all preceding optional singletons were absent. Fixed with `isFirst` flag
  pattern.
- **MeasureLayout isOneLineOnly**: explicit child-presence check instead of `hasContents()`
  for elements with `ELEMENT_HAS_CONTENTS_ALWAYS_TRUE`.

### Final D-cluster (Metronome/Tempo round-trip)

After mass cleanup, 13 failures remained, all metronome/tempo. Re-triaged as D1–D4:

- **D3 — `MetronomeAttributes` missing `halign`/`justify` defaults** (`LeftCenterRight::center`):
  trivial `ATTR_DEFAULT_OVERRIDE` entries.
- **D1 — choice classes called `toStream` for empty-`streamName` containers** (BeatUnitPer,
  NoteRelationNote produced literal `<>...</>`): added `is_container` field to
  `TreeChoiceBranch`; emit `streamContents` instead of `toStream` for container branches in
  `generate_tree_choice_cpp`.
- **D2 — `DirectionWriter.cpp:370` throws on non-BPM tempos**: bisect confirmed pre-revgen
  `a0500803` did pass these tests, so revgen introduced the regression in the reader/writer
  path. Subsumed by D4.
- **D4 — Metronome container-branch `fromXElementImpl` missing**: `generate_tree_parent_cpp`
  emitted no dispatch for `is_container=True` branches. Synthetic containers have no XML
  tag, so the reader silently skipped all metronome children. Fix: added `trigger_names`
  field to `TreeChoiceBranch` (computed from first child element name); emit
  `importContainer<X>` declarations and bodies in the parent; dispatch in
  `fromXElementImpl` keyed on trigger names. Generated `Metronome::fromXElementImpl` matches
  HEAD exactly. 9 → 0 failed. **M2 complete.**

### Lessons / invariants captured

- `git checkout -- src/private/mx/core/` preserves mtimes; incremental cmake then links
  partly-old `.o`s, giving stale test counts. Use `make clean && make test-all` for any
  authoritative measurement.
- `make test-all` must run with generated files present (Issue A's `UpDownNone`
  hand-backport in HEAD is incompatible with the schema-faithful regen, so a reset-first
  build fails to compile).
- When removing a previously-emitted byte from a shared template, survey the whole HEAD
  population that template emits, not just one or two representative files (R2's
  DirectionType regression).
- Bespoke handlers should still read the parsed XSD model — pattern is "custom algorithm,
  schema-driven data" (Direction's element-name tables derived from
  `model.complex_types["direction-type"].content_tree.branches`).

## M3: fix-core-dev (2026-05-22 — )

### 2026-05-22 16:40 — iteration 1

Baselines on a clean build: `make test-core-dev` = 31 failed / 361 cases;
`make test` = 0 failed. Smallest diffs in `data/testOutput/corert` (4 lines each):
`foundsuite_Invention 2.xml`, `foundsuite_Invention_5.xml`, `musuite_testInvalid.xml`.

Picked Invention 2 (Invention 5 is the same root cause). Diff was
`<measure width="564.40">` vs `<measure width="564.4">`. `width` is a `tenths`
(decimal) attribute. `PreciseDecimal::toStream` in the hand-written
`src/private/mx/core/Decimals.cpp` strips trailing zeros from the decimal portion,
so `mx::core` writes `564.4`. The corert and api-import pipelines already strip
trailing zeros from the expected XML for 9 other tenths-typed fields
(`default-x`, `default-y`, `tenths`, ...) via `mxtest::stripZerosFromDecimalFields`
+ the `decimalFields` set in `src/private/mxtest/import/DecimalFields.h`. The set
was missing `"width"`.

User direction: fix on the test side, normalize the expected XML the same way the
library normalizes its output. Not a generator bug; nothing to regenerate.

Fix: added `"width"` to `decimalFields`. Result: `make test-core-dev` 31 → 14
failed; `make test` still 0. No new failures introduced. Cleared 17 failures with
a one-line change, all driven by the same width-attribute pattern across the
MuseScore/foundsuite corpus.

Committed as `639d46a3` on branch `fix-core-dev`.

## 2026-05-22 16:47

M3 iteration 2. Baselines this session: test-core-dev = 14 failed, test = 0
failed. Picked smallest diff: musuite/testInvalid.xml (4 lines; two non-XSD
elements <invalid_element/> and <another_invalid_element/> dropped on
fromXDoc). Not a generator or library bug: the file is intentionally invalid
MusicXML, named testInvalid.xml, and self-documents that fact in a
miscellaneous-field. A strongly-typed schema-generated DOM cannot preserve
unknown elements without an architectural passthrough we do not want.

Per user direction introduced a repo-wide marker convention: a sibling file
named {file}.invalid next to any invalid MusicXML input, body is a
human-readable explanation. Documented in data/README.md. Updated
src/private/mxtest/corert/CoreRoundtripImpl.cpp::discoverInputFiles to skip
any file with a sibling .invalid marker. Added
data/musuite/testInvalid.xml.invalid. Other suites (api import) keep
processing such files; only the schema-strict core roundtrip honors the
marker.

## 2026-05-22 16:59

Static analysis sweep of all 13 corert failures against the MusicXML XSD
using xmllint --schema (with xml.xsd and xlink.xsd downloaded to /tmp and
the schema imports rewritten to local paths). For each failing file
compared the schema violations against the corert diff symptom; marked a
file only when (a) the schema flagged a clear violation and (b) the
violation explains the round-trip diff.

Ten files met both criteria and got .invalid markers:
- foundsuite/Deutscher Tanz D.820.1.xml (midi-program=0)
- foundsuite/O_Holy_Night-Adam-1871.xml (midi-channel=0)
- foundsuite/O_Holy_Night.xml (midi-channel=0)
- foundsuite/Rimsky-Korsakov Op11 No4.xml (sound/@dynamics=-1.11)
- lysuite/ly01e_Pitches_ParenthesizedAccidentals.xml (accidental='double-flat')
- lysuite/ly32a_Notations.xml (empty <fret/>)
- lysuite/ly41g_PartNoId.xml (part missing required id)
- lysuite/ly74a_FiguredBass.xml (figured-bass missing required figure)
- lysuite/ly75a_AccordionRegistrations.xml (accordion-middle out of range)
- musuite/test_harmony.xml (kind enum violations, degree-type misorder)

Three failures remained because the files are schema-valid and the diff
is a real library bug: ly22b_Staff_Notestyles.xml (child count mismatch
in measure-style), ly41e_StaffGroups_InstrumentNames_Linebroken.xml
(part-name text mismatch), ly45f_Repeats_InvalidEndings.xml (the schema
allows both "1, 2, 3" and "1,2,3" via the ending-number pattern; mx::core
is collapsing the spaces, which is a library bug despite the misleading
"Invalid" in the filename).

Result: test-core-dev 13 -> 3 failed. test stayed at 0.

## 2026-05-22 17:04

M3 iteration 3 baseline: cleared data/testOutput, ran make test-core-dev
(3 failed) and make test (0 failed) — matches state.md. Diffed each
corert pair:

- ly45f_Repeats_InvalidEndings.xml: 8 lines
- ly41e_StaffGroups_InstrumentNames_Linebroken.xml: 10 lines
- ly22b_Staff_Notestyles.xml: 24 lines

test_to_fix = ly45f_Repeats_InvalidEndings.xml. Diff is two ending
@number attributes: expected "1, 2, 3", actual "1,2,3".

Root cause: CommaSeparatedListOfPositiveIntegers in
src/private/mx/core/CommaSeparatedPositiveIntegers.cpp. parse() runs
onlyAllow(text, "", "1234567890,-") which strips spaces, then never
touches myIsSpacingDesired (default false). toStream emits
"a,b,c" unless myIsSpacingDesired is true. So whether the input had
spaces is lost on import and the output is always space-less.

The schema's ending-number pattern [1-9][0-9]*(, ?[1-9][0-9]*)* allows
both "1, 2, 3" and "1,2,3" — they are distinct lexical forms of the same
value. Round-tripping should preserve which form the input used.

This type is hand-written, not generated (gen/generate.py only maps the
header path as a dependency; the .h/.cpp are not produced). Fix lives
directly in CommaSeparatedPositiveIntegers.cpp::parse: detect ", " in
the raw input and call setUseSpaceBetweenItems(true). Not a gen change,
not a schema-faithful-defaults question — just a parser losing
information.

Reporting analysis to user, awaiting direction before fixing.

## 2026-05-22 17:14

Applied fix to CommaSeparatedListOfPositiveIntegers::parse: detect
digit-comma-space-digit in the raw input and set myIsSpacingDesired
accordingly. Added #include <cctype>. Detection is intentionally narrow
(requires digits on both sides of ", ") so the existing
StringsTest::EndingNumber02 case — which feeds junk like
"-2,-1,,,,XYZ, 0,  @#$@*&#^1,2,3,3,3,3,3" and expects "1,2,3" out —
stays green.

Gates: make fmt clean, make check clean, make test-all 3026/3028
passed (the two remaining failures are the known ly22b and ly41e
corert cases; no new failures, no regressions). Committed as
461b96d2 on branch fix-core-dev.

Iteration 3 done. M3 remaining: ly22b (24-line diff, measure-style
child count) and ly41e (10-line diff, part-name linebroken text). Next
session picks the smaller — ly41e.

## 2026-05-22 17:22

M3 iteration 4. Baseline confirmed: test-core-dev=2, test=0.
Smallest core-roundtrip diff was lysuite/ly41e_StaffGroups_InstrumentNames_Linebroken.xml
(10 lines) as predicted. Root cause: input has &#xd; in <part-name> text;
mx::core::XsString::toStream escapes only <, >, &. On write, raw \r is emitted,
then pugixml's parse_eol normalizes \r to \n on the next load, so the in-memory
"Long\rStaff\rName" becomes "Long\nStaff\nName" after toXDoc. Fix: add a
'\r' case to XsString::toStream emitting "&#xd;". XsString.cpp is hand-written
(only its header path is mapped in gen/), so no regen. Risk minimal — escape only
fires on the output side and only when a \r is actually present in the string.

## 2026-05-22 17:26

Iteration 4 fix landed in commit 040b2152 ("core: escape carriage return as
&#xd; in XsString writer"). Verified: make fmt clean, make check clean,
make test-core-dev 1 failed (was 2), make test 0 failed, make test-all
3027/3028 (was 3026/3028). Only remaining failure is
lysuite/ly22b_Staff_Notestyles.xml — child-count mismatch inside
measure-style/slash. That is the iteration 5 target.

## 2026-05-22 17:38

M3 iteration 5 started. Baseline: make test-core-dev = 1 failed (lysuite/ly22b_Staff_Notestyles.xml, child count mismatch in measure-style/slash). make test was 0 at start of M3 iter 4 commit; not re-run this session.

Diagnosed root cause: writer unconditionally emits `<slash-type>eighth</slash-type>` inside every `<slash>` element. XSD has `<xs:group ref="slash" minOccurs="0"/>` inside `complexType slash` (and inside `complexType beat-repeat`), so the entire group is optional and the test input has empty `<slash type="..."/>` with no children. HEAD treats `slash-type` as always-present, no `myHas` flag. Generator preserves this via explicit CHILD_MIN_OCCURS_OVERRIDE entries at gen/generate.py:1083-1084 keyed on (slash, slash-type) and (beat-repeat, slash-type).

Classified as pre-existing hand-rolled bug replicated by revgen — exactly the "defaulting to having an element present when minOccurs=0" category flagged in plan.md M3.

mx/api and mx/impl do not reference Slash, BeatRepeat, or SlashType element classes; only unrelated enums (doubleSlashFlat etc.) carry the substring. No impl/api changes needed.

Confirmed with user: fix both slash and beat-repeat together (single conceptual change), regenerate data/expected/lysuite_ly22b_Staff_Notestyles.xml.expected.xml as the standard api-import fix path.

Plan: drop both override entries, regen mx/core, regen the api-import expected file, run full gate.

## 2026-05-22 18:00

Discovered the actual "buggy assertion" the user anticipated: src/private/mxtest/import/ExpectedFiles.cpp:38-40 patched the test input via `addChildIfNone(*xdoc, "slash", "slash-type", "eighth")` with a comment claiming the input was XSD-invalid. The XSD analysis shows the opposite — the slash group is minOccurs=0, so empty `<slash/>` is fine. Removed the patch (test harness change, pre-authorized in this session). The mx/api / mx/impl layer is untouched; no assertion there to fix.

Generator change: removed both CHILD_MIN_OCCURS_OVERRIDE entries (slash/slash-type and beat-repeat/slash-type) plus their explanatory comment. Regenerated mx/core. Only Slash.{h,cpp} and BeatRepeat.{h,cpp} had semantic diffs (added myHasSlashType + gated emission); other files were pure clang-format whitespace drift resolved by `make fmt`.

Local gates so far: make test-core-dev 350/350 (was 349/350); make test 2717/2717.

## 2026-05-22 18:17

`make test-all` after the gen fix surfaced 25 assertion failures in 23 cases across 12 mxtest/core/*Test.cpp files. None in api-import or core-roundtrip — only the per-element fixture tests that only build under MX_BUILD_CORE_TESTS=ON and that codified the buggy "slash-type emitted by default" behavior.

Two categories:
- Category A (SlashTest, BeatRepeatTest): default-constructed object asserts buggy stream output.
- Category B (MeasureStyleTest, PropertiesTest + 9 files cascading via the Properties tgen fixture): setValue without setHasSlashType, leaving the new has-flag false so the writer correctly omits the element, but expected streams still assert it.

User authorized fixing both. Approach: minimal surgical addition of setHasSlashType(true) calls; no new assertions, no removed assertions. Six edits total across four files (SlashTest x2, BeatRepeatTest x2, MeasureStyleTest x2, PropertiesTest x2). The PropertiesTest builder is shared, so its two-line edit cascades to fix 11 of the 12 broken files.

Final gates: make fmt clean, make check passed, make test-all passed (3028/3028, 9914 assertions). M3 milestone complete: lysuite/ly22b_Staff_Notestyles.xml was the last core-roundtrip failure.
