// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/ScoreTimewise.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/elements/TimewiseMeasure.h"
#include "mx/core/FromXElement.h"
#include "mx/xml/XElement.h"
#include "mx/xml/XElementIterator.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ScoreTimewise::ScoreTimewise()
        :myAttributes( std::make_shared<ScoreTimewiseAttributes>() )
        ,myScoreHeaderGroup( makeScoreHeaderGroup() )
        ,myTimewiseMeasureSet()
        {
            myTimewiseMeasureSet.push_back( makeTimewiseMeasure() );
        }


        bool ScoreTimewise::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& ScoreTimewise::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& ScoreTimewise::streamName( std::ostream& os ) const
        {
            os << "score-timewise";
            return os;
        }


        bool ScoreTimewise::hasContents() const
        {
            return true;
        }


        std::ostream& ScoreTimewise::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myScoreHeaderGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            for ( auto x : myTimewiseMeasureSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        ScoreTimewiseAttributesPtr ScoreTimewise::getAttributes() const
        {
            return myAttributes;
        }


        void ScoreTimewise::setAttributes( const ScoreTimewiseAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        ScoreHeaderGroupPtr ScoreTimewise::getScoreHeaderGroup() const
        {
            return myScoreHeaderGroup;
        }


        void ScoreTimewise::setScoreHeaderGroup( const ScoreHeaderGroupPtr& value )
        {
            if ( value )
            {
                myScoreHeaderGroup = value;
            }
        }


        const TimewiseMeasureSet& ScoreTimewise::getTimewiseMeasureSet() const
        {
            return myTimewiseMeasureSet;
        }


        void ScoreTimewise::removeTimewiseMeasure( const TimewiseMeasureSetIterConst& value )
        {
            if ( value != myTimewiseMeasureSet.cend() )
            {
                if ( myTimewiseMeasureSet.size() > 1 )
                {
                    myTimewiseMeasureSet.erase( value );
                }
            }
        }


        void ScoreTimewise::addTimewiseMeasure( const TimewiseMeasurePtr& value )
        {
            if ( value )
            {
                myTimewiseMeasureSet.push_back( value );
            }
        }


        void ScoreTimewise::clearTimewiseMeasureSet()
        {
            myTimewiseMeasureSet.clear();
            myTimewiseMeasureSet.push_back( makeTimewiseMeasure() );
        }


        TimewiseMeasurePtr ScoreTimewise::getTimewiseMeasure( const TimewiseMeasureSetIterConst& setIterator ) const
        {
            if( setIterator != myTimewiseMeasureSet.cend() )
            {
                return *setIterator;
            }
            return TimewiseMeasurePtr();
        }
        
        bool ScoreTimewise::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isFirstMeasureAdded = false;

            isSuccess &= myAttributes->fromXElement( message, xelement );
            isSuccess &= myScoreHeaderGroup->fromXElement( message, xelement );
            
            auto it = xelement.begin();
            auto end = xelement.end();
            
            for( ; it != end; ++it )
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
                else if( elementName == "measure" )
                {
                    auto measure = makeTimewiseMeasure();
                    
                    if( !isFirstMeasureAdded && myTimewiseMeasureSet.size() == 1 )
                    {
                        *( myTimewiseMeasureSet.begin() ) = measure;
                        isFirstMeasureAdded = true;
                    }
                    else
                    {
                        myTimewiseMeasureSet.push_back( measure );
                        isFirstMeasureAdded = true;
                    }
                    
                    isSuccess &= measure->fromXElement( message, *it );
                }
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}


