// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NoteAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NoteAttributes::NoteAttributes()
        :defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,fontFamily()
        ,fontStyle( FontStyle::normal )
        ,fontSize( CssFontSize::medium )
        ,fontWeight( FontWeight::normal )
        ,color()
        ,printObject( YesNo::no )
        ,printDot( YesNo::no )
        ,printSpacing( YesNo::no )
        ,printLyric( YesNo::no )
        ,dynamics()
        ,endDynamics()
        ,attack()
        ,release()
        ,timeOnly()
        ,pizzicato( YesNo::no )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        ,hasColor( false )
        ,hasPrintObject( false )
        ,hasPrintDot( false )
        ,hasPrintSpacing( false )
        ,hasPrintLyric( false )
        ,hasDynamics( false )
        ,hasEndDynamics( false )
        ,hasAttack( false )
        ,hasRelease( false )
        ,hasTimeOnly( false )
        ,hasPizzicato( false )
        {}


        bool NoteAttributes::hasValues() const
        {
            return hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasFontFamily ||
            hasFontStyle ||
            hasFontSize ||
            hasFontWeight ||
            hasColor ||
            hasPrintObject ||
            hasPrintDot ||
            hasPrintSpacing ||
            hasPrintLyric ||
            hasDynamics ||
            hasEndDynamics ||
            hasAttack ||
            hasRelease ||
            hasTimeOnly ||
            hasPizzicato;
        }


        std::ostream& NoteAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
                streamAttribute( os, color, "color", hasColor );
                streamAttribute( os, printObject, "print-object", hasPrintObject );
                streamAttribute( os, printDot, "print-dot", hasPrintDot );
                streamAttribute( os, printSpacing, "print-spacing", hasPrintSpacing );
                streamAttribute( os, printLyric, "print-lyric", hasPrintLyric );
                streamAttribute( os, dynamics, "dynamics", hasDynamics );
                streamAttribute( os, endDynamics, "end-dynamics", hasEndDynamics );
                streamAttribute( os, attack, "attack", hasAttack );
                streamAttribute( os, release, "release", hasRelease );
                streamAttribute( os, timeOnly, "time-only", hasTimeOnly );
                streamAttribute( os, pizzicato, "pizzicato", hasPizzicato );
            }
            return os;
        }


        bool NoteAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "NoteAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printObject, hasPrintObject, "print-object", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printDot, hasPrintDot, "print-dot", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printSpacing, hasPrintSpacing, "print-spacing", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printLyric, hasPrintLyric, "print-lyric", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, dynamics, hasDynamics, "dynamics" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, endDynamics, hasEndDynamics, "end-dynamics" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, attack, hasAttack, "attack" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, release, hasRelease, "release" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, timeOnly, hasTimeOnly, "time-only" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, pizzicato, hasPizzicato, "pizzicato", &parseYesNo ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
