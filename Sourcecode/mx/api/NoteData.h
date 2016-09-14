// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

//#include "mx/api/NoteAttachment.h"

#include <vector>


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

    	enum class NoteType
    	{
    		normal,
    		grace,
    		cue
    	};

    	enum class DurationName
    	{

    		unspecified,
            maxima,
    		longa,
    		breve,
    		whole,
    		quarter,
            half,
    		eighth,
    		dur16th,
    		dur32nd,
    		dur64th,
    		dur128th,
    		dur256th,
    		dur512th,
    		dur1024th
    	};
        
        constexpr long double DUR_QUARTERS_VALUE_MAXIMA = 32.0L;
        constexpr long double DUR_QUARTERS_VALUE_LONGA = 16.0L;
        constexpr long double DUR_QUARTERS_VALUE_BREVE = 8.0L;
        constexpr long double DUR_QUARTERS_VALUE_WHOLE = 4.0L;
        constexpr long double DUR_QUARTERS_VALUE_HALF = 2.0L;
        constexpr long double DUR_QUARTERS_VALUE_QUARTER = 1.0L;
        constexpr long double DUR_QUARTERS_VALUE_EIGHTH = 1.0L / 2.0L;
        constexpr long double DUR_QUARTERS_VALUE_16TH = 1.0L / 4.0L;
        constexpr long double DUR_QUARTERS_VALUE_32ND = 1.0L / 8.0L;
        constexpr long double DUR_QUARTERS_VALUE_64TH = 1.0L / 16.0L;
        constexpr long double DUR_QUARTERS_VALUE_128TH = 1.0L / 32.0L;
        constexpr long double DUR_QUARTERS_VALUE_256TH = 1.0L / 64.0L;
        constexpr long double DUR_QUARTERS_VALUE_512TH = 1.0L / 128.0L;
        constexpr long double DUR_QUARTERS_VALUE_1024TH = 1.0L / 256.0L;

        enum class Stem
    	{
    		unspecified,
    		up,
    		down,
    		none
    	};

    	class NoteData
    	{

    	public:

    		bool isRest;                       // if isRest is true then isUnpitched can be ignored
            bool isMeasureRest;                // only valid if isRest is true
            bool isUnpitched;                  // only relevant if isRest is false
            bool isDisplayStepOctaveSpecified; // MusicXML can optionally specify display step and octave for rests and unpitched notes
            bool isChord;                      // the note is part of a chord
            NoteType noteType;                 // normal, cue, grace
            Step step;
            int alter;
            bool showAccidental;               // the type of accidental will be automatically deduced from the alter value, -2=bb, -1=b, 0=Nat, 1=#, 2=x
            int octave;
            int staffIndex;                    // this will be one less than the number shown in the <staff> element
            int userRequestedVoiceNumber;
            Stem stem;
            DurationName durationName;         // i.e. quarter, eighth etc
            int durationDots;
            int durationTicks;
            int startPosition;
            // TODO tuplet alteration
            // TODO int durationTicks;
            // TODO int durationTicksPerQuarter;
            // TODO int startPosition;
            // TODO int stopPosition;
            // TODO beaming

            NoteData()
            : isRest{ false }
            , isMeasureRest{ false }
            , isUnpitched{ false }
            , isDisplayStepOctaveSpecified{ false }
            , isChord{ false }
            , noteType{ NoteType::normal }
            , step{ Step::c }
            , alter{ 0 }
            , showAccidental{ false }
            , octave{ 4 }
            , staffIndex{ 0 }
            , userRequestedVoiceNumber{ -1 }
            , stem{ Stem::unspecified }
            , durationName{ DurationName::unspecified }
            , durationDots{ 0 }
            , durationTicks{ 0 }
            , startPosition{ 0 }
            
            {

            }
    	private:
    		 
    	};
    }
}
