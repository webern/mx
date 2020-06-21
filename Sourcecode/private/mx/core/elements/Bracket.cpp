// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Bracket.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Bracket::Bracket()
        :ElementInterface()
        ,myAttributes( std::make_shared<BracketAttributes>() )
        {}


        bool Bracket::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Bracket::hasContents() const  { return false; }
        std::ostream& Bracket::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Bracket::streamName( std::ostream& os ) const  { os << "bracket"; return os; }
        std::ostream& Bracket::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        BracketAttributesPtr Bracket::getAttributes() const
        {
            return myAttributes;
        }


        void Bracket::setAttributes( const BracketAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Bracket::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
