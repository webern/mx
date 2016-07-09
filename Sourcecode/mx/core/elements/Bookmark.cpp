// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Bookmark.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Bookmark::Bookmark()
        :ElementInterface()
        ,myAttributes( std::make_shared<BookmarkAttributes>() )
        {}


        bool Bookmark::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Bookmark::hasContents() const  { return false; }
        std::ostream& Bookmark::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Bookmark::streamName( std::ostream& os ) const  { os << "bookmark"; return os; }
        std::ostream& Bookmark::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        BookmarkAttributesPtr Bookmark::getAttributes() const
        {
            return myAttributes;
        }


        void Bookmark::setAttributes( const BookmarkAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Bookmark::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
