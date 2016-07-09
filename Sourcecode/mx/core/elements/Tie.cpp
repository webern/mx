// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Tie.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Tie::Tie()
        :ElementInterface()
        ,myAttributes( std::make_shared<TieAttributes>() )
        {}


        bool Tie::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Tie::hasContents() const  { return false; }
        std::ostream& Tie::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Tie::streamName( std::ostream& os ) const  { os << "tie"; return os; }
        std::ostream& Tie::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        TieAttributesPtr Tie::getAttributes() const
        {
            return myAttributes;
        }


        void Tie::setAttributes( const TieAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Tie::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
