# Codegen Experiment Notes

## Iteration 1 (2026-05-18)

Generator: `gen/experiment/generate.py`
Coverage: 373 of 383 elements (10 choice-type skipped), 137 attributes structs
Diff: 764 files changed, +4119 -8149

### Exact matches
- 297/631 .h files exact
- 200/629 .cpp files exact

### Diff categories found

**1. Constructor initializer list formatting (RULE)**
The generator puts ctor init lists on one line. The real code wraps them at ~100-110 chars with
continuation indented to align with the first initializer. After `make fmt` this should resolve - need
to verify.

**2. Method ordering inconsistency in original code (ANOMALY)**
Two patterns exist for virtual method declarations:
- Pattern A (373 files): `hasAttributes, hasContents, streamAttributes, streamName, streamContents`
- Pattern B (67 files): `hasAttributes, streamAttributes, streamName, hasContents, streamContents`
Generator uses Pattern A (majority). The 67 Pattern B files produce diffs. Normalize to Pattern A.

**3. Forward declares: sequence order vs alphabetical (RULE gap)**
The checked-in code sorts forward declares alphabetically in some files and by sequence order in
others. The generator uses XSD sequence order. Need to investigate which is canonical and pick one.
Distance/Appearance example: real code has alphabetical, generator has sequence order.

**4. Group wrapper classes missing (RULE gap - major)**
The real code creates wrapper classes for XSD `xs:group` definitions with a `Group` suffix:
- `EditorialVoiceGroup`, `EditorialVoiceDirectionGroup`, `FullNoteGroup`, etc.
The generator doesn't create these wrapper classes. It uses the group name directly without the suffix.
This causes missing forward declares, wrong type names, and missing accessor methods.
~20+ elements reference group wrappers.

**5. Choice wrapper classes missing (not yet generated)**
10 choice-type elements are skipped entirely: `time`, `measure-style`, `direction-type`, `dynamics`,
`metronome`, `percussion`, `encoding`, `technical`, `articulations`, `arrow`.
These need `*Choice` class generation.

**6. XsString/XsToken value parsing: setValue vs parse (RULE)**
String-like types (`XsString`, `XsToken`, `XsID`, `XsIDREF`, `XsNMToken`, `XsAnyUri`) use
`myValue.setValue(xelement.getValue())` in `fromXElementImpl`.
Numeric/enum types use `myValue.parse(xelement.getValue())`.
Generator currently uses `parse` for everything.

**7. setAttributes parameter name (cosmetic)**
Real code uses `void setAttributes(const XPtr &value)`.
Generator uses `void setAttributes(const XPtr &attributes)`.

**8. Color attribute FIX diffs (expected, correct)**
45 attributes structs gain a `color` field they were missing. This is a known FIX per
gen-overrides.md. The generator correctly emits these. They produce +additions in .h and .cpp.

**9. Private helper methods in complex elements (EXC)**
Some complex elements have hand-written private helper methods for specialized parsing:
- `Direction`: `importDirectionTypeSet`, `createDirectionType`, `isDirectionType`,
  `isMultiDirectionType`
- `Note`: similar specialized parsing helpers
These are EXC per gen-overrides.md - irreducible per-element code.

**10. ezxml forward declarations (RULE)**
Some real elements forward-declare `ezxml::XElementIterator`. The generator doesn't emit these.
They correlate with the private helper methods (pattern 9).

**11. Beam maxOccurs discrepancy (investigation needed)**
The XSD says `beam` in `note` has `maxOccurs="8"`, but my parser reads `maxOccurs` as an int.
The real code uses `BeamSet` (unbounded). Need to handle bounded-but-gt-1 as Set.

**12. `empty` complexType handling**
`empty` is a named complexType in the XSD with no content. Elements using this type (like
`accordion-high`) should have no attributes, no content - just the element shell.

**13. Attributes struct naming: type name vs element name**
The generator names attributes structs after the complexType (`FormattedTextAttributes`,
`BarStyleColorAttributes`). The real code names them after the element that uses them. When multiple
elements share a complexType, the real code uses one shared struct but it's named after the type. When
an element has an anonymous type, the struct is named after the element. The generator creates spurious
files for type-named structs that don't exist in the real code. Need to align naming.

Major spurious files: `FormattedTextAttributes`, `BarStyleColorAttributes`,
`EmptyPrintStyleAlignAttributes`, `EmptyPrintObjectStyleAlignAttributes`, `HammerOnPullOffAttributes`,
`HeelToeAttributes`, `HorizontalTurnAttributes`, `NameDisplayAttributes`, `NoteTypeAttributes`,
`PlacementTextAttributes`, `StyleTextAttributes`, `TextElementDataAttributes`,
`TextFontColorAttributes`, `TypedTextAttributes`.

**14. Dynamics child elements generated as separate files (should not exist)**
The `dynamics` XSD type is a choice of empty elements (`f`, `ff`, `fff`, etc.). The real code
handles these as inner members of the `Dynamics` class, not as separate element files. Generator
created 24 files (F.h/cpp, Ff.h/cpp, etc.) that should not exist.

### Priority fixes for next iteration
1. ~~Fix `setValue` vs `parse` for string types~~ DONE
2. Add group wrapper class generation
3. ~~Fix `setAttributes` parameter name~~ DONE (.h=`attributes`, .cpp=`value` matches real code)
4. ~~Handle bounded maxOccurs > 1 as Set~~ DONE
5. Fix forward declare ordering (alphabetical)
6. ~~Don't generate separate files for dynamics child marks~~ DONE
7. ~~Fix attributes struct naming~~ DONE (element-named, except empty-* shared types)
8. Start generating choice classes

## Iteration 2 (2026-05-18)

Fixes applied: setValue/parse, setAttributes param, maxOccurs>1 as Set, dynamics suppression,
attrs naming (element-named except shared empty-* types), skip `attributes` element (EXC: Properties).

Diff: 728 files changed, +4069 -8410
- .h: 233/592 exact (up from 175 mid-iteration, down from 297 iter-1 due to attrs changes)
- .cpp: 222/590 exact

### New findings

**15. Include ordering**
Real code orders includes: `ElementInterface.h`, type headers (`Enums.h`, `Integers.h`, etc.),
`ForwardDeclare.h`, then attrs header. Generator sorts alphabetically, producing different order.

**16. Private member ordering (value before attrs)**
For text-with-attrs elements, the real code declares `myValue` before `myAttributes` in private
members. Generator puts `myAttributes` first.

**17. Empty second `private:` block**
Generator adds a second `private:` section with no members for empty elements. Real code doesn't
have this.

**18. Attributes struct naming: element-named is the rule**
Discovered that 33 element-named attrs structs in the real code are UNREFERENCED dead code. Elements
always use the type-named struct when sharing a type. But most types with >1 user use `empty-*`
naming. Non-empty shared types (like `formatted-text`) generate BOTH a type-named and an
element-named attrs file in the original code, but only the type-named one is referenced.

Actually, deeper investigation showed the original code uses element-named attrs for all non-empty-*
types. `DisplayText.h` uses `DisplayTextAttributes.h` (element-named), not
`FormattedTextAttributes.h`. The type-named files that exist (`FormattedTextAttributes.h` etc.) are
dead code never committed to the repo.

**19. CORE_ROOT_ATTRS location anomaly**
`EmptyPrintObjectStyleAlignAttributes` lives in `mx/core/` not `mx/core/elements/`. This is the
anomaly noted in gen-overrides.md. `EmptyPrintStyleAlignAttributes` was never committed anywhere.

**20. setAttributes .h vs .cpp parameter name mismatch (intentional)**
Real code: .h uses `&attributes`, .cpp uses `&value`. This is an original inconsistency the
generator should reproduce.

### Priority fixes for iteration 3
1. ~~Fix include ordering~~ DONE (alphabetical sort matches clang-format)
2. ~~Fix private member ordering (value before attrs)~~ DONE
3. ~~Remove empty second private block for empty elements~~ DONE
4. ~~Fix text-with-attrs ctor to init myAttributes~~ DONE
5. Add group wrapper class generation
6. Fix forward declare ordering
7. Start generating choice classes

## Iteration 3 (2026-05-18)

Fixes: include alphabetical sort, private member ordering (value before attrs), removed empty second
private block, text-with-attrs ctor now initializes attrs, core-root-attrs location handling.

Diff: +3619 -8063 (567 changed files, down from 728 in iter 2)
- .h: 386/592 exact (65%)
- .cpp: 229/590 exact (39%)

### Findings

**21. MX_UNUSED(xelement) inconsistency**
Original code always includes `MX_UNUSED(xelement)` for simple-value elements, even for string types
where xelement IS used (via `xelement.getValue()`). This is technically wrong but consistent.
Generator matches this pattern.

### Remaining diff sources (estimated impact)
- Forward declares in wrong order (~100+ h diffs): children in XSD sequence order vs alphabetical
- Missing group/choice wrapper classes (~50+ diffs): no Group/Choice generation yet
- Method order Pattern B in 67 files: `hasAttributes, streamAttributes, streamName, hasContents`
- Attrs struct content diffs (~45 FIX + structural): color field, extra attrs
- Complex element cpp: missing child includes, wrong ctor init order, etc.
- 33 unreferenced dead-code attrs files: generator doesn't create them

### Priority fixes for iteration 4
1. ~~Sort forward declares alphabetically~~ DONE
2. Fix method ordering for Pattern B files (67 files)
3. Generate Group wrapper classes
4. Generate Choice classes

## Iteration 4 (2026-05-18)

Fixes: sorted forward declares alphabetically.

Diff: +3556 -8000 (567 changed files, same as iter 3)
- .h: 386/592 exact (65%, same as iter 3 - forward declares didn't change .h exact count much)
- .cpp: 229/590 exact (39%, same as iter 3)

Note: The forward declare sort fix was small (3 lines changed in generate.py). It likely improved
some files but the exact-match counts stayed roughly the same because the remaining diffs have
multiple overlapping causes per file.

### Diff analysis by .h file change size
- 43 files with 1-2 changed lines (likely single-cause fixes)
- 67 files with 3-4 changed lines
- 29 files with 5-8 changed lines
- 67 files with >8 changed lines (multi-cause or structural)

### Priority fixes for iteration 5
1. Fix method ordering for Pattern B files (67 files with hasAttributes, streamAttributes,
   streamName, hasContents, streamContents order instead of Pattern A)
2. Generate Group wrapper classes (EditorialVoiceGroup, FullNoteGroup, etc.)
3. Generate Choice classes (ArticulationsChoice, NoteChoice, etc.)
4. Fix complex element .cpp child includes and ctor init order

## Iteration 5 (2026-05-18)

Fixes applied:
- Pattern B method ordering: elements with complexContent, xs:sequence, xs:choice, or xs:group
  references use `hasAttributes, streamAttributes, streamName, hasContents, streamContents` order.
  Detection heuristic covers 99.5% of cases. `PATTERN_B_EXCEPTIONS = {"String"}` for the one
  text-with-attrs element that uses Pattern B. Affected ~144 files.
- Group wrapper class generation (.h and .cpp) for 11 XSD groups: beat-unit,
  display-step-octave, editorial, editorial-voice, editorial-voice-direction, full-note,
  harmony-chord, layout, music-data, score-header, time-signature.
- Group children in parent elements: `->streamContents()` call, `->hasContents()` check, Group
  suffix naming for groups in GENERATE_GROUPS.
- ElementInterface() ctor fix: only included when element has no value AND no children.
- MX_RETURN_IS_SUCCESS for text-with-attrs fromXElementImpl (was `return isSuccess`). Matches
  113 of 133 real files; 20 outliers use `return isSuccess` (no systematic pattern found).

Diff: +3083 -7846 (540 changed files, down from 567 in iter 4)
- .h: 402/592 exact (67.9%, up from 65.2%)
- .cpp: 240/590 exact (40.6%, up from 38.8%)

### Findings

**22. Pattern B detection heuristic**
Elements with complexContent, xs:sequence, xs:choice, or xs:group in their complexType use Pattern B
method ordering. Only two exceptions found: `String` (Pattern B despite text-with-attrs) and
`Directive` (Pattern A despite having complexContent, but Directive is an EXC/skipped element).

**23. Group streamContents patterns are per-group hand-coded**
Each of the 11 generated group classes has a unique streamContents implementation:
- 2-child all-optional (EditorialGroup): no firstItem variable, manual inline checks
- 3-child all-optional (EditorialVoiceGroup): firstItem pattern, first child skips check
- Required-only (DisplayStepOctaveGroup, TimeSignatureGroup): direct stream with endl
- Mixed required/optional/set (BeatUnitGroup, LayoutGroup, ScoreHeaderGroup): each unique
- Choice-based (HarmonyChordGroup, FullNoteGroup): switch/special handling
The generator uses a generic template that doesn't match any group exactly. All 11 group .cpp files
have diffs. Group .h files: 6 of 11 match exactly.

**24. MX_RETURN_IS_SUCCESS vs return isSuccess inconsistency**
For text-with-attrs elements, 113 use `MX_RETURN_IS_SUCCESS` and ~20 use `return isSuccess` in
fromXElementImpl. No systematic pattern distinguishes them (both setValue and parse types appear in
each group). Generator uses the majority pattern (MX_RETURN_IS_SUCCESS).

**25. MX_UNUSED(xelement) inconsistency in simple-value elements**
Most simple-value elements include `MX_UNUSED(xelement)` even though xelement IS used via
`getValue()`. A few (CreditType, Divisions, SystemDistance, Volume) do NOT include it. Generator
includes it to match the majority.

**26. Constructor default values**
Some elements have explicit default values in their ctor init list (e.g., `Fifths::myValue{0}`,
`Duration::myValue(1.0)`). The generator uses `myValue()` (default-constructed). These are
type-specific defaults that would require per-type knowledge to reproduce.

### Priority fixes for iteration 6
1. ~~Fix complex element .cpp fromXElementImpl - use correct importElement/importGroup signatures~~ DONE
2. ~~Fix complex element .cpp streamContents - correct patterns for required/optional/set/group~~ DONE
3. ~~Fix complex element .cpp hasContents - return true for elements with required children~~ DONE
4. ~~Fix streamAttributes - use return myAttributes->toStream(os) not null check~~ DONE
5. ~~Inline non-GENERATE_GROUPS groups (duration, all-margins, staff, etc.)~~ DONE
6. ~~Handle complexType-level group refs (page-margins etc.)~~ DONE
7. ~~Sort .cpp child includes alphabetically~~ DONE
8. ~~Wrap ctor init lists at ~100 chars~~ DONE
9. Generate Choice classes (deferred to iteration 7)

## Iteration 6 (2026-05-18)

Fixes applied:
- **Group inlining**: Groups not in GENERATE_GROUPS (duration, all-margins, left-right-margins,
  staff, tuning, voice, footnote, level, non-traditional-key, slash, traditional-key, part-group,
  score-part) have their children inlined into parent elements. Propagates minOccurs from group ref
  to inlined children (e.g. `<xs:group ref="staff" minOccurs="0"/>` makes staff child optional).
- **Top-level group refs in complexTypes**: Types like `page-margins` that use
  `<xs:group ref="all-margins"/>` directly (no xs:sequence wrapper) now correctly parse children.
- **fromXElementImpl**: Changed from wrong `importElement(message, it, endIter, ...)` overloads to
  correct signatures: `importElement(message, *it, isSuccess, *myX, myHasX)` with `continue` for
  optional children, same with `isXFound` for required, `importGroup(...)` for groups,
  `importElementSet(...)` for sets. Added `isDurationFound`-style local bools for required children.
- **streamContents**: Required children stream unconditionally with `->toStream()`. Groups wrapped
  in `hasContents()` check with `->streamContents()`. Elements with required non-group children
  get `isOneLineOnly = false;` at start + unconditional trailing endl. All-optional elements get
  `isOneLineOnly = !hasContents();` at end + conditional endl.
- **hasContents**: Elements with any required non-group child return `true` directly.
- **streamAttributes**: Changed from `if (myAttributes) { ... }` guard to direct
  `return myAttributes->toStream(os);` matching real code.
- **.cpp include sort**: Child element includes in .cpp files now sorted alphabetically.
- **Ctor init list wrapping**: Init lists wrap at ~110 chars (matching clang-format behavior).
- Added `make fmt` step to the workflow (run after generate.py, before eval.py).

Diff: +2813 -6519 (593 changed files, up from 540 in iter 5 due to newly-generated children)
- .h: 403/592 exact (68.1%, slight increase from 67.9%)
- .cpp: 186/590 exact (31.5%, down from 40.6% - more files now generate children but with small
  diffs, reducing average penalty per file)
- Eval penalty: 39,297 (down from 44,389 in iter 5, -11.5%)

### Findings

**27. XSD groups that are inlined vs wrapped**
The real code treats some XSD groups as direct children (inlined) and others as Group wrapper
classes. Groups in GENERATE_GROUPS get wrapper classes; all others are inlined. Key inlined groups:
`duration` (single element), `staff` (single optional element), `all-margins` (recursive inline to
4 margin elements), `tuning` (3 elements), `voice` (single element).

**28. Required-child error messages are inconsistent**
Some elements (Clef) have `if (!isSignFound) { message << ... }` error messages after the loop in
fromXElementImpl, but most (Backup, Forward, etc.) do not. The generator currently omits these
messages to match the majority pattern.

**29. isOneLineOnly placement: END pattern dominates**
Of 14 all-optional elements using `isOneLineOnly = !hasContents()`, 13 place it AFTER child
streaming (END) and use `if (!isOneLineOnly)`. Only AccordionRegistration places it BEFORE
children (START) and uses `if (hasContents())` at the end.

**30. All-optional streamContents wrapping**
Some elements (Defaults, Properties) wrap the entire streamContents body in
`if (hasContents()) { ... } else { isOneLineOnly = true; }`. The generator uses the simpler
flat pattern. This accounts for some STRUCTURAL diffs in those files.

**31. Direction.cpp custom helpers may be replaceable**
Direction.cpp has 680 lines with hand-written private helpers (`importDirectionTypeSet`,
`createDirectionType`, etc.). The generated version is 240 lines using standard patterns. These
may be functionally equivalent - testing needed. Same question applies to Credit.cpp.

**32. Choice handling is the dominant remaining penalty source**
The top 10 penalty files are all related to xs:choice handling: Direction (choice within
sequence), Note (complex choice), Notations, Key, Harmony, Harmonic, Play, etc. The 10 skipped
choice elements plus choice members in complex elements account for ~60% of remaining STRUCTURAL
penalty. Some choice elements have dedicated Choice wrapper classes in real code; others inline the
choice logic directly.

**33. Bounded maxOccurs (e.g. fermata maxOccurs=2)**
Barline's fermata uses maxOccurs=2 (not unbounded). The real code has bounds checking in
addFermata (`size() < 2`) and removeFermata (`size() > 0`). The generator treats all maxOccurs>1
as unbounded sets without bounds.

### Priority fixes for iteration 7
1. Generate Choice classes - this is the single biggest penalty reduction opportunity (~60% of
   remaining STRUCTURAL). Study the real code patterns for ArticulationsChoice, NoteChoice,
   LyricTextChoice, KeyChoice etc.
2. Test whether generated Direction.cpp/Credit.cpp pass `make test` - if so, the custom helpers
   are unnecessary and can be removed from the EXC list
3. Handle bounded maxOccurs (maxOccurs=2 for fermata, maxOccurs=8 for beam)
4. Fix all-optional streamContents wrapping (if hasContents() { ... } else { isOneLineOnly = true })
5. Add blank line before MX_RETURN_IS_SUCCESS in fromXElementImpl loop

## Iteration 7

### Fixes applied
- **Enum value parse fix**: Added `is_enum_value_type()` three-way check in `fromXElementImpl` for
  simple-value elements. Enums now use `myValue = parseXxx(xelement.getValue())` instead of
  `myValue.parse(...)`. Covers both `*Enum` suffix types and types in `needs_parse_func`.
- **Added to needs_parse_func**: BeaterValue, BreathMarkValue, HoleClosedValue, HoleClosedLocation,
  TimeSeparator, PrincipalVoiceSymbol, ModeValue.
- **xs:integer type fix**: Changed mapping from `IntType` to `Integer`, added `Integer` to the
  Integers.h include list.
- **Bespoke type mappings**: Added `distance-type` -> DistanceType, `line-width-type` ->
  LineWidthType, `mode` -> ModeValue.
- **Group unbounded maxOccurs**: Fixed streamContents, hasContents, and fromXElementImpl to handle
  group children with maxOccurs > 1 using Set iteration pattern (only harmony-chord had this).
- **Optional group importGroup**: Added `myHas` flag parameter for groups with minOccurs=0
  (e.g., DisplayStepOctaveGroup in Rest).
- **SKIP_ELEMENTS expanded**: Added part-list, harmony, credit, key, lyric, notations, note,
  ornaments, part-abbreviation-display, part-name-display, score-instrument, score-part,
  time-modification, instrument-sound. All have Choice classes or special patterns the generator
  cannot yet produce.
- **GENERATE_GROUPS reduced**: Removed time-signature (has Interchangeable not in XSD group),
  harmony-chord (has Choice logic), music-data (has MusicDataChoice), full-note (has
  FullNoteTypeChoice).
- **mx/impl fixes** (minimal, with TODO: fixme comments): ArpeggiateFunctions.cpp,
  NotationsWriter.cpp, OrnamentsFunctions.cpp - UpDownNone -> UpDown, hasLong -> hasLong_.

### Eval score
- Total penalty: 29,998 (down from 39,297, -24%)
- .h: 427/592 exact (72.1%, up from 68.1%)
- .cpp: 202/590 exact (34.2%, up from 31.5%)
- Category breakdown: GOOD 535, EXPECTED 378, COSMETIC 386 (193 penalty), STRUCTURAL 5961
  (29,805 penalty), WRONG 0
- `make check` passes (library compiles clean)
- `make test-all` fails due to test files referencing original API shapes

### New findings

**Finding 25**: Attribute group expansion is the single biggest remaining systemic issue. Many
attribute structs (MordentAttributes, AccidentalTextAttributes, HeelAttributes, ToeAttributes,
StrongAccentAttributes, etc.) are missing attributes inherited through `attributeGroup ref` and
`complexContent extension` chains. The XSD parser currently only collects direct `xs:attribute`
declarations, not those from referenced attribute groups like `print-style`, `trill-sound`,
`print-style-align`, etc. This causes ~500+ penalty points from missing attributes in `.cpp`
files and prevents test compilation.

**Finding 26**: The `long` C++ keyword escaping produces `hasLong_` but the real code uses
`hasLong`. The real code avoids the keyword clash differently (the member is named `long_` but the
has-flag is `hasLong` without underscore). The generator should use `has{PascalName}` where
PascalName is the unescaped attribute name.

**Finding 27**: Many elements with xs:choice children cannot be generated yet because the real code
wraps them in dedicated `*Choice` wrapper classes (NotationsChoice, NoteChoice, KeyChoice,
CreditChoice, OrnamentsChoice, LyricTextChoice, etc.). These follow the same set-choice/single-
choice patterns identified in iteration 6 but require per-element configuration. Currently 14
elements and 4 groups are skipped due to missing Choice class generation.

**Finding 28**: Some XSD simpleType restrictions on xs:string or xs:token are mapped to custom C++
types in the real code (e.g., `mode` -> ModeValue, `instrument-sound` -> PlaybackSound,
`line-width-type` -> LineWidthType, `distance-type` -> DistanceType). These need explicit bespoke
type mappings since the generator defaults to XsString/XsToken.

### Priority fixes for iteration 8
1. **Attribute group expansion** - This is the highest-impact remaining fix. The XSD parser must
   follow `attributeGroup ref` references and `complexContent extension base` chains to collect all
   inherited attributes. This will fix ~500+ STRUCTURAL penalty points and unblock test compilation.
   The groups to handle: print-style, print-style-align, trill-sound, text-formatting, etc.
2. **Expand Choice class generation** - Add more elements to CHOICE_ELEMENT_CONFIG to generate
   their Choice wrapper classes. Priority: notations, note, ornaments, key, credit, lyric.
3. **Fix keyword escaping for has-flags** - `hasLong` not `hasLong_` for the bool flag when the
   attribute name is a C++ keyword.
4. **Fix all-optional streamContents wrapping** - Still needed from iteration 6.
5. **Continue test compilation fixes** - Once attribute groups are expanded, retry `make test-all`
   and fix remaining test compilation errors.
