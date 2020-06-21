// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Fingering.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Fingering::Fingering()
        :myValue()
        ,myAttributes( std::make_shared<FingeringAttributes>() )
        {}


        Fingering::Fingering( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<FingeringAttributes>() )
        {}


        bool Fingering::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Fingering::hasContents() const
        {
            return true;
        }


        std::ostream& Fingering::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Fingering::streamName( std::ostream& os ) const
        {
            os << "fingering";
            return os;
        }


        std::ostream& Fingering::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FingeringAttributesPtr Fingering::getAttributes() const
        {
            return myAttributes;
        }


        void Fingering::setAttributes( const FingeringAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Fingering::getValue() const
        {
            return myValue;
        }


        void Fingering::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Fingering::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
