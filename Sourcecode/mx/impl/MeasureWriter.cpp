// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/MeasureWriter.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/impl/Converter.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/Directive.h"
#include "mx/core/elements/Divisions.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Instruments.h"
#include "mx/core/elements/Key.h"
#include "mx/core/elements/KeyChoice.h"
#include "mx/core/elements/TraditionalKey.h"
#include "mx/core/elements/Cancel.h"
#include "mx/core/elements/Fifths.h"
#include "mx/core/elements/Mode.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/MeasureStyle.h"
#include "mx/core/elements/PartSymbol.h"
#include "mx/core/elements/StaffDetails.h"
#include "mx/core/elements/Staves.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/Transpose.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/ClefOctaveChange.h"
#include "mx/core/elements/Line.h"
#include "mx/core/elements/Sign.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/core/elements/Print.h"
#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/elements/MeasureLayout.h"
#include "mx/core/elements/MeasureNumbering.h"
#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/elements/SystemLayout.h"
#include "mx/core/elements/SystemDistance.h"
#include "mx/core/elements/SystemDividers.h"
#include "mx/core/elements/SystemMargins.h"
#include "mx/core/elements/TopSystemDistance.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Accidental.h"
#include "mx/core/elements/Beam.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/Dot.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/Grace.h"
#include "mx/core/elements/Cue.h"
#include "mx/core/elements/Instrument.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Lyric.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Notehead.h"
#include "mx/core/elements/NoteheadText.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Play.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Stem.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Tie.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/impl/NoteWriter.h"

namespace mx
{
    namespace core
    {
        class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
    }

    namespace impl
    {
        MeasureWriter::MeasureWriter( const api::MeasureData& inMeasureData, const MeasureCursor& inCursor, const ScoreWriter& inScoreWriter )
        : myMeasureData{ inMeasureData }
        , myOutMeasure{ nullptr }
        , myCursor{ inCursor }
        , myPreviousCursor{ inCursor }
        , myScoreWriter{ inScoreWriter }
        {

        }


        core::PartwiseMeasurePtr MeasureWriter::getPartwiseMeasure() const
        {
            myOutMeasure = core::makePartwiseMeasure();
            myCursor.reset();
            
            auto& measureAttr = *myOutMeasure->getAttributes();
            
            if( myMeasureData.number.size() > 0 )
            {
                measureAttr.number = core::XsToken{ myMeasureData.number };
            }
            else
            {
                measureAttr.number = core::XsToken{ std::to_string( myCursor.measureIndex + 1 ) };
            }
            
            if( myMeasureData.width >= 0.0 )
            {
                measureAttr.hasWidth = true;
                measureAttr.width = core::PositiveDivisionsValue{ myMeasureData.width };
            }
            
            Converter converter;
            
            if( myMeasureData.implicit != api::Bool::unspecified )
            {
                measureAttr.hasImplicit = true;
                measureAttr.implicit = converter.convert( myMeasureData.implicit );
            }
            
            if( myCursor.isFirstMeasureInPart )
            {
                if( myScoreWriter.isStartOfSystem( myCursor.measureIndex ) )
                {
                    writeSystemInfo();
                }
                writeFirstMeasureProperties();
            }
            
            writeStaffData();
            return myOutMeasure;
        }

        
        void MeasureWriter::writeSystemInfo() const
        {
            auto systemData = myScoreWriter.getSystemData(myCursor.measureIndex );
            auto printMdc = core::makeMusicDataChoice();
            printMdc->setChoice( core::MusicDataChoice::Choice::print );
            auto& print = *printMdc->getPrint();
            print.getAttributes()->hasNewSystem = true;
            print.getAttributes()->newSystem = core::YesNo::yes;
            auto& layoutGroup = *print.getLayoutGroup();
            myOutMeasure->getMusicDataGroup()->addMusicDataChoice(printMdc );
            if( systemData.isMarginSpecified || systemData.isSystemDistanceSpecified || systemData.isTopSystemDistanceSpecified )
            {
                layoutGroup.setHasSystemLayout( true );
                auto& systemLayout = *layoutGroup.getSystemLayout();

                if( systemData.isMarginSpecified )
                {
                    systemLayout.setHasSystemMargins( true );
                    auto& margins = *systemLayout.getSystemMargins();
                    margins.getLeftMargin()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.leftMargin ) } );
                    margins.getRightMargin()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.rightMargin ) } );
                }

                if( systemData.isTopSystemDistanceSpecified )
                {
                    systemLayout.setHasTopSystemDistance( true );
                    systemLayout.getTopSystemDistance()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.topSystemDistance ) } );
                }

                if( systemData.isSystemDistanceSpecified )
                {
                    systemLayout.setHasSystemDistance( true );
                    systemLayout.getSystemDistance()->setValue( core::TenthsValue{static_cast<core::DecimalType>( systemData.systemDistance ) } );
                }
           }
        }
        
        
        void MeasureWriter::writeFirstMeasureProperties() const
        {
            auto& props = createAndInsertMeasureProperties();
            writeDivisions( props );
            writeInitialClefs( props );
            writeTime( props );
            writeNumStaves( props );
        }
        
        
        core::Properties& MeasureWriter::createAndInsertMeasureProperties() const
        {
            auto mdc = core::makeMusicDataChoice();
            myOutMeasure->getMusicDataGroup()->addMusicDataChoice( mdc );
            mdc->setChoice( core::MusicDataChoice::Choice::properties );
            return *mdc->getProperties();
        }
        
        
        void MeasureWriter::writeDivisions( core::Properties& outProperties ) const
        {
            outProperties.setHasDivisions( true );
            outProperties.getDivisions()->setValue( core::PositiveDecimal{ static_cast<long double>( myCursor.ticksPerQuarter ) } );
        }
        
        
        void MeasureWriter::writeKey(int staffIndex, const api::KeyData& inKeyData, core::Properties& outProperties ) const
        {
            // TODO - support non-traditional keys
            // TODO - support placement and other attributes

            auto key = core::makeKey();
            
            if( staffIndex >= 0 )
            {
                key->getAttributes()->hasNumber = true;
                key->getAttributes()->number = core::StaffNumber{ staffIndex + 1 };
            }
            
            key->getKeyChoice()->setChoice( core::KeyChoice::Choice::traditionalKey );
            auto traditionalKey = key->getKeyChoice()->getTraditionalKey();
            traditionalKey->getFifths()->setValue( core::FifthsValue{ inKeyData.fifths } );
            
            if ( inKeyData.cancel != 0 )
            {
                traditionalKey->setHasCancel( true );
                traditionalKey->getCancel()->setValue( core::FifthsValue{ inKeyData.cancel } );
            }
            
            if( inKeyData.mode == api::KeyMode::major || inKeyData.mode == api::KeyMode::minor )
            {
                traditionalKey->setHasMode( true );
                traditionalKey->getMode()->setValue( core::ModeValue{ inKeyData.mode == api::KeyMode::major ? core::ModeEnum::major : core::ModeEnum::minor } );
            }
            outProperties.addKey( key );
        }
        
        
        void MeasureWriter::writeTime( core::Properties& outProperties ) const
        {
            const auto& timeData = myMeasureData.timeSignature;
//            // TODO - find out why the first measure is coming up with an 'implicit' time-signature
//            if( timeData.isImplicit )
//            {
//                return;
//            }
            
            auto time = core::makeTime();
            outProperties.addTime( time );
            time->getTimeChoice()->setChoice( core::TimeChoice::Choice::timeSignature );
            auto sigGrp = time->getTimeChoice()->getTimeSignatureGroupSet().front();
            sigGrp->getBeats()->setValue( core::XsString{ std::to_string( timeData.beats ) } );
            sigGrp->getBeatType()->setValue( core::XsString{ std::to_string( timeData.beatType ) } );
            
            const auto symbol = myMeasureData.timeSignature.symbol;
            if( symbol != api::TimeSignatureSymbol::unspecified )
            {
                time->getAttributes()->hasSymbol = true;
                // TODO - cut time and common time
                // time->getAttributes()->symbol = converter.convert( symbol );
            }
            
            Converter converter;
            if( myMeasureData.timeSignature.display != api::Bool::unspecified )
            {
                time->getAttributes()->hasPrintObject = true;
                time->getAttributes()->printObject = converter.convert( myMeasureData.timeSignature.display );
            }
        }
        
        
        void MeasureWriter::writeNumStaves( core::Properties& outProperties ) const
        {
            outProperties.setHasStaves( true );
            outProperties.getStaves()->setValue( core::NonNegativeInteger{ myCursor.getNumStaves() } );
        }
        
        
        void MeasureWriter::writeClef( int staffIndex, const api::ClefData& inClefData, core::Properties& outProperties ) const
        {
            auto mxClef = core::makeClef();
            
            if( staffIndex >= 0 )
            {
                mxClef->getAttributes()->hasNumber = true;
                mxClef->getAttributes()->number = core::StaffNumber{ staffIndex + 1 };
            }
            
            Converter converter;
            mxClef->getSign()->setValue( converter.convert( inClefData.symbol ) );
            
            if( inClefData.line >= 0 )
            {
                mxClef->setHasLine( true );
                mxClef->getLine()->setValue( core::StaffLine{ inClefData.line } );
            }
            
            if( inClefData.octaveChange != 0 )
            {
                mxClef->setHasClefOctaveChange( true );
                mxClef->getClefOctaveChange()->setValue( core::Integer{ inClefData.octaveChange } );
            }
            outProperties.addClef( mxClef );
        }
        
        
        void MeasureWriter::writeInitialClefs( core::Properties& outProperties ) const
        {
            int staffIndex = 0;
            for( const auto& staff : myMeasureData.staves )
            {
                auto clefsAtZero = findItemsAtTimePosition( staff.clefs, 0 );
                if( clefsAtZero.size() == 1 )
                {
                    writeClef( staffIndex, clefsAtZero.front(), outProperties );
                }
                ++staffIndex;
            }
        }
        
        
        void MeasureWriter::writeInitialKeys( core::Properties& outProperties ) const
        {
            // TODO - this is wrong - properly parse the keys
            // and place them per staff in case they are different
            // in a multi-staff part
            if( myMeasureData.keys.size() > 0 )
            {
                const auto& keyData = myMeasureData.keys.front();
                writeKey( -1, keyData, outProperties );
            }
            else
            {
                writeKey( -1, api::KeyData{}, outProperties );
            }
        }
        
        
        void MeasureWriter::writeStaffData() const
        {
            myCursor.tickTimePosition = 0;
            myCursor.staffIndex = 0;
            myCursor.voiceIndex = 0;
            myPreviousCursor = myCursor;
            for( const auto& staff : myMeasureData.staves )
            {
                // auto clefIter = staff.clefs.cbegin();
                // auto clefEnd = staff.clefs.cend();
                
                writeVoiceData( staff );
                ++myCursor.staffIndex;
            }
        }
        
        void MeasureWriter::writeVoiceData( const api::StaffData& inStaff ) const
        {
            for( const auto& voice : inStaff.voices )
            {
                myCursor.voiceIndex = voice.first;
                for( const auto& apiNote : voice.second.notes )
                {
//                    auto mxNote = core::makeNote();
//                    mxNote->setHasType( true );
//                    mxNote->getType()->setValue( converter.convert( apiNote.durationData.durationName ) );
//                    for( int d = 0; d < apiNote.durationData.durationDots; ++ d )
//                    {
//                        mxNote->addDot( core::makeDot() );
//                    }
//                    if( apiNote.pitchData.accidental != api::Accidental::none )
//                    {
//                        mxNote->setHasAccidental( true );
//                        mxNote->getAccidental()->setValue( converter.convert( apiNote.pitchData.accidental ) );
//                    }
//                    if( apiNote.stem != api::Stem::unspecified )
//                    {
//                        mxNote->setHasStem( true );
//                        mxNote->getStem()->setValue( converter.convert( apiNote.stem ) );
//                    }
//                    auto nc = mxNote->getNoteChoice();
//                    nc->setChoice( core::NoteChoice::Choice::normal );
//                    auto nng = nc->getNormalNoteGroup();
                    
                    auto mdc = core::makeMusicDataChoice();
                    mdc->setChoice( core::MusicDataChoice::Choice::note );
                    NoteWriter writer{ apiNote, myCursor, myScoreWriter };
                    mdc->setNote( writer.getNote() );
                    myOutMeasure->getMusicDataGroup()->addMusicDataChoice( mdc );
                }
            }
        }
    }
}
