// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

namespace mx
{
    namespace api
    {
        enum class LineType
        {
            unspecified,
            solid,
            dashed,
            dotted,
            wavy
        };
        
        struct LineData
        {
            LineType lineType;
            bool isDashLengthSpecified;
            long double dashLength;
            bool isSpaceLengthSpecified;
            long double spaceLength;
            
            inline bool isSpecified() const
            {
                return lineType != LineType::unspecified ||
                isDashLengthSpecified ||
                isSpaceLengthSpecified;
            }
            
            LineData()
            : lineType{ LineType::unspecified }
            , isDashLengthSpecified{ false }
            , dashLength{ 0.0 }
            , isSpaceLengthSpecified{ false }
            , spaceLength{ 0.0 }
            {
                
            }
        };
    }
}
