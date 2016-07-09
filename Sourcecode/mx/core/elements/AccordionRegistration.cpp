// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccordionHigh.h"
#include "mx/core/elements/AccordionLow.h"
#include "mx/core/elements/AccordionMiddle.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        AccordionRegistration::AccordionRegistration()
        :myAttributes( std::make_shared<AccordionRegistrationAttributes>() )
        ,myAccordionHigh( makeAccordionHigh() )
        ,myHasAccordionHigh( false )
        ,myAccordionMiddle( makeAccordionMiddle() )
        ,myHasAccordionMiddle( false )
        ,myAccordionLow( makeAccordionLow() )
        ,myHasAccordionLow( false )
        {}


        bool AccordionRegistration::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& AccordionRegistration::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& AccordionRegistration::streamName( std::ostream& os ) const
        {
            os << "accordion-registration";
            return os;
        }


        bool AccordionRegistration::hasContents() const
        {
            return myHasAccordionHigh || myHasAccordionMiddle || myHasAccordionLow;
        }


        std::ostream& AccordionRegistration::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = !hasContents();
            if ( myHasAccordionHigh )
            {
                os << std::endl;
                myAccordionHigh->toStream( os, indentLevel+1 );
            }
            if ( myHasAccordionMiddle )
            {
                os << std::endl;
                myAccordionMiddle->toStream( os, indentLevel+1 );
            }
            if ( myHasAccordionLow )
            {
                os << std::endl;
                myAccordionLow->toStream( os, indentLevel+1 );
            }
            if ( hasContents() )
            {
                os << std::endl;
            }
            return os;
        }


        AccordionRegistrationAttributesPtr AccordionRegistration::getAttributes() const
        {
            return myAttributes;
        }


        void AccordionRegistration::setAttributes( const AccordionRegistrationAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        AccordionHighPtr AccordionRegistration::getAccordionHigh() const
        {
            return myAccordionHigh;
        }


        void AccordionRegistration::setAccordionHigh( const AccordionHighPtr& value )
        {
            if( value )
            {
                myAccordionHigh = value;
            }
        }


        bool AccordionRegistration::getHasAccordionHigh() const
        {
            return myHasAccordionHigh;
        }


        void AccordionRegistration::setHasAccordionHigh( const bool value )
        {
            myHasAccordionHigh = value;
        }


        AccordionMiddlePtr AccordionRegistration::getAccordionMiddle() const
        {
            return myAccordionMiddle;
        }


        void AccordionRegistration::setAccordionMiddle( const AccordionMiddlePtr& value )
        {
            if( value )
            {
                myAccordionMiddle = value;
            }
        }


        bool AccordionRegistration::getHasAccordionMiddle() const
        {
            return myHasAccordionMiddle;
        }


        void AccordionRegistration::setHasAccordionMiddle( const bool value )
        {
            myHasAccordionMiddle = value;
        }


        AccordionLowPtr AccordionRegistration::getAccordionLow() const
        {
            return myAccordionLow;
        }


        void AccordionRegistration::setAccordionLow( const AccordionLowPtr& value )
        {
            if( value )
            {
                myAccordionLow = value;
            }
        }


        bool AccordionRegistration::getHasAccordionLow() const
        {
            return myHasAccordionLow;
        }


        void AccordionRegistration::setHasAccordionLow( const bool value )
        {
            myHasAccordionLow = value;
        }


        bool AccordionRegistration::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myAccordionHigh, myHasAccordionHigh ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myAccordionMiddle, myHasAccordionMiddle ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myAccordionLow, myHasAccordionLow ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
