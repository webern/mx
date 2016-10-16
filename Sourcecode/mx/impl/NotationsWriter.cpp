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
#include "mx/utility/OptionalMembers.h"
#include "mx/impl/CurveFunctions.h"
#include "mx/core/elements/Tuplet.h"
#include "mx/core/elements/TupletActual.h"
#include "mx/core/elements/TupletNormal.h"
#include "mx/core/elements/TupletDot.h"
#include "mx/core/elements/TupletNumber.h"
#include "mx/core/elements/TupletType.h"

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
                    writeCurveStartAttributes( curve, attr );
                }
                else if( curve.curveType == api::CurveType::slur )
                {
                    curveNotationsChoice->setChoice( core::NotationsChoice::Choice::slur );
                    auto element = curveNotationsChoice->getSlur();
                    auto attr = element->getAttributes();
                    writeCurveStartAttributes( curve, attr );
                }
            }
            
            // TODO - curve continues and curve stops
            
            for( const auto& tupletStart : myNoteData.noteAttachmentData.tupletStarts )
            {
                auto tupletNotationsChoice = core::makeNotationsChoice();
                myOutNotations->addNotationsChoice( tupletNotationsChoice );
                tupletNotationsChoice->setChoice( core::NotationsChoice::Choice::tuplet );
                auto tuplet = tupletNotationsChoice->getTuplet();
                tuplet->getAttributes()->type = core::StartStop::start;
                tuplet->setHasTupletActual( true );
                tuplet->setHasTupletNormal( true );
                tuplet->getTupletActual()->getTupletNumber()->setValue( core::NonNegativeInteger{ tupletStart.actualNumber } );
                tuplet->getTupletNormal()->getTupletNumber()->setValue( core::NonNegativeInteger{ tupletStart.normalNumber } );
                tuplet->getTupletNormal()->setHasTupletNumber( true );
                tuplet->getTupletActual()->setHasTupletNumber( true );
                
                if( tupletStart.numberLevel > 0 )
                {
                    tuplet->getAttributes()->hasNumber = true;
                    tuplet->getAttributes()->number = core::NumberLevel{ tupletStart.numberLevel };
                }
                
                if( tupletStart.bracket != api::Bool::unspecified )
                {
                    tuplet->getAttributes()->hasBracket = true;
                    tuplet->getAttributes()->bracket = myConverter.convert( tupletStart.bracket );
                }
                
                if( tupletStart.showActualNumber != api::Bool::unspecified )
                {
                    auto& hasShow = tuplet->getAttributes()->hasShowNumber;
                    auto& show = tuplet->getAttributes()->showNumber;
                    
                    if( tupletStart.showActualNumber == api::Bool::yes )
                    {
                        if( tupletStart.showNormalNumber == api::Bool::yes )
                        {
                            hasShow = true;
                            show = core::ShowTuplet::both;
                        }
                        else
                        {
                            hasShow = true;
                            show = core::ShowTuplet::actual;
                        }
                    }
                    else if( tupletStart.showActualNumber == api::Bool::no )
                    {
                        hasShow = true;
                        show = core::ShowTuplet::none;
                    }
                }
            }
            
            
            for( const auto& tupletEnd : myNoteData.noteAttachmentData.tupletEnds )
            {
                auto tupletNotationsChoice = core::makeNotationsChoice();
                myOutNotations->addNotationsChoice( tupletNotationsChoice );
                tupletNotationsChoice->setChoice( core::NotationsChoice::Choice::tuplet );
                auto tuplet = tupletNotationsChoice->getTuplet();
                tuplet->getAttributes()->type = core::StartStop::stop;
                
                if( tupletEnd.numberLevel > 0 )
                {
                    tuplet->getAttributes()->hasNumber = true;
                    tuplet->getAttributes()->number = core::NumberLevel{ tupletEnd.numberLevel };
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
