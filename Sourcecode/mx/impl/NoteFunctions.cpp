// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/NoteFunctions.h"
#include "mx/impl/MxNoteReader.h"
#include "mx/impl/TimeFunctions.h"
#include "mx/utility/Throw.h"

#include <algorithm>

namespace mx
{
    namespace impl
    {
        std::map<core::StepEnum, api::Step> NoteFunctions::stepMap =
        {
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::c, api::Step::c },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::d, api::Step::d },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::e, api::Step::e },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::f, api::Step::f },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::g, api::Step::g },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::a, api::Step::a },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::b, api::Step::b },
        };
        
        std::map<core::NoteTypeValue, api::DurationName> NoteFunctions::durationMap =
        {
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::maxima, api::DurationName::maxima },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::long_, api::DurationName::longa },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::breve, api::DurationName::breve },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::whole, api::DurationName::whole },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::half, api::DurationName::half },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::quarter, api::DurationName::quarter },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::eighth, api::DurationName::eighth },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::sixteenth, api::DurationName::dur16th },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::thirtySecond, api::DurationName::dur32nd },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::sixtyFourth, api::DurationName::dur64th },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::oneHundredTwentyEighth, api::DurationName::dur128th },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::twoHundredFifthySixth, api::DurationName::dur256th },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::fiveHundredTwelfth, api::DurationName::dur512th },
            std::pair<core::NoteTypeValue, api::DurationName>{ core::NoteTypeValue::oneThousandTwentyFourth, api::DurationName::dur1024th },
        };
        
        NoteFunctions::NoteFunctions() {}
        
        api::NoteData NoteFunctions::parseNote( const core::Note& inMxNote, const impl::Cursor& cursor ) const
        {
            impl::MxNoteReader reader{ inMxNote };
            api::NoteData outNoteData{};
            
            if( reader.getIsRest() )
            {
                outNoteData.isRest = true;
                outNoteData.isMeasureRest = reader.getIsMeasureRest();
            }
            
            outNoteData.isUnpitched = reader.getIsUnpitched();
            outNoteData.isDisplayStepOctaveSpecified = reader.getIsDisplayStepOctaveSpecified();
            // outNoteData.isChord = reader.getIsChord(); // TODO - add this to the reader
            
            if( reader.getIsCue() )
            {
                outNoteData.noteType = api::NoteType::cue;
            }
            
            if( reader.getIsGrace() )
            {
                outNoteData.noteType = api::NoteType::grace;
            }
            
            if( reader.getIsNormal() )
            {
                outNoteData.noteType = api::NoteType::normal;
            }
            
            outNoteData.step = this->convert( reader.getStep() );
            outNoteData.alter = reader.getAlter();
            // outNoteData.showAccidental = reader.getIsAccidentalShown(); // TODO - add this to the reader
            outNoteData.octave = reader.getOctave();
            outNoteData.staffIndex = reader.getStaffNumber() - 1;
            outNoteData.userRequestedVoiceNumber = reader.getVoiceNumber(); // TODO - make sure this is -1 when not found in the musicxml
            
            if( reader.getIsDurationTypeSpecified() )
            {
                outNoteData.durationName = this->convert( reader.getDurationType() );
            }
            else
            {
                outNoteData.durationName = api::DurationName::unspecified;
            }
            
            outNoteData.durationDots = reader.getNumDots();
            impl::TimeFunctions timeFunc;
            outNoteData.durationTicks = timeFunc.convertDurationToGlobalTickScale( cursor, reader.getDurationValue() );
            outNoteData.startPosition = cursor.position;
                        
            return outNoteData;
        }
        
        
        api::Step NoteFunctions::convert( core::StepEnum inStep ) const
        {
            auto it = stepMap.find( inStep );
            if( it == stepMap.cend() )
            {
                return api::Step::c;
            }
            return it->second;
        }
        
        
        core::StepEnum NoteFunctions::convert( api::Step inStep ) const
        {
            auto compare = [&inStep]( const std::pair<core::StepEnum, api::Step>& v )
            {
                return v.second == inStep;
            };
            
            auto it = std::find_if( stepMap.cbegin(), stepMap.cend(), compare );
            
            if( it == stepMap.cend() )
            {
                return core::StepEnum::c;
            }
            
            return it->first;
        }
        

        api::DurationName NoteFunctions::convert( core::NoteTypeValue inDur ) const
        {
            auto it = durationMap.find( inDur );
            if( it == durationMap.cend() )
            {
                return api::DurationName::unspecified;
            }
            return it->second;
        }
        
        
        core::NoteTypeValue NoteFunctions::convert( api::DurationName inDur ) const
        {
            auto compare = [&inDur]( const std::pair<core::NoteTypeValue, api::DurationName>& v )
            {
                return v.second == inDur;
            };
            
            auto it = std::find_if( durationMap.cbegin(), durationMap.cend(), compare );
            
            if( it == durationMap.cend() )
            {
                return core::NoteTypeValue::maxima;
            }
            
            return it->first;
        }
    }
}
