// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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


        bool GroupTime::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
