// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/ArrowGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/ArrowDirection.h"
#include "mx/core/elements/ArrowStyle.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ArrowGroup::ArrowGroup()
        :myArrowDirection( makeArrowDirection() )
        ,myArrowStyle( makeArrowStyle() )
        ,myHasArrowStyle( false )
        {}


        bool ArrowGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& ArrowGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& ArrowGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool ArrowGroup::hasContents() const
        {
            return true;
        }


        std::ostream& ArrowGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            myArrowDirection->toStream( os, indentLevel );
            if ( myHasArrowStyle )
            {
                os << std::endl;
                myArrowStyle->toStream( os, indentLevel );
            }
            isOneLineOnly = false;
            return os;
        }


        ArrowDirectionPtr ArrowGroup::getArrowDirection() const
        {
            return myArrowDirection;
        }


        void ArrowGroup::setArrowDirection( const ArrowDirectionPtr& value )
        {
            if ( value )
            {
                myArrowDirection = value;
            }
        }


        ArrowStylePtr ArrowGroup::getArrowStyle() const
        {
            return myArrowStyle;
        }


        void ArrowGroup::setArrowStyle( const ArrowStylePtr& value )
        {
            if ( value )
            {
                myArrowStyle = value;
            }
        }


        bool ArrowGroup::getHasArrowStyle() const
        {
            return myHasArrowStyle;
        }


        void ArrowGroup::setHasArrowStyle( const bool value )
        {
            myHasArrowStyle = value;
        }
        
        
        bool ArrowGroup::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isArrowDirectionFound = false;
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                if( elementName == "arrow-direction" )
                {
                    isArrowDirectionFound = true;
                    isSuccess &= myArrowDirection->fromXElement( message, *it );
                }
                else if( elementName == "arrow-style" )
                {
                    myHasArrowStyle = true;
                    isSuccess &= myArrowStyle->fromXElement( message, *it );
                }
                else
                {
                    if( !isArrowDirectionFound )
                    {
                        isSuccess = false;
                        message << "ArrowGroup: 'arrow-direction' element is required but was not found" << std::endl;
                    }
                    break;
                }
            }
            MX_RETURN_IS_SUCCESS;
        }
        
    }
}
