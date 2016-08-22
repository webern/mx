// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/StaffDetailsAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffDetailsAttributes::StaffDetailsAttributes()
        :number()
        ,showFrets( ShowFrets::numbers )
        ,printObject( YesNo::no )
        ,printSpacing( YesNo::no )
        ,hasNumber( false )
        ,hasShowFrets( false )
        ,hasPrintObject( false )
        ,hasPrintSpacing( false )
        {}


        bool StaffDetailsAttributes::hasValues() const
        {
            return hasNumber ||
            hasShowFrets ||
            hasPrintObject ||
            hasPrintSpacing;
        }


        std::ostream& StaffDetailsAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, showFrets, "show-frets", hasShowFrets );
                streamAttribute( os, printObject, "print-object", hasPrintObject );
                streamAttribute( os, printSpacing, "print-spacing", hasPrintSpacing );
            }
            return os;
        }


        bool StaffDetailsAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "StaffDetailsAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, showFrets, hasShowFrets, "show-frets", &parseShowFrets ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printObject, hasPrintObject, "print-object", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printSpacing, hasPrintSpacing, "print-spacing", &parseYesNo ) ) { continue; }
            }
        
        
            return isSuccess;
        }

    }
}
