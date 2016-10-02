// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/DirectionReader.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialVoiceDirectionGroup.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"
#include "mx/impl/MetronomeReader.h"

namespace mx
{
    namespace impl
    {
        DirectionReader::DirectionReader( const core::Direction& inDirection, Cursor inCursor )
        : myMutex{}
        , myDirection{ inDirection }
        , myCursor{ inCursor }
        , mySet{ inDirection.getDirectionTypeSet() }
        {
            
        }
        
        void DirectionReader::getDirectionData( api::MeasureData& outMeasureData ) const
        {
            std::lock_guard<std::mutex> lock{ myMutex };
            myOutMeasureDataP = &outMeasureData;
            
            for( const auto& directionTypePtr : mySet )
            {
                const auto& directionType = *directionTypePtr;
                
                switch ( directionType.getChoice() )
                {
                    case core::DirectionType::Choice::rehearsal:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::segno:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::words:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::coda:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::wedge:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::dynamics:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::dashes:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::bracket:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::pedal:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::metronome:
                    {
                        parseMetronome( directionType );
                        break;
                    }
                    case core::DirectionType::Choice::octaveShift:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::harpPedals:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::damp:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::dampAll:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::eyeglasses:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::stringMute:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::scordatura:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::image:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::principalVoice:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::accordionRegistration:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::percussion:
                    {
                        break;
                    }
                    case core::DirectionType::Choice::otherDirection:
                    {
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        
        
        void DirectionReader::parseRehearsal( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseSegno( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseWords( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseCoda( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseWedge( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseDynamics( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseDashes( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseBracket( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parsePedal( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseMetronome( const core::DirectionType& directionType ) const
        {
            const auto& metronome = *directionType.getMetronome();
            MetronomeReader reader{ metronome };
            myOutMeasureDataP->tempos.emplace_back( reader.getTempoData() );
        }
        
        
        void DirectionReader::parseOctaveShift( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseHarpPedals( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseDamp( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseDampAll( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseEyeglasses( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseStringMute( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseScordatura( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseImage( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parsePrincipalVoice( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseAccordionRegistration( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parsePercussion( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseOtherDirection( const core::DirectionType& directionType ) const
        {
            MX_UNUSED( directionType );
        }
    }
}
