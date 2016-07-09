// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MeasureAttributes.h"
#include "mx/core/FromXElement.h"
#include "mx/core/FromString.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MeasureAttributes::MeasureAttributes()
        :number()
        ,implicit( YesNo::no )
        ,nonControlling( YesNo::no )
        ,width()
        ,hasNumber( true )
        ,hasImplicit( false )
        ,hasNonControlling( false )
        ,hasWidth( false )
        {}


        bool MeasureAttributes::hasValues() const
        {
            return hasNumber ||
            hasImplicit ||
            hasNonControlling ||
            hasWidth;
        }


        std::ostream& MeasureAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, implicit, "implicit", hasImplicit );
                streamAttribute( os, nonControlling, "non-controlling", hasNonControlling );
                streamAttribute( os, width, "width", hasWidth );
            }
            return os;
        }


        bool MeasureAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "MeasureAttributes";
            bool isSuccess = true;
            bool isNumberFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, number, isNumberFound, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, implicit, hasImplicit, "implicit", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, nonControlling, hasNonControlling, "non-controlling", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, width, hasWidth, "width" ) ) { continue; }
            }
        
            if( !isNumberFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            return isSuccess;
        }

    }
}
