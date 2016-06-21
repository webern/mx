// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include <string>
#include <cctype>

namespace mx
{
    namespace utility
    {
        inline bool charCompareCaseInsensitive( char a, char b )
        {
            return std::tolower( a ) == std::tolower( b );
        }
        
        
        inline bool charCompareCaseSensitive( char a, char b )
        {
            return a == b;
        }
        
        
        inline bool compareCaseInsensitive( const std::string& a, const std::string& b )
        {
            if ( a.length() == b.length() )
            {
                return std::equal( b.begin(), b.end(), a.begin(), charCompareCaseInsensitive );
            }
            else
            {
                return false;
            }
        }
    }
}