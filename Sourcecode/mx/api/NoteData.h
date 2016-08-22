// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/NoteAttachment.h"

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
    		Maxima,
    		Longa,
    		Breve,
    		Whole,
    		Quarter,
    		Eighth,
    		dur16th,
    		dur32nd,
    		dur64th,
    		dur128th,
    		dur256th,
    		dur512th,
    		dur1024th
    	};

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

    		bool isRest;
            bool isChord;
    		int type;
            Step step;
            int alter;
            bool showAccidental;
            int octave;
            int staff;
            int voice;
            Stem stem;
            DurationName durationName;
            int durationDots;
            // TODO tuplet alteration
            // TODO int durationTicks;
            // TODO int durationTicksPerQuarter;
            // TODO int startPosition;
            // TODO int stopPosition;
            // TODO beaming

    	private:
    		 
    	};
    }
}