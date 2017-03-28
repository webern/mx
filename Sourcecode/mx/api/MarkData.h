// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
            unknownDynamics,

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
            tremoloSingleOne,   ///< A tremolo on a single note (a glyph, not a spanner) with 1 slash
            tremoloSingleTwo,   ///< A tremolo on a single note (a glyph, not a spanner) with 2 slashes
            tremoloSingleThree, ///< A tremolo on a single note (a glyph, not a spanner) with 3 slashes
            tremoloSingleFour,  ///< A tremolo on a single note (a glyph, not a spanner) with 4 slashes
            tremoloSingleFive,  ///< A tremolo on a single note (a glyph, not a spanner) with 5 slashes
            unknownOrnament,

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
            accidentalUnknown,
            
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
            unknownTechnical,
            
            // fermata
            fermata,       // <fermata/>
            fermataNormal, // <fermata>normal</fermata>
            fermataAngled, // <fermata>angled</fermata>
            fermataSquare, // <fermata>square</fermata>
            
            // sadly, in MusicXML we do not have an 'above' 'below' attribute for the fermata.
            // instead it has an attribute 'type' that is either 'upright' or 'inverted'.
            // it appears that implementations interpret 'upright' to mean the 'normal' or
            // correct orientation and 'inverted' to mean that the fermata should be 'flipped'
            // from its correct orientation.  in otherwords the implementation has to figure
            // out if the fermata is above or below the note on its own (using its own
            // algorithm combined with any default-y and relative-y values).  this mess is
            // going to be beyond the scope of the MusicXML Class Library at this time.  You
            // cause the fermata 'type' attribute to be set using the enum values below, but
            // it's not possible for this library to calculate the correct fermata glyph.
            fermataUpright,        // <fermata type="upright"/>
            fermataNormalUpright,  // <fermata type="upright">normal</fermata>
            fermataAngledUpright,  // <fermata type="upright">angled</fermata>
            fermataSquareUpright,  // <fermata type="upright">square</fermata>
            fermataInverted,       // <fermata type="inverted"/>
            fermataNormalInverted, // <fermata type="inverted">normal</fermata>
            fermataAngledInverted, // <fermata type="inverted">angled</fermata>
            fermataSquareInverted, // <fermata type="inverted">square</fermata>
            unknownFermata,        // bad state, unknown type, error
            
            pedal, // the typical piano pedal mark
            damp,  // the typical piano end-pedal mark
        };

        bool isMarkDynamic( MarkType );
        bool isMarkArticulation( MarkType );
        bool isMarkOrnament( MarkType );
        bool isMarkFermata( MarkType );
        bool isMarkPedal( MarkType );
        
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
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( MarkData );
    }
}
