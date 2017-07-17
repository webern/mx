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
            std::pair<core::OrnamentsChoice::Choice, api::MarkType>{ core::OrnamentsChoice::Choice::tremolo, api::MarkType::tremoloSingleThree },
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
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::dotted, api::BarlineType::dotted },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::dashed, api::BarlineType::dashed },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::heavy, api::BarlineType::heavy },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::lightLight, api::BarlineType::lightLight },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::lightHeavy, api::BarlineType::lightHeavy },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::heavyLight, api::BarlineType::heavyLight },
            std::pair<core::BarStyleEnum, api::BarlineType>{ core::BarStyleEnum::heavyHeavy, api::BarlineType::heavyHeavy },
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


        const std::map<core::GroupSymbolValue, api::BracketType> Converter::bracketMap =
        {
            std::pair<core::GroupSymbolValue, api::BracketType>{ core::GroupSymbolValue::brace, api::BracketType::brace },
            std::pair<core::GroupSymbolValue, api::BracketType>{ core::GroupSymbolValue::bracket, api::BracketType::bracket },
            std::pair<core::GroupSymbolValue, api::BracketType>{ core::GroupSymbolValue::line, api::BracketType::line },
            std::pair<core::GroupSymbolValue, api::BracketType>{ core::GroupSymbolValue::none, api::BracketType::none },
            std::pair<core::GroupSymbolValue, api::BracketType>{ core::GroupSymbolValue::square, api::BracketType::square },
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


        const std::map<core::PlaybackSound, api::SoundID> Converter::instrumentMap
        {
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassAlphorn, api::SoundID::brassAlphorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassAltoHorn, api::SoundID::brassAltoHorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassBaritoneHorn, api::SoundID::brassBaritoneHorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassBugle, api::SoundID::brassBugle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassBugleAlto, api::SoundID::brassBugleAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassBugleBaritone, api::SoundID::brassBugleBaritone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassBugleContrabass, api::SoundID::brassBugleContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassBugleEuphoniumBugle, api::SoundID::brassBugleEuphoniumBugle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassBugleMellophoneBugle, api::SoundID::brassBugleMellophoneBugle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassBugleSoprano, api::SoundID::brassBugleSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassCimbasso, api::SoundID::brassCimbasso },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassConchShell, api::SoundID::brassConchShell },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassCornet, api::SoundID::brassCornet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassCornetSoprano, api::SoundID::brassCornetSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassCornett, api::SoundID::brassCornett },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassCornettTenor, api::SoundID::brassCornettTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassCornettino, api::SoundID::brassCornettino },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassDidgeridoo, api::SoundID::brassDidgeridoo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassEuphonium, api::SoundID::brassEuphonium },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassFiscorn, api::SoundID::brassFiscorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassFlugelhorn, api::SoundID::brassFlugelhorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassFrenchHorn, api::SoundID::brassFrenchHorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassGroup, api::SoundID::brassGroup },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassGroupSynth, api::SoundID::brassGroupSynth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassHelicon, api::SoundID::brassHelicon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassHoragai, api::SoundID::brassHoragai },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassKuhlohorn, api::SoundID::brassKuhlohorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassMellophone, api::SoundID::brassMellophone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassNaturalHorn, api::SoundID::brassNaturalHorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassOphicleide, api::SoundID::brassOphicleide },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassPosthorn, api::SoundID::brassPosthorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassRagDung, api::SoundID::brassRagDung },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassSackbutt, api::SoundID::brassSackbutt },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassSackbuttAlto, api::SoundID::brassSackbuttAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassSackbuttBass, api::SoundID::brassSackbuttBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassSackbuttTenor, api::SoundID::brassSackbuttTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassSaxhorn, api::SoundID::brassSaxhorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassSerpent, api::SoundID::brassSerpent },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassShofar, api::SoundID::brassShofar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassSousaphone, api::SoundID::brassSousaphone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrombone, api::SoundID::brassTrombone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTromboneAlto, api::SoundID::brassTromboneAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTromboneBass, api::SoundID::brassTromboneBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTromboneContrabass, api::SoundID::brassTromboneContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTromboneTenor, api::SoundID::brassTromboneTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpet, api::SoundID::brassTrumpet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetBaroque, api::SoundID::brassTrumpetBaroque },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetBass, api::SoundID::brassTrumpetBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetBflat, api::SoundID::brassTrumpetBflat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetC, api::SoundID::brassTrumpetC },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetD, api::SoundID::brassTrumpetD },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetPiccolo, api::SoundID::brassTrumpetPiccolo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetPocket, api::SoundID::brassTrumpetPocket },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetSlide, api::SoundID::brassTrumpetSlide },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTrumpetTenor, api::SoundID::brassTrumpetTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTuba, api::SoundID::brassTuba },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTubaBass, api::SoundID::brassTubaBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassTubaSubcontrabass, api::SoundID::brassTubaSubcontrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassViennaHorn, api::SoundID::brassViennaHorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassVuvuzela, api::SoundID::brassVuvuzela },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::brassWagnerTuba, api::SoundID::brassWagnerTuba },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumApentemma, api::SoundID::drumApentemma },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumAshiko, api::SoundID::drumAshiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumAtabaque, api::SoundID::drumAtabaque },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumAtoke, api::SoundID::drumAtoke },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumAtsimevu, api::SoundID::drumAtsimevu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumAxatse, api::SoundID::drumAxatse },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBassDrum, api::SoundID::drumBassDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBata, api::SoundID::drumBata },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBataItotele, api::SoundID::drumBataItotele },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBataIya, api::SoundID::drumBataIya },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBataOkonkolo, api::SoundID::drumBataOkonkolo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBendir, api::SoundID::drumBendir },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBodhran, api::SoundID::drumBodhran },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBombo, api::SoundID::drumBombo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBongo, api::SoundID::drumBongo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBougarabou, api::SoundID::drumBougarabou },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumBuffaloDrum, api::SoundID::drumBuffaloDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumCajon, api::SoundID::drumCajon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumChenda, api::SoundID::drumChenda },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumChuDaiko, api::SoundID::drumChuDaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumConga, api::SoundID::drumConga },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumCuica, api::SoundID::drumCuica },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDabakan, api::SoundID::drumDabakan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDaff, api::SoundID::drumDaff },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDafli, api::SoundID::drumDafli },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDaibyosi, api::SoundID::drumDaibyosi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDamroo, api::SoundID::drumDamroo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDarabuka, api::SoundID::drumDarabuka },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDef, api::SoundID::drumDef },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDhol, api::SoundID::drumDhol },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDholak, api::SoundID::drumDholak },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDjembe, api::SoundID::drumDjembe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDoira, api::SoundID::drumDoira },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDondo, api::SoundID::drumDondo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDounDounBa, api::SoundID::drumDounDounBa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDuff, api::SoundID::drumDuff },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumDumbek, api::SoundID::drumDumbek },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumFontomfrom, api::SoundID::drumFontomfrom },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumFrameDrum, api::SoundID::drumFrameDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumFrameDrumArabian, api::SoundID::drumFrameDrumArabian },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumGeduk, api::SoundID::drumGeduk },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumGhatam, api::SoundID::drumGhatam },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumGome, api::SoundID::drumGome },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumGroup, api::SoundID::drumGroup },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumGroupChinese, api::SoundID::drumGroupChinese },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumGroupEwe, api::SoundID::drumGroupEwe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumGroupIndian, api::SoundID::drumGroupIndian },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumGroupSet, api::SoundID::drumGroupSet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumHandDrum, api::SoundID::drumHandDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumHiraDaiko, api::SoundID::drumHiraDaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumIbo, api::SoundID::drumIbo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumIgihumurizo, api::SoundID::drumIgihumurizo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumInyahura, api::SoundID::drumInyahura },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumIshakwe, api::SoundID::drumIshakwe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumJangGu, api::SoundID::drumJangGu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKagan, api::SoundID::drumKagan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKakko, api::SoundID::drumKakko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKanjira, api::SoundID::drumKanjira },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKendhang, api::SoundID::drumKendhang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKendhangAgeng, api::SoundID::drumKendhangAgeng },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKendhangCiblon, api::SoundID::drumKendhangCiblon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKenkeni, api::SoundID::drumKenkeni },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKhol, api::SoundID::drumKhol },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKickDrum, api::SoundID::drumKickDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKidi, api::SoundID::drumKidi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKoDaiko, api::SoundID::drumKoDaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKpanlogo, api::SoundID::drumKpanlogo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumKudum, api::SoundID::drumKudum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumLambeg, api::SoundID::drumLambeg },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumLionDrum, api::SoundID::drumLionDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumLogDrum, api::SoundID::drumLogDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumLogDrumAfrican, api::SoundID::drumLogDrumAfrican },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumLogDrumNative, api::SoundID::drumLogDrumNative },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumLogDrumNigerian, api::SoundID::drumLogDrumNigerian },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumMadal, api::SoundID::drumMadal },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumMaddale, api::SoundID::drumMaddale },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumMridangam, api::SoundID::drumMridangam },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumNaal, api::SoundID::drumNaal },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumNagadoDaiko, api::SoundID::drumNagadoDaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumNagara, api::SoundID::drumNagara },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumNaqara, api::SoundID::drumNaqara },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumODaiko, api::SoundID::drumODaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumOkawa, api::SoundID::drumOkawa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumOkedoDaiko, api::SoundID::drumOkedoDaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumPahuHula, api::SoundID::drumPahuHula },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumPakhawaj, api::SoundID::drumPakhawaj },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumPandeiro, api::SoundID::drumPandeiro },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumPandero, api::SoundID::drumPandero },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumPowwow, api::SoundID::drumPowwow },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumPueblo, api::SoundID::drumPueblo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumRepinique, api::SoundID::drumRepinique },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumRiq, api::SoundID::drumRiq },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumRototom, api::SoundID::drumRototom },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSabar, api::SoundID::drumSabar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSakara, api::SoundID::drumSakara },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSampho, api::SoundID::drumSampho },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSangban, api::SoundID::drumSangban },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumShimeDaiko, api::SoundID::drumShimeDaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSlitDrum, api::SoundID::drumSlitDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSlitDrumKrin, api::SoundID::drumSlitDrumKrin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSnareDrum, api::SoundID::drumSnareDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSnareDrumElectric, api::SoundID::drumSnareDrumElectric },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSogo, api::SoundID::drumSogo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumSurdo, api::SoundID::drumSurdo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTabla, api::SoundID::drumTabla },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTablaBayan, api::SoundID::drumTablaBayan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTablaDayan, api::SoundID::drumTablaDayan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTaiko, api::SoundID::drumTaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTalking, api::SoundID::drumTalking },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTama, api::SoundID::drumTama },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTamborita, api::SoundID::drumTamborita },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTambourine, api::SoundID::drumTambourine },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTamte, api::SoundID::drumTamte },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTangku, api::SoundID::drumTangku },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTanTan, api::SoundID::drumTanTan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTaphon, api::SoundID::drumTaphon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTar, api::SoundID::drumTar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTasha, api::SoundID::drumTasha },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTenorDrum, api::SoundID::drumTenorDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTeponaxtli, api::SoundID::drumTeponaxtli },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumThavil, api::SoundID::drumThavil },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTheBox, api::SoundID::drumTheBox },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTimbale, api::SoundID::drumTimbale },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTimpani, api::SoundID::drumTimpani },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTinaja, api::SoundID::drumTinaja },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumToere, api::SoundID::drumToere },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTombak, api::SoundID::drumTombak },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTomTom, api::SoundID::drumTomTom },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTomTomSynth, api::SoundID::drumTomTomSynth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTsuzumi, api::SoundID::drumTsuzumi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumTumbak, api::SoundID::drumTumbak },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumUchiwaDaiko, api::SoundID::drumUchiwaDaiko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumUdaku, api::SoundID::drumUdaku },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumUdu, api::SoundID::drumUdu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::drumZarb, api::SoundID::drumZarb },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectAeolianHarp, api::SoundID::effectAeolianHarp },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectAirHorn, api::SoundID::effectAirHorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectApplause, api::SoundID::effectApplause },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectBassStringSlap, api::SoundID::effectBassStringSlap },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectBird, api::SoundID::effectBird },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectBirdNightingale, api::SoundID::effectBirdNightingale },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectBirdTweet, api::SoundID::effectBirdTweet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectBreath, api::SoundID::effectBreath },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectBubble, api::SoundID::effectBubble },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectBullroarer, api::SoundID::effectBullroarer },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectBurst, api::SoundID::effectBurst },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectCar, api::SoundID::effectCar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectCarCrash, api::SoundID::effectCarCrash },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectCarEngine, api::SoundID::effectCarEngine },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectCarPass, api::SoundID::effectCarPass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectCarStop, api::SoundID::effectCarStop },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectCrickets, api::SoundID::effectCrickets },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectDog, api::SoundID::effectDog },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectDoorCreak, api::SoundID::effectDoorCreak },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectDoorSlam, api::SoundID::effectDoorSlam },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectExplosion, api::SoundID::effectExplosion },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectFluteKeyClick, api::SoundID::effectFluteKeyClick },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectFootsteps, api::SoundID::effectFootsteps },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectFrogs, api::SoundID::effectFrogs },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectGuitarCutting, api::SoundID::effectGuitarCutting },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectGuitarFret, api::SoundID::effectGuitarFret },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectGunshot, api::SoundID::effectGunshot },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectHandClap, api::SoundID::effectHandClap },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectHeartbeat, api::SoundID::effectHeartbeat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectHelicopter, api::SoundID::effectHelicopter },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectHighQ, api::SoundID::effectHighQ },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectHorseGallop, api::SoundID::effectHorseGallop },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectJetPlane, api::SoundID::effectJetPlane },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectLaserGun, api::SoundID::effectLaserGun },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectLaugh, api::SoundID::effectLaugh },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectLionsRoar, api::SoundID::effectLionsRoar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectMachineGun, api::SoundID::effectMachineGun },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectMarchingMachine, api::SoundID::effectMarchingMachine },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectMetronomeBell, api::SoundID::effectMetronomeBell },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectMetronomeClick, api::SoundID::effectMetronomeClick },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectPat, api::SoundID::effectPat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectPunch, api::SoundID::effectPunch },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectRain, api::SoundID::effectRain },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectScratch, api::SoundID::effectScratch },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectScream, api::SoundID::effectScream },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectSeashore, api::SoundID::effectSeashore },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectSiren, api::SoundID::effectSiren },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectSlap, api::SoundID::effectSlap },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectSnap, api::SoundID::effectSnap },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectStamp, api::SoundID::effectStamp },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectStarship, api::SoundID::effectStarship },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectStream, api::SoundID::effectStream },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectTelephoneRing, api::SoundID::effectTelephoneRing },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectThunder, api::SoundID::effectThunder },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectTrain, api::SoundID::effectTrain },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectTrashCan, api::SoundID::effectTrashCan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectWhip, api::SoundID::effectWhip },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectWhistle, api::SoundID::effectWhistle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectWhistleMouthSiren, api::SoundID::effectWhistleMouthSiren },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectWhistlePolice, api::SoundID::effectWhistlePolice },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectWhistleSlide, api::SoundID::effectWhistleSlide },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectWhistleTrain, api::SoundID::effectWhistleTrain },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::effectWind, api::SoundID::effectWind },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardAccordion, api::SoundID::keyboardAccordion },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardBandoneon, api::SoundID::keyboardBandoneon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardCelesta, api::SoundID::keyboardCelesta },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardClavichord, api::SoundID::keyboardClavichord },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardClavichordSynth, api::SoundID::keyboardClavichordSynth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardConcertina, api::SoundID::keyboardConcertina },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardFortepiano, api::SoundID::keyboardFortepiano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardHarmonium, api::SoundID::keyboardHarmonium },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardHarpsichord, api::SoundID::keyboardHarpsichord },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardOndesMartenot, api::SoundID::keyboardOndesMartenot },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardOrgan, api::SoundID::keyboardOrgan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardOrganDrawbar, api::SoundID::keyboardOrganDrawbar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardOrganPercussive, api::SoundID::keyboardOrganPercussive },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardOrganPipe, api::SoundID::keyboardOrganPipe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardOrganReed, api::SoundID::keyboardOrganReed },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardOrganRotary, api::SoundID::keyboardOrganRotary },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardPiano, api::SoundID::keyboardPiano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardPianoElectric, api::SoundID::keyboardPianoElectric },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardPianoGrand, api::SoundID::keyboardPianoGrand },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardPianoHonkyTonk, api::SoundID::keyboardPianoHonkyTonk },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardPianoPrepared, api::SoundID::keyboardPianoPrepared },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardPianoToy, api::SoundID::keyboardPianoToy },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardPianoUpright, api::SoundID::keyboardPianoUpright },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::keyboardVirginal, api::SoundID::keyboardVirginal },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalAdodo, api::SoundID::metalAdodo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalAnvil, api::SoundID::metalAnvil },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBabendil, api::SoundID::metalBabendil },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsAgogo, api::SoundID::metalBellsAgogo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsAlmglocken, api::SoundID::metalBellsAlmglocken },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsBellPlate, api::SoundID::metalBellsBellPlate },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsBellTree, api::SoundID::metalBellsBellTree },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsCarillon, api::SoundID::metalBellsCarillon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsChimes, api::SoundID::metalBellsChimes },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsChimta, api::SoundID::metalBellsChimta },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsChippli, api::SoundID::metalBellsChippli },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsChurch, api::SoundID::metalBellsChurch },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsCowbell, api::SoundID::metalBellsCowbell },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsDawuro, api::SoundID::metalBellsDawuro },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsGankokwe, api::SoundID::metalBellsGankokwe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsGhungroo, api::SoundID::metalBellsGhungroo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsHatheli, api::SoundID::metalBellsHatheli },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsJingleBell, api::SoundID::metalBellsJingleBell },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsKhartal, api::SoundID::metalBellsKhartal },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsMarkTree, api::SoundID::metalBellsMarkTree },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsSistrum, api::SoundID::metalBellsSistrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsSleighBells, api::SoundID::metalBellsSleighBells },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsTemple, api::SoundID::metalBellsTemple },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsTibetan, api::SoundID::metalBellsTibetan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsTinklebell, api::SoundID::metalBellsTinklebell },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsTrychel, api::SoundID::metalBellsTrychel },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsWindChimes, api::SoundID::metalBellsWindChimes },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBellsZills, api::SoundID::metalBellsZills },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBerimbau, api::SoundID::metalBerimbau },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalBrakeDrums, api::SoundID::metalBrakeDrums },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCrotales, api::SoundID::metalCrotales },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalBo, api::SoundID::metalCymbalBo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalCengCeng, api::SoundID::metalCymbalCengCeng },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalChabara, api::SoundID::metalCymbalChabara },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalChinese, api::SoundID::metalCymbalChinese },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalChing, api::SoundID::metalCymbalChing },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalClash, api::SoundID::metalCymbalClash },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalCrash, api::SoundID::metalCymbalCrash },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalFinger, api::SoundID::metalCymbalFinger },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalHand, api::SoundID::metalCymbalHand },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalKesi, api::SoundID::metalCymbalKesi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalManjeera, api::SoundID::metalCymbalManjeera },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalReverse, api::SoundID::metalCymbalReverse },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalRide, api::SoundID::metalCymbalRide },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalSizzle, api::SoundID::metalCymbalSizzle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalSplash, api::SoundID::metalCymbalSplash },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalSuspended, api::SoundID::metalCymbalSuspended },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalTebyoshi, api::SoundID::metalCymbalTebyoshi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalTibetan, api::SoundID::metalCymbalTibetan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalCymbalTingsha, api::SoundID::metalCymbalTingsha },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalFlexatone, api::SoundID::metalFlexatone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGong, api::SoundID::metalGong },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongAgeng, api::SoundID::metalGongAgeng },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongAgung, api::SoundID::metalGongAgung },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongChanchiki, api::SoundID::metalGongChanchiki },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongChinese, api::SoundID::metalGongChinese },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongGandingan, api::SoundID::metalGongGandingan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongKempul, api::SoundID::metalGongKempul },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongKempyang, api::SoundID::metalGongKempyang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongKetuk, api::SoundID::metalGongKetuk },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongKkwenggwari, api::SoundID::metalGongKkwenggwari },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongLuo, api::SoundID::metalGongLuo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongSinging, api::SoundID::metalGongSinging },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGongThai, api::SoundID::metalGongThai },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalGuira, api::SoundID::metalGuira },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalHang, api::SoundID::metalHang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalHiHat, api::SoundID::metalHiHat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalJawHarp, api::SoundID::metalJawHarp },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalKengong, api::SoundID::metalKengong },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalMurchang, api::SoundID::metalMurchang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalMusicalSaw, api::SoundID::metalMusicalSaw },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalSingingBowl, api::SoundID::metalSingingBowl },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalSpoons, api::SoundID::metalSpoons },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalSteelDrums, api::SoundID::metalSteelDrums },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalTamtam, api::SoundID::metalTamtam },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalThundersheet, api::SoundID::metalThundersheet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalTriangle, api::SoundID::metalTriangle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::metalWashboard, api::SoundID::metalWashboard },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionAngklung, api::SoundID::pitchedPercussionAngklung },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionBalafon, api::SoundID::pitchedPercussionBalafon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionBellLyre, api::SoundID::pitchedPercussionBellLyre },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionBells, api::SoundID::pitchedPercussionBells },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionBianqing, api::SoundID::pitchedPercussionBianqing },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionBianzhong, api::SoundID::pitchedPercussionBianzhong },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionBonang, api::SoundID::pitchedPercussionBonang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionCimbalom, api::SoundID::pitchedPercussionCimbalom },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionCrystalGlasses, api::SoundID::pitchedPercussionCrystalGlasses },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionDanTamThapLuc, api::SoundID::pitchedPercussionDanTamThapLuc },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionFangxiang, api::SoundID::pitchedPercussionFangxiang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGandinganAKayo, api::SoundID::pitchedPercussionGandinganAKayo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGangsa, api::SoundID::pitchedPercussionGangsa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGender, api::SoundID::pitchedPercussionGender },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGiying, api::SoundID::pitchedPercussionGiying },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGlassHarmonica, api::SoundID::pitchedPercussionGlassHarmonica },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGlockenspiel, api::SoundID::pitchedPercussionGlockenspiel },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGlockenspielAlto, api::SoundID::pitchedPercussionGlockenspielAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGlockenspielSoprano, api::SoundID::pitchedPercussionGlockenspielSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionGyil, api::SoundID::pitchedPercussionGyil },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionHammerDulcimer, api::SoundID::pitchedPercussionHammerDulcimer },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionHandbells, api::SoundID::pitchedPercussionHandbells },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionKalimba, api::SoundID::pitchedPercussionKalimba },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionKantil, api::SoundID::pitchedPercussionKantil },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionKhim, api::SoundID::pitchedPercussionKhim },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionKulintang, api::SoundID::pitchedPercussionKulintang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionKulintangAKayo, api::SoundID::pitchedPercussionKulintangAKayo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionKulintangATiniok, api::SoundID::pitchedPercussionKulintangATiniok },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionLikembe, api::SoundID::pitchedPercussionLikembe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionLuntang, api::SoundID::pitchedPercussionLuntang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMarimba, api::SoundID::pitchedPercussionMarimba },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMarimbaBass, api::SoundID::pitchedPercussionMarimbaBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMbira, api::SoundID::pitchedPercussionMbira },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMbiraArray, api::SoundID::pitchedPercussionMbiraArray },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMetallophone, api::SoundID::pitchedPercussionMetallophone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMetallophoneAlto, api::SoundID::pitchedPercussionMetallophoneAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMetallophoneBass, api::SoundID::pitchedPercussionMetallophoneBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMetallophoneSoprano, api::SoundID::pitchedPercussionMetallophoneSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionMusicBox, api::SoundID::pitchedPercussionMusicBox },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionPelogPanerus, api::SoundID::pitchedPercussionPelogPanerus },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionPemade, api::SoundID::pitchedPercussionPemade },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionPenyacah, api::SoundID::pitchedPercussionPenyacah },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionRanatEk, api::SoundID::pitchedPercussionRanatEk },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionRanatEkLek, api::SoundID::pitchedPercussionRanatEkLek },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionRanatThum, api::SoundID::pitchedPercussionRanatThum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionRanatThumLek, api::SoundID::pitchedPercussionRanatThumLek },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionReyong, api::SoundID::pitchedPercussionReyong },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionSanza, api::SoundID::pitchedPercussionSanza },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionSaronBarung, api::SoundID::pitchedPercussionSaronBarung },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionSaronDemong, api::SoundID::pitchedPercussionSaronDemong },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionSaronPanerus, api::SoundID::pitchedPercussionSaronPanerus },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionSlendroPanerus, api::SoundID::pitchedPercussionSlendroPanerus },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionSlentem, api::SoundID::pitchedPercussionSlentem },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionTsymbaly, api::SoundID::pitchedPercussionTsymbaly },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionTubes, api::SoundID::pitchedPercussionTubes },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionTubularBells, api::SoundID::pitchedPercussionTubularBells },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionVibraphone, api::SoundID::pitchedPercussionVibraphone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionXylophone, api::SoundID::pitchedPercussionXylophone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionXylophoneAlto, api::SoundID::pitchedPercussionXylophoneAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionXylophoneBass, api::SoundID::pitchedPercussionXylophoneBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionXylophoneSoprano, api::SoundID::pitchedPercussionXylophoneSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionXylorimba, api::SoundID::pitchedPercussionXylorimba },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pitchedPercussionYangqin, api::SoundID::pitchedPercussionYangqin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckArchlute, api::SoundID::pluckArchlute },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckAutoharp, api::SoundID::pluckAutoharp },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBaglama, api::SoundID::pluckBaglama },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBajo, api::SoundID::pluckBajo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBalalaika, api::SoundID::pluckBalalaika },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBalalaikaAlto, api::SoundID::pluckBalalaikaAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBalalaikaBass, api::SoundID::pluckBalalaikaBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBalalaikaContrabass, api::SoundID::pluckBalalaikaContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBalalaikaPiccolo, api::SoundID::pluckBalalaikaPiccolo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBalalaikaPrima, api::SoundID::pluckBalalaikaPrima },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBalalaikaSecunda, api::SoundID::pluckBalalaikaSecunda },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBandola, api::SoundID::pluckBandola },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBandura, api::SoundID::pluckBandura },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBandurria, api::SoundID::pluckBandurria },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBanjo, api::SoundID::pluckBanjo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBanjoTenor, api::SoundID::pluckBanjoTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBanjolele, api::SoundID::pluckBanjolele },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBarbat, api::SoundID::pluckBarbat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBass, api::SoundID::pluckBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassAcoustic, api::SoundID::pluckBassAcoustic },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassBolon, api::SoundID::pluckBassBolon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassElectric, api::SoundID::pluckBassElectric },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassFretless, api::SoundID::pluckBassFretless },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassGuitarron, api::SoundID::pluckBassGuitarron },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassSynth, api::SoundID::pluckBassSynth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassSynthLead, api::SoundID::pluckBassSynthLead },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassWashtub, api::SoundID::pluckBassWashtub },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBassWhamola, api::SoundID::pluckBassWhamola },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBegena, api::SoundID::pluckBegena },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBiwa, api::SoundID::pluckBiwa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBordonua, api::SoundID::pluckBordonua },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBouzouki, api::SoundID::pluckBouzouki },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckBouzoukiIrish, api::SoundID::pluckBouzoukiIrish },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckCelticHarp, api::SoundID::pluckCelticHarp },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckCharango, api::SoundID::pluckCharango },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckChitarraBattente, api::SoundID::pluckChitarraBattente },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckCithara, api::SoundID::pluckCithara },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckCittern, api::SoundID::pluckCittern },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckCuatro, api::SoundID::pluckCuatro },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDanBau, api::SoundID::pluckDanBau },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDanNguyet, api::SoundID::pluckDanNguyet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDanTranh, api::SoundID::pluckDanTranh },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDanTyBa, api::SoundID::pluckDanTyBa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDiddleyBow, api::SoundID::pluckDiddleyBow },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDomra, api::SoundID::pluckDomra },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDomu, api::SoundID::pluckDomu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDulcimer, api::SoundID::pluckDulcimer },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDutar, api::SoundID::pluckDutar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckDuxianqin, api::SoundID::pluckDuxianqin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckEktara, api::SoundID::pluckEktara },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGeomungo, api::SoundID::pluckGeomungo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGottuvadhyam, api::SoundID::pluckGottuvadhyam },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitar, api::SoundID::pluckGuitar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitarAcoustic, api::SoundID::pluckGuitarAcoustic },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitarElectric, api::SoundID::pluckGuitarElectric },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitarNylonString, api::SoundID::pluckGuitarNylonString },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitarPedalSteel, api::SoundID::pluckGuitarPedalSteel },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitarPortuguese, api::SoundID::pluckGuitarPortuguese },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitarRequinto, api::SoundID::pluckGuitarRequinto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitarResonator, api::SoundID::pluckGuitarResonator },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitarSteelString, api::SoundID::pluckGuitarSteelString },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitjo, api::SoundID::pluckGuitjo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuitjoDoubleNeck, api::SoundID::pluckGuitjoDoubleNeck },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuqin, api::SoundID::pluckGuqin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuzheng, api::SoundID::pluckGuzheng },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckGuzhengChoazhou, api::SoundID::pluckGuzhengChoazhou },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckHarp, api::SoundID::pluckHarp },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckHarpGuitar, api::SoundID::pluckHarpGuitar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckHuapanguera, api::SoundID::pluckHuapanguera },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckJaranaHuasteca, api::SoundID::pluckJaranaHuasteca },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckJaranaJarocha, api::SoundID::pluckJaranaJarocha },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckJaranaJarochaMosquito, api::SoundID::pluckJaranaJarochaMosquito },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckJaranaJarochaPrimera, api::SoundID::pluckJaranaJarochaPrimera },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckJaranaJarochaSegunda, api::SoundID::pluckJaranaJarochaSegunda },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckJaranaJarochaTercera, api::SoundID::pluckJaranaJarochaTercera },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKabosy, api::SoundID::pluckKabosy },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKantele, api::SoundID::pluckKantele },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKanun, api::SoundID::pluckKanun },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKayagum, api::SoundID::pluckKayagum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKobza, api::SoundID::pluckKobza },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKomuz, api::SoundID::pluckKomuz },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKora, api::SoundID::pluckKora },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKoto, api::SoundID::pluckKoto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckKutiyapi, api::SoundID::pluckKutiyapi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckLangeleik, api::SoundID::pluckLangeleik },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckLaud, api::SoundID::pluckLaud },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckLute, api::SoundID::pluckLute },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckLyre, api::SoundID::pluckLyre },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMandobass, api::SoundID::pluckMandobass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMandocello, api::SoundID::pluckMandocello },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMandola, api::SoundID::pluckMandola },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMandolin, api::SoundID::pluckMandolin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMandolinOctave, api::SoundID::pluckMandolinOctave },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMandora, api::SoundID::pluckMandora },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMandore, api::SoundID::pluckMandore },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMarovany, api::SoundID::pluckMarovany },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckMusicalBow, api::SoundID::pluckMusicalBow },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckNgoni, api::SoundID::pluckNgoni },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckOud, api::SoundID::pluckOud },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckPipa, api::SoundID::pluckPipa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckPsaltery, api::SoundID::pluckPsaltery },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckRuan, api::SoundID::pluckRuan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSallaneh, api::SoundID::pluckSallaneh },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSanshin, api::SoundID::pluckSanshin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSantoor, api::SoundID::pluckSantoor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSanxian, api::SoundID::pluckSanxian },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSarod, api::SoundID::pluckSarod },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSaung, api::SoundID::pluckSaung },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSaz, api::SoundID::pluckSaz },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSe, api::SoundID::pluckSe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSetar, api::SoundID::pluckSetar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckShamisen, api::SoundID::pluckShamisen },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSitar, api::SoundID::pluckSitar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSynth, api::SoundID::pluckSynth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSynthCharang, api::SoundID::pluckSynthCharang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSynthChiff, api::SoundID::pluckSynthChiff },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckSynthStick, api::SoundID::pluckSynthStick },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTambura, api::SoundID::pluckTambura },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTamburaBulgarian, api::SoundID::pluckTamburaBulgarian },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTamburaFemale, api::SoundID::pluckTamburaFemale },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTamburaMale, api::SoundID::pluckTamburaMale },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTar, api::SoundID::pluckTar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTheorbo, api::SoundID::pluckTheorbo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTimple, api::SoundID::pluckTimple },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTiple, api::SoundID::pluckTiple },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckTres, api::SoundID::pluckTres },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckUkulele, api::SoundID::pluckUkulele },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckUkuleleTenor, api::SoundID::pluckUkuleleTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckValiha, api::SoundID::pluckValiha },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckVeena, api::SoundID::pluckVeena },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckVeenaMohan, api::SoundID::pluckVeenaMohan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckVeenaRudra, api::SoundID::pluckVeenaRudra },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckVeenaVichitra, api::SoundID::pluckVeenaVichitra },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckVihuela, api::SoundID::pluckVihuela },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckVihuelaMexican, api::SoundID::pluckVihuelaMexican },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckXalam, api::SoundID::pluckXalam },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckYueqin, api::SoundID::pluckYueqin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckZither, api::SoundID::pluckZither },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::pluckZitherOvertone, api::SoundID::pluckZitherOvertone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleAfoxe, api::SoundID::rattleAfoxe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleBirds, api::SoundID::rattleBirds },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleCabasa, api::SoundID::rattleCabasa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleCaxixi, api::SoundID::rattleCaxixi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleCog, api::SoundID::rattleCog },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleGanza, api::SoundID::rattleGanza },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleHosho, api::SoundID::rattleHosho },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleJawbone, api::SoundID::rattleJawbone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleKayamba, api::SoundID::rattleKayamba },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleKpokoKpoko, api::SoundID::rattleKpokoKpoko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleLavaStones, api::SoundID::rattleLavaStones },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleMaraca, api::SoundID::rattleMaraca },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleRainStick, api::SoundID::rattleRainStick },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleRatchet, api::SoundID::rattleRatchet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleRattle, api::SoundID::rattleRattle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleShaker, api::SoundID::rattleShaker },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleShakerEgg, api::SoundID::rattleShakerEgg },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleShekere, api::SoundID::rattleShekere },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleSistre, api::SoundID::rattleSistre },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleTelevi, api::SoundID::rattleTelevi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleVibraslap, api::SoundID::rattleVibraslap },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::rattleWasembe, api::SoundID::rattleWasembe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsAjaeng, api::SoundID::stringsAjaeng },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsArpeggione, api::SoundID::stringsArpeggione },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsBaryton, api::SoundID::stringsBaryton },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsCello, api::SoundID::stringsCello },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsCelloPiccolo, api::SoundID::stringsCelloPiccolo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsContrabass, api::SoundID::stringsContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsCrwth, api::SoundID::stringsCrwth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsDanGao, api::SoundID::stringsDanGao },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsDihu, api::SoundID::stringsDihu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsErhu, api::SoundID::stringsErhu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsErxian, api::SoundID::stringsErxian },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsEsraj, api::SoundID::stringsEsraj },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsFiddle, api::SoundID::stringsFiddle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsFiddleHardanger, api::SoundID::stringsFiddleHardanger },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsGadulka, api::SoundID::stringsGadulka },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsGaohu, api::SoundID::stringsGaohu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsGehu, api::SoundID::stringsGehu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsGroup, api::SoundID::stringsGroup },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsGroupSynth, api::SoundID::stringsGroupSynth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsHaegeum, api::SoundID::stringsHaegeum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsHurdyGurdy, api::SoundID::stringsHurdyGurdy },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsIgil, api::SoundID::stringsIgil },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsKamancha, api::SoundID::stringsKamancha },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsKokyu, api::SoundID::stringsKokyu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsLaruan, api::SoundID::stringsLaruan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsLeiqin, api::SoundID::stringsLeiqin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsLirone, api::SoundID::stringsLirone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsLyraByzantine, api::SoundID::stringsLyraByzantine },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsLyraCretan, api::SoundID::stringsLyraCretan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsMorinKhuur, api::SoundID::stringsMorinKhuur },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsNyckelharpa, api::SoundID::stringsNyckelharpa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsOctobass, api::SoundID::stringsOctobass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsRebab, api::SoundID::stringsRebab },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsRebec, api::SoundID::stringsRebec },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsSarangi, api::SoundID::stringsSarangi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsStrohViolin, api::SoundID::stringsStrohViolin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsTrombaMarina, api::SoundID::stringsTrombaMarina },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsVielle, api::SoundID::stringsVielle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViol, api::SoundID::stringsViol },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolAlto, api::SoundID::stringsViolAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolBass, api::SoundID::stringsViolBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolTenor, api::SoundID::stringsViolTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolTreble, api::SoundID::stringsViolTreble },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolViolone, api::SoundID::stringsViolViolone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViola, api::SoundID::stringsViola },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolaDamore, api::SoundID::stringsViolaDamore },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolin, api::SoundID::stringsViolin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolonoPiccolo, api::SoundID::stringsViolonoPiccolo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsViolotta, api::SoundID::stringsViolotta },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsYayliTanbur, api::SoundID::stringsYayliTanbur },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsYazheng, api::SoundID::stringsYazheng },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::stringsZhonghu, api::SoundID::stringsZhonghu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffects, api::SoundID::synthEffects },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffectsAtmosphere, api::SoundID::synthEffectsAtmosphere },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffectsBrightness, api::SoundID::synthEffectsBrightness },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffectsCrystal, api::SoundID::synthEffectsCrystal },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffectsEchoes, api::SoundID::synthEffectsEchoes },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffectsGoblins, api::SoundID::synthEffectsGoblins },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffectsRain, api::SoundID::synthEffectsRain },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffectsSciFi, api::SoundID::synthEffectsSciFi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthEffectsSoundtrack, api::SoundID::synthEffectsSoundtrack },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthGroup, api::SoundID::synthGroup },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthGroupFifths, api::SoundID::synthGroupFifths },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthGroupOrchestra, api::SoundID::synthGroupOrchestra },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthPad, api::SoundID::synthPad },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthPadBowed, api::SoundID::synthPadBowed },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthPadChoir, api::SoundID::synthPadChoir },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthPadHalo, api::SoundID::synthPadHalo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthPadMetallic, api::SoundID::synthPadMetallic },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthPadPolysynth, api::SoundID::synthPadPolysynth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthPadSweep, api::SoundID::synthPadSweep },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthPadWarm, api::SoundID::synthPadWarm },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthTheremin, api::SoundID::synthTheremin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthToneSawtooth, api::SoundID::synthToneSawtooth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthToneSine, api::SoundID::synthToneSine },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::synthToneSquare, api::SoundID::synthToneSquare },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceAa, api::SoundID::voiceAa },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceAlto, api::SoundID::voiceAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceAw, api::SoundID::voiceAw },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceBaritone, api::SoundID::voiceBaritone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceBass, api::SoundID::voiceBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceChild, api::SoundID::voiceChild },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceCountertenor, api::SoundID::voiceCountertenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceDoo, api::SoundID::voiceDoo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceEe, api::SoundID::voiceEe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceFemale, api::SoundID::voiceFemale },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceKazoo, api::SoundID::voiceKazoo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceMale, api::SoundID::voiceMale },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceMezzoSoprano, api::SoundID::voiceMezzoSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceMm, api::SoundID::voiceMm },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceOo, api::SoundID::voiceOo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voicePercussion, api::SoundID::voicePercussion },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voicePercussionBeatbox, api::SoundID::voicePercussionBeatbox },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceSoprano, api::SoundID::voiceSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceSynth, api::SoundID::voiceSynth },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceTalkBox, api::SoundID::voiceTalkBox },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceTenor, api::SoundID::voiceTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::voiceVocals, api::SoundID::voiceVocals },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesBansuri, api::SoundID::windFlutesBansuri },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesBlownBottle, api::SoundID::windFlutesBlownBottle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesCalliope, api::SoundID::windFlutesCalliope },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesDanso, api::SoundID::windFlutesDanso },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesDiZi, api::SoundID::windFlutesDiZi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesDvojnice, api::SoundID::windFlutesDvojnice },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFife, api::SoundID::windFlutesFife },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFlageolet, api::SoundID::windFlutesFlageolet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFlute, api::SoundID::windFlutesFlute },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFluteAlto, api::SoundID::windFlutesFluteAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFluteBass, api::SoundID::windFlutesFluteBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFluteContraAlto, api::SoundID::windFlutesFluteContraAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFluteContrabass, api::SoundID::windFlutesFluteContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFluteDoubleContrabass, api::SoundID::windFlutesFluteDoubleContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFluteIrish, api::SoundID::windFlutesFluteIrish },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFlutePiccolo, api::SoundID::windFlutesFlutePiccolo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFluteSubcontrabass, api::SoundID::windFlutesFluteSubcontrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesFujara, api::SoundID::windFlutesFujara },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesGemshorn, api::SoundID::windFlutesGemshorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesHocchiku, api::SoundID::windFlutesHocchiku },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesHun, api::SoundID::windFlutesHun },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesKaval, api::SoundID::windFlutesKaval },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesKawala, api::SoundID::windFlutesKawala },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesKhlui, api::SoundID::windFlutesKhlui },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesKnotweed, api::SoundID::windFlutesKnotweed },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesKoncovkaAlto, api::SoundID::windFlutesKoncovkaAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesKoudi, api::SoundID::windFlutesKoudi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesNey, api::SoundID::windFlutesNey },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesNohkan, api::SoundID::windFlutesNohkan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesNose, api::SoundID::windFlutesNose },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesOcarina, api::SoundID::windFlutesOcarina },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesOvertoneTenor, api::SoundID::windFlutesOvertoneTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesPalendag, api::SoundID::windFlutesPalendag },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesPanpipes, api::SoundID::windFlutesPanpipes },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesQuena, api::SoundID::windFlutesQuena },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorder, api::SoundID::windFlutesRecorder },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderAlto, api::SoundID::windFlutesRecorderAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderBass, api::SoundID::windFlutesRecorderBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderContrabass, api::SoundID::windFlutesRecorderContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderDescant, api::SoundID::windFlutesRecorderDescant },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderGarklein, api::SoundID::windFlutesRecorderGarklein },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderGreatBass, api::SoundID::windFlutesRecorderGreatBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderSopranino, api::SoundID::windFlutesRecorderSopranino },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderSoprano, api::SoundID::windFlutesRecorderSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRecorderTenor, api::SoundID::windFlutesRecorderTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesRyuteki, api::SoundID::windFlutesRyuteki },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesShakuhachi, api::SoundID::windFlutesShakuhachi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesShepherdsPipe, api::SoundID::windFlutesShepherdsPipe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesShinobue, api::SoundID::windFlutesShinobue },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesShvi, api::SoundID::windFlutesShvi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesSuling, api::SoundID::windFlutesSuling },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesTarka, api::SoundID::windFlutesTarka },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesTumpong, api::SoundID::windFlutesTumpong },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesVenu, api::SoundID::windFlutesVenu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesWhistle, api::SoundID::windFlutesWhistle },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesWhistleAlto, api::SoundID::windFlutesWhistleAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesWhistleLowIrish, api::SoundID::windFlutesWhistleLowIrish },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesWhistleShiva, api::SoundID::windFlutesWhistleShiva },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesWhistleSlide, api::SoundID::windFlutesWhistleSlide },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesWhistleTin, api::SoundID::windFlutesWhistleTin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesWhistleTinBflat, api::SoundID::windFlutesWhistleTinBflat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesWhistleTinD, api::SoundID::windFlutesWhistleTinD },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesXiao, api::SoundID::windFlutesXiao },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windFlutesXun, api::SoundID::windFlutesXun },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windGroup, api::SoundID::windGroup },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windJug, api::SoundID::windJug },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windPipesBagpipes, api::SoundID::windPipesBagpipes },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windPipesGaida, api::SoundID::windPipesGaida },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windPipesHighland, api::SoundID::windPipesHighland },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windPipesUilleann, api::SoundID::windPipesUilleann },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windPungi, api::SoundID::windPungi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedAlbogue, api::SoundID::windReedAlbogue },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedAlboka, api::SoundID::windReedAlboka },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedAlgaita, api::SoundID::windReedAlgaita },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedArghul, api::SoundID::windReedArghul },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedBassetHorn, api::SoundID::windReedBassetHorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedBassoon, api::SoundID::windReedBassoon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedBawu, api::SoundID::windReedBawu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedBifora, api::SoundID::windReedBifora },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedBombarde, api::SoundID::windReedBombarde },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedChalumeau, api::SoundID::windReedChalumeau },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinet, api::SoundID::windReedClarinet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetA, api::SoundID::windReedClarinetA },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetAlto, api::SoundID::windReedClarinetAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetBass, api::SoundID::windReedClarinetBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetBasset, api::SoundID::windReedClarinetBasset },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetBflat, api::SoundID::windReedClarinetBflat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetContraAlto, api::SoundID::windReedClarinetContraAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetContrabass, api::SoundID::windReedClarinetContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetEflat, api::SoundID::windReedClarinetEflat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetPiccoloAflat, api::SoundID::windReedClarinetPiccoloAflat },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedClarinetteDamour, api::SoundID::windReedClarinetteDamour },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedContrabass, api::SoundID::windReedContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedContrabassoon, api::SoundID::windReedContrabassoon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedCornamuse, api::SoundID::windReedCornamuse },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedCromorne, api::SoundID::windReedCromorne },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedCrumhorn, api::SoundID::windReedCrumhorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedCrumhornAlto, api::SoundID::windReedCrumhornAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedCrumhornBass, api::SoundID::windReedCrumhornBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedCrumhornGreatBass, api::SoundID::windReedCrumhornGreatBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedCrumhornSoprano, api::SoundID::windReedCrumhornSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedCrumhornTenor, api::SoundID::windReedCrumhornTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedDiple, api::SoundID::windReedDiple },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedDiplica, api::SoundID::windReedDiplica },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedDuduk, api::SoundID::windReedDuduk },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedDulcian, api::SoundID::windReedDulcian },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedDulzaina, api::SoundID::windReedDulzaina },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedEnglishHorn, api::SoundID::windReedEnglishHorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedGuanzi, api::SoundID::windReedGuanzi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHarmonica, api::SoundID::windReedHarmonica },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHarmonicaBass, api::SoundID::windReedHarmonicaBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHeckelClarina, api::SoundID::windReedHeckelClarina },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHeckelphone, api::SoundID::windReedHeckelphone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHeckelphonePiccolo, api::SoundID::windReedHeckelphonePiccolo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHeckelphoneClarinet, api::SoundID::windReedHeckelphoneClarinet },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHichiriki, api::SoundID::windReedHichiriki },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHirtenschalmei, api::SoundID::windReedHirtenschalmei },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHne, api::SoundID::windReedHne },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHornpipe, api::SoundID::windReedHornpipe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHouguan, api::SoundID::windReedHouguan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedHulusi, api::SoundID::windReedHulusi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedJogiBaja, api::SoundID::windReedJogiBaja },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedKenBau, api::SoundID::windReedKenBau },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedKhaenMouthOrgan, api::SoundID::windReedKhaenMouthOrgan },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedLauneddas, api::SoundID::windReedLauneddas },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedMaqrunah, api::SoundID::windReedMaqrunah },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedMelodica, api::SoundID::windReedMelodica },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedMijwiz, api::SoundID::windReedMijwiz },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedMizmar, api::SoundID::windReedMizmar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedNadaswaram, api::SoundID::windReedNadaswaram },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedOboe, api::SoundID::windReedOboe },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedOboeBass, api::SoundID::windReedOboeBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedOboePiccolo, api::SoundID::windReedOboePiccolo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedOboeDaCaccia, api::SoundID::windReedOboeDaCaccia },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedOboeDamore, api::SoundID::windReedOboeDamore },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedOctavin, api::SoundID::windReedOctavin },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedPi, api::SoundID::windReedPi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedPibgorn, api::SoundID::windReedPibgorn },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedPiri, api::SoundID::windReedPiri },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedRackett, api::SoundID::windReedRackett },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedRauschpfeife, api::SoundID::windReedRauschpfeife },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedRhaita, api::SoundID::windReedRhaita },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedRothphone, api::SoundID::windReedRothphone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSarrusaphone, api::SoundID::windReedSarrusaphone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxonette, api::SoundID::windReedSaxonette },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophone, api::SoundID::windReedSaxophone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneAlto, api::SoundID::windReedSaxophoneAlto },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneAulochrome, api::SoundID::windReedSaxophoneAulochrome },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneBaritone, api::SoundID::windReedSaxophoneBaritone },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneBass, api::SoundID::windReedSaxophoneBass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneContrabass, api::SoundID::windReedSaxophoneContrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneMelody, api::SoundID::windReedSaxophoneMelody },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneMezzoSoprano, api::SoundID::windReedSaxophoneMezzoSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneSopranino, api::SoundID::windReedSaxophoneSopranino },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneSopranissimo, api::SoundID::windReedSaxophoneSopranissimo },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneSoprano, api::SoundID::windReedSaxophoneSoprano },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneSubcontrabass, api::SoundID::windReedSaxophoneSubcontrabass },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSaxophoneTenor, api::SoundID::windReedSaxophoneTenor },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedShawm, api::SoundID::windReedShawm },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedShenai, api::SoundID::windReedShenai },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSheng, api::SoundID::windReedSheng },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSipsi, api::SoundID::windReedSipsi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSopila, api::SoundID::windReedSopila },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSorna, api::SoundID::windReedSorna },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSralai, api::SoundID::windReedSralai },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSuona, api::SoundID::windReedSuona },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedSurnai, api::SoundID::windReedSurnai },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedTaepyeongso, api::SoundID::windReedTaepyeongso },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedTarogato, api::SoundID::windReedTarogato },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedTarogatoAncient, api::SoundID::windReedTarogatoAncient },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedTrompetaChina, api::SoundID::windReedTrompetaChina },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedTubax, api::SoundID::windReedTubax },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedXaphoon, api::SoundID::windReedXaphoon },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedZhaleika, api::SoundID::windReedZhaleika },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedZurla, api::SoundID::windReedZurla },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::windReedZurna, api::SoundID::windReedZurna },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodAgogoBlock, api::SoundID::woodAgogoBlock },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodAgungATamlang, api::SoundID::woodAgungATamlang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodAhoko, api::SoundID::woodAhoko },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodBones, api::SoundID::woodBones },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodCastanets, api::SoundID::woodCastanets },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodClaves, api::SoundID::woodClaves },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodDrumSticks, api::SoundID::woodDrumSticks },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodGourd, api::SoundID::woodGourd },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodGraniteBlock, api::SoundID::woodGraniteBlock },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodGuban, api::SoundID::woodGuban },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodGuiro, api::SoundID::woodGuiro },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodHyoushigi, api::SoundID::woodHyoushigi },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodIpu, api::SoundID::woodIpu },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodJamBlock, api::SoundID::woodJamBlock },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodKaekeeke, api::SoundID::woodKaekeeke },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodKagul, api::SoundID::woodKagul },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodKalaau, api::SoundID::woodKalaau },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodKashiklar, api::SoundID::woodKashiklar },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodKubing, api::SoundID::woodKubing },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodPanClappers, api::SoundID::woodPanClappers },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodSandBlock, api::SoundID::woodSandBlock },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodSlapstick, api::SoundID::woodSlapstick },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodStirDrum, api::SoundID::woodStirDrum },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodTempleBlock, api::SoundID::woodTempleBlock },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodTicTocBlock, api::SoundID::woodTicTocBlock },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodTonetang, api::SoundID::woodTonetang },
            std::pair<core::PlaybackSound, api::SoundID>{ core::PlaybackSound::woodWoodBlock, api::SoundID::woodWoodBlock },
        };


        const std::map<core::KindValue, api::ChordKind> Converter::kindMap
        {
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::major, api::ChordKind::major },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::minor, api::ChordKind::minor },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::augmented, api::ChordKind::augmented },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::diminished, api::ChordKind::diminished },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::dominant, api::ChordKind::dominant },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::majorSeventh, api::ChordKind::majorSeventh },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::minorSeventh, api::ChordKind::minorSeventh },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::diminishedSeventh, api::ChordKind::diminishedSeventh },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::augmentedSeventh, api::ChordKind::augmentedSeventh },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::halfDiminished, api::ChordKind::halfDiminished },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::majorMinor, api::ChordKind::majorMinor },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::majorSixth, api::ChordKind::majorSixth },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::minorSixth, api::ChordKind::minorSixth },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::dominantNinth, api::ChordKind::dominantNinth },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::majorNinth, api::ChordKind::majorNinth },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::minorNinth, api::ChordKind::minorNinth },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::dominant11Th, api::ChordKind::dominant11Th },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::major11Th, api::ChordKind::major11Th },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::minor11Th, api::ChordKind::minor11Th },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::dominant13Th, api::ChordKind::dominant13Th },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::major13Th, api::ChordKind::major13Th },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::minor13Th, api::ChordKind::minor13Th },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::suspendedSecond, api::ChordKind::suspendedSecond },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::suspendedFourth, api::ChordKind::suspendedFourth },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::neapolitan, api::ChordKind::neapolitan },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::italian, api::ChordKind::italian },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::french, api::ChordKind::french },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::german, api::ChordKind::german },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::pedal, api::ChordKind::pedal },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::power, api::ChordKind::power },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::tristan, api::ChordKind::tristan },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::other, api::ChordKind::other },
            std::pair<core::KindValue, api::ChordKind>{ core::KindValue::none, api::ChordKind::none },
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


        core::GroupSymbolValue Converter::convert( api::BracketType value ) const
        {
            return findCoreItem( bracketMap, core::GroupSymbolValue::none, value );
        }


        api::BracketType Converter::convert( core::GroupSymbolValue value ) const
        {
            return findApiItem( bracketMap, api::BracketType::unspecified, value );
        }
        
        
        core::FermataShape Converter::convertFermata( api::MarkType value ) const
        {
            return findCoreItem( fermataMap, core::FermataShape::emptystring, value );
        }
        
        
        api::MarkType Converter::convertFermata( core::FermataShape value ) const
        {
            return findApiItem( fermataMap, api::MarkType::unspecified, value );
        }


        core::PlaybackSound Converter::convert( api::SoundID value ) const
        {
            return findCoreItem( instrumentMap, core::PlaybackSound::keyboardPiano, value );
        }


        api::SoundID Converter::convert( core::PlaybackSound value ) const
        {
            return findApiItem( instrumentMap, api::SoundID::unspecified, value );
        }


        core::KindValue Converter::convert( api::ChordKind value ) const
        {
            return findCoreItem( kindMap, core::KindValue::none, value );
        }


        api::ChordKind Converter::convert( core::KindValue value ) const
        {
            return findApiItem( kindMap, api::ChordKind::unspecified, value );
        }
        
    }
}
