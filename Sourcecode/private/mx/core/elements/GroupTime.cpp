// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GroupTime.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GroupTime::GroupTime() : ElementInterface() {}


        bool GroupTime::hasAttributes() const { return false; }


        bool GroupTime::hasContents() const  { return false; }
        std::ostream& GroupTime::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& GroupTime::streamName( std::ostream& os ) const  { os << "group-time"; return os; }
        std::ostream& GroupTime::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool GroupTime::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
