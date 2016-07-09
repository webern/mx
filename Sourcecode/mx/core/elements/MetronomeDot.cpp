// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MetronomeDot.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MetronomeDot::MetronomeDot() : ElementInterface() {}


        bool MetronomeDot::hasAttributes() const { return false; }


        bool MetronomeDot::hasContents() const  { return false; }
        std::ostream& MetronomeDot::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& MetronomeDot::streamName( std::ostream& os ) const  { os << "metronome-dot"; return os; }
        std::ostream& MetronomeDot::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool MetronomeDot::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
