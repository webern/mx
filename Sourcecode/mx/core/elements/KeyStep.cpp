// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/KeyStep.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        KeyStep::KeyStep()
        :myValue( StepEnum::a )
        {}


        KeyStep::KeyStep( const StepEnum& value )
        :myValue( value )
        {}


        bool KeyStep::hasAttributes() const
        {
            return false;
        }


        bool KeyStep::hasContents() const
        {
            return true;
        }


        std::ostream& KeyStep::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& KeyStep::streamName( std::ostream& os ) const
        {
            os << "key-step";
            return os;
        }


        std::ostream& KeyStep::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StepEnum KeyStep::getValue() const
        {
            return myValue;
        }


        void KeyStep::setValue( const StepEnum& value )
        {
            myValue = value;
        }


        bool KeyStep::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue = parseStepEnum( xelement.getValue() );
            return true;
        }

    }
}
