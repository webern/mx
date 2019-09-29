// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Function.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Function::Function()
        :myValue()
        ,myAttributes( std::make_shared<FunctionAttributes>() )
        {}


        Function::Function( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<FunctionAttributes>() )
        {}


        bool Function::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Function::hasContents() const
        {
            return true;
        }


        std::ostream& Function::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Function::streamName( std::ostream& os ) const
        {
            os << "function";
            return os;
        }


        std::ostream& Function::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FunctionAttributesPtr Function::getAttributes() const
        {
            return myAttributes;
        }


        void Function::setAttributes( const FunctionAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Function::getValue() const
        {
            return myValue;
        }


        void Function::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Function::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
