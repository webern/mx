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


        const std::map<core::PlaybackSound, api::InstrumentSound> Converter::instrumentMap
        {
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassAlphorn, api::InstrumentSound::brassAlphorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassAltoHorn, api::InstrumentSound::brassAltoHorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassBaritoneHorn, api::InstrumentSound::brassBaritoneHorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassBugle, api::InstrumentSound::brassBugle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassBugleAlto, api::InstrumentSound::brassBugleAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassBugleBaritone, api::InstrumentSound::brassBugleBaritone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassBugleContrabass, api::InstrumentSound::brassBugleContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassBugleEuphoniumBugle, api::InstrumentSound::brassBugleEuphoniumBugle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassBugleMellophoneBugle, api::InstrumentSound::brassBugleMellophoneBugle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassBugleSoprano, api::InstrumentSound::brassBugleSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassCimbasso, api::InstrumentSound::brassCimbasso },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassConchShell, api::InstrumentSound::brassConchShell },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassCornet, api::InstrumentSound::brassCornet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassCornetSoprano, api::InstrumentSound::brassCornetSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassCornett, api::InstrumentSound::brassCornett },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassCornettTenor, api::InstrumentSound::brassCornettTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassCornettino, api::InstrumentSound::brassCornettino },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassDidgeridoo, api::InstrumentSound::brassDidgeridoo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassEuphonium, api::InstrumentSound::brassEuphonium },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassFiscorn, api::InstrumentSound::brassFiscorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassFlugelhorn, api::InstrumentSound::brassFlugelhorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassFrenchHorn, api::InstrumentSound::brassFrenchHorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassGroup, api::InstrumentSound::brassGroup },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassGroupSynth, api::InstrumentSound::brassGroupSynth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassHelicon, api::InstrumentSound::brassHelicon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassHoragai, api::InstrumentSound::brassHoragai },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassKuhlohorn, api::InstrumentSound::brassKuhlohorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassMellophone, api::InstrumentSound::brassMellophone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassNaturalHorn, api::InstrumentSound::brassNaturalHorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassOphicleide, api::InstrumentSound::brassOphicleide },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassPosthorn, api::InstrumentSound::brassPosthorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassRagDung, api::InstrumentSound::brassRagDung },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassSackbutt, api::InstrumentSound::brassSackbutt },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassSackbuttAlto, api::InstrumentSound::brassSackbuttAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassSackbuttBass, api::InstrumentSound::brassSackbuttBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassSackbuttTenor, api::InstrumentSound::brassSackbuttTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassSaxhorn, api::InstrumentSound::brassSaxhorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassSerpent, api::InstrumentSound::brassSerpent },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassShofar, api::InstrumentSound::brassShofar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassSousaphone, api::InstrumentSound::brassSousaphone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrombone, api::InstrumentSound::brassTrombone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTromboneAlto, api::InstrumentSound::brassTromboneAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTromboneBass, api::InstrumentSound::brassTromboneBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTromboneContrabass, api::InstrumentSound::brassTromboneContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTromboneTenor, api::InstrumentSound::brassTromboneTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpet, api::InstrumentSound::brassTrumpet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetBaroque, api::InstrumentSound::brassTrumpetBaroque },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetBass, api::InstrumentSound::brassTrumpetBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetBflat, api::InstrumentSound::brassTrumpetBflat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetC, api::InstrumentSound::brassTrumpetC },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetD, api::InstrumentSound::brassTrumpetD },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetPiccolo, api::InstrumentSound::brassTrumpetPiccolo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetPocket, api::InstrumentSound::brassTrumpetPocket },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetSlide, api::InstrumentSound::brassTrumpetSlide },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTrumpetTenor, api::InstrumentSound::brassTrumpetTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTuba, api::InstrumentSound::brassTuba },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTubaBass, api::InstrumentSound::brassTubaBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassTubaSubcontrabass, api::InstrumentSound::brassTubaSubcontrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassViennaHorn, api::InstrumentSound::brassViennaHorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassVuvuzela, api::InstrumentSound::brassVuvuzela },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::brassWagnerTuba, api::InstrumentSound::brassWagnerTuba },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumApentemma, api::InstrumentSound::drumApentemma },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumAshiko, api::InstrumentSound::drumAshiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumAtabaque, api::InstrumentSound::drumAtabaque },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumAtoke, api::InstrumentSound::drumAtoke },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumAtsimevu, api::InstrumentSound::drumAtsimevu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumAxatse, api::InstrumentSound::drumAxatse },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBassDrum, api::InstrumentSound::drumBassDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBata, api::InstrumentSound::drumBata },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBataItotele, api::InstrumentSound::drumBataItotele },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBataIya, api::InstrumentSound::drumBataIya },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBataOkonkolo, api::InstrumentSound::drumBataOkonkolo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBendir, api::InstrumentSound::drumBendir },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBodhran, api::InstrumentSound::drumBodhran },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBombo, api::InstrumentSound::drumBombo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBongo, api::InstrumentSound::drumBongo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBougarabou, api::InstrumentSound::drumBougarabou },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumBuffaloDrum, api::InstrumentSound::drumBuffaloDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumCajon, api::InstrumentSound::drumCajon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumChenda, api::InstrumentSound::drumChenda },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumChuDaiko, api::InstrumentSound::drumChuDaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumConga, api::InstrumentSound::drumConga },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumCuica, api::InstrumentSound::drumCuica },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDabakan, api::InstrumentSound::drumDabakan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDaff, api::InstrumentSound::drumDaff },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDafli, api::InstrumentSound::drumDafli },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDaibyosi, api::InstrumentSound::drumDaibyosi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDamroo, api::InstrumentSound::drumDamroo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDarabuka, api::InstrumentSound::drumDarabuka },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDef, api::InstrumentSound::drumDef },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDhol, api::InstrumentSound::drumDhol },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDholak, api::InstrumentSound::drumDholak },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDjembe, api::InstrumentSound::drumDjembe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDoira, api::InstrumentSound::drumDoira },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDondo, api::InstrumentSound::drumDondo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDounDounBa, api::InstrumentSound::drumDounDounBa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDuff, api::InstrumentSound::drumDuff },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumDumbek, api::InstrumentSound::drumDumbek },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumFontomfrom, api::InstrumentSound::drumFontomfrom },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumFrameDrum, api::InstrumentSound::drumFrameDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumFrameDrumArabian, api::InstrumentSound::drumFrameDrumArabian },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumGeduk, api::InstrumentSound::drumGeduk },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumGhatam, api::InstrumentSound::drumGhatam },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumGome, api::InstrumentSound::drumGome },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumGroup, api::InstrumentSound::drumGroup },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumGroupChinese, api::InstrumentSound::drumGroupChinese },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumGroupEwe, api::InstrumentSound::drumGroupEwe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumGroupIndian, api::InstrumentSound::drumGroupIndian },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumGroupSet, api::InstrumentSound::drumGroupSet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumHandDrum, api::InstrumentSound::drumHandDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumHiraDaiko, api::InstrumentSound::drumHiraDaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumIbo, api::InstrumentSound::drumIbo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumIgihumurizo, api::InstrumentSound::drumIgihumurizo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumInyahura, api::InstrumentSound::drumInyahura },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumIshakwe, api::InstrumentSound::drumIshakwe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumJangGu, api::InstrumentSound::drumJangGu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKagan, api::InstrumentSound::drumKagan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKakko, api::InstrumentSound::drumKakko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKanjira, api::InstrumentSound::drumKanjira },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKendhang, api::InstrumentSound::drumKendhang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKendhangAgeng, api::InstrumentSound::drumKendhangAgeng },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKendhangCiblon, api::InstrumentSound::drumKendhangCiblon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKenkeni, api::InstrumentSound::drumKenkeni },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKhol, api::InstrumentSound::drumKhol },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKickDrum, api::InstrumentSound::drumKickDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKidi, api::InstrumentSound::drumKidi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKoDaiko, api::InstrumentSound::drumKoDaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKpanlogo, api::InstrumentSound::drumKpanlogo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumKudum, api::InstrumentSound::drumKudum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumLambeg, api::InstrumentSound::drumLambeg },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumLionDrum, api::InstrumentSound::drumLionDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumLogDrum, api::InstrumentSound::drumLogDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumLogDrumAfrican, api::InstrumentSound::drumLogDrumAfrican },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumLogDrumNative, api::InstrumentSound::drumLogDrumNative },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumLogDrumNigerian, api::InstrumentSound::drumLogDrumNigerian },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumMadal, api::InstrumentSound::drumMadal },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumMaddale, api::InstrumentSound::drumMaddale },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumMridangam, api::InstrumentSound::drumMridangam },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumNaal, api::InstrumentSound::drumNaal },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumNagadoDaiko, api::InstrumentSound::drumNagadoDaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumNagara, api::InstrumentSound::drumNagara },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumNaqara, api::InstrumentSound::drumNaqara },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumODaiko, api::InstrumentSound::drumODaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumOkawa, api::InstrumentSound::drumOkawa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumOkedoDaiko, api::InstrumentSound::drumOkedoDaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumPahuHula, api::InstrumentSound::drumPahuHula },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumPakhawaj, api::InstrumentSound::drumPakhawaj },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumPandeiro, api::InstrumentSound::drumPandeiro },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumPandero, api::InstrumentSound::drumPandero },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumPowwow, api::InstrumentSound::drumPowwow },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumPueblo, api::InstrumentSound::drumPueblo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumRepinique, api::InstrumentSound::drumRepinique },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumRiq, api::InstrumentSound::drumRiq },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumRototom, api::InstrumentSound::drumRototom },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSabar, api::InstrumentSound::drumSabar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSakara, api::InstrumentSound::drumSakara },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSampho, api::InstrumentSound::drumSampho },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSangban, api::InstrumentSound::drumSangban },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumShimeDaiko, api::InstrumentSound::drumShimeDaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSlitDrum, api::InstrumentSound::drumSlitDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSlitDrumKrin, api::InstrumentSound::drumSlitDrumKrin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSnareDrum, api::InstrumentSound::drumSnareDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSnareDrumElectric, api::InstrumentSound::drumSnareDrumElectric },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSogo, api::InstrumentSound::drumSogo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumSurdo, api::InstrumentSound::drumSurdo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTabla, api::InstrumentSound::drumTabla },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTablaBayan, api::InstrumentSound::drumTablaBayan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTablaDayan, api::InstrumentSound::drumTablaDayan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTaiko, api::InstrumentSound::drumTaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTalking, api::InstrumentSound::drumTalking },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTama, api::InstrumentSound::drumTama },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTamborita, api::InstrumentSound::drumTamborita },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTambourine, api::InstrumentSound::drumTambourine },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTamte, api::InstrumentSound::drumTamte },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTangku, api::InstrumentSound::drumTangku },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTanTan, api::InstrumentSound::drumTanTan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTaphon, api::InstrumentSound::drumTaphon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTar, api::InstrumentSound::drumTar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTasha, api::InstrumentSound::drumTasha },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTenorDrum, api::InstrumentSound::drumTenorDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTeponaxtli, api::InstrumentSound::drumTeponaxtli },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumThavil, api::InstrumentSound::drumThavil },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTheBox, api::InstrumentSound::drumTheBox },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTimbale, api::InstrumentSound::drumTimbale },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTimpani, api::InstrumentSound::drumTimpani },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTinaja, api::InstrumentSound::drumTinaja },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumToere, api::InstrumentSound::drumToere },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTombak, api::InstrumentSound::drumTombak },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTomTom, api::InstrumentSound::drumTomTom },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTomTomSynth, api::InstrumentSound::drumTomTomSynth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTsuzumi, api::InstrumentSound::drumTsuzumi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumTumbak, api::InstrumentSound::drumTumbak },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumUchiwaDaiko, api::InstrumentSound::drumUchiwaDaiko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumUdaku, api::InstrumentSound::drumUdaku },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumUdu, api::InstrumentSound::drumUdu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::drumZarb, api::InstrumentSound::drumZarb },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectAeolianHarp, api::InstrumentSound::effectAeolianHarp },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectAirHorn, api::InstrumentSound::effectAirHorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectApplause, api::InstrumentSound::effectApplause },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectBassStringSlap, api::InstrumentSound::effectBassStringSlap },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectBird, api::InstrumentSound::effectBird },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectBirdNightingale, api::InstrumentSound::effectBirdNightingale },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectBirdTweet, api::InstrumentSound::effectBirdTweet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectBreath, api::InstrumentSound::effectBreath },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectBubble, api::InstrumentSound::effectBubble },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectBullroarer, api::InstrumentSound::effectBullroarer },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectBurst, api::InstrumentSound::effectBurst },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectCar, api::InstrumentSound::effectCar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectCarCrash, api::InstrumentSound::effectCarCrash },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectCarEngine, api::InstrumentSound::effectCarEngine },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectCarPass, api::InstrumentSound::effectCarPass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectCarStop, api::InstrumentSound::effectCarStop },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectCrickets, api::InstrumentSound::effectCrickets },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectDog, api::InstrumentSound::effectDog },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectDoorCreak, api::InstrumentSound::effectDoorCreak },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectDoorSlam, api::InstrumentSound::effectDoorSlam },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectExplosion, api::InstrumentSound::effectExplosion },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectFluteKeyClick, api::InstrumentSound::effectFluteKeyClick },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectFootsteps, api::InstrumentSound::effectFootsteps },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectFrogs, api::InstrumentSound::effectFrogs },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectGuitarCutting, api::InstrumentSound::effectGuitarCutting },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectGuitarFret, api::InstrumentSound::effectGuitarFret },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectGunshot, api::InstrumentSound::effectGunshot },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectHandClap, api::InstrumentSound::effectHandClap },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectHeartbeat, api::InstrumentSound::effectHeartbeat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectHelicopter, api::InstrumentSound::effectHelicopter },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectHighQ, api::InstrumentSound::effectHighQ },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectHorseGallop, api::InstrumentSound::effectHorseGallop },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectJetPlane, api::InstrumentSound::effectJetPlane },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectLaserGun, api::InstrumentSound::effectLaserGun },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectLaugh, api::InstrumentSound::effectLaugh },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectLionsRoar, api::InstrumentSound::effectLionsRoar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectMachineGun, api::InstrumentSound::effectMachineGun },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectMarchingMachine, api::InstrumentSound::effectMarchingMachine },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectMetronomeBell, api::InstrumentSound::effectMetronomeBell },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectMetronomeClick, api::InstrumentSound::effectMetronomeClick },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectPat, api::InstrumentSound::effectPat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectPunch, api::InstrumentSound::effectPunch },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectRain, api::InstrumentSound::effectRain },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectScratch, api::InstrumentSound::effectScratch },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectScream, api::InstrumentSound::effectScream },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectSeashore, api::InstrumentSound::effectSeashore },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectSiren, api::InstrumentSound::effectSiren },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectSlap, api::InstrumentSound::effectSlap },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectSnap, api::InstrumentSound::effectSnap },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectStamp, api::InstrumentSound::effectStamp },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectStarship, api::InstrumentSound::effectStarship },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectStream, api::InstrumentSound::effectStream },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectTelephoneRing, api::InstrumentSound::effectTelephoneRing },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectThunder, api::InstrumentSound::effectThunder },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectTrain, api::InstrumentSound::effectTrain },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectTrashCan, api::InstrumentSound::effectTrashCan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectWhip, api::InstrumentSound::effectWhip },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectWhistle, api::InstrumentSound::effectWhistle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectWhistleMouthSiren, api::InstrumentSound::effectWhistleMouthSiren },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectWhistlePolice, api::InstrumentSound::effectWhistlePolice },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectWhistleSlide, api::InstrumentSound::effectWhistleSlide },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectWhistleTrain, api::InstrumentSound::effectWhistleTrain },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::effectWind, api::InstrumentSound::effectWind },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardAccordion, api::InstrumentSound::keyboardAccordion },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardBandoneon, api::InstrumentSound::keyboardBandoneon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardCelesta, api::InstrumentSound::keyboardCelesta },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardClavichord, api::InstrumentSound::keyboardClavichord },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardClavichordSynth, api::InstrumentSound::keyboardClavichordSynth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardConcertina, api::InstrumentSound::keyboardConcertina },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardFortepiano, api::InstrumentSound::keyboardFortepiano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardHarmonium, api::InstrumentSound::keyboardHarmonium },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardHarpsichord, api::InstrumentSound::keyboardHarpsichord },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardOndesMartenot, api::InstrumentSound::keyboardOndesMartenot },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardOrgan, api::InstrumentSound::keyboardOrgan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardOrganDrawbar, api::InstrumentSound::keyboardOrganDrawbar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardOrganPercussive, api::InstrumentSound::keyboardOrganPercussive },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardOrganPipe, api::InstrumentSound::keyboardOrganPipe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardOrganReed, api::InstrumentSound::keyboardOrganReed },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardOrganRotary, api::InstrumentSound::keyboardOrganRotary },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardPiano, api::InstrumentSound::keyboardPiano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardPianoElectric, api::InstrumentSound::keyboardPianoElectric },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardPianoGrand, api::InstrumentSound::keyboardPianoGrand },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardPianoHonkyTonk, api::InstrumentSound::keyboardPianoHonkyTonk },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardPianoPrepared, api::InstrumentSound::keyboardPianoPrepared },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardPianoToy, api::InstrumentSound::keyboardPianoToy },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardPianoUpright, api::InstrumentSound::keyboardPianoUpright },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::keyboardVirginal, api::InstrumentSound::keyboardVirginal },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalAdodo, api::InstrumentSound::metalAdodo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalAnvil, api::InstrumentSound::metalAnvil },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBabendil, api::InstrumentSound::metalBabendil },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsAgogo, api::InstrumentSound::metalBellsAgogo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsAlmglocken, api::InstrumentSound::metalBellsAlmglocken },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsBellPlate, api::InstrumentSound::metalBellsBellPlate },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsBellTree, api::InstrumentSound::metalBellsBellTree },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsCarillon, api::InstrumentSound::metalBellsCarillon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsChimes, api::InstrumentSound::metalBellsChimes },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsChimta, api::InstrumentSound::metalBellsChimta },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsChippli, api::InstrumentSound::metalBellsChippli },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsChurch, api::InstrumentSound::metalBellsChurch },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsCowbell, api::InstrumentSound::metalBellsCowbell },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsDawuro, api::InstrumentSound::metalBellsDawuro },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsGankokwe, api::InstrumentSound::metalBellsGankokwe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsGhungroo, api::InstrumentSound::metalBellsGhungroo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsHatheli, api::InstrumentSound::metalBellsHatheli },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsJingleBell, api::InstrumentSound::metalBellsJingleBell },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsKhartal, api::InstrumentSound::metalBellsKhartal },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsMarkTree, api::InstrumentSound::metalBellsMarkTree },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsSistrum, api::InstrumentSound::metalBellsSistrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsSleighBells, api::InstrumentSound::metalBellsSleighBells },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsTemple, api::InstrumentSound::metalBellsTemple },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsTibetan, api::InstrumentSound::metalBellsTibetan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsTinklebell, api::InstrumentSound::metalBellsTinklebell },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsTrychel, api::InstrumentSound::metalBellsTrychel },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsWindChimes, api::InstrumentSound::metalBellsWindChimes },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBellsZills, api::InstrumentSound::metalBellsZills },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBerimbau, api::InstrumentSound::metalBerimbau },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalBrakeDrums, api::InstrumentSound::metalBrakeDrums },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCrotales, api::InstrumentSound::metalCrotales },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalBo, api::InstrumentSound::metalCymbalBo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalCengCeng, api::InstrumentSound::metalCymbalCengCeng },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalChabara, api::InstrumentSound::metalCymbalChabara },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalChinese, api::InstrumentSound::metalCymbalChinese },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalChing, api::InstrumentSound::metalCymbalChing },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalClash, api::InstrumentSound::metalCymbalClash },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalCrash, api::InstrumentSound::metalCymbalCrash },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalFinger, api::InstrumentSound::metalCymbalFinger },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalHand, api::InstrumentSound::metalCymbalHand },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalKesi, api::InstrumentSound::metalCymbalKesi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalManjeera, api::InstrumentSound::metalCymbalManjeera },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalReverse, api::InstrumentSound::metalCymbalReverse },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalRide, api::InstrumentSound::metalCymbalRide },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalSizzle, api::InstrumentSound::metalCymbalSizzle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalSplash, api::InstrumentSound::metalCymbalSplash },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalSuspended, api::InstrumentSound::metalCymbalSuspended },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalTebyoshi, api::InstrumentSound::metalCymbalTebyoshi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalTibetan, api::InstrumentSound::metalCymbalTibetan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalCymbalTingsha, api::InstrumentSound::metalCymbalTingsha },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalFlexatone, api::InstrumentSound::metalFlexatone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGong, api::InstrumentSound::metalGong },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongAgeng, api::InstrumentSound::metalGongAgeng },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongAgung, api::InstrumentSound::metalGongAgung },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongChanchiki, api::InstrumentSound::metalGongChanchiki },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongChinese, api::InstrumentSound::metalGongChinese },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongGandingan, api::InstrumentSound::metalGongGandingan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongKempul, api::InstrumentSound::metalGongKempul },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongKempyang, api::InstrumentSound::metalGongKempyang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongKetuk, api::InstrumentSound::metalGongKetuk },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongKkwenggwari, api::InstrumentSound::metalGongKkwenggwari },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongLuo, api::InstrumentSound::metalGongLuo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongSinging, api::InstrumentSound::metalGongSinging },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGongThai, api::InstrumentSound::metalGongThai },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalGuira, api::InstrumentSound::metalGuira },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalHang, api::InstrumentSound::metalHang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalHiHat, api::InstrumentSound::metalHiHat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalJawHarp, api::InstrumentSound::metalJawHarp },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalKengong, api::InstrumentSound::metalKengong },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalMurchang, api::InstrumentSound::metalMurchang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalMusicalSaw, api::InstrumentSound::metalMusicalSaw },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalSingingBowl, api::InstrumentSound::metalSingingBowl },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalSpoons, api::InstrumentSound::metalSpoons },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalSteelDrums, api::InstrumentSound::metalSteelDrums },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalTamtam, api::InstrumentSound::metalTamtam },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalThundersheet, api::InstrumentSound::metalThundersheet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalTriangle, api::InstrumentSound::metalTriangle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::metalWashboard, api::InstrumentSound::metalWashboard },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionAngklung, api::InstrumentSound::pitchedPercussionAngklung },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionBalafon, api::InstrumentSound::pitchedPercussionBalafon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionBellLyre, api::InstrumentSound::pitchedPercussionBellLyre },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionBells, api::InstrumentSound::pitchedPercussionBells },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionBianqing, api::InstrumentSound::pitchedPercussionBianqing },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionBianzhong, api::InstrumentSound::pitchedPercussionBianzhong },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionBonang, api::InstrumentSound::pitchedPercussionBonang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionCimbalom, api::InstrumentSound::pitchedPercussionCimbalom },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionCrystalGlasses, api::InstrumentSound::pitchedPercussionCrystalGlasses },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionDanTamThapLuc, api::InstrumentSound::pitchedPercussionDanTamThapLuc },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionFangxiang, api::InstrumentSound::pitchedPercussionFangxiang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGandinganAKayo, api::InstrumentSound::pitchedPercussionGandinganAKayo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGangsa, api::InstrumentSound::pitchedPercussionGangsa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGender, api::InstrumentSound::pitchedPercussionGender },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGiying, api::InstrumentSound::pitchedPercussionGiying },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGlassHarmonica, api::InstrumentSound::pitchedPercussionGlassHarmonica },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGlockenspiel, api::InstrumentSound::pitchedPercussionGlockenspiel },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGlockenspielAlto, api::InstrumentSound::pitchedPercussionGlockenspielAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGlockenspielSoprano, api::InstrumentSound::pitchedPercussionGlockenspielSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionGyil, api::InstrumentSound::pitchedPercussionGyil },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionHammerDulcimer, api::InstrumentSound::pitchedPercussionHammerDulcimer },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionHandbells, api::InstrumentSound::pitchedPercussionHandbells },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionKalimba, api::InstrumentSound::pitchedPercussionKalimba },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionKantil, api::InstrumentSound::pitchedPercussionKantil },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionKhim, api::InstrumentSound::pitchedPercussionKhim },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionKulintang, api::InstrumentSound::pitchedPercussionKulintang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionKulintangAKayo, api::InstrumentSound::pitchedPercussionKulintangAKayo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionKulintangATiniok, api::InstrumentSound::pitchedPercussionKulintangATiniok },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionLikembe, api::InstrumentSound::pitchedPercussionLikembe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionLuntang, api::InstrumentSound::pitchedPercussionLuntang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMarimba, api::InstrumentSound::pitchedPercussionMarimba },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMarimbaBass, api::InstrumentSound::pitchedPercussionMarimbaBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMbira, api::InstrumentSound::pitchedPercussionMbira },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMbiraArray, api::InstrumentSound::pitchedPercussionMbiraArray },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMetallophone, api::InstrumentSound::pitchedPercussionMetallophone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMetallophoneAlto, api::InstrumentSound::pitchedPercussionMetallophoneAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMetallophoneBass, api::InstrumentSound::pitchedPercussionMetallophoneBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMetallophoneSoprano, api::InstrumentSound::pitchedPercussionMetallophoneSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionMusicBox, api::InstrumentSound::pitchedPercussionMusicBox },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionPelogPanerus, api::InstrumentSound::pitchedPercussionPelogPanerus },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionPemade, api::InstrumentSound::pitchedPercussionPemade },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionPenyacah, api::InstrumentSound::pitchedPercussionPenyacah },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionRanatEk, api::InstrumentSound::pitchedPercussionRanatEk },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionRanatEkLek, api::InstrumentSound::pitchedPercussionRanatEkLek },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionRanatThum, api::InstrumentSound::pitchedPercussionRanatThum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionRanatThumLek, api::InstrumentSound::pitchedPercussionRanatThumLek },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionReyong, api::InstrumentSound::pitchedPercussionReyong },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionSanza, api::InstrumentSound::pitchedPercussionSanza },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionSaronBarung, api::InstrumentSound::pitchedPercussionSaronBarung },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionSaronDemong, api::InstrumentSound::pitchedPercussionSaronDemong },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionSaronPanerus, api::InstrumentSound::pitchedPercussionSaronPanerus },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionSlendroPanerus, api::InstrumentSound::pitchedPercussionSlendroPanerus },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionSlentem, api::InstrumentSound::pitchedPercussionSlentem },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionTsymbaly, api::InstrumentSound::pitchedPercussionTsymbaly },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionTubes, api::InstrumentSound::pitchedPercussionTubes },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionTubularBells, api::InstrumentSound::pitchedPercussionTubularBells },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionVibraphone, api::InstrumentSound::pitchedPercussionVibraphone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionXylophone, api::InstrumentSound::pitchedPercussionXylophone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionXylophoneAlto, api::InstrumentSound::pitchedPercussionXylophoneAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionXylophoneBass, api::InstrumentSound::pitchedPercussionXylophoneBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionXylophoneSoprano, api::InstrumentSound::pitchedPercussionXylophoneSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionXylorimba, api::InstrumentSound::pitchedPercussionXylorimba },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pitchedPercussionYangqin, api::InstrumentSound::pitchedPercussionYangqin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckArchlute, api::InstrumentSound::pluckArchlute },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckAutoharp, api::InstrumentSound::pluckAutoharp },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBaglama, api::InstrumentSound::pluckBaglama },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBajo, api::InstrumentSound::pluckBajo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBalalaika, api::InstrumentSound::pluckBalalaika },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBalalaikaAlto, api::InstrumentSound::pluckBalalaikaAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBalalaikaBass, api::InstrumentSound::pluckBalalaikaBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBalalaikaContrabass, api::InstrumentSound::pluckBalalaikaContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBalalaikaPiccolo, api::InstrumentSound::pluckBalalaikaPiccolo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBalalaikaPrima, api::InstrumentSound::pluckBalalaikaPrima },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBalalaikaSecunda, api::InstrumentSound::pluckBalalaikaSecunda },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBandola, api::InstrumentSound::pluckBandola },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBandura, api::InstrumentSound::pluckBandura },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBandurria, api::InstrumentSound::pluckBandurria },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBanjo, api::InstrumentSound::pluckBanjo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBanjoTenor, api::InstrumentSound::pluckBanjoTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBanjolele, api::InstrumentSound::pluckBanjolele },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBarbat, api::InstrumentSound::pluckBarbat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBass, api::InstrumentSound::pluckBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassAcoustic, api::InstrumentSound::pluckBassAcoustic },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassBolon, api::InstrumentSound::pluckBassBolon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassElectric, api::InstrumentSound::pluckBassElectric },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassFretless, api::InstrumentSound::pluckBassFretless },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassGuitarron, api::InstrumentSound::pluckBassGuitarron },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassSynth, api::InstrumentSound::pluckBassSynth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassSynthLead, api::InstrumentSound::pluckBassSynthLead },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassWashtub, api::InstrumentSound::pluckBassWashtub },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBassWhamola, api::InstrumentSound::pluckBassWhamola },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBegena, api::InstrumentSound::pluckBegena },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBiwa, api::InstrumentSound::pluckBiwa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBordonua, api::InstrumentSound::pluckBordonua },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBouzouki, api::InstrumentSound::pluckBouzouki },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckBouzoukiIrish, api::InstrumentSound::pluckBouzoukiIrish },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckCelticHarp, api::InstrumentSound::pluckCelticHarp },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckCharango, api::InstrumentSound::pluckCharango },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckChitarraBattente, api::InstrumentSound::pluckChitarraBattente },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckCithara, api::InstrumentSound::pluckCithara },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckCittern, api::InstrumentSound::pluckCittern },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckCuatro, api::InstrumentSound::pluckCuatro },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDanBau, api::InstrumentSound::pluckDanBau },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDanNguyet, api::InstrumentSound::pluckDanNguyet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDanTranh, api::InstrumentSound::pluckDanTranh },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDanTyBa, api::InstrumentSound::pluckDanTyBa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDiddleyBow, api::InstrumentSound::pluckDiddleyBow },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDomra, api::InstrumentSound::pluckDomra },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDomu, api::InstrumentSound::pluckDomu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDulcimer, api::InstrumentSound::pluckDulcimer },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDutar, api::InstrumentSound::pluckDutar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckDuxianqin, api::InstrumentSound::pluckDuxianqin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckEktara, api::InstrumentSound::pluckEktara },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGeomungo, api::InstrumentSound::pluckGeomungo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGottuvadhyam, api::InstrumentSound::pluckGottuvadhyam },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitar, api::InstrumentSound::pluckGuitar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitarAcoustic, api::InstrumentSound::pluckGuitarAcoustic },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitarElectric, api::InstrumentSound::pluckGuitarElectric },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitarNylonString, api::InstrumentSound::pluckGuitarNylonString },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitarPedalSteel, api::InstrumentSound::pluckGuitarPedalSteel },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitarPortuguese, api::InstrumentSound::pluckGuitarPortuguese },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitarRequinto, api::InstrumentSound::pluckGuitarRequinto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitarResonator, api::InstrumentSound::pluckGuitarResonator },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitarSteelString, api::InstrumentSound::pluckGuitarSteelString },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitjo, api::InstrumentSound::pluckGuitjo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuitjoDoubleNeck, api::InstrumentSound::pluckGuitjoDoubleNeck },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuqin, api::InstrumentSound::pluckGuqin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuzheng, api::InstrumentSound::pluckGuzheng },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckGuzhengChoazhou, api::InstrumentSound::pluckGuzhengChoazhou },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckHarp, api::InstrumentSound::pluckHarp },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckHarpGuitar, api::InstrumentSound::pluckHarpGuitar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckHuapanguera, api::InstrumentSound::pluckHuapanguera },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckJaranaHuasteca, api::InstrumentSound::pluckJaranaHuasteca },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckJaranaJarocha, api::InstrumentSound::pluckJaranaJarocha },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckJaranaJarochaMosquito, api::InstrumentSound::pluckJaranaJarochaMosquito },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckJaranaJarochaPrimera, api::InstrumentSound::pluckJaranaJarochaPrimera },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckJaranaJarochaSegunda, api::InstrumentSound::pluckJaranaJarochaSegunda },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckJaranaJarochaTercera, api::InstrumentSound::pluckJaranaJarochaTercera },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKabosy, api::InstrumentSound::pluckKabosy },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKantele, api::InstrumentSound::pluckKantele },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKanun, api::InstrumentSound::pluckKanun },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKayagum, api::InstrumentSound::pluckKayagum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKobza, api::InstrumentSound::pluckKobza },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKomuz, api::InstrumentSound::pluckKomuz },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKora, api::InstrumentSound::pluckKora },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKoto, api::InstrumentSound::pluckKoto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckKutiyapi, api::InstrumentSound::pluckKutiyapi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckLangeleik, api::InstrumentSound::pluckLangeleik },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckLaud, api::InstrumentSound::pluckLaud },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckLute, api::InstrumentSound::pluckLute },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckLyre, api::InstrumentSound::pluckLyre },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMandobass, api::InstrumentSound::pluckMandobass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMandocello, api::InstrumentSound::pluckMandocello },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMandola, api::InstrumentSound::pluckMandola },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMandolin, api::InstrumentSound::pluckMandolin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMandolinOctave, api::InstrumentSound::pluckMandolinOctave },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMandora, api::InstrumentSound::pluckMandora },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMandore, api::InstrumentSound::pluckMandore },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMarovany, api::InstrumentSound::pluckMarovany },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckMusicalBow, api::InstrumentSound::pluckMusicalBow },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckNgoni, api::InstrumentSound::pluckNgoni },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckOud, api::InstrumentSound::pluckOud },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckPipa, api::InstrumentSound::pluckPipa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckPsaltery, api::InstrumentSound::pluckPsaltery },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckRuan, api::InstrumentSound::pluckRuan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSallaneh, api::InstrumentSound::pluckSallaneh },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSanshin, api::InstrumentSound::pluckSanshin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSantoor, api::InstrumentSound::pluckSantoor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSanxian, api::InstrumentSound::pluckSanxian },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSarod, api::InstrumentSound::pluckSarod },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSaung, api::InstrumentSound::pluckSaung },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSaz, api::InstrumentSound::pluckSaz },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSe, api::InstrumentSound::pluckSe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSetar, api::InstrumentSound::pluckSetar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckShamisen, api::InstrumentSound::pluckShamisen },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSitar, api::InstrumentSound::pluckSitar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSynth, api::InstrumentSound::pluckSynth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSynthCharang, api::InstrumentSound::pluckSynthCharang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSynthChiff, api::InstrumentSound::pluckSynthChiff },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckSynthStick, api::InstrumentSound::pluckSynthStick },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTambura, api::InstrumentSound::pluckTambura },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTamburaBulgarian, api::InstrumentSound::pluckTamburaBulgarian },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTamburaFemale, api::InstrumentSound::pluckTamburaFemale },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTamburaMale, api::InstrumentSound::pluckTamburaMale },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTar, api::InstrumentSound::pluckTar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTheorbo, api::InstrumentSound::pluckTheorbo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTimple, api::InstrumentSound::pluckTimple },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTiple, api::InstrumentSound::pluckTiple },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckTres, api::InstrumentSound::pluckTres },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckUkulele, api::InstrumentSound::pluckUkulele },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckUkuleleTenor, api::InstrumentSound::pluckUkuleleTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckValiha, api::InstrumentSound::pluckValiha },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckVeena, api::InstrumentSound::pluckVeena },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckVeenaMohan, api::InstrumentSound::pluckVeenaMohan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckVeenaRudra, api::InstrumentSound::pluckVeenaRudra },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckVeenaVichitra, api::InstrumentSound::pluckVeenaVichitra },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckVihuela, api::InstrumentSound::pluckVihuela },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckVihuelaMexican, api::InstrumentSound::pluckVihuelaMexican },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckXalam, api::InstrumentSound::pluckXalam },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckYueqin, api::InstrumentSound::pluckYueqin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckZither, api::InstrumentSound::pluckZither },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::pluckZitherOvertone, api::InstrumentSound::pluckZitherOvertone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleAfoxe, api::InstrumentSound::rattleAfoxe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleBirds, api::InstrumentSound::rattleBirds },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleCabasa, api::InstrumentSound::rattleCabasa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleCaxixi, api::InstrumentSound::rattleCaxixi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleCog, api::InstrumentSound::rattleCog },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleGanza, api::InstrumentSound::rattleGanza },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleHosho, api::InstrumentSound::rattleHosho },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleJawbone, api::InstrumentSound::rattleJawbone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleKayamba, api::InstrumentSound::rattleKayamba },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleKpokoKpoko, api::InstrumentSound::rattleKpokoKpoko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleLavaStones, api::InstrumentSound::rattleLavaStones },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleMaraca, api::InstrumentSound::rattleMaraca },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleRainStick, api::InstrumentSound::rattleRainStick },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleRatchet, api::InstrumentSound::rattleRatchet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleRattle, api::InstrumentSound::rattleRattle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleShaker, api::InstrumentSound::rattleShaker },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleShakerEgg, api::InstrumentSound::rattleShakerEgg },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleShekere, api::InstrumentSound::rattleShekere },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleSistre, api::InstrumentSound::rattleSistre },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleTelevi, api::InstrumentSound::rattleTelevi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleVibraslap, api::InstrumentSound::rattleVibraslap },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::rattleWasembe, api::InstrumentSound::rattleWasembe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsAjaeng, api::InstrumentSound::stringsAjaeng },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsArpeggione, api::InstrumentSound::stringsArpeggione },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsBaryton, api::InstrumentSound::stringsBaryton },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsCello, api::InstrumentSound::stringsCello },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsCelloPiccolo, api::InstrumentSound::stringsCelloPiccolo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsContrabass, api::InstrumentSound::stringsContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsCrwth, api::InstrumentSound::stringsCrwth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsDanGao, api::InstrumentSound::stringsDanGao },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsDihu, api::InstrumentSound::stringsDihu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsErhu, api::InstrumentSound::stringsErhu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsErxian, api::InstrumentSound::stringsErxian },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsEsraj, api::InstrumentSound::stringsEsraj },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsFiddle, api::InstrumentSound::stringsFiddle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsFiddleHardanger, api::InstrumentSound::stringsFiddleHardanger },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsGadulka, api::InstrumentSound::stringsGadulka },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsGaohu, api::InstrumentSound::stringsGaohu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsGehu, api::InstrumentSound::stringsGehu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsGroup, api::InstrumentSound::stringsGroup },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsGroupSynth, api::InstrumentSound::stringsGroupSynth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsHaegeum, api::InstrumentSound::stringsHaegeum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsHurdyGurdy, api::InstrumentSound::stringsHurdyGurdy },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsIgil, api::InstrumentSound::stringsIgil },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsKamancha, api::InstrumentSound::stringsKamancha },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsKokyu, api::InstrumentSound::stringsKokyu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsLaruan, api::InstrumentSound::stringsLaruan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsLeiqin, api::InstrumentSound::stringsLeiqin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsLirone, api::InstrumentSound::stringsLirone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsLyraByzantine, api::InstrumentSound::stringsLyraByzantine },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsLyraCretan, api::InstrumentSound::stringsLyraCretan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsMorinKhuur, api::InstrumentSound::stringsMorinKhuur },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsNyckelharpa, api::InstrumentSound::stringsNyckelharpa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsOctobass, api::InstrumentSound::stringsOctobass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsRebab, api::InstrumentSound::stringsRebab },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsRebec, api::InstrumentSound::stringsRebec },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsSarangi, api::InstrumentSound::stringsSarangi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsStrohViolin, api::InstrumentSound::stringsStrohViolin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsTrombaMarina, api::InstrumentSound::stringsTrombaMarina },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsVielle, api::InstrumentSound::stringsVielle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViol, api::InstrumentSound::stringsViol },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolAlto, api::InstrumentSound::stringsViolAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolBass, api::InstrumentSound::stringsViolBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolTenor, api::InstrumentSound::stringsViolTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolTreble, api::InstrumentSound::stringsViolTreble },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolViolone, api::InstrumentSound::stringsViolViolone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViola, api::InstrumentSound::stringsViola },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolaDamore, api::InstrumentSound::stringsViolaDamore },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolin, api::InstrumentSound::stringsViolin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolonoPiccolo, api::InstrumentSound::stringsViolonoPiccolo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsViolotta, api::InstrumentSound::stringsViolotta },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsYayliTanbur, api::InstrumentSound::stringsYayliTanbur },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsYazheng, api::InstrumentSound::stringsYazheng },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::stringsZhonghu, api::InstrumentSound::stringsZhonghu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffects, api::InstrumentSound::synthEffects },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffectsAtmosphere, api::InstrumentSound::synthEffectsAtmosphere },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffectsBrightness, api::InstrumentSound::synthEffectsBrightness },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffectsCrystal, api::InstrumentSound::synthEffectsCrystal },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffectsEchoes, api::InstrumentSound::synthEffectsEchoes },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffectsGoblins, api::InstrumentSound::synthEffectsGoblins },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffectsRain, api::InstrumentSound::synthEffectsRain },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffectsSciFi, api::InstrumentSound::synthEffectsSciFi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthEffectsSoundtrack, api::InstrumentSound::synthEffectsSoundtrack },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthGroup, api::InstrumentSound::synthGroup },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthGroupFifths, api::InstrumentSound::synthGroupFifths },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthGroupOrchestra, api::InstrumentSound::synthGroupOrchestra },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthPad, api::InstrumentSound::synthPad },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthPadBowed, api::InstrumentSound::synthPadBowed },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthPadChoir, api::InstrumentSound::synthPadChoir },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthPadHalo, api::InstrumentSound::synthPadHalo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthPadMetallic, api::InstrumentSound::synthPadMetallic },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthPadPolysynth, api::InstrumentSound::synthPadPolysynth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthPadSweep, api::InstrumentSound::synthPadSweep },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthPadWarm, api::InstrumentSound::synthPadWarm },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthTheremin, api::InstrumentSound::synthTheremin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthToneSawtooth, api::InstrumentSound::synthToneSawtooth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthToneSine, api::InstrumentSound::synthToneSine },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::synthToneSquare, api::InstrumentSound::synthToneSquare },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceAa, api::InstrumentSound::voiceAa },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceAlto, api::InstrumentSound::voiceAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceAw, api::InstrumentSound::voiceAw },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceBaritone, api::InstrumentSound::voiceBaritone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceBass, api::InstrumentSound::voiceBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceChild, api::InstrumentSound::voiceChild },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceCountertenor, api::InstrumentSound::voiceCountertenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceDoo, api::InstrumentSound::voiceDoo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceEe, api::InstrumentSound::voiceEe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceFemale, api::InstrumentSound::voiceFemale },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceKazoo, api::InstrumentSound::voiceKazoo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceMale, api::InstrumentSound::voiceMale },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceMezzoSoprano, api::InstrumentSound::voiceMezzoSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceMm, api::InstrumentSound::voiceMm },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceOo, api::InstrumentSound::voiceOo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voicePercussion, api::InstrumentSound::voicePercussion },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voicePercussionBeatbox, api::InstrumentSound::voicePercussionBeatbox },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceSoprano, api::InstrumentSound::voiceSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceSynth, api::InstrumentSound::voiceSynth },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceTalkBox, api::InstrumentSound::voiceTalkBox },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceTenor, api::InstrumentSound::voiceTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::voiceVocals, api::InstrumentSound::voiceVocals },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesBansuri, api::InstrumentSound::windFlutesBansuri },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesBlownBottle, api::InstrumentSound::windFlutesBlownBottle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesCalliope, api::InstrumentSound::windFlutesCalliope },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesDanso, api::InstrumentSound::windFlutesDanso },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesDiZi, api::InstrumentSound::windFlutesDiZi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesDvojnice, api::InstrumentSound::windFlutesDvojnice },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFife, api::InstrumentSound::windFlutesFife },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFlageolet, api::InstrumentSound::windFlutesFlageolet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFlute, api::InstrumentSound::windFlutesFlute },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFluteAlto, api::InstrumentSound::windFlutesFluteAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFluteBass, api::InstrumentSound::windFlutesFluteBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFluteContraAlto, api::InstrumentSound::windFlutesFluteContraAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFluteContrabass, api::InstrumentSound::windFlutesFluteContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFluteDoubleContrabass, api::InstrumentSound::windFlutesFluteDoubleContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFluteIrish, api::InstrumentSound::windFlutesFluteIrish },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFlutePiccolo, api::InstrumentSound::windFlutesFlutePiccolo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFluteSubcontrabass, api::InstrumentSound::windFlutesFluteSubcontrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesFujara, api::InstrumentSound::windFlutesFujara },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesGemshorn, api::InstrumentSound::windFlutesGemshorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesHocchiku, api::InstrumentSound::windFlutesHocchiku },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesHun, api::InstrumentSound::windFlutesHun },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesKaval, api::InstrumentSound::windFlutesKaval },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesKawala, api::InstrumentSound::windFlutesKawala },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesKhlui, api::InstrumentSound::windFlutesKhlui },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesKnotweed, api::InstrumentSound::windFlutesKnotweed },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesKoncovkaAlto, api::InstrumentSound::windFlutesKoncovkaAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesKoudi, api::InstrumentSound::windFlutesKoudi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesNey, api::InstrumentSound::windFlutesNey },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesNohkan, api::InstrumentSound::windFlutesNohkan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesNose, api::InstrumentSound::windFlutesNose },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesOcarina, api::InstrumentSound::windFlutesOcarina },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesOvertoneTenor, api::InstrumentSound::windFlutesOvertoneTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesPalendag, api::InstrumentSound::windFlutesPalendag },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesPanpipes, api::InstrumentSound::windFlutesPanpipes },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesQuena, api::InstrumentSound::windFlutesQuena },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorder, api::InstrumentSound::windFlutesRecorder },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderAlto, api::InstrumentSound::windFlutesRecorderAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderBass, api::InstrumentSound::windFlutesRecorderBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderContrabass, api::InstrumentSound::windFlutesRecorderContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderDescant, api::InstrumentSound::windFlutesRecorderDescant },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderGarklein, api::InstrumentSound::windFlutesRecorderGarklein },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderGreatBass, api::InstrumentSound::windFlutesRecorderGreatBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderSopranino, api::InstrumentSound::windFlutesRecorderSopranino },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderSoprano, api::InstrumentSound::windFlutesRecorderSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRecorderTenor, api::InstrumentSound::windFlutesRecorderTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesRyuteki, api::InstrumentSound::windFlutesRyuteki },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesShakuhachi, api::InstrumentSound::windFlutesShakuhachi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesShepherdsPipe, api::InstrumentSound::windFlutesShepherdsPipe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesShinobue, api::InstrumentSound::windFlutesShinobue },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesShvi, api::InstrumentSound::windFlutesShvi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesSuling, api::InstrumentSound::windFlutesSuling },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesTarka, api::InstrumentSound::windFlutesTarka },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesTumpong, api::InstrumentSound::windFlutesTumpong },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesVenu, api::InstrumentSound::windFlutesVenu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesWhistle, api::InstrumentSound::windFlutesWhistle },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesWhistleAlto, api::InstrumentSound::windFlutesWhistleAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesWhistleLowIrish, api::InstrumentSound::windFlutesWhistleLowIrish },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesWhistleShiva, api::InstrumentSound::windFlutesWhistleShiva },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesWhistleSlide, api::InstrumentSound::windFlutesWhistleSlide },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesWhistleTin, api::InstrumentSound::windFlutesWhistleTin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesWhistleTinBflat, api::InstrumentSound::windFlutesWhistleTinBflat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesWhistleTinD, api::InstrumentSound::windFlutesWhistleTinD },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesXiao, api::InstrumentSound::windFlutesXiao },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windFlutesXun, api::InstrumentSound::windFlutesXun },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windGroup, api::InstrumentSound::windGroup },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windJug, api::InstrumentSound::windJug },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windPipesBagpipes, api::InstrumentSound::windPipesBagpipes },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windPipesGaida, api::InstrumentSound::windPipesGaida },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windPipesHighland, api::InstrumentSound::windPipesHighland },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windPipesUilleann, api::InstrumentSound::windPipesUilleann },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windPungi, api::InstrumentSound::windPungi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedAlbogue, api::InstrumentSound::windReedAlbogue },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedAlboka, api::InstrumentSound::windReedAlboka },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedAlgaita, api::InstrumentSound::windReedAlgaita },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedArghul, api::InstrumentSound::windReedArghul },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedBassetHorn, api::InstrumentSound::windReedBassetHorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedBassoon, api::InstrumentSound::windReedBassoon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedBawu, api::InstrumentSound::windReedBawu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedBifora, api::InstrumentSound::windReedBifora },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedBombarde, api::InstrumentSound::windReedBombarde },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedChalumeau, api::InstrumentSound::windReedChalumeau },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinet, api::InstrumentSound::windReedClarinet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetA, api::InstrumentSound::windReedClarinetA },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetAlto, api::InstrumentSound::windReedClarinetAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetBass, api::InstrumentSound::windReedClarinetBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetBasset, api::InstrumentSound::windReedClarinetBasset },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetBflat, api::InstrumentSound::windReedClarinetBflat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetContraAlto, api::InstrumentSound::windReedClarinetContraAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetContrabass, api::InstrumentSound::windReedClarinetContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetEflat, api::InstrumentSound::windReedClarinetEflat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetPiccoloAflat, api::InstrumentSound::windReedClarinetPiccoloAflat },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedClarinetteDamour, api::InstrumentSound::windReedClarinetteDamour },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedContrabass, api::InstrumentSound::windReedContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedContrabassoon, api::InstrumentSound::windReedContrabassoon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedCornamuse, api::InstrumentSound::windReedCornamuse },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedCromorne, api::InstrumentSound::windReedCromorne },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedCrumhorn, api::InstrumentSound::windReedCrumhorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedCrumhornAlto, api::InstrumentSound::windReedCrumhornAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedCrumhornBass, api::InstrumentSound::windReedCrumhornBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedCrumhornGreatBass, api::InstrumentSound::windReedCrumhornGreatBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedCrumhornSoprano, api::InstrumentSound::windReedCrumhornSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedCrumhornTenor, api::InstrumentSound::windReedCrumhornTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedDiple, api::InstrumentSound::windReedDiple },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedDiplica, api::InstrumentSound::windReedDiplica },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedDuduk, api::InstrumentSound::windReedDuduk },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedDulcian, api::InstrumentSound::windReedDulcian },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedDulzaina, api::InstrumentSound::windReedDulzaina },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedEnglishHorn, api::InstrumentSound::windReedEnglishHorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedGuanzi, api::InstrumentSound::windReedGuanzi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHarmonica, api::InstrumentSound::windReedHarmonica },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHarmonicaBass, api::InstrumentSound::windReedHarmonicaBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHeckelClarina, api::InstrumentSound::windReedHeckelClarina },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHeckelphone, api::InstrumentSound::windReedHeckelphone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHeckelphonePiccolo, api::InstrumentSound::windReedHeckelphonePiccolo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHeckelphoneClarinet, api::InstrumentSound::windReedHeckelphoneClarinet },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHichiriki, api::InstrumentSound::windReedHichiriki },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHirtenschalmei, api::InstrumentSound::windReedHirtenschalmei },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHne, api::InstrumentSound::windReedHne },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHornpipe, api::InstrumentSound::windReedHornpipe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHouguan, api::InstrumentSound::windReedHouguan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedHulusi, api::InstrumentSound::windReedHulusi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedJogiBaja, api::InstrumentSound::windReedJogiBaja },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedKenBau, api::InstrumentSound::windReedKenBau },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedKhaenMouthOrgan, api::InstrumentSound::windReedKhaenMouthOrgan },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedLauneddas, api::InstrumentSound::windReedLauneddas },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedMaqrunah, api::InstrumentSound::windReedMaqrunah },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedMelodica, api::InstrumentSound::windReedMelodica },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedMijwiz, api::InstrumentSound::windReedMijwiz },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedMizmar, api::InstrumentSound::windReedMizmar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedNadaswaram, api::InstrumentSound::windReedNadaswaram },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedOboe, api::InstrumentSound::windReedOboe },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedOboeBass, api::InstrumentSound::windReedOboeBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedOboePiccolo, api::InstrumentSound::windReedOboePiccolo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedOboeDaCaccia, api::InstrumentSound::windReedOboeDaCaccia },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedOboeDamore, api::InstrumentSound::windReedOboeDamore },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedOctavin, api::InstrumentSound::windReedOctavin },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedPi, api::InstrumentSound::windReedPi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedPibgorn, api::InstrumentSound::windReedPibgorn },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedPiri, api::InstrumentSound::windReedPiri },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedRackett, api::InstrumentSound::windReedRackett },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedRauschpfeife, api::InstrumentSound::windReedRauschpfeife },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedRhaita, api::InstrumentSound::windReedRhaita },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedRothphone, api::InstrumentSound::windReedRothphone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSarrusaphone, api::InstrumentSound::windReedSarrusaphone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxonette, api::InstrumentSound::windReedSaxonette },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophone, api::InstrumentSound::windReedSaxophone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneAlto, api::InstrumentSound::windReedSaxophoneAlto },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneAulochrome, api::InstrumentSound::windReedSaxophoneAulochrome },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneBaritone, api::InstrumentSound::windReedSaxophoneBaritone },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneBass, api::InstrumentSound::windReedSaxophoneBass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneContrabass, api::InstrumentSound::windReedSaxophoneContrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneMelody, api::InstrumentSound::windReedSaxophoneMelody },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneMezzoSoprano, api::InstrumentSound::windReedSaxophoneMezzoSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneSopranino, api::InstrumentSound::windReedSaxophoneSopranino },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneSopranissimo, api::InstrumentSound::windReedSaxophoneSopranissimo },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneSoprano, api::InstrumentSound::windReedSaxophoneSoprano },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneSubcontrabass, api::InstrumentSound::windReedSaxophoneSubcontrabass },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSaxophoneTenor, api::InstrumentSound::windReedSaxophoneTenor },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedShawm, api::InstrumentSound::windReedShawm },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedShenai, api::InstrumentSound::windReedShenai },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSheng, api::InstrumentSound::windReedSheng },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSipsi, api::InstrumentSound::windReedSipsi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSopila, api::InstrumentSound::windReedSopila },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSorna, api::InstrumentSound::windReedSorna },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSralai, api::InstrumentSound::windReedSralai },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSuona, api::InstrumentSound::windReedSuona },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedSurnai, api::InstrumentSound::windReedSurnai },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedTaepyeongso, api::InstrumentSound::windReedTaepyeongso },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedTarogato, api::InstrumentSound::windReedTarogato },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedTarogatoAncient, api::InstrumentSound::windReedTarogatoAncient },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedTrompetaChina, api::InstrumentSound::windReedTrompetaChina },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedTubax, api::InstrumentSound::windReedTubax },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedXaphoon, api::InstrumentSound::windReedXaphoon },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedZhaleika, api::InstrumentSound::windReedZhaleika },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedZurla, api::InstrumentSound::windReedZurla },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::windReedZurna, api::InstrumentSound::windReedZurna },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodAgogoBlock, api::InstrumentSound::woodAgogoBlock },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodAgungATamlang, api::InstrumentSound::woodAgungATamlang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodAhoko, api::InstrumentSound::woodAhoko },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodBones, api::InstrumentSound::woodBones },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodCastanets, api::InstrumentSound::woodCastanets },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodClaves, api::InstrumentSound::woodClaves },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodDrumSticks, api::InstrumentSound::woodDrumSticks },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodGourd, api::InstrumentSound::woodGourd },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodGraniteBlock, api::InstrumentSound::woodGraniteBlock },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodGuban, api::InstrumentSound::woodGuban },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodGuiro, api::InstrumentSound::woodGuiro },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodHyoushigi, api::InstrumentSound::woodHyoushigi },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodIpu, api::InstrumentSound::woodIpu },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodJamBlock, api::InstrumentSound::woodJamBlock },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodKaekeeke, api::InstrumentSound::woodKaekeeke },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodKagul, api::InstrumentSound::woodKagul },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodKalaau, api::InstrumentSound::woodKalaau },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodKashiklar, api::InstrumentSound::woodKashiklar },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodKubing, api::InstrumentSound::woodKubing },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodPanClappers, api::InstrumentSound::woodPanClappers },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodSandBlock, api::InstrumentSound::woodSandBlock },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodSlapstick, api::InstrumentSound::woodSlapstick },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodStirDrum, api::InstrumentSound::woodStirDrum },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodTempleBlock, api::InstrumentSound::woodTempleBlock },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodTicTocBlock, api::InstrumentSound::woodTicTocBlock },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodTonetang, api::InstrumentSound::woodTonetang },
            std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::woodWoodBlock, api::InstrumentSound::woodWoodBlock },
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


        core::PlaybackSound Converter::convert( api::InstrumentSound value ) const
        {
            return findCoreItem( instrumentMap, core::PlaybackSound::keyboardPiano, value );
        }


        api::InstrumentSound Converter::convert( core::PlaybackSound value ) const
        {
            return findApiItem( instrumentMap, api::InstrumentSound::unspecified, value );
        }
        
    }
}
