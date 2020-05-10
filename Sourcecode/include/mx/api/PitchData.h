// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
    		b = 6,
            unspecified = -1
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
            // default construction is middle c (c4)
            PitchData();

            constexpr explicit PitchData(
                Step inStep,
                int inAlter = 0,
                int inOctave = 4,
                Accidental inAccidental = Accidental::none,

                // less-often used params last
                double inCents = 0.0,
                bool inIsAccidentalParenthetical = false,
                bool inIsAccidentalCautionary = false,
                bool inIsAccidentalEditorial = false,
                bool inIsAccidentalBracketed = false
            )
            : step{ inStep }
            , alter{ inAlter }
            , cents{ inCents }
            , accidental{ inAccidental }
            , isAccidentalParenthetical{ inIsAccidentalParenthetical }
            , isAccidentalCautionary{ inIsAccidentalCautionary }
            , isAccidentalEditorial{ inIsAccidentalEditorial }
            , isAccidentalBracketed{ inIsAccidentalBracketed }
            , octave{ inOctave }
            {

            }

            // the note name. i.e. c, d, e, f, g, a, b
            Step step;

            // the alteration (number of semitones of pitch distance) from the step. for example, if step is 'c' and
            // alter is 1, then the sounding pitch is c#. if step is 'd' and alter is -2, then the sounding pitch is
            // 'c' (i.e. d double flat). alter only affects the sounding pitch of the note. accidentals are applied
            // independently. alter is always required to produce the correct sounding pitch, regardless of key
            // signature or accidentals.
            int alter;

            // additional alteration to the sounding pitch (in hundredths of a semitone). the MusicXML alter value is
            // a floating point number to facilitate microtonal music. however for mx::api we wanted the simplicity of
            // dealing with integrals for the more common case on non-microtonal music. in order to still support
            // microtones without resorting to a floating-point alter value, we break out microtonal adjustments to a
            // separate 'cents' field, which will be added to the alter integral:
            // <alter> = (double)alter + (cents / 100.0)
            double cents;

            // in MusicXML, the accidental is completely independent of the sounding pitch and is only present when you
            // actually want to show the accidental in the notated music. i.e. accidental is purely visual. for example,
            // if you have a measure consisting of repeated c# notes, you would typically notate this with an accidental
            // on the first note only. the rest of the notes of the measure would be 'sharped' by virtue of the first
            // note's sharp. in MusicXML, the first note should have an accidental of 'sharp' and an alter of '1', and
            // the remaining notes of the measure should have an accidental of 'none' and an alter of '1'.
            Accidental accidental;
            bool isAccidentalParenthetical;
            bool isAccidentalCautionary;
            bool isAccidentalEditorial;
            bool isAccidentalBracketed;

            // which octave the note is located in. middle c is in octave 4.
            int octave;

            // automatically set the Accidental enum value by parsing the alter value (does not consider the value of
            // cents). this is a convenience function that simply adds the correct accidental given the current value
            // of alter. for example, if alter is 1, then showAccidental will set the accidental field to 'sharp'.
            void showAccidental();
            
            // set the accidental value to 'none' and clear out the other accidental-related values
            void hideAccidental();
        };
        
        MXAPI_EQUALS_BEGIN( PitchData )
        MXAPI_EQUALS_MEMBER( step )
        MXAPI_EQUALS_MEMBER( alter )
        MXAPI_DOUBLES_EQUALS_MEMBER( cents )
        MXAPI_EQUALS_MEMBER( accidental )
        MXAPI_EQUALS_MEMBER( isAccidentalParenthetical )
        MXAPI_EQUALS_MEMBER( isAccidentalCautionary )
        MXAPI_EQUALS_MEMBER( isAccidentalEditorial )
        MXAPI_EQUALS_MEMBER( isAccidentalBracketed )
        MXAPI_EQUALS_MEMBER( octave )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PitchData );
    }
}
