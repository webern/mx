// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Words.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Words::Words()
        :myValue()
        ,myAttributes( std::make_shared<WordsAttributes>() )
        {}


        Words::Words( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<WordsAttributes>() )
        {}


        bool Words::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Words::hasContents() const
        {
            return true;
        }


        std::ostream& Words::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Words::streamName( std::ostream& os ) const
        {
            os << "words";
            return os;
        }


        std::ostream& Words::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        WordsAttributesPtr Words::getAttributes() const
        {
            return myAttributes;
        }


        void Words::setAttributes( const WordsAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Words::getValue() const
        {
            return myValue;
        }


        void Words::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Words::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
