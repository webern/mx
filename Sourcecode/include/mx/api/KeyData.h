// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
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
        // key where C's are sharp and D's are one-quarter-tone sharp.
        //
        // KeyComponent cSharp{ Step::c, 1, Accidental::sharp };
        // KeyComponent dQuarterTone{ Step::d, 0.5, Accidental:quarterSharp };
        // KeyData key;
        // key.custom.push_back( cSharp );
        // key.custom.push_back( dQuarterTone );
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

            // Mode specifies whether the key is major or minor. It is optional.
            KeyMode mode;

            // Supports changing the key somewhere other than at the start of a measure.
            int tickTimePosition;

            // this value is optional. -1 means unspecified. when value is
            // unspecified it means that the key signature applies to all staves
            // within the part
            int staffIndex;

            // TODO support position data and/or other attribtues

            // Supports the creation of customized, non-traditional key signatures by specifying the exact note
            // alterations. When custom is non-empty, then fifths and mode are ignored.
            std::vector<KeyComponent> custom;

            KeyData()
            : fifths{ 0 }
            , cancel{ 0 }
            , mode{ KeyMode::unspecified }
            , tickTimePosition{ 0 }
            , staffIndex{ -1 }
            , custom{}
            {

            }
        };

        MXAPI_EQUALS_BEGIN( KeyData )
        MXAPI_EQUALS_MEMBER( fifths )
        MXAPI_EQUALS_MEMBER( cancel )
        MXAPI_EQUALS_MEMBER( mode )
        MXAPI_EQUALS_MEMBER( tickTimePosition )
        MXAPI_EQUALS_MEMBER( staffIndex )
        MXAPI_EQUALS_MEMBER( custom )
        MXAPI_EQUALS_END;

        MXAPI_NOT_EQUALS_AND_VECTORS( KeyData );
    }
}
