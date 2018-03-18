// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/LyricType.h"

namespace mx
{
    namespace impl
    {
        LyricType::LyricType()
        : mySyllabic( core::SyllabicEnum::single )
        {
            
        }

        
        LyricType::LyricType( const std::string& value, core::SyllabicEnum syllabic )
        : myText{ value }
        , mySyllabic{ syllabic }
        {
            
        }
    }
}

