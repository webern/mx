// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/XsString.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <set>

namespace mx::core
{
    static constexpr const char* const kWhitespace = " \t\n\v\f\r";
    static constexpr const char* const kNCNameAllowed = "-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";


    inline StringType trim(const StringType& str, const StringType& whitespace = kWhitespace )
    {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == StringType::npos)
            return ""; // no content
        
        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin + 1;
        
        return str.substr(strBegin, strRange);
    }
    

    inline StringType reduce(
        const StringType& str,
        const StringType& fill = " ",
        const StringType& whitespace = kWhitespace)
    {
        // trim first
        auto result = trim(str, whitespace);
        
        // replace sub ranges
        auto beginSpace = result.find_first_of(whitespace);
        while (beginSpace != mx::core::StringType::npos)
        {
            const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
            const auto range = endSpace - beginSpace;
            
            result.replace(beginSpace, range, fill);
            
            const auto newStart = beginSpace + fill.length();
            beginSpace = result.find_first_of(whitespace, newStart);
        }
        
        return result;
    }


    inline StringType onlyAllow(
        const StringType& input,
        const StringType& fill = "_",
        const StringType& onlyAllow = kNCNameAllowed)
    {
        std::string str = input;
        auto firstBadChar = str.find_first_not_of( onlyAllow );
        while ( firstBadChar != StringType::npos )
        {
            str = str.replace( firstBadChar, 1, fill );
            firstBadChar = str.find_first_not_of( onlyAllow );
        }
        return str;
    }
    

    // inline std::set<int> copyPositives( const std::set<int>& input )
    // {
    //     std::set<int> output;
    //     std::copy_if( input.cbegin(), input.cend(), std::inserter( output, output.begin() ),
    //                  [](const int i){ return i > 0; } );
    //     return output;
    // }
}
