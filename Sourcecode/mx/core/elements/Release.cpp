// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Release.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Release::Release() : ElementInterface() {}


        bool Release::hasAttributes() const { return false; }


        bool Release::hasContents() const  { return false; }
        std::ostream& Release::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& Release::streamName( std::ostream& os ) const  { os << "release"; return os; }
        std::ostream& Release::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool Release::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
