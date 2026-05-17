# Codegen Forensics

This document records what we learned by statically analyzing (a) the historical Ruby codegen
scripts in `gen/version-a/` and (b) the C++ output they produced in `src/private/mx/core/`. The goal
is to understand what a new generator must reproduce, what was bespoke human judgment, and where the
original process had gaps.

* * *

## Part 1: Ruby Pipeline Analysis (`gen/version-a/`)

### Pipeline Overview

The pipeline was **not** a clean XSD-to-C++ pipeline. It operated in four historical phases with
significant human intervention between each, and it cannot be re-run today.

**Phase 1 - Original hand-built monolith (lost):** A single massive `Elements.h` (`~37,000+` lines)
and matching `Elements.cpp` contained every class declaration and definition. Brute force,
iteratively generated with Ruby scripts that are lost.

**Phase 2 - Splitting:** `writeHFiles.rb` and `writeElementsCppFiles.rb` read the monolithic files
and emitted one `.h`/`.cpp` pair per class into `src/mx/core/elements/`. The split was driven by
line-range detection via `parseElementsH.rb` (scanned for `class X : public ElementInterface` and
matching `};`) and `parseElementsCpp.rb` (scanned for `ClassName::method` patterns, hardcoded
terminal line 37340).

**Phase 3 - Patching `fromXElement`:** `fromXElement.rb` and `attributes.rb`/`attributesLoop.rb`
read the split files and surgically inserted `fromXElement`/`fromXElementImpl` method bodies. The 14
`*Group.rb` scripts each generated a group element's `fromXElement` body to stdout; those snippets
were copy-pasted into the relevant `.cpp` files by hand.

**Phase 4 - Ad-hoc maintenance:** `adjustElementsH.rb`, `adjustElementsCpp.rb`,
`findFilesWithBadCopyright.rb`, `smuflHeader.rb`, `parseElementsHOrder.rb` were run one-off as
needed.

The `mxdeploy*.rb` scripts are unrelated to codegen - they built iOS/macOS frameworks for a
different app.

### XSD Parsing

`parseXsd.rb` is a stub. It uses Ruby's REXML to dump `xs:element` names from `docs/musicxml.xsd` to
stdout and does nothing else. It was never integrated into the emit pipeline. The scripts that
generated the monolithic Elements.h/cpp have been lost.

### Code Emission

**Header splitting (`writeHFiles.rb`):**
- Detects class boundaries by scanning for `class X : public ElementInterface` /
  `struct X : public AttributesInterface` and matching `};`
- Per-class `.h` file: copyright banner, `#pragma once`, conditional includes (only Decimals.h,
  Enums.h, Integers.h, Strings.h), forward declarations, then the class body verbatim
- Include selection uses `isSymbolFound.rb`: regex `(\s#{symbol}\s|\s#{symbol}&)` - only matches
  tokens surrounded by whitespace or followed by `&`. Misses tokens in template args, after `*`,
  after `(`, or at line start

**CPP splitting (`writeElementsCppFiles.rb`):**
- Groups function bodies by class name (scanned from `ClassName::methodName` patterns)
- Hardcodes line 37340 as the end of `Elements.cpp`

**Attributes `fromXElement` (`attributes.rb`, `attributesLoop.rb`):**
- Parses `public:` section of a split `.h` to build member list
- Derives XML attribute name from camelCase member name via
  `name.split(/([[:upper:]][[:lower:]]*)/).join("-").downcase`
- Pairs `has___` booleans with members to determine optionality
- Emits `parseAttribute(...)` calls; enum members get `&parseEnumType` function pointer
- `attributesLoop.rb` batch-applies to all `*Attributes.h` files

**Element `fromXElement` (`fromXElement.rb`):**
- Classifies members by naming convention: `myAttributes`, `myHas___` flag, `___Set`,
  `___GroupPtr/Set`, `___ChoicePtr/Set`
- Emits five body patterns based on classification:
  1. Empty element (no members): `return true;`
  2. Attributes only: delegate to `myAttributes->fromXElement(...)`
  3. Value + attributes: parse attributes, then `myValue.parse(xelement.getValue())`
  4. Value only: `myValue.parse(xelement.getValue())`
  5. Full element: iterate child elements, dispatch on element name string

**Group scripts (`*Group.rb`):**
- 14 standalone scripts, each nearly identical, differing only in class name and members array
- Each member is hand-enumerated with XML element name string, C++ class name, and cardinality
  (`required`, `optional`, or `collection`)
- All write to stdout; copy-pasted into `.cpp` files by hand

### Bespoke Touches in the Ruby Scripts

1. **`Properties` class name override.** The XSD element is named `attributes`, which clashes with
   the C++ keyword and member naming conventions. The C++ class is `Properties`; its `streamName`
   outputs `"attributes"`. This is the only element where C++ class name does not match XSD element
   name. Noted in `todo.txt`.

2. **`EditorialVoiceDirectionGroup` manual registration.** Added by hand to `listElementClasses.rb`
   (`element_classes << "EditorialVoiceDirectionGroup"`) because it doesn't inherit from
   `ElementInterface` in the way the parser could auto-detect.

3. **Hardcoded line 37340** in `parseElementsCpp.rb` as the last line of the monolithic
   `Elements.cpp`. Stale if the file ever changed.

4. **Six hardcoded "special classes"** in `listSpecialClasses.rb`: `Color`, `Date`, `FontSize`,
   `NumberOrNormal`, `PositiveIntegerOrEmpty`, `YesNoNumber`. These live in `core/` not `elements/`
   and required manual registration. `writeHFiles.rb` also hardcodes
   `EmptyPrintObjectStyleAlignAttributes`, `Document`, `DocumentPartwise`, `DocumentTimewise`,
   `DocumentHeader`, `DocumentSpec` with standalone include calls.

5. **`listDecimals.rb` manual fixups.** `kDefaultPrecision` and `kNonZeroAmount` are manually
   appended after parsing `Decimals.h`; `"impl"` is manually deleted.

6. **Required-attribute error message bug.** In `attributes.rb`, the required-attribute error is
   hardcoded as `"'number' is a required attribute but was not found"` regardless of which attribute
   is actually required. This bug propagates to all attributes structs.

7. **Misnamed group scripts.** `midiDeviceInstrumentGroup.rb` generates `NormalTypeNormalDotGroup`
   (not a MIDI group). `layoutGroup.rb` generates `MetronomeRelationGroup`.
   `displayStepOctaveGroup.rb` generates `EditorialGroup`. These were copy-pasted and filenames were
   never updated.

8. **`cueNoteGroup.rb` placeholder bug.** `FullNoteGroup` (a group, not a leaf element) has no XSD
   element name; the script inserts the string `"TODO fix this"` as its element name, generating
   `if( elementName == "TODO fix this" )` - which never matches.

9. **Group scripts output to stdout only.** No script automatically writes a group's `fromXElement`
   body to disk. All group `fromXElement` bodies were copy-pasted by hand.

10. **`elementsHUsingFile.rb` was a one-time op.** Creates C++ `using` alias headers for cases where
    multiple XSD names map to the same C++ class. The call sites in `writeHFiles.rb` are commented
    out.

11. **`MetronomeRelation` frozen value.** Per `todo.txt`, the only valid MusicXML value is
    `"equals"`, so `setValue` is commented out and the constructor hardcodes `myValue("equals")`.

12. **SMUFL glyph map** (`smuflHeader.rb`) generated from `data/smufl/glyphnames.json`, producing
    `src/mx/impl/SMUFLGlyphMap.h`. Entirely outside the MusicXML XSD.

13. **`isSymbolFound` regex fragility.** Matches only `SomeType` or ` SomeType&`. Tokens in
    `<SomeType>`, after `*`, after `(`, or at line start are missed - potentially producing missing
    includes or forward declarations.

14. **Scripts reference stale paths.** All scripts reference `../../src/mx/core/`; the current path
    is `src/private/mx/core/`. Scripts cannot be re-run without path fixes and the absent monolithic
    input files.

### Gaps / Unknowns

- The scripts which produced the original monolithic `Elements.h` are lost.
- 10 group classes have no corresponding group script (some correspond to the misnamed scripts
  listed above; others are simply absent).
- How `*Choice` classes and complex groups were bootstrapped in the monolith is unknown.
- `addStubs.rb` is 0 bytes; its intended purpose is unknown.
- `ElementsOrder.xlsx` is binary; it appears to have established class declaration ordering in the
  monolith, but its content cannot be statically analyzed.
- `adjustElementsCpp.rb` produces a `.replace` file; whether it was ever used to overwrite the
  original is unknown.
- The exact script run order is undocumented; there is no master driver script.

* * *

## Part 2: `mx/core` Code Analysis

### Type Taxonomy

**Primitive / scalar types** (`src/private/mx/core/*.h`, not in `elements/`):

| Category          | Examples                                                                     |
|-------------------|------------------------------------------------------------------------------|
| String wrappers   | `XsString`, `XsToken`, `XsID`, `XsIDREF`, `XsNMToken`, `XsAnyUri`            |
| Integer types     | `Integer` -> `IntRange` -> `BeamLevel`, `OctaveValue`, `Midi128`, etc. (~15) |
| Decimal types     | `Decimal` -> `DecimalRange` -> `TenthsValue`, `Percent`, etc. (~10)          |
| Union types       | `FontSize` (`Decimal`)                                                         |
| pImpl union types | `NumberOrNormal`, `PositiveIntegerOrEmpty`                                   |
| Complex scalars   | `Color` (ARGB/RGB, custom parse/serialize), `CommaSeparatedText`,            |
|                   | `CommaSeparatedListOfPositiveIntegers`                                       |
| Type aliases      | `TimeOnly`, `EndingNumber`, `XlinkHref`, `XlinkRole`, `XlinkTitle`           |

**Enums** (`Enums.h`, `EnumsBuiltin.h`):

89 total (85 + 4 builtins). Each has `parse<Name>`, `tryParse<Name>` (returns `std::optional`),
`toString`, `toStream`, `operator<<`. Builtins (`XlinkType`, `XmlSpace`, `XlinkActuate`,
`XlinkShow`) lack `tryParse`.

Suffix rules:
- No suffix (47): XSD type name has no C++ collision (`AboveBelow`, `CssFontSize`, `YesNo`).
- `Enum` suffix (23): XSD type name collides with an element class name (`BarStyleEnum`,
  `DynamicsEnum`, `StepEnum`).
- `Value` suffix (15): XSD extensible/open type (`BeamValue`, `AccidentalValue`, `BeaterValue`).
- 4 "open extension" wrapper classes: wrap an `enum class` + `std::string myCustomValue` for
  `other-*` values (`DistanceType`, `DynamicsValue`, `LineWidthType`, `ModeValue`).

**Element classes** (`elements/`, 1182 files = 591 pairs):

| Subcategory               | Count (approx) | Notes                                   |
|---------------------------|----------------|-----------------------------------------|
| `*Attributes` structs     | 174 pairs      | One per element that has XSD attributes |
| `*Choice` classes         | 21 pairs       | XSD `xs:choice` constructs              |
| `*Group` classes          | 24 pairs       | XSD `xs:group` named groups             |
| Leaf / container elements | ~372 pairs     | Normal element classes                  |

**Infrastructure** (`core/` non-element headers): `ElementInterface`, `AttributesInterface`,
`ForwardDeclare.h` macros, `FromXElement.h` template helpers, `Document`, `DocumentSpec`,
`ScoreConversions`, `ProcessingInstruction`, `EmptyPrintObjectStyleAlignAttributes`.

### Standard Element Template

Every "leaf element with a value" follows this exact pattern:

**Header:**
```cpp
MX_FORWARD_DECLARE_ATTRIBUTES(XxxAttributes)  // if element has attributes
MX_FORWARD_DECLARE_ELEMENT(ClassName)

inline ClassNamePtr makeClassName() { ... }
inline ClassNamePtr makeClassName(const ValueType&) { ... }
inline ClassNamePtr makeClassName(ValueType&&) { ... }

class ClassName : public ElementInterface {
public:
    ClassName();
    ClassName(const ValueType& value);
    virtual bool hasAttributes() const;
    virtual bool hasContents() const;
    virtual std::ostream& streamAttributes(std::ostream& os) const;
    virtual std::ostream& streamName(std::ostream& os) const;
    virtual std::ostream& streamContents(std::ostream& os, int indentLevel,
                                         bool& isOneLineOnly) const;
    XxxAttributesPtr getAttributes() const;
    void setAttributes(const XxxAttributesPtr& attributes);
    ValueType getValue() const;
    void setValue(const ValueType& value);
private:
    virtual bool fromXElementImpl(std::ostream& message, ::ezxml::XElement& xelement);
private:
    ValueType myValue;
    XxxAttributesPtr myAttributes;
};
```

**Attributes struct:**
```cpp
struct XxxAttributes : public AttributesInterface {
    XxxAttributes();
    virtual bool hasValues() const;
    virtual std::ostream& toStream(std::ostream& os) const;
    Type1 field1;    bool hasField1;
    Type2 field2;    bool hasField2;
private:
    virtual bool fromXElementImpl(std::ostream& message, ::ezxml::XElement& xelement);
};
```

Constructor initializes all `has*` to `false`. `hasValues()` is OR of all `has*` flags. `toStream`
calls `streamAttribute(os, field, "xsd-attr-name", hasField)` per attribute. `fromXElementImpl`
iterates `xelement.attributesBegin()..End()` with `if (parseAttribute(...)) continue;` chains;
enum-typed attributes pass a `&parseXxx` pointer.

### Variance Map

| Aspect                     | How it varies                                               | XSD source              |
|----------------------------|-------------------------------------------------------------|-------------------------|
| Class name                 | PascalCase of XSD element name                              | `xs:element name=""`    |
| `streamName` string        | Verbatim XSD kebab-case name                                | `xs:element name=""`    |
| Value type                 | enum, XsString, XsToken, Integer/Decimal subclass, or none  | `simpleType`/content    |
| `hasContents()`            | `true` if text content or children; `false` if empty        | Content model           |
| Attributes struct          | Own `XxxAttributes` pair or shared `EmptyXxx` from `core/`  | Attribute group         |
| `makeXxx(value)` overloads | Present only if element has a value type                    | same                    |
| Child accessors            | See cardinality table below                                 | `minOccurs`/`maxOccurs` |
| `fromXElementImpl`         | `importElement(...)` required; sets `myHasXxx` for optional | Content model           |

**Child accessor pattern by cardinality:**

- `min=1, max=1` (required): member `ChildPtr myChild;`, accessors `getChild()` / `setChild()`.
- `min=0, max=1` (optional): member `ChildPtr myChild; bool myHasChild;`, accessors `getChild()` /
  `setChild()` / `getHasChild()` / `setHasChild(bool)`.
- `max=unbounded` (set): member `ChildSet myChildSet;`, accessors `getChildSet()` / `addChild()` /
  `removeChild(iter)` / `clearChildSet()`.

For `min=1, max=unbounded`: pre-populated with one default instance; `clearChildSet()` re-adds one;
`removeChild()` refuses to erase the last item.

**Group elements:** `streamName` returns empty string (no XML tag). `hasAttributes()` returns
`false`. They are transparent wrappers for XSD named groups.

**Choice elements:** Hold an inner `enum class Choice { ... }` (values start at 0 in some, 1 in
others - inconsistent). All alternatives are pre-allocated in the constructor. `streamContents`
switches on `myChoice`. `fromXElementImpl` dispatches on `xelement.getName()`.

### Naming Conventions

**XSD element name -> C++ class name:** kebab-case to PascalCase. `score-partwise` ->
`ScorePartwise`, `time-modification` -> `TimeModification`.

**Member variable prefix:** `my` for all members: `myValue`, `myAttributes`, `myHasChild`.

**Typedef pattern (generated by `MX_FORWARD_DECLARE_ELEMENT`):** `ClassNamePtr`, `ClassNameUPtr`,
`ClassNameSet`, `ClassNameSetIter`, `ClassNameSetIterConst`.

**Factory functions:** `makeClassName()`, `makeClassName(const T&)`, `makeClassName(T&&)` as inline
free functions in the header.

**XSD attribute name -> C++ field name:** kebab-case to camelCase. `default-x` -> `defaultX`,
`font-family` -> `fontFamily`. Presence flag: `has` + PascalCase: `hasDefaultX`, `hasFontFamily`.

**Enum type suffix disambiguation:**
- No suffix when the XSD type name has no class-name collision.
- `Enum` suffix when a class with the same PascalCase name also exists (e.g., `BarStyle` element ->
  type becomes `BarStyleEnum`).
- `Value` suffix when the XSD type is extensible / ends in `-value`.

**Enum value C++ names:** PascalCase from the XSD string. Reserved words get a trailing `_`:
`continue_`, `double_`, `short_`, `new_`, `default_`.

**XML string literals in `streamName` / `streamAttribute`:** always the original XSD kebab-case
string verbatim.

### Sequence vs. Choice vs. Group

- **`xs:sequence`:** child members are declared in order on the class. `streamContents` outputs them
  in sequence with newlines between. `fromXElementImpl` uses `importElement(...)` template or manual
  `if (elementName == "xxx")` dispatch.
- **`xs:choice`:** generates a `*Choice` class with inner `enum class Choice`. All alternatives are
  pre-allocated. `streamContents` switches on `myChoice`. Some choices implement `fromXElementImpl`
  by dispatching on `xelement.getName()`; others use `MX_FROM_XELEMENT_UNUSED` when the parent
  handles dispatch.
- **`xs:group` (named groups):** generates `*Group` class. `streamName` returns empty string. Parent
  streams the group's contents directly via `group->streamContents(...)`.

### Bespoke and Anomalous Items

**`Properties` class** (`elements/Properties.h`): C++ class name is `Properties`; `streamName`
outputs `"attributes"`. Only element where class name does not match XSD element name. The XSD
element name `attributes` would clash with C++ keywords and naming conventions.

**`Accidental.cpp`**: Hand-written error tolerance: if the parsed value is `"double-flat"` (not
valid in MusicXML 3.0), it is silently remapped to `"flat-flat"` with a log message. Not derivable
from the XSD.

**`Dynamics.h/.cpp`**: Two structural deviations. (1) `setAttributes` takes
`const DynamicsAttributes& attributes` by const-ref value rather than the universal
`const DynamicsAttributesPtr&` pointer pattern. (2) `streamContents` hand-writes inner XML
sub-elements (`<ff/>`, `<other-dynamics>text</other-dynamics>`), breaking the standard "element text
= `os << myValue`" pattern. `fromXElementImpl` reads `xelement.begin()->getName()` to reconstruct
the dynamic mark from a child element name. The only element that self-serializes child tags inside
`streamContents`.

**`Note.h/.cpp`**: The most complex element. Three private parse helpers: `parseNoteChoice`,
`parseFullNoteGroup`, `parseEditorialVoiceGroup`. Large hand-written `fromXElementImpl`. Optional
children use JIT allocation with `MX_MUTEX`/`MX_LOCK`/ `MX_JIT_ALLOCATE` macros and `mutable`
members. `addBeam` enforces `maxOccurs=8` with a size guard (the only finite-`maxOccurs` enforcement
anywhere).

**JIT-allocated elements** (`MX_MUTEX`/`MX_LOCK`/`MX_JIT_ALLOCATE`): `Note`, `MusicDataChoice`,
`FullNoteGroup`, `FullNoteTypeChoice`, `PartwiseMeasure`. No clear structural rule separates these
from other complex elements that pre-allocate. Likely applied manually where profiling showed
allocation cost.

**`MX_FROM_XELEMENT_UNUSED` elements:** `NoteChoice`, `KeyChoice`, `EditorialGroup` (and possibly
others). These are parsed by their parent elements rather than independently.

**`ScorePartwise.cpp`**: `fromXElementImpl` explicitly checks child element names to skip
header-group elements in the `<part>` loop, delegating to `myScoreHeaderGroup->fromXElement(...)`.
Hand-written cross-delegation logic.

**`EmptyPrintObjectStyleAlignAttributes.h`** (in `core/`, not `elements/`): Only attributes struct
at the `core/` level. Declares `fromXElementImpl` as `public` rather than `private` - the sole
inconsistency of this kind across all attributes structs.

**`PlaybackSound.h`**: Uses X-macro technique (`Enum.h` utility) rather than the hand-written
`if`/`else` chains used in `Enums.h`. Generates `toString`/`fromString` but not the full
`parse`/`tryParse`/`operator<<`/`toStream` suite the other enums have. Likely added later when the
instrument sound list grew too large for manual maintenance.

**`DocumentSpec.h`**: Defines `enum DocumentChoice` (old-style, not `enum class`) alongside two
`enum class` definitions. Inconsistent with the rest of the codebase.

**`PositiveDivisionsValue`** in `Decimals.h`: Marked with
`/// TODO - this needs to be properly generated`. Appears to be an incomplete or redundant type
duplicating `PositiveDivisions`.

**`TupletReader.h`**: Empty file (1 line). No `.cpp`. Dead placeholder.

**`AttributesIterface.h`** (note typo: `Iterface`): Stub struct with no fields, no `.cpp`. Used only
in `Elements.h`. Appears to be an unused stub with a typo in the name.

**`Choice` enum starting values:** Inner `enum class Choice` starts at 0 in some choice classes and
1 in others. The inconsistency has no apparent semantic significance.

### Gaps and Open Questions

1. **Enum suffix selection** (`Enum` vs. `Value` vs. no suffix) requires collision detection against
   the full set of element class names. A new generator must build that set first.

2. **`EmptyPrintObjectStyleAlignAttributes` placement** in `core/` vs. `elements/` has no clear
   structural criterion. May simply have been promoted to `core/` because multiple elements share it
   via attribute groups.

3. **JIT allocation trigger**: Which elements use `MX_JIT_ALLOCATE` vs. pre-allocate is not
   derivable from the XSD. Treat pre-allocation as the default; JIT allocation can be applied
   manually to performance-critical paths.

4. **`MX_FROM_XELEMENT_UNUSED` application**: Which groups and choices delegate to their parent for
   parsing is not fully derivable from the XSD. Groups generally do; some choices do.

5. **`PlaybackSound` X-macro pattern**: Needs a separate codegen path or manual treatment in a new
   generator.

6. **`PositiveDivisionsValue`** relationship to `PositiveDivisions` needs XSD verification before a
   new generator handles it.

7. **`Note::addBeam` maxOccurs=8 guard**: The only enforced finite `maxOccurs > 1`. Whether a new
   generator should emit such guards generally needs a policy decision.

8. **Shared `EmptyXxx` attribute structs**: Multiple elements share one struct (e.g.,
   `EmptyPlacementAttributes`). XSD uses attribute groups for this. The mapping from attribute group
   -> shared struct needs explicit handling in a new generator.

* * *

## Summary: What a New Generator Must Handle

| Concern                                   | Derivable from XSD?    | Notes                                                     |
|-------------------------------------------|------------------------|-----------------------------------------------------------|
| Class names (kebab -> PascalCase)         | Yes                    |                                                           |
| XML string literals (`streamName`)        | Yes                    |                                                           |
| Enum definitions (most)                   | Yes                    | Suffix disambiguation requires class-name collision check |
| Enum open-extension wrappers              | Yes                    | Extensible types with `other-*` values                    |
| Attributes struct pattern                 | Yes                    |                                                           |
| Child cardinality (optional/required/set) | Yes                    |                                                           |
| Sequence/choice/group structure           | Yes                    |                                                           |
| `Properties` class name                   | No                     | `attributes` -> `Properties` override                     |
| `MetronomeRelation` frozen value          | No                     | Only valid value is `"equals"`                            |
| `Accidental` double-flat remap            | No                     | Error-tolerance hand-patch                                |
| `Dynamics` child-tag serialization        | No                     | Non-standard serialization pattern                        |
| `Note` complexity / JIT alloc             | No                     | Manual performance work                                   |
| `PlaybackSound` X-macro pattern           | No                     | Or unify with Enums.h approach                            |
| SMUFL glyph map                           | No                     | Separate data source (`glyphnames.json`)                  |
| `DocumentSpec` old-style enum             | No                     | Minor inconsistency; could normalize                      |
| `PositiveDivisionsValue` TODO             | No                     | Needs resolution                                          |
| Required-attribute error strings          | Partially              | Bug in original; a new generator should fix               |
| Shared attribute group structs            | Yes (attribute groups) | Explicit mapping step needed                              |
