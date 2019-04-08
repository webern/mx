// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BeatUnitDot.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeatUnitDot::BeatUnitDot() : ElementInterface() {}


        bool BeatUnitDot::hasAttributes() const { return false; }


        bool BeatUnitDot::hasContents() const  { return false; }
        std::ostream& BeatUnitDot::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& BeatUnitDot::streamName( std::ostream& os ) const  { os << "beat-unit-dot"; return os; }
        std::ostream& BeatUnitDot::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool BeatUnitDot::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
