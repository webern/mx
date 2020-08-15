// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include <iostream>
#include <string>
#include <sstream>
#include "mx/core/ElementInterface.h"

namespace mxtest
{
    
    enum class TestMode
    {
        one = 1,
        two = 2,
        three = 3
    };
    
    
    inline std::string indent( int times )
    {
        std::stringstream output;
        for ( int i = 0; i < times; ++i )
        {
            output << mx::core::INDENT;
        }
        return output.str();
    }
    
    
    inline std::ostream& streamLine( std::ostream& os, int indents, const std::string& line, bool newline )
    {
        os << indent( indents );
        os << line;
        if ( newline )
        {
            os << std::endl;
        }
        return os;
    }
    
    
    inline std::ostream& streamLine( std::ostream& os, int indents, const std::string& line )
    {
        return streamLine( os, indents, line, true );
    }
    
}

#endif

