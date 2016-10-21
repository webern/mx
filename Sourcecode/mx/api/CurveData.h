// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/LineData.h"
#include "mx/api/ColorData.h"
#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
    	enum class CurveType
        {
            unspecified,
            slur,
            tie
        };

        enum class CurveOrientation
        {
            unspecified,
            overhand,
            underhand
        };
        
        struct CurvePoints
        {
            // the 'default-x' attribute which specifies the the
            // a point along the curve (either an endpoint or
            // continuation point).
            bool isXSpecified;
            long double x;

            // the 'default-y' attribute which specifies the the
            // a point along the curve (either an endpoint or
            // continuation point).
            bool isYSpecified;
            long double y;
            
            // the 'bezier-x' (or 'bezier-x2') attribute which
            // specifies the bezier curve handle point
            bool isBezierXSpecified;
            long double bezierX;

            // the 'bezier-x' (or 'bezier-x2') attribute which
            // specifies the bezier curve handle point
            bool isBezierYSpecified;
            long double bezierY;
            
            // this is the 'bezier-offset' (or bezier-offset2')
            // attribute.  it is very unclear what this is for.
            // it's a decimal in the spec, but using an int
            // here since it is denominated in time duration
            // units and throughout the api we have decided to
            // only allow time to be specified in integer values
            bool isTimeOffsetTicksSpecified;
            int timeOffsetTicks;
            
            // convenience, check if any of the available fields
            // are specified.
            inline bool isSpecified() const
            {
                return isXSpecified || isYSpecified || isBezierXSpecified || isBezierYSpecified || isTimeOffsetTicksSpecified;
            }
            
            CurvePoints()
            : isXSpecified{ false }
            , x{ 0.0 }
            , isYSpecified{ false }
            , y{ 0.0 }
            , isBezierXSpecified{ false }
            , bezierX{ 0.0 }
            , isBezierYSpecified{ false }
            , bezierY{ 0.0 }
            , isTimeOffsetTicksSpecified{ false }
            , timeOffsetTicks{ 0 }
            {
                
            }
        };

        struct CurveStart
        {
            CurveType curveType;
            int numberLevel;
            CurvePoints curvePoints;
            CurveOrientation curveOrientation;
            Placement placement;
            LineData lineData;
            bool isColorSpecified;
            ColorData colorData;
            
            CurveStart( CurveType inCurveType )
            : curveType{ inCurveType }
            , numberLevel{ -1 }
            , curvePoints{}
            , curveOrientation{ CurveOrientation::unspecified }
            , placement{ Placement::unspecified }
            , lineData{}
            , isColorSpecified{ false }
            , colorData{}
            {
                
            }
        };


        struct CurveContinue
        {
            CurveType curveType;
            int numberLevel;
            CurvePoints incomingCurvePoints;
            CurvePoints outgoingCurvePoints;
            
            CurveContinue( CurveType inCurveType )
            : curveType{ inCurveType }
            , numberLevel{ -1 }
            , incomingCurvePoints{}
            , outgoingCurvePoints{}
            {
                
            }
        };


        struct CurveEnd
        {
            CurveType curveType;
            int numberLevel;
            CurvePoints curvePoints;
            
            CurveEnd( CurveType inCurveType )
            : curveType{ inCurveType }
            , numberLevel{ -1 }
            , curvePoints{}
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( CurvePoints )
        MXAPI_EQUALS_FIRST_MEMBER( isXSpecified )
        MXAPI_EQUALS_NEXT_MEMBER( x )
        MXAPI_EQUALS_NEXT_MEMBER( isYSpecified )
        MXAPI_EQUALS_NEXT_MEMBER( y )
        MXAPI_EQUALS_NEXT_MEMBER( isBezierXSpecified )
        MXAPI_EQUALS_NEXT_MEMBER( bezierX )
        MXAPI_EQUALS_NEXT_MEMBER( isBezierYSpecified )
        MXAPI_EQUALS_NEXT_MEMBER( bezierY )
        MXAPI_EQUALS_NEXT_MEMBER( isTimeOffsetTicksSpecified )
        MXAPI_EQUALS_LAST_MEMBER( timeOffsetTicks )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurvePoints );

        MXAPI_EQUALS_BEGIN( CurveStart )
        MXAPI_EQUALS_FIRST_MEMBER( curveType )
        MXAPI_EQUALS_NEXT_MEMBER( numberLevel )
        MXAPI_EQUALS_NEXT_MEMBER( curvePoints )
        MXAPI_EQUALS_NEXT_MEMBER( curveOrientation )
        MXAPI_EQUALS_NEXT_MEMBER( placement )
        MXAPI_EQUALS_NEXT_MEMBER( lineData )
        MXAPI_EQUALS_NEXT_MEMBER( isColorSpecified )
        MXAPI_EQUALS_LAST_MEMBER( colorData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurveStart );

        MXAPI_EQUALS_BEGIN( CurveContinue )
        MXAPI_EQUALS_FIRST_MEMBER( curveType )
        MXAPI_EQUALS_NEXT_MEMBER( numberLevel )
        MXAPI_EQUALS_NEXT_MEMBER( incomingCurvePoints )
        MXAPI_EQUALS_LAST_MEMBER( outgoingCurvePoints )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurveContinue );

        MXAPI_EQUALS_BEGIN( CurveEnd )
        MXAPI_EQUALS_FIRST_MEMBER( curveType )
        MXAPI_EQUALS_NEXT_MEMBER( numberLevel )
        MXAPI_EQUALS_LAST_MEMBER( curvePoints )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurveEnd );
    }
}
