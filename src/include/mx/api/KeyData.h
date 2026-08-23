// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mx/api/ApiCommon.h"
#include "mx/api/Id.h"
#include "mx/api/KeyComponent.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// KeyMode is the mode of a traditional key signature, the MusicXML <mode> element. Together with
// fifths it tells you which key the signature spells: zero fifths with major is C major, with minor
// it is A minor, with dorian it is D dorian. The mode does not change the accidentals that are
// drawn; fifths alone determines those.
//
// `none` is the mode of music that has no tonal center. It is how a keyless staff is written:
// zero fifths with <mode>none</mode>. It is a mode value like any other, and it says nothing about
// how the signature is drawn.
//
// MusicXML's <mode> element is optional, so `unspecified` (a key signature that does not state a
// mode) is distinct from `none` (a key signature that states there is no mode).
enum class KeyMode
{
    unspecified, // no mode is stated
    unsupported, // a mode was stated, but it is outside the standard vocabulary below
    major,
    minor,
    dorian,
    phrygian,
    lydian,
    mixolydian,
    aeolian,
    ionian,
    locrian,
    none
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
// Example, a keyless signature (no tonal center):
// KeyData key;
// key.fifths = 0; // (i.e. no sharps or flats)
// key.mode = KeyMode::none;
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

    // The mode of the key signature (major, minor, dorian, none, and so on). It is optional: the
    // default, KeyMode::unspecified, states no mode and writes no <mode> element. KeyMode::none
    // is a stated mode, and it writes <mode>none</mode>.
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

    // The <key> element's id attribute (see Id.h).
    std::optional<Id> id;

    KeyData()
        : fifths{0}, cancel{0}, cancelLocation{CancelLocation::unspecified}, mode{KeyMode::unspecified},
          tickTimePosition{0}, staffIndex{INDEX_UNSPECIFIED}, nonTraditional{}, id{}
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
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;

MXAPI_NOT_EQUALS_AND_VECTORS(KeyData);
} // namespace api
} // namespace mx
