// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/PitchData.h"

namespace mx
{
    namespace api
    {
        PitchData::PitchData()
        : step{ Step::c }
        , alter{ 0 }
        , accidental{Accidental::none}
        , isAccidentalParenthetical{ false }
        , isAccidentalCautionary{ false }
        , isAccidentalEditorial{ false }
        , isAccidentalBracketed{ false }
        , octave{ 4 }
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
