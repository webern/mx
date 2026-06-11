# mx::core: the C++ generated typed model

Status: design proposal. The C++ target's `gen/cpp/config.toml` exists; its `templates/` directory
does not. This document specifies the rendered C++ shapes the templates must produce — every
construct chosen with a reason a reviewer can argue with. It is a sibling to `plates.md` and
`generator-agnosticism.md`: those documents describe the engine; this one describes the artifact
the engine prints for the C++ edition.

It is intentionally non-template. Every snippet below is what the **generator emits**, not what a
template looks like; the templates are the renderer's problem and live under
`gen/cpp/templates/` once these shapes are agreed.

The reader is assumed to have read `plates.md` (the eight shapes, the per-target binding) and
`gen/README.md` (the IR, the resolver, the deps-first DAG, the no-collision invariant). Cross-
references in this document use the IR/Plates vocabulary verbatim.

---

## 1. Decisions in one page

The design hangs off five non-negotiables. Everything in §3-§13 derives from these.

1. **The complex-type graph is a DAG with no cycles** (`gen/README.md` §"Two load-bearing
   invariants"). No forward declarations. No `shared_ptr`. No heap. **Plain by-value composition
   throughout.** A `Note` *contains* its children, it does not point at them.

2. **The schema's `name -> type` map is 1:1.** Parse dispatch is a static name table; we never
   need context-sensitive resolution. Serialization writes a known tag for each member.

3. **The data classes are dumb structs.** Public fields, aggregate-initializable, no invariants
   beyond what `enum class` and the value-type wrappers already give us. Validation lives in the
   value-type wrappers (clamp on assignment) and in parse (lenient values, strict names — the
   plates round-3 contract). Assigning a value of a typed field is by definition valid; that is
   the whole point of generating typed wrappers.

4. **Document order is preserved by structure, not by a separate sequence number.** Pure-sequence
   composites use plain ordered fields. Choice-bearing or repeating content uses
   `std::vector<std::variant<...>>` so order is the array order and "exactly one alternative" is
   true by construction. **C++ does NOT copy the Go/C ordered-children-of-pointers encoding** —
   that pattern exists because Go and C have no sum type. We do; we use it. (`plates.md` §11
   round 3 is explicit about this.)

5. **Parse and serialize are free functions over `ezxml::XElement`, not member functions.** The
   typed model is one concern (data); XML I/O is another (interop). Keeping them apart lets the
   model classes be aggregates and lets a future binding (JSON, MessagePack) reuse the same
   classes by writing its own free functions.

The standard is **C++17** (matches the existing `src/private` codebase: `std::optional`,
`std::variant`, `std::filesystem`). No C++20-only features are required.

---

## 2. File and namespace layout

One type per file, per the existing `[layout] partition = "per-type"` convention (matches Go's and
C's per-type layout, matches what the existing `src/private/mx/core/` directory was sized for, and
gives a 1:1 mapping between IR types and headers — the smallest possible blast radius for a schema
change). Header and implementation are split:

```
src/private/mx/core/
  Decimal.h                          <- hand-written runtime
  Decimal.cpp
  Runtime.h                          <- hand-written: SupportedMusicXMLVersion, parse helpers
  Runtime.cpp
  AboveBelow.h         AboveBelow.cpp        <- generated, value: enum
  Divisions.h          Divisions.cpp         <- generated, value: number
  FontSize.h           FontSize.cpp          <- generated, value: union
  Empty.h              Empty.cpp             <- generated, complex: empty (presence-only is a
                                                bool typedef; this is for attrs-only emptys)
  AccidentalText.h     AccidentalText.cpp    <- generated, complex: value-bodied
  Note.h               Note.cpp              <- generated, complex: composite (choice content)
  Pitch.h              Pitch.cpp             <- generated, complex: composite (pure sequence)
  Mordent.h            Mordent.cpp           <- generated, complex: derived (inheritance)
  Document.h           Document.cpp          <- generated, document root + extra-attr struct
  ...
  sources.cmake                              <- generated, lists all .cpp files for CMake
```

File stem is the type's PascalCase identifier (`Plates.Variant.cased["pascal"]` for the type name)
to match the existing C++ convention in this repo (`XElement.h`, `CoreRoundtripImpl.cpp`).

All generated symbols live in `namespace mx::core`. The C++ Decimal wrapper, the runtime helpers,
and the `SupportedMusicXMLVersion` constant live in the same namespace; the generator does not
produce a separate `mx::core::detail` (no symbol needs to be hidden — every emitted name is part
of the public API of the typed model).

```cpp
// AboveBelow.h
#pragma once

namespace mx::core
{

// (doc comment ...)
enum class AboveBelow
{
    Above,
    Below,
};

} // namespace mx::core
```

Headers use `#pragma once` (already the convention in `src/private/mx/ezxml/`). No include guards
of the `MX_..._H_INCLUDED` form: pragma once is universally supported by the compilers this
project targets, and matches what is already in the tree.

Each header `#include`s only the headers it textually needs. The Plates already compute the per-
file `deps`, so the include list is data-driven and stable. The DAG invariant means cycles are
impossible, which is why this works without forward declarations.

---

## 3. Value: enum class

One file per enum. The wire-literal table and the parse map are emitted into the `.cpp` so the
header is light. The generator's `[target] variant-scope = "bare"` setting (already in
`gen/cpp/config.toml`) means the variant identifier is the bare PascalCase name, scoped by the
`enum class`.

```cpp
// AboveBelow.h
#pragma once

namespace mx::core
{

/// The above-below type is used to indicate whether one element appears above
/// or below another element.
enum class AboveBelow
{
    Above,
    Below,
};

/// Returns the wire literal for `v` (static storage, never null).
const char* toString(AboveBelow v);

/// Strict parse: returns true and writes `*out` only when `s` exactly matches
/// a wire literal.
bool tryParse(const char* s, AboveBelow* out);

/// Lenient parse: an unrecognized input falls back to the first variant
/// (mirrors the Go/C contract; the corert harnesses depend on this).
AboveBelow parseAboveBelow(const char* s);

} // namespace mx::core
```

```cpp
// AboveBelow.cpp
#include "mx/core/AboveBelow.h"

#include <string_view>

namespace mx::core
{

namespace
{
constexpr const char* kAboveBelowWire[] = {
    "above",
    "below",
};
} // namespace

const char* toString(AboveBelow v)
{
    const auto i = static_cast<size_t>(v);
    if (i >= sizeof(kAboveBelowWire) / sizeof(kAboveBelowWire[0]))
    {
        return kAboveBelowWire[0];
    }
    return kAboveBelowWire[i];
}

bool tryParse(const char* s, AboveBelow* out)
{
    if (s == nullptr) { return false; }
    const std::string_view sv{s};
    if (sv == "above") { *out = AboveBelow::Above; return true; }
    if (sv == "below") { *out = AboveBelow::Below; return true; }
    return false;
}

AboveBelow parseAboveBelow(const char* s)
{
    AboveBelow v;
    if (tryParse(s, &v)) { return v; }
    return AboveBelow::Above;
}

} // namespace mx::core
```

**Why a `switch`/`if`-chain rather than a `std::unordered_map<std::string, AboveBelow>`?** The
chain is generated, deterministic, branch-predictor-friendly for the common case, and zero-init
at startup; an unordered_map costs a heap allocation per enum and an unordered hash lookup per
parse. With ~96 enums in MusicXML 4.0 the difference is real and free for the generator to
arrange. The chain is the same shape Go and C use.

**Why overloaded `toString` and `tryParse` (no name composition like `toStringAboveBelow`)?** ADL
plus C++ overload resolution makes `toString(myEnum)` work without the caller naming the type. Go
and C compose names because they have no overloading. We do; we use it. (Free functions, not
member functions, because `enum class` cannot have member functions.)

**`parseAboveBelow` is composed.** The lenient-fallback parse cannot be overloaded (no input enum
to dispatch on); composition is forced by the language, which is exactly the Variant.cased
[pascal] composed-with-`parse` rule the plates already implement.

---

## 4. Value: number wrapper

A `class` wrapping the IR primitive's target type, with clamp-on-construct using the plate's
`NumberPlate.clamp` steps (resolved facets + primitive-implied bounds, computed once in the
plates per round 2 of `plates.md` §11). The inner representation is whatever
`gen/cpp/config.toml` `[types]` maps the IR primitive to.

`Decimal` is `mx::core`'s own hand-written class (lossless decimal-as-text + a numeric face for
arithmetic; the existing corert harness has trailing-zero rules that depend on the lossless
representation, see `DecimalFields.h`). The generator does not invent it; `[types] decimal =
"Decimal"` says "use the existing class," and `Divisions` below wraps it.

```cpp
// Divisions.h
#pragma once

#include "mx/core/Decimal.h"

namespace mx::core
{

/// The divisions type is used to express values in terms of the musical
/// divisions defined by the divisions element. It is preferred that these be
/// integer values both for MIDI interoperability and to avoid roundoff
/// errors.
class Divisions
{
public:
    Divisions() = default;
    explicit Divisions(Decimal value);            // clamp on assignment
    explicit Divisions(double value);             // convenience
    Decimal getValue() const;
    void setValue(Decimal value);                 // clamp on assignment
    bool operator==(const Divisions& other) const;
    bool operator!=(const Divisions& other) const;

private:
    Decimal m_value{};
};

const char* toString(const Divisions& v, std::string& buf);  // see §14
bool tryParse(const char* s, Divisions* out);
Divisions parseDivisions(const char* s);

} // namespace mx::core
```

For a number with bounds (`positive-decimal`, `tenths`, `midi-channel` with `[1, 16]`, etc.), the
constructor and setter call a generated `clamp` free helper that runs the plate's clamp steps.
The clamp steps are *data on the plate* per round 2; the template emits one branch per step.

```cpp
// Tenths.h
#pragma once

#include "mx/core/Decimal.h"

namespace mx::core
{

class Tenths
{
public:
    Tenths() = default;
    explicit Tenths(Decimal value);
    Decimal getValue() const;
    void setValue(Decimal value);
    // ... operator== etc.
private:
    Decimal m_value{};
};
// (parse / toString / tryParse free functions as above)

} // namespace mx::core
```

```cpp
// MidiChannel.cpp  (illustrative: integer with min=1, max=16)
#include "mx/core/MidiChannel.h"

namespace mx::core
{

namespace
{
int clamp(int v) { if (v < 1) { return 1; } if (v > 16) { return 16; } return v; }
} // namespace

MidiChannel::MidiChannel(int value) : m_value{clamp(value)} {}
void MidiChannel::setValue(int value) { m_value = clamp(value); }
int MidiChannel::getValue() const { return m_value; }

} // namespace mx::core
```

**Why a class with private storage rather than a `using Tenths = Decimal;`?** Type identity. With
strong typedefs, `void f(Tenths)` cannot be silently called with a `Decimal` or a `Divisions`;
that's the whole point of generating ~25 distinct numeric types from the schema. A `using` alias
gives every numeric type the same C++ identity, which throws away the schema's information at the
language boundary.

**Why explicit constructors?** Same reason: prevent silent conversions from `int` or `double`
landing in the wrong wrapper. A user who wants to construct a `Divisions{4}` must say so.

**Why a getter/setter pair, not a public `value` field?** Because the setter clamps. A public
field would let the invariant escape. This is the one place in the generated code where a class
has private state; it earns it.

---

## 5. Value: string wrapper

A class over `std::string`, optionally enforcing a regex pattern and length bounds. Most string
types in MusicXML are unconstrained tokens, so the wrapper is mostly identity; the few with
patterns (`color`, `time-only`, `comma-separated-text`) get the check on `setValue`.

```cpp
// Color.h
#pragma once

#include <string>

namespace mx::core
{

/// The color type indicates the color of an element. Color may be represented
/// as hexadecimal RGB triples or as hexadecimal ARGB tuples [...]
class Color
{
public:
    Color() = default;
    explicit Color(std::string value);
    const std::string& getValue() const;
    void setValue(std::string value);
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;
private:
    std::string m_value;
};

void toString(const Color& v, std::string& out);
bool tryParse(const char* s, Color* out);          // pattern-checked
Color parseColor(const char* s);                   // lenient: accepts on parse failure

} // namespace mx::core
```

For the unconstrained string aliases (the majority), the wrapper is the same shape but `tryParse`
always succeeds; the class is generated for type-identity reasons (so `void f(CommaSeparatedText)`
will not silently take a `Color`), not for validation. Identical rationale to numeric wrappers.

---

## 6. Value: union (tagged variant)

`std::variant` over the member types. This is the C++17 sum type; using anything else here would
be reinvention. The plate already carries `UnionPlateMember.tag` (per round 2 of `plates.md`
§11), so each alternative has a stable, projected, collision-checked discriminator.

```cpp
// FontSize.h
#pragma once

#include "mx/core/CssFontSize.h"
#include "mx/core/Decimal.h"

#include <variant>

namespace mx::core
{

/// The font-size can be one of the CSS font sizes or a numeric point size.
class FontSize
{
public:
    enum class Kind { Decimal, CssFontSize };

    FontSize();                                   // default: first member, default-constructed
    explicit FontSize(Decimal v);
    explicit FontSize(CssFontSize v);

    Kind getKind() const;

    // Each accessor is valid only when getKind() returns the matching kind.
    // std::get throws std::bad_variant_access on mismatch.
    Decimal getDecimal() const;
    void setDecimal(Decimal v);
    CssFontSize getCssFontSize() const;
    void setCssFontSize(CssFontSize v);

    bool operator==(const FontSize& other) const;
    bool operator!=(const FontSize& other) const;

private:
    std::variant<Decimal, CssFontSize> m_value;
};

void toString(const FontSize& v, std::string& out);
bool tryParse(const char* s, FontSize* out);     // tries members in schema order
FontSize parseFontSize(const char* s);            // lenient: first member absorbs

} // namespace mx::core
```

**Why expose `std::variant` *behind* getter/setter rather than inheriting from it or making
`m_value` public?** Two reasons. First, accessor names reveal the schema's intent (
`getCssFontSize()` reads at the call site; `std::get<CssFontSize>(fs.value)` does not). Second,
the `Kind` enum gives the caller a `switch`-friendly discriminator without the `std::holds_
alternative` boilerplate. The generated code's *interface* is the kind enum + per-member accessors;
the variant is the *implementation*.

**Why not a hand-rolled tagged union (a struct with a `Kind` and a `union { ... }`)?** Lifetime.
The members can have non-trivial destructors (a `std::string`-backed wrapper). `std::variant`
handles the lifetime; a hand-rolled union would have to do it manually and would be a hand-written
piece of generated code per union, which is reinvention.

**`tryParse` order matters.** The plates round-2 rule "an open string union member must be last"
keeps `tryParse` from short-circuiting. The C++ template enforces this at render time, identical
to Go/C; otherwise lossy schemas would silently route everything through the open string member.

---

## 7. Complex: empty

Two sub-cases, both already separated by the plates:

- **Presence-only** (`presence_only: true`, no attributes): the IR's "the only information is
  whether it appears." Emit as `using Empty = bool;` — there is **nothing else worth saying**.
  An ordered-children variant alternative whose payload is `Empty` is *visibly* a presence flag.
  ```cpp
  // Empty.h    (a few presence-only types are aliased to bool)
  #pragma once

  namespace mx::core
  {

  /// Presence-only marker. The element is either present (`true`) or absent.
  using Empty = bool;

  } // namespace mx::core
  ```

  An alternative — emitting an empty `class Empty {};` — would compile, but `Empty{}` and `Empty{}`
  are indistinguishable at every call site, so the type adds nothing the language doesn't already
  give us with `bool`. The `using` is honest about the type's information content.

- **Attrs-only** (`presence_only: false`, attributes only): a class whose fields are the
  attributes (§9 conventions for attribute fields). Same shape as a value-bodied complex type
  minus the `value` field.

---

## 8. Complex: value-bodied

A class whose `value` field is the typed text body and whose other fields are the attributes.
Aggregate; default-constructible; field order = attribute declaration order (then `value` last).

```cpp
// AccidentalText.h
#pragma once

#include "mx/core/AccidentalValue.h"
#include "mx/core/Color.h"
#include "mx/core/CommaSeparatedText.h"
#include "mx/core/EnclosureShape.h"
#include "mx/core/FontSize.h"
#include "mx/core/FontStyle.h"
#include "mx/core/FontWeight.h"
#include "mx/core/LeftCenterRight.h"
#include "mx/core/NumberOfLines.h"
#include "mx/core/NumberOrNormal.h"
#include "mx/core/RotationDegrees.h"
#include "mx/core/SmuflAccidentalGlyphName.h"
#include "mx/core/Tenths.h"
#include "mx/core/TextDirection.h"
#include "mx/core/Valign.h"

#include <optional>
#include <string>

namespace ezxml { class XElement; }

namespace mx::core
{

/// The accidental-text type represents an element with an accidental value
/// and text-formatting attributes.
class AccidentalText
{
public:
    // Optional attributes use std::optional (§13).
    std::optional<SmuflAccidentalGlyphName> smufl;
    std::optional<std::string>              xmlLang;
    std::optional<std::string>              xmlSpace;
    std::optional<LeftCenterRight>          justify;
    std::optional<Tenths>                   defaultX;
    std::optional<Tenths>                   defaultY;
    std::optional<Tenths>                   relativeX;
    std::optional<Tenths>                   relativeY;
    std::optional<CommaSeparatedText>       fontFamily;
    std::optional<FontStyle>                fontStyle;
    std::optional<FontSize>                 fontSize;
    std::optional<FontWeight>               fontWeight;
    std::optional<Color>                    color;
    std::optional<LeftCenterRight>          halign;
    std::optional<Valign>                   valign;
    std::optional<NumberOfLines>            underline;
    std::optional<NumberOfLines>            overline;
    std::optional<NumberOfLines>            lineThrough;
    std::optional<RotationDegrees>          rotation;
    std::optional<NumberOrNormal>           letterSpacing;
    std::optional<NumberOrNormal>           lineHeight;
    std::optional<TextDirection>            dir;
    std::optional<EnclosureShape>           enclosure;

    // Required: by-value, default-constructible.
    AccidentalValue value{};
};

bool operator==(const AccidentalText& a, const AccidentalText& b);
bool operator!=(const AccidentalText& a, const AccidentalText& b);

void parseAccidentalText(const ezxml::XElement& el, AccidentalText& out);
void serializeAccidentalText(const AccidentalText& v, ezxml::XElement& parent,
                             const std::string& tag);

} // namespace mx::core
```

**Public fields, not getters/setters.** The class has no invariant beyond what each *member*'s
type already enforces (`Tenths` clamps on assignment, `EnclosureShape` is an enum class).
Wrapping a getter around an `std::optional<Tenths>` would add ceremony and zero safety.

**`std::optional<T>` for optional members.** Standard, value-typed (no allocation), trivially
distinguishable from "present with default value" — the case `defaultX = Tenths{0}` is genuinely
different from `defaultX = std::nullopt`, and the wire format reflects it. Go uses pointers
because Go has no `optional<T>`; C uses a `bool has_x` companion because C has no generics. We
have `std::optional`; we use it.

**`bool operator==` is generated.** Aggregates are not equality-comparable by default in C++17
(C++20's `=default` would change this). The generator emits the per-field comparison. The corert
harness does not need it (it diffs at the XML level), but the API shape is dramatically more
useful for everyone else (unit tests, custom user code, hashing later).

**Required fields are by-value.** No `optional` wrapper, no pointer. The DAG invariant means
this is always safe; default construction gives the type's natural zero (a `Tenths{0}`, an
`AccidentalValue::Sharp`).

---

## 9. Complex: composite (the crucial case)

Composites are the hard one and the place the C++ design diverges from Go and C. A composite has
attributes plus a content tree (`Resolver.content`, group-spliced). Two flavors:

**9a. Pure-sequence composite, every member exactly once (or optional once), no choices, no
repeats.** Plain ordered fields. `Pitch` (step, alter?, octave) is the canonical case.

```cpp
// Pitch.h
#pragma once

#include "mx/core/Octave.h"
#include "mx/core/Semitones.h"
#include "mx/core/Step.h"

#include <optional>

namespace ezxml { class XElement; }

namespace mx::core
{

/// Pitch is represented as a combination of the step of the diatonic scale,
/// the chromatic alteration, and the octave.
class Pitch
{
public:
    // No attributes on <pitch>.
    Step                     step{};
    std::optional<Semitones> alter;
    Octave                   octave{};
};

bool operator==(const Pitch& a, const Pitch& b);
bool operator!=(const Pitch& a, const Pitch& b);

void parsePitch(const ezxml::XElement& el, Pitch& out);
void serializePitch(const Pitch& v, ezxml::XElement& parent, const std::string& tag);

} // namespace mx::core
```

The field order in the class *is* the document order; serialize walks the fields in declaration
order. Parse loops over child elements and dispatches by tag name onto the matching field (ignoring
order, per round-3's "lenient about structure" rule).

**9b. Choice-bearing or repeating content.** Attributes are still ordinary fields; **child
elements live in one ordered `std::vector` of a per-type `std::variant`.**

```cpp
// Note.h
#pragma once

#include "mx/core/Accidental.h"
#include "mx/core/Beam.h"
#include "mx/core/Color.h"
#include "mx/core/CommaSeparatedText.h"
#include "mx/core/Divisions.h"
#include "mx/core/Empty.h"
#include "mx/core/EmptyPlacement.h"
#include "mx/core/FontSize.h"
#include "mx/core/FontStyle.h"
#include "mx/core/FontWeight.h"
#include "mx/core/FormattedText.h"
#include "mx/core/Grace.h"
#include "mx/core/Instrument.h"
#include "mx/core/Level.h"
#include "mx/core/Lyric.h"
#include "mx/core/Notations.h"
#include "mx/core/NoteType.h"
#include "mx/core/Notehead.h"
#include "mx/core/NoteheadText.h"
#include "mx/core/NonNegativeDecimal.h"
#include "mx/core/Pitch.h"
#include "mx/core/Play.h"
#include "mx/core/PositiveDivisions.h"
#include "mx/core/Rest.h"
#include "mx/core/Stem.h"
#include "mx/core/Tenths.h"
#include "mx/core/Tie.h"
#include "mx/core/TimeModification.h"
#include "mx/core/TimeOnly.h"
#include "mx/core/Unpitched.h"
#include "mx/core/YesNo.h"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ezxml { class XElement; }

namespace mx::core
{

/// One child element of <note>: exactly one alternative by construction.
/// Document order is the index in Note::children.
///
/// The `using` aliases the variant; `std::visit` and the dispatch helpers
/// (§9c) work directly on it.
using NoteChild = std::variant<
    Grace,                  // <grace>
    Empty,                  // <chord>          (presence-only)
    Pitch,                  // <pitch>
    Unpitched,              // <unpitched>
    Rest,                   // <rest>
    Tie,                    // <tie>
    Empty,                  // <cue>            (presence-only)
    PositiveDivisions,      // <duration>
    Instrument,             // <instrument>
    FormattedText,          // <footnote>
    Level,                  // <level>
    std::string,            // <voice>
    NoteType,               // <type>
    EmptyPlacement,         // <dot>
    Accidental,             // <accidental>
    TimeModification,       // <time-modification>
    Stem,                   // <stem>
    Notehead,               // <notehead>
    NoteheadText,           // <notehead-text>
    int,                    // <staff>
    Beam,                   // <beam>
    Notations,              // <notations>
    Lyric,                  // <lyric>
    Play                    // <play>
>;

/// Notes are the most common type of MusicXML data [...]
class Note
{
public:
    // Attributes — same conventions as §8.
    std::optional<YesNo>              printLeger;
    std::optional<NonNegativeDecimal> dynamics;
    std::optional<NonNegativeDecimal> endDynamics;
    std::optional<Divisions>          attack;
    std::optional<Divisions>          release;
    std::optional<TimeOnly>           timeOnly;
    std::optional<YesNo>              pizzicato;
    std::optional<Tenths>             defaultX;
    std::optional<Tenths>             defaultY;
    std::optional<Tenths>             relativeX;
    std::optional<Tenths>             relativeY;
    std::optional<CommaSeparatedText> fontFamily;
    std::optional<FontStyle>          fontStyle;
    std::optional<FontSize>           fontSize;
    std::optional<FontWeight>         fontWeight;
    std::optional<Color>              color;
    std::optional<YesNo>              printDot;
    std::optional<YesNo>              printLyric;
    std::optional<YesNo>              printObject;
    std::optional<YesNo>              printSpacing;
    std::optional<std::string>        id;

    // Children in document order. Each entry is exactly one alternative by
    // construction (it is a std::variant); zero/multiple is unrepresentable.
    std::vector<NoteChild> children;
};

void parseNote(const ezxml::XElement& el, Note& out);
void serializeNote(const Note& v, ezxml::XElement& parent, const std::string& tag);

} // namespace mx::core
```

**Why `std::vector<std::variant<...>>` and not the Go/C "struct of typed pointers" pattern?** The
Go and C encoding (`struct NoteChild { *Pitch pitch; *Unpitched unpitched; ...; }`) exists because
those languages have no sum type; the pointer field IS the discriminator. The cost is real:
"zero or multiple fields set is undefined" — a contract enforced only by docs. In C++,
`std::variant<Pitch, Unpitched, ...>` makes the same guarantee at compile time. The collision
hazard the Go/C round-3 note flags ("harmony has a child element literally named `kind`, so any
synthetic field can collide") evaporates: there is no synthetic field, and there is no `Kind`
enum to collide with.

**The variant has duplicate alternatives** (`Empty` appears twice: `<chord>` and `<cue>`). That is
how `std::variant` is *meant* to handle this — alternatives are positional, not nominal — but the
generator must use **the index, not the type**, to dispatch parse/serialize. The generated
`parseNote` writes `out.children.emplace_back(std::in_place_index<6>, parseEmpty(c))` for `<cue>`
to disambiguate the two `Empty`s. This is a known C++ idiom; doing it correctly is one line of
generator logic per duplicated alternative. Worked in the dispatch helper below.

**The `using ...Child = std::variant<...>` is per type, in the type's own header.** Templates need
no helper file; visitors are written inline against the alias. Naming the alternatives lives in
documentation comments on the variant declaration and in the parse/serialize code, where it is
load-bearing.

### 9c. Dispatch: parse and serialize for choice content

The generated parse builds the variant directly from the tag name; the generated serialize
`std::visit`s with one lambda per alternative, indexed (so duplicate types route correctly to
their own tag names).

```cpp
// Note.cpp  (sketch — only the children dispatch shown)
void parseNote(const ezxml::XElement& el, Note& out)
{
    // ... attribute loop (omitted, identical in shape to AccidentalText) ...

    for (auto it = el.begin(); it != el.end(); ++it)
    {
        const auto child = *it;
        const std::string& tag = child->getName();
        if (tag == "grace")
        {
            Grace v; parseGrace(*child, v);
            out.children.emplace_back(std::in_place_index<0>, std::move(v));
        }
        else if (tag == "chord")
        {
            // <chord> is presence-only -> Empty (which is bool); presence is true.
            out.children.emplace_back(std::in_place_index<1>, true);
        }
        else if (tag == "pitch")
        {
            Pitch v; parsePitch(*child, v);
            out.children.emplace_back(std::in_place_index<2>, std::move(v));
        }
        else if (tag == "cue")
        {
            // Same Empty type as <chord>; index disambiguates.
            out.children.emplace_back(std::in_place_index<6>, true);
        }
        else if (tag == "voice")
        {
            out.children.emplace_back(std::in_place_index<11>, child->getValue());
        }
        // ... rest of the cases ...
        else
        {
            throw std::runtime_error("unknown element <" + tag + "> in <note>");
        }
    }
}

void serializeNote(const Note& v, ezxml::XElement& parent, const std::string& tag)
{
    auto el = parent.appendChild(tag);
    // ... attribute writes (omitted) ...

    for (const auto& c : v.children)
    {
        switch (c.index())
        {
        case 0: serializeGrace(std::get<0>(c), *el, "grace"); break;
        case 1: el->appendChild("chord"); break;        // presence-only
        case 2: serializePitch(std::get<2>(c), *el, "pitch"); break;
        // ... down through index 23 ...
        case 6: el->appendChild("cue"); break;          // presence-only, same Empty type
        case 11: { auto* v = el->appendChild("voice"); v->setValue(std::get<11>(c)); break; }
        // ...
        }
    }
}
```

**Why a `switch` on `index()` rather than `std::visit`?** Two reasons. (1) The tag name is
*per-alternative*, not per-type, and `std::visit` dispatches on type — duplicate alternatives
(`<chord>` and `<cue>` both `Empty`) can't be told apart by visit. (2) The `switch` on
`std::variant::index()` is a jump table and is friendlier to the optimizer than the nested-
ternary expansion `std::visit` produces for ~24 alternatives. Both reasons are real; either alone
would be enough.

---

## 10. Complex: derived (real inheritance)

The `gen/cpp/config.toml` does not yet declare `[target] inheritance` (the C target sets `false`).
The C++ default is `true` — derive types extend their base, exactly mirroring the IR's
`complexContent extension` shape. The plates already expose both `base` and `all_members` so the
choice is config, not template logic.

```cpp
// Mordent.h
#pragma once

#include "mx/core/AboveBelow.h"
#include "mx/core/EmptyTrillSound.h"  // base class
#include "mx/core/YesNo.h"

#include <optional>

namespace ezxml { class XElement; }

namespace mx::core
{

/// The mordent type is used for both [...] The long attribute is "no" by
/// default. The approach and departure attributes are used for compound
/// ornaments [...]
class Mordent : public EmptyTrillSound
{
public:
    std::optional<YesNo>      mordentLong;   // attribute "long"  (renamed: "long" is reserved)
    std::optional<AboveBelow> approach;
    std::optional<AboveBelow> departure;
};

bool operator==(const Mordent& a, const Mordent& b);
bool operator!=(const Mordent& a, const Mordent& b);

void parseMordent(const ezxml::XElement& el, Mordent& out);
void serializeMordent(const Mordent& v, ezxml::XElement& parent, const std::string& tag);

} // namespace mx::core
```

**Public inheritance, not private.** This is the schema's `complexContent extension` — a
genuine "is-a" relationship, not implementation reuse. A `Mordent` IS-AN `EmptyTrillSound` and
the API contract honors it: a function taking an `EmptyTrillSound&` reads a `Mordent`'s base
attributes correctly.

**No virtual functions, no virtual destructor.** These are data classes, not polymorphic ones.
There is exactly one place in the schema (the document root, §11) that needs run-time
discrimination, and that one is handled by `std::variant`, not by virtual dispatch. Nothing else
in mx::core calls a function through a base pointer; the base class exists to share fields and
the IS-A relation, not to dispatch. Adding `virtual` would impose a vtable on every leaf type
(228 of them) for a feature nothing uses. (If a future use case appeared, adding `virtual` to the
base class is a one-edit, base-only change — much smaller blast radius than the inverse.)

**Slicing is fine here.** Without virtual functions there is no slicing problem: copying a
`Mordent` to an `EmptyTrillSound` simply produces an `EmptyTrillSound` with the right fields,
which is the correct behavior.

**`mordentLong` for the `long` attribute** is the `[reserved] words = ["long", ...]` rename
landing as a Plates-side identifier choice, surfaced through `rename.attribute.mordent.long =
"mordent-long"` in `gen/cpp/config.toml`. The wire form stays `long`; only the C++ identifier
mangles. This is the existing Plates §6.2 mechanism, used as designed.

**Parse/serialize delegate to the base.** `parseMordent` calls `parseEmptyTrillSound(el, out)`
first to populate the inherited attributes, then handles the three new ones. `serializeMordent`
calls `serializeEmptyTrillSound`'s body inline (the base's serializer takes a `parent` and a
`tag`, so we cannot reuse it directly without writing the same element twice; the generator
emits the merged attribute list in the derived `.cpp` rather than chaining serialize calls).

---

## 11. The document and its roots

`Document` is the entry point. Two roots (`score-partwise`, `score-timewise`); exactly one is
present. `std::variant` again.

```cpp
// Document.h
#pragma once

#include "mx/core/ScorePartwise.h"
#include "mx/core/ScoreTimewise.h"

#include <string>
#include <variant>
#include <vector>

namespace ezxml { class XDoc; }

namespace mx::core
{

/// An attribute on the document root that is outside the schema; in
/// practice this is the namespace declarations (xmlns, xmlns:xlink).
/// Round-trip parity requires preserving them verbatim.
struct ExtraAttr
{
    std::string key;
    std::string value;
};

/// A parsed MusicXML document. Exactly one root variant is held.
class Document
{
public:
    using Root = std::variant<ScorePartwise, ScoreTimewise>;

    Root                   root;
    std::vector<ExtraAttr> rootNamespaces;
};

/// Parse an ezxml document into the typed model. Throws std::runtime_error
/// on a structural error (no root, unknown root tag, unknown attribute or
/// element name); lenient about values per the round-3 contract.
Document fromXDoc(const ezxml::XDoc& doc);

/// Serialize the typed model back to an ezxml document. The returned doc
/// is freshly allocated by ezxml::XFactory; ownership is the caller's.
ezxml::XDocPtr toXDoc(const Document& d);

} // namespace mx::core
```

This is the only place in the API where `std::variant` shows up at the document level. The
discriminant is the schema (one root or the other; never both, never neither) so the variant is
the natural type.

**`fromXDoc` returns by value.** RVO/NRVO and move semantics make this free; a `unique_ptr<
Document>` would be ceremony with no payoff (the caller can put the value in a
`unique_ptr` if they want one).

**`toXDoc` returns `ezxml::XDocPtr`.** That is ezxml's existing ownership type (a `shared_ptr<
XDoc>`). We do not invent a new ownership shape just because we'd prefer a `unique_ptr`; we use
the layer's contract.

---

## 12. Parse and serialize: the API shape

Pattern, applied uniformly:

```cpp
// For every complex type T:
void parse<T>(const ezxml::XElement& el, T& out);                       // throws on structural error
void serialize<T>(const T& v, ezxml::XElement& parent, const std::string& tag);

// For every value type V:
void   toString(const V& v, std::string& out);     // append to `out`; never throws
const char* toString(EnumValue v);                 // for plain enums; static storage
bool   tryParse(const char* s, V* out);            // strict; returns false on bad input
V      parse<V>(const char* s);                    // lenient: bad input -> first variant / 0 / clamp
```

**Why an out-parameter for parse rather than a return?** Parse is the inner loop of the round-
trip; the generated bodies call it ~24 times in `parseNote` alone. Out-parameter avoids one move
per call and lets the caller `emplace` directly into a `std::vector<std::variant<...>>` slot.
This is a hot path (see `gen/README.md` "the corert test ... ~1,347 files"); we measured it in
the C/Go ports' equivalents, and the generated body is shaped to match.

**Why free functions, not member functions?** Because the data classes are aggregates (§3, §8).
Adding `parse` / `serialize` member functions would force the classes to be non-aggregates and
prevent designated-initializer construction. The cost is exactly one symbol per type; the benefit
is keeping the data layer pure.

**Why `const char*` for parse input rather than `std::string_view`?** Most call sites have a
`std::string` in hand from ezxml (`getValue() const` returns `std::string`); both `const char*`
and `string_view` accept that. `const char*` matches what Go's templated free functions look like
when ported and the existing parse/clamp helpers use it. (`string_view` would be equally fine; it
is a preference, not a load-bearing choice. Lock either one and stay consistent.)

**`throw std::runtime_error` for structural parse failures.** The corert harness already
`try { ... } catch(std::exception& e) { result.message = e.what(); }`; matching that contract
keeps the harness untouched. The lenient-on-values rule means the throws are rare and limited to
unknown names + missing root, which the harness wants to know about.

---

## 13. Optionality and collections in detail

| IR cardinality | C++ representation | Why |
|---|---|---|
| required (1)   | `T value;`                     | DAG -> always safe by value |
| optional (0..1)| `std::optional<T> value;`      | C++17 native; value-typed; clear engaged/disengaged |
| vector (0..n)  | `std::vector<T> values;`       | C++17 native; contiguous; supports the choice-vector pattern |

Special cases:

- `std::vector<std::variant<...>>` for the children of choice-bearing composites (§9b).
- `std::variant<...>` for a value-type union (§6).
- Required attributes are still `std::optional` if `corert` parity demands it. The plates
  round-3 note is explicit: "required attributes included, because the corert contract is 'write
  back exactly what was parsed' and corpus files do omit required attributes." The C++ target
  inherits this rule. `Note::id` and other required-but-omitted-in-corpus attributes use
  `std::optional`; the API does not pretend they are guaranteed present when the data shows they
  are not. This is the same trade-off Go made (`*string` for required attributes); we just have a
  better wrapper for it.

---

## 14. Hand-written runtime

Three files, hand-written, live alongside the generated code:

### 14a. `Decimal.h` / `Decimal.cpp`

The lossless decimal class. Already an `mx::core` decision per `gen/cpp/config.toml` `[types]
decimal = "Decimal"`. Sketch of API:

```cpp
namespace mx::core
{

class Decimal
{
public:
    Decimal();
    explicit Decimal(double v);
    explicit Decimal(int v);
    explicit Decimal(const std::string& s);   // parse exact text; lossless

    double getValue() const;                  // numeric face for arithmetic / clamps
    const std::string& getText() const;       // wire face: lossless reproduction

    void setValue(double v);                  // re-renders text using the configured policy
    void setText(const std::string& s);

    bool operator==(const Decimal& other) const;
    bool operator!=(const Decimal& other) const;
    // Arithmetic intentionally absent: a typed model is not a calculator.
};

} // namespace mx::core
```

The trailing-zero handling that `DecimalFields.h` performs at the corert level is the *test*
side of the same coin: `Decimal` is the *production* side (it preserves the input text), and the
corert normalizer strips trailing zeros from both sides for comparison, exactly as today.

### 14b. `Runtime.h` / `Runtime.cpp`

The version constant (per round-3, generated from the schema stem so retargeting cannot leave it
stale) plus the small numeric/string parse helpers the generated code calls.

```cpp
namespace mx::core
{

inline constexpr const char* SupportedMusicXMLVersion = "4.0";

bool   tryParseInt(const char* s, int* out);
int    parseInt(const char* s);              // lenient: 0 on failure
bool   tryParseDecimal(const char* s, Decimal* out);
Decimal parseDecimal(const char* s);

} // namespace mx::core
```

`SupportedMusicXMLVersion` is **generated** into Runtime.h, despite Runtime.h being hand-written
elsewhere — or, more cleanly, the generator emits a separate `SupportedVersion.h` with the
constant and Runtime.h includes it. (Either works; pick the one that keeps the "regen safe"
invariant: any file the generator might rewrite must contain only generated content.)

### 14c. The existing `mx/ezxml/` layer is the dependency

The C++ target builds **on** the existing ezxml DOM (XDoc, XElement, XAttribute). It does not
parse XML itself. The corert harness already does this for the file load/save; the generated code
walks ezxml elements. This matches the Go (etree) and C (libxml2) targets; ezxml is C++'s
counterpart.

---

## 15. CMake integration

The press's `[render]` manifest emits a `sources.cmake` (the C target already does this) listing
the generated `.cpp` files. The repository `CMakeLists.txt` adds the generated directory and
`include()`s the manifest:

```cmake
# CMakeLists.txt — sketch of the mx::core target
add_library(mx_core STATIC
    src/private/mx/core/Decimal.cpp
    src/private/mx/core/Runtime.cpp
)
include(src/private/mx/core/sources.cmake)   # appends generated .cpp files
target_sources(mx_core PRIVATE ${MX_CORE_GENERATED_SOURCES})
target_include_directories(mx_core PUBLIC src/private)
target_link_libraries(mx_core PUBLIC ezxml)
target_compile_features(mx_core PUBLIC cxx_std_17)
```

Generated `sources.cmake` writes a single `set(MX_CORE_GENERATED_SOURCES ...)` so the build file
remains hand-managed for everything except the per-type list. Same shape as the C target's
`sources.cmake` template.

---

## 16. What to NOT do (and why)

These are dead-ends a reviewer might independently propose; the design has rejected each.

- **`std::shared_ptr` / `std::unique_ptr` everywhere.** The DAG invariant means by-value works, so
  heap indirection costs allocation and locality for nothing. Reach for a smart pointer only when
  the schema invariant changes — and then the choice is the architecture review of the day, not a
  default.
- **Virtual destructors / runtime polymorphism.** Nothing in the schema needs it; it imposes a
  vtable on every type for one notional use case. See §10.
- **Hand-rolled tagged unions.** `std::variant` exists. See §6, §9b.
- **A synthetic `Kind` enum on each composite's child variant.** The pointer-struct pattern in
  Go/C needs it; the variant pattern does not. Adding one would re-introduce the harmony-`<kind>`
  collision the variant solution avoids by construction.
- **Member function `parse` / `serialize`.** Breaks aggregate-ness; bloats every class with two
  symbols that are inherently I/O. Free functions, see §12.
- **Strong typedef via `using`.** Loses type identity at the language level. Class with private
  storage instead, §4-§5.
- **Reflection / Boost.Hana / a registry-of-types.** The generator already has the IR; we do not
  need C++ to discover at runtime what we know at build time. Templates emit straight-line C++
  from straight-line plate data.
- **A `Visitor` base class for tree walks.** Users who want a visitor write one with `std::visit`
  and a member-pointer table; we do not generate one. Nothing in the corert harness or the public
  API needs it, and a generated visitor base would be a wide, shallow interface to a deep tree —
  the kind of thing the architect-review skill explicitly flags.
- **Coroutines, ranges, `std::expected`.** All would be defensible in a greenfield 2026 design;
  none are reachable from C++17, which the rest of `src/private` is on. Holding the line on the
  language baseline keeps blast radius local.

---

## 17. What this design intentionally leaves open

A short list, with the reason each is deferred:

- **Header/impl split granularity.** §2 says one type per pair. The plates already support a
  per-type file; making one type span "header-only inline" plus "out-of-line cpp" is a future
  partition setting (`plates.md` §10 lists this as future work). For now: every type gets a
  `.cpp`, even the ones whose parse/serialize is short, so the header dependency is identical for
  every consumer.
- **`operator==` for variants whose alternatives are duplicated types.** `std::variant`'s default
  `==` works on (index, value) pairs, which is what we want. No generator action required, but
  worth a unit test on (e.g.) `NoteChild{std::in_place_index<1>, true}` !=
  `NoteChild{std::in_place_index<6>, true}` to confirm the index discriminates as expected.
- **`std::hash` / set/map keys.** Not generated. A future need can add it as a one-shape template
  edit; nothing depends on it today.
- **Streaming serialize.** `serialize` emits to ezxml's DOM, which then writes to a stream. A
  direct streaming serializer (skip the DOM) is possible and may be a follow-up if benchmarks
  show the DOM allocation hurts; the current shape is the same as the C++ Reference
  implementation has used and is fast enough for the corert workload.
- **The `Decimal` class's exact API.** §14a is a sketch; the precise trailing-zero policy and
  whether to surface arithmetic operators is an open call. Whatever lands must round-trip the
  decimal-fields corpus tests under `mxtest/import/` byte-for-byte.

---

## 18. Architecture review against `arch-review`

A quick self-audit against the principles the review skill applies, in priority order:

**Domain boundaries.** The data layer (mx::core types) is decoupled from the I/O layer (ezxml +
free-function parse/serialize). The hand-written runtime (`Decimal`, version constant, parse
helpers) is the smallest possible *seam* — three files. Adding a JSON serializer is "write
parseFooFromJson / serializeFooToJson," not "rewrite Note." Boundary holds.

**Simple, deep abstractions.** The interface of every generated type is small (fields + free
parse/serialize). The internal functionality of, say, `NumberPlate` clamping or the variant-
index dispatch is rich. Wide-and-shallow would be a typed model that re-exposes XSD facets at
runtime; we do not.

**Blast radius.** A schema bump regenerates files; no hand-written code changes. Adding an
attribute to one element changes one `.h`/`.cpp` pair. The biggest hand-written surface is
`Decimal`, which is ~2 files; if we replaced it tomorrow, only the `[types]` mapping moves.

**Clarity.** `enum class`, `std::optional`, `std::variant`, `std::vector` are exactly what every
C++17 reader expects. Naming follows the existing `src/private` convention (PascalCase types,
camelCase fields, `parseFoo` free functions matching `mxtest/import/`). No surprises.

The one concession the design makes is the pure-sequence vs choice-bearing split (§9a vs §9b).
That's a real bifurcation in the generated code shape, but it is *visible from the plate*
(`ComplexPlate.content` is enough to choose), it is *generate-by-shape* (one template branch,
not per-element specialization), and it directly serves clarity for the consumer (`Pitch::octave`
reads better than `pitch.children[2]`). It earns its complexity.
