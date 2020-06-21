// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Slur.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Slur::Slur()
        :ElementInterface()
        ,myAttributes( std::make_shared<SlurAttributes>() )
        {}


        bool Slur::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Slur::hasContents() const  { return false; }
        std::ostream& Slur::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Slur::streamName( std::ostream& os ) const  { os << "slur"; return os; }
        std::ostream& Slur::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        SlurAttributesPtr Slur::getAttributes() const
        {
            return myAttributes;
        }


        void Slur::setAttributes( const SlurAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Slur::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
