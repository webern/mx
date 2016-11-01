// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
        enum class ClefSymbol
        {
            none,
            g,
            f,
            c,
            percussion,
            tab,
            jianpu
        };
        
        enum class ClefLocation
        {
            unspecified,
            beforeBarline,
            afterBarline,
            midMeasure
        };
        
        constexpr ClefSymbol DEFAULT_CLEF_SYMBOL = ClefSymbol::g;
        constexpr int DEFAULT_CLEF_LINE = 2;
        constexpr int DEFAULT_CLEF_OCTAVE_CHANGE = 0;
        

        class ClefData
        {
        public:
            
            ClefData();
            
            //int staffIndex;
            ClefSymbol symbol;
            int line;
            int octaveChange;
            int tickTimePosition;
            ClefLocation location;
            std::string toString() const;
            
            // convenience - set symbol, line and octave for common clefs
            void setTreble();
            void setBass();
            void setAlto();
            void setTenor();
            void setBaritone();
            void setSoprano();
            void setMezzoSoprano();
            
            // convenience - check symbol, line and octave for common clefs
            bool isTreble() const;
            bool isBass() const;
            bool isAlto() const;
            bool isTenor() const;
            bool isBaritone() const;
            bool isSoprano() const;
            bool isMezzoSoprano() const;
        };

        MXAPI_EQUALS_BEGIN( ClefData )
        //MXAPI_EQUALS_MEMBER( staffIndex )
        MXAPI_EQUALS_MEMBER( symbol )
        MXAPI_EQUALS_MEMBER( line )
        MXAPI_EQUALS_MEMBER( octaveChange )
        MXAPI_EQUALS_MEMBER( tickTimePosition )
        MXAPI_EQUALS_MEMBER( location )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( ClefData );
    }
}
