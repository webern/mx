// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <string>

// From the MusicXML Documentation
//
// The bezier attribute group is used to indicate the
// curvature of slurs and ties, representing the control
// points for a cubic bezier curve. For ties, the bezier
// attribute group is used with the tied element.
//
// Normal slurs, S-shaped slurs, and ties need only two
// bezier points: one associated with the start of the
// slur or tie, the other with the stop. Complex slurs
// and slurs divided over system breaks can specify
// additional bezier data at slur elements with a
// continue type.
//    
// The bezier-offset, bezier-x, and bezier-y attributes
// describe the outgoing bezier point for slurs and ties
// with a start type, and the incoming bezier point for
// slurs and ties with types of stop or continue. The
// attributes bezier-offset2, bezier-x2, and bezier-y2
// are only valid with slurs of type continue, and
// describe the outgoing bezier point.
//    
// The bezier-offset and bezier-offset2 attributes are
// measured in terms of musical divisions, like the
// offset element. These are the recommended attributes
// for specifying horizontal position. The other
// attributes are specified in tenths, relative to any
// position settings associated with the slur or tied
// element.

namespace mx
{
    namespace api
    {
        
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
            // attribute.  it is very unclear what this is for
            // it's a decimal in the spec, but using an int
            // here since it is denominated in time duration
            // units and throughout the api we have decided to
            // only allow time to be specified in integer values
            bool isTimeOffsetTicksSpecified;
            int timeOffsetTicks;
            
            // convience, check if any of the available fields
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
    }
}
