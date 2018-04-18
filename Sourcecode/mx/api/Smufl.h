// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>
#include <unordered_map>

namespace mx
{
    namespace api
    {
        using SmuflPair = std::pair<const std::string, const char16_t>;
        using SmuflMap = std::map<const std::string, const char16_t>;
        using SmuflIter = SmuflMap::const_iterator;
        
        
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
                        
            // returns an immutable ref to the map
            const SmuflMap& getMap();
            
        private:
            Smufl();
            
            void createSmuflMap();

            SmuflMap mMap;
        };
    }
}
