// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/KeyOctaveAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        KeyOctaveAttributes::KeyOctaveAttributes()
        :number( PositiveInteger{ 1 } )
        ,cancel( YesNo::no )
        ,hasNumber( true )
        ,hasCancel( false )
        {}


        bool KeyOctaveAttributes::hasValues() const
        {
            return hasNumber ||
            hasCancel;
        }


        std::ostream& KeyOctaveAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, cancel, "cancel", hasCancel );
            }
            return os;
        }


        bool KeyOctaveAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "KeyOctaveAttributes";
            bool isSuccess = true;
            bool isNumberFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, number, isNumberFound, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, cancel, hasCancel, "cancel", &parseYesNo ) ) { continue; }
            }
        
            if( !isNumberFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
