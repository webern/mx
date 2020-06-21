// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Suffix.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Suffix::Suffix()
        :myValue()
        ,myAttributes( std::make_shared<SuffixAttributes>() )
        {}


        Suffix::Suffix( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<SuffixAttributes>() )
        {}


        bool Suffix::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Suffix::hasContents() const
        {
            return true;
        }


        std::ostream& Suffix::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Suffix::streamName( std::ostream& os ) const
        {
            os << "suffix";
            return os;
        }


        std::ostream& Suffix::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        SuffixAttributesPtr Suffix::getAttributes() const
        {
            return myAttributes;
        }


        void Suffix::setAttributes( const SuffixAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Suffix::getValue() const
        {
            return myValue;
        }


        void Suffix::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Suffix::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
