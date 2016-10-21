// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
    	enum class Step
    	{
    		c = 0,
    		d = 1,
    		e = 2,
    		f = 3,
    		g = 4,
    		a = 5,
    		b = 6
    	};

        enum class Accidental
        {
            // none means the note does not have an accidental
            none,

            // the rest of these values map to the musicxml specification
            sharp,
            natural,
            flat,
            doubleSharp,
            sharpSharp,
            flatFlat,
            naturalSharp,
            naturalFlat,
            quarterFlat,
            quarterSharp,
            threeQuartersFlat,
            threeQuartersSharp,
            sharpDown,
            sharpUp,
            naturalDown,
            naturalUp,
            flatDown,
            flatUp,
            tripleSharp,
            tripleFlat,
            slashQuarterSharp,
            slashSharp,
            slashFlat,
            doubleSlashFlat,
            sharp1,
            sharp2,
            sharp3,
            sharp5,
            flat1,
            flat2,
            flat3,
            flat4,
            sori,
            koron
        };

        struct PitchData
        {
            PitchData();

            Step step;
            int alter;
            Accidental accidental;
            bool isAccidentalParenthetical;
            bool isAccidentalCautionary;
            bool isAccidentalEditorial;
            bool isAccidentalBracketed;
            int octave;

            // automatically set the Accidental enum value by
            // parsing the alter value
            void showAccidental();
            
            // set the accidental value to 'none' and clear out the
            // other accidental-related values
            void hideAccidental();
        };
        
        MXAPI_EQUALS_BEGIN( PitchData )
        MXAPI_EQUALS_FIRST_MEMBER( step )
        MXAPI_EQUALS_NEXT_MEMBER( alter )
        MXAPI_EQUALS_NEXT_MEMBER( accidental )
        MXAPI_EQUALS_NEXT_MEMBER( isAccidentalParenthetical )
        MXAPI_EQUALS_NEXT_MEMBER( isAccidentalCautionary )
        MXAPI_EQUALS_NEXT_MEMBER( isAccidentalEditorial )
        MXAPI_EQUALS_NEXT_MEMBER( isAccidentalBracketed )
        MXAPI_EQUALS_LAST_MEMBER( octave )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PitchData );
    }
}
