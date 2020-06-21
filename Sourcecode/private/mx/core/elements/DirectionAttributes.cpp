// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DirectionAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DirectionAttributes::DirectionAttributes()
        :placement( AboveBelow::below )
        ,directive( YesNo::no )
        ,hasPlacement( false )
        ,hasDirective( false )
        {}


        bool DirectionAttributes::hasValues() const
        {
            return hasPlacement ||
            hasDirective;
        }


        std::ostream& DirectionAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, placement, "placement", hasPlacement );
                streamAttribute( os, directive, "directive", hasDirective );
            }
            return os;
        }


        bool DirectionAttributes::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            const char* const className = "DirectionAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, placement, hasPlacement, "placement", &parseAboveBelow ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, directive, hasDirective, "directive", &parseYesNo ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
