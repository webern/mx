// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Ending.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Ending::Ending()
        :myValue()
        ,myAttributes( std::make_shared<EndingAttributes>() )
        {}


        Ending::Ending( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<EndingAttributes>() )
        {}


        bool Ending::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Ending::hasContents() const
        {
            return true;
        }


        std::ostream& Ending::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Ending::streamName( std::ostream& os ) const
        {
            os << "ending";
            return os;
        }


        std::ostream& Ending::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        EndingAttributesPtr Ending::getAttributes() const
        {
            return myAttributes;
        }


        void Ending::setAttributes( const EndingAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Ending::getValue() const
        {
            return myValue;
        }


        void Ending::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Ending::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
