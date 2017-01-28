// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
#include "mx/core/elements/Dynamics.h"
#include "mx/utility/OptionalMembers.h"
#include "mx/impl/CurveFunctions.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/core/elements/Tuplet.h"
#include "mx/core/elements/TupletActual.h"
#include "mx/core/elements/TupletNormal.h"
#include "mx/core/elements/TupletDot.h"
#include "mx/core/elements/TupletNumber.h"
#include "mx/core/elements/TupletType.h"
#include "mx/impl/DynamicsWriter.h"
#include "mx/core/elements/Fermata.h"

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
                    writeAttributesFromCurveStart( curve, *attr );
                }
                else if( curve.curveType == api::CurveType::slur )
                {
                    curveNotationsChoice->setChoice( core::NotationsChoice::Choice::slur );
                    auto element = curveNotationsChoice->getSlur();
                    auto attr = element->getAttributes();
                    writeAttributesFromCurveStart( curve, *attr );
                }
            }
            
            for( const auto& curve : myNoteData.noteAttachmentData.curveContinuations )
            {
                auto curveNotationsChoice = core::makeNotationsChoice();
                myOutNotations->addNotationsChoice( curveNotationsChoice );
                
                if( curve.curveType == api::CurveType::tie )
                {
                    curveNotationsChoice->setChoice( core::NotationsChoice::Choice::tied );
                    auto element = curveNotationsChoice->getTied();
                    auto attr = element->getAttributes();
                    writeAttributesFromCurveContinue( curve, *attr );
                }
                else if( curve.curveType == api::CurveType::slur )
                {
                    curveNotationsChoice->setChoice( core::NotationsChoice::Choice::slur );
                    auto element = curveNotationsChoice->getSlur();
                    auto attr = element->getAttributes();
                    writeAttributesFromCurveContinue( curve, *attr );
                }
            }
            
            for( const auto& curve : myNoteData.noteAttachmentData.curveStops )
            {
                auto curveNotationsChoice = core::makeNotationsChoice();
                myOutNotations->addNotationsChoice( curveNotationsChoice );
                
                if( curve.curveType == api::CurveType::tie )
                {
                    curveNotationsChoice->setChoice( core::NotationsChoice::Choice::tied );
                    auto element = curveNotationsChoice->getTied();
                    auto attr = element->getAttributes();
                    writeAttributesFromCurveStop( curve, *attr );
                }
                else if( curve.curveType == api::CurveType::slur )
                {
                    curveNotationsChoice->setChoice( core::NotationsChoice::Choice::slur );
                    auto element = curveNotationsChoice->getSlur();
                    auto attr = element->getAttributes();
                    writeAttributesFromCurveStop( curve, *attr );
                }
            }
            
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
            
            
            for( const auto& tupletStop : myNoteData.noteAttachmentData.tupletStops )
            {
                auto tupletNotationsChoice = core::makeNotationsChoice();
                myOutNotations->addNotationsChoice( tupletNotationsChoice );
                tupletNotationsChoice->setChoice( core::NotationsChoice::Choice::tuplet );
                auto tuplet = tupletNotationsChoice->getTuplet();
                tuplet->getAttributes()->type = core::StartStop::stop;
                
                if( tupletStop.numberLevel > 0 )
                {
                    tuplet->getAttributes()->hasNumber = true;
                    tuplet->getAttributes()->number = core::NumberLevel{ tupletStop.numberLevel };
                }
            }
            
            
            for( const auto& mark : myNoteData.noteAttachmentData.marks )
            {
                if( isMarkArticulation( mark.markType ) )
                {
                    this->addArticulation( mark, articulations );
                }
                else if( isMarkDynamic( mark.markType) )
                {
                    auto dynamicNotationsChoice = core::makeNotationsChoice();
                    myOutNotations->addNotationsChoice( dynamicNotationsChoice );
                    dynamicNotationsChoice->setChoice( core::NotationsChoice::Choice::dynamics );
                    DynamicsWriter dynamicsWriter{ mark, myCursor };
                    dynamicNotationsChoice->setDynamics( dynamicsWriter.getDynamics() );
                }
                else if( isMarkFermata( mark.markType ) )
                {
                    auto fermataNotationsChoice = core::makeNotationsChoice();
                    myOutNotations->addNotationsChoice( fermataNotationsChoice );
                    fermataNotationsChoice->setChoice( core::NotationsChoice::Choice::fermata );
                    auto& fermata = *fermataNotationsChoice->getFermata();
                    auto& attr = *fermata.getAttributes();
                    impl::setAttributesFromMarkData( mark, attr );
                    
                    if( mark.markType == api::MarkType::fermata )
                    {
                        fermata.setValue( core::FermataShape::emptystring );
                        attr.hasType = false;
                    }
                    else if( mark.markType == api::MarkType::fermataNormal )
                    {
                        fermata.setValue( core::FermataShape::normal );
                        attr.hasType = false;
                    }
                    else if( mark.markType == api::MarkType::fermataAngled )
                    {
                        fermata.setValue( core::FermataShape::angled );
                        attr.hasType = false;
                    }
                    else if( mark.markType == api::MarkType::fermataSquare )
                    {
                        fermata.setValue( core::FermataShape::square );
                        attr.hasType = false;
                    }
                    else if( mark.markType == api::MarkType::fermataUpright )
                    {
                        fermata.setValue( core::FermataShape::emptystring );
                        attr.hasType = true;
                        attr.type = core::UprightInverted::upright;
                    }
                    else if( mark.markType == api::MarkType::fermataNormalUpright )
                    {
                        fermata.setValue( core::FermataShape::normal );
                        attr.hasType = true;
                        attr.type = core::UprightInverted::upright;
                    }
                    else if( mark.markType == api::MarkType::fermataAngledUpright )
                    {
                        fermata.setValue( core::FermataShape::angled );
                        attr.hasType = true;
                        attr.type = core::UprightInverted::upright;
                    }
                    else if( mark.markType == api::MarkType::fermataSquareUpright )
                    {
                        fermata.setValue( core::FermataShape::square );
                        attr.hasType = true;
                        attr.type = core::UprightInverted::upright;
                    }
                    else if( mark.markType == api::MarkType::fermataInverted )
                    {
                        fermata.setValue( core::FermataShape::emptystring );
                        attr.hasType = true;
                        attr.type = core::UprightInverted::inverted;
                    }
                    else if( mark.markType == api::MarkType::fermataNormalInverted )
                    {
                        fermata.setValue( core::FermataShape::normal );
                        attr.hasType = true;
                        attr.type = core::UprightInverted::inverted;
                    }
                    else if( mark.markType == api::MarkType::fermataAngledInverted )
                    {
                        fermata.setValue( core::FermataShape::angled );
                        attr.hasType = true;
                        attr.type = core::UprightInverted::inverted;
                    }
                    else if( mark.markType == api::MarkType::fermataSquareInverted )
                    {
                        fermata.setValue( core::FermataShape::square );
                        attr.hasType = true;
                        attr.type = core::UprightInverted::inverted;
                    }
                    
                }
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
