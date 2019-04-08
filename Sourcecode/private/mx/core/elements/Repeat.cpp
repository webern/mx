// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Repeat.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Repeat::Repeat()
        :ElementInterface()
        ,myAttributes( std::make_shared<RepeatAttributes>() )
        {}


        bool Repeat::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Repeat::hasContents() const  { return false; }
        std::ostream& Repeat::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Repeat::streamName( std::ostream& os ) const  { os << "repeat"; return os; }
        std::ostream& Repeat::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        RepeatAttributesPtr Repeat::getAttributes() const
        {
            return myAttributes;
        }


        void Repeat::setAttributes( const RepeatAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Repeat::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
