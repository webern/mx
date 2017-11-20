// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/LevelAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LevelAttributes::LevelAttributes()
        :reference()
        ,parentheses()
        ,bracket()
        ,size()
        ,hasReference( false )
        ,hasParentheses( false )
        ,hasBracket( false )
        ,hasSize( false )
        {}


        bool LevelAttributes::hasValues() const
        {
            return hasReference ||
            hasParentheses ||
            hasBracket ||
            hasSize;
        }


        std::ostream& LevelAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, reference, "reference", hasReference );
                streamAttribute( os, parentheses, "parentheses", hasParentheses );
                streamAttribute( os, bracket, "bracket", hasBracket );
                streamAttribute( os, size, "size", hasSize );
            }
            return os;
        }


        bool LevelAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "LevelAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, reference, hasReference, "reference", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, parentheses, hasParentheses, "parentheses", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bracket, hasBracket, "bracket", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, size, hasSize, "size", &parseSymbolSize ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
