// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DetachedLegato.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DetachedLegato::DetachedLegato()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyPlacementAttributes>() )
        {}


        bool DetachedLegato::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool DetachedLegato::hasContents() const  { return false; }
        std::ostream& DetachedLegato::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& DetachedLegato::streamName( std::ostream& os ) const  { os << "detached-legato"; return os; }
        std::ostream& DetachedLegato::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyPlacementAttributesPtr DetachedLegato::getAttributes() const
        {
            return myAttributes;
        }


        void DetachedLegato::setAttributes( const EmptyPlacementAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool DetachedLegato::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
