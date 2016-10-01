// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/Smufl.h"
#include "mx/impl/SmuflGlyphMap.h"

#include <locale>
#include <codecvt>
#include <algorithm>

namespace mx
{
    namespace api
    {
        SmuflGlyphname::SmuflGlyphname( std::string inNameAbove,
                                        std::string inNameBelow,
                                        std::string inNameUnspecified )
        : above{ inNameAbove }
        , below{ inNameBelow }
        , unspecified{ inNameUnspecified }
        {
            
        }
        
        
        const std::string& SmuflGlyphname::getName() const
        {
            return unspecified;
        }
        
        
        const std::string& SmuflGlyphname::getName( Placement placement ) const
        {
            switch ( placement )
            {
                case Placement::above: return above;
                case Placement::below: return below;
                case Placement::unspecified: return unspecified;
                default:
                    break;
            }
            return unspecified;
        }
        
        
        const Smufl& Smufl::instance()
        {
            static const Smufl staticInstance;
            return staticInstance;
        }

        
        Smufl::Smufl()
        : myMap{}
        {
            myMap = impl::createSmuflMap();
        }
        
        
        char16_t Smufl::findCodepoint(const std::string& inName)
        {
            const SmuflIter iter = getMap().find( inName );
            if( iter == Smufl::end() )
            {
                return 0;
            }
            return iter->second;
        }
        
        
        const std::string& Smufl::findName( char16_t codepoint )
        {
            static std::string empty = "";
            
            auto lambda = [&codepoint](const SmuflPair item)
            {
                return item.second == codepoint;
            };
            
            const SmuflIter iter = std::find_if( Smufl::begin(), Smufl::end(), lambda );
            
            if( iter == Smufl::end() )
            {
                return empty;
            }
            
            return iter->first;
        }
        
        
        bool Smufl::isNameValid(const std::string& inName)
        {
            return Smufl::findCodepoint( inName ) != 0;
        }
        

        bool Smufl::isCodepointValid( char16_t codepoint )
        {
            return Smufl::findName( codepoint ).size() > 0;
        }
        
        
        const SmuflMap& Smufl::getMap()
        {
            return Smufl::instance().myMap;
        }
        
        
        SmuflIter Smufl::begin()
        {
            return getMap().cbegin();
        }
        
        
        SmuflIter Smufl::end()
        {
            return getMap().cend();
        }
    }
}
