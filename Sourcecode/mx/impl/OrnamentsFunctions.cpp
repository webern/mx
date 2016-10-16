// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/OrnamentsFunctions.h"
#include "mx/core/elements/DelayedInvertedTurn.h"
#include "mx/core/elements/DelayedTurn.h"
#include "mx/core/elements/InvertedMordent.h"
#include "mx/core/elements/InvertedTurn.h"
#include "mx/core/elements/Mordent.h"
#include "mx/core/elements/Ornaments.h"
#include "mx/core/elements/OrnamentsChoice.h"
#include "mx/core/elements/OtherOrnament.h"
#include "mx/core/elements/Schleifer.h"
#include "mx/core/elements/Shake.h"
#include "mx/core/elements/Tremolo.h"
#include "mx/core/elements/TrillMark.h"
#include "mx/core/elements/Turn.h"
#include "mx/core/elements/VerticalTurn.h"
#include "mx/core/elements/WavyLine.h"
#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/impl/ParseMarkDataAttributes.h"

namespace mx
{
    namespace impl
    {
        OrnamentsFunctions::OrnamentsFunctions( const core::Ornaments& inOrnaments, impl::Cursor inCursor )
        : myOrnaments{ inOrnaments }
        , myCursor{ inCursor }
        {
            
        }
        
        void OrnamentsFunctions::parseOrnaments( std::vector<api::MarkData>& outMarks ) const
        {
            parseOrnamentsSet( outMarks );
            parseAccidentalMarkSet( outMarks );
        }
        
        
        void OrnamentsFunctions::parseOrnamentsSet( std::vector<api::MarkData>& outMarks ) const
        {
            for( const auto& ornament : myOrnaments.getOrnamentsChoiceSet() )
            {
                const auto ornamentType = ornament->getChoice();
                Converter converter;
                const auto markType = converter.convertOrnament( ornamentType );
                auto markData = api::MarkData{};
                markData.markType = markType;
                markData.tickTimePosition = myCursor.tickTimePosition;
                
                parseOrnament( *ornament, markData );
                
                markData.smuflName = api::MarkSmufl::getName( markType, markData.positionData.placement );
                markData.smuflCodepoint = api::MarkSmufl::getCodepoint( markType, markData.positionData.placement );
                outMarks.emplace_back( std::move( markData ) );
            }
        }
        
        
        void OrnamentsFunctions::parseAccidentalMarkSet( std::vector<api::MarkData>& outMarks ) const
        {
            for( const auto& am : myOrnaments.getAccidentalMarkSet() )
            {
                AccidentalMarkFunctions funcs{ *am, myCursor };
                outMarks.emplace_back( funcs.parseAccidentalMark() );
            }
        }

        
        void OrnamentsFunctions::parseOrnament( const core::OrnamentsChoice& choiceObj, api::MarkData& outMark ) const
        {
            switch ( choiceObj.getChoice() )
            {
                case core::OrnamentsChoice::Choice::trillMark:
                {
                    outMark.name = "trill-mark";
                    parseMarkDataAttributes( *choiceObj.getTrillMark()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::turn:
                {
                    outMark.name = "turn";
                    parseMarkDataAttributes( *choiceObj.getTurn()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::delayedTurn:
                {
                    outMark.name = "delayed-turn";
                    parseMarkDataAttributes( *choiceObj.getDelayedTurn()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::invertedTurn:
                {
                    outMark.name = "inverted-turn";
                    parseMarkDataAttributes( *choiceObj.getInvertedTurn()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::delayedInvertedTurn:
                {
                    outMark.name = "delayed-inverted-turn";
                    parseMarkDataAttributes( *choiceObj.getDelayedInvertedTurn()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::verticalTurn:
                {
                    outMark.name = "vertical-turn";
                    parseMarkDataAttributes( *choiceObj.getVerticalTurn()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::shake:
                {
                    outMark.name = "shake";
                    parseMarkDataAttributes( *choiceObj.getShake()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::wavyLine:
                {
                    outMark.name = "wavy-line";
                    parseMarkDataAttributes( *choiceObj.getWavyLine()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::mordent:
                {
                    outMark.name = "mordent";
                    parseMarkDataAttributes( *choiceObj.getMordent()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::invertedMordent:
                {
                    outMark.name = "inverted-mordent";
                    parseMarkDataAttributes( *choiceObj.getInvertedMordent()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::schleifer:
                {
                    outMark.name = "schleifer";
                    parseMarkDataAttributes( *choiceObj.getSchleifer()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::tremolo:
                {
                    outMark.name = "tremolo";
                    parseMarkDataAttributes( *choiceObj.getTremolo()->getAttributes(), outMark );
                    break;
                }
                case core::OrnamentsChoice::Choice::otherOrnament:
                {
                    const auto value = choiceObj.getOtherOrnament()->getValue().getValue();
                    parseMarkDataAttributes( *choiceObj.getOtherOrnament()->getAttributes(), outMark );
                    
                    if( value.empty() )
                    {
                        outMark.name = "other-ornament";
                    }
                    else
                    {
                        outMark.name = value;
                    }
                    
                    const auto tempChar = api::Smufl::findCodepoint( value );
                    
                    if( tempChar != 0 )
                    {
                        outMark.smuflName = value;
                        outMark.smuflCodepoint = tempChar;
                    }

                    break;
                }
                default:
                    break;
            }
        }
    }
}
