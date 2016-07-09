// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Beater.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Beater::Beater()
        :myValue( BeaterValue::snareStick )
        ,myAttributes( std::make_shared<BeaterAttributes>() )
        {}


        Beater::Beater( const BeaterValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<BeaterAttributes>() )
        {}


        bool Beater::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Beater::hasContents() const
        {
            return true;
        }


        std::ostream& Beater::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Beater::streamName( std::ostream& os ) const
        {
            os << "beater";
            return os;
        }


        std::ostream& Beater::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        BeaterAttributesPtr Beater::getAttributes() const
        {
            return myAttributes;
        }


        void Beater::setAttributes( const BeaterAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        BeaterValue Beater::getValue() const
        {
            return myValue;
        }


        void Beater::setValue( const BeaterValue& value )
        {
            myValue = value;
        }


        bool Beater::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseBeaterValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
