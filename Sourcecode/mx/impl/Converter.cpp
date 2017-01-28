// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::begin, api::Beam::begin },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::end, api::Beam::end },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::continue_, api::Beam::extend },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::forwardHook, api::Beam::forwardBroken },
            std::pair<core::BeamValue, api::Beam>{ core::BeamValue::backwardHook, api::Beam::backwardBroken },
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
        
        
        const std::map<core::CssFontSize, api::CssSize> Converter::cssMap =
        {
            std::pair<core::CssFontSize, api::CssSize>{ core::CssFontSize::xxSmall, api::CssSize::xxSmall },
            std::pair<core::CssFontSize, api::CssSize>{ core::CssFontSize::xSmall, api::CssSize::xSmall },
            std::pair<core::CssFontSize, api::CssSize>{ core::CssFontSize::small, api::CssSize::small },
            std::pair<core::CssFontSize, api::CssSize>{ core::CssFontSize::medium, api::CssSize::medium },
            std::pair<core::CssFontSize, api::CssSize>{ core::CssFontSize::large, api::CssSize::large },
            std::pair<core::CssFontSize, api::CssSize>{ core::CssFontSize::xLarge, api::CssSize::xLarge },
            std::pair<core::CssFontSize, api::CssSize>{ core::CssFontSize::xxLarge, api::CssSize::xxLarge },
        };
        
        
        const std::map<core::ArticulationsChoice::Choice, api::MarkType> Converter::articulationsMap =
        {
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::accent, api::MarkType::accent },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::strongAccent, api::MarkType::strongAccent },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::staccato, api::MarkType::staccato },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::tenuto, api::MarkType::tenuto },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::detachedLegato, api::MarkType::detachedLegato },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::staccatissimo, api::MarkType::staccatissimo },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::spiccato, api::MarkType::spiccato },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::scoop, api::MarkType::scoop },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::plop, api::MarkType::plop },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::doit, api::MarkType::doit },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::falloff, api::MarkType::falloff },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::breathMark, api::MarkType::breathMark },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::caesura, api::MarkType::caesura },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::stress, api::MarkType::stress },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::unstress, api::MarkType::unstress },
            std::pair<core::ArticulationsChoice::Choice, api::MarkType>{ core::ArticulationsChoice::Choice::otherArticulation, api::MarkType::otherArticulation },
        };
        
        
        const std::map<core::DynamicsEnum, api::MarkType> Converter::dynamicsMap =
        {
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::p, api::MarkType::p },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::pp, api::MarkType::pp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::ppp, api::MarkType::ppp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::pppp, api::MarkType::pppp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::ppppp, api::MarkType::ppppp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::pppppp, api::MarkType::pppppp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::f, api::MarkType::f },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::ff, api::MarkType::ff },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::fff, api::MarkType::fff },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::ffff, api::MarkType::ffff },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::fffff, api::MarkType::fffff },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::ffffff, api::MarkType::ffffff },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::mp, api::MarkType::mp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::mf, api::MarkType::mf },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::sf, api::MarkType::sf },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::sfp, api::MarkType::sfp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::sfpp, api::MarkType::sfpp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::fp, api::MarkType::fp },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::rf, api::MarkType::rf },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::rfz, api::MarkType::rfz },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::sfz, api::MarkType::sfz },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::sffz, api::MarkType::sffz },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::fz, api::MarkType::fz },
            std::pair<core::DynamicsEnum, api::MarkType>{ core::DynamicsEnum::otherDynamics, api::MarkType::otherDynamics },
        };


        const std::map<core::OrnamentsChoice::Choice, api::MarkType> Converter::ornamentsMap =
        {
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::trillMark, api::MarkType::trillMark },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::turn, api::MarkType::turn },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::delayedTurn, api::MarkType::delayedTurn },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::invertedTurn, api::MarkType::invertedTurn },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::delayedInvertedTurn, api::MarkType::delayedInvertedTurn },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::verticalTurn, api::MarkType::verticalTurn },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::shake, api::MarkType::shake },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::wavyLine, api::MarkType::wavyLine },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::mordent, api::MarkType::mordent },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::invertedMordent, api::MarkType::invertedMordent },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::schleifer, api::MarkType::schleifer },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::tremolo, api::MarkType::tremolo },
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::otherOrnament, api::MarkType::otherOrnament },
        };
        
        
        const std::map<core::AccidentalValue, api::MarkType> Converter::accidentalMarkMap =
        {
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sharp, api::MarkType::accidentalMarkSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::natural, api::MarkType::accidentalMarkNatural },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::flat, api::MarkType::accidentalMarkFlat },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::doubleSharp, api::MarkType::accidentalMarkDoubleSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sharpSharp, api::MarkType::accidentalMarkSharpSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::flatFlat, api::MarkType::accidentalMarkFlatFlat },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::naturalSharp, api::MarkType::accidentalMarkNaturalSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::naturalFlat, api::MarkType::accidentalMarkNaturalFlat },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::quarterFlat, api::MarkType::accidentalMarkQuarterFlat },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::quarterSharp, api::MarkType::accidentalMarkQuarterSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::threeQuartersFlat, api::MarkType::accidentalMarkThreeQuartersFlat },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::threeQuartersSharp, api::MarkType::accidentalMarkThreeQuartersSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sharpDown, api::MarkType::accidentalMarkSharpDown },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sharpUp, api::MarkType::accidentalMarkSharpUp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::naturalDown, api::MarkType::accidentalMarkNaturalDown },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::naturalUp, api::MarkType::accidentalMarkNaturalUp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::flatDown, api::MarkType::accidentalMarkFlatDown },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::flatUp, api::MarkType::accidentalMarkFlatUp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::tripleSharp, api::MarkType::accidentalMarkTripleSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::tripleFlat, api::MarkType::accidentalMarkTripleFlat },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::slashQuarterSharp, api::MarkType::accidentalMarkSlashQuarterSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::slashSharp, api::MarkType::accidentalMarkSlashSharp },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::slashFlat, api::MarkType::accidentalMarkSlashFlat },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::doubleSlashFlat, api::MarkType::accidentalMarkDoubleSlashFlat },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sharp1, api::MarkType::accidentalMarkSharp1 },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sharp2, api::MarkType::accidentalMarkSharp2 },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sharp3, api::MarkType::accidentalMarkSharp3 },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sharp5, api::MarkType::accidentalMarkSharp5 },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::flat1, api::MarkType::accidentalMarkFlat1 },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::flat2, api::MarkType::accidentalMarkFlat2 },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::flat3, api::MarkType::accidentalMarkFlat3 },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::flat4, api::MarkType::accidentalMarkFlat4 },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::sori, api::MarkType::accidentalMarkSori },
            std::pair<core::AccidentalValue, api::MarkType>{ core::AccidentalValue::koron, api::MarkType::accidentalMarkKoron },
        };
        
        
        const std::map<core::FontStyle, api::FontStyle> Converter::fontStyleMap =
        {
            std::pair<core::FontStyle, api::FontStyle>{ core::FontStyle::normal, api::FontStyle::normal },
            std::pair<core::FontStyle, api::FontStyle>{ core::FontStyle::italic, api::FontStyle::italic },
        };
        
        
        const std::map<core::FontWeight, api::FontWeight> Converter::fontWeightMap =
        {
            std::pair<core::FontWeight, api::FontWeight>{ core::FontWeight::normal, api::FontWeight::normal },
            std::pair<core::FontWeight, api::FontWeight>{ core::FontWeight::bold, api::FontWeight::bold },
        };
        
        
        const std::map<core::MeasureNumberingValue, api::MeasureNumbering> Converter::measureNumberingMap =
        {
            std::make_pair( core::MeasureNumberingValue::measure, api::MeasureNumbering::measure ),
            std::make_pair( core::MeasureNumberingValue::none, api::MeasureNumbering::none ),
            std::make_pair( core::MeasureNumberingValue::system, api::MeasureNumbering::system ),
        };

        
        const std::map<core::TechnicalChoice::Choice, api::MarkType> Converter::technicalMarkMap =
        {
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::technical, api::MarkType::unspecified },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::upBow, api::MarkType::upBow },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::downBow, api::MarkType::downBow },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::harmonic, api::MarkType::harmonic },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::openString, api::MarkType::openString },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::thumbPosition, api::MarkType::thumbPosition },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::fingering, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::pluck, api::MarkType::unspecified },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::doubleTongue, api::MarkType::doubleTongue },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::tripleTongue, api::MarkType::tripleTongue },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::stopped, api::MarkType::stopped },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::snapPizzicato, api::MarkType::snapPizzicato },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::fret, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::string_, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::hammerOn, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::pullOff, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::bend, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::tap, api::MarkType::unspecified },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::heel, api::MarkType::heel },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::toe, api::MarkType::toe },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::fingernails, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::hole, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::arrow, api::MarkType::unspecified },
            // std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::handbell, api::MarkType::unspecified },
            std::pair<core::TechnicalChoice::Choice, api::MarkType>{ core::TechnicalChoice::Choice::otherTechnical, api::MarkType::otherTechnical },
        };
        
        
        const std::map<core::StemValue, api::Stem> Converter::stemMap =
        {
            std::pair<core::StemValue, api::Stem>{ core::StemValue::up, api::Stem::up },
            std::pair<core::StemValue, api::Stem>{ core::StemValue::down, api::Stem::down },
            std::pair<core::StemValue, api::Stem>{ core::StemValue::none, api::Stem::none },
            std::pair<core::StemValue, api::Stem>{ core::StemValue::double_, api::Stem::both },
        };
        
        
        const std::map<core::LineType, api::LineType> Converter::lineType =
        {
            std::pair<core::LineType, api::LineType>{ core::LineType::dashed, api::LineType::dashed },
            std::pair<core::LineType, api::LineType>{ core::LineType::dotted, api::LineType::dotted },
            std::pair<core::LineType, api::LineType>{ core::LineType::solid, api::LineType::solid },
            std::pair<core::LineType, api::LineType>{ core::LineType::wavy, api::LineType::wavy },
        };
        
        
        const std::map<core::WedgeType, api::WedgeType> Converter::wedgeMap =
        {
            std::pair<core::WedgeType, api::WedgeType>{ core::WedgeType::crescendo, api::WedgeType::crescendo },
            std::pair<core::WedgeType, api::WedgeType>{ core::WedgeType::diminuendo, api::WedgeType::diminuendo },
        };
        
        
        const std::map<core::BarStyleEnum, api::BarlineType> Converter::barlineMap =
        {
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::regular, api::BarlineType::normal },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::dotted, api::BarlineType::unsupported },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::dashed, api::BarlineType::unsupported },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::heavy, api::BarlineType::unsupported },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::lightLight, api::BarlineType::lightLight },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::lightHeavy, api::BarlineType::lightHeavy },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::heavyLight, api::BarlineType::heavyLight },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::heavyHeavy, api::BarlineType::unsupported },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::tick, api::BarlineType::unsupported },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::short_, api::BarlineType::unsupported },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::none, api::BarlineType::none },
        };
        
        
        const std::map<core::LineEnd, api::LineHook> Converter::lineStopMap =
        {
            std::pair<core::LineEnd, api::LineHook>{ core::LineEnd::arrow, api::LineHook::arrow },
            std::pair<core::LineEnd, api::LineHook>{ core::LineEnd::both, api::LineHook::both },
            std::pair<core::LineEnd, api::LineHook>{ core::LineEnd::down, api::LineHook::down },
            std::pair<core::LineEnd, api::LineHook>{ core::LineEnd::up, api::LineHook::up },
            std::pair<core::LineEnd, api::LineHook>{ core::LineEnd::none, api::LineHook::none },
        };
        
        
        const std::map<core::RightLeftMiddle, api::HorizontalAlignment> Converter::barlinePlacementMap =
        {
            std::pair<core::RightLeftMiddle, api::HorizontalAlignment>{ core::RightLeftMiddle::right, api::HorizontalAlignment::right },
            std::pair<core::RightLeftMiddle, api::HorizontalAlignment>{ core::RightLeftMiddle::left, api::HorizontalAlignment::left },
            std::pair<core::RightLeftMiddle, api::HorizontalAlignment>{ core::RightLeftMiddle::middle, api::HorizontalAlignment::center },
        };
        
        
        const std::map<core::FermataShape, api::MarkType> Converter::fermataMap =
        {
            std::pair<core::FermataShape, api::MarkType>{ core::FermataShape::normal, api::MarkType::fermataNormal },
            std::pair<core::FermataShape, api::MarkType>{ core::FermataShape::angled, api::MarkType::fermataAngled },
            std::pair<core::FermataShape, api::MarkType>{ core::FermataShape::square, api::MarkType::fermataSquare },
            std::pair<core::FermataShape, api::MarkType>{ core::FermataShape::emptystring, api::MarkType::fermata },
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
        
        
        core::CssFontSize Converter::convert( api::CssSize value ) const
        {
            return findCoreItem( cssMap, core::CssFontSize::medium, value );
        }
        
        
        api::CssSize Converter::convert( core::CssFontSize value ) const
        {
            return findApiItem( cssMap, api::CssSize::unspecified, value );
        }
        
        
        core::FontStyle Converter::convert( api::FontStyle value ) const
        {
            return findCoreItem( fontStyleMap, core::FontStyle::normal, value );
        }
        
        
        api::FontStyle Converter::convert( core::FontStyle value ) const
        {
            return findApiItem( fontStyleMap, api::FontStyle::unspecified, value );
        }
        
        
        core::FontWeight Converter::convert( api::FontWeight value ) const
        {
            return findCoreItem( fontWeightMap, core::FontWeight::normal, value );
        }
        
        
        api::FontWeight Converter::convert( core::FontWeight value ) const
        {
            return findApiItem( fontWeightMap, api::FontWeight::unspecified, value );
        }
        
        
        core::ArticulationsChoice::Choice Converter::convertArticulation( api::MarkType value ) const
        {
            return findCoreItem( articulationsMap, core::ArticulationsChoice::Choice::otherArticulation, value );
        }
        
        
        api::MarkType Converter::convertArticulation( core::ArticulationsChoice::Choice value ) const
        {
            return findApiItem( articulationsMap, api::MarkType::unspecified, value );
        }
        
        
        bool Converter::isArticulation( api::MarkType value ) const
        {
            return value == api::MarkType::accent ||
            value == api::MarkType::strongAccent ||
            value == api::MarkType::staccato ||
            value == api::MarkType::tenuto ||
            value == api::MarkType::detachedLegato ||
            value == api::MarkType::staccatissimo ||
            value == api::MarkType::spiccato ||
            value == api::MarkType::scoop ||
            value == api::MarkType::plop ||
            value == api::MarkType::doit ||
            value == api::MarkType::falloff ||
            value == api::MarkType::breathMark ||
            value == api::MarkType::caesura ||
            value == api::MarkType::stress ||
            value == api::MarkType::unstress ||
            value == api::MarkType::otherArticulation;
        }
        
        
        core::DynamicsEnum Converter::convertDynamic( api::MarkType value ) const
        {
            return findCoreItem( dynamicsMap, core::DynamicsEnum::otherDynamics, value );
        }
        
        
        api::MarkType Converter::convertDynamic( core::DynamicsEnum value ) const
        {
            return findApiItem( dynamicsMap, api::MarkType::unspecified, value );
        }

        
        core::OrnamentsChoice::Choice Converter::convertOrnament( api::MarkType value ) const
        {
            return findCoreItem( ornamentsMap, core::OrnamentsChoice::Choice::otherOrnament, value );
        }
        
        
        api::MarkType Converter::convertOrnament( core::OrnamentsChoice::Choice value ) const
        {
            return findApiItem( ornamentsMap, api::MarkType::unspecified, value );
        }

        
        core::AccidentalValue Converter::convertAccidentalMark( api::MarkType value ) const
        {
            return findCoreItem( accidentalMarkMap, core::AccidentalValue::natural, value );
        }
        
        
        api::MarkType Converter::convertAccidentalMark( core::AccidentalValue value ) const
        {
            return findApiItem( accidentalMarkMap, api::MarkType::unspecified, value );
        }
        
        
        core::TechnicalChoice::Choice Converter::convertTechnicalMark( api::MarkType value ) const
        {
            return findCoreItem( technicalMarkMap, core::TechnicalChoice::Choice::otherTechnical, value );
        }
        
        
        api::MarkType Converter::convertTechnicalMark( core::TechnicalChoice::Choice value ) const
        {
            return findApiItem( technicalMarkMap, api::MarkType::unspecified, value );
        }


        core::MeasureNumberingValue Converter::convertMeasureNumbering( api::MeasureNumbering value ) const
        {
            return findCoreItem( measureNumberingMap, core::MeasureNumberingValue::none, value );
        }


        api::MeasureNumbering Converter::convertMeasureNumbering( core::MeasureNumberingValue value ) const
        {
            return findApiItem( measureNumberingMap, api::MeasureNumbering::unspecified, value );
        }
        
        
        core::StemValue Converter::convert( api::Stem value ) const
        {
            return findCoreItem( stemMap, core::StemValue::up, value );
        }
        
        
        api::Stem Converter::convert( core::StemValue value ) const
        {
            return findApiItem( stemMap, api::Stem::unspecified, value );
        }
        
        
        core::LineType Converter::convert( api::LineType value ) const
        {
            return findCoreItem( lineType, core::LineType::solid, value );
        }
        
        
        api::LineType Converter::convert( core::LineType value ) const
        {
            return findApiItem( lineType, api::LineType::unspecified, value );
        }
        
        
        core::WedgeType Converter::convert( api::WedgeType value ) const
        {
            return findCoreItem( wedgeMap, core::WedgeType::continue_, value );
        }
        
        
        api::WedgeType Converter::convert( core::WedgeType value ) const
        {
            return findApiItem( wedgeMap, api::WedgeType::unspecified, value );
        }
        
        
        
        core::BarStyleEnum Converter::convert( api::BarlineType value ) const
        {
            return findCoreItem( barlineMap, core::BarStyleEnum::regular, value );
        }
        
        
        api::BarlineType Converter::convert( core::BarStyleEnum value ) const
        {
            return findApiItem( barlineMap, api::BarlineType::unsupported, value );
        }
        
        
        core::RightLeftMiddle Converter::convertBarlinePlacement( api::HorizontalAlignment value ) const
        {
            return findCoreItem( barlinePlacementMap, core::RightLeftMiddle::right, value );
        }
        
        
        api::HorizontalAlignment Converter::convertBarlinePlacement( core::RightLeftMiddle value ) const
        {
            return findApiItem( barlinePlacementMap, api::HorizontalAlignment::unspecified, value );
        }
        
        
        core::LineEnd Converter::convert( api::LineHook value ) const
        {
            return findCoreItem( lineStopMap, core::LineEnd::none, value );
        }
        
        
        api::LineHook Converter::convert( core::LineEnd value ) const
        {
            return findApiItem( lineStopMap, api::LineHook::unspecified, value );
        }
        
        
        core::FermataShape Converter::convertFermata( api::MarkType value ) const
        {
            return findCoreItem( fermataMap, core::FermataShape::emptystring, value );
        }
        
        
        api::MarkType Converter::convertFermata( core::FermataShape value ) const
        {
            return findApiItem( fermataMap, api::MarkType::unspecified, value );
        }
        
    }
}
