// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/EndLine.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        EndLine::EndLine() : ElementInterface() {}


        bool EndLine::hasAttributes() const { return false; }


        bool EndLine::hasContents() const  { return false; }
        std::ostream& EndLine::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& EndLine::streamName( std::ostream& os ) const  { os << "end-line"; return os; }
        std::ostream& EndLine::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool EndLine::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
