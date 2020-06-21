// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/VirtualLibrary.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        VirtualLibrary::VirtualLibrary()
        :myValue()
        {}


        VirtualLibrary::VirtualLibrary( const XsString& value )
        :myValue( value )
        {}


        bool VirtualLibrary::hasAttributes() const
        {
            return false;
        }


        bool VirtualLibrary::hasContents() const
        {
            return true;
        }


        std::ostream& VirtualLibrary::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& VirtualLibrary::streamName( std::ostream& os ) const
        {
            os << "virtual-library";
            return os;
        }


        std::ostream& VirtualLibrary::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString VirtualLibrary::getValue() const
        {
            return myValue;
        }


        void VirtualLibrary::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool VirtualLibrary::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
