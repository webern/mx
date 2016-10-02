// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/CurveContinue.h"
#include "mx/api/CurveEnd.h"
#include "mx/api/CurveStart.h"

namespace mx
{
    namespace impl
    {
        // takes either an mx::core::Tie or an mx::core::Slur
        // populates the outNoteData.curveStart, cureContinuations
        // or curveEnd vector with the result
        template<typename SLUR_OR_TIE_ELEMENT_TYPE>
        void slurryness( const SLUR_OR_TIE_ELEMENT_TYPE& slurOrTie, api::NoteData& outNoteData )
        {
            const auto curveType = slurOrTie.getElementName() == "slur" ? api::CurveType::slur : api::CurveType::tie;
            Converter converter;
            const auto& attr = *slurOrTie.getAttributes();
            const int number = attr.hasNumber ? attr.number.getValue() : 1;
            auto orientation = api::CurveOrientation::unspecified;
            
            if( attr.hasOrientation && attr.orientation == core::OverUnder::over )
            {
                orientation = api::CurveOrientation::overhand;
            }
            else if( attr.hasOrientation && attr.orientation == core::OverUnder::under )
            {
                orientation = api::CurveOrientation::underhand;
            }
            
            auto placement = api::Placement::unspecified;
            
            if( attr.hasPlacement )
            {
                placement = converter.convert( attr.placement );
            }
            
            auto p1 = api::CurvePoints{};
            p1.isXSpecified = attr.hasDefaultX;
            p1.isYSpecified = attr.hasDefaultY;
            p1.isBezierXSpecified = attr.hasBezierX;
            p1.isBezierYSpecified = attr.hasBezierY;
            p1.isTimeOffsetTicksSpecified = attr.hasBezierOffset;
            
            p1.x = p1.isXSpecified ? attr.defaultX.getValue() : 0.0;
            p1.y = p1.isYSpecified ? attr.defaultY.getValue() : 0.0;
            p1.bezierX = p1.isBezierXSpecified ? attr.bezierX.getValue() : 0.0;
            p1.bezierY = p1.isBezierYSpecified ? attr.bezierY.getValue() : 0.0;
            
            p1.timeOffsetTicks = p1.isTimeOffsetTicksSpecified ?
            static_cast<int>( attr.bezierOffset.getValue() ) : 0;
            
            if( attr.type == core::StartStopContinue::start )
            {
                api::CurveStart curveStart{ curveType };
                curveStart.numberLevel = number;
                curveStart.curveOrientation = orientation;
                curveStart.placement = placement;
                curveStart.curvePoints = p1;
                
                auto lineData = api::LineData{};
                
                // TODO - move to converter
                if( attr.hasLineType )
                {
                    switch ( attr.lineType )
                    {
                        case core::LineType::solid:
                            lineData.lineType = api::LineType::solid;
                            break;
                        case core::LineType::wavy:
                            lineData.lineType = api::LineType::wavy;
                            break;
                        case core::LineType::dashed:
                            lineData.lineType = api::LineType::dashed;
                            break;
                        case core::LineType::dotted:
                            lineData.lineType = api::LineType::dotted;
                            break;
                        default:
                            lineData.lineType = api::LineType::unspecified;
                            break;
                    }
                }
                
                curveStart.isColorSpecified = attr.hasColor;
                if( attr.hasColor )
                {
                    curveStart.colorData.red = static_cast<uint8_t>( attr.color.getRed() );
                    curveStart.colorData.green = static_cast<uint8_t>( attr.color.getGreen() );
                    curveStart.colorData.blue = static_cast<uint8_t>( attr.color.getBlue() );
                    if( attr.color.getColorType() == core::Color::ColorType::ARGB )
                    {
                        curveStart.colorData.isAlphaSpecified = true;
                        curveStart.colorData.alpha = static_cast<uint8_t>( attr.color.getAlpha() );
                    }
                }
                
                if( attr.hasDashLength )
                {
                    lineData.isDashLengthSpecified = true;
                    lineData.dashLength = attr.dashLength.getValue();
                }
                
                if( attr.hasSpaceLength )
                {
                    lineData.isSpaceLengthSpecified = true;
                    lineData.spaceLength = attr.spaceLength.getValue();
                }
                curveStart.lineData = std::move( lineData );
                outNoteData.noteAttachmentData.curveStarts.emplace_back( std::move( curveStart ) );
            }
            else if ( attr.type == core::StartStopContinue::continue_ )
            {
                api::CurveContinue curveContinue{ curveType };
                curveContinue.numberLevel = number;
                curveContinue.incomingCurvePoints = p1;
                
                auto p2 = api::CurvePoints{};
                p2.isXSpecified = attr.hasDefaultX;
                p2.isYSpecified = attr.hasDefaultY;
                p2.isBezierXSpecified = attr.hasBezierX;
                p2.isBezierYSpecified = attr.hasBezierY;
                p2.isTimeOffsetTicksSpecified = attr.hasBezierOffset;
                
                p2.x = p2.isXSpecified ? attr.defaultX.getValue() : 0.0;
                p2.y = p2.isYSpecified ? attr.defaultY.getValue() : 0.0;
                p2.bezierX = p2.isBezierXSpecified ? attr.bezierX.getValue() : 0.0;
                p2.bezierY = p2.isBezierYSpecified ? attr.bezierY.getValue() : 0.0;
                
                p2.timeOffsetTicks = p2.isTimeOffsetTicksSpecified ?
                static_cast<int>( attr.bezierOffset.getValue() ) : 0;
                
                curveContinue.outgoingCurvePoints = p2;
                outNoteData.noteAttachmentData.curveContinuations.emplace_back( std::move( curveContinue ) );
                
            }
            else if ( attr.type == core::StartStopContinue::stop )
            {
                api::CurveEnd curveEnd{ curveType };
                curveEnd.numberLevel = number;
                curveEnd.curvePoints = p1;
                outNoteData.noteAttachmentData.curveEnds.emplace_back( std::move( curveEnd ) );
            }
        }
    }
}
