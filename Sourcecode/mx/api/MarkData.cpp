// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/MarkData.h"

namespace mx
{
    namespace api
    {
        MarkSmufl::MarkSmufl()
        : myMap{}
        {
            // articulations
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
            
            // dynamics
            myMap.emplace( std::make_pair( MarkType::p, SmuflGlyphname{ "dynamicPiano", "dynamicPiano", "dynamicPiano" } ) );
            myMap.emplace( std::make_pair( MarkType::pp, SmuflGlyphname{ "dynamicPP", "dynamicPP", "dynamicPP" } ) );
            myMap.emplace( std::make_pair( MarkType::ppp, SmuflGlyphname{ "dynamicPPP", "dynamicPPP", "dynamicPPP" } ) );
            myMap.emplace( std::make_pair( MarkType::pppp, SmuflGlyphname{ "dynamicPPPP", "dynamicPPPP", "dynamicPPPP" } ) );
            myMap.emplace( std::make_pair( MarkType::ppppp, SmuflGlyphname{ "dynamicPPPPP", "dynamicPPPPP", "dynamicPPPPP" } ) );
            myMap.emplace( std::make_pair( MarkType::pppppp, SmuflGlyphname{ "dynamicPPPPPP", "dynamicPPPPPP", "dynamicPPPPPP" } ) );
            myMap.emplace( std::make_pair( MarkType::f, SmuflGlyphname{ "dynamicForte", "dynamicForte", "dynamicForte" } ) );
            myMap.emplace( std::make_pair( MarkType::ff, SmuflGlyphname{ "dynamicFF", "dynamicFF", "dynamicFF" } ) );
            myMap.emplace( std::make_pair( MarkType::fff, SmuflGlyphname{ "dynamicFFF", "dynamicFFF", "dynamicFFF" } ) );
            myMap.emplace( std::make_pair( MarkType::ffff, SmuflGlyphname{ "dynamicFFFF", "dynamicFFFF", "dynamicFFFF" } ) );
            myMap.emplace( std::make_pair( MarkType::fffff, SmuflGlyphname{ "dynamicFFFFF", "dynamicFFFFF", "dynamicFFFFF" } ) );
            myMap.emplace( std::make_pair( MarkType::ffffff, SmuflGlyphname{ "dynamicFFFFFF", "dynamicFFFFFF", "dynamicFFFFFF" } ) );
            myMap.emplace( std::make_pair( MarkType::mp, SmuflGlyphname{ "dynamicMP", "dynamicMP", "dynamicMP" } ) );
            myMap.emplace( std::make_pair( MarkType::mf, SmuflGlyphname{ "dynamicMF", "dynamicMF", "dynamicMF" } ) );
            myMap.emplace( std::make_pair( MarkType::sf, SmuflGlyphname{ "dynamicSforzando1", "dynamicSforzando1", "dynamicSforzando1" } ) );
            myMap.emplace( std::make_pair( MarkType::sfp, SmuflGlyphname{ "dynamicSforzandoPiano", "dynamicSforzandoPiano", "dynamicSforzandoPiano" } ) );
            myMap.emplace( std::make_pair( MarkType::sfpp, SmuflGlyphname{ "dynamicSforzandoPianissimo", "dynamicSforzandoPianissimo", "dynamicSforzandoPianissimo" } ) );
            myMap.emplace( std::make_pair( MarkType::fp, SmuflGlyphname{ "dynamicFortePiano", "dynamicFortePiano", "dynamicFortePiano" } ) );
            myMap.emplace( std::make_pair( MarkType::rf, SmuflGlyphname{ "dynamicRinforzando1", "dynamicRinforzando1", "dynamicRinforzando1" } ) );
            myMap.emplace( std::make_pair( MarkType::rfz, SmuflGlyphname{ "dynamicRinforzando2", "dynamicRinforzando2", "dynamicRinforzando2" } ) );
            myMap.emplace( std::make_pair( MarkType::sfz, SmuflGlyphname{ "dynamicSforzato", "dynamicSforzato", "dynamicSforzato" } ) );
            myMap.emplace( std::make_pair( MarkType::sffz, SmuflGlyphname{ "dynamicSforzatoFF", "dynamicSforzatoFF", "dynamicSforzatoFF" } ) );
            myMap.emplace( std::make_pair( MarkType::fz, SmuflGlyphname{ "dynamicForzando", "dynamicForzando", "dynamicForzando" } ) );
            myMap.emplace( std::make_pair( MarkType::otherDynamics, SmuflGlyphname{ "", "", "" } ) );
            
            // ornaments
            myMap.emplace( std::make_pair( MarkType::trillMark, SmuflGlyphname{ "ornamentTrill", "ornamentTrill", "ornamentTrill" } ) );
            myMap.emplace( std::make_pair( MarkType::turn, SmuflGlyphname{ "ornamentTurn", "ornamentTurn", "ornamentTurn" } ) );
            myMap.emplace( std::make_pair( MarkType::delayedTurn, SmuflGlyphname{ "ornamentTurn", "ornamentTurn", "ornamentTurn" } ) );
            myMap.emplace( std::make_pair( MarkType::invertedTurn, SmuflGlyphname{ "ornamentTurnInverted", "ornamentTurnInverted", "ornamentTurnInverted" } ) );
            myMap.emplace( std::make_pair( MarkType::delayedInvertedTurn, SmuflGlyphname{ "ornamentTurnInverted", "ornamentTurnInverted", "ornamentTurnInverted" } ) );
            myMap.emplace( std::make_pair( MarkType::verticalTurn, SmuflGlyphname{ "ornamentTurnUp", "ornamentTurnUp", "ornamentTurnUp" } ) );
            myMap.emplace( std::make_pair( MarkType::shake, SmuflGlyphname{ "wiggleVIbratoMediumSlower", "wiggleVIbratoMediumSlower", "wiggleVIbratoMediumSlower" } ) );
            myMap.emplace( std::make_pair( MarkType::wavyLine, SmuflGlyphname{ "ornamentTremblement", "ornamentTremblement", "ornamentTremblement" } ) );
            myMap.emplace( std::make_pair( MarkType::mordent, SmuflGlyphname{ "ornamentMordent", "ornamentMordent", "ornamentMordent" } ) );
            myMap.emplace( std::make_pair( MarkType::invertedMordent, SmuflGlyphname{ "ornamentMordentInverted", "ornamentMordentInverted", "ornamentMordentInverted" } ) );
            myMap.emplace( std::make_pair( MarkType::schleifer, SmuflGlyphname{ "ornamentSchleifer", "ornamentSchleifer", "ornamentSchleifer" } ) );
            myMap.emplace( std::make_pair( MarkType::tremolo, SmuflGlyphname{ "tremolo3", "tremolo3", "tremolo3" } ) );
            myMap.emplace( std::make_pair( MarkType::otherOrnament, SmuflGlyphname{ "", "", "" } ) );
            
            myMap.emplace( std::make_pair( MarkType::upBow, SmuflGlyphname{ "stringsUpBow", "stringsUpBow", "stringsUpBow" } ) );
            myMap.emplace( std::make_pair( MarkType::downBow, SmuflGlyphname{ "stringsDownBow", "stringsDownBow", "stringsDownBow" } ) );
            myMap.emplace( std::make_pair( MarkType::harmonic, SmuflGlyphname{ "stringsHarmonic", "stringsHarmonic", "stringsHarmonic" } ) );
            // TODO - find the right glyph for openString
            myMap.emplace( std::make_pair( MarkType::openString, SmuflGlyphname{ "stringsHarmonic", "stringsHarmonic", "stringsHarmonic" } ) );
            myMap.emplace( std::make_pair( MarkType::thumbPosition, SmuflGlyphname{ "stringsThumbPosition", "stringsThumbPosition", "stringsThumbPosition" } ) );
            // myMap.emplace( std::make_pair( MarkType::fingering, SmuflGlyphname{ "", "", "" } ) );
            // myMap.emplace( std::make_pair( MarkType::pluck, SmuflGlyphname{ "", "", "" } ) );
            myMap.emplace( std::make_pair( MarkType::doubleTongue, SmuflGlyphname{ "doubleTongueAbove", "doubleTongueBelow", "doubleTongueAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::tripleTongue, SmuflGlyphname{ "tripleTongueAbove", "tripleTongueBelow", "tripleTongueAbove" } ) );
            myMap.emplace( std::make_pair( MarkType::stopped, SmuflGlyphname{ "brassMuteClosed", "brassMuteClosed", "brassMuteClosed" } ) );
            myMap.emplace( std::make_pair( MarkType::snapPizzicato, SmuflGlyphname{ "pluckedSnapPizzicatoAbove", "pluckedSnapPizzicatoBelow", "pluckedSnapPizzicatoAbove" } ) );
            // myMap.emplace( std::make_pair( MarkType::fret, SmuflGlyphname{ "", "", "" } ) );
            // myMap.emplace( std::make_pair( MarkType::string_, SmuflGlyphname{ "", "", "" } ) );
            // myMap.emplace( std::make_pair( MarkType::hammerOn, SmuflGlyphname{ "", "", "" } ) );
            // myMap.emplace( std::make_pair( MarkType::pullOff, SmuflGlyphname{ "", "", "" } ) );
            // myMap.emplace( std::make_pair( MarkType::bend, SmuflGlyphname{ "", "", "" } ) );
            // myMap.emplace( std::make_pair( MarkType::tap, SmuflGlyphname{ "", "", "" } ) );
            myMap.emplace( std::make_pair( MarkType::heel, SmuflGlyphname{ "keyboardPedalHeel1", "keyboardPedalHeel1", "keyboardPedalHeel1" } ) );
            myMap.emplace( std::make_pair( MarkType::toe, SmuflGlyphname{ "keyboardPedalToe1", "keyboardPedalToe1", "keyboardPedalToe1" } ) );
            myMap.emplace( std::make_pair( MarkType::fingernails, SmuflGlyphname{ "pluckedWithFingernails", "pluckedWithFingernails", "pluckedWithFingernails" } ) );
            // myMap.emplace( std::make_pair( MarkType::hole, SmuflGlyphname{ "", "", "" } ) );
            // myMap.emplace( std::make_pair( MarkType::arrow, SmuflGlyphname{ "", "", "" } ) );
            // myMap.emplace( std::make_pair( MarkType::handbell, SmuflGlyphname{ "", "", "" } ) );
            myMap.emplace( std::make_pair( MarkType::otherTechnical, SmuflGlyphname{ "", "", "" } ) );
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
        
        bool isMarkDynamic( MarkType markType )
        {
            return ( markType == MarkType::p ) ||
            ( markType == MarkType::p ) ||
            ( markType == MarkType::pp ) ||
            ( markType == MarkType::ppp ) ||
            ( markType == MarkType::pppp ) ||
            ( markType == MarkType::ppppp ) ||
            ( markType == MarkType::pppppp ) ||
            ( markType == MarkType::f ) ||
            ( markType == MarkType::ff ) ||
            ( markType == MarkType::fff ) ||
            ( markType == MarkType::ffff ) ||
            ( markType == MarkType::fffff ) ||
            ( markType == MarkType::ffffff ) ||
            ( markType == MarkType::mp ) ||
            ( markType == MarkType::mf ) ||
            ( markType == MarkType::sf ) ||
            ( markType == MarkType::sfp ) ||
            ( markType == MarkType::sfpp ) ||
            ( markType == MarkType::fp ) ||
            ( markType == MarkType::rf ) ||
            ( markType == MarkType::rfz ) ||
            ( markType == MarkType::sfz ) ||
            ( markType == MarkType::sffz ) ||
            ( markType == MarkType::fz ) ||
            ( markType == MarkType::otherDynamics );
        }
        
        bool isMarkArticulation( MarkType markType )
        {
            return ( markType == MarkType::accent ) ||
            ( markType == MarkType::strongAccent ) ||
            ( markType == MarkType::staccato ) ||
            ( markType == MarkType::tenuto ) ||
            ( markType == MarkType::detachedLegato ) ||
            ( markType == MarkType::staccatissimo ) ||
            ( markType == MarkType::spiccato ) ||
            ( markType == MarkType::scoop ) ||
            ( markType == MarkType::plop ) ||
            ( markType == MarkType::doit ) ||
            ( markType == MarkType::falloff ) ||
            ( markType == MarkType::breathMark ) ||
            ( markType == MarkType::caesura ) ||
            ( markType == MarkType::stress ) ||
            ( markType == MarkType::unstress ) ||
            ( markType == MarkType::otherArticulation );
        }
    }
}
