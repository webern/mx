// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TimewiseMeasure.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/TimewisePart.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TimewiseMeasure::TimewiseMeasure()
        :myAttributes( std::make_shared<MeasureAttributes>() )
        ,myTimewisePartSet()
        {
            myTimewisePartSet.push_back( makeTimewisePart() );
        }


        bool TimewiseMeasure::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& TimewiseMeasure::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& TimewiseMeasure::streamName( std::ostream& os ) const
        {
            os << "measure";
            return os;
        }


        bool TimewiseMeasure::hasContents() const
        {
            return true;
        }


        std::ostream& TimewiseMeasure::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                for ( auto x : myTimewisePartSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        MeasureAttributesPtr TimewiseMeasure::getAttributes() const
        {
            return myAttributes;
        }


        void TimewiseMeasure::setAttributes( const MeasureAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const TimewisePartSet& TimewiseMeasure::getTimewisePartSet() const
        {
            return myTimewisePartSet;
        }


        void TimewiseMeasure::addTimewisePart( const TimewisePartPtr& value )
        {
            if ( value )
            {
                myTimewisePartSet.push_back( value );
            }
        }


        void TimewiseMeasure::removeTimewisePart( const TimewisePartSetIterConst& value )
        {
            if ( value != myTimewisePartSet.cend() )
            {
                myTimewisePartSet.erase( value );
            }
        }


        void TimewiseMeasure::clearTimewisePartSet()
        {
            myTimewisePartSet.clear();
        }


        TimewisePartPtr TimewiseMeasure::getTimewisePart( const TimewisePartSetIterConst& setIterator ) const
        {
            if( setIterator != myTimewisePartSet.cend() )
            {
                return *setIterator;
            }
            return TimewisePartPtr();
        }
        
        
        bool TimewiseMeasure::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            bool isFirstTimewisePartFound = false;
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                if( elementName != "part" )
                {
                    isSuccess = false;
                    message << "TimewiseMeasure: unexpected element '" << elementName << "' encountered" << std::endl;
                }
                else
                {
                    auto timewisePart = makeTimewisePart();
                    isSuccess &= timewisePart->fromXElement( message, *it );
                    
                    if( !isFirstTimewisePartFound && myTimewisePartSet.size() == 1 )
                    {
                        *( myTimewisePartSet.begin() ) = timewisePart;
                        isFirstTimewisePartFound = true;
                    }
                    else
                    {
                        myTimewisePartSet.push_back( timewisePart );
                        isFirstTimewisePartFound = true;
                    }
                }
            }
            
            if( !isFirstTimewisePartFound )
            {
                message << "TimewiseMeasure: no 'part' elements were found";
                isSuccess = false;
            }
            
            return isSuccess;
        }

    }
}
