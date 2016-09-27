// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/Enums.h"
#include "mx/api/ScoreData.h"
#include "mx/api/NoteData.h"
#include "mx/api/ClefData.h"

#include <map>

namespace mx
{
    namespace impl
    {
        class Converter
        {
        public:
            api::Step convert( core::StepEnum value ) const;
            core::StepEnum convert( api::Step value ) const;
            api::DurationName convert( core::NoteTypeValue value ) const;
            core::NoteTypeValue convert( api::DurationName value ) const;
            core::BeamValue convert( api::Beam value ) const;
            api::Beam convert( core::BeamValue value ) const;
            api::Accidental convert( core::AccidentalValue value ) const;
            core::AccidentalValue convert( api::Accidental value ) const;
            core::ClefSign convert( api::ClefSymbol value ) const;
            api::ClefSymbol convert( core::ClefSign value ) const;
            

            static std::map<core::StepEnum, api::Step> stepMap;
            static std::map<core::NoteTypeValue, api::DurationName> durationMap;
            static std::map<core::BeamValue, api::Beam> beamMap;
            static std::map<core::AccidentalValue, api::Accidental> accidentalMap;
            static std::map<core::ClefSign, api::ClefSymbol> clefMap;
            
        private:
            template<typename CORE_TYPE, typename API_TYPE>
            API_TYPE findApiItem( const std::map<CORE_TYPE, API_TYPE>& enumMap, API_TYPE defaultToReturn, CORE_TYPE itemToFind ) const
            {
                auto it = enumMap.find( itemToFind );
                if( it == enumMap.cend() )
                {
                    return defaultToReturn;
                }
                return it->second;
            }
            
            template<typename CORE_TYPE, typename API_TYPE>
            CORE_TYPE findCoreItem( const std::map<CORE_TYPE, API_TYPE>& enumMap, CORE_TYPE defaultToReturn, API_TYPE itemToFind ) const
            {
                auto compare = [&itemToFind]( const std::pair<CORE_TYPE, API_TYPE>& v )
                {
                    return v.second == itemToFind;
                };
                
                auto it = std::find_if( enumMap.cbegin(), enumMap.cend(), compare );
                
                if( it == enumMap.cend() )
                {
                    return defaultToReturn;
                }
                
                return it->first;
            }
        };
    }
}
