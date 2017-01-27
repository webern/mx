// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/LineData.h"
#include "mx/api/ColorData.h"
#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"

// MusicXML Documentation for Bezier Attributes Group
// The bezier attribute group is used to indicate the curvature of slurs
// and ties, representing the control points for a cubic bezier curve.
// For ties, the bezier attribute group is used with the tied element.
//
// Normal slurs, S-shaped slurs, and ties need only two bezier points:
// one associated with the start of the slur or tie, the other with the
// stop. Complex slurs and slurs divided over system breaks can specify
// additional bezier data at slur elements with a continue type.
//
// The bezier-offset, bezier-x, and bezier-y attributes describe the
// outgoing bezier point for slurs and ties with a start type, and the
// incoming bezier point for slurs and ties with types of stop or continue.
// The attributes bezier-offset2, bezier-x2, and bezier-y2 are only valid
// with slurs of type continue, and describe the outgoing bezier point.
//    
// The bezier-offset and bezier-offset2 attributes are measured in terms of
// musical divisions, like the offset element. These are the recommended
// attributes for specifying horizontal position. The other attributes are
// specified in tenths, relative to any position settings associated with
// the slur or tied element.

// MusicXML Documentation for Slur Element
// Slur types are empty. Most slurs are represented with two elements: one
// with a start type, and one with a stop type. Slurs can add more elements
// using a continue type. This is typically used to specify the formatting
// of cross-system slurs, or to specify the shape of very complex slurs.

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
            // positionData represents a point along the curve.
            // either the endpoint in the case of starts and stops,
            // or the continuation points in the case of a 'continue'.
            // only the following PositionData fields are supported:
            // defaultX, defaultY, relativeX, relativeY, placement
            PositionData positionData;
            
            bool isBezierXSpecified;
            long double bezierX;

            bool isBezierYSpecified;
            long double bezierY;
            
            bool isBezierOffsetSpecified;
            int bezierOffset;
            
            // convenience, check if any of the available fields
            // are specified.
            inline bool isSpecified() const
            {
                return positionData.isDefaultXSpecified
                || positionData.isDefaultYSpecified
                || isBezierXSpecified
                || isBezierYSpecified
                || isBezierOffsetSpecified;
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


        struct CurveStop
        {
            CurveType curveType;
            int numberLevel;
            CurvePoints curvePoints;
            
            CurveStop( CurveType inCurveType )
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

        MXAPI_EQUALS_BEGIN( CurveStop )
        MXAPI_EQUALS_MEMBER( curveType )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( curvePoints )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CurveStop );
    }
}
