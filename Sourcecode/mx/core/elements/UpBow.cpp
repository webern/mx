// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/UpBow.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        UpBow::UpBow()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyPlacementAttributes>() )
        {}


        bool UpBow::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool UpBow::hasContents() const  { return false; }
        std::ostream& UpBow::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& UpBow::streamName( std::ostream& os ) const  { os << "up-bow"; return os; }
        std::ostream& UpBow::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyPlacementAttributesPtr UpBow::getAttributes() const
        {
            return myAttributes;
        }


        void UpBow::setAttributes( const EmptyPlacementAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool UpBow::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
