// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MetronomeBeam.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MetronomeBeam::MetronomeBeam()
        :myValue( BeamValue::begin )
        ,myAttributes( std::make_shared<MetronomeBeamAttributes>() )
        {}


        MetronomeBeam::MetronomeBeam( const BeamValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<MetronomeBeamAttributes>() )
        {}


        bool MetronomeBeam::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool MetronomeBeam::hasContents() const
        {
            return true;
        }


        std::ostream& MetronomeBeam::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& MetronomeBeam::streamName( std::ostream& os ) const
        {
            os << "metronome-beam";
            return os;
        }


        std::ostream& MetronomeBeam::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MetronomeBeamAttributesPtr MetronomeBeam::getAttributes() const
        {
            return myAttributes;
        }


        void MetronomeBeam::setAttributes( const MetronomeBeamAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        BeamValue MetronomeBeam::getValue() const
        {
            return myValue;
        }


        void MetronomeBeam::setValue( const BeamValue& value )
        {
            myValue = value;
        }


        bool MetronomeBeam::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseBeamValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
