// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MusicFont.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MusicFont::MusicFont()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyFontAttributes>() )
        {}


        bool MusicFont::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool MusicFont::hasContents() const  { return false; }
        std::ostream& MusicFont::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& MusicFont::streamName( std::ostream& os ) const  { os << "music-font"; return os; }
        std::ostream& MusicFont::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyFontAttributesPtr MusicFont::getAttributes() const
        {
            return myAttributes;
        }


        void MusicFont::setAttributes( const EmptyFontAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool MusicFont::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
