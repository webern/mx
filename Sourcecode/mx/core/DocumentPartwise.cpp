// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/DocumentPartwise.h"
#include "mx/core/DocumentHeader.h"
#include "mx/core/DocumentTimewise.h"
#include <set>
#include "mx/core/Strings.h"
#include <algorithm>

using namespace mx::core;

namespace mx
{
    namespace core
    {
        DocumentPartwise::DocumentPartwise()
        :myScorePartwise( makeScorePartwise() )
        {}
        /* _________ ScorePartwise minOccurs = 1, maxOccurs = 1 _________ */
        ScorePartwisePtr DocumentPartwise::getScorePartwise() const
        {
            return myScorePartwise;
        }
        void DocumentPartwise::setScorePartwise( const ScorePartwisePtr& value )
        {
            if ( value )
            {
                myScorePartwise = value;
            }
        }
        std::ostream& DocumentPartwise::toStream( std::ostream& os ) const
        {
            DocumentHeader tempheader;
            tempheader.setDocumentType( DocumentType::partwise );
            tempheader.toStream( os );
            os << std::endl;
            myScorePartwise->toStream( os, 0 );
            return os;
        }
        DocumentPartwisePtr DocumentPartwise::convert( const DocumentTimewisePtr& input )
        {
            auto output = makeDocumentPartwise();
            auto score_old = input->getScoreTimewise();
            auto score_new = output->getScorePartwise();
            score_new->setScoreHeaderGroup( score_old->getScoreHeaderGroup() );
            auto attr_old = score_old->getAttributes();
            auto attr_new = score_new->getAttributes();
            attr_new->hasVersion = attr_old->hasVersion;
            attr_new->version = attr_old->version;
            PartwisePartSet pwParts;
            using Intz = TimewisePartSet::size_type;
            
            /* Create and add a ScorePartwisePart for each part in the main list */
            auto firstPart = makePartwisePart();
            firstPart->getAttributes()->id = score_old->getScoreHeaderGroup()->getPartList()->getScorePart()->getAttributes()->id;
            score_new->addPartwisePart( firstPart );
            score_new->removePartwisePart( score_new->getPartwisePartSet().cbegin() );
            for ( auto pgosp : score_old->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet() )
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
                /* inner loop: for each measure in score_old */
                bool isFirst = true;
                for ( auto m : score_old->getTimewiseMeasureSet() )
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
            
            return output;
        }
    }
}