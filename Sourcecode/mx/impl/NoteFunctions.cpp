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
        
        
        std::map<core::BeamValue, api::Beam> NoteFunctions::beamMap =
        {
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::begin, api::Beam::start },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::end, api::Beam::stop },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::continue_, api::Beam::continue_ },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::forwardHook, api::Beam::forwardHook },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::backwardHook, api::Beam::backwardHook },
        };
        
        
        std::map<core::AccidentalValue, api::Accidental> NoteFunctions::accidentalMap =
        {
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::sharp, api::Accidental::sharp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::natural, api::Accidental::natural },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::flat, api::Accidental::flat },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::doubleSharp, api::Accidental::doubleSharp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::flatFlat, api::Accidental::flatFlat },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::naturalSharp, api::Accidental::naturalSharp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::naturalFlat, api::Accidental::naturalFlat },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::quarterFlat, api::Accidental::quarterFlat },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::quarterSharp, api::Accidental::quarterSharp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::threeQuartersFlat, api::Accidental::threeQuartersFlat },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::threeQuartersSharp, api::Accidental::threeQuartersSharp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::sharpDown, api::Accidental::sharpDown },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::sharpUp, api::Accidental::sharpUp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::naturalDown, api::Accidental::naturalDown },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::naturalUp, api::Accidental::naturalUp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::flatDown, api::Accidental::flatDown },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::flatUp, api::Accidental::flatUp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::tripleSharp, api::Accidental::tripleSharp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::tripleFlat, api::Accidental::tripleFlat },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::slashQuarterSharp, api::Accidental::slashQuarterSharp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::slashSharp, api::Accidental::slashSharp },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::slashFlat, api::Accidental::slashFlat },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::doubleSlashFlat, api::Accidental::doubleSlashFlat },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::sharp1, api::Accidental::sharp1 },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::sharp2, api::Accidental::sharp2 },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::sharp3, api::Accidental::sharp3 },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::sharp5, api::Accidental::sharp5 },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::flat1, api::Accidental::flat1 },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::flat2, api::Accidental::flat2 },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::flat3, api::Accidental::flat3 },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::flat4, api::Accidental::flat4 },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::sori, api::Accidental::sori },
            std::pair<core::AccidentalValue, api::Accidental>{ core::AccidentalValue::koron, api::Accidental::koron },
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
            
            outNoteData.accidental = api::Accidental::none;
            
            if( reader.getHasAccidental() )
            {
                outNoteData.accidental = convert( reader.getAccidental() );
                outNoteData.isAccidentalParenthetical = reader.getIsAccidentalParenthetical();
                outNoteData.isAccidentalCautionary = reader.getIsAccidentalCautionary();
                outNoteData.isAccidentalEditorial = reader.getIsAccidentalEditorial();
                outNoteData.isAccidentalBracketed = reader.getIsAccidentalBracketed();
            }
            
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
            outNoteData.durationTimeTicks = timeFunc.convertDurationToGlobalTickScale( cursor, reader.getDurationValue() );
            outNoteData.startTimeTicks = cursor.position;
            
            for( const auto& coreBeamVal : reader.getBeams() )
            {
                outNoteData.beams.push_back( convert( coreBeamVal ) );
            }
            
            outNoteData.timeModificationActualNotes = reader.getTimeModificationActualNotes();
            outNoteData.timeModificationNormalNotes = reader.getTimeModificationNormalNotes();
            
            const core::NoteTypeValue timeModType = reader.getTimeModificationNormalType();
            const int timeModTypeDots = reader.getTimeModificationNormalTypeDots();
            bool isTimeModTypeSpecified = ( timeModTypeDots > 0 ) && ( timeModType != reader.getDurationType() );
            
            if( isTimeModTypeSpecified )
            {
                outNoteData.timeModificationNormalType = convert( timeModType );
                outNoteData.timeModificationNormalTypeDots = timeModTypeDots;
            }
            else
            {
                outNoteData.timeModificationNormalType = api::DurationName::unspecified;
                outNoteData.timeModificationNormalTypeDots = 0;
            }
            
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
        
        
        core::BeamValue NoteFunctions::convert( api::Beam value ) const
        {
            auto compare = [&value]( const std::pair<core::BeamValue, api::Beam>& v )
            {
                return v.second == value;
            };
            
            auto it = std::find_if( beamMap.cbegin(), beamMap.cend(), compare );
            
            if( it == beamMap.cend() )
            {
                return core::BeamValue::begin;
            }
            
            return it->first;
        }
        
        
        api::Beam NoteFunctions::convert( core::BeamValue value ) const
        {
            auto it = beamMap.find( value );
            if( it == beamMap.cend() )
            {
                return api::Beam::unspecified;
            }
            return it->second;
        }
        
        
        api::Accidental NoteFunctions::convert( core::AccidentalValue value ) const
        {
            auto it = accidentalMap.find( value );
            if( it == accidentalMap.cend() )
            {
                return api::Accidental::none;
            }
            return it->second;
        }
        
        
        core::AccidentalValue NoteFunctions::convert( api::Accidental value ) const
        {
            auto compare = [&value]( const std::pair<core::AccidentalValue, api::Accidental>& v )
            {
                return v.second == value;
            };
            
            auto it = std::find_if( accidentalMap.cbegin(), accidentalMap.cend(), compare );
            
            if( it == accidentalMap.cend() )
            {
                return core::AccidentalValue::natural;
            }
            
            return it->first;
        }
    }
}
