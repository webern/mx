// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/Document.h"
#include "mx/core/Elements.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"
#include "mx/xml/XElement.h"
#include "mx/xml/XAttribute.h"
#include "mx/xml/XElementIterator.h"
#include "mx/xml/XAttributeIterator.h"
#include "mx/utility/Throw.h"

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
        : myChoice( DEFAULT_DOCUMENT_CHOICE )
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
            // TODO - get the code from DocumentPartwise and DocumentTimewise
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
                    os << DOCTYPE_SCORE_PARTWISE << std::endl;
                    myScorePartwise->toStream( os, 0 );
                }
                    break;
                case DocumentChoice::timewise:
                {
                    os << DOCTYPE_SCORE_TIMEWISE << std::endl;
                    myScoreTimewise->toStream( os, 0 );
                }
                    break;
                default:
                    MX_THROW( "myChoice had a bad value" )
                    break;
            }
            return os;
        }
        
        
        void Document::toXDoc( xml::XDoc& outXDoc ) const
        {
            std::stringstream ss;
            toStream( ss );
            std::istringstream is( ss.str() );
            outXDoc.parse( is );
            switch ( myChoice )
            {
                case DocumentChoice::partwise:
                {
                    outXDoc.setDoctypeValue( DOCTYPE_SCORE_PARTWISE );
                }
                    break;
                case DocumentChoice::timewise:
                {
                    outXDoc.setDoctypeValue( DOCTYPE_SCORE_TIMEWISE );
                }
                    break;
                default:
                    MX_THROW( "myChoice had a bad value" )
                    break;
            }
        }
        
        
        bool Document::fromXDoc( std::ostream& messages, const xml::XDoc& inXDoc )
        {
            auto root = inXDoc.getRoot();
            if( inXDoc.getRoot()->getName() == "score-partwise" )
            {
                myScorePartwise = makeScorePartwise();
                return myScorePartwise->fromXElement( messages, *root );
            }
            else if( inXDoc.getRoot()->getName() == "score-timewise" )
            {
                myScorePartwise = makeScorePartwise();
                return myScorePartwise->fromXElement( messages, *root );
            }
            else
            {
                MX_THROW( "bad input - could not find the score-timewise or score-partwise node" );
            }
            return true;            
        }
        
    }
}