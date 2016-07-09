// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/SnapPizzicato.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SnapPizzicato::SnapPizzicato()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyPlacementAttributes>() )
        {}


        bool SnapPizzicato::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool SnapPizzicato::hasContents() const  { return false; }
        std::ostream& SnapPizzicato::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& SnapPizzicato::streamName( std::ostream& os ) const  { os << "snap-pizzicato"; return os; }
        std::ostream& SnapPizzicato::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyPlacementAttributesPtr SnapPizzicato::getAttributes() const
        {
            return myAttributes;
        }


        void SnapPizzicato::setAttributes( const EmptyPlacementAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool SnapPizzicato::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
