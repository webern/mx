// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/Enums.h"

namespace mx
{
    namespace impl
    {
        class LyricType
        {
            
        public:
            explicit LyricType( const std::string& value, core::SyllabicEnum syllabic );
            LyricType();
            
        private:
            std::string myText;
            core::SyllabicEnum mySyllabic;
        };
    }
}
