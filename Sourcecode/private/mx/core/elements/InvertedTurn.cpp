// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/InvertedTurn.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        InvertedTurn::InvertedTurn()
        :ElementInterface()
        ,myAttributes( std::make_shared<InvertedTurnAttributes>() )
        {}


        bool InvertedTurn::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool InvertedTurn::hasContents() const  { return false; }
        std::ostream& InvertedTurn::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& InvertedTurn::streamName( std::ostream& os ) const  { os << "inverted-turn"; return os; }
        std::ostream& InvertedTurn::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        InvertedTurnAttributesPtr InvertedTurn::getAttributes() const
        {
            return myAttributes;
        }


        void InvertedTurn::setAttributes( const InvertedTurnAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool InvertedTurn::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
