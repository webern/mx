// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/NotationsWriter.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NotationsChoice.h"
#include "mx/core/elements/Articulations.h"
#include "mx/core/elements/ArticulationsChoice.h"
#include "mx/core/elements/Accent.h"
#include "mx/core/elements/BreathMark.h"
#include "mx/core/elements/Caesura.h"
#include "mx/core/elements/DetachedLegato.h"
#include "mx/core/elements/Doit.h"
#include "mx/core/elements/Falloff.h"
#include "mx/core/elements/OtherArticulation.h"
#include "mx/core/elements/Plop.h"
#include "mx/core/elements/Scoop.h"
#include "mx/core/elements/Spiccato.h"
#include "mx/core/elements/Staccatissimo.h"
#include "mx/core/elements/Staccato.h"
#include "mx/core/elements/Stress.h"
#include "mx/core/elements/StrongAccent.h"
#include "mx/core/elements/Tenuto.h"
#include "mx/core/elements/Unstress.h"
#include "mx/core/elements/Tied.h"
#include "mx/core/elements/Slur.h"

namespace mx
{
    namespace impl
    {
        NotationsWriter::NotationsWriter( const api::NoteData& inNoteData, const MeasureCursor& inCursor, const ScoreWriter& inScoreWriter )
        : myNoteData{ inNoteData }
        , myCursor{ inCursor }
        , myScoreWriter{ inScoreWriter }
        , myConverter{}
        {
            
        }
        
        
        core::NotationsPtr NotationsWriter::getNotations() const
        {
            myOutNotations = core::makeNotations();
            auto articulationsNotationChoice = makeArticulationsNotationsChoice();
            auto articulations = articulationsNotationChoice->getArticulations();
            
            for( const auto& curve : myNoteData.noteAttachmentData.curveStarts )
            {
                auto curveNotationsChoice = core::makeNotationsChoice();
                myOutNotations->addNotationsChoice( curveNotationsChoice );
                
                if( curve.curveType == api::CurveType::tie )
                {
                    curveNotationsChoice->setChoice( core::NotationsChoice::Choice::tied );
                    auto element = curveNotationsChoice->getTied();
                    auto attr = element->getAttributes();
                    if( curve.numberLevel > 0 )
                    {
                        attr->hasNumber = true;
                        attr->number = core::NumberLevel{ curve.numberLevel };
                    }
                    
                    if( curve.curvePoints.isSpecified() )
                    {
                        const auto& p = curve.curvePoints;
                        
                        if( p.isBezierXSpecified )
                        {
                            attr->hasBezierX = true;
                            attr->bezierX = core::TenthsValue{ p.bezierX };
                        }
                        
                        if( p.isBezierYSpecified )
                        {
                            attr->hasBezierY = true;
                            attr->bezierY = core::TenthsValue{ p.bezierY };
                        }
                        
                        if( p.isXSpecified )
                        {
                            attr->hasDefaultX = true;
                            attr->defaultX = core::TenthsValue{ p.x };
                        }
                        
                        if( p.isYSpecified )
                        {
                            attr->hasDefaultY = true;
                            attr->defaultY = core::TenthsValue{ p.y };
                        }
                        
                        if( p.isTimeOffsetTicksSpecified )
                        {
                            attr->hasBezierOffset = true;
                            attr->bezierOffset = core::DivisionsValue{ static_cast<core::DecimalType>(p.timeOffsetTicks) };
                        }
                    }
                    
                    if( curve.isColorSpecified )
                    {
                        attr->hasColor = true;
                        attr->color.setRed( curve.colorData.red );
                        attr->color.setGreen( curve.colorData.green );
                        attr->color.setBlue( curve.colorData.blue );
                        if( curve.colorData.isAlphaSpecified )
                        {
                            attr->color.setAlpha( curve.colorData.alpha );
                            attr->color.setColorType( core::Color::ColorType::ARGB );
                        }
                        else
                        {
                            attr->color.setColorType( core::Color::ColorType::RGB );
                        }
                    }
                    
                    if( curve.placement != api::Placement::unspecified )
                    {
                        attr->hasPlacement = true;
                        attr->placement = myConverter.convert( curve.placement );
                    }
                    
                    if( curve.lineData.isSpecified() )
                    {
                        if( curve.lineData.lineType != api::LineType::unspecified )
                        {
                            attr->hasLineType = true;
                            attr->lineType = myConverter.convert( curve.lineData.lineType );
                        }
                        
                        if( curve.lineData.isSpaceLengthSpecified )
                        {
                            attr->hasSpaceLength = true;
                            attr->spaceLength = core::TenthsValue{ curve.lineData.spaceLength };
                        }
                        
                        if( curve.lineData.isDashLengthSpecified )
                        {
                            attr->hasDashLength = true;
                            attr->dashLength = core::TenthsValue{ curve.lineData.dashLength };
                        }
                    }
                }
                else if( curve.curveType == api::CurveType::slur )
                {
                    curveNotationsChoice->setChoice( core::NotationsChoice::Choice::slur );
                    auto element = curveNotationsChoice->getSlur();
                    auto attr = element->getAttributes();
                    if( curve.numberLevel > 0 )
                    {
                        attr->hasNumber = true;
                        attr->number = core::NumberLevel{ curve.numberLevel };
                    }
                    
                    if( curve.curvePoints.isSpecified() )
                    {
                        const auto& p = curve.curvePoints;
                        
                        if( p.isBezierXSpecified )
                        {
                            attr->hasBezierX = true;
                            attr->bezierX = core::TenthsValue{ p.bezierX };
                        }
                        
                        if( p.isBezierYSpecified )
                        {
                            attr->hasBezierY = true;
                            attr->bezierY = core::TenthsValue{ p.bezierY };
                        }
                        
                        if( p.isXSpecified )
                        {
                            attr->hasDefaultX = true;
                            attr->defaultX = core::TenthsValue{ p.x };
                        }
                        
                        if( p.isYSpecified )
                        {
                            attr->hasDefaultY = true;
                            attr->defaultY = core::TenthsValue{ p.y };
                        }
                        
                        if( p.isTimeOffsetTicksSpecified )
                        {
                            attr->hasBezierOffset = true;
                            attr->bezierOffset = core::DivisionsValue{ static_cast<core::DecimalType>(p.timeOffsetTicks) };
                        }
                    }
                    
                    if( curve.isColorSpecified )
                    {
                        attr->hasColor = true;
                        attr->color.setRed( curve.colorData.red );
                        attr->color.setGreen( curve.colorData.green );
                        attr->color.setBlue( curve.colorData.blue );
                        if( curve.colorData.isAlphaSpecified )
                        {
                            attr->color.setAlpha( curve.colorData.alpha );
                            attr->color.setColorType( core::Color::ColorType::ARGB );
                        }
                        else
                        {
                            attr->color.setColorType( core::Color::ColorType::RGB );
                        }
                    }
                    
                    if( curve.placement != api::Placement::unspecified )
                    {
                        attr->hasPlacement = true;
                        attr->placement = myConverter.convert( curve.placement );
                    }
                    
                    if( curve.lineData.isSpecified() )
                    {
                        if( curve.lineData.lineType != api::LineType::unspecified )
                        {
                            attr->hasLineType = true;
                            attr->lineType = myConverter.convert( curve.lineData.lineType );
                        }
                        
                        if( curve.lineData.isSpaceLengthSpecified )
                        {
                            attr->hasSpaceLength = true;
                            attr->spaceLength = core::TenthsValue{ curve.lineData.spaceLength };
                        }
                        
                        if( curve.lineData.isDashLengthSpecified )
                        {
                            attr->hasDashLength = true;
                            attr->dashLength = core::TenthsValue{ curve.lineData.dashLength };
                        }
                    }
                }
            }
            
            for( const auto& mark : myNoteData.noteAttachmentData.marks )
            {
                addArticulation( mark, articulations );
            }
            
            if( articulations->getArticulationsChoiceSet().size() > 0 )
            {
                myOutNotations->addNotationsChoice( articulationsNotationChoice );
            }
            
            return myOutNotations;
        }
        
        
        core::NotationsChoicePtr NotationsWriter::makeArticulationsNotationsChoice() const
        {
            myOutNotations = core::makeNotations();
            auto articulations = core::makeArticulations();
            auto notationsChoice = core::makeNotationsChoice();
            notationsChoice->setChoice( core::NotationsChoice::Choice::articulations );
            return notationsChoice;
        }
        
        
        void NotationsWriter::addArticulation( const api::MarkData& mark, const core::ArticulationsPtr& outArticulationsPtr ) const
        {
            auto articulationsChoice = core::makeArticulationsChoice();
            auto choiceValue = myConverter.convertArticulation( mark.markType );
            articulationsChoice->setChoice( choiceValue );
            outArticulationsPtr->addArticulationsChoice( articulationsChoice );
            
            if( !myConverter.isArticulation( mark.markType ) )
            {
                return;
            }
            
            if( mark.markType == api::MarkType::accent )
            {

            }
            else if( mark.markType == api::MarkType::strongAccent )
            {
                
            }
            else if( mark.markType == api::MarkType::staccato )
            {
                
            }
            else if( mark.markType == api::MarkType::tenuto )
            {
                
            }
            else if( mark.markType == api::MarkType::detachedLegato )
            {
                
            }
            else if( mark.markType == api::MarkType::staccatissimo )
            {
                
            }
            else if( mark.markType == api::MarkType::spiccato )
            {
                
            }
            else if( mark.markType == api::MarkType::scoop )
            {
                
            }
            else if( mark.markType == api::MarkType::plop )
            {
                
            }
            else if( mark.markType == api::MarkType::doit )
            {
                
            }
            else if( mark.markType == api::MarkType::falloff )
            {
                
            }
            else if( mark.markType == api::MarkType::breathMark )
            {
                
            }
            else if( mark.markType == api::MarkType::caesura )
            {
                
            }
            else if( mark.markType == api::MarkType::stress )
            {
                
            }
            else if( mark.markType == api::MarkType::unstress )
            {
                
            }
            else if( mark.markType == api::MarkType::otherArticulation )
            {
                
            }
        }
    }
}
