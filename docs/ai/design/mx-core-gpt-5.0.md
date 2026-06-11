# C++ `mx/core` design

Status: design proposal for the generated C++20 `mx::core` product.

The Go and C targets proved the generator pipeline: XSD -> IR -> Plates -> target-owned Mustache
works, the press is language-agnostic, and the corert flow can be made green from generated code.
They also proved what the C++ target must **not** copy. Go and C are deliberately pragmatic test
bindings: they preserve document order with an ordered child list, parse values leniently, and
expose plain data. The C++ target is the real product and should be a stricter, deeper abstraction.

## Position

Generate a valid-by-construction, order-faithful MusicXML model.

The public C++ API should make invalid MusicXML states unreachable through normal API use:

- no public mutable fields;
- no public default constructors for model objects whose schema content is required;
- required attributes and required children appear in constructors or `make` factories;
- optional schema members use `std::optional` behind accessors;
- repeated members are stored privately and exposed as read-only spans;
- schema choices use `std::variant`, not nullable sibling fields;
- XML parsing validates structure and required content before producing a model object;
- import leniency may clamp out-of-range numbers, but the object produced is still valid.

C++ cannot protect against undefined behavior, memory scribbles, malicious casts, or every possible
moved-from misuse. The contract is: the generated API does not provide an operation that creates an
invalid serializable model object.

## Lessons from Go and C

What to keep:

1. **One source of truth.** All schema facts must still come from the Plates. C++ is a target
   directory: `gen/cpp/config.toml` plus `gen/cpp/templates/`. Do not add C++ knowledge to
   `gen/*.py`.
2. **Order fidelity matters.** A MusicXML measure interleaves `note`, `backup`, `direction`,
   `attributes`, and other music-data elements. A flat field per element is not enough.
3. **Generated version constants matter.** The C++ runtime should emit `SupportedMusicXMLVersion`
   from `Plates.schema_version`, just like Go and C now do.
4. **Unknown names are errors.** With version gating, an unknown element or attribute is a generator
   or input problem, not data to preserve silently.
5. **Numeric clamping is an import policy.** The clamp steps now live on the Plates. C++ should use
   that data exactly once in its value parsers.

What not to keep:

1. **No multi-null child structs.** Go/C use `NoteChild{Pitch: &p}` because they lack a good sum
   type in the chosen style. C++ has `std::variant`; use it.
2. **No public structs as the product API.** Go/C are harness bindings. C++ should hide invariants
   behind small interfaces.
3. **No always-flat child list.** The C++ target should consume `ComplexPlate.content`, not just
   `elements`. Pure sequences should become sequence-shaped classes; repeated choices should become
   ordered variants.
4. **No enum fallback to the first value for user construction.** Unknown enum input should fail.
   Clamping numbers can be compatible with a valid model; inventing enum values is not.

## C++ feature choices

| Feature                      | Use                                                                 | Rule                                                                                |
|------------------------------|---------------------------------------------------------------------|-------------------------------------------------------------------------------------|
| C++20                        | Target language level                                               | Raise CMake to C++20 when this target lands.                                        |
| `class` with private members | Generated model types                                               | Default for all complex and constrained value types.                                |
| `enum class`                 | Internal tags                                                       | Do not expose raw enums as constructible domain values. Wrap them in value classes. |
| `std::variant`               | XML choices, unions, document root                                  | Exactly-one by construction.                                                        |
| `std::optional`              | Optional attributes/elements and defaulted/fixed attribute presence | Never for required schema content.                                                  |
| `std::vector`                | Repeated homogeneous content and repeated choice groups             | Kept private; expose `std::span<const T>`.                                          |
| `std::unique_ptr`            | Alternatives inside repeated heterogeneous choice vectors           | Avoids a huge `std::variant<LargeType...>` object per child.                        |
| `std::span`                  | Read-only views of repeated content                                 | No mutable vector exposure.                                                         |
| `std::string_view`           | Parse/format APIs and wire names                                    | Avoids unnecessary string copies at API boundaries.                                 |
| `constexpr std::array`       | Wire literal tables                                                 | No dynamic enum maps unless profiling proves need.                                  |
| `std::from_chars`            | Numeric lexical parsing                                             | Strict, locale-free parsing.                                                        |
| non-polymorphic inheritance  | Only for IR `derived` complex types                                 | Schema extension only; no virtual model hierarchy.                                  |
| `Result<T>` runtime type     | Parse/factory errors                                                | C++20 has no `std::expected`; provide a small target runtime `Result`.              |

Avoid:

- raw owning pointers;
- `new` / `delete` in generated code;
- public mutable containers;
- runtime polymorphism for every element;
- `std::shared_ptr` by default;
- `std::regex` in headers;
- template-heavy generated code.

## Output layout

Recommended generated layout under `src/private/mx/core/`:

```text
mx/core/
  Document.h / Document.cpp
  Result.h
  Error.h
  Decimal.h / Decimal.cpp
  Xml.h / Xml.cpp
  UpDown.h / UpDown.cpp
  FontSize.h / FontSize.cpp
  Pitch.h / Pitch.cpp
  PartwiseMeasure.h / PartwiseMeasure.cpp
  ... one header/source pair per generated type ...
  Sources.cmake
```

Headers should contain type declarations, small accessors, and inline trivial factories. Parsing,
serialization, string tables, pattern checks, and visitors should live in `.cpp` files. Compile time
is the first optimization priority; do not dump large parse functions into headers.

## Error and result shape

The runtime support can define generic infrastructure, but generated code should use concrete result
spelling. Rendered code should look like this at call sites:

```cpp
namespace mx::core
{

enum class ErrorCode
{
    unknownElement,
    unknownAttribute,
    missingRequiredElement,
    missingRequiredAttribute,
    wrongElementOrder,
    tooManyElements,
    invalidValue,
    unsupportedVersion,
};

struct Error
{
    ErrorCode code;
    std::string path;
    std::string message;
};

Result<Pitch> parsePitch(const ezxml::XElement &element);
Result<Document> parseDocument(const ezxml::XDoc &doc);

} // namespace mx::core
```

The generated functions should accumulate path context
(`/score-partwise/part[0]/measure[2]/note[1]`) so corert failures are diagnosable. The old
`bool fromXDoc(std::ostream&, ...)` shape can be kept as a thin adapter; it should not be the core
API.

## Value type shapes

### Closed enum values: wrapper class, not a raw enum

A raw C++ enum can be fabricated with `static_cast`. Use a small value class with a private tag
constructor. Rendered `up-down` should look like this:

```cpp
#pragma once

#include "mx/core/Result.h"

#include <cstdint>
#include <string_view>

namespace mx::core
{

class UpDown final
{
  public:
    enum class Tag : std::uint8_t
    {
        up,
        down,
    };

    static constexpr UpDown up() noexcept { return UpDown(Tag::up); }
    static constexpr UpDown down() noexcept { return UpDown(Tag::down); }

    static Result<UpDown> parse(std::string_view text);

    constexpr Tag tag() const noexcept { return tag_; }
    constexpr bool isUp() const noexcept { return tag_ == Tag::up; }
    constexpr bool isDown() const noexcept { return tag_ == Tag::down; }

    std::string_view wire() const noexcept;

  private:
    explicit constexpr UpDown(Tag tag) noexcept : tag_(tag) {}

    Tag tag_;
};

} // namespace mx::core
```

The `.cpp` keeps the wire table out of dependent translation units:

```cpp
#include "mx/core/UpDown.h"

#include <array>

namespace mx::core
{
namespace
{
constexpr std::array<std::string_view, 2> kWire = {"up", "down"};
}

Result<UpDown> UpDown::parse(std::string_view text)
{
    if (text == "up")
    {
        return UpDown::up();
    }
    if (text == "down")
    {
        return UpDown::down();
    }
    return Error{ErrorCode::invalidValue, {}, "invalid up-down value"};
}

std::string_view UpDown::wire() const noexcept
{
    return kWire[static_cast<std::size_t>(tag_)];
}

} // namespace mx::core
```

### Numeric wrappers: strict construction, explicit import clamping

The Plates already carry clamp policy. Use it for XML import, not for ordinary construction.
Rendered `midi-channel` should look like this:

```cpp
#pragma once

#include "mx/core/Result.h"

#include <string_view>

namespace mx::core
{

class MIDIChannel final
{
  public:
    static Result<MIDIChannel> make(int value);
    static Result<MIDIChannel> parse(std::string_view text);
    static MIDIChannel importClamp(int value) noexcept;

    constexpr int value() const noexcept { return value_; }
    std::string toString() const;

  private:
    explicit constexpr MIDIChannel(int value) noexcept : value_(value) {}

    int value_;
};

} // namespace mx::core
```

```cpp
#include "mx/core/MIDIChannel.h"
#include "mx/core/NumberParse.h"

namespace mx::core
{

Result<MIDIChannel> MIDIChannel::make(int value)
{
    if (value < 1 || value > 16)
    {
        return Error{ErrorCode::invalidValue, {}, "midi-channel must be in [1, 16]"};
    }
    return MIDIChannel(value);
}

MIDIChannel MIDIChannel::importClamp(int value) noexcept
{
    if (value < 1)
    {
        value = 1;
    }
    if (value > 16)
    {
        value = 16;
    }
    return MIDIChannel(value);
}

Result<MIDIChannel> MIDIChannel::parse(std::string_view text)
{
    const auto parsed = parseInteger(text);
    if (!parsed)
    {
        return parsed.error();
    }
    return MIDIChannel::make(parsed.value());
}

std::string MIDIChannel::toString() const
{
    return formatInteger(value_);
}

} // namespace mx::core
```

The XML parser can call `importClamp` when the target chooses compatibility import. User code gets
`make`, which rejects invalid values.

### Strings: domain wrappers own validation

String wrappers should not be aliases. They are the only place length and pattern constraints can be
enforced.

```cpp
#pragma once

#include "mx/core/Result.h"

#include <string>
#include <string_view>

namespace mx::core
{

class Color final
{
  public:
    static Result<Color> make(std::string value);
    static Result<Color> parse(std::string_view text);

    const std::string &value() const noexcept { return value_; }
    std::string_view wire() const noexcept { return value_; }

  private:
    explicit Color(std::string value) : value_(std::move(value)) {}

    std::string value_;
};

} // namespace mx::core
```

Pattern validation belongs in `.cpp`. If the XSD regex subset needs custom handling (`\c` appears in
MusicXML), hide it in `XmlPattern.cpp`; do not generate ad-hoc regex logic into every header.

### Unions: `std::variant`

Rendered `font-size` should be a tiny sum type. This is strictly better than Go/C's hand-rolled
`Kind` plus fields because `std::variant` cannot hold two members at once.

```cpp
#pragma once

#include "mx/core/CSSFontSize.h"
#include "mx/core/Decimal.h"
#include "mx/core/Result.h"

#include <string>
#include <string_view>
#include <variant>

namespace mx::core
{

class FontSize final
{
  public:
    using Value = std::variant<Decimal, CSSFontSize>;

    static FontSize points(Decimal value) { return FontSize(Value{value}); }
    static FontSize css(CSSFontSize value) { return FontSize(Value{value}); }
    static Result<FontSize> parse(std::string_view text);

    bool isPoints() const noexcept { return std::holds_alternative<Decimal>(value_); }
    bool isCSS() const noexcept { return std::holds_alternative<CSSFontSize>(value_); }

    const Value &value() const noexcept { return value_; }
    std::string toString() const;

  private:
    explicit FontSize(Value value) : value_(std::move(value)) {}

    Value value_;
};

} // namespace mx::core
```

```cpp
Result<FontSize> FontSize::parse(std::string_view text)
{
    if (auto decimal = Decimal::parse(text))
    {
        return FontSize::points(decimal.value());
    }
    if (auto css = CSSFontSize::parse(text))
    {
        return FontSize::css(css.value());
    }
    return Error{ErrorCode::invalidValue, {}, "invalid font-size"};
}

std::string FontSize::toString() const
{
    return std::visit(
        [](const auto &member) { return member.toString(); },
        value_);
}
```

Open string unions, such as `instrument-sound` after the sounds companion fold, should put the open
string alternative last in parsing, matching the Plates rule.

## Complex type shapes

### Pure sequence: fields, not a child vector

`pitch` is a sequence: `step`, optional `alter`, `octave`. It should not become an ordered vector of
three alternatives. Generate the shape the schema says.

```cpp
#pragma once

#include "mx/core/Octave.h"
#include "mx/core/Result.h"
#include "mx/core/Semitones.h"
#include "mx/core/Step.h"

#include <optional>

namespace mx::core
{

class Pitch final
{
  public:
    Pitch(Step step, Octave octave)
        : step_(step), octave_(octave)
    {
    }

    static Result<Pitch> make(Step step, std::optional<Semitones> alter, Octave octave);
    static Result<Pitch> parseXml(const ezxml::XElement &element);

    const Step &step() const noexcept { return step_; }
    const std::optional<Semitones> &alter() const noexcept { return alter_; }
    const Octave &octave() const noexcept { return octave_; }

    void setStep(Step value) noexcept { step_ = value; }
    void setAlter(std::optional<Semitones> value) { alter_ = std::move(value); }
    void setOctave(Octave value) noexcept { octave_ = value; }

    void writeXml(ezxml::XElement &parent, std::string_view tag) const;

  private:
    Step step_;
    std::optional<Semitones> alter_;
    Octave octave_;
};

} // namespace mx::core
```

A strict parser for `pitch` should be a small generated state machine:

```cpp
Result<Pitch> Pitch::parseXml(const ezxml::XElement &element)
{
    std::optional<Step> step;
    std::optional<Semitones> alter;
    std::optional<Octave> octave;

    enum class State
    {
        start,
        afterStep,
        afterAlter,
        afterOctave,
    };

    State state = State::start;

    for (auto it = element.begin(); it != element.end(); ++it)
    {
        const auto child = *it;
        const auto name = child->getName();

        if (name == "step" && state == State::start)
        {
            auto parsed = Step::parse(child->getValue());
            if (!parsed)
            {
                return parsed.error();
            }
            step = parsed.value();
            state = State::afterStep;
        }
        else if (name == "alter" && state == State::afterStep)
        {
            auto parsed = Semitones::parse(child->getValue());
            if (!parsed)
            {
                return parsed.error();
            }
            alter = parsed.value();
            state = State::afterAlter;
        }
        else if (name == "octave" && (state == State::afterStep || state == State::afterAlter))
        {
            auto parsed = Octave::parse(child->getValue());
            if (!parsed)
            {
                return parsed.error();
            }
            octave = parsed.value();
            state = State::afterOctave;
        }
        else
        {
            return Error{ErrorCode::wrongElementOrder, {}, "invalid child in pitch"};
        }
    }

    if (!step)
    {
        return Error{ErrorCode::missingRequiredElement, {}, "pitch is missing step"};
    }
    if (!octave)
    {
        return Error{ErrorCode::missingRequiredElement, {}, "pitch is missing octave"};
    }

    return Pitch::make(step.value(), alter, octave.value());
}
```

This is the key difference from Go/C: C++ should consume the content tree and enforce sequence
order.

### Repeated homogeneous members: private vector

For repeated elements like `beam` in `note`, use private vectors and bounded append APIs.

```cpp
class Note final
{
  public:
    std::span<const Beam> beams() const noexcept { return beams_; }

    Result<void> addBeam(Beam beam)
    {
        if (beams_.size() == 8)
        {
            return Error{ErrorCode::tooManyElements, {}, "note has too many beam elements"};
        }
        beams_.push_back(std::move(beam));
        return {};
    }

  private:
    std::vector<Beam> beams_;
};
```

If `maxOccurs` is unbounded, `addX` cannot fail for cardinality. If `minOccurs` is at least one, the
constructor or factory takes the first value and the vector never exposes `clear`.

### Repeated heterogeneous choices: boxed variants

For measure music-data, order is semantically important and alternatives have very different sizes.
Do not use `std::variant<Note, Backup, Forward, ...>` directly in a vector: every vector slot would
be as large as the largest alternative. Use `std::variant<std::unique_ptr<Note>, ...>` and keep the
container private.

Rendered `partwise-measure` should look like this:

```cpp
#pragma once

#include "mx/core/Attributes.h"
#include "mx/core/Backup.h"
#include "mx/core/Barline.h"
#include "mx/core/Bookmark.h"
#include "mx/core/Direction.h"
#include "mx/core/FiguredBass.h"
#include "mx/core/Forward.h"
#include "mx/core/Grouping.h"
#include "mx/core/Harmony.h"
#include "mx/core/Link.h"
#include "mx/core/MeasureText.h"
#include "mx/core/Note.h"
#include "mx/core/Print.h"
#include "mx/core/Result.h"
#include "mx/core/Sound.h"
#include "mx/core/Tenths.h"
#include "mx/core/YesNo.h"

#include <memory>
#include <optional>
#include <span>
#include <string>
#include <variant>
#include <vector>

namespace mx::core
{

class PartwiseMeasure final
{
  public:
    using MusicData = std::variant<
        std::unique_ptr<Note>,
        std::unique_ptr<Backup>,
        std::unique_ptr<Forward>,
        std::unique_ptr<Direction>,
        std::unique_ptr<Attributes>,
        std::unique_ptr<Harmony>,
        std::unique_ptr<FiguredBass>,
        std::unique_ptr<Print>,
        std::unique_ptr<Sound>,
        std::unique_ptr<Barline>,
        std::unique_ptr<Grouping>,
        std::unique_ptr<Link>,
        std::unique_ptr<Bookmark>>;

    explicit PartwiseMeasure(std::string number)
        : number_(std::move(number))
    {
    }

    PartwiseMeasure(const PartwiseMeasure &other);
    PartwiseMeasure &operator=(const PartwiseMeasure &other);
    PartwiseMeasure(PartwiseMeasure &&) noexcept = default;
    PartwiseMeasure &operator=(PartwiseMeasure &&) noexcept = default;

    const std::string &number() const noexcept { return number_; }
    void setNumber(std::string value) { number_ = std::move(value); }

    const std::optional<MeasureText> &text() const noexcept { return text_; }
    void setText(std::optional<MeasureText> value) { text_ = std::move(value); }

    std::span<const MusicData> musicData() const noexcept { return music_data_; }

    void appendNote(Note note) { music_data_.push_back(std::make_unique<Note>(std::move(note))); }
    void appendBackup(Backup backup) { music_data_.push_back(std::make_unique<Backup>(std::move(backup))); }
    void appendForward(Forward forward) { music_data_.push_back(std::make_unique<Forward>(std::move(forward))); }
    void appendDirection(Direction direction) { music_data_.push_back(std::make_unique<Direction>(std::move(direction))); }
    void appendAttributes(Attributes attributes) { music_data_.push_back(std::make_unique<Attributes>(std::move(attributes))); }
    void appendHarmony(Harmony harmony) { music_data_.push_back(std::make_unique<Harmony>(std::move(harmony))); }
    void appendFiguredBass(FiguredBass figuredBass) { music_data_.push_back(std::make_unique<FiguredBass>(std::move(figuredBass))); }
    void appendPrint(Print print) { music_data_.push_back(std::make_unique<Print>(std::move(print))); }
    void appendSound(Sound sound) { music_data_.push_back(std::make_unique<Sound>(std::move(sound))); }
    void appendBarline(Barline barline) { music_data_.push_back(std::make_unique<Barline>(std::move(barline))); }
    void appendGrouping(Grouping grouping) { music_data_.push_back(std::make_unique<Grouping>(std::move(grouping))); }
    void appendLink(Link link) { music_data_.push_back(std::make_unique<Link>(std::move(link))); }
    void appendBookmark(Bookmark bookmark) { music_data_.push_back(std::make_unique<Bookmark>(std::move(bookmark))); }

    static Result<PartwiseMeasure> parseXml(const ezxml::XElement &element);
    void writeXml(ezxml::XElement &parent, std::string_view tag) const;

  private:
    std::string number_;
    std::optional<MeasureText> text_;
    std::optional<YesNo> implicit_;
    std::optional<YesNo> non_controlling_;
    std::optional<Tenths> width_;
    std::optional<std::string> id_;
    std::vector<MusicData> music_data_;
};

} // namespace mx::core
```

The copy constructor clones the pointed alternatives. The public API still feels value-like, while
each child slot remains small.

Serialization dispatch is straightforward and safe:

```cpp
void PartwiseMeasure::writeXml(ezxml::XElement &parent, std::string_view tag) const
{
    auto element = parent.appendChild(std::string(tag));
    element->appendAttribute("number")->setValue(number_);

    if (text_)
    {
        element->appendAttribute("text")->setValue(text_->toString());
    }

    for (const auto &item : music_data_)
    {
        std::visit(
            [&](const auto &ptr)
            {
                using Pointer = std::decay_t<decltype(ptr)>;
                if constexpr (std::is_same_v<Pointer, std::unique_ptr<Note>>)
                {
                    ptr->writeXml(*element, "note");
                }
                else if constexpr (std::is_same_v<Pointer, std::unique_ptr<Backup>>)
                {
                    ptr->writeXml(*element, "backup");
                }
                else if constexpr (std::is_same_v<Pointer, std::unique_ptr<Forward>>)
                {
                    ptr->writeXml(*element, "forward");
                }
                else if constexpr (std::is_same_v<Pointer, std::unique_ptr<Direction>>)
                {
                    ptr->writeXml(*element, "direction");
                }
            },
            item);
    }
}
```

The rendered file would include all alternatives; the snippet is shortened only after `Direction`.
The shape is the important decision: boxed variant, private vector, typed append methods.

### Non-repeated choice: by-value variant

For one-of content that occurs once, prefer by-value alternatives. Example shape for the
`pitch | unpitched | rest` part of `note`:

```cpp
class Note final
{
  public:
    using FullNote = std::variant<Pitch, Unpitched, Rest>;

    static Result<Note> make(FullNote fullNote, std::optional<PositiveDivisions> duration);

    const FullNote &fullNote() const noexcept { return full_note_; }
    void setFullNote(FullNote value) { full_note_ = std::move(value); }

  private:
    FullNote full_note_;
    std::optional<PositiveDivisions> duration_;
};
```

This is compact and enforces exactly one full-note alternative.

### Attributes

Attributes should be separate from child content. Required attributes are stored by value. Optional,
defaulted, and fixed attributes store presence separately from effective value.

For defaulted attributes, preserve wire absence but provide an effective getter:

```cpp
class Barline final
{
  public:
    RightLeftMiddle location() const noexcept
    {
        return location_.value_or(RightLeftMiddle::right());
    }

    bool hasLocationAttribute() const noexcept { return location_.has_value(); }
    void setLocationAttribute(RightLeftMiddle value) { location_ = value; }
    void clearLocationAttribute() noexcept { location_.reset(); }

  private:
    std::optional<RightLeftMiddle> location_;
};
```

For fixed attributes, do not expose a setter that accepts arbitrary values. Expose presence only:

```cpp
class Link final
{
  public:
    bool hasXlinkTypeAttribute() const noexcept { return has_xlink_type_; }
    void writeXlinkTypeAttribute() noexcept { has_xlink_type_ = true; }
    void omitXlinkTypeAttribute() noexcept { has_xlink_type_ = false; }

  private:
    bool has_xlink_type_ = false;
};
```

The serializer writes the fixed value when the presence flag is true.

### Empty elements

Presence-only elements should not become empty structs in parent APIs. In a sequence field, use a
specific marker type only when the element itself needs a type in a `std::variant`.

```cpp
class Empty final
{
  public:
    static constexpr Empty present() noexcept { return Empty(); }

  private:
    constexpr Empty() noexcept = default;
};
```

An optional presence-only child in a pure sequence can be `std::optional<Empty>`. A presence-only
choice alternative can be `Empty` in a variant.

### Schema derivation

MusicXML derivation is small and attribute-only. Use non-polymorphic public inheritance only for IR
`derived` types. Do not introduce a virtual base class for all elements.

```cpp
class EmptyTrillSound
{
  public:
    const std::optional<StartStop> &type() const noexcept { return type_; }
    void setType(std::optional<StartStop> value) { type_ = std::move(value); }

  private:
    std::optional<StartStop> type_;
    std::optional<YesNo> accelerate_;
    std::optional<TrillBeats> beats_;
};

class Mordent final : public EmptyTrillSound
{
  public:
    const std::optional<AboveBelow> &placement() const noexcept { return placement_; }
    void setPlacement(std::optional<AboveBelow> value) { placement_ = std::move(value); }

  private:
    std::optional<AboveBelow> placement_;
};
```

No virtual destructor is needed because this is not a polymorphic hierarchy and the API should not
traffic in owning base pointers.

## Document root

The document has exactly one root. Use a root variant.

```cpp
#pragma once

#include "mx/core/Result.h"
#include "mx/core/ScorePartwise.h"
#include "mx/core/ScoreTimewise.h"

#include <string>
#include <variant>
#include <vector>

namespace mx::core
{

inline constexpr std::string_view SupportedMusicXMLVersion = "4.0";
inline constexpr std::string_view DoctypeValueScorePartwise =
    "-//Recordare//DTD MusicXML 4.0 Partwise//EN";
inline constexpr std::string_view DoctypeValueScoreTimewise =
    "-//Recordare//DTD MusicXML 4.0 Timewise//EN";

struct ExtraAttribute
{
    std::string name;
    std::string value;
};

class Document final
{
  public:
    using Root = std::variant<ScorePartwise, ScoreTimewise>;

    explicit Document(ScorePartwise score) : root_(std::move(score)) {}
    explicit Document(ScoreTimewise score) : root_(std::move(score)) {}

    static Result<Document> fromXDoc(const ezxml::XDoc &doc);

    bool isPartwise() const noexcept { return std::holds_alternative<ScorePartwise>(root_); }
    bool isTimewise() const noexcept { return std::holds_alternative<ScoreTimewise>(root_); }

    const Root &root() const noexcept { return root_; }

    const std::vector<ExtraAttribute> &rootNamespaces() const noexcept { return root_namespaces_; }
    void preserveRootNamespace(ExtraAttribute attr) { root_namespaces_.push_back(std::move(attr)); }

    void toXDoc(ezxml::XDoc &out) const;

  private:
    Root root_;
    std::vector<ExtraAttribute> root_namespaces_;
};

} // namespace mx::core
```

The current corert harness calls `makeDocument()->fromXDoc(...)`. Keep that as a compatibility
adapter, not as the model itself:

```cpp
namespace mx::core
{

class DocumentIo final
{
  public:
    bool fromXDoc(std::ostream &errors, const ezxml::XDoc &doc)
    {
        auto parsed = Document::fromXDoc(doc);
        if (!parsed)
        {
            errors << parsed.error().message;
            return false;
        }
        document_ = std::move(parsed.value());
        return true;
    }

    void toXDoc(ezxml::XDoc &out) const
    {
        document_.value().toXDoc(out);
    }

  private:
    std::optional<Document> document_;
};

using DocumentPtr = std::unique_ptr<DocumentIo>;

inline DocumentPtr makeDocument()
{
    return std::make_unique<DocumentIo>();
}

} // namespace mx::core
```

`DocumentIo` may be empty while parsing; `Document` never is. That keeps legacy harness shape from
infecting the product model.

## Parsing policy

Use two distinct policies:

1. **Structure is strict.** Unknown elements, unknown attributes, wrong order, missing required
   content, and too many bounded elements are errors.
2. **Numeric import may clamp.** Existing mx behavior and fixup sidecars expect out-of-range numeric
   values to be clamped into range. That is allowed because the resulting model is valid.

Do not silently repair these:

- unknown enum literals;
- failed string patterns;
- missing required children or attributes;
- choice branches with multiple alternatives;
- newer MusicXML root versions.

Version gating should happen at document parse before dispatching deep into the model. A document
declaring a newer version should return `unsupportedVersion`, letting the corert harness skip it.

## Serialization policy

Serialization should be total for every valid model object.

Rules:

- sequence-shaped classes emit in schema order;
- repeated choice vectors emit in vector order;
- optional attributes emit only when present;
- defaulted attributes use effective getters for API reads but preserve absence on write;
- fixed attributes emit their fixed literal only when their presence flag is set;
- root namespace declarations captured during parse are written back on the root;
- XML declaration and doctype normalization remain in the corert normalization layer, not in every
  model serializer.

## Template strategy

The C++ templates should consume more of the Plates than Go/C did:

- value templates consume `EnumPlate`, `NumberPlate`, `StringPlate`, and `UnionPlate` as Go/C do;
- complex templates must consume `ComplexPlate.content` for child structure;
- attributes still come from `attributes` / `merged_attributes`;
- schema derivation uses `strategy = inherit` from the C++ config default;
- repeated heterogeneous choices render boxed variants;
- pure sequences render named fields;
- repeated homogeneous elements render private vectors and append APIs.

This must still be done without adding C++ branches to the generator. If a needed datum is missing,
first ask whether it is a neutral schema fact. If yes, it belongs in the Plates for all targets. If
it is C++ spelling, it belongs in `gen/cpp/config.toml` or `gen/cpp/templates/`.

Expected C++ target manifest shape:

```toml
[render]
dir = "templates"
format = ["clang-format", "-i", "{dir}/**/*.{h,cpp}"]

[[render.type]]
strategies = ["enum-class"]
template = "enum.h.tmpl"
output = "{pascal}.h"

[[render.type]]
strategies = ["enum-class"]
template = "enum.cpp.tmpl"
output = "{pascal}.cpp"

[[render.type]]
strategies = ["numeric-wrapper"]
template = "number.h.tmpl"
output = "{pascal}.h"

[[render.type]]
strategies = ["numeric-wrapper"]
template = "number.cpp.tmpl"
output = "{pascal}.cpp"

[[render.type]]
strategies = ["string-wrapper"]
template = "string.h.tmpl"
output = "{pascal}.h"

[[render.type]]
strategies = ["string-wrapper"]
template = "string.cpp.tmpl"
output = "{pascal}.cpp"

[[render.type]]
strategies = ["tagged-variant"]
template = "union.h.tmpl"
output = "{pascal}.h"

[[render.type]]
strategies = ["tagged-variant"]
template = "union.cpp.tmpl"
output = "{pascal}.cpp"

[[render.type]]
strategies = ["value-class", "composite-class", "attrs-class", "flag", "inherit"]
template = "complex.h.tmpl"
output = "{pascal}.h"

[[render.type]]
strategies = ["value-class", "composite-class", "attrs-class", "flag", "inherit"]
template = "complex.cpp.tmpl"
output = "{pascal}.cpp"

[[render.once]]
template = "Document.h.tmpl"
output = "Document.h"

[[render.once]]
template = "Document.cpp.tmpl"
output = "Document.cpp"

[[render.once]]
template = "runtime.h.tmpl"
output = "Result.h"
```

The exact filenames can change, but source/header split should not.

## Rejected designs

### Raw public structs

Rejected. This copies the Go/C harness shape and leaks invariants to every caller. A public
`std::vector` or public `std::optional` for required content lets users construct invalid MusicXML.

### One `std::vector<std::variant<...>>` for every complex type

Rejected as the universal representation. It preserves order but erases sequence structure and makes
simple types like `Pitch` unnecessarily dynamic. Use it only for repeated heterogeneous choices.

### Nullable child structs

Rejected. `struct NoteChild { Pitch *pitch; Rest *rest; ... }` has invalid states: none set or many
set. C++ has `std::variant`; use it.

### A virtual `Element` base class

Rejected. It adds heap allocation and vtables everywhere, makes ownership harder, and hides schema
facts behind runtime checks. MusicXML's complex graph is a DAG; most members can be by value.

### `std::shared_ptr` everywhere

Rejected. Shared ownership should be a rare explicit decision, not the model's default ownership
story. Use values for ordinary members and `std::unique_ptr` only to keep repeated heterogeneous
choice nodes small.

### Strict validation only as a separate pass

Rejected. A separate `validate()` pass means invalid documents can exist in memory. The product API
should make validity the normal state and use builders/parsers as temporary staging areas.

## Implementation order

1. Add the C++ target templates and render manifest without touching generator Python.
2. Generate value types first: enum, number, string, union; compile a values smoke test.
3. Generate simple complex types with attributes and value content.
4. Generate pure sequences such as `Pitch`.
5. Generate repeated choice content such as `PartwiseMeasure`.
6. Generate `Document` and version-gated root dispatch.
7. Wire corert through the compatibility `DocumentIo` adapter.
8. Mark or fix corpus files that are structurally invalid if strict parsing exposes them.
9. Keep Go and C green to prove the target did not contaminate the generator.

The hard part is not C++ syntax. The hard part is preserving the architecture: C++ must be just
another target while still being a much better product API than the proof targets.
