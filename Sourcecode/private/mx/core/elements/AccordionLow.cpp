// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/AccordionLow.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        AccordionLow::AccordionLow() : ElementInterface() {}


        bool AccordionLow::hasAttributes() const { return false; }


        bool AccordionLow::hasContents() const  { return false; }
        std::ostream& AccordionLow::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& AccordionLow::streamName( std::ostream& os ) const  { os << "accordion-low"; return os; }
        std::ostream& AccordionLow::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool AccordionLow::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
