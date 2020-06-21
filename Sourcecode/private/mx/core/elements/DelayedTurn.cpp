// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DelayedTurn.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DelayedTurn::DelayedTurn()
        :ElementInterface()
        ,myAttributes( std::make_shared<DelayedTurnAttributes>() )
        {}


        bool DelayedTurn::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool DelayedTurn::hasContents() const  { return false; }
        std::ostream& DelayedTurn::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& DelayedTurn::streamName( std::ostream& os ) const  { os << "delayed-turn"; return os; }
        std::ostream& DelayedTurn::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        DelayedTurnAttributesPtr DelayedTurn::getAttributes() const
        {
            return myAttributes;
        }


        void DelayedTurn::setAttributes( const DelayedTurnAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool DelayedTurn::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
