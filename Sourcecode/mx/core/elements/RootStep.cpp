// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/RootStep.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        RootStep::RootStep()
        :myValue( StepEnum::a )
        ,myAttributes( std::make_shared<RootStepAttributes>() )
        {}


        RootStep::RootStep( const StepEnum& value )
        :myValue( value )
        ,myAttributes( std::make_shared<RootStepAttributes>() )
        {}


        bool RootStep::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool RootStep::hasContents() const
        {
            return true;
        }


        std::ostream& RootStep::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& RootStep::streamName( std::ostream& os ) const
        {
            os << "root-step";
            return os;
        }


        std::ostream& RootStep::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        RootStepAttributesPtr RootStep::getAttributes() const
        {
            return myAttributes;
        }


        void RootStep::setAttributes( const RootStepAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        StepEnum RootStep::getValue() const
        {
            return myValue;
        }


        void RootStep::setValue( const StepEnum& value )
        {
            myValue = value;
        }


        bool RootStep::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseStepEnum( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
