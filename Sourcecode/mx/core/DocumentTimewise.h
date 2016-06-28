// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include <memory>
#include "mx/core/Elements.h"

namespace mx
{
    namespace core
    {
        class DocumentPartwise;
        using DocumentPartwisePtr = std::shared_ptr<DocumentPartwise>;
        
        class DocumentTimewise;
        using DocumentTimewisePtr = std::shared_ptr<DocumentTimewise>;
        inline DocumentTimewisePtr makeDocumentTimewise() { return std::make_shared<DocumentTimewise>(); }
        
        class DocumentTimewise
        {
        public:
            DocumentTimewise();
            /* _________ ScoreTimewise minOccurs = 1, maxOccurs = 1 _________ */
            ScoreTimewisePtr getScoreTimewise() const;
            void setScoreTimewise( const ScoreTimewisePtr& value );
            std::ostream& toStream( std::ostream& os ) const;
            static DocumentTimewisePtr convert( const DocumentPartwisePtr& input );
        private:
            ScoreTimewisePtr myScoreTimewise;
        };
    }
}