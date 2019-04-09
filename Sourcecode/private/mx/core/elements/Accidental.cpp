// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Accidental.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Accidental::Accidental()
        :myValue( AccidentalValue::natural )
        ,myAttributes( std::make_shared<AccidentalAttributes>() )
        {}


        Accidental::Accidental( const AccidentalValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<AccidentalAttributes>() )
        {}


        bool Accidental::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Accidental::hasContents() const
        {
            return true;
        }


        std::ostream& Accidental::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Accidental::streamName( std::ostream& os ) const
        {
            os << "accidental";
            return os;
        }


        std::ostream& Accidental::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        AccidentalAttributesPtr Accidental::getAttributes() const
        {
            return myAttributes;
        }


        void Accidental::setAttributes( const AccidentalAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        AccidentalValue Accidental::getValue() const
        {
            return myValue;
        }


        void Accidental::setValue( const AccidentalValue& value )
        {
            myValue = value;
        }


        bool Accidental::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            if( xelement.getValue() == "double-flat" )
            {
                message << "Accidental: 'double-flat' is not a valid MusicXML 'accidental' value - converting it to 'flat-flat'" << std::endl;
                myValue = parseAccidentalValue( "flat-flat" );
            }
            else
            {
                myValue = parseAccidentalValue( xelement.getValue() );
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
