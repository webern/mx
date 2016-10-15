// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/ClefData.h"
#include <sstream>

namespace mx
{
    namespace api
    {
        ClefData::ClefData()
        : staffIndex{ 0 }
        , symbol{ DEFAULT_CLEF_SYMBOL }
        , line{ DEFAULT_CLEF_LINE }
        , octaveChange{ DEFAULT_CLEF_OCTAVE_CHANGE }
        , tickTimePosition{ 0 }
        , location{ ClefLocation::unspecified }
        {

        }


        std::string ClefData::toString() const
        {
            std::stringstream ss;
            ss << "Symbol ";
            switch(symbol)
            {
                case ClefSymbol::g: ss << "G"; break;
                case ClefSymbol::f: ss << "F"; break;
                case ClefSymbol::c: ss << "C"; break;
                case ClefSymbol::percussion: ss << "Percussion"; break;
                case ClefSymbol::tab: ss << "Tab"; break;
                case ClefSymbol::jianpu: ss << "Jianpu"; break;
                case ClefSymbol::none: ss << "None"; break;
                default: ss << "Unknown"; break;      
            }
            ss << ", Line " << line;
            ss << ", OctaveChange " << octaveChange;
            return ss.str();
        }


    	void ClefData::setTreble()
        {
            symbol = ClefSymbol::g;
            line = 2;
            octaveChange = 0;
        }


    	void ClefData::setBass()
        {
            symbol = ClefSymbol::f;
            line = 4;
            octaveChange = 0;
        }

        
    	void ClefData::setAlto()
        {
            symbol = ClefSymbol::c;
            line = 3;
            octaveChange = 0;
        }

        
    	void ClefData::setTenor()
        {
            symbol = ClefSymbol::c;
            line = 4;
            octaveChange = 0;
        }

        
    	void ClefData::setBaritone()
        {
            symbol = ClefSymbol::c;
            line = 5;
            octaveChange = 0;
        }

        
    	void ClefData::setSoprano()
        {
            symbol = ClefSymbol::c;
            line = 0;
            octaveChange = 0;
        }

        
    	void ClefData::setMezzoSoprano()
        {
            symbol = ClefSymbol::c;
            line = 1;
            octaveChange = 0;
        }

        
    	bool ClefData::isTreble() const
        {
            return ( symbol == ClefSymbol::g ) && ( line == 2 ) && ( octaveChange == 0 );
        }

        
    	bool ClefData::isBass() const
        {
            return ( symbol == ClefSymbol::f ) && ( line == 4 ) && ( octaveChange == 0 );
        }

        
    	bool ClefData::isAlto() const
        {
            return ( symbol == ClefSymbol::c ) && ( line == 3 ) && ( octaveChange == 0 );
        }

        
    	bool ClefData::isTenor() const
        {
            return ( symbol == ClefSymbol::c ) && ( line == 4 ) && ( octaveChange == 0 );
        }

        
    	bool ClefData::isBaritone() const
        {
            return ( symbol == ClefSymbol::c ) && ( line == 5 ) && ( octaveChange == 0 );
        }

        
    	bool ClefData::isSoprano() const
        {
            return ( symbol == ClefSymbol::c ) && ( line == 0 ) && ( octaveChange == 0 );
        }

        
    	bool ClefData::isMezzoSoprano() const
        {
            return ( symbol == ClefSymbol::c ) && ( line == 1 ) && ( octaveChange == 0 );
        }
    }
}
