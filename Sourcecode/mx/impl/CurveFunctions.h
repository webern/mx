// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/CurveData.h"
#include "mx/impl/SpannerFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/PrintFunctions.h"

namespace mx
{
    namespace impl
    {
        
        template<typename ATTRIBUTES_TYPE>
        api::CurvePoints parseCurvePoints( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::CurvePoints p;
            p.positionData = impl::getPositionData( inAttributes );
            
            if( inAttributes.hasBezierX )
            {
                p.isBezierXSpecified = true;
                p.bezierX = inAttributes.bezierX.getValue();
            }
            
            if( inAttributes.hasBezierY )
            {
                p.isBezierYSpecified = true;
                p.bezierY = inAttributes.bezierY.getValue();
            }
            
            if( inAttributes.hasBezierOffset )
            {
                p.isBezierOffsetSpecified = true;
                p.bezierOffset = static_cast<int>( std::ceil( inAttributes.bezierOffset.getValue() - 0.5 ) );
            }
            
            return p;
        }
        
        
        template<typename ATTRIBUTES_TYPE>
        void writeAttributesFromCurvePoints( const api::CurvePoints& curvePoints, ATTRIBUTES_TYPE& outAttributes )
        {
            impl::setAttributesFromPositionData( curvePoints.positionData, outAttributes );
            
            if( curvePoints.isBezierXSpecified )
            {
                outAttributes.hasBezierX = true;
                outAttributes.bezierX.setValue( curvePoints.bezierX );
            }
            
            if( curvePoints.isBezierYSpecified )
            {
                outAttributes.hasBezierY = true;
                outAttributes.bezierY.setValue( curvePoints.bezierY );
            }
            
            if( curvePoints.isBezierOffsetSpecified )
            {
                outAttributes.hasBezierOffset = true;
                outAttributes.bezierOffset.setValue( curvePoints.bezierOffset );
            }
        }
        
        
        template<typename SLUR_OR_TIE_ELEMENT_TYPE>
        api::CurveStart parseCurveStart( const SLUR_OR_TIE_ELEMENT_TYPE& inSlurOrTie )
        {
            const auto& inAttributes = *inSlurOrTie.getAttributes();
            const auto curveType = inSlurOrTie.getElementName() == "slur" ? api::CurveType::slur : api::CurveType::tie;
            api::CurveStart c{ curveType };
            c.numberLevel = impl::checkNumber( &inAttributes );
            
            c.curvePoints = parseCurvePoints( inAttributes );
            c.lineData = impl::getLineData( inAttributes );
            c.isColorSpecified = checkHasColor( &inAttributes );

            if( c.isColorSpecified )
            {
                c.colorData = impl::getColor( inAttributes );
            }
            
            if( inAttributes.hasOrientation )
            {
                c.curveOrientation = inAttributes.orientation == core::OverUnder::over ? api::CurveOrientation::overhand : api::CurveOrientation::underhand;
            }
            return c;
        }
        
        template<typename SLUR_OR_TIE_ELEMENT_TYPE>
        api::CurveContinue parseCurveContinue( const SLUR_OR_TIE_ELEMENT_TYPE& inSlurOrTie )
        {
            const auto& inAttributes = *inSlurOrTie.getAttributes();
            const auto curveType = inSlurOrTie.getElementName() == "slur" ? api::CurveType::slur : api::CurveType::tie;
            api::CurveContinue c{ curveType };
            c.numberLevel = impl::checkNumber( &inAttributes );
            
            if( inAttributes.hasBezierX2 )
            {
                c.isBezierX2Specified = true;
                c.bezierX2 = inAttributes.bezierX2.getValue();
            }
            
            if( inAttributes.hasBezierY2 )
            {
                c.isBezierY2Specified = true;
                c.bezierY2 = inAttributes.bezierY2.getValue();
            }
            
            if( inAttributes.hasBezierOffset2 )
            {
                c.isBezierOffset2Specified = true;
                c.bezierOffset2 = inAttributes.bezierOffset2.getValue();
            }
            
            c.curvePoints = parseCurvePoints( inAttributes );
            return c;
        }
        
        template<typename SLUR_OR_TIE_ELEMENT_TYPE>
        api::CurveEnd parseCurveStop( const SLUR_OR_TIE_ELEMENT_TYPE& inSlurOrTie )
        {
            const auto& inAttributes = *inSlurOrTie.getAttributes();
            const auto curveType = inSlurOrTie.getElementName() == "slur" ? api::CurveType::slur : api::CurveType::tie;
            api::CurveEnd c{ curveType };
            c.numberLevel = impl::checkNumber( &inAttributes );
            c.curvePoints = parseCurvePoints( inAttributes );
            return c;
        }

        
        template<typename ATTRIBUTES_TYPE>
        void writeAttributesFromCurveStart( const api::CurveStart inCurve, ATTRIBUTES_TYPE& outAttributes )
        {
            outAttributes.type = core::StartStopContinue::start;
            impl::setAttributesFromPositionData( inCurve.curvePoints.positionData, outAttributes );
            impl::setAttributesFromLineData( inCurve.lineData, outAttributes );
            
            if( inCurve.isColorSpecified )
            {
                outAttributes.hasColor = true;
                setAttributesFromColorData( inCurve.colorData, outAttributes );
            }

            if( inCurve.numberLevel > 0 )
            {
                outAttributes.hasNumber = true;
                impl::lookForAndSetNumber( inCurve.numberLevel, &outAttributes );
            }

            if( inCurve.curvePoints.isBezierOffsetSpecified )
            {
                outAttributes.hasBezierOffset = true;
                outAttributes.bezierOffset.setValue( static_cast<long double>( inCurve.curvePoints.bezierOffset ) );
            }
            
            if( inCurve.curvePoints.isBezierXSpecified )
            {
                outAttributes.hasBezierX = true;
                outAttributes.bezierX.setValue( inCurve.curvePoints.bezierX );
            }
            
            if( inCurve.curvePoints.isBezierYSpecified )
            {
                outAttributes.hasBezierY = true;
                outAttributes.bezierY.setValue( inCurve.curvePoints.bezierY );
            }
            
            if( inCurve.curveOrientation != api::CurveOrientation::unspecified )
            {
                outAttributes.hasOrientation = true;
                outAttributes.orientation = inCurve.curveOrientation == api::CurveOrientation::overhand ? core::OverUnder::over : core::OverUnder::under;
            }
        }
        
        template<typename ATTRIBUTES_TYPE>
        void writeAttributesFromCurveContinue( const api::CurveContinue inCurve, ATTRIBUTES_TYPE& outAttributes )
        {
            outAttributes.type = core::StartStopContinue::continue_;
            impl::setAttributesFromPositionData( inCurve.curvePoints.positionData, outAttributes );

            if( inCurve.numberLevel > 0 )
            {
                outAttributes.hasNumber = true;
                impl::lookForAndSetNumber( inCurve.numberLevel, &outAttributes );
            }

            if( inCurve.curvePoints.isBezierOffsetSpecified )
            {
                outAttributes.hasBezierOffset = true;
                outAttributes.bezierOffset.setValue( static_cast<long double>( inCurve.curvePoints.bezierOffset ) );
            }
            
            if( inCurve.curvePoints.isBezierXSpecified )
            {
                outAttributes.hasBezierX = true;
                outAttributes.bezierX.setValue( inCurve.curvePoints.bezierX );
            }
            
            if( inCurve.curvePoints.isBezierYSpecified )
            {
                outAttributes.hasBezierY = true;
                outAttributes.bezierY.setValue( inCurve.curvePoints.bezierY );
            }
            
            if( inCurve.isBezierX2Specified )
            {
                outAttributes.hasBezierX2 = true;
                outAttributes.bezierX2.setValue( inCurve.bezierX2 );
            }
            
            if( inCurve.isBezierY2Specified )
            {
                outAttributes.hasBezierY2 = true;
                outAttributes.bezierY2.setValue( inCurve.bezierY2 );
            }
            
            if( inCurve.isBezierOffset2Specified )
            {
                outAttributes.hasBezierOffset2 = true;
                outAttributes.bezierOffset2.setValue( inCurve.bezierOffset2 );
            }
        }
        
        template<typename ATTRIBUTES_TYPE>
        void writeAttributesFromCurveEnd( const api::CurveEnd inCurve, ATTRIBUTES_TYPE& outAttributes )
        {
            outAttributes.type = core::StartStopContinue::stop;
            impl::setAttributesFromPositionData( inCurve.curvePoints.positionData, outAttributes );
            
            if( inCurve.numberLevel > 0 )
            {
                outAttributes.hasNumber = true;
                impl::lookForAndSetNumber( inCurve.numberLevel, &outAttributes );
            }
            
            if( inCurve.curvePoints.isBezierOffsetSpecified )
            {
                outAttributes.hasBezierOffset = true;
                outAttributes.bezierOffset.setValue( static_cast<long double>( inCurve.curvePoints.bezierOffset ) );
            }
            
            if( inCurve.curvePoints.isBezierXSpecified )
            {
                outAttributes.hasBezierX = true;
                outAttributes.bezierX.setValue( inCurve.curvePoints.bezierX );
            }
            
            if( inCurve.curvePoints.isBezierYSpecified )
            {
                outAttributes.hasBezierY = true;
                outAttributes.bezierY.setValue( inCurve.curvePoints.bezierY );
            }
        }
        
        // takes either an mx::core::Tie or an mx::core::Slur
        // populates the outNoteData.curveStart, cureContinuations
        // or curveEnd vector with the result
        template<typename SLUR_OR_TIE_ELEMENT_TYPE>
        void parseCurve( const SLUR_OR_TIE_ELEMENT_TYPE& slurOrTie, api::NoteData& outNoteData )
        {
            const auto outputType = slurOrTie.getAttributes()->type;
            
            if( core::StartStopContinue::start == outputType )
            {
                outNoteData.noteAttachmentData.curveStarts.emplace_back( parseCurveStart( slurOrTie ) );
            }
            else if( core::StartStopContinue::continue_ == outputType )
            {
                outNoteData.noteAttachmentData.curveContinuations.emplace_back( parseCurveContinue( slurOrTie ) );
            }
            else if( core::StartStopContinue::stop == outputType )
            {
                outNoteData.noteAttachmentData.curveEnds.emplace_back( parseCurveStop( slurOrTie ) );
            }
        }
    }
}
