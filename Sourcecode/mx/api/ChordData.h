// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PitchData.h"
#include "mx/api/MiscData.h"

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

        enum class ExtensionType
        {
            add,
            remove,
            alter
        };

        enum class ExtensionNumber
        {
            first,
            second,
            third,
            fourth,
            fifth,
            sixth,
            seventh,
            ninth,
            eleventh,
            thirteenth,
        };

        enum class ExtensionAlter
        {
            none,
            flat,
            sharp,
            doubleFlat,
            doubleSharp
        };

        class Extension
        {
        public:
            Extension();

            ExtensionType extensionType;
            ExtensionAlter extensionAlter;
            ExtensionNumber extensionNumber;
            Bool printObject;
        };

        MXAPI_EQUALS_BEGIN( Extension )
        MXAPI_EQUALS_MEMBER( extensionType )
        MXAPI_EQUALS_MEMBER( extensionAlter )
        MXAPI_EQUALS_MEMBER( extensionNumber )
        MXAPI_EQUALS_MEMBER( printObject )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( Extension );

        class ChordData
        {
        public:
            ChordData();

        public:
            Step root;
            int rootAlter;
            ChordKind chordKind;
            std::string text;
            Bool useSymbols;
            Step bass;
            int bassAlter;
            std::vector<Extension> extensions;
            std::vector<MiscData> miscData;
        };

        MXAPI_EQUALS_BEGIN( ChordData )
        MXAPI_EQUALS_MEMBER( root )
        MXAPI_EQUALS_MEMBER( rootAlter )
        MXAPI_EQUALS_MEMBER( chordKind )
        MXAPI_EQUALS_MEMBER( text )
        MXAPI_EQUALS_MEMBER( useSymbols )
        MXAPI_EQUALS_MEMBER( bass )
        MXAPI_EQUALS_MEMBER( bassAlter )
        MXAPI_EQUALS_MEMBER( extensions )
        MXAPI_EQUALS_MEMBER( miscData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( ChordData );

    }
}
