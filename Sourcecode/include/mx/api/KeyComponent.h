// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mx/api/ApiCommon.h"
#include "mx/api/PitchData.h"

namespace mx
{
    namespace api
    {

        // KeyComponent is only used for non-traditional key signatures. It facilitates specifying the
        // exact accidentals that are found in the key signature.
        struct KeyComponent
        {
            // The note name that is to be altered by the key signature.
            Step step;

            // The amount that notes of this note name should be altered, in semitones.
            int alter;

            // Additional amount that notes of this note name should be altered, in cents. If alter = 1 and cents = 25.1
            // then the note is altered by 1.251 semitones in total.
            double cents;

            // The accidental to display for this note name.
            Accidental accidental;
        };


        MXAPI_EQUALS_BEGIN( KeyComponent )
        MXAPI_EQUALS_MEMBER( step )
        MXAPI_EQUALS_MEMBER( alter )
        MXAPI_EQUALS_MEMBER( cents )
        MXAPI_EQUALS_MEMBER( accidental )
        MXAPI_EQUALS_END;

        MXAPI_NOT_EQUALS_AND_VECTORS( KeyComponent );
    }
}
