// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DisplayStep.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DisplayStep::DisplayStep()
        :myValue( StepEnum::a )
        {}


        DisplayStep::DisplayStep( const StepEnum& value )
        :myValue( value )
        {}


        bool DisplayStep::hasAttributes() const
        {
            return false;
        }


        bool DisplayStep::hasContents() const
        {
            return true;
        }


        std::ostream& DisplayStep::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& DisplayStep::streamName( std::ostream& os ) const
        {
            os << "display-step";
            return os;
        }


        std::ostream& DisplayStep::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StepEnum DisplayStep::getValue() const
        {
            return myValue;
        }


        void DisplayStep::setValue( const StepEnum& value )
        {
            myValue = value;
        }


        bool DisplayStep::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseStepEnum( xelement.getValue() );
            return true;
        }

    }
}
