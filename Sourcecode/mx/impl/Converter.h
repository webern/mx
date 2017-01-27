// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ClefData.h"
#include "mx/api/MarkData.h"
#include "mx/api/NoteData.h"
#include "mx/api/PositionData.h"
#include "mx/api/ScoreData.h"
#include "mx/api/BarlineData.h"
#include "mx/core/elements/ArticulationsChoice.h"
#include "mx/core/elements/OrnamentsChoice.h"
#include "mx/core/elements/TechnicalChoice.h"
#include "mx/core/Enums.h"

#include <map>

namespace mx
{
    namespace impl
    {
        class Converter
        {
        public:
            core::StepEnum convert( api::Step value ) const;
            api::Step convert( core::StepEnum value ) const;
            
            core::NoteTypeValue convert( api::DurationName value ) const;
            api::DurationName convert( core::NoteTypeValue value ) const;
            
            core::BeamValue convert( api::Beam value ) const;
            api::Beam convert( core::BeamValue value ) const;
            
            core::AccidentalValue convert( api::Accidental value ) const;
            api::Accidental convert( core::AccidentalValue value ) const;
            
            core::ClefSign convert( api::ClefSymbol value ) const;
            api::ClefSymbol convert( core::ClefSign value ) const;
            
            core::AboveBelow convert( api::Placement value ) const;
            api::Placement convert( core::AboveBelow value ) const;
            
            core::YesNo convert( api::Bool value ) const;
            api::Bool convert( core::YesNo value ) const;
            
            core::Valign convert( api::VerticalAlignment value ) const;
            api::VerticalAlignment convert( core::Valign value ) const;
            
            core::LeftCenterRight convert( api::HorizontalAlignment value ) const;
            api::HorizontalAlignment convert( core::LeftCenterRight value ) const;
            
            core::CssFontSize convert( api::CssSize value ) const;
            api::CssSize convert( core::CssFontSize value ) const;
            
            core::FontStyle convert( api::FontStyle value ) const;
            api::FontStyle convert( core::FontStyle value ) const;
            
            core::FontWeight convert( api::FontWeight value ) const;
            api::FontWeight convert( core::FontWeight value ) const;
            
            core::ArticulationsChoice::Choice convertArticulation( api::MarkType value ) const;
            api::MarkType convertArticulation( core::ArticulationsChoice::Choice value ) const;
            bool isArticulation( api::MarkType value ) const;
            
            core::DynamicsEnum convertDynamic( api::MarkType value ) const;
            api::MarkType convertDynamic( core::DynamicsEnum value ) const;
            
            core::OrnamentsChoice::Choice convertOrnament( api::MarkType value ) const;
            api::MarkType convertOrnament( core::OrnamentsChoice::Choice value ) const;
            
            core::AccidentalValue convertAccidentalMark( api::MarkType value ) const;
            api::MarkType convertAccidentalMark( core::AccidentalValue value ) const;
            
            core::TechnicalChoice::Choice convertTechnicalMark( api::MarkType value ) const;
            api::MarkType convertTechnicalMark( core::TechnicalChoice::Choice value ) const;
            
            core::MeasureNumberingValue convertMeasureNumbering( api::MeasureNumbering value ) const;
            api::MeasureNumbering convertMeasureNumbering( core::MeasureNumberingValue value ) const;
            
            core::StemValue convert( api::Stem value ) const;
            api::Stem convert( core::StemValue value ) const;
            
            core::LineType convert( api::LineType value ) const;
            api::LineType convert( core::LineType value ) const;
            
            core::WedgeType convert( api::WedgeType value ) const;
            api::WedgeType convert( core::WedgeType value ) const;
            
            core::BarStyleEnum convert( api::BarlineType value ) const;
            api::BarlineType convert( core::BarStyleEnum value ) const;
            
            core::RightLeftMiddle convertBarlinePlacement( api::HorizontalAlignment value ) const;
            api::HorizontalAlignment convertBarlinePlacement( core::RightLeftMiddle value ) const;
            
            core::LineEnd convert( api::LineHook value ) const;
            api::LineHook convert( core::LineEnd value ) const;
            
            core::FermataShape convertFermata( api::MarkType value ) const;
            api::MarkType convertFermata( core::FermataShape value ) const;
            
            const static std::map<core::StepEnum, api::Step> stepMap;
            const static std::map<core::NoteTypeValue, api::DurationName> durationMap;
            const static std::map<core::BeamValue, api::Beam> beamMap;
            const static std::map<core::AccidentalValue, api::Accidental> accidentalMap;
            const static std::map<core::ClefSign, api::ClefSymbol> clefMap;
            const static std::map<core::AboveBelow, api::Placement> placementMap;
            const static std::map<core::YesNo, api::Bool> boolMap;
            const static std::map<core::Valign, api::VerticalAlignment> valignMap;
            const static std::map<core::LeftCenterRight, api::HorizontalAlignment> halignMap;
            const static std::map<core::CssFontSize, api::CssSize> cssMap;
            const static std::map<core::FontStyle, api::FontStyle> fontStyleMap;
            const static std::map<core::FontWeight, api::FontWeight> fontWeightMap;
            const static std::map<core::ArticulationsChoice::Choice, api::MarkType> articulationsMap;
            const static std::map<core::DynamicsEnum, api::MarkType> dynamicsMap;
            const static std::map<core::OrnamentsChoice::Choice, api::MarkType> ornamentsMap;
            const static std::map<core::AccidentalValue, api::MarkType> accidentalMarkMap;
            const static std::map<core::TechnicalChoice::Choice, api::MarkType> technicalMarkMap;
            const static std::map<core::MeasureNumberingValue, api::MeasureNumbering> measureNumberingMap;
            const static std::map<core::StemValue, api::Stem> stemMap;
            const static std::map<core::LineType, api::LineType> lineType;
            const static std::map<core::WedgeType, api::WedgeType> wedgeMap;
            const static std::map<core::BarStyleEnum, api::BarlineType> barlineMap;
            const static std::map<core::RightLeftMiddle, api::HorizontalAlignment> barlinePlacementMap;
            const static std::map<core::LineEnd, api::LineHook> lineStopMap;
            const static std::map<core::FermataShape, api::MarkType> fermataMap;
            
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
