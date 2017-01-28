// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TupletAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TupletAttributes::TupletAttributes()
        :type()
        ,number()
        ,bracket( YesNo::no )
        ,showNumber()
        ,showType()
        ,lineShape()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,placement( AboveBelow::below )
        ,hasType( true )
        ,hasNumber( false )
        ,hasBracket( false )
        ,hasShowNumber( false )
        ,hasShowType( false )
        ,hasLineShape( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasPlacement( false )
        {}


        bool TupletAttributes::hasValues() const
        {
            return hasType ||
            hasNumber ||
            hasBracket ||
            hasShowNumber ||
            hasShowType ||
            hasLineShape ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasPlacement;
        }


        std::ostream& TupletAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, bracket, "bracket", hasBracket );
                streamAttribute( os, showNumber, "show-number", hasShowNumber );
                streamAttribute( os, showType, "show-type", hasShowType );
                streamAttribute( os, lineShape, "line-shape", hasLineShape );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, placement, "placement", hasPlacement );
            }
            return os;
        }


        bool TupletAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "TupletAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStop ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bracket, hasBracket, "bracket", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, showNumber, hasShowNumber, "show-number", &parseShowTuplet ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, showType, hasShowType, "show-type", &parseShowTuplet ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lineShape, hasLineShape, "line-shape", &parseLineShape ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, placement, hasPlacement, "placement", &parseAboveBelow ) ) { continue; }
            }
        
            if( !isTypeFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
