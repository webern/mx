// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DelayedInvertedTurn.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DelayedInvertedTurn::DelayedInvertedTurn()
        :ElementInterface()
        ,myAttributes( std::make_shared<DelayedInvertedTurnAttributes>() )
        {}


        bool DelayedInvertedTurn::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool DelayedInvertedTurn::hasContents() const  { return false; }
        std::ostream& DelayedInvertedTurn::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& DelayedInvertedTurn::streamName( std::ostream& os ) const  { os << "delayed-inverted-turn"; return os; }
        std::ostream& DelayedInvertedTurn::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        DelayedInvertedTurnAttributesPtr DelayedInvertedTurn::getAttributes() const
        {
            return myAttributes;
        }


        void DelayedInvertedTurn::setAttributes( const DelayedInvertedTurnAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool DelayedInvertedTurn::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
