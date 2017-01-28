// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ThumbPosition.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ThumbPosition::ThumbPosition()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyPlacementAttributes>() )
        {}


        bool ThumbPosition::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool ThumbPosition::hasContents() const  { return false; }
        std::ostream& ThumbPosition::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& ThumbPosition::streamName( std::ostream& os ) const  { os << "thumb-position"; return os; }
        std::ostream& ThumbPosition::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyPlacementAttributesPtr ThumbPosition::getAttributes() const
        {
            return myAttributes;
        }


        void ThumbPosition::setAttributes( const EmptyPlacementAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool ThumbPosition::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
