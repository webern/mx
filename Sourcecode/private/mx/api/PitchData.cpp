// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/PitchData.h"

namespace mx
{
    namespace api
    {
        PitchData::PitchData()
        : PitchData{ Step::c }
        {
            
        }

        PitchData::PitchData(
            Step inStep,
            int inAlter,
            int inOctave,
            Accidental inAccidental,
            double inCents,
            bool inIsAccidentalParenthetical,
            bool inIsAccidentalCautionary,
            bool inIsAccidentalEditorial,
            bool inIsAccidentalBracketed
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



        void PitchData::showAccidental()
        {
            switch( alter )
            {
                case -3: accidental = Accidental::tripleFlat; break;
                case -2: accidental = Accidental::flatFlat; break;
                case -1: accidental = Accidental::flat; break;
                case  0: accidental = Accidental::natural; break;
                case  1: accidental = Accidental::sharp; break;
                case  2: accidental = Accidental::doubleSharp; break;
                case  3: accidental = Accidental::tripleSharp; break;
                default: accidental = Accidental::none;
            }
        }
        
        
        void PitchData::hideAccidental()
        {
            accidental = Accidental::none;
            isAccidentalBracketed = false;
            isAccidentalCautionary = false;
            isAccidentalEditorial = false;
            isAccidentalParenthetical = false;
        }
    }
}
