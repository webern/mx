// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MidiDevice.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MidiDevice::MidiDevice()
        :myValue()
        ,myAttributes( std::make_shared<MidiDeviceAttributes>() )
        {}


        MidiDevice::MidiDevice( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<MidiDeviceAttributes>() )
        {}


        bool MidiDevice::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool MidiDevice::hasContents() const
        {
            return true;
        }


        std::ostream& MidiDevice::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& MidiDevice::streamName( std::ostream& os ) const
        {
            os << "midi-device";
            return os;
        }


        std::ostream& MidiDevice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MidiDeviceAttributesPtr MidiDevice::getAttributes() const
        {
            return myAttributes;
        }


        void MidiDevice::setAttributes( const MidiDeviceAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString MidiDevice::getValue() const
        {
            return myValue;
        }


        void MidiDevice::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool MidiDevice::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
