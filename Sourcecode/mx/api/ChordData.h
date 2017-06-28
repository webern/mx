// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/ApiCommon.h"
#include "mx/api/PitchData.h"

namespace mx
{
    namespace api
    {
        enum class ChordKind
        {
            unspecified,
            major,
            minor,
            augmented,
            diminished,
            dominant,
            majorSeventh,
            minorSeventh,
            diminishedSeventh,
            augmentedSeventh,
            halfDiminished,
            majorMinor,
            majorSixth,
            minorSixth,
            dominantNinth,
            majorNinth,
            minorNinth,
            dominant11Th,
            major11Th,
            minor11Th,
            dominant13Th,
            major13Th,
            minor13Th,
            suspendedSecond,
            suspendedFourth,
            neapolitan,
            italian,
            french,
            german,
            pedal,
            power,
            tristan,
            other,
            none
        };

        class ChordData
        {
        public:
            ChordData();

        public:
            Step root;
            int rootAlter;
            ChordKind chordKind;
            std::string text;
            Step bass;
            int bassAlter;

        };

        MXAPI_EQUALS_BEGIN( ChordData )
        MXAPI_EQUALS_MEMBER( root )
        MXAPI_EQUALS_MEMBER( rootAlter )
        MXAPI_EQUALS_MEMBER( chordKind )
        MXAPI_EQUALS_MEMBER( text )
        MXAPI_EQUALS_MEMBER( bass )
        MXAPI_EQUALS_MEMBER( bassAlter )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( ChordData );

    }
}
