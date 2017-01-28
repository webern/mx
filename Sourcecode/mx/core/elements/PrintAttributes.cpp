// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PrintAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PrintAttributes::PrintAttributes()
        :staffSpacing()
        ,newSystem( YesNo::no )
        ,newPage( YesNo::no )
        ,blankPage()
        ,pageNumber()
        ,hasStaffSpacing( false )
        ,hasNewSystem( false )
        ,hasNewPage( false )
        ,hasBlankPage( false )
        ,hasPageNumber( false )
        {}


        bool PrintAttributes::hasValues() const
        {
            return hasStaffSpacing ||
            hasNewSystem ||
            hasNewPage ||
            hasBlankPage ||
            hasPageNumber;
        }


        std::ostream& PrintAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, staffSpacing, "staff-spacing", hasStaffSpacing );
                streamAttribute( os, newSystem, "new-system", hasNewSystem );
                streamAttribute( os, newPage, "new-page", hasNewPage );
                streamAttribute( os, blankPage, "blank-page", hasBlankPage );
                streamAttribute( os, pageNumber, "page-number", hasPageNumber );
            }
            return os;
        }


        bool PrintAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "PrintAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, staffSpacing, hasStaffSpacing, "staff-spacing" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, newSystem, hasNewSystem, "new-system", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, newPage, hasNewPage, "new-page", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, blankPage, hasBlankPage, "blank-page" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, pageNumber, hasPageNumber, "page-number" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
