// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BassStep.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BassStep::BassStep()
        :myValue( StepEnum::a )
        ,myAttributes( std::make_shared<BassStepAttributes>() )
        {}


        BassStep::BassStep( const StepEnum& value )
        :myValue( value )
        ,myAttributes( std::make_shared<BassStepAttributes>() )
        {}


        bool BassStep::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool BassStep::hasContents() const
        {
            return true;
        }


        std::ostream& BassStep::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& BassStep::streamName( std::ostream& os ) const
        {
            os << "bass-step";
            return os;
        }


        std::ostream& BassStep::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        BassStepAttributesPtr BassStep::getAttributes() const
        {
            return myAttributes;
        }


        void BassStep::setAttributes( const BassStepAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        StepEnum BassStep::getValue() const
        {
            return myValue;
        }


        void BassStep::setValue( const StepEnum& value )
        {
            myValue = value;
        }


        bool BassStep::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseStepEnum( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
