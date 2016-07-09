// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include <memory>
#include "mx/core/Elements.h"

// Forward Declarations /////////////////////////////////////////////////////

namespace mx
{
    namespace core
    {
        class Document;
        using DocumentPtr = std::shared_ptr<Document>;
        
        class ScorePartwise;
        using ScorePartwisePtr = std::shared_ptr<ScorePartwise>;
        
        class ScoreTimewise;
        using ScoreTimewisePtr = std::shared_ptr<ScoreTimewise>;
    }
    
    namespace xml
    {
        class XDoc;
        using XDocPtr = std::shared_ptr<XDoc>;
    }


////////////////////////////////////////////////////////////////////////////

    namespace core
    {
        constexpr const char* const DEFAULT_XML_DECLARATION = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
        constexpr const char* const DOCTYPE_VALUE_SCORE_PARTWISE = "score-partwise PUBLIC \"-//Recordare//DTD MusicXML 3.0 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\"";
        constexpr const char* const DOCTYPE_VALUE_SCORE_TIMEWISE = "score-timewise PUBLIC \"-//Recordare//DTD MusicXML 3.0 Timewise//EN\" \"http://www.musicxml.org/dtds/timewise.dtd\"";
        
        enum class DocumentChoice
        {
            partwise,
            timewise
        };

        constexpr const DocumentChoice DEFAULT_DOCUMENT_CHOICE = DocumentChoice::partwise;
        
        DocumentPtr makeDocument();
        DocumentPtr makeDocument( DocumentChoice choice );
        
        class Document
        {
        public:
            Document();
            Document( DocumentChoice choice );

            // indicates which type of musicxml document is held
            // setting the choice does not alter the contents of
            // ScorePartwise or ScoreTimewise object, is just
            // indicates which object is being used and which
            // object will be written during toStream/toXDoc
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
            
            // you should use these functions if choice == partwise
            ScorePartwisePtr getScorePartwise() const;
            void setScorePartwise( const ScorePartwisePtr& value );

            // you should use these functions if choice == timewise
            ScoreTimewisePtr getScoreTimewise() const;
            void setScoreTimewise( const ScoreTimewisePtr& value );

            std::ostream& toStream( std::ostream& os ) const;
            void toXDoc( xml::XDoc& outXDoc ) const;
            bool fromXDoc( std::ostream& messages, const xml::XDoc& inXDoc );

        private:
            DocumentChoice myChoice;
            ScorePartwisePtr myScorePartwise;
            ScoreTimewisePtr myScoreTimewise;
        };
    }
}
