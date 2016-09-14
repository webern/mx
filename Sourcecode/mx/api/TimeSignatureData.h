// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <string>

namespace mx
{
    namespace api
    {
        enum class TimeSignatureSymbol
        {
            none,
            common,
            cut
        };

        class TimeSignatureData
        {
        public:
            TimeSignatureSymbol symbol;
            int beats;
            int beatType;
            bool isImplicit;
            bool isPrintObjectYes;

            inline bool isEqualTo( const TimeSignatureData& other ) const
            {
                return    ( beats == other.beats )
                       && ( beatType == other.beatType )
                       && ( symbol == other.symbol );
            }

            TimeSignatureData()
            : symbol{ TimeSignatureSymbol::none }
            , beats{ 4 }
            , beatType{ 4 }
            , isImplicit{ true }
            , isPrintObjectYes{ false }
            {

            }
        };
    }
}
