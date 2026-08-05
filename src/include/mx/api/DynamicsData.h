// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace mx
{
namespace api
{

// A standard dynamic abbreviation represented by a dedicated MusicXML element.
enum class StandardDynamic
{
    p,
    pp,
    ppp,
    pppp,
    ppppp,
    pppppp,
    f,
    ff,
    fff,
    ffff,
    fffff,
    ffffff,
    mp,
    mf,
    sf,
    sfp,
    sfpp,
    fp,
    rf,
    rfz,
    sfz,
    sffz,
    fz,
    n,
    pf,
    sfzp
};

// The letters of the symbol, e.g. "ff" -- also the name of the MusicXML element that carries it.
std::string toString(StandardDynamic value);

// A component of a dynamic mark that has no dedicated MusicXML dynamic element. text is the
// visible fallback; smufl, when present, names the exact glyph to draw.
struct OtherDynamicsData
{
    std::string text;
    std::optional<std::string> smufl;
};

MXAPI_EQUALS_BEGIN(OtherDynamicsData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OtherDynamicsData);

// One ordered component of a compound dynamic mark: either a standard abbreviation or a custom
// component with fallback text and an optional SMuFL glyph name.
class DynamicsComponent
{
  public:
    enum class Kind
    {
        standard,
        other
    };

    DynamicsComponent();
    DynamicsComponent(StandardDynamic value);
    DynamicsComponent(OtherDynamicsData value);

    Kind kind() const;
    bool isStandard() const;
    bool isOther() const;

    // Returns the standard dynamic, or p when this holds an other-dynamics component.
    StandardDynamic standard() const;

    // Returns the custom component, or a default value when this holds a standard dynamic.
    OtherDynamicsData other() const;

    bool operator==(const DynamicsComponent &other) const;

  private:
    std::variant<StandardDynamic, OtherDynamicsData> myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(DynamicsComponent);

// The component's letters: the symbol for a standard component, the fallback text for one that has
// no dedicated MusicXML element.
std::string toString(const DynamicsComponent &value);

// A dynamic mark assembled from multiple symbols in order, such as ff followed by z for ffz.
// MusicXML writes these as children of one <dynamics> element.
struct CompoundDynamicsData
{
    std::vector<DynamicsComponent> components;
};

MXAPI_EQUALS_BEGIN(CompoundDynamicsData)
MXAPI_EQUALS_MEMBER(components)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(CompoundDynamicsData);

// The letters of the whole mark, its components run together -- "ffz" for ff followed by z.
std::string toString(const CompoundDynamicsData &value);

} // namespace api
} // namespace mx
