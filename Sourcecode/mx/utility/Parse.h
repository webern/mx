// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include <string>
#include <cctype>
#include <locale>
#include <iostream>
#include <algorithm>

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
        
        // http://stackoverflow.com/a/3152296/2779792
        // templated version of my_equal so it could work with both char and wchar_t
        template<typename charT>
        struct my_equal
        {
            my_equal( const std::locale& loc )
            :myLoc( loc )
            {
                
            }
            
            bool operator()(charT ch1, charT ch2)
            {
                return std::toupper(ch1, myLoc) == std::toupper(ch2, myLoc);
            }
        private:
            const std::locale& myLoc;
        };

        
        // find substring (case insensitive)
        template<typename T>
        inline int findCaseInsensitive( const T& str1, const T& str2, const std::locale& loc = std::locale() )
        {
            typename T::const_iterator it =
                std::search( str1.begin(), str1.end(),
                             str2.begin(), str2.end(),
                             my_equal<typename T::value_type>(loc) );
            
            if ( it != str1.end() )
            {
                return static_cast<int>( it - str1.begin() );
            }
            return -1;
        }
        
        
        inline bool containsCaseInsensitive(
            const std::string& substringToFind,
            const std::string& stringToFindItIn  )
        {
            auto position = findCaseInsensitive( stringToFindItIn, substringToFind );
            return position > -1;
        }
        
    }
}
