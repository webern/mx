// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/VerticalTurn.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        VerticalTurn::VerticalTurn()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyTrillSoundAttributes>() )
        {}


        bool VerticalTurn::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool VerticalTurn::hasContents() const  { return false; }
        std::ostream& VerticalTurn::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& VerticalTurn::streamName( std::ostream& os ) const  { os << "vertical-turn"; return os; }
        std::ostream& VerticalTurn::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyTrillSoundAttributesPtr VerticalTurn::getAttributes() const
        {
            return myAttributes;
        }


        void VerticalTurn::setAttributes( const EmptyTrillSoundAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool VerticalTurn::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
