// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/MarkData.h"
#include "mx/api/Smufl.h"

namespace mx
{
    namespace api
    {
        MarkSmufl::MarkSmufl()
        : myMap{}
        {
            myMap.emplace( std::make_pair( MarkType::accent, SmuflGlyphname{ "articAccentAbove", "articAccentBelow", "articAccentAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::strongAccent, SmuflGlyphname{ "articMarcatoAbove", "articMarcatoBelow", "articMarcatoAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::staccato, SmuflGlyphname{ "articStaccatoAbove", "articStaccatoBelow", "articStaccatoAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::tenuto, SmuflGlyphname{ "articTenutoAbove", "articTenutoBelow", "articTenutoAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::detachedLegato, SmuflGlyphname{ "articTenutoStaccatoAbove", "articTenutoStaccatoBelow", "articTenutoStaccatoAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::staccatissimo, SmuflGlyphname{ "articStaccatissimoAbove", "articStaccatissimoBelow", "articStaccatissimoAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::spiccato, SmuflGlyphname{ "articStaccatissimoWedgeAbove", "articStaccatissimoWedgeBelow", "articStaccatissimoWedgeAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::scoop, SmuflGlyphname{ "brassScoop", "brassScoop", "brassScoop" } ) );
            myMap.emplace( std::make_pair( MarkType::plop, SmuflGlyphname{ "brassPlop", "brassPlop", "brassPlop" } ) );
            myMap.emplace( std::make_pair( MarkType::doit, SmuflGlyphname{ "brassDoitMedium", "brassDoitMedium", "brassDoitMedium" } ) );
            myMap.emplace( std::make_pair( MarkType::falloff, SmuflGlyphname{ "brassFallLipMedium", "brassFallLipMedium", "brassFallLipMedium" } ) );
            myMap.emplace( std::make_pair( MarkType::breathMark, SmuflGlyphname{ "breathMarkComma", "breathMarkComma", "breathMarkComma" } ) );
            myMap.emplace( std::make_pair( MarkType::caesura, SmuflGlyphname{ "caesura", "caesura", "caesura" } ) );
            myMap.emplace( std::make_pair( MarkType::stress, SmuflGlyphname{ "articStressAbove", "articStressBelow", "articStressAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::unstress, SmuflGlyphname{ "articUnstressAbove", "articUnstressBelow", "articUnstressAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::otherArticulation, SmuflGlyphname{ "", "", "" } ) );
        }
        
        
        const std::string& MarkSmufl::getName( MarkType mark )
        {
            return getName( mark, Placement::unspecified );
        }
        
        
        const std::string& MarkSmufl::getName( MarkType mark, Placement placement )
        {
            const auto& glyphNames = getSmuflGlyphname( mark );
            
            switch ( placement )
            {
                case Placement::unspecified: return glyphNames.unspecified;
                case Placement::above: return glyphNames.above;
                case Placement::below: return glyphNames.below;
                default: break;
            }
            return glyphNames.unspecified;
        }
        
        
        char16_t MarkSmufl::getCodepoint( MarkType mark )
        {
            return Smufl::findCodepoint( getName( mark ) );
        }
        
        
        char16_t MarkSmufl::getCodepoint( MarkType mark, Placement placement )
        {
            return Smufl::findCodepoint( getName( mark, placement ) );
        }
        
        
        const SmuflGlyphname& MarkSmufl::getSmuflGlyphname( MarkType mark )
        {
            const auto& theMap = getMap();
            const auto iter = theMap.find( mark );
            
            if( iter == theMap.cend() )
            {
                static const SmuflGlyphname emptySmuflGlyphname{ "", "", "" };
                return emptySmuflGlyphname;
            }
            
            return iter->second;
        }
        
        
        const MarkSmuflMap& MarkSmufl::getMap()
        {
            return getInstance().myMap;
        }
        
        
        const MarkSmufl& MarkSmufl::getInstance()
        {
            static const MarkSmufl theInstance;
            return theInstance;
        }
    }
}
