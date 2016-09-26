// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <vector>

namespace mx
{
	enum class ClefSymbol
	{
		none,
		g,
		f,
		c,
		percussion,
		tab,
		jianpu
	};

	enum class ClefLocation
	{
		unspecified,
		beforeBarline,
		afterBarline,
		midMeasure
	};

	constexpr ClefSymbol DEFAULT_CLEF_SYMBOL = ClefSymbol::g;
	constexpr int DEFAULT_CLEF_LINE = 2;
	constexpr int DEFAULT_CLEF_OCTAVE_CHANGE = 0;

    namespace api
    {
        class ClefData
        {
        public:

        	ClefData();

        	int staffIndex;
        	ClefSymbol symbol;
        	int line;
        	int octaveChange;
        	int tickPosition;
        	ClefLocation location;

        	// convenience - set symbol, line and octave for common clefs
        	void setTreble();
        	void setBass();
        	void setAlto();
        	void setTenor();
        	void setBaritone();
        	void setSoprano();
        	void setMezzoSoprano();

        	// convenience - check symbol, line and octave for common clefs
        	bool isTreble() const;
        	bool isBass() const;
        	bool isAlto() const;
        	bool isTenor() const;
        	bool isBaritone() const;
        	bool isSoprano() const;
        	bool isMessoSoprano() const;
        };
    }
}
