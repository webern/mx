// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PartAbbreviationDisplayAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartAbbreviationDisplayAttributes::PartAbbreviationDisplayAttributes()
        :printObject( YesNo::no )
        ,hasPrintObject( false )
        {}


        bool PartAbbreviationDisplayAttributes::hasValues() const
        {
            return hasPrintObject;
        }


        std::ostream& PartAbbreviationDisplayAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, printObject, "print-object", hasPrintObject );
            }
            return os;
        }


        bool PartAbbreviationDisplayAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "PartAbbreviationDisplayAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, printObject, hasPrintObject, "print-object", &parseYesNo ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
