// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ScorePartwise::ScorePartwise()
        :myAttributes( std::make_shared<ScorePartwiseAttributes>() )
        ,myScoreHeaderGroup( makeScoreHeaderGroup() )
        ,myPartwisePartSet()
        {
            myPartwisePartSet.push_back( makePartwisePart() );
        }


        bool ScorePartwise::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& ScorePartwise::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& ScorePartwise::streamName( std::ostream& os ) const
        {
            os << "score-partwise";
            return os;
        }


        bool ScorePartwise::hasContents() const
        {
            return true;
        }


        std::ostream& ScorePartwise::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myScoreHeaderGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            for ( auto x : myPartwisePartSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        ScorePartwiseAttributesPtr ScorePartwise::getAttributes() const
        {
            return myAttributes;
        }


        void ScorePartwise::setAttributes( const ScorePartwiseAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        ScoreHeaderGroupPtr ScorePartwise::getScoreHeaderGroup() const
        {
            return myScoreHeaderGroup;
        }


        void ScorePartwise::setScoreHeaderGroup( const ScoreHeaderGroupPtr& value )
        {
            if ( value )
            {
                myScoreHeaderGroup = value;
            }
        }


        const PartwisePartSet& ScorePartwise::getPartwisePartSet() const
        {
            return myPartwisePartSet;
        }


        void ScorePartwise::removePartwisePart( const PartwisePartSetIterConst& value )
        {
            if ( value != myPartwisePartSet.cend() )
            {
                if ( myPartwisePartSet.size() > 1 )
                {
                    myPartwisePartSet.erase( value );
                }
            }
        }


        void ScorePartwise::addPartwisePart( const PartwisePartPtr& value )
        {
            if ( value )
            {
                myPartwisePartSet.push_back( value );
            }
        }


        void ScorePartwise::clearPartwisePartSet()
        {
            myPartwisePartSet.clear();
            myPartwisePartSet.push_back( makePartwisePart() );
        }


        PartwisePartPtr ScorePartwise::getPartwisePart( const PartwisePartSetIterConst& setIterator ) const
        {
            if( setIterator != myPartwisePartSet.cend() )
            {
                return *setIterator;
            }
            return PartwisePartPtr();
        }
        
        
        bool ScorePartwise::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isFirstPartAdded = false;
            
            isSuccess &= myAttributes->fromXElement( message, xelement );
            isSuccess &= myScoreHeaderGroup->fromXElement( message, xelement );
            
            auto it = xelement.begin();
            auto end = xelement.end();
            
            for( ; it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                if( elementName == "work"
                   || elementName == "movement-number"
                   || elementName == "movement-title"
                   || elementName == "identification"
                   || elementName == "defaults"
                   || elementName == "credit" )
                {
                    continue;
                }
                else if( elementName == "part" )
                {
                    auto part = makePartwisePart();
                    
                    if( !isFirstPartAdded && myPartwisePartSet.size() == 1 )
                    {
                        *( myPartwisePartSet.begin() ) = part;
                        isFirstPartAdded = true;
                    }
                    else
                    {
                        myPartwisePartSet.push_back( part );
                        isFirstPartAdded = true;
                    }
                    
                    isSuccess &= part->fromXElement( message, *it );
                }
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
