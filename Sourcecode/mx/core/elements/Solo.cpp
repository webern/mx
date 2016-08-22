// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Solo.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Solo::Solo() : ElementInterface() {}


        bool Solo::hasAttributes() const { return false; }


        bool Solo::hasContents() const  { return false; }
        std::ostream& Solo::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& Solo::streamName( std::ostream& os ) const  { os << "solo"; return os; }
        std::ostream& Solo::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool Solo::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( xelement );
            MX_UNUSED( message );
            return true;
        }

    }
}
