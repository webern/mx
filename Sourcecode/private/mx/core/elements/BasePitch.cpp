// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BasePitch.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BasePitch::BasePitch() : ElementInterface() {}


        bool BasePitch::hasAttributes() const { return false; }


        bool BasePitch::hasContents() const  { return false; }
        std::ostream& BasePitch::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& BasePitch::streamName( std::ostream& os ) const  { os << "base-pitch"; return os; }
        std::ostream& BasePitch::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool BasePitch::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
