// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/NoteFunctions.h"
#include "mx/api/MarkData.h"
#include "mx/api/SpannerData.h"
#include "mx/core/elements/Accent.h"
#include "mx/core/elements/AccidentalMark.h"
#include "mx/core/elements/Arpeggiate.h"
#include "mx/core/elements/Articulations.h"
#include "mx/core/elements/ArticulationsChoice.h"
#include "mx/core/elements/BreathMark.h"
#include "mx/core/elements/Caesura.h"
#include "mx/core/elements/DetachedLegato.h"
#include "mx/core/elements/Doit.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Falloff.h"
#include "mx/core/elements/Fermata.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Glissando.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/NonArpeggiate.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NotationsChoice.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Ornaments.h"
#include "mx/core/elements/OtherArticulation.h"
#include "mx/core/elements/OtherNotation.h"
#include "mx/core/elements/Plop.h"
#include "mx/core/elements/Scoop.h"
#include "mx/core/elements/Slide.h"
#include "mx/core/elements/Slur.h"
#include "mx/core/elements/Spiccato.h"
#include "mx/core/elements/Staccatissimo.h"
#include "mx/core/elements/Staccato.h"
#include "mx/core/elements/Stress.h"
#include "mx/core/elements/StrongAccent.h"
#include "mx/core/elements/Technical.h"
#include "mx/core/elements/Tenuto.h"
#include "mx/core/elements/Tied.h"
#include "mx/core/elements/Tuplet.h"
#include "mx/core/elements/Unstress.h"
#include "mx/core/FromXElement.h"
#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/impl/ArticulationsFunctions.h"
#include "mx/impl/CurveFunctions.h"
#include "mx/impl/DynamicsFunctions.h"
#include "mx/impl/NoteReader.h"
#include "mx/impl/OrnamentsFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/TechnicalFunctions.h"
#include "mx/impl/TimeReader.h"
#include "mx/impl/TupletReader.h"
#include "mx/utility/Throw.h"

#include <algorithm>

namespace mx
{
    namespace impl
    {
        NoteFunctions::NoteFunctions( const core::Note& inMxNote, impl::Cursor inCursor )
        : myNote{ inMxNote }
        , myCursor{ inCursor }
        , myOutNoteData{}
        {}
        
        
        api::NoteData NoteFunctions::parseNote( ) const
        {
            // lock it because we have the function marked as const
            // but we are writing to a mutable NoteData (for convenience)
            std::lock_guard<std::mutex> guard{ myMutex };
            myOutNoteData = api::NoteData{};
            impl::NoteReader reader{ myNote };
            
            if( reader.getIsRest() )
            {
                myOutNoteData.isRest = true;
                myOutNoteData.isMeasureRest = reader.getIsMeasureRest();
            }
            
            myOutNoteData.isUnpitched = reader.getIsUnpitched();
            myOutNoteData.isDisplayStepOctaveSpecified = reader.getIsDisplayStepOctaveSpecified();
            myOutNoteData.isChord = reader.getIsChord();
            
            if( reader.getIsCue() )
            {
                myOutNoteData.noteType = api::NoteType::cue;
            }
            
            if( reader.getIsGrace() )
            {
                myOutNoteData.noteType = api::NoteType::grace;
            }
            
            if( reader.getIsNormal() )
            {
                myOutNoteData.noteType = api::NoteType::normal;
            }
            auto converter = Converter{};
            myOutNoteData.pitchData.step = converter.convert( reader.getStep() );
            myOutNoteData.pitchData.alter = reader.getAlter();
            
            myOutNoteData.pitchData.accidental = api::Accidental::none;
            
            if( reader.getHasAccidental() )
            {
                myOutNoteData.pitchData.accidental = converter.convert( reader.getAccidental() );
                myOutNoteData.pitchData.isAccidentalParenthetical = reader.getIsAccidentalParenthetical();
                myOutNoteData.pitchData.isAccidentalCautionary = reader.getIsAccidentalCautionary();
                myOutNoteData.pitchData.isAccidentalEditorial = reader.getIsAccidentalEditorial();
                myOutNoteData.pitchData.isAccidentalBracketed = reader.getIsAccidentalBracketed();
            }
            
            myOutNoteData.pitchData.octave = reader.getOctave();
            myOutNoteData.staffIndex = reader.getStaffNumber() - 1;
            myOutNoteData.userRequestedVoiceNumber = reader.getVoiceNumber();
            
            if( reader.getIsDurationTypeSpecified() )
            {
                myOutNoteData.durationData.durationName = converter.convert( reader.getDurationType() );
            }
            else
            {
                myOutNoteData.durationData.durationName = deriveNoteTypeFromDurationValue( reader );
            }
            
            myOutNoteData.durationData.durationDots = reader.getNumDots();
            myOutNoteData.durationData.durationTimeTicks = myCursor.convertDurationToGlobalTickScale( reader.getDurationValue() );
            myOutNoteData.tickTimePosition = myCursor.tickTimePosition;
            
            for( const auto& coreBeamVal : reader.getBeams() )
            {
                myOutNoteData.beams.push_back( converter.convert( coreBeamVal ) );
            }
            
            myOutNoteData.durationData.timeModificationActualNotes = reader.getTimeModificationActualNotes();
            myOutNoteData.durationData.timeModificationNormalNotes = reader.getTimeModificationNormalNotes();
            
            const core::NoteTypeValue timeModType = reader.getTimeModificationNormalType();
            const int timeModTypeDots = reader.getTimeModificationNormalTypeDots();
            bool isTimeModTypeSpecified = ( timeModTypeDots > 0 ) && ( timeModType != reader.getDurationType() );
            
            if( isTimeModTypeSpecified )
            {
                myOutNoteData.durationData.timeModificationNormalType = converter.convert( timeModType );
                myOutNoteData.durationData.timeModificationNormalTypeDots = timeModTypeDots;
            }
            else
            {
                myOutNoteData.durationData.timeModificationNormalType = api::DurationName::unspecified;
                myOutNoteData.durationData.timeModificationNormalTypeDots = 0;
            }
            parseNotations();
            myOutNoteData.positionData = impl::getPositionData( *myNote.getAttributes() );
            myOutNoteData.printData = impl::getPrintData( *myNote.getAttributes() );
            
            if( reader.getIsStemSpecified() )
            {
                myOutNoteData.stem = converter.convert( reader.getStem() );
            }
            myOutNoteData.isTieStart = reader.getIsTieStart();
            myOutNoteData.isTieStop = reader.getIsTieStop();
            return myOutNoteData;
        }
        
        // TODO - make this work even if notes are dotted and if they are tupleted
        api::DurationName NoteFunctions::deriveNoteTypeFromDurationValue( const NoteReader& reader ) const
        {
            const long double durationValue = reader.getDurationValue();
            const long double ticksPerQuarter = static_cast<long double>( myCursor.getGlobalTicksPerQuarter() );
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_QUARTER * ticksPerQuarter ) )
            {
                return api::DurationName::quarter;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_HALF * ticksPerQuarter ) )
            {
                return api::DurationName::half;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_WHOLE * ticksPerQuarter ) )
            {
                return api::DurationName::whole;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_EIGHTH * ticksPerQuarter ) )
            {
                return api::DurationName::eighth;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_16TH * ticksPerQuarter ) )
            {
                return api::DurationName::dur16th;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_32ND * ticksPerQuarter ) )
            {
                return api::DurationName::dur32nd;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_64TH * ticksPerQuarter ) )
            {
                return api::DurationName::dur64th;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_128TH * ticksPerQuarter ) )
            {
                return api::DurationName::dur128th;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_512TH * ticksPerQuarter ) )
            {
                return api::DurationName::dur512th;
            }
            
            if( api::areSame( durationValue, api::DUR_QUARTERS_VALUE_1024TH * ticksPerQuarter ) )
            {
                return api::DurationName::dur1024th;
            }

            return api::DurationName::unspecified;
        }
        
        
        void NoteFunctions::parseNotations() const
        {
            for( const auto& notationsPtr : myNote.getNotationsSet() )
            {
                for( const auto& notationsChoicePtr : notationsPtr->getNotationsChoiceSet() )
                {
                    const auto& notationsChoice = *notationsChoicePtr;
                    const auto choice = notationsChoice.getChoice();
                    
                    switch (choice)
                    {
                        case core::NotationsChoice::Choice::tied:
                            slurryness( *notationsChoice.getTied(), myOutNoteData );
                            break;
                        case core::NotationsChoice::Choice::slur:
                        {
                            Converter converter;
                            const auto& slur = *notationsChoice.getSlur();
                            const auto& attr = *slur.getAttributes();
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
                                api::CurveStart curveStart{ api::CurveType::slur };
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
                                myOutNoteData.noteAttachmentData.curveStarts.emplace_back( std::move( curveStart ) );
                            }
                            else if ( attr.type == core::StartStopContinue::continue_ )
                            {
                                api::CurveContinue curveContinue{ api::CurveType::slur };
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
                                myOutNoteData.noteAttachmentData.curveContinuations.emplace_back( std::move( curveContinue ) );
                                
                            }
                            else if ( attr.type == core::StartStopContinue::stop )
                            {
                                api::CurveEnd curveEnd{ api::CurveType::slur };
                                curveEnd.numberLevel = number;
                                curveEnd.curvePoints = p1;
                                myOutNoteData.noteAttachmentData.curveEnds.emplace_back( std::move( curveEnd ) );
                            }
                            break;
                        }
                        case core::NotationsChoice::Choice::tuplet:
                        {
                            TupletReader reader{ *notationsChoice.getTuplet(), myCursor, myNote };
                            reader.parseTuplet( myOutNoteData.noteAttachmentData.tupletStarts, myOutNoteData.noteAttachmentData.tupletEnds );
                            break;
                        }
                        case core::NotationsChoice::Choice::slide:
                        {
                            const auto& slide = *notationsChoice.getSlide();
                            const auto& attr = *slide.getAttributes();
                            const auto slideType = attr.type;

                            api::SpannerData spannerData;
                            spannerData.name = "slide";
                            spannerData.spannerType = api::SpannerType::slide;
                            spannerData.tickTimePosition = myCursor.tickTimePosition;
                            
                            if( attr.hasNumber )
                            {
                                spannerData.numberLevel = attr.number.getValue();
                            }

                            if( slideType == core::StartStop::start )
                            {
                                myOutNoteData.noteAttachmentData.spannerStarts.emplace_back( std::move( spannerData ) );
                            }
                            else
                            {
                                myOutNoteData.noteAttachmentData.spannerEnds.emplace_back( std::move( spannerData ) );
                            }
                            
                            break;
                        }
                        case core::NotationsChoice::Choice::glissando:
                        {
                            break;
                        }
                        case core::NotationsChoice::Choice::ornaments:
                        {
                            // TODO - some ornaments should be treated as spanners instead of marks
                            OrnamentsFunctions funcs{ *notationsChoice.getOrnaments(), myCursor };
                            funcs.parseOrnaments( myOutNoteData.noteAttachmentData.marks );
                            break;
                        }
                        case core::NotationsChoice::Choice::technical:
                        {
                            // TODO - some technical items are spanners and others are complex/not-implemented yet
                            TechnicalFunctions funcs{ notationsChoice.getTechnical()->getTechnicalChoiceSet(), myCursor };
                            funcs.parseTechnicalMarks( myOutNoteData.noteAttachmentData.marks );
                            break;
                        }
                        case core::NotationsChoice::Choice::articulations:
                        {
                            ArticulationsFunctions funcs{ notationsChoice.getArticulations()->getArticulationsChoiceSet(), myCursor };
                            funcs.parseArticulations( myOutNoteData.noteAttachmentData.marks );
                            break;
                        }
                        case core::NotationsChoice::Choice::dynamics:
                        {
                            DynamicsFunctions funcs{ *notationsChoice.getDynamics(), myCursor };
                            funcs.parseDynamics( myOutNoteData.noteAttachmentData.marks );
                            break;
                        }
                        case core::NotationsChoice::Choice::fermata:
                        {
                            // TODO - import fermatas
                            break;
                        }
                        case core::NotationsChoice::Choice::arpeggiate:
                        {
                            // TODO - import arpeggiate
                            break;
                        }
                        case core::NotationsChoice::Choice::nonArpeggiate:
                        {
                            // TODO - import non-arpeggiate
                            break;
                        }
                        case core::NotationsChoice::Choice::accidentalMark:
                        {
                            AccidentalMarkFunctions funcs{ *notationsChoice.getAccidentalMark(), myCursor };
                            myOutNoteData.noteAttachmentData.marks.emplace_back( funcs.parseAccidentalMark() );
                            break;
                        }
                        case core::NotationsChoice::Choice::otherNotation:
                        {
                            // TODO - import otherNotation
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        }
    }
}
