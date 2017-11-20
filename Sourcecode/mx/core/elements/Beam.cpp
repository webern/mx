// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Beam.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Beam::Beam()
        :myValue( BeamValue::begin )
        ,myAttributes( std::make_shared<BeamAttributes>() )
        {}


        Beam::Beam( const BeamValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<BeamAttributes>() )
        {}


        bool Beam::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Beam::hasContents() const
        {
            return true;
        }


        std::ostream& Beam::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Beam::streamName( std::ostream& os ) const
        {
            os << "beam";
            return os;
        }


        std::ostream& Beam::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        BeamAttributesPtr Beam::getAttributes() const
        {
            return myAttributes;
        }


        void Beam::setAttributes( const BeamAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        BeamValue Beam::getValue() const
        {
            return myValue;
        }


        void Beam::setValue( const BeamValue& value )
        {
            myValue = value;
        }


        bool Beam::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseBeamValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
