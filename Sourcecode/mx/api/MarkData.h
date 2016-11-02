// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"
#include "mx/api/Smufl.h"

#include <string>

namespace mx
{
    namespace api
    {

        enum class MarkType
        {
            unspecified,
            
            // articulations
            accent,
            strongAccent,    // marcato
            staccato,
            tenuto,
            detachedLegato,  // tenuto with dot
            staccatissimo,
            spiccato,        // ? using "StaccatissimoWedge"
            scoop,
            plop,
            doit,
            falloff,
            breathMark,
            caesura,
            stress,
            unstress,
            otherArticulation,
            
            // dynamics
            p,
            pp,
            ppp,
            pppp,
            ppppp,
            pppppp,
            f,
            ff,
            fff,
            ffff,
            fffff,
            ffffff,
            mp,
            mf,
            sf,
            sfp,
            sfpp,
            fp,
            rf,
            rfz,
            sfz,
            sffz,
            fz,
            otherDynamics,

            // ornaments
            trillMark,
            turn,
            delayedTurn,
            invertedTurn,
            delayedInvertedTurn,
            verticalTurn,
            shake,
            wavyLine,
            mordent,
            invertedMordent,
            schleifer,
            tremolo,
            otherOrnament,

            // accidental marks
            accidentalMarkSharp,
            accidentalMarkNatural,
            accidentalMarkFlat,
            accidentalMarkDoubleSharp,
            accidentalMarkSharpSharp,
            accidentalMarkFlatFlat,
            accidentalMarkNaturalSharp,
            accidentalMarkNaturalFlat,
            accidentalMarkQuarterFlat,
            accidentalMarkQuarterSharp,
            accidentalMarkThreeQuartersFlat,
            accidentalMarkThreeQuartersSharp,
            accidentalMarkSharpDown,
            accidentalMarkSharpUp,
            accidentalMarkNaturalDown,
            accidentalMarkNaturalUp,
            accidentalMarkFlatDown,
            accidentalMarkFlatUp,
            accidentalMarkTripleSharp,
            accidentalMarkTripleFlat,
            accidentalMarkSlashQuarterSharp,
            accidentalMarkSlashSharp,
            accidentalMarkSlashFlat,
            accidentalMarkDoubleSlashFlat,
            accidentalMarkSharp1,
            accidentalMarkSharp2,
            accidentalMarkSharp3,
            accidentalMarkSharp5,
            accidentalMarkFlat1,
            accidentalMarkFlat2,
            accidentalMarkFlat3,
            accidentalMarkFlat4,
            accidentalMarkSori,
            accidentalMarkKoron,
            
            // technical
            upBow,
            downBow,
            harmonic,
            openString,
            thumbPosition,
            // fingering,
            // pluck,
            doubleTongue,
            tripleTongue,
            stopped,
            snapPizzicato,
            // fret,
            // string_,
            // hammerOn,
            // pullOff,
            // bend,
            // tap,
            heel,
            toe,
            fingernails,
            // hole,
            // arrow,
            // handbell,
            otherTechnical,
            
            // fermata
            fermata,       // <fermata/>
            fermataNormal, // <fermata>normal</fermata>
            fermataAngled, // <fermata>angled</fermata>
            fermataSquare, // <fermata>square</fermata>
        };

        bool isMarkDynamic( MarkType );
        bool isMarkArticulation( MarkType );
        bool isMarkFermata( MarkType );
        
        using MarkSmuflEntry = std::pair<const MarkType, const SmuflGlyphname>;
        using MarkSmuflMap = std::map<const MarkType, const SmuflGlyphname>;
        
        class MarkSmufl
        {
        public:
            static const std::string& getName( MarkType mark );
            static const std::string& getName( MarkType mark, Placement placement );
            static char16_t getCodepoint( MarkType mark );
            static char16_t getCodepoint( MarkType mark, Placement placement );
            static const SmuflGlyphname& getSmuflGlyphname( MarkType mark );
            static const MarkSmuflMap& getMap();
            
        private:
            MarkSmufl();
            static const MarkSmufl& getInstance();
            MarkSmuflMap myMap;
        };
        
        
        struct MarkData
        {
            MarkType markType;
            std::string name;
            std::string smuflName;
            char16_t smuflCodepoint;
            int tickTimePosition;
            PrintData printData;
            PositionData positionData;
            FontData fontData;
            
            MarkData();
            
            // smulf and name fields will be filled in if possible
            // placement 'unspecified' will be assumed for smufl
            MarkData( MarkType inMarkType );
            
            // smulf and name fields will be filled in if possible
            // positionData.placement field will also be filled in
            MarkData( Placement inPlacement, MarkType inMarkType );
        };
        
        MXAPI_EQUALS_BEGIN( MarkData )
        MXAPI_EQUALS_MEMBER( markType )
        MXAPI_EQUALS_MEMBER( name )
        MXAPI_EQUALS_MEMBER( smuflName )
        MXAPI_EQUALS_MEMBER( smuflCodepoint )
        MXAPI_EQUALS_MEMBER( tickTimePosition )
        MXAPI_EQUALS_MEMBER( printData )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( fontData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( MarkData );
    }
}
