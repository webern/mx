// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SlideAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SlideAttributes::SlideAttributes()
        :type( StartStop::start )
        ,number( 1 )
        ,lineType( LineType::solid )
        ,dashLength()
        ,spaceLength()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,fontFamily()
        ,fontStyle( FontStyle::normal )
        ,fontSize( FontSize{ CssFontSize::medium } )
        ,fontWeight( FontWeight::normal )
        ,accelerate()
        ,beats()
        ,firstBeat()
        ,lastBeat()
        ,hasType( true )
        ,hasNumber( false )
        ,hasLineType( false )
        ,hasDashLength( false )
        ,hasSpaceLength( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        ,hasAccelerate( false )
        ,hasBeats( false )
        ,hasFirstBeat( false )
        ,hasLastBeat( false )
        {}


        bool SlideAttributes::hasValues() const
        {
            return hasType ||
            hasNumber ||
            hasLineType ||
            hasDashLength ||
            hasSpaceLength ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasFontFamily ||
            hasFontStyle ||
            hasFontSize ||
            hasFontWeight ||
            hasAccelerate ||
            hasBeats ||
            hasFirstBeat ||
            hasLastBeat;
        }


        std::ostream& SlideAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, lineType, "line-type", hasLineType );
                streamAttribute( os, dashLength, "dash-length", hasDashLength );
                streamAttribute( os, spaceLength, "space-length", hasSpaceLength );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
                streamAttribute( os, accelerate, "accelerate", hasAccelerate );
                streamAttribute( os, beats, "beats", hasBeats );
                streamAttribute( os, firstBeat, "first-beat", hasFirstBeat );
                streamAttribute( os, lastBeat, "last-beat", hasLastBeat );
            }
            return os;
        }


        bool SlideAttributes::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            const char* const className = "SlideAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStop ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lineType, hasLineType, "line-type", &parseLineType ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, dashLength, hasDashLength, "dash-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, spaceLength, hasSpaceLength, "space-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, accelerate, hasAccelerate, "accelerate", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, beats, hasBeats, "beats" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, firstBeat, hasFirstBeat, "first-beat" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lastBeat, hasLastBeat, "last-beat" ) ) { continue; }
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
