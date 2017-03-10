// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PartWriter.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DisplayText.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include "mx/core/elements/Elevation.h"
#include "mx/core/elements/Ensemble.h"
#include "mx/core/elements/Forward.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/InstrumentAbbreviation.h"
#include "mx/core/elements/InstrumentName.h"
#include "mx/core/elements/InstrumentSound.h"
#include "mx/core/elements/MidiBank.h"
#include "mx/core/elements/MidiChannel.h"
#include "mx/core/elements/MidiDevice.h"
#include "mx/core/elements/MidiDeviceInstrumentGroup.h"
#include "mx/core/elements/MidiInstrument.h"
#include "mx/core/elements/MidiName.h"
#include "mx/core/elements/MidiProgram.h"
#include "mx/core/elements/MidiUnpitched.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Pan.h"
#include "mx/core/elements/PartAbbreviation.h"
#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/elements/PartName.h"
#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/ScoreInstrument.h"
#include "mx/core/elements/ScorePart.h"
#include "mx/core/elements/Solo.h"
#include "mx/core/elements/SoloOrEnsembleChoice.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Staves.h"
#include "mx/core/elements/VirtualInstrument.h"
#include "mx/core/elements/VirtualLibrary.h"
#include "mx/core/elements/VirtualName.h"
#include "mx/core/elements/Volume.h"
#include "mx/impl/MeasureWriter.h"
#include "mx/impl/MeasureCursor.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/api/DocumentManager.h"

#include <sstream>

namespace mx
{
    namespace impl
    {
        PartWriter::PartWriter( const api::PartData& inPartData, int inPartIndex, int inTicksPerQuarter, const ScoreWriter& inScoreWriter )
        : myPartData{ inPartData }
        , myPartIndex{ inPartIndex }
        , myTicksPerQuarter{ inTicksPerQuarter }
        , myMutex{}
        , myOutScorePart{ nullptr }
        , myOutPartwisePart{ nullptr }
        , myScoreWriter{ inScoreWriter }
        {

        }
        

        core::ScorePartPtr PartWriter::getScorePart() const
        {
            myOutScorePart = core::makeScorePart();
            auto& attr = *myOutScorePart->getAttributes();
            attr.id = core::XsID{ myPartData.uniqueId };
            myOutScorePart->getPartName()->setValue( core::XsString{ myPartData.name } );
            myOutScorePart->getPartName()->getAttributes()->hasPrintObject = true;
            myOutScorePart->getPartName()->getAttributes()->printObject = core::YesNo::no;
            
            if( myPartData.abbreviation.size() > 0 )
            {
                myOutScorePart->setHasPartAbbreviation( true );
                myOutScorePart->getPartAbbreviation()->setValue( core::XsString{ myPartData.abbreviation } );
                myOutScorePart->getPartAbbreviation()->getAttributes()->hasPrintObject = true;
                myOutScorePart->getPartAbbreviation()->getAttributes()->printObject = core::YesNo::no;
            }
            
            if( myPartData.displayName.size() > 0 )
            {
                myOutScorePart->setHasPartNameDisplay( true );
                auto dtoat = core::makeDisplayTextOrAccidentalText();
                dtoat->setChoice( core::DisplayTextOrAccidentalText::Choice::displayText );
                dtoat->getDisplayText()->setValue( core::XsString{ myPartData.displayName } );
                myOutScorePart->getPartNameDisplay()->addDisplayTextOrAccidentalText( dtoat );
            }
            if( myPartData.displayAbbreviation.size() > 0 )
            {
                myOutScorePart->setHasPartAbbreviationDisplay( true );
                auto dtoat = core::makeDisplayTextOrAccidentalText();
                dtoat->setChoice( core::DisplayTextOrAccidentalText::Choice::displayText );
                dtoat->getDisplayText()->setValue( core::XsString{ myPartData.displayAbbreviation } );
                myOutScorePart->getPartAbbreviationDisplay()->addDisplayTextOrAccidentalText( dtoat );
            }
            auto scoreIntstrument = core::makeScoreInstrument();
            bool addScoreInstrument = false;
            scoreIntstrument->getAttributes()->id = core::XsID{ myPartData.instrumentData.uniqueId };
            if( myPartData.instrumentData.name.size() > 0 )
            {
                addScoreInstrument = true;
                scoreIntstrument->getInstrumentName()->setValue( core::XsString{ myPartData.instrumentData.name } );
            }
            if( myPartData.instrumentData.abbreviation.size() > 0 )
            {
                addScoreInstrument = true;
                scoreIntstrument->setHasInstrumentAbbreviation( true );
                scoreIntstrument->getInstrumentAbbreviation()->setValue( core::XsString{ myPartData.instrumentData.abbreviation } );
            }
            if( myPartData.instrumentData.soloOrEnsemble != api::SoloOrEnsemble::unspecified )
            {
                addScoreInstrument = true;
                scoreIntstrument->setHasSoloOrEnsembleChoice( true );
                const auto value = myPartData.instrumentData.soloOrEnsemble == api::SoloOrEnsemble::ensemble ?
                    core::SoloOrEnsembleChoice::Choice::ensemble : core::SoloOrEnsembleChoice::Choice::solo;
                scoreIntstrument->getSoloOrEnsembleChoice()->setChoice( value );
            }
            
            if( myPartData.instrumentData.midiData.virtualName.size() > 0 || myPartData.instrumentData.midiData.virtualLibrary.size() > 0 )
            {
                addScoreInstrument = true;
                scoreIntstrument->setHasVirtualInstrument( true );
                auto virtualInstrument = scoreIntstrument->getVirtualInstrument();
                
                if( myPartData.instrumentData.midiData.virtualName.size() > 0 )
                {
                    virtualInstrument->setHasVirtualName( true );
                    virtualInstrument->getVirtualName()->setValue( core::XsString{ myPartData.instrumentData.midiData.virtualName } );
                }
                
                if( myPartData.instrumentData.midiData.virtualLibrary.size() > 0 )
                {
                    virtualInstrument->setHasVirtualLibrary( true );
                    virtualInstrument->getVirtualLibrary()->setValue( core::XsString{ myPartData.instrumentData.midiData.virtualLibrary } );
                }
            }
            
            if( myPartData.instrumentData.sound.size() > 0 )
            {
                scoreIntstrument->setHasInstrumentSound( true );
                scoreIntstrument->getInstrumentSound()->setValue( core::XsString{ myPartData.instrumentData.sound } );
            }
            
            if( addScoreInstrument )
            {
                myOutScorePart->addScoreInstrument( scoreIntstrument );
            }
            
            bool addMidiElement = false;
            auto midiGroup = core::makeMidiDeviceInstrumentGroup();
            auto& midiDevice = *midiGroup->getMidiDevice();
            auto& midiInstrument = *midiGroup->getMidiInstrument();
            midiInstrument.getAttributes()->id = core::XsID{ myPartData.instrumentData.uniqueId };
            
            if( myPartData.instrumentData.midiData.device.size() > 0 )
            {
                addMidiElement = true;
                midiGroup->setHasMidiDevice( true );
                midiDevice.setValue( core::XsString{ myPartData.instrumentData.midiData.device } );
            }
            if( myPartData.instrumentData.midiData.bank >= 0 )
            {
                addMidiElement = true;
                midiGroup->setHasMidiInstrument( true );
                midiInstrument.setHasMidiBank( true );
                midiInstrument.getMidiBank()->setValue( core::Midi16384{ myPartData.instrumentData.midiData.bank } );
            }
            if( myPartData.instrumentData.midiData.channel >= 0 )
            {
                addMidiElement = true;
                midiGroup->setHasMidiInstrument( true );
                midiInstrument.setHasMidiChannel( true );
                midiInstrument.getMidiChannel()->setValue( core::Midi16{ myPartData.instrumentData.midiData.channel } );
            }
            if( myPartData.instrumentData.midiData.program >= 0 )
            {
                addMidiElement = true;
                midiGroup->setHasMidiInstrument( true );
                midiInstrument.setHasMidiProgram( true );
                midiInstrument.getMidiProgram()->setValue( core::Midi128{ myPartData.instrumentData.midiData.program } );
            }
            if( myPartData.instrumentData.midiData.isElevationSpecified )
            {
                addMidiElement = true;
                midiGroup->setHasMidiInstrument( true );
                midiInstrument.setHasElevation( true );
                midiInstrument.getElevation()->setValue( core::RotationDegrees{ myPartData.instrumentData.midiData.elevation } );
            }
            if( myPartData.instrumentData.midiData.isPanSpecified )
            {
                addMidiElement = true;
                midiGroup->setHasMidiInstrument( true );
                midiInstrument.setHasPan( true );
                midiInstrument.getPan()->setValue( core::RotationDegrees{ myPartData.instrumentData.midiData.pan } );
            }
            if( myPartData.instrumentData.midiData.isVolumeSpecified )
            {
                addMidiElement = true;
                midiGroup->setHasMidiInstrument( true );
                midiInstrument.setHasVolume( true );
                midiInstrument.getVolume()->setValue( core::Percent{ myPartData.instrumentData.midiData.volume } );
            }
            if( addMidiElement )
            {
                myOutScorePart->addMidiDeviceInstrumentGroup( midiGroup );
            }
            if( addMidiElement && !addScoreInstrument )
            {
                if( myPartData.instrumentData.uniqueId.size() == 0 )
                {
                    std::stringstream ss;
                    ss << "ID";
                    ss << api::DocumentManager::getInstance().getUniqueId();
                    scoreIntstrument->getAttributes()->id = core::XsID{ ss.str() };
                }
                else
                {
                    scoreIntstrument->getAttributes()->id = core::XsID{ myPartData.instrumentData.uniqueId };
                }

                myOutScorePart->addScoreInstrument( scoreIntstrument );
            }
            return myOutScorePart;
        }


        core::PartwisePartPtr PartWriter::getPartwisePart() const
        {
            myOutPartwisePart = core::makePartwisePart();
            auto& attr = *myOutPartwisePart->getAttributes();
            attr.id = core::XsID{ myPartData.uniqueId };
            writeMeasures();
            return myOutPartwisePart;
        }
        
        void PartWriter::writeMeasures() const
        {
            if( myPartData.measures.size() == 0 )
            {
                return;
            }
            
            MeasureCursor cursor{ static_cast<int>( myPartData.measures.at( 0 ).staves.size() ), myTicksPerQuarter };
            cursor.measureIndex = 0;
            cursor.partIndex = myPartIndex;
            cursor.isFirstMeasureInPart = true;
            
            for( const auto& measure : myPartData.measures )
            {
                MeasureWriter writer{ measure, cursor, myScoreWriter };
                myOutPartwisePart->addPartwiseMeasure( writer.getPartwiseMeasure() );
                
                auto& partwiseMeasureSet = myOutPartwisePart->getPartwiseMeasureSet();
                
                if( cursor.measureIndex == 0 && partwiseMeasureSet.size() == 2 )
                {
                    myOutPartwisePart->removePartwiseMeasure( partwiseMeasureSet.cbegin() );
                }
                
                cursor.isFirstMeasureInPart = false;
                ++cursor.measureIndex;
            }
        }
    }
}
