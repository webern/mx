// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/LyricLanguage.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LyricLanguage::LyricLanguage()
        :ElementInterface()
        ,myAttributes( std::make_shared<LyricLanguageAttributes>() )
        {}


        bool LyricLanguage::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool LyricLanguage::hasContents() const  { return false; }
        std::ostream& LyricLanguage::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& LyricLanguage::streamName( std::ostream& os ) const  { os << "lyric-language"; return os; }
        std::ostream& LyricLanguage::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        LyricLanguageAttributesPtr LyricLanguage::getAttributes() const
        {
            return myAttributes;
        }


        void LyricLanguage::setAttributes( const LyricLanguageAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool LyricLanguage::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
