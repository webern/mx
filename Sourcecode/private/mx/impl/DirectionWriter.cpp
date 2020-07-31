// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DirectionWriter.h"
#include "mx/api/BarlineData.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Bass.h"
#include "mx/core/elements/BassAlter.h"
#include "mx/core/elements/BassStep.h"
#include "mx/core/elements/BeatUnit.h"
#include "mx/core/elements/BeatUnitDot.h"
#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/elements/BeatUnitPer.h"
#include "mx/core/elements/BeatUnitPerOrNoteRelationNoteChoice.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Degree.h"
#include "mx/core/elements/DegreeAlter.h"
#include "mx/core/elements/DegreeType.h"
#include "mx/core/elements/DegreeValue.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/EditorialVoiceDirectionGroup.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Frame.h"
#include "mx/core/elements/Function.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/HarmonyChordGroup.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Inversion.h"
#include "mx/core/elements/Kind.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PerMinute.h"
#include "mx/core/elements/PerMinuteOrBeatUnitChoice.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Root.h"
#include "mx/core/elements/RootAlter.h"
#include "mx/core/elements/RootStep.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"
#include "mx/impl/DynamicsWriter.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/SpannerFunctions.h"

namespace mx
{
    namespace impl
    {
        DirectionWriter::DirectionWriter( const api::DirectionData& inDirectionData, const Cursor& inCursor )
        : myDirectionData{ inDirectionData }
        , myCursor{ inCursor }
        , myConverter{}
        , myPlacements{}
        {
            
        }

        core::MusicDataChoiceSet DirectionWriter::getDirectionLikeThings()
        {
            core::MusicDataChoiceSet output{};
            auto directionPtr = core::makeDirection();
            myPlacements.clear();
            myIsFirstDirectionTypeAdded = false;
            auto& directionAttributes = *directionPtr->getAttributes();
            
            if( myDirectionData.placement != api::Placement::unspecified )
            {
                directionAttributes.hasPlacement = true;
                directionAttributes.placement = myConverter.convert( myDirectionData.placement );
            }
            
            if( myDirectionData.isStaffValueSpecified || myCursor.staffIndex != 0 )
            {
                directionPtr->setHasStaff( true );
                directionPtr->getStaff()->setValue( core::PositiveInteger{ myCursor.staffIndex + 1 } );
            }

            int offset = 0;

            if( myDirectionData.tickTimePosition != myCursor.tickTimePosition )
            {
                offset = myDirectionData.tickTimePosition - myCursor.tickTimePosition;
                directionPtr->setHasOffset( true );
                directionPtr->getOffset()->setValue( core::DivisionsValue{ static_cast<core::DecimalType>( offset ) } );
                directionPtr->getOffset()->getAttributes()->hasSound = true;
                directionPtr->getOffset()->getAttributes()->sound = core::YesNo::yes;
            }
            
            for( auto mark : myDirectionData.marks )
            {
                mark.tickTimePosition = myDirectionData.tickTimePosition;
                
                // TODO - skip marks that aren't of the correct type (i.e. direction marks)
                // if !isDirection( mark ) continue;
                if( isMarkDynamic( mark.markType ) )
                {
                    auto directionTypePtr = core::makeDirectionType();
                    this->addDirectionType( directionTypePtr, directionPtr );
                    directionTypePtr->setChoice( core::DirectionType::Choice::dynamics );
                    DynamicsWriter dynamicsWriter{ mark, myCursor };
                    MX_ASSERT( directionTypePtr->getDynamicsSet().size() == 1 );
                    directionTypePtr->addDynamics( dynamicsWriter.getDynamics() );
                    directionTypePtr->removeDynamics( directionTypePtr->getDynamicsSet().cbegin() );
                }

                if( isMarkPedal( mark.markType ) )
                {
                    auto directionTypePtr = core::makeDirectionType();
                    this->addDirectionType( directionTypePtr, directionPtr );
                    directionTypePtr->setChoice( core::DirectionType::Choice::pedal );
                    auto pedalPtr = directionTypePtr->getPedal();
                    auto attr = pedalPtr->getAttributes();

                    if( mark.positionData.placement != api::Placement::unspecified )
                    {
                        directionPtr->getAttributes()->hasPlacement = true;
                        Converter c;
                        directionPtr->getAttributes()->placement = c.convert( mark.positionData.placement );
                    }

                    if( mark.markType == api::MarkType::pedal )
                    {
                        attr->type = core::StartStopChangeContinue::start;
                    }
                    else if ( mark.markType == api::MarkType::damp )
                    {
                        attr->type = core::StartStopChangeContinue::stop;
                    }

                    attr->hasLine = true;
                    attr->line = core::YesNo::no;
                    attr->hasSign = true;
                    attr->sign = core::YesNo::yes;
                    setAttributesFromPositionData( mark.positionData, *attr );
                }
            }
            
            for( const auto& wedgeStart : myDirectionData.wedgeStarts )
            {
                auto wedgeStartDirectionTypePtr = core::makeDirectionType();
                wedgeStartDirectionTypePtr->setChoice( core::DirectionType::Choice::wedge );
                this->addDirectionType( wedgeStartDirectionTypePtr, directionPtr );
                auto wedgePtr = wedgeStartDirectionTypePtr->getWedge();
                auto& attr = *wedgePtr->getAttributes();

                if( wedgeStart.wedgeType != api::WedgeType::unspecified )
                {
                    attr.type = myConverter.convert( wedgeStart.wedgeType );
                }
                
                if( wedgeStart.isSpreadSpecified )
                {
                    attr.hasSpread = true;
                    attr.spread = core::DivisionsValue{ static_cast<core::DecimalType>( wedgeStart.spread ) };
                }
                
                auto& attributes = *wedgePtr->getAttributes();
                
                setAttributesFromPositionData( wedgeStart.positionData, attributes );
                setAttributesFromLineData(wedgeStart.lineData, attributes);
                setAttributesFromColorData(wedgeStart.colorData, attributes);
            }
            
            for( const auto& wedgeStop : myDirectionData.wedgeStops )
            {
                auto wedgeStartDirectionTypePtr = core::makeDirectionType();
                wedgeStartDirectionTypePtr->setChoice( core::DirectionType::Choice::wedge );
                this->addDirectionType( wedgeStartDirectionTypePtr, directionPtr );
                auto wedgePtr = wedgeStartDirectionTypePtr->getWedge();
                auto& attr = *wedgePtr->getAttributes();
                attr.type = core::WedgeType::stop;
                
                if( wedgeStop.isSpreadSpecified )
                {
                    attr.hasSpread = true;
                    attr.spread = core::DivisionsValue{ static_cast<core::DecimalType>( wedgeStop.spread ) };
                }
                auto& attributes = *wedgePtr->getAttributes();
                setAttributesFromPositionData( wedgeStop.positionData, attributes );
            }
            
            for( const auto& ottavaStart : myDirectionData.ottavaStarts )
            {
                auto oStartDir = core::makeDirectionType();
                this->addDirectionType( oStartDir, directionPtr );
                oStartDir->setChoice( core::DirectionType::Choice::octaveShift );
                auto oStart = oStartDir->getOctaveShift();
                auto& attr = *oStart->getAttributes();
                impl::setAttributesFromLineData( ottavaStart.spannerStart.lineData, attr );
                
                attr.hasSize = true;
                
                switch( ottavaStart.ottavaType )
                {
                    case api::OttavaType::o15ma:
                    {
                        attr.type = core::UpDownStopContinue::up;
                        attr.size = core::PositiveInteger{ 15 };
                        break;
                    }
                    case api::OttavaType::o15mb:
                    {
                        attr.type = core::UpDownStopContinue::down;
                        attr.size = core::PositiveInteger{ 15 };
                        break;
                    }
                    case api::OttavaType::o8va:
                    {
                        attr.type = core::UpDownStopContinue::up;
                        attr.size = core::PositiveInteger{ 8 };
                        break;
                    }
                    case api::OttavaType::o8vb:
                    {
                        attr.type = core::UpDownStopContinue::down;
                        attr.size = core::PositiveInteger{ 8 };
                        break;
                    }
                    default: break;
                }
            }
            
            for( const auto& ottavaStop : myDirectionData.ottavaStops )
            {
                auto oStopDir = core::makeDirectionType();
                this->addDirectionType( oStopDir, directionPtr );
                oStopDir->setChoice( core::DirectionType::Choice::octaveShift );
                auto oStart = oStopDir->getOctaveShift();
                auto& attr = *oStart->getAttributes();
                attr.type = core::UpDownStopContinue::stop;
                MX_UNUSED( ottavaStop );
            }
            
            for( const auto& item : myDirectionData.bracketStarts )
            {
                auto outDirType = core::makeDirectionType();
                this->addDirectionType( outDirType, directionPtr );
                outDirType->setChoice( core::DirectionType::Choice::bracket );
                auto outElement = outDirType->getBracket();
                auto& attr = *outElement->getAttributes();
                setAttributesFromSpannerStart( item, attr );
            }

            for( const auto& tempo : myDirectionData.tempos )
            {
                if( tempo.tempoType != api::TempoType::beatsPerMinute )
                {
                    MX_THROW( "Only api::TempoType::beatsPerMinute is supported, others are not implemented" );
                }
                
                auto outDirType = core::makeDirectionType();
                this->addDirectionType( outDirType, directionPtr );
                outDirType->setChoice( core::DirectionType::Choice::metronome );
                auto outElement = outDirType->getMetronome();
                auto choice = outElement->getBeatUnitPerOrNoteRelationNoteChoice();
                choice->setChoice( core::BeatUnitPerOrNoteRelationNoteChoice::Choice::beatUnitPer );
                auto bpm = choice->getBeatUnitPer();
                auto beatUnitGroup = bpm->getBeatUnitGroup();
                auto beatUnit = beatUnitGroup->getBeatUnit();
                Converter converter;
                beatUnit->setValue(converter.convert(tempo.beatsPerMinute.durationName));
                
                for( int d = 0; d < tempo.beatsPerMinute.dots; ++d )
                {
                    beatUnitGroup->addBeatUnitDot(core::makeBeatUnitDot());
                }
                
                auto pmobuc = bpm->getPerMinuteOrBeatUnitChoice();
                pmobuc->setChoice( core::PerMinuteOrBeatUnitChoice::Choice::perMinute );
                auto pm = pmobuc->getPerMinute();
                auto str = std::to_string( tempo.beatsPerMinute.beatsPerMinute );
                pm->setValue( core::XsString{ str } );
                //auto& attr = *outElement->getAttributes();
                //setAttributesFromSpannerStart( item, attr );
            }

            if( myDirectionData.words.size() > 0 )
            {
                bool isFirstWordsAdded = false;
                auto outDirType = core::makeDirectionType();
                outDirType->setChoice( core::DirectionType::Choice::words );

                for( const auto& wordsData : myDirectionData.words )
                {
                    auto outWords = core::makeWords();
                    outDirType->addWords( outWords );

                    if (!isFirstWordsAdded)
                    {
                        isFirstWordsAdded = true;
                        outDirType->removeWords( outDirType->getWordsSet().cbegin() );
                    }

                    auto& attr = *(outWords->getAttributes());
                    outWords->setValue( core::XsString{ wordsData.text } );
                    setAttributesFromPositionData( wordsData.positionData, attr );
                    setAttributesFromFontData( wordsData.fontData, attr );

                    if( wordsData.isColorSpecified )
                    {
                        // TODO - oops color is missing from words
//                        attr.hasColor = true;
                        setAttributesFromColorData( wordsData.colorData, attr );
                    }
                }

                if( isFirstWordsAdded )
                {
                    addDirectionType( outDirType, directionPtr );
                }
            }

            if( myIsFirstDirectionTypeAdded )
            {
                auto directionMdc = core::makeMusicDataChoice();
                directionMdc->setChoice( core::MusicDataChoice::Choice::direction );
                directionMdc->setDirection( directionPtr );
                output.push_back( directionMdc );
            }

            auto harmonyMdcs = createHarmonyElements( offset );
            addMusicDataChoices(harmonyMdcs, output);

            // clear state
            myPlacements.clear();
            myIsFirstDirectionTypeAdded = false;
            return output;
        }
        
        
        void DirectionWriter::addDirectionType( const core::DirectionTypePtr& directionType, const core::DirectionPtr& ioDirection )
        {
            ioDirection->addDirectionType( directionType );

            if( !myIsFirstDirectionTypeAdded && ioDirection->getDirectionTypeSet().size() == 2 )
            {
                ioDirection->removeDirectionType( ioDirection->getDirectionTypeSet().cbegin() );
            }
            
            myIsFirstDirectionTypeAdded = true;
        }


        core::MusicDataChoiceSet DirectionWriter::createHarmonyElements( int inOffset )
        {
            if( myDirectionData.chords.empty() )
            {
                return core::MusicDataChoiceSet{};
            }

            core::MusicDataChoiceSet output;
            auto mdc = core::makeMusicDataChoice();
            mdc->setChoice( core::MusicDataChoice::Choice::harmony );

            auto harmony = mdc->getHarmony();

            if( inOffset != 0 )
            {
                harmony->setHasOffset( true );
                harmony->getOffset()->setValue( core::DivisionsValue{ static_cast<core::DecimalType>( inOffset ) } );
            }

            if( myDirectionData.isStaffValueSpecified )
            {
                harmony->setHasStaff( true );
                harmony->getStaff()->setValue( core::PositiveInteger{ myCursor.staffIndex + 1 } );
            }

            const auto& chords = myDirectionData.chords;

            auto chordIter = chords.cbegin();
            const auto chordEnd = chords.cend();

            bool isFirstChord = true;
            for( ; chordIter != chordEnd; ++chordIter )
            {
                if ( isFirstChord )
                {
                    isFirstChord = false;
                    auto& attributes = *harmony->getAttributes();
                    setAttributesFromPositionData( chordIter->positionData, attributes );
                }

                core::HarmonyChordGroupPtr grp = nullptr;
                if( chordIter == chords.cbegin() )
                {
                    grp = harmony->getHarmonyChordGroupSet().front();
                }
                else
                {
                    grp = core::makeHarmonyChordGroup();
                    harmony->addHarmonyChordGroup( grp );
                }

                grp->setChoice( core::HarmonyChordGroup::Choice::root );

                auto step = chordIter->root == api::Step::unspecified ? api::Step::c : chordIter->root;
                grp->getRoot()->getRootStep()->setValue( myConverter.convert( step ) );
                if( chordIter->rootAlter != 0 )
                {
                    grp->getRoot()->setHasRootAlter( true );
                    grp->getRoot()->getRootAlter()->setValue( core::Semitones{ static_cast<core::DecimalType>( chordIter->rootAlter ) } );
                }

                if( chordIter->bass != api::Step::unspecified )
                {
                    grp->setHasBass( true );
                    auto bass = grp->getBass();
                    bass->getBassStep()->setValue( myConverter.convert( chordIter->bass ) );
                    if( chordIter->bassAlter != 0 )
                    {
                        bass->setHasBassAlter( true );
                        bass->getBassAlter()->setValue( core::Semitones{ static_cast<core::DecimalType>( chordIter->bassAlter ) } );
                    }
                }

                const auto k = myConverter.convert( chordIter->chordKind );
                grp->getKind()->setValue( k );

                if( !chordIter->text.empty() )
                {
                    auto a = grp->getKind()->getAttributes();
                    a->hasText = true;
                    a->text = core::XsToken{ chordIter->text };
                }

                if( chordIter->useSymbols != api::Bool::unspecified )
                {
                    grp->getKind()->getAttributes()->hasUseSymbols = true;
                    grp->getKind()->getAttributes()->useSymbols = chordIter->useSymbols == api::Bool::yes ? mx::core::YesNo::yes : mx::core::YesNo::no;
                }

                for( const auto& extension : chordIter->extensions )
                {
                    auto degree = core::makeDegree();

                    if( extension.extensionType == api::ExtensionType::add )
                    {
                        degree->getDegreeType()->setValue( core::DegreeTypeValue::add );
                    }
                    else if( extension.extensionType == api::ExtensionType::remove )
                    {
                        degree->getDegreeType()->setValue( core::DegreeTypeValue::subtract );
                    }
                    else if( extension.extensionType == api::ExtensionType::alter )
                    {
                        degree->getDegreeType()->setValue( core::DegreeTypeValue::alter );
                    }

                    switch ( extension.extensionNumber )
                    {
                        case api::ExtensionNumber::first: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 1 } ); break;
                        case api::ExtensionNumber::second: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 2 } ); break;
                        case api::ExtensionNumber::third: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 3 } ); break;
                        case api::ExtensionNumber::fourth: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 4 } ); break;
                        case api::ExtensionNumber::fifth: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 5 } ); break;
                        case api::ExtensionNumber::sixth: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 6 } ); break;
                        case api::ExtensionNumber::seventh: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 7 } ); break;
                        case api::ExtensionNumber::ninth: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 9 } ); break;
                        case api::ExtensionNumber::eleventh: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 11 } ); break;
                        case api::ExtensionNumber::thirteenth: degree->getDegreeValue()->setValue( mx::core::PositiveInteger{ 13 } ); break;
                        default: break;
                    }

                    switch( extension.extensionAlter )
                    {
                        case api::ExtensionAlter::doubleFlat: degree->getDegreeAlter()->setValue( core::Semitones{ -2.0 } ); break;
                        case api::ExtensionAlter::doubleSharp: degree->getDegreeAlter()->setValue( core::Semitones{ 2.0 } ); break;
                        case api::ExtensionAlter::flat: degree->getDegreeAlter()->setValue( core::Semitones{ -1.0 } ); break;
                        case api::ExtensionAlter::sharp: degree->getDegreeAlter()->setValue( core::Semitones{ 1.0 } ); break;
                        case api::ExtensionAlter::none: degree->getDegreeAlter()->setValue( core::Semitones{ 0.0 } ); break;
                    }

                    grp->addDegree( degree );
                }

                auto miscIter = chordIter->miscData.cbegin();
                const auto miscEnd = chordIter->miscData.cend();
                for( ; miscIter != miscEnd; ++miscIter ) {
                    harmony->addProcessingInstruction(core::ProcessingInstruction{miscIter->name, miscIter->data});
                }

                if( output.empty() )
                {
                    output.push_back( mdc );
                }
            }

            return output;
        }


        void DirectionWriter::addMusicDataChoices( const core::MusicDataChoiceSet& inMdcs, core::MusicDataChoiceSet& ioOutputSet )
        {
            for( const auto& mdc : inMdcs )
            {
                ioOutputSet.push_back( mdc );
            }
        }
    }
}
