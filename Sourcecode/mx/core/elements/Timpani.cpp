// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Timpani.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Timpani::Timpani() : ElementInterface() {}


        bool Timpani::hasAttributes() const { return false; }


        bool Timpani::hasContents() const  { return false; }
        std::ostream& Timpani::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& Timpani::streamName( std::ostream& os ) const  { os << "timpani"; return os; }
        std::ostream& Timpani::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool Timpani::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
