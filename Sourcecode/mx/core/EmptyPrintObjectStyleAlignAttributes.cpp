// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/EmptyPrintObjectStyleAlignAttributes.h"
#include "mx/core/FromXElement.h"

namespace mx
{
    namespace core
    {
        EmptyPrintObjectStyleAlignAttributes::EmptyPrintObjectStyleAlignAttributes()
        :printObject( YesNo::yes )
        ,defaultX( 0.0 )
        ,defaultY( 0.0 )
        ,relativeX( 0.0 )
        ,relativeY( 0.0 )
        ,fontFamily( "" )
        ,fontStyle( FontStyle::normal )
        ,fontSize( CssFontSize::medium )
        ,fontWeight( FontWeight::bold )
        ,color( 255, 255, 255 )
        ,halign( LeftCenterRight::center )
        ,valign( Valign::top )
        ,hasPrintObject( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        ,hasColor( false )
        ,hasHalign( false )
        ,hasValign( false )
        {}


        bool EmptyPrintObjectStyleAlignAttributes::hasValues() const
        {
            return hasPrintObject
                || hasDefaultX
                || hasDefaultY
                || hasRelativeX
                || hasRelativeY
                || hasFontFamily
                || hasFontStyle
                || hasFontSize
                || hasFontWeight
                || hasColor
                || hasHalign
                || hasValign;
        }
        

        std::ostream& EmptyPrintObjectStyleAlignAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, printObject, "print-object", hasPrintObject );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
                streamAttribute( os, color, "color", hasColor );
                streamAttribute( os, halign, "halign", hasHalign );
                streamAttribute( os, valign, "valign", hasValign );
            }
            return os;
        }


        bool EmptyPrintObjectStyleAlignAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "EmptyPrintObjectStyleAlignAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, printObject, hasPrintObject, "print-object", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, halign, hasHalign, "halign", &parseLeftCenterRight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, valign, hasValign, "valign", &parseValign ) ) { continue; }

            }
        
        
            MX_RETURN_IS_SUCCESS;
        }
    }
}
