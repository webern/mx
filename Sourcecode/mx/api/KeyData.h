// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

namespace mx
{
	namespace api
	{
        enum class KeyMode
        {
            // a mode value was not provided
            unspecified,
            
            // a mode value was provided but
            // is not supported
            unsupported,
            
            major,
            minor
            
        };
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

            KeyMode mode;
            int tickTimePosition;
            
            // this value is optional. -1 means unspecified. when value is
            // unspecified it means that the key signature applies to all staves
            // within the part
            int staffIndex;
            
			// TODO support position data and/or other attribtues
            
            KeyData()
            : fifths{ 0 }
            , cancel{ 0 }
            , mode{ KeyMode::unspecified }
            , tickTimePosition{ 0 }
            , staffIndex{ -1 }
            {
                
            }
		};
        
        MXAPI_EQUALS_BEGIN( KeyData )
        MXAPI_EQUALS_FIRST_MEMBER( fifths )
        MXAPI_EQUALS_NEXT_MEMBER( cancel )
        MXAPI_EQUALS_NEXT_MEMBER( mode )
        MXAPI_EQUALS_NEXT_MEMBER( tickTimePosition )
        MXAPI_EQUALS_LAST_MEMBER( staffIndex )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( KeyData );
	}
}
