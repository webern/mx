// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/AccidentalText.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        AccidentalText::AccidentalText()
        :myValue( AccidentalValue::natural )
        ,myAttributes( std::make_shared<AccidentalTextAttributes>() )
        {}


        AccidentalText::AccidentalText( const AccidentalValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<AccidentalTextAttributes>() )
        {}


        bool AccidentalText::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool AccidentalText::hasContents() const
        {
            return true;
        }


        std::ostream& AccidentalText::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& AccidentalText::streamName( std::ostream& os ) const
        {
            os << "accidental-text";
            return os;
        }


        std::ostream& AccidentalText::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        AccidentalTextAttributesPtr AccidentalText::getAttributes() const
        {
            return myAttributes;
        }


        void AccidentalText::setAttributes( const AccidentalTextAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        AccidentalValue AccidentalText::getValue() const
        {
            return myValue;
        }


        void AccidentalText::setValue( const AccidentalValue& value )
        {
            myValue = value;
        }


        bool AccidentalText::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseAccidentalValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
