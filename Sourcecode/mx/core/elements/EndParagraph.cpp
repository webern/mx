// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/EndParagraph.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        EndParagraph::EndParagraph() : ElementInterface() {}


        bool EndParagraph::hasAttributes() const { return false; }


        bool EndParagraph::hasContents() const  { return false; }
        std::ostream& EndParagraph::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& EndParagraph::streamName( std::ostream& os ) const  { os << "end-paragraph"; return os; }
        std::ostream& EndParagraph::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool EndParagraph::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
