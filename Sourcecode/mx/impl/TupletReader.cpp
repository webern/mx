// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/TupletReader.h"
#include "mx/core/elements/ActualNotes.h"
#include "mx/core/elements/NormalDot.h"
#include "mx/core/elements/NormalNotes.h"
#include "mx/core/elements/NormalType.h"
#include "mx/core/elements/NormalTypeNormalDotGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Tuplet.h"
#include "mx/core/elements/TupletActual.h"
#include "mx/core/elements/TupletDot.h"
#include "mx/core/elements/TupletNormal.h"
#include "mx/core/elements/TupletNumber.h"
#include "mx/core/elements/TupletType.h"
#include "mx/impl/PositionFunctions.h"

namespace mx
{
    namespace impl
    {
        TupletReader::TupletReader( const core::Tuplet& inMxTuplet, impl::Cursor inCursor, const core::Note& inNote )
        : myTuplet{ inMxTuplet }
        , myNote{ inNote }
        , myCursor{ inCursor }
        {
            
        }
        
        void TupletReader::parseTuplet( std::vector<api::TupletStart>& outTupletStarts, std::vector<api::TupletStop>& outTupletStops )
        {
            const auto& attr = *myTuplet.getAttributes();
            api::TupletStart tupletStart;
            tupletStart.positionData = getPositionData( attr );
            
            if( attr.hasNumber )
            {
                tupletStart.numberLevel = attr.number.getValue();
            }
 
            if( attr.type == core::StartStop::stop )
            {
                api::TupletStop tupletStop;
                tupletStop.positionData = tupletStart.positionData;
                tupletStop.numberLevel = tupletStart.numberLevel;
                outTupletStops.emplace_back( std::move( tupletStop ) );
                return;
            }
            
            if( attr.hasShowNumber )
            {
                switch ( attr.showNumber )
                {
                    case core::ShowTuplet::none:
                        tupletStart.showActualNumber = api::Bool::no;
                        tupletStart.showNormalNumber = api::Bool::no;
                        break;
                    case core::ShowTuplet::both:
                        tupletStart.showActualNumber = api::Bool::yes;
                        tupletStart.showNormalNumber = api::Bool::yes;
                        break;
                    case core::ShowTuplet::actual:
                        tupletStart.showActualNumber = api::Bool::yes;
                        tupletStart.showNormalNumber = api::Bool::no;
                        break;
                    default:
                        break;
                }
            }

            Converter converter;
            
            if( attr.hasBracket )
            {
                tupletStart.bracket = converter.convert( attr.bracket );
            }
            

            if( myTuplet.getHasTupletActual() )
            {
                const auto& actual = *myTuplet.getTupletActual();
                
                if( actual.getHasTupletNumber() )
                {
                    tupletStart.actualNumber = actual.getTupletNumber()->getValue().getValue();
                }
                
                if( actual.getHasTupletType() )
                {
                    tupletStart.actualDurationName = converter.convert( actual.getTupletType()->getValue() );
                }
                
                if( actual.getTupletDotSet().size() > 0 )
                {
                    tupletStart.actualDots = static_cast<int>( actual.getTupletDotSet().size() );
                }
            }
            else
            {
                guessActualFromNote( tupletStart );
            }
            
            if( myTuplet.getHasTupletNormal() )
            {
                const auto& normal = *myTuplet.getTupletNormal();
                
                if( normal.getHasTupletNumber() )
                {
                    tupletStart.normalNumber = normal.getTupletNumber()->getValue().getValue();
                }
                
                if( normal.getHasTupletType() )
                {
                    tupletStart.normalDurationName = converter.convert( normal.getTupletType()->getValue() );
                }
                
                if( normal.getTupletDotSet().size() > 0 )
                {
                    tupletStart.normalDots = static_cast<int>( normal.getTupletDotSet().size() );
                }
            }
            else
            {
                guessNormalFromNote( tupletStart );
            }
            
            outTupletStarts.emplace_back( std::move( tupletStart) );
        }
        
        
        void TupletReader::guessNormalFromNote( api::TupletStart& outTupletStart )
        {
            if( !myNote.getHasTimeModification() )
            {
                return;
            }
            const auto& timeMod = *myNote.getTimeModification();
            outTupletStart.actualNumber = timeMod.getActualNotes()->getValue().getValue();
            
            Converter converter;
            if( timeMod.getHasNormalTypeNormalDotGroup() )
            {
                const auto& grp = *timeMod.getNormalTypeNormalDotGroup();
                const auto durName = converter.convert( grp.getNormalType()->getValue() );
                outTupletStart.normalDurationName = durName;
                outTupletStart.normalDots = static_cast<int>( grp.getNormalDotSet().size() );
            }

        }
        
        
        void TupletReader::guessActualFromNote( api::TupletStart& outTupletStart )
        {
            if( !myNote.getHasTimeModification() )
            {
                return;
            }
            const auto& timeMod = *myNote.getTimeModification();
            outTupletStart.normalNumber = timeMod.getNormalNotes()->getValue().getValue();
            
            Converter converter;
            if( timeMod.getHasNormalTypeNormalDotGroup() )
            {
                const auto& grp = *timeMod.getNormalTypeNormalDotGroup();
                const auto durName = converter.convert( grp.getNormalType()->getValue() );
                outTupletStart.actualDurationName = durName;
                outTupletStart.actualDots = static_cast<int>( grp.getNormalDotSet().size() );
            }
        }
        
    }
}
