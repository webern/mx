// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
#include "mx/core/elements/PartGroupOrScorePart.h"
#include "mx/core/elements/PartList.h"
#include "mx/core/elements/PartName.h"
#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/elements/ScoreInstrument.h"
#include "mx/core/elements/ScorePart.h"
#include "mx/core/elements/ScorePartwise.h"
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
        PartReader::PartReader( const core::ScorePart& inScorePart, const core::PartwisePart& inPartwisePartRef, int globalTicksPerMeasure, const core::ScorePartwise& inScore, int inDivisionsValue )
        : myPartwisePart{ inPartwisePartRef }
        , myScorePart{ inScorePart }
        , myNumStaves{ -1 }
        , myGlobalTicksPerMeasure{ globalTicksPerMeasure }
        , myScore{ inScore }
        , myPartIndex{ -1 }
        , myConstructedDivisionsValue{ inDivisionsValue }
        {
            const auto ppId = myPartwisePart.getAttributes()->id.getValue();
            const auto spId = myScorePart.getAttributes()->id.getValue();
            if( ppId != spId )
            {
                MX_THROW( "the partwise-part id must match the score-part id" );
            }
            const auto partIndex = findPartIndex( ppId );
            MX_ASSERT( partIndex >= 0 );
            myPartIndex = partIndex;
            myNumStaves = calculateNumStaves();
        }
        
        
        api::PartData PartReader::getPartData()
        {
            std::lock_guard<std::mutex> lock{ myMutex };
            myOutPartData = api::PartData{};
            parseScorePart();
            
            myCurrentCursor = MeasureCursor{ myNumStaves, myGlobalTicksPerMeasure };
            myCurrentCursor.partIndex = myPartIndex;

            if( myConstructedDivisionsValue > 0 )
            {
                myCurrentCursor.ticksPerQuarter = myConstructedDivisionsValue;
            }

            myPreviousCursor = myCurrentCursor;

            for( const auto& mxMeasurePtr : myPartwisePart.getPartwiseMeasureSet() )
            {
                const auto& mxMeasure = *mxMeasurePtr;
                MeasureReader reader{ mxMeasure, myCurrentCursor, myPreviousCursor };
                // the reader returns the measure data and any data that needs to be written at
                // the part-level (e.g. transposition). currently this is done as a pair.
                auto measureDataPair = reader.getMeasureData();
                auto measureData = measureDataPair.first;
                auto& transpositionData = measureDataPair.second;
                // if it's the first measure, and if we received transposition data, then we
                // should write it to the part.
                if( myCurrentCursor.isFirstMeasureInPart && transpositionData.has_value() )
                {
                    myOutPartData.transposition = transpositionData;
                }
                myCurrentCursor.timeSignature = measureData.timeSignature;
                myCurrentCursor.ticksPerQuarter = reader.getCursor().ticksPerQuarter;
                myOutPartData.measures.emplace_back( std::move( measureData ) );
                ++myCurrentCursor.measureIndex;
                myPreviousCursor = myCurrentCursor;
            }
            
            api::PartData tempReturn = std::move( myOutPartData );
            myOutPartData = api::PartData{};
            return tempReturn;
        }


        MeasureCursor PartReader::getCursor() const
        {
            return myCurrentCursor;
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
                Converter c;
                myOutPartData.instrumentData.soundID = c.convert( scoreInstrument.getInstrumentSound()->getValue() );
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
                myOutPartData.instrumentData.midiData.virtualName = virtualInstrument.getVirtualName()->getValue().getValue();
            }
            
            if( virtualInstrument.getHasVirtualLibrary() )
            {
                myOutPartData.instrumentData.midiData.virtualLibrary = virtualInstrument.getVirtualLibrary()->getValue().getValue();
            }
        }
        
        
        void PartReader::parseMidiDeviceInstrumentGroup( const core::MidiDeviceInstrumentGroup& grp ) const
        {
            if( grp.getHasMidiDevice() )
            {
                myOutPartData.instrumentData.midiData.device = grp.getMidiDevice()->getValue().getValue();
            }
            
            if( grp.getHasMidiInstrument() )
            {
                parseMidiInstrument( *grp.getMidiInstrument() );
            }
        }
        
        void PartReader::parseMidiInstrument( const core::MidiInstrument& inst ) const
        {
            if( inst.getHasMidiChannel() )
            {
                myOutPartData.instrumentData.midiData.channel = inst.getMidiChannel()->getValue().getValue();
            }
            
            if( inst.getHasMidiName() )
            {
                myOutPartData.instrumentData.midiData.name = inst.getMidiName()->getValue().getValue();
            }
            
            if( inst.getHasMidiChannel() )
            {
                myOutPartData.instrumentData.midiData.channel = inst.getMidiChannel()->getValue().getValue();
            }
            
            if( inst.getHasMidiBank() )
            {
                myOutPartData.instrumentData.midiData.bank = inst.getMidiBank()->getValue().getValue();
            }
            
            if( inst.getHasMidiProgram() )
            {
                myOutPartData.instrumentData.midiData.program = inst.getMidiProgram()->getValue().getValue();
            }
            
            if( inst.getHasMidiUnpitched() )
            {
                myOutPartData.instrumentData.midiData.unpitched = inst.getMidiUnpitched()->getValue().getValue();
            }
            
            if( inst.getHasVolume() )
            {
                myOutPartData.instrumentData.midiData.isVolumeSpecified = true;
                myOutPartData.instrumentData.midiData.volume = inst.getVolume()->getValue().getValue();
            }
            
            if( inst.getHasPan() )
            {
                myOutPartData.instrumentData.midiData.isPanSpecified = true;
                myOutPartData.instrumentData.midiData.pan = inst.getPan()->getValue().getValue();
            }
            
            if( inst.getHasElevation() )
            {
                myOutPartData.instrumentData.midiData.isElevationSpecified = true;
                myOutPartData.instrumentData.midiData.elevation = inst.getElevation()->getValue().getValue();
            }
        }

        int PartReader::findPartIndex( const std::string& inPartId ) const
        {
            const auto& partList = *myScore.getScoreHeaderGroup()->getPartList();
            const auto& firstPart = *partList.getScorePart();
            int index = 0;

            if( firstPart.getAttributes()->id.getValue() == inPartId )
            {
                return index;
            }

            ++index;

            for( const auto& p : partList.getPartGroupOrScorePartSet() )
            {
                if( p->getChoice() == core::PartGroupOrScorePart::Choice::scorePart )
                {
                    if( p->getScorePart()->getAttributes()->id.getValue() == inPartId )
                    {
                        return index;
                    }
                    ++index;
                }
            }
            return -1;
        }
    }
}
