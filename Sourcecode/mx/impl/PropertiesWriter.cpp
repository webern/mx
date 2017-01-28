// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PropertiesWriter.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/Directive.h"
#include "mx/core/elements/Divisions.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Instruments.h"
#include "mx/core/elements/Key.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/MeasureStyle.h"
#include "mx/core/elements/PartSymbol.h"
#include "mx/core/elements/StaffDetails.h"
#include "mx/core/elements/Staves.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/Transpose.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/impl/Converter.h"
#include "mx/core/elements/Backup.h"
#include "mx/core/elements/Forward.h"
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
	namespace impl
	{
        PropertiesWriter::PropertiesWriter( const core::PartwiseMeasurePtr& inPartwiseMeasure )
        : myProperties{ nullptr }
        , myPartwiseMeasure{ inPartwiseMeasure }
        {
            allocate();
            MX_ASSERT( myPartwiseMeasure != nullptr );
        }

        
        void PropertiesWriter::clearBuffer()
        {
            allocate();
        }
        
        
        void PropertiesWriter::flushBuffer()
        {
            if( !isPropertiesEmpty() )
            {
                auto mdc = core::makeMusicDataChoice();
                mdc->setChoice( core::MusicDataChoice::Choice::properties );
                mdc->setProperties( myProperties );
                myPartwiseMeasure->getMusicDataGroup()->addMusicDataChoice( mdc );
                allocate();
            }
        }
        
        
        bool PropertiesWriter::isPropertiesEmpty()
        {
            if( !myProperties )
            {
                allocate();
                return false;
            }
            const bool hasContents = myProperties->hasContents();
            return !hasContents;
        }
        
        
        void PropertiesWriter::writeDivisions( int value )
        {
            myProperties->setHasDivisions( true );
            myProperties->getDivisions()->setValue( core::PositiveDecimal{ static_cast<long double>( value ) } );
        }
    
        
        void PropertiesWriter::writeKey( int staffIndex, const api::KeyData& inKeyData )
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
            myProperties->addKey( key );
        }
        
        
        void PropertiesWriter::writeTime( const api::TimeSignatureData& value )
        {
            auto time = core::makeTime();
            myProperties->addTime( time );
            time->getTimeChoice()->setChoice( core::TimeChoice::Choice::timeSignature );
            auto sigGrp = time->getTimeChoice()->getTimeSignatureGroupSet().front();
            sigGrp->getBeats()->setValue( core::XsString{ std::to_string( value.beats ) } );
            sigGrp->getBeatType()->setValue( core::XsString{ std::to_string( value.beatType ) } );
            
            const auto symbol = value.symbol;
            if( symbol != api::TimeSignatureSymbol::unspecified )
            {
                time->getAttributes()->hasSymbol = true;
                if( symbol == api::TimeSignatureSymbol::common )
                {
                    time->getAttributes()->symbol = core::TimeSymbol::common;
                }
                else if( symbol == api::TimeSignatureSymbol::cut )
                {
                    time->getAttributes()->symbol = core::TimeSymbol::cut;
                }
            }
            
            Converter converter;
            if( value.display != api::Bool::unspecified )
            {
                time->getAttributes()->hasPrintObject = true;
                time->getAttributes()->printObject = converter.convert( value.display );
            }
        }
        
        
        void PropertiesWriter::writeNumStaves( int value )
        {
            myProperties->setHasStaves( true );
            myProperties->getStaves()->setValue( core::NonNegativeInteger{ value } );
        }
        
        
        void PropertiesWriter::writeClef( int staffIndex, const api::ClefData& inClefData )
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
            myProperties->addClef( mxClef );
        }
        
        
        void PropertiesWriter::allocate()
        {
            myProperties = core::makeProperties();
        }
	}
}
