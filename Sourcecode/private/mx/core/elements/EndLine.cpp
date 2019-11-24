// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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


        bool EndLine::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
