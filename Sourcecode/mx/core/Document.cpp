// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Document.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XAttribute.h"
#include "mx/xml/XElementIterator.h"
#include "mx/xml/XAttributeIterator.h"
#include "mx/core/ScoreConversions.h"

using namespace mx::xml;

namespace mx
{
    namespace core
    {
        
        DocumentPtr makeDocument()
        {
            return std::make_shared<Document>();
        }
        
        
        DocumentPtr makeDocument( DocumentChoice choice )
        {
            return std::make_shared<Document>( choice );
        }
        
        
        Document::Document()
        : myChoice( DEFAULT_DOCUMENT_CHOICE )
        , myScorePartwise( makeScorePartwise() )
        , myScoreTimewise( makeScoreTimewise() )
        {
            
        }
        
        
        Document::Document( DocumentChoice choice )
        : myChoice( choice )
        , myScorePartwise( makeScorePartwise() )
        , myScoreTimewise( makeScoreTimewise() )
        {
            
        }
        
        
        void Document::setChoice( DocumentChoice choice )
        {
            myChoice = choice;
        }
        
        
        DocumentChoice Document::getChoice() const
        {
            return myChoice;
        }
        
        
        void Document::convertContents()
        {
            switch ( myChoice )
            {
                case DocumentChoice::partwise:
                {
                    myScoreTimewise = partwiseTimewise( myScorePartwise );
                    myScorePartwise = makeScorePartwise();
                    myChoice = DocumentChoice::timewise;
                }
                    break;
                case DocumentChoice::timewise:
                {
                    myScorePartwise = timewisePartwise( myScoreTimewise );
                    myScoreTimewise = makeScoreTimewise();
                    myChoice = DocumentChoice::partwise;
                }
                    break;
                default:
                    break;
            }
        }
        
        
        ScorePartwisePtr Document::getScorePartwise() const
        {
            return myScorePartwise;
        }
        
        
        void Document::setScorePartwise( const ScorePartwisePtr& value )
        {
            if( value )
            {
                myScorePartwise = value;
            }
        }
        
        
        ScoreTimewisePtr Document::getScoreTimewise() const
        {
            return myScoreTimewise;
        }
        
        
        void Document::setScoreTimewise( const ScoreTimewisePtr& value )
        {
            if( value )
            {
                myScoreTimewise = value;
            }
        }
        
        
        std::ostream& Document::toStream( std::ostream& os ) const
        {
            os << DEFAULT_XML_DECLARATION << std::endl;
            switch ( myChoice )
            {
                case DocumentChoice::partwise:
                {
                    os << "<!DOCTYPE " << DOCTYPE_VALUE_SCORE_PARTWISE << ">" << std::endl;
                    myScorePartwise->toStream( os, 0 );
                }
                    break;
                case DocumentChoice::timewise:
                {
                    os << "<!DOCTYPE " << DOCTYPE_VALUE_SCORE_TIMEWISE << ">" << std::endl;
                    myScoreTimewise->toStream( os, 0 );
                }
                    break;
                default:
                    MX_THROW( "myChoice had a bad value" )
            }
            return os;
        }
        
        
        void Document::toXDoc( xml::XDoc& outXDoc ) const
        {
            std::stringstream ss;
            toStream( ss );
            std::istringstream is( ss.str() );
            
            outXDoc.loadStream( is );
            switch ( myChoice )
            {
                case DocumentChoice::partwise:
                {
                    outXDoc.setDoctypeValue( DOCTYPE_VALUE_SCORE_PARTWISE );
                }
                    break;
                case DocumentChoice::timewise:
                {
                    outXDoc.setDoctypeValue( DOCTYPE_VALUE_SCORE_TIMEWISE );
                }
                    break;
                default:
                    MX_THROW( "myChoice had a bad value" )
            }
        }
        
        
        bool Document::fromXDoc( std::ostream& messages, const xml::XDoc& inXDoc )
        {
            auto root = inXDoc.getRoot();
            if( inXDoc.getRoot()->getName() == "score-partwise" )
            {
                myChoice = DocumentChoice::partwise;
                myScorePartwise = makeScorePartwise();
                return myScorePartwise->fromXElement( messages, *root );
            }
            else if( inXDoc.getRoot()->getName() == "score-timewise" )
            {
                myChoice = DocumentChoice::timewise;
                myScoreTimewise = makeScoreTimewise();
                return myScoreTimewise->fromXElement( messages, *root );
            }
            else
            {
                MX_THROW( "bad input - could not find the score-timewise or score-partwise node" );
            }          
        }
        
    }
}
