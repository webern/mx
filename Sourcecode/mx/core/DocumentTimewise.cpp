// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/DocumentTimewise.h"
#include "mx/core/DocumentHeader.h"
#include "mx/core/DocumentPartwise.h"
#include <algorithm>

using namespace mx::core;
namespace mx
{
    namespace core
    {
        DocumentTimewise::DocumentTimewise()
        :myScoreTimewise( makeScoreTimewise() )
        {}
        /* _________ ScoreTimewise minOccurs = 1, maxOccurs = 1 _________ */
        ScoreTimewisePtr DocumentTimewise::getScoreTimewise() const
        {
            return myScoreTimewise;
        }
        void DocumentTimewise::setScoreTimewise( const ScoreTimewisePtr& value )
        {
            if ( value )
            {
                myScoreTimewise = value;
            }
        }
        std::ostream& DocumentTimewise::toStream( std::ostream& os ) const
        {
            DocumentHeader tempheader;
            tempheader.setDocumentType( DocumentType::timewise );
            tempheader.toStream( os );
            os << std::endl;
            myScoreTimewise->toStream( os, 0 );
            return os;
        }
        DocumentTimewisePtr DocumentTimewise::convert( const DocumentPartwisePtr& input )
        {
            auto output = makeDocumentTimewise();
            auto score_old = input->getScorePartwise();
            auto score_new = output->getScoreTimewise();
            score_new->setScoreHeaderGroup( score_old->getScoreHeaderGroup() );
            auto attr_old = score_old->getAttributes();
            auto attr_new = score_new->getAttributes();
            attr_new->hasVersion = attr_old->hasVersion;
            attr_new->version = attr_old->version;
            TimewiseMeasureSet twMeasures;
            using Intz = PartwiseMeasureSet::size_type;
            
            /* find max number of measures */
            Intz measureCount = 0;
            for ( auto p : score_old->getPartwisePartSet() )
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
                partlist.push_back( score_old->getScoreHeaderGroup()->getPartList()->getScorePart() );
                for ( auto pgosp : score_old->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet() )
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
                    auto partIter = std::find_if( score_old->getPartwisePartSet().cbegin(),
                                  score_old->getPartwisePartSet().cend(),
                                  [&]( const PartwisePartPtr& searchingForThis )
                    {
                        return searchingForThis->getAttributes()->id.getValue() == sp->getAttributes()->id.getValue();
                    });
                    if ( partIter != score_old->getPartwisePartSet().cend() )
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
            
            return output;
        }
    }
}