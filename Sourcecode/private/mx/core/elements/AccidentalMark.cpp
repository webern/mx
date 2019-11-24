// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/AccidentalMark.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        AccidentalMark::AccidentalMark()
        :myValue( AccidentalValue::natural )
        ,myAttributes( std::make_shared<AccidentalMarkAttributes>() )
        {}


        AccidentalMark::AccidentalMark( const AccidentalValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<AccidentalMarkAttributes>() )
        {}


        bool AccidentalMark::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool AccidentalMark::hasContents() const
        {
            return true;
        }


        std::ostream& AccidentalMark::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& AccidentalMark::streamName( std::ostream& os ) const
        {
            os << "accidental-mark";
            return os;
        }


        std::ostream& AccidentalMark::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        AccidentalMarkAttributesPtr AccidentalMark::getAttributes() const
        {
            return myAttributes;
        }


        void AccidentalMark::setAttributes( const AccidentalMarkAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        AccidentalValue AccidentalMark::getValue() const
        {
            return myValue;
        }


        void AccidentalMark::setValue( const AccidentalValue& value )
        {
            myValue = value;
        }


        bool AccidentalMark::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseAccidentalValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
