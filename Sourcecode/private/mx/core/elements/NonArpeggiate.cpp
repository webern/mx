// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NonArpeggiate.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NonArpeggiate::NonArpeggiate()
        :ElementInterface()
        ,myAttributes( std::make_shared<NonArpeggiateAttributes>() )
        {}


        bool NonArpeggiate::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool NonArpeggiate::hasContents() const  { return false; }
        std::ostream& NonArpeggiate::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& NonArpeggiate::streamName( std::ostream& os ) const  { os << "non-arpeggiate"; return os; }
        std::ostream& NonArpeggiate::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        NonArpeggiateAttributesPtr NonArpeggiate::getAttributes() const
        {
            return myAttributes;
        }


        void NonArpeggiate::setAttributes( const NonArpeggiateAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool NonArpeggiate::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
