// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Footnote.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Footnote::Footnote()
        :myValue()
        ,myAttributes( std::make_shared<FootnoteAttributes>() )
        {}


        Footnote::Footnote( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<FootnoteAttributes>() )
        {}


        bool Footnote::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Footnote::hasContents() const
        {
            return true;
        }


        std::ostream& Footnote::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Footnote::streamName( std::ostream& os ) const
        {
            os << "footnote";
            return os;
        }


        std::ostream& Footnote::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FootnoteAttributesPtr Footnote::getAttributes() const
        {
            return myAttributes;
        }


        void Footnote::setAttributes( const FootnoteAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Footnote::getValue() const
        {
            return myValue;
        }


        void Footnote::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Footnote::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
