// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/Converter.h"
#include "mx/utility/Throw.h"

#include <algorithm>

namespace mx
{
    namespace impl
    {
        const std::map<core::StepEnum, api::Step> Converter::stepMap =
        {
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::c, api::Step::c },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::d, api::Step::d },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::e, api::Step::e },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::f, api::Step::f },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::g, api::Step::g },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::a, api::Step::a },
            std::pair<core::StepEnum, api::Step>{ core::StepEnum::b, api::Step::b },
        };
        
        const std::map<core::NoteTypeValue, api::DurationName> Converter::durationMap =
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
        
        
        const std::map<core::BeamValue, api::Beam> Converter::beamMap =
        {
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::begin, api::Beam::start },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::end, api::Beam::stop },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::continue_, api::Beam::continue_ },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::forwardHook, api::Beam::forwardHook },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::backwardHook, api::Beam::backwardHook },
        };
        
        
        const std::map<core::AccidentalValue, api::Accidental> Converter::accidentalMap =
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
        
        
        const std::map<core::ClefSign, api::ClefSymbol> Converter::clefMap =
        {
            std::pair<core::ClefSign, api::ClefSymbol>{ core::ClefSign::c, api::ClefSymbol::c },
            std::pair<core::ClefSign, api::ClefSymbol>{ core::ClefSign::f, api::ClefSymbol::f },
            std::pair<core::ClefSign, api::ClefSymbol>{ core::ClefSign::g, api::ClefSymbol::g },
            std::pair<core::ClefSign, api::ClefSymbol>{ core::ClefSign::jianpu, api::ClefSymbol::jianpu },
            std::pair<core::ClefSign, api::ClefSymbol>{ core::ClefSign::none, api::ClefSymbol::none },
            std::pair<core::ClefSign, api::ClefSymbol>{ core::ClefSign::percussion, api::ClefSymbol::percussion },
            std::pair<core::ClefSign, api::ClefSymbol>{ core::ClefSign::tab, api::ClefSymbol::tab }
        };
        
        
        const std::map<core::AboveBelow, api::Placement> Converter::placementMap =
        {
            std::pair<core::AboveBelow, api::Placement>{ core::AboveBelow::above, api::Placement::above },
            std::pair<core::AboveBelow, api::Placement>{ core::AboveBelow::below, api::Placement::below },
        };
        
        
        const std::map<core::YesNo, api::Bool> Converter::boolMap =
        {
            std::pair<core::YesNo, api::Bool>{ core::YesNo::yes, api::Bool::yes },
            std::pair<core::YesNo, api::Bool>{ core::YesNo::no, api::Bool::no },
        };
        
        
        const std::map<core::Valign, api::VerticalAlignment> Converter::valignMap =
        {
            std::pair<core::Valign, api::VerticalAlignment>{ core::Valign::baseline, api::VerticalAlignment::baseline },
            std::pair<core::Valign, api::VerticalAlignment>{ core::Valign::bottom, api::VerticalAlignment::bottom },
            std::pair<core::Valign, api::VerticalAlignment>{ core::Valign::middle, api::VerticalAlignment::middle },
            std::pair<core::Valign, api::VerticalAlignment>{ core::Valign::top, api::VerticalAlignment::top },
        };
        
        
        const std::map<core::LeftCenterRight, api::HorizontalAlignment> Converter::halignMap =
        {
            std::pair<core::LeftCenterRight, api::HorizontalAlignment>{ core::LeftCenterRight::left, api::HorizontalAlignment::left },
            std::pair<core::LeftCenterRight, api::HorizontalAlignment>{ core::LeftCenterRight::center, api::HorizontalAlignment::center },
            std::pair<core::LeftCenterRight, api::HorizontalAlignment>{ core::LeftCenterRight::right, api::HorizontalAlignment::right },
        };
        
        
        api::Step Converter::convert( core::StepEnum inStep ) const
        {
            auto it = stepMap.find( inStep );
            if( it == stepMap.cend() )
            {
                return api::Step::c;
            }
            return it->second;
        }
        
        
        core::StepEnum Converter::convert( api::Step inStep ) const
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
        
        
        api::DurationName Converter::convert( core::NoteTypeValue inDur ) const
        {
            auto it = durationMap.find( inDur );
            if( it == durationMap.cend() )
            {
                return api::DurationName::unspecified;
            }
            return it->second;
        }
        
        
        core::NoteTypeValue Converter::convert( api::DurationName inDur ) const
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
        
        
        core::BeamValue Converter::convert( api::Beam value ) const
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
        
        
        api::Beam Converter::convert( core::BeamValue value ) const
        {
            auto it = beamMap.find( value );
            if( it == beamMap.cend() )
            {
                return api::Beam::unspecified;
            }
            return it->second;
        }
        
        
        api::Accidental Converter::convert( core::AccidentalValue value ) const
        {
            auto it = accidentalMap.find( value );
            if( it == accidentalMap.cend() )
            {
                return api::Accidental::none;
            }
            return it->second;
        }
        
        
        core::AccidentalValue Converter::convert( api::Accidental value ) const
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
        
        core::ClefSign Converter::convert( api::ClefSymbol value ) const
        {
            return findCoreItem( clefMap, core::ClefSign::g, value );
        }
        
        
        api::ClefSymbol Converter::convert( core::ClefSign value ) const
        {
            return findApiItem( clefMap, api::ClefSymbol::none, value );
        }
        
        
        api::Placement Converter::convert( core::AboveBelow value ) const
        {
            return findApiItem( placementMap, api::Placement::unspecified, value );
        }
        
        
        core::AboveBelow Converter::convert( api::Placement value ) const
        {
            return findCoreItem( placementMap, core::AboveBelow::below, value );
        }
        
        
        api::Bool Converter::convert( core::YesNo value ) const
        {
            return findApiItem( boolMap, api::Bool::unspecified, value );
        }
        
        
        core::YesNo Converter::convert( api::Bool value ) const
        {
            return findCoreItem( boolMap, core::YesNo::no, value );
        }
        
        
        api::VerticalAlignment Converter::convert( core::Valign value ) const
        {
            return findApiItem( valignMap, api::VerticalAlignment::unspecified, value );
        }
        
        
        core::Valign Converter::convert( api::VerticalAlignment value ) const
        {
            return findCoreItem( valignMap, core::Valign::baseline, value );
        }
        
        
        api::HorizontalAlignment Converter::convert( core::LeftCenterRight value ) const
        {
            return findApiItem( halignMap, api::HorizontalAlignment::unspecified, value );
        }
        
        
        core::LeftCenterRight Converter::convert( api::HorizontalAlignment value ) const
        {
            return findCoreItem( halignMap, core::LeftCenterRight::left, value );
        }
        
        
    }
}
