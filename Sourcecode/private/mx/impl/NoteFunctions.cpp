// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NoteFunctions.h"
#include "mx/impl/PositionFunctions.h"
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
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/FromXElement.h"
#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/impl/ArticulationsFunctions.h"
#include "mx/impl/CurveFunctions.h"
#include "mx/impl/DynamicsReader.h"
#include "mx/impl/NoteReader.h"
#include "mx/impl/OrnamentsFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/TechnicalFunctions.h"
#include "mx/impl/TimeReader.h"
#include "mx/impl/TupletReader.h"
#include "mx/utility/Throw.h"
#include "mx/impl/MarkDataFunctions.h"

#include <algorithm>

namespace mx
{
    namespace impl
    {
        NoteFunctions::NoteFunctions( const core::Note& inMxNote, impl::Cursor inCursor )
        : myNote{ inMxNote }
        , myCursor{ inCursor }
        , myOutNoteData{}
        , myConverter()
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
            myOutNoteData.pitchData.cents = reader.getCents();
            
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
            //myOutNoteData.staffIndex = reader.getStaffNumber() - 1;
            myOutNoteData.userRequestedVoiceNumber = reader.getVoiceNumber();
            
            myOutNoteData.notehead = converter.convert( reader.getNoteheadValue() );

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
            myOutNoteData.durationData.timeModificationNormalType = converter.convert( reader.getTimeModificationNormalType() );
            myOutNoteData.durationData.timeModificationNormalTypeDots = reader.getTimeModificationNormalTypeDots();

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

            parseMiscData();
            const auto& incomingNoteAttributes = *(myNote.getAttributes());
            myOutNoteData.positionData = impl::getPositionData( incomingNoteAttributes );

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
                        {
                            parseCurve( *notationsChoice.getTied(), myOutNoteData );
                            break;
                        }
                        case core::NotationsChoice::Choice::slur:
                        {
                            parseCurve( *notationsChoice.getSlur(), myOutNoteData );
                            break;
                        }
                        case core::NotationsChoice::Choice::tuplet:
                        {
                            TupletReader reader{ *notationsChoice.getTuplet(), myCursor, myNote };
                            reader.parseTuplet( myOutNoteData.noteAttachmentData.tupletStarts, myOutNoteData.noteAttachmentData.tupletStops );
                            break;
                        }
                        case core::NotationsChoice::Choice::slide:
                        {
//                            const auto& slide = *notationsChoice.getSlide();
//                            const auto& attr = *slide.getAttributes();
//                            const auto slideType = attr.type;

//                            api::SpannerData spannerData;
//                            spannerData.name = "slide";
//                            spannerData.spannerType = api::SpannerType::slide;
//                            spannerData.tickTimePosition = myCursor.tickTimePosition;
//                            
//                            if( attr.hasNumber )
//                            {
//                                spannerData.numberLevel = attr.number.getValue();
//                            }
//
//                            if( slideType == core::StartStop::start )
//                            {
//                                myOutNoteData.noteAttachmentData.spannerStarts.emplace_back( std::move( spannerData ) );
//                            }
//                            else
//                            {
//                                myOutNoteData.noteAttachmentData.spannerStops.emplace_back( std::move( spannerData ) );
//                            }
                            
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
                            DynamicsReader funcs{ *notationsChoice.getDynamics(), myCursor };
                            funcs.parseDynamics( myOutNoteData.noteAttachmentData.marks );
                            break;
                        }
                        case core::NotationsChoice::Choice::fermata:
                        {
                            const auto& fermata = *notationsChoice.getFermata();
                            const auto& attr = *fermata.getAttributes();
                            const auto shape = fermata.getValue();
                            auto markType = api::MarkType::fermata;
                            
                            if( shape == core::FermataShape::emptystring )
                            {
                                if( !attr.hasType )
                                {
                                    markType = api::MarkType::fermata;
                                }
                                else if ( attr.type == core::UprightInverted::upright )
                                {
                                    markType = api::MarkType::fermataUpright;
                                }
                                else if ( attr.type == core::UprightInverted::inverted )
                                {
                                    markType = api::MarkType::fermataInverted;
                                }
                            }
                            else if( shape == core::FermataShape::normal )
                            {
                                if( !attr.hasType )
                                {
                                    markType = api::MarkType::fermataNormal;
                                }
                                else if ( attr.type == core::UprightInverted::upright )
                                {
                                    markType = api::MarkType::fermataNormalUpright;
                                }
                                else if ( attr.type == core::UprightInverted::inverted )
                                {
                                    markType = api::MarkType::fermataNormalInverted;
                                }
                                
                            }
                            else if( shape == core::FermataShape::angled )
                            {
                                if( !attr.hasType )
                                {
                                    markType = api::MarkType::fermataAngled;
                                }
                                else if ( attr.type == core::UprightInverted::upright )
                                {
                                    markType = api::MarkType::fermataAngledUpright;
                                }
                                else if ( attr.type == core::UprightInverted::inverted )
                                {
                                    markType = api::MarkType::fermataAngledInverted;
                                }
                            }
                            else if( shape == core::FermataShape::square )
                            {
                                if( !attr.hasType )
                                {
                                    markType = api::MarkType::fermataSquare;
                                }
                                else if ( attr.type == core::UprightInverted::upright )
                                {
                                    markType = api::MarkType::fermataSquareUpright;
                                }
                                else if ( attr.type == core::UprightInverted::inverted )
                                {
                                    markType = api::MarkType::fermataSquareInverted;
                                }
                            }
                            
                            // Unfortunately, without doing a lot of guess word, we can't
                            // know whether the correct glyph is "above" or "below"
                            api::MarkData markData{ markType };
                            impl::parseMarkDataAttributes( attr, markData );
                            markData.tickTimePosition = myCursor.tickTimePosition;
                            impl::parseMarkDataAttributes( attr, markData );
                            myOutNoteData.noteAttachmentData.marks.emplace_back( std::move( markData ) );
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

        void NoteFunctions::parseMiscData() const
        {
            if (!myNote.getEditorialVoiceGroup()->getHasFootnote())
            {
                return;
            }

            auto footnote = myNote.getEditorialVoiceGroup()->getFootnote();

            if( footnote->getValue().getValue().size() > 0 )
            {
                // we expect this element to be unused if it is
                // stuffed with ##misc-data## in the font-family
                return;
            }

            auto attr = footnote->getAttributes();

            if( !attr->hasFontFamily )
            {
                return;
            }

            auto& miscData = attr->fontFamily;
            auto iter = miscData.getValuesBeginConst();
            const auto end = miscData.getValuesEndConst();

            if( iter == end )
            {
                return;
            }

            const auto firstString = iter->getValue();

            if( firstString.substr(0,13) != "##misc-data##" )
            {
                return;
            }

            myOutNoteData.miscData.push_back( firstString.substr( 13 ) );
            ++iter;

            for( ; iter != end; ++iter)
            {
                myOutNoteData.miscData.push_back( iter->getValue() );
            }
        }
    }
}
