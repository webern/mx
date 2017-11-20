// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PrincipalVoice::PrincipalVoice()
        :myValue()
        ,myAttributes( std::make_shared<PrincipalVoiceAttributes>() )
        {}


        PrincipalVoice::PrincipalVoice( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<PrincipalVoiceAttributes>() )
        {}


        bool PrincipalVoice::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool PrincipalVoice::hasContents() const
        {
            return true;
        }


        std::ostream& PrincipalVoice::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& PrincipalVoice::streamName( std::ostream& os ) const
        {
            os << "principal-voice";
            return os;
        }


        std::ostream& PrincipalVoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PrincipalVoiceAttributesPtr PrincipalVoice::getAttributes() const
        {
            return myAttributes;
        }


        void PrincipalVoice::setAttributes( const PrincipalVoiceAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString PrincipalVoice::getValue() const
        {
            return myValue;
        }


        void PrincipalVoice::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool PrincipalVoice::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
