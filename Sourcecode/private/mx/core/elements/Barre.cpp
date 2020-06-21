// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Barre.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Barre::Barre()
        :ElementInterface()
        ,myAttributes( std::make_shared<BarreAttributes>() )
        {}


        bool Barre::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Barre::hasContents() const  { return false; }
        std::ostream& Barre::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Barre::streamName( std::ostream& os ) const  { os << "barre"; return os; }
        std::ostream& Barre::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        BarreAttributesPtr Barre::getAttributes() const
        {
            return myAttributes;
        }


        void Barre::setAttributes( const BarreAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Barre::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
