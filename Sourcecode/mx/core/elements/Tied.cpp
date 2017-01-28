// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Tied.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Tied::Tied()
        :ElementInterface()
        ,myAttributes( std::make_shared<TiedAttributes>() )
        {}


        bool Tied::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Tied::hasContents() const  { return false; }
        std::ostream& Tied::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Tied::streamName( std::ostream& os ) const  { os << "tied"; return os; }
        std::ostream& Tied::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        TiedAttributesPtr Tied::getAttributes() const
        {
            return myAttributes;
        }


        void Tied::setAttributes( const TiedAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Tied::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
