// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include <memory>
#include "mx/core/DocumentSpec.h"
#include "mx/core/Elements.h"

namespace mx
{
    namespace core
    {
        class Document;
        using DocumentPtr = std::shared_ptr<Document>;
        
        //enum class DocumentChoice
        //{
        //    partwise,
        //    timewise
        //};

        DocumentPtr makeDocument();
        DocumentPtr makeDocument( DocumentChoice choice );
        
        class Document
        {
        public:
            Document();
            Document( DocumentChoice t );

            // indicates which type of musicxml document is held
            // setting the choice does not alter the contents of
            // ScorePartwise or ScoreTimewise object, is just
            // indicates which object is being used and which
            // object will be written during toStream
            void setChoice( DocumentChoice choice );
            DocumentChoice getChoice() const;
            
            // switches the DocumentChoice from partwise to
            // timewise or vice versa and converts the contents
            // over to the new type. For example if choice is
            // currently partwise and there is data in the
            // ScorePartwise object, the choice will be changed
            // to timewise, the ScoreTimewise object will be
            // populated from the contents of ScorePartwise,
            // and ScorePartwise will become empty
            void convertContents();
            
            // you should use these functions choice == partwise
            ScorePartwisePtr getScorePartwise() const;
            void setScorePartwise( const ScorePartwisePtr& value );

            // you should use these functions choice == timewise
            ScoreTimewisePtr getScoreTimewise() const;
            void setScoreTimewise( const ScoreTimewisePtr& value );

            std::ostream& toStream( std::ostream& os ) const;


        private:
            ScorePartwisePtr myScorePartwise;
            ScoreTimewisePtr myScoreTimewise;
        };
    }
}
