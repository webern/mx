// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/SlurAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SlurAttributes::SlurAttributes()
        :type( StartStopContinue::start )
        ,number( 1 )
        ,lineType( LineType::solid )
        ,dashLength()
        ,spaceLength()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,placement( AboveBelow::below )
        ,orientation()
        ,bezierOffset()
        ,bezierOffset2()
        ,bezierX()
        ,bezierY()
        ,bezierX2()
        ,bezierY2()
        ,color()
        ,hasType( true )
        ,hasNumber( false )
        ,hasLineType( false )
        ,hasDashLength( false )
        ,hasSpaceLength( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasPlacement( false )
        ,hasOrientation( false )
        ,hasBezierOffset( false )
        ,hasBezierOffset2( false )
        ,hasBezierX( false )
        ,hasBezierY( false )
        ,hasBezierX2( false )
        ,hasBezierY2( false )
        ,hasColor( false )
        {}


        bool SlurAttributes::hasValues() const
        {
            return hasType ||
            hasNumber ||
            hasDashLength ||
            hasSpaceLength ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasPlacement ||
            hasOrientation ||
            hasBezierOffset ||
            hasBezierOffset2 ||
            hasBezierX ||
            hasBezierY ||
            hasBezierX2 ||
            hasBezierY2;
        }


        std::ostream& SlurAttributes::toStream( std::ostream& os ) const
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
                streamAttribute( os, placement, "placement", hasPlacement );
                streamAttribute( os, orientation, "orientation", hasOrientation );
                streamAttribute( os, bezierOffset, "bezier-offset", hasBezierOffset );
                streamAttribute( os, bezierOffset2, "bezier-offset2", hasBezierOffset2 );
                streamAttribute( os, bezierX, "bezier-x", hasBezierX );
                streamAttribute( os, bezierY, "bezier-y", hasBezierY );
                streamAttribute( os, bezierX2, "bezier-x2", hasBezierX2 );
                streamAttribute( os, bezierY2, "bezier-y2", hasBezierY2 );
                streamAttribute( os, color, "color", hasColor );
            }
            return os;
        }


        bool SlurAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "SlurAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStopContinue ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lineType, hasLineType, "line-type", &parseLineType ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, dashLength, hasDashLength, "dash-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, spaceLength, hasSpaceLength, "space-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, placement, hasPlacement, "placement", &parseAboveBelow ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, orientation, hasOrientation, "orientation", &parseOverUnder ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bezierOffset, hasBezierOffset, "bezier-offset" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bezierOffset2, hasBezierOffset2, "bezier-offset-2" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bezierX, hasBezierX, "bezier-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bezierY, hasBezierY, "bezier-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bezierX2, hasBezierX2, "bezier-x2" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bezierY2, hasBezierY2, "bezier-y2" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
            }
        
            if( !isTypeFound )
            {
                isSuccess = false;
                message << className << ": 'type' is a required attribute but was not found" << std::endl;
            }
        
            return isSuccess;
        }

    }
}
