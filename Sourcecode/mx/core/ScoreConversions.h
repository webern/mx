// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/ScoreTimewise.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include <memory>

namespace mx
{
    namespace core
    {
    	// Converts a 'score-timewise' to 'score-partwise'
    	// caution - uses shallow copies, does not clone
    	ScorePartwisePtr timewisePartwise( const ScoreTimewisePtr& inScore );

    	// Converts a 'score-timewise' to 'score-partwise'
    	// caution - uses shallow copies, does not clone
    	ScoreTimewisePtr partwiseTimewise( const ScorePartwisePtr& inScore );
    }
}
