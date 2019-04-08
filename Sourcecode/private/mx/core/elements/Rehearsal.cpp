// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Rehearsal.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Rehearsal::Rehearsal()
        :myValue()
        ,myAttributes( std::make_shared<RehearsalAttributes>() )
        {}


        Rehearsal::Rehearsal( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<RehearsalAttributes>() )
        {}


        bool Rehearsal::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Rehearsal::hasContents() const
        {
            return true;
        }


        std::ostream& Rehearsal::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Rehearsal::streamName( std::ostream& os ) const
        {
            os << "rehearsal";
            return os;
        }


        std::ostream& Rehearsal::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        RehearsalAttributesPtr Rehearsal::getAttributes() const
        {
            return myAttributes;
        }


        void Rehearsal::setAttributes( const RehearsalAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Rehearsal::getValue() const
        {
            return myValue;
        }


        void Rehearsal::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Rehearsal::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
