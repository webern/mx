// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Plop.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Plop::Plop()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyLineAttributes>() )
        {}


        bool Plop::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Plop::hasContents() const  { return false; }
        std::ostream& Plop::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Plop::streamName( std::ostream& os ) const  { os << "plop"; return os; }
        std::ostream& Plop::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyLineAttributesPtr Plop::getAttributes() const
        {
            return myAttributes;
        }


        void Plop::setAttributes( const EmptyLineAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Plop::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
