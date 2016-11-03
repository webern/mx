// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/LineData.h"
#include "mx/api/ColorData.h"
#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"

namespace mx
{
    namespace api
    {

        // TODO - use PositionData which includes Placement
        // eleminate the one-off versions of DefaultX/Y

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
            // positionData represents a point along the curve.
            // either the endpoint in the case of starts and stops,
            // or the continuation points in the case of a 'continue'.
            // only the following PositionData fields are supported
            // defaultX
            // defaultY
            // relativeX
            // relativeY
            // placement
            PositionData positionData;
            
            // the 'bezier-x' (or 'bezier-x2') attribute which
            // specifies the bezier curve handle point
            bool isBezierXSpecified;
            long double bezierX;

            // the 'bezier-x' (or 'bezier-x2') attribute which
            // specifies the bezier curve handle point
            bool isBezierYSpecified;
            long double bezierY;
            
            // this is the 'bezier-offset' attribute.  it is very
            // unclear what this is for. it's a decimal in the spec,
            // but using an int here since it is denominated in time
            // duration units and throughout the api we have decided
            // to only allow time to be specified in integer values.
            bool isBezierOffsetSpecified;
            int bezierOffset;
            
            // convenience, check if any of the available fields
            // are specified.
            inline bool isSpecified() const
            {
                return positionData.isDefaultXSpecified || positionData.isDefaultYSpecified || isBezierXSpecified || isBezierYSpecified || isBezierOffsetSpecified;
            }
            
            CurvePoints()
            : positionData{}
            , isBezierXSpecified{ false }
            , bezierX{ 0.0 }
            , isBezierYSpecified{ false }
            , bezierY{ 0.0 }
            , isBezierOffsetSpecified{ false }
            , bezierOffset{ 0 }
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
            CurvePoints curvePoints;
            bool isBezierX2Specified;
            long double bezierX2;
            bool isBezierY2Specified;
            long double bezierY2;
            bool isBezierOffset2Specified;
            long double bezierOffset2;
            
            CurveContinue( CurveType inCurveType )
            : curveType{ inCurveType }
            , numberLevel{ -1 }
            , curvePoints{}
            , isBezierX2Specified{ false }
            , bezierX2{ 0.0 }
            , isBezierY2Specified{ false }
            , bezierY2{ 0.0 }
            , isBezierOffset2Specified{ false }
            , bezierOffset2{ 0.0 }
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
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( isBezierXSpecified )
        MXAPI_EQUALS_MEMBER( bezierX )
        MXAPI_EQUALS_MEMBER( isBezierYSpecified )
        MXAPI_EQUALS_MEMBER( bezierY )
        MXAPI_EQUALS_MEMBER( isBezierOffsetSpecified )
        MXAPI_EQUALS_MEMBER( bezierOffset )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurvePoints );

        MXAPI_EQUALS_BEGIN( CurveStart )
        MXAPI_EQUALS_MEMBER( curveType )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( curvePoints )
        MXAPI_EQUALS_MEMBER( curveOrientation )
        MXAPI_EQUALS_MEMBER( placement )
        MXAPI_EQUALS_MEMBER( lineData )
        MXAPI_EQUALS_MEMBER( isColorSpecified )
        MXAPI_EQUALS_MEMBER( colorData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurveStart );

        MXAPI_EQUALS_BEGIN( CurveContinue )
        MXAPI_EQUALS_MEMBER( curveType )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( curvePoints )
        MXAPI_EQUALS_MEMBER( isBezierX2Specified )
        MXAPI_EQUALS_MEMBER( bezierX2 )
        MXAPI_EQUALS_MEMBER( isBezierY2Specified )
        MXAPI_EQUALS_MEMBER( bezierY2 )
        MXAPI_EQUALS_MEMBER( isBezierOffset2Specified )
        MXAPI_EQUALS_MEMBER( bezierOffset2 )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurveContinue );

        MXAPI_EQUALS_BEGIN( CurveEnd )
        MXAPI_EQUALS_MEMBER( curveType )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( curvePoints )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurveEnd );
    }
}
