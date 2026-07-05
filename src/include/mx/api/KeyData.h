// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mx/api/ApiCommon.h"
#include "mx/api/KeyComponent.h"

namespace mx
{
namespace api
{
enum class KeyMode
{
    unspecified, // a mode value was not provided
    unsupported, // a mode value was provided but is not supported
    major,
    minor
};

// CancelLocation represents the cancel element's optional location attribute. From MusicXML
// Specification: The cancel-location type is used to indicate where a key signature
// cancellation appears relative to a new key signature: to the left, to the right, or before
// the barline and to the left. It is left by default. For mid-measure key elements, a
// cancel-location of before-barline should be treated like a cancel-location of left.
enum class CancelLocation
{
    unspecified, // a location value was not provided
    left,
    right,
    beforeBarline
};

// KeyData represents a key signature. It can be in one of two configurations. Either you specify
// 'fifths' and 'mode', or you can create a custom key signature by adding items to the customKey
// vector. If anything is found in the customKey vector, then fifths and mode will be ignored.
//
// Example, a traditional key signature (D Major):
// KeyData key;
// key.fifths = 2; // (i.e. 2 sharps)
// key.mode = KeyMode::major; // (optional)
//
// Example, a traditional key signature (G Minor):
// KeyData key;
// key.fifths = -2; // (i.e. 2 flats)
// key.mode = KeyMode::minor; // (optional)
//
// If you want to create a custom time signature, you can do so like this. Here we are creating a
// key where C's are sharp and D's are one-quarter-tone sharp. See KeyComponent for details.
//
// KeyComponent cSharp{ Step::c, 1, 0.0, Accidental::sharp };
// KeyComponent dQuarterTone{ Step::d, 0, 50.0, Accidental:quarterSharp };
// KeyData key;
// key.nonTraditional.push_back( cSharp );
// key.nonTraditional.push_back( dQuarterTone );
//
struct KeyData
{
    // From MusicXML Specification:
    // The fifths type represents the number of flats or sharps in a
    // traditional key signature. Negative numbers are used for flats
    // and positive numbers for sharps, reflecting the key's
    // placement within the circle of fifths (hence the type name).
    int fifths;

    // From MusicXML Specification:
    // A cancel element indicates that the old key signature should be
    // cancelled before the new one appears. This will always happen
    // when changing to C major or A minor and need not be specified
    // then. The cancel value matches the fifths value of the cancelled
    // key signature (e.g., a cancel of -2 will provide an explicit
    // cancellation for changing from B flat major to F major). The
    // optional location attribute indicates whether the cancellation
    // appears relative to the new key signature.
    int cancel;

    // The cancel element's optional location attribute. It is ignored unless cancel is non-zero
    // (i.e. unless a cancel element is present).
    CancelLocation cancelLocation;

    // Mode specifies whether the key is major or minor. It is optional.
    KeyMode mode;

    // Supports changing the key somewhere other than at the start of a measure.
    int tickTimePosition;

    // this value is optional. INDEX_UNSPECIFIED means unspecified. when value is
    // unspecified it means that the key signature applies to all staves
    // within the part
    int staffIndex;

    // TODO support position data and/or other attributes

    // Supports the creation of customized, non-traditional key signatures by specifying the exact note
    // alterations. When custom is non-empty, then fifths and mode are ignored.
    std::vector<KeyComponent> nonTraditional;

    KeyData()
        : fifths{0}, cancel{0}, cancelLocation{CancelLocation::unspecified}, mode{KeyMode::unspecified},
          tickTimePosition{0}, staffIndex{INDEX_UNSPECIFIED}, nonTraditional{}
    {
    }
};

MXAPI_EQUALS_BEGIN(KeyData)
MXAPI_EQUALS_MEMBER(fifths)
MXAPI_EQUALS_MEMBER(cancel)
MXAPI_EQUALS_MEMBER(cancelLocation)
MXAPI_EQUALS_MEMBER(mode)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(staffIndex)
MXAPI_EQUALS_MEMBER(nonTraditional)
MXAPI_EQUALS_END;

MXAPI_NOT_EQUALS_AND_VECTORS(KeyData);
} // namespace api
} // namespace mx
