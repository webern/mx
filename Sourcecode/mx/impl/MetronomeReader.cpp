// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/MetronomeReader.h"
#include "mx/core/elements/BeatUnit.h"
#include "mx/core/elements/BeatUnitDot.h"
#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/elements/BeatUnitPer.h"
#include "mx/core/elements/BeatUnitPerOrNoteRelationNoteChoice.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/MetronomeNote.h"
#include "mx/core/elements/MetronomeRelation.h"
#include "mx/core/elements/MetronomeRelationGroup.h"
#include "mx/core/elements/NoteRelationNote.h"
#include "mx/core/elements/PerMinute.h"
#include "mx/core/elements/PerMinuteOrBeatUnitChoice.h"
#include "mx/impl/Converter.h"
#include "mx/utility/StringToInt.h"


namespace mx
{
    namespace impl
    {
        MetronomeReader::MetronomeReader( MetronomeReaderParameters&& params )
        : myMutex{}
        , myOutTempoData{}
        , myMetronome{ params.metronome }
        , myPreviousTempoData{ std::move( params.previousTempoData ) }
        , myCursor{ std::move( params.cursor ) }
        , myBeatUnitPerOrNoteRelationNoteChoice{ *myMetronome.getBeatUnitPerOrNoteRelationNoteChoice() }
        {
           
        }
        
        api::TempoData MetronomeReader::getTempoData() const
        {
            std::lock_guard<std::mutex> lock{ myMutex };
            myOutTempoData = api::TempoData{};
            using FirstChoice = core::BeatUnitPerOrNoteRelationNoteChoice::Choice;
            const auto firstChoice = myBeatUnitPerOrNoteRelationNoteChoice.getChoice();
            
            switch ( firstChoice )
            {
                case FirstChoice::beatUnitPer:
                {
                    parseBeatUnitPer();
                    break;
                }
                case FirstChoice::noteRelationNote:
                {
                    parseNoteRelationNote();
                    break;
                }
                default:
                    break;
            }
            
            api::TempoData temp{ std::move( myOutTempoData ) };
            myOutTempoData = api::TempoData{};
            return temp;
        }
        
        
        void MetronomeReader::parseBeatUnitPer() const
        {
            const auto beatUnitPer = *myBeatUnitPerOrNoteRelationNoteChoice.getBeatUnitPer();
            const auto choice = beatUnitPer.getPerMinuteOrBeatUnitChoice()->getChoice();
            
            switch ( choice )
            {
                case core::PerMinuteOrBeatUnitChoice::Choice::perMinute:
                {
                    parseBeatsPerMinute();
                    break;
                }
                case core::PerMinuteOrBeatUnitChoice::Choice::beatUnitGroup:
                {
                    parseMetronomeModulation();
                    break;
                }
                default:
                    break;
            }
        }
        
        
        void MetronomeReader::parseNoteRelationNote() const
        {
            MX_THROW( "wtf is this" );
        }
        
        
        void MetronomeReader::parseBeatsPerMinute() const
        {
            myOutTempoData.tempoType = api::TempoType::beatsPerMinute;
            const auto& beatUnitPer = *myBeatUnitPerOrNoteRelationNoteChoice.getBeatUnitPer();
            const auto& grp = *beatUnitPer.getBeatUnitGroup();
            Converter converter;
            myOutTempoData.beatsPerMinute.durationName =  converter.convert( grp.getBeatUnit()->getValue() );
            myOutTempoData.beatsPerMinute.dots = static_cast<int>( grp.getBeatUnitDotSet().size() );
            const auto bpmStr = beatUnitPer.getPerMinuteOrBeatUnitChoice()->getPerMinute()->getValue().getValue();
            int bpm = -1;
            bool isNumeric = utility::stringToInt( bpmStr, bpm );
            if( !isNumeric )
            {
                myOutTempoData.tempoType = api::TempoType::tempoText;
                myOutTempoData.tempoText.tempoText = bpmStr;
            }
            else
            {
                myOutTempoData.beatsPerMinute.beatsPerMinute = bpm;
            }
        }
        
        
        void MetronomeReader::parseMetronomeModulation() const
        {
            
        }
    }
}
