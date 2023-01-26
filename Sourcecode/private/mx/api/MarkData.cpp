// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/MarkData.h"
#include "mx/impl/Converter.h"
#include "mx/core/Enums.h"

namespace mx
{
    namespace api
    {
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
        
        bool isMarkArpeggiate( MarkType markType )
        {
            return ( markType == MarkType::arpeggiate ) ||
            ( markType == MarkType::arpeggiateDown ) ||
            ( markType == MarkType::arpeggiateUp );
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


        bool isMarkOrnament( MarkType markType )
        {
            return ( markType == MarkType::trillMark ) ||
            ( markType == MarkType::turn ) ||
            ( markType == MarkType::delayedTurn ) ||
            ( markType == MarkType::invertedTurn ) ||
            ( markType == MarkType::delayedInvertedTurn ) ||
            ( markType == MarkType::verticalTurn ) ||
            ( markType == MarkType::shake ) ||
            ( markType == MarkType::wavyLine ) ||
            ( markType == MarkType::mordent ) ||
            ( markType == MarkType::invertedMordent ) ||
            ( markType == MarkType::schleifer ) ||
            ( markType == MarkType::tremoloSingleOne ) ||
            ( markType == MarkType::tremoloSingleTwo ) ||
            ( markType == MarkType::tremoloSingleThree ) ||
            ( markType == MarkType::tremoloSingleFour ) ||
            ( markType == MarkType::tremoloSingleFive ) ||
            ( markType == MarkType::otherOrnament ) ||
            ( markType == MarkType::unknownOrnament );
        }
        
        
        bool isMarkFermata( MarkType markType )
        {
            return ( markType == MarkType::fermata ) ||
            ( markType == MarkType::fermataNormal ) ||
            ( markType == MarkType::fermataAngled ) ||
            ( markType == MarkType::fermataSquare ) ||
            ( markType == MarkType::fermataUpright ) ||
            ( markType == MarkType::fermataNormalUpright ) ||
            ( markType == MarkType::fermataAngledUpright ) ||
            ( markType == MarkType::fermataSquareUpright ) ||
            ( markType == MarkType::fermataInverted ) ||
            ( markType == MarkType::fermataNormalInverted ) ||
            ( markType == MarkType::fermataAngledInverted ) ||
            ( markType == MarkType::fermataSquareInverted );
        }
        
    
        bool isMarkNonArpeggiate( MarkType markType )
        {
            return ( markType == MarkType::nonArpeggiate );
        }

    
        bool isMarkPedal( MarkType markType )
        {
            return ( markType == MarkType::pedal ) ||
            ( markType == MarkType::damp );
        }
        
        
        bool isMarkTechnical( MarkType markType )
        {
            return ( markType == MarkType::upBow ) ||
            ( markType == MarkType::downBow ) ||
            ( markType == MarkType::harmonic ) ||
            ( markType == MarkType::openString ) ||
            ( markType == MarkType::thumbPosition ) ||
            ( markType == MarkType::doubleTongue ) ||
            ( markType == MarkType::tripleTongue ) ||
            ( markType == MarkType::stopped ) ||
            ( markType == MarkType::snapPizzicato ) ||
            ( markType == MarkType::heel ) ||
            ( markType == MarkType::toe ) ||
            ( markType == MarkType::fingernails ) ||
            ( markType == MarkType::otherTechnical );
        }


        bool isMarkTremolo( MarkType markType )
        {
            return ( markType == MarkType::tremoloSingleOne ) ||
            ( markType == MarkType::tremoloSingleTwo ) ||
            ( markType == MarkType::tremoloSingleThree ) ||
            ( markType == MarkType::tremoloSingleFour ) ||
            ( markType == MarkType::tremoloSingleFive );
        }
        
        
        bool isMarkCustom( MarkType markType )
        {
            return ( markType == MarkType::customAccentTenuto );
        }
        
        
        std::string getCustomMarkName( MarkType markType )
        {
            switch ( markType )
            {
                case MarkType::customAccentTenuto:
                    return markStringCustomAccentTenuto;
                default:
                    break;
            }
            
            return "";
        }
        
        
        MarkType getMarkTypeFromCustomString( const std::string& inString )
        {
            if( inString == markStringCustomAccentTenuto ) {
                return api::MarkType::customAccentTenuto;
            }
            
            return api::MarkType::customErrorUnknown;
        }
        

        int numTremoloSlashes( MarkType markType )
        {
            if( markType == MarkType::tremoloSingleOne )
            {
                return 1;
            }

            if( markType == MarkType::tremoloSingleTwo )
            {
                return 2;
            }

            if( markType == MarkType::tremoloSingleThree )
            {
                return 3;
            }

            if( markType == MarkType::tremoloSingleFour )
            {
                return 4;
            }

            if( markType == MarkType::tremoloSingleFive )
            {
                return 5;
            }

            return -1;
        }

        
        MarkData::MarkData()
        : markType( MarkType::unspecified )
        , name{}
        , tickTimePosition{ 0 }
        , printData{}
        , positionData{}
        {
            
        }
        
        
        MarkData::MarkData( MarkType inMarkType )
        : markType( inMarkType )
        , name{}
        , tickTimePosition{ 0 }
        , printData{}
        , positionData{}
        {
            impl::Converter converter;
            if( isMarkDynamic( markType ) )
            {
                name = mx::core::toString( converter.convertDynamic( markType ) );
            }
            else if( isMarkArticulation( markType ) )
            {
                name = "articulation";
            }
            else if( isMarkFermata( markType ) )
            {
                name = mx::core::toString( converter.convertFermata( markType ) );
            }
        }
        
        
        MarkData::MarkData( Placement inPlacement, MarkType inMarkType )
        : markType( inMarkType )
        , name{}
        , tickTimePosition{ 0 }
        , printData{}
        , positionData{}
        {
            positionData.placement = inPlacement;
            impl::Converter converter;
            if( isMarkDynamic( markType ) )
            {
                name = mx::core::toString( converter.convertDynamic( markType ) );
            }
            else if( isMarkArticulation( markType ) )
            {
                name = "articulation";
            }
            else if( isMarkFermata( markType ) )
            {
                name = mx::core::toString( converter.convertFermata( markType ) );
            }
        }
    }
}
