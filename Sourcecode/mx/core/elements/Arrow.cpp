// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Arrow.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/ArrowGroup.h"
#include "mx/core/elements/CircularArrow.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Arrow::Arrow()
        :myChoice( Choice::arrowGroup )
        ,myAttributes( std::make_shared<ArrowAttributes>() )
        ,myArrowGroup( makeArrowGroup() )
        ,myCircularArrow( makeCircularArrow() )
        {}


        bool Arrow::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Arrow::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Arrow::streamName( std::ostream& os ) const
        {
            os << "arrow";
            return os;
        }


        bool Arrow::hasContents() const
        {
            return true;
        }


        std::ostream& Arrow::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::arrowGroup:
                {
                    os << std::endl;
                    myArrowGroup->streamContents( os, indentLevel+1, isOneLineOnly );
                    os << std::endl;
                    isOneLineOnly = false;
                }
                    break;
                case Choice::circularArrow:
                {
                    os << std::endl;
                    myCircularArrow->toStream( os, indentLevel+1 );
                    os << std::endl;
                    isOneLineOnly = false;
                }
                    break;
                default:
                    break;
            }
            return os;
        }


        ArrowAttributesPtr Arrow::getAttributes() const
        {
            return myAttributes;
        }


        void Arrow::setAttributes( const ArrowAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        Arrow::Choice Arrow::getChoice() const
        {
            return myChoice;
        }


        void Arrow::setChoice( const Arrow::Choice value )
        {
            myChoice = value;
        }


        ArrowGroupPtr Arrow::getArrowGroup() const
        {
            return myArrowGroup;
        }


        void Arrow::setArrowGroup( const ArrowGroupPtr& value )
        {
            if( value )
            {
                myArrowGroup = value;
            }
        }


        CircularArrowPtr Arrow::getCircularArrow() const
        {
            return myCircularArrow;
        }


        void Arrow::setCircularArrow( const CircularArrowPtr& value )
        {
            if( value )
            {
                myCircularArrow = value;
            }
        }


        bool Arrow::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if( it->getName() == "circular-arrow" )
                {
                    myChoice = Choice::circularArrow;
                    isSuccess &= myCircularArrow->fromXElement( message, *it );
                    // this is a mistake I think MX_RETURN_IS_SUCCESS;
                }
                myChoice = Choice::arrowGroup;
                isSuccess = myArrowGroup->fromXElement( message, xelement );
                // this is a mistake I think MX_RETURN_IS_SUCCESS;
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
