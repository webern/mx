// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
