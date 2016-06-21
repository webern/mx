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
        class DocumentTimewise;
        using DocumentTimewisePtr = std::shared_ptr<DocumentTimewise>;
        
        class DocumentPartwise;
        using DocumentPartwisePtr = std::shared_ptr<DocumentPartwise>;
        inline DocumentPartwisePtr makeDocumentPartwise() { return std::make_shared<DocumentPartwise>(); }
        class DocumentPartwise
        {
        public:
            DocumentPartwise();
            /* _________ ScorePartwise minOccurs = 1, maxOccurs = 1 _________ */
            core::ScorePartwisePtr getScorePartwise() const;
            void setScorePartwise( const ScorePartwisePtr& value );
            std::ostream& toStream( std::ostream& os ) const;
            static DocumentPartwisePtr convert( const DocumentTimewisePtr& input );
        private:
            ScorePartwisePtr myScorePartwise;
        };
    }
}
