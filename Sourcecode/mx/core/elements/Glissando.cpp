// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Glissando.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Glissando::Glissando()
        :myValue()
        ,myAttributes( std::make_shared<GlissandoAttributes>() )
        {}


        Glissando::Glissando( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<GlissandoAttributes>() )
        {}


        bool Glissando::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Glissando::hasContents() const
        {
            return true;
        }


        std::ostream& Glissando::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Glissando::streamName( std::ostream& os ) const
        {
            os << "glissando";
            return os;
        }


        std::ostream& Glissando::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        GlissandoAttributesPtr Glissando::getAttributes() const
        {
            return myAttributes;
        }


        void Glissando::setAttributes( const GlissandoAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Glissando::getValue() const
        {
            return myValue;
        }


        void Glissando::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Glissando::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
