// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/LyricFont.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LyricFont::LyricFont()
        :ElementInterface()
        ,myAttributes( std::make_shared<LyricFontAttributes>() )
        {}


        bool LyricFont::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool LyricFont::hasContents() const  { return false; }
        std::ostream& LyricFont::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& LyricFont::streamName( std::ostream& os ) const  { os << "lyric-font"; return os; }
        std::ostream& LyricFont::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        LyricFontAttributesPtr LyricFont::getAttributes() const
        {
            return myAttributes;
        }


        void LyricFont::setAttributes( const LyricFontAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool LyricFont::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
