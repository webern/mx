// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TrillMark.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TrillMark::TrillMark()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyTrillSoundAttributes>() )
        {}


        bool TrillMark::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool TrillMark::hasContents() const  { return false; }
        std::ostream& TrillMark::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& TrillMark::streamName( std::ostream& os ) const  { os << "trill-mark"; return os; }
        std::ostream& TrillMark::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyTrillSoundAttributesPtr TrillMark::getAttributes() const
        {
            return myAttributes;
        }


        void TrillMark::setAttributes( const EmptyTrillSoundAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool TrillMark::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
