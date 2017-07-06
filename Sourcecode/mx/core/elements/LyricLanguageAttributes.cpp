// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/LyricLanguageAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LyricLanguageAttributes::LyricLanguageAttributes()
        :number()
        ,name()
        ,lang( "it" )
        ,hasNumber( false )
        ,hasName( false )
        ,hasLang( true )
        {}


        bool LyricLanguageAttributes::hasValues() const
        {
            return hasNumber ||
            hasName ||
            hasLang;
        }


        std::ostream& LyricLanguageAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, name, "name", hasName );
                streamAttribute( os, lang, "xml:lang", hasLang );
            }
            return os;
        }


        bool LyricLanguageAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "LyricLanguageAttributes";
            bool isSuccess = true;
            bool isLangFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, name, hasName, "name" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lang, isLangFound, "xml:lang" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lang, isLangFound, "lang" ) ) { continue; }
            }
        
            if( !isLangFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
