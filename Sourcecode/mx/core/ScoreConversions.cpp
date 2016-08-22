// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/ScoreConversions.h"
#include "mx/core/elements/TimewiseMeasure.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/TimewisePart.h"
#include "mx/core/elements/ScorePart.h"
#include "mx/core/elements/PartList.h"
#include "mx/core/elements/PartGroup.h"
#include "mx/core/elements/PartGroupOrScorePart.h"

#include <algorithm>

namespace mx
{
    namespace core
    {
        ScoreTimewisePtr partwiseTimewise( const ScorePartwisePtr& inScore )
        {
            auto score_new = makeScoreTimewise();
            score_new->setScoreHeaderGroup( inScore->getScoreHeaderGroup() );
            auto attr_old = inScore->getAttributes();
            auto attr_new = score_new->getAttributes();
            attr_new->hasVersion = attr_old->hasVersion;
            attr_new->version = attr_old->version;
            TimewiseMeasureSet twMeasures;
            using Intz = PartwiseMeasureSet::size_type;
            
            /* find max number of measures */
            Intz measureCount = 0;
            for ( auto p : inScore->getPartwisePartSet() )
            {
                if ( p->getPartwiseMeasureSet().size() > measureCount )
                {
                    measureCount = p->getPartwiseMeasureSet().size();
                }
            }
            
            /* Outer Loop, Once for each Measure */
            for ( Intz currentMeasureIndex = 0;
                  currentMeasureIndex < measureCount;
                  ++currentMeasureIndex )
            {
                TimewiseMeasurePtr newMeasure;
                if ( currentMeasureIndex == 0 )
                {
                    newMeasure = *( score_new->getTimewiseMeasureSet().cbegin() );
                }
                else
                {
                    newMeasure = makeTimewiseMeasure();
                    score_new->addTimewiseMeasure( newMeasure );
                }
                
                /* Create a simple list of parts from the score header */
                std::vector<ScorePartPtr> partlist;
                partlist.push_back( inScore->getScoreHeaderGroup()->getPartList()->getScorePart() );
                for ( auto pgosp : inScore->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet() )
                {
                    if ( pgosp->getChoice() == PartGroupOrScorePart::Choice::scorePart )
                    {
                        partlist.push_back( pgosp->getScorePart() );
                    }
                }
                
                /* First Nested Loop, for each Part */
                bool isFirst = true;
                for ( auto sp : partlist )
                {
                    /* Find the Correct Part */
                    auto partIter = std::find_if( inScore->getPartwisePartSet().cbegin(),
                                  inScore->getPartwisePartSet().cend(),
                                  [&]( const PartwisePartPtr& searchingForThis )
                    {
                        return searchingForThis->getAttributes()->id.getValue() == sp->getAttributes()->id.getValue();
                    });
                    if ( partIter != inScore->getPartwisePartSet().cend() )
                    {
                        auto measureIter = (*partIter)->getPartwiseMeasureSet().cbegin();
                        /* Find the Correct Measure Within The Part */
                        for( Intz x = 0;
                            x < currentMeasureIndex;
                            ++measureIter, ++x ) {}
                        
                        /* Create and add the Part Music to the outer loop's Measure */
                        TimewisePartPtr newPart;
                        if ( isFirst )
                        {
                            newPart = *( newMeasure->getTimewisePartSet().cbegin() );
                            isFirst = false;
                        }
                        else
                        {
                            newPart = makeTimewisePart();
                            newMeasure->addTimewisePart( newPart );
                        }
                        newPart->getAttributes()->id = (*partIter)->getAttributes()->id;
                        newPart->setMusicDataGroup( (*measureIter)->getMusicDataGroup() );
                        newMeasure->setAttributes( (*measureIter)->getAttributes() );
                    }
                }
            }
            return score_new;
        }


        ScorePartwisePtr timewisePartwise( const ScoreTimewisePtr& inScore )
        {
            auto score_new = makeScorePartwise();
            score_new->setScoreHeaderGroup( inScore->getScoreHeaderGroup() );
            auto attr_old = inScore->getAttributes();
            auto attr_new = score_new->getAttributes();
            attr_new->hasVersion = attr_old->hasVersion;
            attr_new->version = attr_old->version;
            PartwisePartSet pwParts;
            using Intz = TimewisePartSet::size_type;
            
            /* Create and add a ScorePartwisePart for each part in the main list */
            auto firstPart = makePartwisePart();
            firstPart->getAttributes()->id = inScore->getScoreHeaderGroup()->getPartList()->getScorePart()->getAttributes()->id;
            score_new->addPartwisePart( firstPart );
            score_new->removePartwisePart( score_new->getPartwisePartSet().cbegin() );
            for ( auto pgosp : inScore->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet() )
            {
                if ( pgosp->getChoice() == PartGroupOrScorePart::Choice::scorePart )
                {
                    auto additionalPart = makePartwisePart();
                    additionalPart->getAttributes()->id = pgosp->getScorePart()->getAttributes()->id;
                    score_new->addPartwisePart( additionalPart );
                }
            }
            /* outter loop: for each scorepart in score_new */
            for ( auto p : score_new->getPartwisePartSet() )
            {
                /* inner loop: for each measure in inScore */
                bool isFirst = true;
                for ( auto m : inScore->getTimewiseMeasureSet() )
                {
                    PartwiseMeasurePtr newMeasure;
                    if ( isFirst )
                    {
                        newMeasure = *( p->getPartwiseMeasureSet().cbegin() );
                        isFirst = false;
                    }
                    else
                    {
                        newMeasure = makePartwiseMeasure();
                        p->addPartwiseMeasure( newMeasure );
                    }
                    newMeasure->setAttributes( m->getAttributes() );
                    
                    /* find the correct music data */
                    auto partIter = std::find_if( m->getTimewisePartSet().cbegin(),
                                                  m->getTimewisePartSet().cend(),
                                                 [&]( const TimewisePartPtr& searchingForThis )
                    {
                        return searchingForThis->getAttributes()->id.getValue() == p->getAttributes()->id.getValue();
                    });
                    if ( partIter != m->getTimewisePartSet().cend() )
                    {
                        newMeasure->setMusicDataGroup( (*partIter)->getMusicDataGroup() );
                    }
                }
            }
            return score_new;
        }
    }
}
