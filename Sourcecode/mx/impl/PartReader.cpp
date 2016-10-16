// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/PartReader.h"
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
#include "mx/impl/MeasureReader.h"
#include "mx/impl/PrintFunctions.h"

#include <sstream>

namespace mx
{
    namespace impl
    {
        PartReader::PartReader( const core::ScorePart& inScorePart, const core::PartwisePart& inPartwisePartRef, int globalTicksPerMeasure )
        : myPartwisePart{ inPartwisePartRef }
        , myScorePart{ inScorePart }
        , myNumStaves{ -1 }
        , myGlobalTicksPerMeasure{ globalTicksPerMeasure }
        {
            const auto ppId = myPartwisePart.getAttributes()->id.getValue();
            const auto spId = myScorePart.getAttributes()->id.getValue();
            if( ppId != spId )
            {
                MX_THROW( "the partwise-part id must match the score-part id" );
            }
            myNumStaves = calculateNumStaves();
        }
        
        
        api::PartData PartReader::getPartData() const
        {
            std::lock_guard<std::mutex> lock{ myMutex };
            myOutPartData = api::PartData{};
            parseScorePart();
            
            bool isFirstMeasure = true;

            // TODO - if we need to use isFirstPart it
            // will need to be passed down from the caller
            bool isFirstPart = false;
            
            int measureIndex = 0;
            for( const auto& mxMeasurePtr : myPartwisePart.getPartwiseMeasureSet() )
            {
                const auto& mxMeasure = *mxMeasurePtr;
                MeasureReaderParameters params;
                params.measureIndex = measureIndex;
                params.globalTicksPerQuarter = myGlobalTicksPerMeasure;
                params.numStaves = myNumStaves;
                params.isFirstPart = isFirstPart;
                params.isFirstMeasure = isFirstMeasure;
                MeasureReader reader{ mxMeasure, params };
                myOutPartData.measures.emplace_back( reader.getMeasureData() );
                isFirstMeasure = false;
                ++measureIndex;
            }
            api::PartData tempReturn = std::move( myOutPartData );
            myOutPartData = api::PartData{};
            return tempReturn;
        }
        
        int PartReader::calculateNumStaves() const
        {
            int numStaves = 1;
            
            for( const auto& measure : myPartwisePart.getPartwiseMeasureSet() )
            {
                for( const auto& mdc : measure->getMusicDataGroup()->getMusicDataChoiceSet() )
                {
                    const auto choiceType = mdc->getChoice();
                    
                    switch ( choiceType )
                    {
                        case core::MusicDataChoice::Choice::note:
                        {
                            updateNumStaves( *mdc->getNote(), numStaves );
                            break;
                        }

                        case core::MusicDataChoice::Choice::forward:
                        {
                            updateNumStaves( *mdc->getForward(), numStaves );
                            break;
                        }
                        case core::MusicDataChoice::Choice::direction:
                        {
                            updateNumStaves( *mdc->getDirection(), numStaves );
                            break;
                        }
                        case core::MusicDataChoice::Choice::properties:
                        {
                            const auto& properties = *mdc->getProperties();
                            if( properties.getHasStaves() )
                            {
                                int temp = properties.getStaves()->getValue().getValue();
                                if( temp > numStaves )
                                {
                                    numStaves = temp;
                                }
                            }
                            break;
                        }
                        case core::MusicDataChoice::Choice::harmony:
                        {
                            updateNumStaves( *mdc->getHarmony(), numStaves );
                            break;
                        }
                        // these are not specific to a staff number
                        case core::MusicDataChoice::Choice::backup:
                        case core::MusicDataChoice::Choice::figuredBass:
                        case core::MusicDataChoice::Choice::print:
                        case core::MusicDataChoice::Choice::sound:
                        case core::MusicDataChoice::Choice::barline:
                        case core::MusicDataChoice::Choice::grouping:
                        case core::MusicDataChoice::Choice::link:
                        case core::MusicDataChoice::Choice::bookmark:
                        default: break;
                    }
                }
            }
            
            return numStaves;
        }
        
        
        void PartReader::parseScorePart() const
        {
            myOutPartData.uniqueId = myScorePart.getAttributes()->id.getValue();
            myOutPartData.name = myScorePart.getPartName()->getValue().getValue();
            const auto& partNameAttributes = *myScorePart.getPartName()->getAttributes();
            myOutPartData.namePrintData = getPrintData( partNameAttributes );
            
            if( myScorePart.getHasPartNameDisplay() )
            {
                myOutPartData.displayName = extractDisplayText( myScorePart.getPartNameDisplay()->getDisplayTextOrAccidentalTextSet() );
            }
            
            if( myScorePart.getHasPartAbbreviation() )
            {
                myOutPartData.abbreviation = myScorePart.getPartAbbreviation()->getValue().getValue();
            }
            
            if( myScorePart.getHasPartAbbreviationDisplay() )
            {
                myOutPartData.displayAbbreviation = extractDisplayText( myScorePart.getPartNameDisplay()->getDisplayTextOrAccidentalTextSet() );
            }
            
            if( myScorePart.getScoreInstrumentSet().size() > 0 )
            {
                parseScoreInstrument( **myScorePart.getScoreInstrumentSet().cbegin() );
            }
            
            if( myScorePart.getMidiDeviceInstrumentGroupSet().size() > 0 )
            {
                parseMidiDeviceInstrumentGroup( **myScorePart.getMidiDeviceInstrumentGroupSet().cbegin() );
            }
        }
        
        
        std::string PartReader::extractDisplayText( const core::DisplayTextOrAccidentalTextSet& items ) const
        {
            std::stringstream ss;
            for( const auto& c : items )
            {
                if( c->getChoice() == core::DisplayTextOrAccidentalText::Choice::displayText )
                {
                    ss << c->getDisplayText()->getValue().getValue();
                }
                else if( c->getChoice() == core::DisplayTextOrAccidentalText::Choice::accidentalText )
                {
                    if( c->getAccidentalText()->getValue() == core::AccidentalValue::flat )
                    {
                        ss << "b"; // TODO - support accidental text correctly
                    }
                    else if( c->getAccidentalText()->getValue() == core::AccidentalValue::flat )
                    {
                        ss << "#";
                    }
                }
            }
            return ss.str();
        }
        
        
        void PartReader::parseScoreInstrument( const core::ScoreInstrument& scoreInstrument ) const
        {
            myOutPartData.instrumentData.uniqueId = scoreInstrument.getAttributes()->id.getValue();
            myOutPartData.instrumentData.name = scoreInstrument.getInstrumentName()->getValue().getValue();
            
            if( scoreInstrument.getHasInstrumentAbbreviation() )
            {
                myOutPartData.instrumentData.abbreviation = scoreInstrument.getInstrumentAbbreviation()->getValue().getValue();
            }
            
            if( scoreInstrument.getHasSoloOrEnsembleChoice() )
            {
                const auto& soec = *scoreInstrument.getSoloOrEnsembleChoice();
                if( soec.getChoice() == core::SoloOrEnsembleChoice::Choice::ensemble )
                {
                    myOutPartData.instrumentData.soloOrEnsemble = api::SoloOrEnsemble::ensemble;
                }
                else if( soec.getChoice() == core::SoloOrEnsembleChoice::Choice::solo )
                {
                    myOutPartData.instrumentData.soloOrEnsemble = api::SoloOrEnsemble::solo;
                }
            }
            
            if( scoreInstrument.getHasInstrumentSound() )
            {
                myOutPartData.instrumentData.sound = scoreInstrument.getInstrumentSound()->getValue().getValue();
            }
            
            if( scoreInstrument.getHasVirtualInstrument() )
            {
                parseVirtualInstrument( *scoreInstrument.getVirtualInstrument() );
            }
        }
        
        
        void PartReader::parseVirtualInstrument( const core::VirtualInstrument& virtualInstrument ) const
        {
            if( virtualInstrument.getHasVirtualName() )
            {
                myOutPartData.midiData.virtualName = virtualInstrument.getVirtualName()->getValue().getValue();
            }
            
            if( virtualInstrument.getHasVirtualLibrary() )
            {
                myOutPartData.midiData.virtualLibrary = virtualInstrument.getVirtualLibrary()->getValue().getValue();
            }
        }
        
        
        void PartReader::parseMidiDeviceInstrumentGroup( const core::MidiDeviceInstrumentGroup& grp ) const
        {
            if( grp.getHasMidiDevice() )
            {
                myOutPartData.midiData.device = grp.getMidiDevice()->getValue().getValue();
            }
            
            if( grp.getHasMidiInstrument() )
            {
                parseMidiInstrument( *grp.getMidiInstrument() );
            }
        }
        
        void PartReader::parseMidiInstrument( const core::MidiInstrument& inst ) const
        {
            myOutPartData.midiData.uniqueId = inst.getAttributes()->id.getValue();
            
            if( inst.getHasMidiChannel() )
            {
                myOutPartData.midiData.channel = inst.getMidiChannel()->getValue().getValue();
            }
            
            if( inst.getHasMidiName() )
            {
                myOutPartData.midiData.name = inst.getMidiName()->getValue().getValue();
            }
            
            if( inst.getHasMidiChannel() )
            {
                myOutPartData.midiData.channel = inst.getMidiChannel()->getValue().getValue();
            }
            
            if( inst.getHasMidiBank() )
            {
                myOutPartData.midiData.bank = inst.getMidiBank()->getValue().getValue();
            }
            
            if( inst.getHasMidiProgram() )
            {
                myOutPartData.midiData.program = inst.getMidiProgram()->getValue().getValue();
            }
            
            if( inst.getHasMidiUnpitched() )
            {
                myOutPartData.midiData.unpitched = inst.getMidiUnpitched()->getValue().getValue();
            }
            
            if( inst.getHasVolume() )
            {
                myOutPartData.midiData.isVolumeSpecified = true;
                myOutPartData.midiData.volume = inst.getVolume()->getValue().getValue();
            }
            
            if( inst.getHasPan() )
            {
                myOutPartData.midiData.isPanSpecified = true;
                myOutPartData.midiData.pan = inst.getPan()->getValue().getValue();
            }
            
            if( inst.getHasElevation() )
            {
                myOutPartData.midiData.isElevationSpecified = true;
                myOutPartData.midiData.elevation = inst.getElevation()->getValue().getValue();
            }
        }
    }
}
