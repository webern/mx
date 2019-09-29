// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/WordFont.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        WordFont::WordFont()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyFontAttributes>() )
        {}


        bool WordFont::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool WordFont::hasContents() const  { return false; }
        std::ostream& WordFont::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& WordFont::streamName( std::ostream& os ) const  { os << "word-font"; return os; }
        std::ostream& WordFont::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyFontAttributesPtr WordFont::getAttributes() const
        {
            return myAttributes;
        }


        void WordFont::setAttributes( const EmptyFontAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool WordFont::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
