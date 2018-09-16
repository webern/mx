// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>

namespace mx
{
    namespace api
    {        
        struct SmuflGlyphname
        {
            SmuflGlyphname( std::string inNameAbove,
                            std::string inNameBelow,
                            std::string inNameUnspecified );
            
            const std::string above;
            const std::string below;
            const std::string unspecified;
            
            const std::string& getName() const;
            const std::string& getName( api::Placement placement ) const;
        };

        
        class Smufl
        {

        public:
            
            static Smufl& getInstance();

            // returns the SMuFL codepoint value for the given name
            // returns 0 (i.e. '\0';) if not found
            static char16_t findCodepoint(const std::string& inName);
            
            // finds the SMuFL glyphname for the given codepoint
            // returns empty string if codepoint is not found
            static const std::string& findName( char16_t codepoint );
                        
        private:
            Smufl();
        };
    }
}
