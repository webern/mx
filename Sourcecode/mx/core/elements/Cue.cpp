// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Cue.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Cue::Cue() : ElementInterface() {}


        bool Cue::hasAttributes() const { return false; }


        bool Cue::hasContents() const  { return false; }
        std::ostream& Cue::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& Cue::streamName( std::ostream& os ) const  { os << "cue"; return os; }
        std::ostream& Cue::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool Cue::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
