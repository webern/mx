// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <cstdlib>
#include <string>

namespace mx
{
    namespace utility
    {
        // only assigns if successful. also returns isSuccess as a bool
        inline bool stringToInt( const std::string& inStr, int& outInt )
        {
            auto temp = std::atoi( inStr.c_str() );
            auto tempStr = std::to_string( temp );
            bool isSuccess = ( tempStr == inStr );
            
            if( isSuccess )
            {
                outInt = temp;
            }
            
            return isSuccess;
        }
    }
}
