// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/Converter.h"
#include "mx/core/Decimal.h"
#include "mx/core/generated/Semitones.h"
#include "mx/core/generated/TransposeGroup.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <optional>

namespace mx
{
namespace impl
{
const Converter::EnumMap<core::Step, api::Step> Converter::stepMap = {
    {core::Step::c(), api::Step::c}, {core::Step::d(), api::Step::d}, {core::Step::e(), api::Step::e},
    {core::Step::f(), api::Step::f}, {core::Step::g(), api::Step::g}, {core::Step::a(), api::Step::a},
    {core::Step::b(), api::Step::b},
};

const Converter::EnumMap<core::NoteTypeValue, api::DurationName> Converter::durationMap = {
    {core::NoteTypeValue::maxima(), api::DurationName::maxima},
    {core::NoteTypeValue::long_(), api::DurationName::longa},
    {core::NoteTypeValue::breve(), api::DurationName::breve},
    {core::NoteTypeValue::whole(), api::DurationName::whole},
    {core::NoteTypeValue::half(), api::DurationName::half},
    {core::NoteTypeValue::quarter(), api::DurationName::quarter},
    {core::NoteTypeValue::eighth(), api::DurationName::eighth},
    {core::NoteTypeValue::_16th(), api::DurationName::dur16th},
    {core::NoteTypeValue::_32nd(), api::DurationName::dur32nd},
    {core::NoteTypeValue::_64th(), api::DurationName::dur64th},
    {core::NoteTypeValue::_128th(), api::DurationName::dur128th},
    {core::NoteTypeValue::_256th(), api::DurationName::dur256th},
    {core::NoteTypeValue::_512th(), api::DurationName::dur512th},
    {core::NoteTypeValue::_1024th(), api::DurationName::dur1024th},
};

const Converter::EnumMap<core::NoteheadValue, api::Notehead> Converter::noteheadMap = {
    {core::NoteheadValue::slash(), api::Notehead::slash},
    {core::NoteheadValue::triangle(), api::Notehead::triangle},
    {core::NoteheadValue::diamond(), api::Notehead::diamond},
    {core::NoteheadValue::square(), api::Notehead::square},
    {core::NoteheadValue::cross(), api::Notehead::cross},
    {core::NoteheadValue::x(), api::Notehead::x},
    {core::NoteheadValue::circleX(), api::Notehead::circleX},
    {core::NoteheadValue::invertedTriangle(), api::Notehead::invertedTriangle},
    {core::NoteheadValue::arrowDown(), api::Notehead::arrowDown},
    {core::NoteheadValue::arrowUp(), api::Notehead::arrowUp},
    {core::NoteheadValue::slashed(), api::Notehead::slashed},
    {core::NoteheadValue::backSlashed(), api::Notehead::backSlashed},
    {core::NoteheadValue::normal(), api::Notehead::normal},
    {core::NoteheadValue::cluster(), api::Notehead::cluster},
    {core::NoteheadValue::circleDot(), api::Notehead::circleDot},
    {core::NoteheadValue::leftTriangle(), api::Notehead::leftTriangle},
    {core::NoteheadValue::rectangle(), api::Notehead::rectangle},
    {core::NoteheadValue::none(), api::Notehead::none},
    {core::NoteheadValue::do_(), api::Notehead::do_},
    {core::NoteheadValue::re(), api::Notehead::re},
    {core::NoteheadValue::mi(), api::Notehead::mi},
    {core::NoteheadValue::fa(), api::Notehead::fa},
    {core::NoteheadValue::la(), api::Notehead::la},
    {core::NoteheadValue::so(), api::Notehead::so},
    {core::NoteheadValue::ti(), api::Notehead::ti},
};

const Converter::EnumMap<core::BeamValue, api::Beam> Converter::beamMap = {
    {core::BeamValue::begin(), api::Beam::begin},
    {core::BeamValue::end(), api::Beam::end},
    {core::BeamValue::continue_(), api::Beam::extend},
    {core::BeamValue::forwardHook(), api::Beam::forwardBroken},
    {core::BeamValue::backwardHook(), api::Beam::backwardBroken},
};

const Converter::EnumMap<core::AccidentalValue, api::Accidental> Converter::accidentalMap = {
    {core::AccidentalValue::sharp(), api::Accidental::sharp},
    {core::AccidentalValue::natural(), api::Accidental::natural},
    {core::AccidentalValue::flat(), api::Accidental::flat},
    {core::AccidentalValue::doubleSharp(), api::Accidental::doubleSharp},
    {core::AccidentalValue::flatFlat(), api::Accidental::flatFlat},
    {core::AccidentalValue::naturalSharp(), api::Accidental::naturalSharp},
    {core::AccidentalValue::naturalFlat(), api::Accidental::naturalFlat},
    {core::AccidentalValue::quarterFlat(), api::Accidental::quarterFlat},
    {core::AccidentalValue::quarterSharp(), api::Accidental::quarterSharp},
    {core::AccidentalValue::threeQuartersFlat(), api::Accidental::threeQuartersFlat},
    {core::AccidentalValue::threeQuartersSharp(), api::Accidental::threeQuartersSharp},
    {core::AccidentalValue::sharpDown(), api::Accidental::sharpDown},
    {core::AccidentalValue::sharpUp(), api::Accidental::sharpUp},
    {core::AccidentalValue::naturalDown(), api::Accidental::naturalDown},
    {core::AccidentalValue::naturalUp(), api::Accidental::naturalUp},
    {core::AccidentalValue::flatDown(), api::Accidental::flatDown},
    {core::AccidentalValue::flatUp(), api::Accidental::flatUp},
    {core::AccidentalValue::tripleSharp(), api::Accidental::tripleSharp},
    {core::AccidentalValue::tripleFlat(), api::Accidental::tripleFlat},
    {core::AccidentalValue::slashQuarterSharp(), api::Accidental::slashQuarterSharp},
    {core::AccidentalValue::slashSharp(), api::Accidental::slashSharp},
    {core::AccidentalValue::slashFlat(), api::Accidental::slashFlat},
    {core::AccidentalValue::doubleSlashFlat(), api::Accidental::doubleSlashFlat},
    {core::AccidentalValue::sharp1(), api::Accidental::sharp1},
    {core::AccidentalValue::sharp2(), api::Accidental::sharp2},
    {core::AccidentalValue::sharp3(), api::Accidental::sharp3},
    {core::AccidentalValue::sharp5(), api::Accidental::sharp5},
    {core::AccidentalValue::flat1(), api::Accidental::flat1},
    {core::AccidentalValue::flat2(), api::Accidental::flat2},
    {core::AccidentalValue::flat3(), api::Accidental::flat3},
    {core::AccidentalValue::flat4(), api::Accidental::flat4},
    {core::AccidentalValue::sori(), api::Accidental::sori},
    {core::AccidentalValue::koron(), api::Accidental::koron},
};

const Converter::EnumMap<core::ClefSign, api::ClefSymbol> Converter::clefMap = {
    {core::ClefSign::c(), api::ClefSymbol::c},       {core::ClefSign::f(), api::ClefSymbol::f},
    {core::ClefSign::g(), api::ClefSymbol::g},       {core::ClefSign::jianpu(), api::ClefSymbol::jianpu},
    {core::ClefSign::none(), api::ClefSymbol::none}, {core::ClefSign::percussion(), api::ClefSymbol::percussion},
    {core::ClefSign::tab(), api::ClefSymbol::tab}};

const Converter::EnumMap<core::AboveBelow, api::Placement> Converter::placementMap = {
    {core::AboveBelow::above(), api::Placement::above},
    {core::AboveBelow::below(), api::Placement::below},
};

const Converter::EnumMap<core::YesNo, api::Bool> Converter::boolMap = {
    {core::YesNo::yes(), api::Bool::yes},
    {core::YesNo::no(), api::Bool::no},
};

const Converter::EnumMap<core::Valign, api::VerticalAlignment> Converter::valignMap = {
    {core::Valign::baseline(), api::VerticalAlignment::baseline},
    {core::Valign::bottom(), api::VerticalAlignment::bottom},
    {core::Valign::middle(), api::VerticalAlignment::middle},
    {core::Valign::top(), api::VerticalAlignment::top},
};

const Converter::EnumMap<core::LeftCenterRight, api::HorizontalAlignment> Converter::halignMap = {
    {core::LeftCenterRight::left(), api::HorizontalAlignment::left},
    {core::LeftCenterRight::center(), api::HorizontalAlignment::center},
    {core::LeftCenterRight::right(), api::HorizontalAlignment::right},
};

const Converter::EnumMap<core::CSSFontSize, api::CssSize> Converter::cssMap = {
    {core::CSSFontSize::xxSmall(), api::CssSize::xxSmall}, {core::CSSFontSize::xSmall(), api::CssSize::xSmall},
    {core::CSSFontSize::small(), api::CssSize::small},     {core::CSSFontSize::medium(), api::CssSize::medium},
    {core::CSSFontSize::large(), api::CssSize::large},     {core::CSSFontSize::xLarge(), api::CssSize::xLarge},
    {core::CSSFontSize::xxLarge(), api::CssSize::xxLarge},
};

// TODO - SMUFLKILL
const Converter::EnumMap<core::ArticulationsChoice::Kind, api::MarkType> Converter::articulationsMap = {
    {core::ArticulationsChoice::Kind::accent, api::MarkType::accent},
    {core::ArticulationsChoice::Kind::strongAccent, api::MarkType::strongAccent},
    {core::ArticulationsChoice::Kind::staccato, api::MarkType::staccato},
    {core::ArticulationsChoice::Kind::tenuto, api::MarkType::tenuto},
    {core::ArticulationsChoice::Kind::detachedLegato, api::MarkType::detachedLegato},
    {core::ArticulationsChoice::Kind::staccatissimo, api::MarkType::staccatissimo},
    {core::ArticulationsChoice::Kind::spiccato, api::MarkType::spiccato},
    {core::ArticulationsChoice::Kind::scoop, api::MarkType::scoop},
    {core::ArticulationsChoice::Kind::plop, api::MarkType::plop},
    {core::ArticulationsChoice::Kind::doit, api::MarkType::doit},
    {core::ArticulationsChoice::Kind::falloff, api::MarkType::falloff},
    {core::ArticulationsChoice::Kind::breathMark, api::MarkType::breathMark},
    {core::ArticulationsChoice::Kind::caesura, api::MarkType::caesura},
    {core::ArticulationsChoice::Kind::stress, api::MarkType::stress},
    {core::ArticulationsChoice::Kind::unstress, api::MarkType::unstress},
    {core::ArticulationsChoice::Kind::otherArticulation, api::MarkType::otherArticulation},
};

const Converter::EnumMap<core::DynamicsChoice::Kind, api::MarkType> Converter::dynamicsMap = {
    {core::DynamicsChoice::Kind::p, api::MarkType::p},
    {core::DynamicsChoice::Kind::pp, api::MarkType::pp},
    {core::DynamicsChoice::Kind::ppp, api::MarkType::ppp},
    {core::DynamicsChoice::Kind::pppp, api::MarkType::pppp},
    {core::DynamicsChoice::Kind::ppppp, api::MarkType::ppppp},
    {core::DynamicsChoice::Kind::pppppp, api::MarkType::pppppp},
    {core::DynamicsChoice::Kind::f, api::MarkType::f},
    {core::DynamicsChoice::Kind::ff, api::MarkType::ff},
    {core::DynamicsChoice::Kind::fff, api::MarkType::fff},
    {core::DynamicsChoice::Kind::ffff, api::MarkType::ffff},
    {core::DynamicsChoice::Kind::fffff, api::MarkType::fffff},
    {core::DynamicsChoice::Kind::ffffff, api::MarkType::ffffff},
    {core::DynamicsChoice::Kind::mp, api::MarkType::mp},
    {core::DynamicsChoice::Kind::mf, api::MarkType::mf},
    {core::DynamicsChoice::Kind::sf, api::MarkType::sf},
    {core::DynamicsChoice::Kind::sfp, api::MarkType::sfp},
    {core::DynamicsChoice::Kind::sfpp, api::MarkType::sfpp},
    {core::DynamicsChoice::Kind::fp, api::MarkType::fp},
    {core::DynamicsChoice::Kind::rf, api::MarkType::rf},
    {core::DynamicsChoice::Kind::rfz, api::MarkType::rfz},
    {core::DynamicsChoice::Kind::sfz, api::MarkType::sfz},
    {core::DynamicsChoice::Kind::sffz, api::MarkType::sffz},
    {core::DynamicsChoice::Kind::fz, api::MarkType::fz},
    {core::DynamicsChoice::Kind::otherDynamics, api::MarkType::otherDynamics},
};

const Converter::EnumMap<core::OrnamentsGroupChoice::Kind, api::MarkType> Converter::ornamentsMap = {
    {core::OrnamentsGroupChoice::Kind::trillMark, api::MarkType::trillMark},
    {core::OrnamentsGroupChoice::Kind::turn, api::MarkType::turn},
    {core::OrnamentsGroupChoice::Kind::delayedTurn, api::MarkType::delayedTurn},
    {core::OrnamentsGroupChoice::Kind::invertedTurn, api::MarkType::invertedTurn},
    {core::OrnamentsGroupChoice::Kind::delayedInvertedTurn, api::MarkType::delayedInvertedTurn},
    {core::OrnamentsGroupChoice::Kind::verticalTurn, api::MarkType::verticalTurn},
    {core::OrnamentsGroupChoice::Kind::shake, api::MarkType::shake},
    {core::OrnamentsGroupChoice::Kind::wavyLine, api::MarkType::wavyLine},
    {core::OrnamentsGroupChoice::Kind::mordent, api::MarkType::mordent},
    {core::OrnamentsGroupChoice::Kind::invertedMordent, api::MarkType::invertedMordent},
    {core::OrnamentsGroupChoice::Kind::schleifer, api::MarkType::schleifer},
    {core::OrnamentsGroupChoice::Kind::tremolo, api::MarkType::tremoloSingleThree},
    {core::OrnamentsGroupChoice::Kind::otherOrnament, api::MarkType::otherOrnament},
};

const Converter::EnumMap<core::AccidentalValue, api::MarkType> Converter::accidentalMarkMap = {
    {core::AccidentalValue::sharp(), api::MarkType::accidentalMarkSharp},
    {core::AccidentalValue::natural(), api::MarkType::accidentalMarkNatural},
    {core::AccidentalValue::flat(), api::MarkType::accidentalMarkFlat},
    {core::AccidentalValue::doubleSharp(), api::MarkType::accidentalMarkDoubleSharp},
    {core::AccidentalValue::sharpSharp(), api::MarkType::accidentalMarkSharpSharp},
    {core::AccidentalValue::flatFlat(), api::MarkType::accidentalMarkFlatFlat},
    {core::AccidentalValue::naturalSharp(), api::MarkType::accidentalMarkNaturalSharp},
    {core::AccidentalValue::naturalFlat(), api::MarkType::accidentalMarkNaturalFlat},
    {core::AccidentalValue::quarterFlat(), api::MarkType::accidentalMarkQuarterFlat},
    {core::AccidentalValue::quarterSharp(), api::MarkType::accidentalMarkQuarterSharp},
    {core::AccidentalValue::threeQuartersFlat(), api::MarkType::accidentalMarkThreeQuartersFlat},
    {core::AccidentalValue::threeQuartersSharp(), api::MarkType::accidentalMarkThreeQuartersSharp},
    {core::AccidentalValue::sharpDown(), api::MarkType::accidentalMarkSharpDown},
    {core::AccidentalValue::sharpUp(), api::MarkType::accidentalMarkSharpUp},
    {core::AccidentalValue::naturalDown(), api::MarkType::accidentalMarkNaturalDown},
    {core::AccidentalValue::naturalUp(), api::MarkType::accidentalMarkNaturalUp},
    {core::AccidentalValue::flatDown(), api::MarkType::accidentalMarkFlatDown},
    {core::AccidentalValue::flatUp(), api::MarkType::accidentalMarkFlatUp},
    {core::AccidentalValue::tripleSharp(), api::MarkType::accidentalMarkTripleSharp},
    {core::AccidentalValue::tripleFlat(), api::MarkType::accidentalMarkTripleFlat},
    {core::AccidentalValue::slashQuarterSharp(), api::MarkType::accidentalMarkSlashQuarterSharp},
    {core::AccidentalValue::slashSharp(), api::MarkType::accidentalMarkSlashSharp},
    {core::AccidentalValue::slashFlat(), api::MarkType::accidentalMarkSlashFlat},
    {core::AccidentalValue::doubleSlashFlat(), api::MarkType::accidentalMarkDoubleSlashFlat},
    {core::AccidentalValue::sharp1(), api::MarkType::accidentalMarkSharp1},
    {core::AccidentalValue::sharp2(), api::MarkType::accidentalMarkSharp2},
    {core::AccidentalValue::sharp3(), api::MarkType::accidentalMarkSharp3},
    {core::AccidentalValue::sharp5(), api::MarkType::accidentalMarkSharp5},
    {core::AccidentalValue::flat1(), api::MarkType::accidentalMarkFlat1},
    {core::AccidentalValue::flat2(), api::MarkType::accidentalMarkFlat2},
    {core::AccidentalValue::flat3(), api::MarkType::accidentalMarkFlat3},
    {core::AccidentalValue::flat4(), api::MarkType::accidentalMarkFlat4},
    {core::AccidentalValue::sori(), api::MarkType::accidentalMarkSori},
    {core::AccidentalValue::koron(), api::MarkType::accidentalMarkKoron},
};

const Converter::EnumMap<core::FontStyle, api::FontStyle> Converter::fontStyleMap = {
    {core::FontStyle::normal(), api::FontStyle::normal},
    {core::FontStyle::italic(), api::FontStyle::italic},
};

const Converter::EnumMap<core::FontWeight, api::FontWeight> Converter::fontWeightMap = {
    {core::FontWeight::normal(), api::FontWeight::normal},
    {core::FontWeight::bold(), api::FontWeight::bold},
};

const Converter::EnumMap<core::MeasureNumberingValue, api::MeasureNumbering> Converter::measureNumberingMap = {
    {core::MeasureNumberingValue::measure(), api::MeasureNumbering::measure},
    {core::MeasureNumberingValue::none(), api::MeasureNumbering::none},
    {core::MeasureNumberingValue::system(), api::MeasureNumbering::system},
};

const Converter::EnumMap<core::TechnicalChoice::Kind, api::MarkType> Converter::technicalMarkMap = {
    // { core::TechnicalChoice::Kind::technical,
    // api::MarkType::unspecified },
    {core::TechnicalChoice::Kind::upBow, api::MarkType::upBow},
    {core::TechnicalChoice::Kind::downBow, api::MarkType::downBow},
    {core::TechnicalChoice::Kind::harmonic, api::MarkType::harmonic},
    {core::TechnicalChoice::Kind::openString, api::MarkType::openString},
    {core::TechnicalChoice::Kind::thumbPosition, api::MarkType::thumbPosition},
    // { core::TechnicalChoice::Kind::fingering,
    // api::MarkType::unspecified }, {
    // core::TechnicalChoice::Kind::pluck, api::MarkType::unspecified },
    {core::TechnicalChoice::Kind::doubleTongue, api::MarkType::doubleTongue},
    {core::TechnicalChoice::Kind::tripleTongue, api::MarkType::tripleTongue},
    {core::TechnicalChoice::Kind::stopped, api::MarkType::stopped},
    {core::TechnicalChoice::Kind::snapPizzicato, api::MarkType::snapPizzicato},
    {core::TechnicalChoice::Kind::fret, api::MarkType::fret},
    {core::TechnicalChoice::Kind::string, api::MarkType::string_},
    // { core::TechnicalChoice::Kind::hammerOn,
    // api::MarkType::unspecified }, {
    // core::TechnicalChoice::Kind::pullOff, api::MarkType::unspecified }, std::pair<core::TechnicalChoice::Kind,
    // api::MarkType>{ core::TechnicalChoice::Kind::bend, api::MarkType::unspecified },
    // { core::TechnicalChoice::Kind::tap,
    // api::MarkType::unspecified },
    {core::TechnicalChoice::Kind::heel, api::MarkType::heel},
    {core::TechnicalChoice::Kind::toe, api::MarkType::toe},
    {core::TechnicalChoice::Kind::fingernails, api::MarkType::fingernails},
    {core::TechnicalChoice::Kind::hole, api::MarkType::hole},
    {core::TechnicalChoice::Kind::arrow, api::MarkType::arrow},
    {core::TechnicalChoice::Kind::handbell, api::MarkType::handbell},
    {core::TechnicalChoice::Kind::otherTechnical, api::MarkType::otherTechnical},
};

const Converter::EnumMap<core::StemValue, api::Stem> Converter::stemMap = {
    {core::StemValue::up(), api::Stem::up},
    {core::StemValue::down(), api::Stem::down},
    {core::StemValue::none(), api::Stem::none},
    {core::StemValue::double_(), api::Stem::both},
};

const Converter::EnumMap<core::LineType, api::LineType> Converter::lineType = {
    {core::LineType::dashed(), api::LineType::dashed},
    {core::LineType::dotted(), api::LineType::dotted},
    {core::LineType::solid(), api::LineType::solid},
    {core::LineType::wavy(), api::LineType::wavy},
};

const Converter::EnumMap<core::WedgeType, api::WedgeType> Converter::wedgeMap = {
    {core::WedgeType::crescendo(), api::WedgeType::crescendo},
    {core::WedgeType::diminuendo(), api::WedgeType::diminuendo},
};

const Converter::EnumMap<core::BarStyle, api::BarlineType> Converter::barlineMap = {
    {core::BarStyle::regular(), api::BarlineType::normal},
    {core::BarStyle::dotted(), api::BarlineType::dotted},
    {core::BarStyle::dashed(), api::BarlineType::dashed},
    {core::BarStyle::heavy(), api::BarlineType::heavy},
    {core::BarStyle::lightLight(), api::BarlineType::lightLight},
    {core::BarStyle::lightHeavy(), api::BarlineType::lightHeavy},
    {core::BarStyle::heavyLight(), api::BarlineType::heavyLight},
    {core::BarStyle::heavyHeavy(), api::BarlineType::heavyHeavy},
    {core::BarStyle::tick(), api::BarlineType::unsupported},
    {core::BarStyle::short_(), api::BarlineType::unsupported},
    {core::BarStyle::none(), api::BarlineType::none},
};

const Converter::EnumMap<core::LineEnd, api::LineHook> Converter::lineStopMap = {
    {core::LineEnd::arrow(), api::LineHook::arrow}, {core::LineEnd::both(), api::LineHook::both},
    {core::LineEnd::down(), api::LineHook::down},   {core::LineEnd::up(), api::LineHook::up},
    {core::LineEnd::none(), api::LineHook::none},
};

const Converter::EnumMap<core::GroupSymbolValue, api::BracketType> Converter::bracketMap = {
    {core::GroupSymbolValue::brace(), api::BracketType::brace},
    {core::GroupSymbolValue::bracket(), api::BracketType::bracket},
    {core::GroupSymbolValue::line(), api::BracketType::line},
    {core::GroupSymbolValue::none(), api::BracketType::none},
    {core::GroupSymbolValue::square(), api::BracketType::square},
};

const Converter::EnumMap<core::RightLeftMiddle, api::HorizontalAlignment> Converter::barlinePlacementMap = {
    {core::RightLeftMiddle::right(), api::HorizontalAlignment::right},
    {core::RightLeftMiddle::left(), api::HorizontalAlignment::left},
    {core::RightLeftMiddle::middle(), api::HorizontalAlignment::center},
};

const Converter::EnumMap<core::StartStopDiscontinue, api::EndingType> Converter::endingMap = {
    {core::StartStopDiscontinue::start(), api::EndingType::start},
    {core::StartStopDiscontinue::stop(), api::EndingType::stop},
    {core::StartStopDiscontinue::discontinue(), api::EndingType::discontinue}};

const Converter::EnumMap<core::FermataShape, api::MarkType> Converter::fermataMap = {
    {core::FermataShape::normal(), api::MarkType::fermataNormal},
    {core::FermataShape::angled(), api::MarkType::fermataAngled},
    {core::FermataShape::square(), api::MarkType::fermataSquare},
    {core::FermataShape::empty(), api::MarkType::fermata},
};

const Converter::EnumMap<core::SoundID, api::SoundID> Converter::instrumentMap = {
    {core::SoundID::brassAlphorn(), api::SoundID::brassAlphorn},
    {core::SoundID::brassAltoHorn(), api::SoundID::brassAltoHorn},
    {core::SoundID::brassBaritoneHorn(), api::SoundID::brassBaritoneHorn},
    {core::SoundID::brassBugle(), api::SoundID::brassBugle},
    {core::SoundID::brassBugleAlto(), api::SoundID::brassBugleAlto},
    {core::SoundID::brassBugleBaritone(), api::SoundID::brassBugleBaritone},
    {core::SoundID::brassBugleContrabass(), api::SoundID::brassBugleContrabass},
    {core::SoundID::brassBugleEuphoniumBugle(), api::SoundID::brassBugleEuphoniumBugle},
    {core::SoundID::brassBugleMellophoneBugle(), api::SoundID::brassBugleMellophoneBugle},
    {core::SoundID::brassBugleSoprano(), api::SoundID::brassBugleSoprano},
    {core::SoundID::brassCimbasso(), api::SoundID::brassCimbasso},
    {core::SoundID::brassConchShell(), api::SoundID::brassConchShell},
    {core::SoundID::brassCornet(), api::SoundID::brassCornet},
    {core::SoundID::brassCornetSoprano(), api::SoundID::brassCornetSoprano},
    {core::SoundID::brassCornett(), api::SoundID::brassCornett},
    {core::SoundID::brassCornettTenor(), api::SoundID::brassCornettTenor},
    {core::SoundID::brassCornettino(), api::SoundID::brassCornettino},
    {core::SoundID::brassDidgeridoo(), api::SoundID::brassDidgeridoo},
    {core::SoundID::brassEuphonium(), api::SoundID::brassEuphonium},
    {core::SoundID::brassFiscorn(), api::SoundID::brassFiscorn},
    {core::SoundID::brassFlugelhorn(), api::SoundID::brassFlugelhorn},
    {core::SoundID::brassFrenchHorn(), api::SoundID::brassFrenchHorn},
    {core::SoundID::brassGroup(), api::SoundID::brassGroup},
    {core::SoundID::brassGroupSynth(), api::SoundID::brassGroupSynth},
    {core::SoundID::brassHelicon(), api::SoundID::brassHelicon},
    {core::SoundID::brassHoragai(), api::SoundID::brassHoragai},
    {core::SoundID::brassKuhlohorn(), api::SoundID::brassKuhlohorn},
    {core::SoundID::brassMellophone(), api::SoundID::brassMellophone},
    {core::SoundID::brassNaturalHorn(), api::SoundID::brassNaturalHorn},
    {core::SoundID::brassOphicleide(), api::SoundID::brassOphicleide},
    {core::SoundID::brassPosthorn(), api::SoundID::brassPosthorn},
    {core::SoundID::brassRagDung(), api::SoundID::brassRagDung},
    {core::SoundID::brassSackbutt(), api::SoundID::brassSackbutt},
    {core::SoundID::brassSackbuttAlto(), api::SoundID::brassSackbuttAlto},
    {core::SoundID::brassSackbuttBass(), api::SoundID::brassSackbuttBass},
    {core::SoundID::brassSackbuttTenor(), api::SoundID::brassSackbuttTenor},
    {core::SoundID::brassSaxhorn(), api::SoundID::brassSaxhorn},
    {core::SoundID::brassSerpent(), api::SoundID::brassSerpent},
    {core::SoundID::brassShofar(), api::SoundID::brassShofar},
    {core::SoundID::brassSousaphone(), api::SoundID::brassSousaphone},
    {core::SoundID::brassTrombone(), api::SoundID::brassTrombone},
    {core::SoundID::brassTromboneAlto(), api::SoundID::brassTromboneAlto},
    {core::SoundID::brassTromboneBass(), api::SoundID::brassTromboneBass},
    {core::SoundID::brassTromboneContrabass(), api::SoundID::brassTromboneContrabass},
    {core::SoundID::brassTromboneTenor(), api::SoundID::brassTromboneTenor},
    {core::SoundID::brassTrumpet(), api::SoundID::brassTrumpet},
    {core::SoundID::brassTrumpetBaroque(), api::SoundID::brassTrumpetBaroque},
    {core::SoundID::brassTrumpetBass(), api::SoundID::brassTrumpetBass},
    {core::SoundID::brassTrumpetBflat(), api::SoundID::brassTrumpetBflat},
    {core::SoundID::brassTrumpetC(), api::SoundID::brassTrumpetC},
    {core::SoundID::brassTrumpetD(), api::SoundID::brassTrumpetD},
    {core::SoundID::brassTrumpetPiccolo(), api::SoundID::brassTrumpetPiccolo},
    {core::SoundID::brassTrumpetPocket(), api::SoundID::brassTrumpetPocket},
    {core::SoundID::brassTrumpetSlide(), api::SoundID::brassTrumpetSlide},
    {core::SoundID::brassTrumpetTenor(), api::SoundID::brassTrumpetTenor},
    {core::SoundID::brassTuba(), api::SoundID::brassTuba},
    {core::SoundID::brassTubaBass(), api::SoundID::brassTubaBass},
    {core::SoundID::brassTubaSubcontrabass(), api::SoundID::brassTubaSubcontrabass},
    {core::SoundID::brassViennaHorn(), api::SoundID::brassViennaHorn},
    {core::SoundID::brassVuvuzela(), api::SoundID::brassVuvuzela},
    {core::SoundID::brassWagnerTuba(), api::SoundID::brassWagnerTuba},
    {core::SoundID::drumApentemma(), api::SoundID::drumApentemma},
    {core::SoundID::drumAshiko(), api::SoundID::drumAshiko},
    {core::SoundID::drumAtabaque(), api::SoundID::drumAtabaque},
    {core::SoundID::drumAtoke(), api::SoundID::drumAtoke},
    {core::SoundID::drumAtsimevu(), api::SoundID::drumAtsimevu},
    {core::SoundID::drumAxatse(), api::SoundID::drumAxatse},
    {core::SoundID::drumBassDrum(), api::SoundID::drumBassDrum},
    {core::SoundID::drumBata(), api::SoundID::drumBata},
    {core::SoundID::drumBataItotele(), api::SoundID::drumBataItotele},
    {core::SoundID::drumBataIya(), api::SoundID::drumBataIya},
    {core::SoundID::drumBataOkonkolo(), api::SoundID::drumBataOkonkolo},
    {core::SoundID::drumBendir(), api::SoundID::drumBendir},
    {core::SoundID::drumBodhran(), api::SoundID::drumBodhran},
    {core::SoundID::drumBombo(), api::SoundID::drumBombo},
    {core::SoundID::drumBongo(), api::SoundID::drumBongo},
    {core::SoundID::drumBougarabou(), api::SoundID::drumBougarabou},
    {core::SoundID::drumBuffaloDrum(), api::SoundID::drumBuffaloDrum},
    {core::SoundID::drumCajon(), api::SoundID::drumCajon},
    {core::SoundID::drumChenda(), api::SoundID::drumChenda},
    {core::SoundID::drumChuDaiko(), api::SoundID::drumChuDaiko},
    {core::SoundID::drumConga(), api::SoundID::drumConga},
    {core::SoundID::drumCuica(), api::SoundID::drumCuica},
    {core::SoundID::drumDabakan(), api::SoundID::drumDabakan},
    {core::SoundID::drumDaff(), api::SoundID::drumDaff},
    {core::SoundID::drumDafli(), api::SoundID::drumDafli},
    {core::SoundID::drumDaibyosi(), api::SoundID::drumDaibyosi},
    {core::SoundID::drumDamroo(), api::SoundID::drumDamroo},
    {core::SoundID::drumDarabuka(), api::SoundID::drumDarabuka},
    {core::SoundID::drumDef(), api::SoundID::drumDef},
    {core::SoundID::drumDhol(), api::SoundID::drumDhol},
    {core::SoundID::drumDholak(), api::SoundID::drumDholak},
    {core::SoundID::drumDjembe(), api::SoundID::drumDjembe},
    {core::SoundID::drumDoira(), api::SoundID::drumDoira},
    {core::SoundID::drumDondo(), api::SoundID::drumDondo},
    {core::SoundID::drumDounDounBa(), api::SoundID::drumDounDounBa},
    {core::SoundID::drumDuff(), api::SoundID::drumDuff},
    {core::SoundID::drumDumbek(), api::SoundID::drumDumbek},
    {core::SoundID::drumFontomfrom(), api::SoundID::drumFontomfrom},
    {core::SoundID::drumFrameDrum(), api::SoundID::drumFrameDrum},
    {core::SoundID::drumFrameDrumArabian(), api::SoundID::drumFrameDrumArabian},
    {core::SoundID::drumGeduk(), api::SoundID::drumGeduk},
    {core::SoundID::drumGhatam(), api::SoundID::drumGhatam},
    {core::SoundID::drumGome(), api::SoundID::drumGome},
    {core::SoundID::drumGroup(), api::SoundID::drumGroup},
    {core::SoundID::drumGroupChinese(), api::SoundID::drumGroupChinese},
    {core::SoundID::drumGroupEwe(), api::SoundID::drumGroupEwe},
    {core::SoundID::drumGroupIndian(), api::SoundID::drumGroupIndian},
    {core::SoundID::drumGroupSet(), api::SoundID::drumGroupSet},
    {core::SoundID::drumHandDrum(), api::SoundID::drumHandDrum},
    {core::SoundID::drumHiraDaiko(), api::SoundID::drumHiraDaiko},
    {core::SoundID::drumIbo(), api::SoundID::drumIbo},
    {core::SoundID::drumIgihumurizo(), api::SoundID::drumIgihumurizo},
    {core::SoundID::drumInyahura(), api::SoundID::drumInyahura},
    {core::SoundID::drumIshakwe(), api::SoundID::drumIshakwe},
    {core::SoundID::drumJangGu(), api::SoundID::drumJangGu},
    {core::SoundID::drumKagan(), api::SoundID::drumKagan},
    {core::SoundID::drumKakko(), api::SoundID::drumKakko},
    {core::SoundID::drumKanjira(), api::SoundID::drumKanjira},
    {core::SoundID::drumKendhang(), api::SoundID::drumKendhang},
    {core::SoundID::drumKendhangAgeng(), api::SoundID::drumKendhangAgeng},
    {core::SoundID::drumKendhangCiblon(), api::SoundID::drumKendhangCiblon},
    {core::SoundID::drumKenkeni(), api::SoundID::drumKenkeni},
    {core::SoundID::drumKhol(), api::SoundID::drumKhol},
    {core::SoundID::drumKickDrum(), api::SoundID::drumKickDrum},
    {core::SoundID::drumKidi(), api::SoundID::drumKidi},
    {core::SoundID::drumKoDaiko(), api::SoundID::drumKoDaiko},
    {core::SoundID::drumKpanlogo(), api::SoundID::drumKpanlogo},
    {core::SoundID::drumKudum(), api::SoundID::drumKudum},
    {core::SoundID::drumLambeg(), api::SoundID::drumLambeg},
    {core::SoundID::drumLionDrum(), api::SoundID::drumLionDrum},
    {core::SoundID::drumLogDrum(), api::SoundID::drumLogDrum},
    {core::SoundID::drumLogDrumAfrican(), api::SoundID::drumLogDrumAfrican},
    {core::SoundID::drumLogDrumNative(), api::SoundID::drumLogDrumNative},
    {core::SoundID::drumLogDrumNigerian(), api::SoundID::drumLogDrumNigerian},
    {core::SoundID::drumMadal(), api::SoundID::drumMadal},
    {core::SoundID::drumMaddale(), api::SoundID::drumMaddale},
    {core::SoundID::drumMridangam(), api::SoundID::drumMridangam},
    {core::SoundID::drumNaal(), api::SoundID::drumNaal},
    {core::SoundID::drumNagadoDaiko(), api::SoundID::drumNagadoDaiko},
    {core::SoundID::drumNagara(), api::SoundID::drumNagara},
    {core::SoundID::drumNaqara(), api::SoundID::drumNaqara},
    {core::SoundID::drumODaiko(), api::SoundID::drumODaiko},
    {core::SoundID::drumOkawa(), api::SoundID::drumOkawa},
    {core::SoundID::drumOkedoDaiko(), api::SoundID::drumOkedoDaiko},
    {core::SoundID::drumPahuHula(), api::SoundID::drumPahuHula},
    {core::SoundID::drumPakhawaj(), api::SoundID::drumPakhawaj},
    {core::SoundID::drumPandeiro(), api::SoundID::drumPandeiro},
    {core::SoundID::drumPandero(), api::SoundID::drumPandero},
    {core::SoundID::drumPowwow(), api::SoundID::drumPowwow},
    {core::SoundID::drumPueblo(), api::SoundID::drumPueblo},
    {core::SoundID::drumRepinique(), api::SoundID::drumRepinique},
    {core::SoundID::drumRiq(), api::SoundID::drumRiq},
    {core::SoundID::drumRototom(), api::SoundID::drumRototom},
    {core::SoundID::drumSabar(), api::SoundID::drumSabar},
    {core::SoundID::drumSakara(), api::SoundID::drumSakara},
    {core::SoundID::drumSampho(), api::SoundID::drumSampho},
    {core::SoundID::drumSangban(), api::SoundID::drumSangban},
    {core::SoundID::drumShimeDaiko(), api::SoundID::drumShimeDaiko},
    {core::SoundID::drumSlitDrum(), api::SoundID::drumSlitDrum},
    {core::SoundID::drumSlitDrumKrin(), api::SoundID::drumSlitDrumKrin},
    {core::SoundID::drumSnareDrum(), api::SoundID::drumSnareDrum},
    {core::SoundID::drumSnareDrumElectric(), api::SoundID::drumSnareDrumElectric},
    {core::SoundID::drumSogo(), api::SoundID::drumSogo},
    {core::SoundID::drumSurdo(), api::SoundID::drumSurdo},
    {core::SoundID::drumTabla(), api::SoundID::drumTabla},
    {core::SoundID::drumTablaBayan(), api::SoundID::drumTablaBayan},
    {core::SoundID::drumTablaDayan(), api::SoundID::drumTablaDayan},
    {core::SoundID::drumTaiko(), api::SoundID::drumTaiko},
    {core::SoundID::drumTalking(), api::SoundID::drumTalking},
    {core::SoundID::drumTama(), api::SoundID::drumTama},
    {core::SoundID::drumTamborita(), api::SoundID::drumTamborita},
    {core::SoundID::drumTambourine(), api::SoundID::drumTambourine},
    {core::SoundID::drumTamte(), api::SoundID::drumTamte},
    {core::SoundID::drumTangku(), api::SoundID::drumTangku},
    {core::SoundID::drumTanTan(), api::SoundID::drumTanTan},
    {core::SoundID::drumTaphon(), api::SoundID::drumTaphon},
    {core::SoundID::drumTar(), api::SoundID::drumTar},
    {core::SoundID::drumTasha(), api::SoundID::drumTasha},
    {core::SoundID::drumTenorDrum(), api::SoundID::drumTenorDrum},
    {core::SoundID::drumTeponaxtli(), api::SoundID::drumTeponaxtli},
    {core::SoundID::drumThavil(), api::SoundID::drumThavil},
    {core::SoundID::drumTheBox(), api::SoundID::drumTheBox},
    {core::SoundID::drumTimbale(), api::SoundID::drumTimbale},
    {core::SoundID::drumTimpani(), api::SoundID::drumTimpani},
    {core::SoundID::drumTinaja(), api::SoundID::drumTinaja},
    {core::SoundID::drumToere(), api::SoundID::drumToere},
    {core::SoundID::drumTombak(), api::SoundID::drumTombak},
    {core::SoundID::drumTomTom(), api::SoundID::drumTomTom},
    {core::SoundID::drumTomTomSynth(), api::SoundID::drumTomTomSynth},
    {core::SoundID::drumTsuzumi(), api::SoundID::drumTsuzumi},
    {core::SoundID::drumTumbak(), api::SoundID::drumTumbak},
    {core::SoundID::drumUchiwaDaiko(), api::SoundID::drumUchiwaDaiko},
    {core::SoundID::drumUdaku(), api::SoundID::drumUdaku},
    {core::SoundID::drumUdu(), api::SoundID::drumUdu},
    {core::SoundID::drumZarb(), api::SoundID::drumZarb},
    {core::SoundID::effectAeolianHarp(), api::SoundID::effectAeolianHarp},
    {core::SoundID::effectAirHorn(), api::SoundID::effectAirHorn},
    {core::SoundID::effectApplause(), api::SoundID::effectApplause},
    {core::SoundID::effectBassStringSlap(), api::SoundID::effectBassStringSlap},
    {core::SoundID::effectBird(), api::SoundID::effectBird},
    {core::SoundID::effectBirdNightingale(), api::SoundID::effectBirdNightingale},
    {core::SoundID::effectBirdTweet(), api::SoundID::effectBirdTweet},
    {core::SoundID::effectBreath(), api::SoundID::effectBreath},
    {core::SoundID::effectBubble(), api::SoundID::effectBubble},
    {core::SoundID::effectBullroarer(), api::SoundID::effectBullroarer},
    {core::SoundID::effectBurst(), api::SoundID::effectBurst},
    {core::SoundID::effectCar(), api::SoundID::effectCar},
    {core::SoundID::effectCarCrash(), api::SoundID::effectCarCrash},
    {core::SoundID::effectCarEngine(), api::SoundID::effectCarEngine},
    {core::SoundID::effectCarPass(), api::SoundID::effectCarPass},
    {core::SoundID::effectCarStop(), api::SoundID::effectCarStop},
    {core::SoundID::effectCrickets(), api::SoundID::effectCrickets},
    {core::SoundID::effectDog(), api::SoundID::effectDog},
    {core::SoundID::effectDoorCreak(), api::SoundID::effectDoorCreak},
    {core::SoundID::effectDoorSlam(), api::SoundID::effectDoorSlam},
    {core::SoundID::effectExplosion(), api::SoundID::effectExplosion},
    {core::SoundID::effectFluteKeyClick(), api::SoundID::effectFluteKeyClick},
    {core::SoundID::effectFootsteps(), api::SoundID::effectFootsteps},
    {core::SoundID::effectFrogs(), api::SoundID::effectFrogs},
    {core::SoundID::effectGuitarCutting(), api::SoundID::effectGuitarCutting},
    {core::SoundID::effectGuitarFret(), api::SoundID::effectGuitarFret},
    {core::SoundID::effectGunshot(), api::SoundID::effectGunshot},
    {core::SoundID::effectHandClap(), api::SoundID::effectHandClap},
    {core::SoundID::effectHeartbeat(), api::SoundID::effectHeartbeat},
    {core::SoundID::effectHelicopter(), api::SoundID::effectHelicopter},
    {core::SoundID::effectHighQ(), api::SoundID::effectHighQ},
    {core::SoundID::effectHorseGallop(), api::SoundID::effectHorseGallop},
    {core::SoundID::effectJetPlane(), api::SoundID::effectJetPlane},
    {core::SoundID::effectLaserGun(), api::SoundID::effectLaserGun},
    {core::SoundID::effectLaugh(), api::SoundID::effectLaugh},
    {core::SoundID::effectLionsRoar(), api::SoundID::effectLionsRoar},
    {core::SoundID::effectMachineGun(), api::SoundID::effectMachineGun},
    {core::SoundID::effectMarchingMachine(), api::SoundID::effectMarchingMachine},
    {core::SoundID::effectMetronomeBell(), api::SoundID::effectMetronomeBell},
    {core::SoundID::effectMetronomeClick(), api::SoundID::effectMetronomeClick},
    {core::SoundID::effectPat(), api::SoundID::effectPat},
    {core::SoundID::effectPunch(), api::SoundID::effectPunch},
    {core::SoundID::effectRain(), api::SoundID::effectRain},
    {core::SoundID::effectScratch(), api::SoundID::effectScratch},
    {core::SoundID::effectScream(), api::SoundID::effectScream},
    {core::SoundID::effectSeashore(), api::SoundID::effectSeashore},
    {core::SoundID::effectSiren(), api::SoundID::effectSiren},
    {core::SoundID::effectSlap(), api::SoundID::effectSlap},
    {core::SoundID::effectSnap(), api::SoundID::effectSnap},
    {core::SoundID::effectStamp(), api::SoundID::effectStamp},
    {core::SoundID::effectStarship(), api::SoundID::effectStarship},
    {core::SoundID::effectStream(), api::SoundID::effectStream},
    {core::SoundID::effectTelephoneRing(), api::SoundID::effectTelephoneRing},
    {core::SoundID::effectThunder(), api::SoundID::effectThunder},
    {core::SoundID::effectTrain(), api::SoundID::effectTrain},
    {core::SoundID::effectTrashCan(), api::SoundID::effectTrashCan},
    {core::SoundID::effectWhip(), api::SoundID::effectWhip},
    {core::SoundID::effectWhistle(), api::SoundID::effectWhistle},
    {core::SoundID::effectWhistleMouthSiren(), api::SoundID::effectWhistleMouthSiren},
    {core::SoundID::effectWhistlePolice(), api::SoundID::effectWhistlePolice},
    {core::SoundID::effectWhistleSlide(), api::SoundID::effectWhistleSlide},
    {core::SoundID::effectWhistleTrain(), api::SoundID::effectWhistleTrain},
    {core::SoundID::effectWind(), api::SoundID::effectWind},
    {core::SoundID::keyboardAccordion(), api::SoundID::keyboardAccordion},
    {core::SoundID::keyboardBandoneon(), api::SoundID::keyboardBandoneon},
    {core::SoundID::keyboardCelesta(), api::SoundID::keyboardCelesta},
    {core::SoundID::keyboardClavichord(), api::SoundID::keyboardClavichord},
    {core::SoundID::keyboardClavichordSynth(), api::SoundID::keyboardClavichordSynth},
    {core::SoundID::keyboardConcertina(), api::SoundID::keyboardConcertina},
    {core::SoundID::keyboardFortepiano(), api::SoundID::keyboardFortepiano},
    {core::SoundID::keyboardHarmonium(), api::SoundID::keyboardHarmonium},
    {core::SoundID::keyboardHarpsichord(), api::SoundID::keyboardHarpsichord},
    {core::SoundID::keyboardOndesMartenot(), api::SoundID::keyboardOndesMartenot},
    {core::SoundID::keyboardOrgan(), api::SoundID::keyboardOrgan},
    {core::SoundID::keyboardOrganDrawbar(), api::SoundID::keyboardOrganDrawbar},
    {core::SoundID::keyboardOrganPercussive(), api::SoundID::keyboardOrganPercussive},
    {core::SoundID::keyboardOrganPipe(), api::SoundID::keyboardOrganPipe},
    {core::SoundID::keyboardOrganReed(), api::SoundID::keyboardOrganReed},
    {core::SoundID::keyboardOrganRotary(), api::SoundID::keyboardOrganRotary},
    {core::SoundID::keyboardPiano(), api::SoundID::keyboardPiano},
    {core::SoundID::keyboardPianoElectric(), api::SoundID::keyboardPianoElectric},
    {core::SoundID::keyboardPianoGrand(), api::SoundID::keyboardPianoGrand},
    {core::SoundID::keyboardPianoHonkyTonk(), api::SoundID::keyboardPianoHonkyTonk},
    {core::SoundID::keyboardPianoPrepared(), api::SoundID::keyboardPianoPrepared},
    {core::SoundID::keyboardPianoToy(), api::SoundID::keyboardPianoToy},
    {core::SoundID::keyboardPianoUpright(), api::SoundID::keyboardPianoUpright},
    {core::SoundID::keyboardVirginal(), api::SoundID::keyboardVirginal},
    {core::SoundID::metalAdodo(), api::SoundID::metalAdodo},
    {core::SoundID::metalAnvil(), api::SoundID::metalAnvil},
    {core::SoundID::metalBabendil(), api::SoundID::metalBabendil},
    {core::SoundID::metalBellsAgogo(), api::SoundID::metalBellsAgogo},
    {core::SoundID::metalBellsAlmglocken(), api::SoundID::metalBellsAlmglocken},
    {core::SoundID::metalBellsBellPlate(), api::SoundID::metalBellsBellPlate},
    {core::SoundID::metalBellsBellTree(), api::SoundID::metalBellsBellTree},
    {core::SoundID::metalBellsCarillon(), api::SoundID::metalBellsCarillon},
    {core::SoundID::metalBellsChimes(), api::SoundID::metalBellsChimes},
    {core::SoundID::metalBellsChimta(), api::SoundID::metalBellsChimta},
    {core::SoundID::metalBellsChippli(), api::SoundID::metalBellsChippli},
    {core::SoundID::metalBellsChurch(), api::SoundID::metalBellsChurch},
    {core::SoundID::metalBellsCowbell(), api::SoundID::metalBellsCowbell},
    {core::SoundID::metalBellsDawuro(), api::SoundID::metalBellsDawuro},
    {core::SoundID::metalBellsGankokwe(), api::SoundID::metalBellsGankokwe},
    {core::SoundID::metalBellsGhungroo(), api::SoundID::metalBellsGhungroo},
    {core::SoundID::metalBellsHatheli(), api::SoundID::metalBellsHatheli},
    {core::SoundID::metalBellsJingleBell(), api::SoundID::metalBellsJingleBell},
    {core::SoundID::metalBellsKhartal(), api::SoundID::metalBellsKhartal},
    {core::SoundID::metalBellsMarkTree(), api::SoundID::metalBellsMarkTree},
    {core::SoundID::metalBellsSistrum(), api::SoundID::metalBellsSistrum},
    {core::SoundID::metalBellsSleighBells(), api::SoundID::metalBellsSleighBells},
    {core::SoundID::metalBellsTemple(), api::SoundID::metalBellsTemple},
    {core::SoundID::metalBellsTibetan(), api::SoundID::metalBellsTibetan},
    {core::SoundID::metalBellsTinklebell(), api::SoundID::metalBellsTinklebell},
    {core::SoundID::metalBellsTrychel(), api::SoundID::metalBellsTrychel},
    {core::SoundID::metalBellsWindChimes(), api::SoundID::metalBellsWindChimes},
    {core::SoundID::metalBellsZills(), api::SoundID::metalBellsZills},
    {core::SoundID::metalBerimbau(), api::SoundID::metalBerimbau},
    {core::SoundID::metalBrakeDrums(), api::SoundID::metalBrakeDrums},
    {core::SoundID::metalCrotales(), api::SoundID::metalCrotales},
    {core::SoundID::metalCymbalBo(), api::SoundID::metalCymbalBo},
    {core::SoundID::metalCymbalCengCeng(), api::SoundID::metalCymbalCengCeng},
    {core::SoundID::metalCymbalChabara(), api::SoundID::metalCymbalChabara},
    {core::SoundID::metalCymbalChinese(), api::SoundID::metalCymbalChinese},
    {core::SoundID::metalCymbalChing(), api::SoundID::metalCymbalChing},
    {core::SoundID::metalCymbalClash(), api::SoundID::metalCymbalClash},
    {core::SoundID::metalCymbalCrash(), api::SoundID::metalCymbalCrash},
    {core::SoundID::metalCymbalFinger(), api::SoundID::metalCymbalFinger},
    {core::SoundID::metalCymbalHand(), api::SoundID::metalCymbalHand},
    {core::SoundID::metalCymbalKesi(), api::SoundID::metalCymbalKesi},
    {core::SoundID::metalCymbalManjeera(), api::SoundID::metalCymbalManjeera},
    {core::SoundID::metalCymbalReverse(), api::SoundID::metalCymbalReverse},
    {core::SoundID::metalCymbalRide(), api::SoundID::metalCymbalRide},
    {core::SoundID::metalCymbalSizzle(), api::SoundID::metalCymbalSizzle},
    {core::SoundID::metalCymbalSplash(), api::SoundID::metalCymbalSplash},
    {core::SoundID::metalCymbalSuspended(), api::SoundID::metalCymbalSuspended},
    {core::SoundID::metalCymbalTebyoshi(), api::SoundID::metalCymbalTebyoshi},
    {core::SoundID::metalCymbalTibetan(), api::SoundID::metalCymbalTibetan},
    {core::SoundID::metalCymbalTingsha(), api::SoundID::metalCymbalTingsha},
    {core::SoundID::metalFlexatone(), api::SoundID::metalFlexatone},
    {core::SoundID::metalGong(), api::SoundID::metalGong},
    {core::SoundID::metalGongAgeng(), api::SoundID::metalGongAgeng},
    {core::SoundID::metalGongAgung(), api::SoundID::metalGongAgung},
    {core::SoundID::metalGongChanchiki(), api::SoundID::metalGongChanchiki},
    {core::SoundID::metalGongChinese(), api::SoundID::metalGongChinese},
    {core::SoundID::metalGongGandingan(), api::SoundID::metalGongGandingan},
    {core::SoundID::metalGongKempul(), api::SoundID::metalGongKempul},
    {core::SoundID::metalGongKempyang(), api::SoundID::metalGongKempyang},
    {core::SoundID::metalGongKetuk(), api::SoundID::metalGongKetuk},
    {core::SoundID::metalGongKkwenggwari(), api::SoundID::metalGongKkwenggwari},
    {core::SoundID::metalGongLuo(), api::SoundID::metalGongLuo},
    {core::SoundID::metalGongSinging(), api::SoundID::metalGongSinging},
    {core::SoundID::metalGongThai(), api::SoundID::metalGongThai},
    {core::SoundID::metalGuira(), api::SoundID::metalGuira},
    {core::SoundID::metalHang(), api::SoundID::metalHang},
    {core::SoundID::metalHiHat(), api::SoundID::metalHiHat},
    {core::SoundID::metalJawHarp(), api::SoundID::metalJawHarp},
    {core::SoundID::metalKengong(), api::SoundID::metalKengong},
    {core::SoundID::metalMurchang(), api::SoundID::metalMurchang},
    {core::SoundID::metalMusicalSaw(), api::SoundID::metalMusicalSaw},
    {core::SoundID::metalSingingBowl(), api::SoundID::metalSingingBowl},
    {core::SoundID::metalSpoons(), api::SoundID::metalSpoons},
    {core::SoundID::metalSteelDrums(), api::SoundID::metalSteelDrums},
    {core::SoundID::metalTamtam(), api::SoundID::metalTamtam},
    {core::SoundID::metalThundersheet(), api::SoundID::metalThundersheet},
    {core::SoundID::metalTriangle(), api::SoundID::metalTriangle},
    {core::SoundID::metalWashboard(), api::SoundID::metalWashboard},
    {core::SoundID::pitchedPercussionAngklung(), api::SoundID::pitchedPercussionAngklung},
    {core::SoundID::pitchedPercussionBalafon(), api::SoundID::pitchedPercussionBalafon},
    {core::SoundID::pitchedPercussionBellLyre(), api::SoundID::pitchedPercussionBellLyre},
    {core::SoundID::pitchedPercussionBells(), api::SoundID::pitchedPercussionBells},
    {core::SoundID::pitchedPercussionBianqing(), api::SoundID::pitchedPercussionBianqing},
    {core::SoundID::pitchedPercussionBianzhong(), api::SoundID::pitchedPercussionBianzhong},
    {core::SoundID::pitchedPercussionBonang(), api::SoundID::pitchedPercussionBonang},
    {core::SoundID::pitchedPercussionCimbalom(), api::SoundID::pitchedPercussionCimbalom},
    {core::SoundID::pitchedPercussionCrystalGlasses(), api::SoundID::pitchedPercussionCrystalGlasses},
    {core::SoundID::pitchedPercussionDanTamThapLuc(), api::SoundID::pitchedPercussionDanTamThapLuc},
    {core::SoundID::pitchedPercussionFangxiang(), api::SoundID::pitchedPercussionFangxiang},
    {core::SoundID::pitchedPercussionGandinganAKayo(), api::SoundID::pitchedPercussionGandinganAKayo},
    {core::SoundID::pitchedPercussionGangsa(), api::SoundID::pitchedPercussionGangsa},
    {core::SoundID::pitchedPercussionGender(), api::SoundID::pitchedPercussionGender},
    {core::SoundID::pitchedPercussionGiying(), api::SoundID::pitchedPercussionGiying},
    {core::SoundID::pitchedPercussionGlassHarmonica(), api::SoundID::pitchedPercussionGlassHarmonica},
    {core::SoundID::pitchedPercussionGlockenspiel(), api::SoundID::pitchedPercussionGlockenspiel},
    {core::SoundID::pitchedPercussionGlockenspielAlto(), api::SoundID::pitchedPercussionGlockenspielAlto},
    {core::SoundID::pitchedPercussionGlockenspielSoprano(), api::SoundID::pitchedPercussionGlockenspielSoprano},
    {core::SoundID::pitchedPercussionGyil(), api::SoundID::pitchedPercussionGyil},
    {core::SoundID::pitchedPercussionHammerDulcimer(), api::SoundID::pitchedPercussionHammerDulcimer},
    {core::SoundID::pitchedPercussionHandbells(), api::SoundID::pitchedPercussionHandbells},
    {core::SoundID::pitchedPercussionKalimba(), api::SoundID::pitchedPercussionKalimba},
    {core::SoundID::pitchedPercussionKantil(), api::SoundID::pitchedPercussionKantil},
    {core::SoundID::pitchedPercussionKhim(), api::SoundID::pitchedPercussionKhim},
    {core::SoundID::pitchedPercussionKulintang(), api::SoundID::pitchedPercussionKulintang},
    {core::SoundID::pitchedPercussionKulintangAKayo(), api::SoundID::pitchedPercussionKulintangAKayo},
    {core::SoundID::pitchedPercussionKulintangATiniok(), api::SoundID::pitchedPercussionKulintangATiniok},
    {core::SoundID::pitchedPercussionLikembe(), api::SoundID::pitchedPercussionLikembe},
    {core::SoundID::pitchedPercussionLuntang(), api::SoundID::pitchedPercussionLuntang},
    {core::SoundID::pitchedPercussionMarimba(), api::SoundID::pitchedPercussionMarimba},
    {core::SoundID::pitchedPercussionMarimbaBass(), api::SoundID::pitchedPercussionMarimbaBass},
    {core::SoundID::pitchedPercussionMbira(), api::SoundID::pitchedPercussionMbira},
    {core::SoundID::pitchedPercussionMbiraArray(), api::SoundID::pitchedPercussionMbiraArray},
    {core::SoundID::pitchedPercussionMetallophone(), api::SoundID::pitchedPercussionMetallophone},
    {core::SoundID::pitchedPercussionMetallophoneAlto(), api::SoundID::pitchedPercussionMetallophoneAlto},
    {core::SoundID::pitchedPercussionMetallophoneBass(), api::SoundID::pitchedPercussionMetallophoneBass},
    {core::SoundID::pitchedPercussionMetallophoneSoprano(), api::SoundID::pitchedPercussionMetallophoneSoprano},
    {core::SoundID::pitchedPercussionMusicBox(), api::SoundID::pitchedPercussionMusicBox},
    {core::SoundID::pitchedPercussionPelogPanerus(), api::SoundID::pitchedPercussionPelogPanerus},
    {core::SoundID::pitchedPercussionPemade(), api::SoundID::pitchedPercussionPemade},
    {core::SoundID::pitchedPercussionPenyacah(), api::SoundID::pitchedPercussionPenyacah},
    {core::SoundID::pitchedPercussionRanatEk(), api::SoundID::pitchedPercussionRanatEk},
    {core::SoundID::pitchedPercussionRanatEkLek(), api::SoundID::pitchedPercussionRanatEkLek},
    {core::SoundID::pitchedPercussionRanatThum(), api::SoundID::pitchedPercussionRanatThum},
    {core::SoundID::pitchedPercussionRanatThumLek(), api::SoundID::pitchedPercussionRanatThumLek},
    {core::SoundID::pitchedPercussionReyong(), api::SoundID::pitchedPercussionReyong},
    {core::SoundID::pitchedPercussionSanza(), api::SoundID::pitchedPercussionSanza},
    {core::SoundID::pitchedPercussionSaronBarung(), api::SoundID::pitchedPercussionSaronBarung},
    {core::SoundID::pitchedPercussionSaronDemong(), api::SoundID::pitchedPercussionSaronDemong},
    {core::SoundID::pitchedPercussionSaronPanerus(), api::SoundID::pitchedPercussionSaronPanerus},
    {core::SoundID::pitchedPercussionSlendroPanerus(), api::SoundID::pitchedPercussionSlendroPanerus},
    {core::SoundID::pitchedPercussionSlentem(), api::SoundID::pitchedPercussionSlentem},
    {core::SoundID::pitchedPercussionTsymbaly(), api::SoundID::pitchedPercussionTsymbaly},
    {core::SoundID::pitchedPercussionTubes(), api::SoundID::pitchedPercussionTubes},
    {core::SoundID::pitchedPercussionTubularBells(), api::SoundID::pitchedPercussionTubularBells},
    {core::SoundID::pitchedPercussionVibraphone(), api::SoundID::pitchedPercussionVibraphone},
    {core::SoundID::pitchedPercussionXylophone(), api::SoundID::pitchedPercussionXylophone},
    {core::SoundID::pitchedPercussionXylophoneAlto(), api::SoundID::pitchedPercussionXylophoneAlto},
    {core::SoundID::pitchedPercussionXylophoneBass(), api::SoundID::pitchedPercussionXylophoneBass},
    {core::SoundID::pitchedPercussionXylophoneSoprano(), api::SoundID::pitchedPercussionXylophoneSoprano},
    {core::SoundID::pitchedPercussionXylorimba(), api::SoundID::pitchedPercussionXylorimba},
    {core::SoundID::pitchedPercussionYangqin(), api::SoundID::pitchedPercussionYangqin},
    {core::SoundID::pluckArchlute(), api::SoundID::pluckArchlute},
    {core::SoundID::pluckAutoharp(), api::SoundID::pluckAutoharp},
    {core::SoundID::pluckBaglama(), api::SoundID::pluckBaglama},
    {core::SoundID::pluckBajo(), api::SoundID::pluckBajo},
    {core::SoundID::pluckBalalaika(), api::SoundID::pluckBalalaika},
    {core::SoundID::pluckBalalaikaAlto(), api::SoundID::pluckBalalaikaAlto},
    {core::SoundID::pluckBalalaikaBass(), api::SoundID::pluckBalalaikaBass},
    {core::SoundID::pluckBalalaikaContrabass(), api::SoundID::pluckBalalaikaContrabass},
    {core::SoundID::pluckBalalaikaPiccolo(), api::SoundID::pluckBalalaikaPiccolo},
    {core::SoundID::pluckBalalaikaPrima(), api::SoundID::pluckBalalaikaPrima},
    {core::SoundID::pluckBalalaikaSecunda(), api::SoundID::pluckBalalaikaSecunda},
    {core::SoundID::pluckBandola(), api::SoundID::pluckBandola},
    {core::SoundID::pluckBandura(), api::SoundID::pluckBandura},
    {core::SoundID::pluckBandurria(), api::SoundID::pluckBandurria},
    {core::SoundID::pluckBanjo(), api::SoundID::pluckBanjo},
    {core::SoundID::pluckBanjoTenor(), api::SoundID::pluckBanjoTenor},
    {core::SoundID::pluckBanjolele(), api::SoundID::pluckBanjolele},
    {core::SoundID::pluckBarbat(), api::SoundID::pluckBarbat},
    {core::SoundID::pluckBass(), api::SoundID::pluckBass},
    {core::SoundID::pluckBassAcoustic(), api::SoundID::pluckBassAcoustic},
    {core::SoundID::pluckBassBolon(), api::SoundID::pluckBassBolon},
    {core::SoundID::pluckBassElectric(), api::SoundID::pluckBassElectric},
    {core::SoundID::pluckBassFretless(), api::SoundID::pluckBassFretless},
    {core::SoundID::pluckBassGuitarron(), api::SoundID::pluckBassGuitarron},
    {core::SoundID::pluckBassSynth(), api::SoundID::pluckBassSynth},
    {core::SoundID::pluckBassSynthLead(), api::SoundID::pluckBassSynthLead},
    {core::SoundID::pluckBassWashtub(), api::SoundID::pluckBassWashtub},
    {core::SoundID::pluckBassWhamola(), api::SoundID::pluckBassWhamola},
    {core::SoundID::pluckBegena(), api::SoundID::pluckBegena},
    {core::SoundID::pluckBiwa(), api::SoundID::pluckBiwa},
    {core::SoundID::pluckBordonua(), api::SoundID::pluckBordonua},
    {core::SoundID::pluckBouzouki(), api::SoundID::pluckBouzouki},
    {core::SoundID::pluckBouzoukiIrish(), api::SoundID::pluckBouzoukiIrish},
    {core::SoundID::pluckCelticHarp(), api::SoundID::pluckCelticHarp},
    {core::SoundID::pluckCharango(), api::SoundID::pluckCharango},
    {core::SoundID::pluckChitarraBattente(), api::SoundID::pluckChitarraBattente},
    {core::SoundID::pluckCithara(), api::SoundID::pluckCithara},
    {core::SoundID::pluckCittern(), api::SoundID::pluckCittern},
    {core::SoundID::pluckCuatro(), api::SoundID::pluckCuatro},
    {core::SoundID::pluckDanBau(), api::SoundID::pluckDanBau},
    {core::SoundID::pluckDanNguyet(), api::SoundID::pluckDanNguyet},
    {core::SoundID::pluckDanTranh(), api::SoundID::pluckDanTranh},
    {core::SoundID::pluckDanTyBa(), api::SoundID::pluckDanTyBa},
    {core::SoundID::pluckDiddleyBow(), api::SoundID::pluckDiddleyBow},
    {core::SoundID::pluckDomra(), api::SoundID::pluckDomra},
    {core::SoundID::pluckDomu(), api::SoundID::pluckDomu},
    {core::SoundID::pluckDulcimer(), api::SoundID::pluckDulcimer},
    {core::SoundID::pluckDutar(), api::SoundID::pluckDutar},
    {core::SoundID::pluckDuxianqin(), api::SoundID::pluckDuxianqin},
    {core::SoundID::pluckEktara(), api::SoundID::pluckEktara},
    {core::SoundID::pluckGeomungo(), api::SoundID::pluckGeomungo},
    {core::SoundID::pluckGottuvadhyam(), api::SoundID::pluckGottuvadhyam},
    {core::SoundID::pluckGuitar(), api::SoundID::pluckGuitar},
    {core::SoundID::pluckGuitarAcoustic(), api::SoundID::pluckGuitarAcoustic},
    {core::SoundID::pluckGuitarElectric(), api::SoundID::pluckGuitarElectric},
    {core::SoundID::pluckGuitarNylonString(), api::SoundID::pluckGuitarNylonString},
    {core::SoundID::pluckGuitarPedalSteel(), api::SoundID::pluckGuitarPedalSteel},
    {core::SoundID::pluckGuitarPortuguese(), api::SoundID::pluckGuitarPortuguese},
    {core::SoundID::pluckGuitarRequinto(), api::SoundID::pluckGuitarRequinto},
    {core::SoundID::pluckGuitarResonator(), api::SoundID::pluckGuitarResonator},
    {core::SoundID::pluckGuitarSteelString(), api::SoundID::pluckGuitarSteelString},
    {core::SoundID::pluckGuitjo(), api::SoundID::pluckGuitjo},
    {core::SoundID::pluckGuitjoDoubleNeck(), api::SoundID::pluckGuitjoDoubleNeck},
    {core::SoundID::pluckGuqin(), api::SoundID::pluckGuqin},
    {core::SoundID::pluckGuzheng(), api::SoundID::pluckGuzheng},
    {core::SoundID::pluckGuzhengChoazhou(), api::SoundID::pluckGuzhengChoazhou},
    {core::SoundID::pluckHarp(), api::SoundID::pluckHarp},
    {core::SoundID::pluckHarpGuitar(), api::SoundID::pluckHarpGuitar},
    {core::SoundID::pluckHuapanguera(), api::SoundID::pluckHuapanguera},
    {core::SoundID::pluckJaranaHuasteca(), api::SoundID::pluckJaranaHuasteca},
    {core::SoundID::pluckJaranaJarocha(), api::SoundID::pluckJaranaJarocha},
    {core::SoundID::pluckJaranaJarochaMosquito(), api::SoundID::pluckJaranaJarochaMosquito},
    {core::SoundID::pluckJaranaJarochaPrimera(), api::SoundID::pluckJaranaJarochaPrimera},
    {core::SoundID::pluckJaranaJarochaSegunda(), api::SoundID::pluckJaranaJarochaSegunda},
    {core::SoundID::pluckJaranaJarochaTercera(), api::SoundID::pluckJaranaJarochaTercera},
    {core::SoundID::pluckKabosy(), api::SoundID::pluckKabosy},
    {core::SoundID::pluckKantele(), api::SoundID::pluckKantele},
    {core::SoundID::pluckKanun(), api::SoundID::pluckKanun},
    {core::SoundID::pluckKayagum(), api::SoundID::pluckKayagum},
    {core::SoundID::pluckKobza(), api::SoundID::pluckKobza},
    {core::SoundID::pluckKomuz(), api::SoundID::pluckKomuz},
    {core::SoundID::pluckKora(), api::SoundID::pluckKora},
    {core::SoundID::pluckKoto(), api::SoundID::pluckKoto},
    {core::SoundID::pluckKutiyapi(), api::SoundID::pluckKutiyapi},
    {core::SoundID::pluckLangeleik(), api::SoundID::pluckLangeleik},
    {core::SoundID::pluckLaud(), api::SoundID::pluckLaud},
    {core::SoundID::pluckLute(), api::SoundID::pluckLute},
    {core::SoundID::pluckLyre(), api::SoundID::pluckLyre},
    {core::SoundID::pluckMandobass(), api::SoundID::pluckMandobass},
    {core::SoundID::pluckMandocello(), api::SoundID::pluckMandocello},
    {core::SoundID::pluckMandola(), api::SoundID::pluckMandola},
    {core::SoundID::pluckMandolin(), api::SoundID::pluckMandolin},
    {core::SoundID::pluckMandolinOctave(), api::SoundID::pluckMandolinOctave},
    {core::SoundID::pluckMandora(), api::SoundID::pluckMandora},
    {core::SoundID::pluckMandore(), api::SoundID::pluckMandore},
    {core::SoundID::pluckMarovany(), api::SoundID::pluckMarovany},
    {core::SoundID::pluckMusicalBow(), api::SoundID::pluckMusicalBow},
    {core::SoundID::pluckNgoni(), api::SoundID::pluckNgoni},
    {core::SoundID::pluckOud(), api::SoundID::pluckOud},
    {core::SoundID::pluckPipa(), api::SoundID::pluckPipa},
    {core::SoundID::pluckPsaltery(), api::SoundID::pluckPsaltery},
    {core::SoundID::pluckRuan(), api::SoundID::pluckRuan},
    {core::SoundID::pluckSallaneh(), api::SoundID::pluckSallaneh},
    {core::SoundID::pluckSanshin(), api::SoundID::pluckSanshin},
    {core::SoundID::pluckSantoor(), api::SoundID::pluckSantoor},
    {core::SoundID::pluckSanxian(), api::SoundID::pluckSanxian},
    {core::SoundID::pluckSarod(), api::SoundID::pluckSarod},
    {core::SoundID::pluckSaung(), api::SoundID::pluckSaung},
    {core::SoundID::pluckSaz(), api::SoundID::pluckSaz},
    {core::SoundID::pluckSe(), api::SoundID::pluckSe},
    {core::SoundID::pluckSetar(), api::SoundID::pluckSetar},
    {core::SoundID::pluckShamisen(), api::SoundID::pluckShamisen},
    {core::SoundID::pluckSitar(), api::SoundID::pluckSitar},
    {core::SoundID::pluckSynth(), api::SoundID::pluckSynth},
    {core::SoundID::pluckSynthCharang(), api::SoundID::pluckSynthCharang},
    {core::SoundID::pluckSynthChiff(), api::SoundID::pluckSynthChiff},
    {core::SoundID::pluckSynthStick(), api::SoundID::pluckSynthStick},
    {core::SoundID::pluckTambura(), api::SoundID::pluckTambura},
    {core::SoundID::pluckTamburaBulgarian(), api::SoundID::pluckTamburaBulgarian},
    {core::SoundID::pluckTamburaFemale(), api::SoundID::pluckTamburaFemale},
    {core::SoundID::pluckTamburaMale(), api::SoundID::pluckTamburaMale},
    {core::SoundID::pluckTar(), api::SoundID::pluckTar},
    {core::SoundID::pluckTheorbo(), api::SoundID::pluckTheorbo},
    {core::SoundID::pluckTimple(), api::SoundID::pluckTimple},
    {core::SoundID::pluckTiple(), api::SoundID::pluckTiple},
    {core::SoundID::pluckTres(), api::SoundID::pluckTres},
    {core::SoundID::pluckUkulele(), api::SoundID::pluckUkulele},
    {core::SoundID::pluckUkuleleTenor(), api::SoundID::pluckUkuleleTenor},
    {core::SoundID::pluckValiha(), api::SoundID::pluckValiha},
    {core::SoundID::pluckVeena(), api::SoundID::pluckVeena},
    {core::SoundID::pluckVeenaMohan(), api::SoundID::pluckVeenaMohan},
    {core::SoundID::pluckVeenaRudra(), api::SoundID::pluckVeenaRudra},
    {core::SoundID::pluckVeenaVichitra(), api::SoundID::pluckVeenaVichitra},
    {core::SoundID::pluckVihuela(), api::SoundID::pluckVihuela},
    {core::SoundID::pluckVihuelaMexican(), api::SoundID::pluckVihuelaMexican},
    {core::SoundID::pluckXalam(), api::SoundID::pluckXalam},
    {core::SoundID::pluckYueqin(), api::SoundID::pluckYueqin},
    {core::SoundID::pluckZither(), api::SoundID::pluckZither},
    {core::SoundID::pluckZitherOvertone(), api::SoundID::pluckZitherOvertone},
    {core::SoundID::rattleAfoxe(), api::SoundID::rattleAfoxe},
    {core::SoundID::rattleBirds(), api::SoundID::rattleBirds},
    {core::SoundID::rattleCabasa(), api::SoundID::rattleCabasa},
    {core::SoundID::rattleCaxixi(), api::SoundID::rattleCaxixi},
    {core::SoundID::rattleCog(), api::SoundID::rattleCog},
    {core::SoundID::rattleGanza(), api::SoundID::rattleGanza},
    {core::SoundID::rattleHosho(), api::SoundID::rattleHosho},
    {core::SoundID::rattleJawbone(), api::SoundID::rattleJawbone},
    {core::SoundID::rattleKayamba(), api::SoundID::rattleKayamba},
    {core::SoundID::rattleKpokoKpoko(), api::SoundID::rattleKpokoKpoko},
    {core::SoundID::rattleLavaStones(), api::SoundID::rattleLavaStones},
    {core::SoundID::rattleMaraca(), api::SoundID::rattleMaraca},
    {core::SoundID::rattleRainStick(), api::SoundID::rattleRainStick},
    {core::SoundID::rattleRatchet(), api::SoundID::rattleRatchet},
    {core::SoundID::rattleRattle(), api::SoundID::rattleRattle},
    {core::SoundID::rattleShaker(), api::SoundID::rattleShaker},
    {core::SoundID::rattleShakerEgg(), api::SoundID::rattleShakerEgg},
    {core::SoundID::rattleShekere(), api::SoundID::rattleShekere},
    {core::SoundID::rattleSistre(), api::SoundID::rattleSistre},
    {core::SoundID::rattleTelevi(), api::SoundID::rattleTelevi},
    {core::SoundID::rattleVibraslap(), api::SoundID::rattleVibraslap},
    {core::SoundID::rattleWasembe(), api::SoundID::rattleWasembe},
    {core::SoundID::stringsAjaeng(), api::SoundID::stringsAjaeng},
    {core::SoundID::stringsArpeggione(), api::SoundID::stringsArpeggione},
    {core::SoundID::stringsBaryton(), api::SoundID::stringsBaryton},
    {core::SoundID::stringsCello(), api::SoundID::stringsCello},
    {core::SoundID::stringsCelloPiccolo(), api::SoundID::stringsCelloPiccolo},
    {core::SoundID::stringsContrabass(), api::SoundID::stringsContrabass},
    {core::SoundID::stringsCrwth(), api::SoundID::stringsCrwth},
    {core::SoundID::stringsDanGao(), api::SoundID::stringsDanGao},
    {core::SoundID::stringsDihu(), api::SoundID::stringsDihu},
    {core::SoundID::stringsErhu(), api::SoundID::stringsErhu},
    {core::SoundID::stringsErxian(), api::SoundID::stringsErxian},
    {core::SoundID::stringsEsraj(), api::SoundID::stringsEsraj},
    {core::SoundID::stringsFiddle(), api::SoundID::stringsFiddle},
    {core::SoundID::stringsFiddleHardanger(), api::SoundID::stringsFiddleHardanger},
    {core::SoundID::stringsGadulka(), api::SoundID::stringsGadulka},
    {core::SoundID::stringsGaohu(), api::SoundID::stringsGaohu},
    {core::SoundID::stringsGehu(), api::SoundID::stringsGehu},
    {core::SoundID::stringsGroup(), api::SoundID::stringsGroup},
    {core::SoundID::stringsGroupSynth(), api::SoundID::stringsGroupSynth},
    {core::SoundID::stringsHaegeum(), api::SoundID::stringsHaegeum},
    {core::SoundID::stringsHurdyGurdy(), api::SoundID::stringsHurdyGurdy},
    {core::SoundID::stringsIgil(), api::SoundID::stringsIgil},
    {core::SoundID::stringsKamancha(), api::SoundID::stringsKamancha},
    {core::SoundID::stringsKokyu(), api::SoundID::stringsKokyu},
    {core::SoundID::stringsLaruan(), api::SoundID::stringsLaruan},
    {core::SoundID::stringsLeiqin(), api::SoundID::stringsLeiqin},
    {core::SoundID::stringsLirone(), api::SoundID::stringsLirone},
    {core::SoundID::stringsLyraByzantine(), api::SoundID::stringsLyraByzantine},
    {core::SoundID::stringsLyraCretan(), api::SoundID::stringsLyraCretan},
    {core::SoundID::stringsMorinKhuur(), api::SoundID::stringsMorinKhuur},
    {core::SoundID::stringsNyckelharpa(), api::SoundID::stringsNyckelharpa},
    {core::SoundID::stringsOctobass(), api::SoundID::stringsOctobass},
    {core::SoundID::stringsRebab(), api::SoundID::stringsRebab},
    {core::SoundID::stringsRebec(), api::SoundID::stringsRebec},
    {core::SoundID::stringsSarangi(), api::SoundID::stringsSarangi},
    {core::SoundID::stringsStrohViolin(), api::SoundID::stringsStrohViolin},
    {core::SoundID::stringsTrombaMarina(), api::SoundID::stringsTrombaMarina},
    {core::SoundID::stringsVielle(), api::SoundID::stringsVielle},
    {core::SoundID::stringsViol(), api::SoundID::stringsViol},
    {core::SoundID::stringsViolAlto(), api::SoundID::stringsViolAlto},
    {core::SoundID::stringsViolBass(), api::SoundID::stringsViolBass},
    {core::SoundID::stringsViolTenor(), api::SoundID::stringsViolTenor},
    {core::SoundID::stringsViolTreble(), api::SoundID::stringsViolTreble},
    {core::SoundID::stringsViolViolone(), api::SoundID::stringsViolViolone},
    {core::SoundID::stringsViola(), api::SoundID::stringsViola},
    {core::SoundID::stringsViolaDamore(), api::SoundID::stringsViolaDamore},
    {core::SoundID::stringsViolin(), api::SoundID::stringsViolin},
    {core::SoundID::stringsViolonoPiccolo(), api::SoundID::stringsViolonoPiccolo},
    {core::SoundID::stringsViolotta(), api::SoundID::stringsViolotta},
    {core::SoundID::stringsYayliTanbur(), api::SoundID::stringsYayliTanbur},
    {core::SoundID::stringsYazheng(), api::SoundID::stringsYazheng},
    {core::SoundID::stringsZhonghu(), api::SoundID::stringsZhonghu},
    {core::SoundID::synthEffects(), api::SoundID::synthEffects},
    {core::SoundID::synthEffectsAtmosphere(), api::SoundID::synthEffectsAtmosphere},
    {core::SoundID::synthEffectsBrightness(), api::SoundID::synthEffectsBrightness},
    {core::SoundID::synthEffectsCrystal(), api::SoundID::synthEffectsCrystal},
    {core::SoundID::synthEffectsEchoes(), api::SoundID::synthEffectsEchoes},
    {core::SoundID::synthEffectsGoblins(), api::SoundID::synthEffectsGoblins},
    {core::SoundID::synthEffectsRain(), api::SoundID::synthEffectsRain},
    {core::SoundID::synthEffectsSciFi(), api::SoundID::synthEffectsSciFi},
    {core::SoundID::synthEffectsSoundtrack(), api::SoundID::synthEffectsSoundtrack},
    {core::SoundID::synthGroup(), api::SoundID::synthGroup},
    {core::SoundID::synthGroupFifths(), api::SoundID::synthGroupFifths},
    {core::SoundID::synthGroupOrchestra(), api::SoundID::synthGroupOrchestra},
    {core::SoundID::synthPad(), api::SoundID::synthPad},
    {core::SoundID::synthPadBowed(), api::SoundID::synthPadBowed},
    {core::SoundID::synthPadChoir(), api::SoundID::synthPadChoir},
    {core::SoundID::synthPadHalo(), api::SoundID::synthPadHalo},
    {core::SoundID::synthPadMetallic(), api::SoundID::synthPadMetallic},
    {core::SoundID::synthPadPolysynth(), api::SoundID::synthPadPolysynth},
    {core::SoundID::synthPadSweep(), api::SoundID::synthPadSweep},
    {core::SoundID::synthPadWarm(), api::SoundID::synthPadWarm},
    {core::SoundID::synthTheremin(), api::SoundID::synthTheremin},
    {core::SoundID::synthToneSawtooth(), api::SoundID::synthToneSawtooth},
    {core::SoundID::synthToneSine(), api::SoundID::synthToneSine},
    {core::SoundID::synthToneSquare(), api::SoundID::synthToneSquare},
    {core::SoundID::voiceAa(), api::SoundID::voiceAa},
    {core::SoundID::voiceAlto(), api::SoundID::voiceAlto},
    {core::SoundID::voiceAw(), api::SoundID::voiceAw},
    {core::SoundID::voiceBaritone(), api::SoundID::voiceBaritone},
    {core::SoundID::voiceBass(), api::SoundID::voiceBass},
    {core::SoundID::voiceChild(), api::SoundID::voiceChild},
    {core::SoundID::voiceCountertenor(), api::SoundID::voiceCountertenor},
    {core::SoundID::voiceDoo(), api::SoundID::voiceDoo},
    {core::SoundID::voiceEe(), api::SoundID::voiceEe},
    {core::SoundID::voiceFemale(), api::SoundID::voiceFemale},
    {core::SoundID::voiceKazoo(), api::SoundID::voiceKazoo},
    {core::SoundID::voiceMale(), api::SoundID::voiceMale},
    {core::SoundID::voiceMezzoSoprano(), api::SoundID::voiceMezzoSoprano},
    {core::SoundID::voiceMm(), api::SoundID::voiceMm},
    {core::SoundID::voiceOo(), api::SoundID::voiceOo},
    {core::SoundID::voicePercussion(), api::SoundID::voicePercussion},
    {core::SoundID::voicePercussionBeatbox(), api::SoundID::voicePercussionBeatbox},
    {core::SoundID::voiceSoprano(), api::SoundID::voiceSoprano},
    {core::SoundID::voiceSynth(), api::SoundID::voiceSynth},
    {core::SoundID::voiceTalkBox(), api::SoundID::voiceTalkBox},
    {core::SoundID::voiceTenor(), api::SoundID::voiceTenor},
    {core::SoundID::voiceVocals(), api::SoundID::voiceVocals},
    {core::SoundID::windFlutesBansuri(), api::SoundID::windFlutesBansuri},
    {core::SoundID::windFlutesBlownBottle(), api::SoundID::windFlutesBlownBottle},
    {core::SoundID::windFlutesCalliope(), api::SoundID::windFlutesCalliope},
    {core::SoundID::windFlutesDanso(), api::SoundID::windFlutesDanso},
    {core::SoundID::windFlutesDiZi(), api::SoundID::windFlutesDiZi},
    {core::SoundID::windFlutesDvojnice(), api::SoundID::windFlutesDvojnice},
    {core::SoundID::windFlutesFife(), api::SoundID::windFlutesFife},
    {core::SoundID::windFlutesFlageolet(), api::SoundID::windFlutesFlageolet},
    {core::SoundID::windFlutesFlute(), api::SoundID::windFlutesFlute},
    {core::SoundID::windFlutesFluteAlto(), api::SoundID::windFlutesFluteAlto},
    {core::SoundID::windFlutesFluteBass(), api::SoundID::windFlutesFluteBass},
    {core::SoundID::windFlutesFluteContraAlto(), api::SoundID::windFlutesFluteContraAlto},
    {core::SoundID::windFlutesFluteContrabass(), api::SoundID::windFlutesFluteContrabass},
    {core::SoundID::windFlutesFluteDoubleContrabass(), api::SoundID::windFlutesFluteDoubleContrabass},
    {core::SoundID::windFlutesFluteIrish(), api::SoundID::windFlutesFluteIrish},
    {core::SoundID::windFlutesFlutePiccolo(), api::SoundID::windFlutesFlutePiccolo},
    {core::SoundID::windFlutesFluteSubcontrabass(), api::SoundID::windFlutesFluteSubcontrabass},
    {core::SoundID::windFlutesFujara(), api::SoundID::windFlutesFujara},
    {core::SoundID::windFlutesGemshorn(), api::SoundID::windFlutesGemshorn},
    {core::SoundID::windFlutesHocchiku(), api::SoundID::windFlutesHocchiku},
    {core::SoundID::windFlutesHun(), api::SoundID::windFlutesHun},
    {core::SoundID::windFlutesKaval(), api::SoundID::windFlutesKaval},
    {core::SoundID::windFlutesKawala(), api::SoundID::windFlutesKawala},
    {core::SoundID::windFlutesKhlui(), api::SoundID::windFlutesKhlui},
    {core::SoundID::windFlutesKnotweed(), api::SoundID::windFlutesKnotweed},
    {core::SoundID::windFlutesKoncovkaAlto(), api::SoundID::windFlutesKoncovkaAlto},
    {core::SoundID::windFlutesKoudi(), api::SoundID::windFlutesKoudi},
    {core::SoundID::windFlutesNey(), api::SoundID::windFlutesNey},
    {core::SoundID::windFlutesNohkan(), api::SoundID::windFlutesNohkan},
    {core::SoundID::windFlutesNose(), api::SoundID::windFlutesNose},
    {core::SoundID::windFlutesOcarina(), api::SoundID::windFlutesOcarina},
    {core::SoundID::windFlutesOvertoneTenor(), api::SoundID::windFlutesOvertoneTenor},
    {core::SoundID::windFlutesPalendag(), api::SoundID::windFlutesPalendag},
    {core::SoundID::windFlutesPanpipes(), api::SoundID::windFlutesPanpipes},
    {core::SoundID::windFlutesQuena(), api::SoundID::windFlutesQuena},
    {core::SoundID::windFlutesRecorder(), api::SoundID::windFlutesRecorder},
    {core::SoundID::windFlutesRecorderAlto(), api::SoundID::windFlutesRecorderAlto},
    {core::SoundID::windFlutesRecorderBass(), api::SoundID::windFlutesRecorderBass},
    {core::SoundID::windFlutesRecorderContrabass(), api::SoundID::windFlutesRecorderContrabass},
    {core::SoundID::windFlutesRecorderDescant(), api::SoundID::windFlutesRecorderDescant},
    {core::SoundID::windFlutesRecorderGarklein(), api::SoundID::windFlutesRecorderGarklein},
    {core::SoundID::windFlutesRecorderGreatBass(), api::SoundID::windFlutesRecorderGreatBass},
    {core::SoundID::windFlutesRecorderSopranino(), api::SoundID::windFlutesRecorderSopranino},
    {core::SoundID::windFlutesRecorderSoprano(), api::SoundID::windFlutesRecorderSoprano},
    {core::SoundID::windFlutesRecorderTenor(), api::SoundID::windFlutesRecorderTenor},
    {core::SoundID::windFlutesRyuteki(), api::SoundID::windFlutesRyuteki},
    {core::SoundID::windFlutesShakuhachi(), api::SoundID::windFlutesShakuhachi},
    {core::SoundID::windFlutesShepherdsPipe(), api::SoundID::windFlutesShepherdsPipe},
    {core::SoundID::windFlutesShinobue(), api::SoundID::windFlutesShinobue},
    {core::SoundID::windFlutesShvi(), api::SoundID::windFlutesShvi},
    {core::SoundID::windFlutesSuling(), api::SoundID::windFlutesSuling},
    {core::SoundID::windFlutesTarka(), api::SoundID::windFlutesTarka},
    {core::SoundID::windFlutesTumpong(), api::SoundID::windFlutesTumpong},
    {core::SoundID::windFlutesVenu(), api::SoundID::windFlutesVenu},
    {core::SoundID::windFlutesWhistle(), api::SoundID::windFlutesWhistle},
    {core::SoundID::windFlutesWhistleAlto(), api::SoundID::windFlutesWhistleAlto},
    {core::SoundID::windFlutesWhistleLowIrish(), api::SoundID::windFlutesWhistleLowIrish},
    {core::SoundID::windFlutesWhistleShiva(), api::SoundID::windFlutesWhistleShiva},
    {core::SoundID::windFlutesWhistleSlide(), api::SoundID::windFlutesWhistleSlide},
    {core::SoundID::windFlutesWhistleTin(), api::SoundID::windFlutesWhistleTin},
    {core::SoundID::windFlutesWhistleTinBflat(), api::SoundID::windFlutesWhistleTinBflat},
    {core::SoundID::windFlutesWhistleTinD(), api::SoundID::windFlutesWhistleTinD},
    {core::SoundID::windFlutesXiao(), api::SoundID::windFlutesXiao},
    {core::SoundID::windFlutesXun(), api::SoundID::windFlutesXun},
    {core::SoundID::windGroup(), api::SoundID::windGroup},
    {core::SoundID::windJug(), api::SoundID::windJug},
    {core::SoundID::windPipesBagpipes(), api::SoundID::windPipesBagpipes},
    {core::SoundID::windPipesGaida(), api::SoundID::windPipesGaida},
    {core::SoundID::windPipesHighland(), api::SoundID::windPipesHighland},
    {core::SoundID::windPipesUilleann(), api::SoundID::windPipesUilleann},
    {core::SoundID::windPungi(), api::SoundID::windPungi},
    {core::SoundID::windReedAlbogue(), api::SoundID::windReedAlbogue},
    {core::SoundID::windReedAlboka(), api::SoundID::windReedAlboka},
    {core::SoundID::windReedAlgaita(), api::SoundID::windReedAlgaita},
    {core::SoundID::windReedArghul(), api::SoundID::windReedArghul},
    {core::SoundID::windReedBassetHorn(), api::SoundID::windReedBassetHorn},
    {core::SoundID::windReedBassoon(), api::SoundID::windReedBassoon},
    {core::SoundID::windReedBawu(), api::SoundID::windReedBawu},
    {core::SoundID::windReedBifora(), api::SoundID::windReedBifora},
    {core::SoundID::windReedBombarde(), api::SoundID::windReedBombarde},
    {core::SoundID::windReedChalumeau(), api::SoundID::windReedChalumeau},
    {core::SoundID::windReedClarinet(), api::SoundID::windReedClarinet},
    {core::SoundID::windReedClarinetA(), api::SoundID::windReedClarinetA},
    {core::SoundID::windReedClarinetAlto(), api::SoundID::windReedClarinetAlto},
    {core::SoundID::windReedClarinetBass(), api::SoundID::windReedClarinetBass},
    {core::SoundID::windReedClarinetBasset(), api::SoundID::windReedClarinetBasset},
    {core::SoundID::windReedClarinetBflat(), api::SoundID::windReedClarinetBflat},
    {core::SoundID::windReedClarinetContraAlto(), api::SoundID::windReedClarinetContraAlto},
    {core::SoundID::windReedClarinetContrabass(), api::SoundID::windReedClarinetContrabass},
    {core::SoundID::windReedClarinetEflat(), api::SoundID::windReedClarinetEflat},
    {core::SoundID::windReedClarinetPiccolo(), api::SoundID::windReedClarinetPiccolo},
    {core::SoundID::windReedClarinetPiccoloAflat(), api::SoundID::windReedClarinetPiccoloAflat},
    {core::SoundID::windReedClarinetteDamour(), api::SoundID::windReedClarinetteDamour},
    {core::SoundID::windReedContrabass(), api::SoundID::windReedContrabass},
    {core::SoundID::windReedContrabassoon(), api::SoundID::windReedContrabassoon},
    {core::SoundID::windReedCornamuse(), api::SoundID::windReedCornamuse},
    {core::SoundID::windReedCromorne(), api::SoundID::windReedCromorne},
    {core::SoundID::windReedCrumhorn(), api::SoundID::windReedCrumhorn},
    {core::SoundID::windReedCrumhornAlto(), api::SoundID::windReedCrumhornAlto},
    {core::SoundID::windReedCrumhornBass(), api::SoundID::windReedCrumhornBass},
    {core::SoundID::windReedCrumhornGreatBass(), api::SoundID::windReedCrumhornGreatBass},
    {core::SoundID::windReedCrumhornSoprano(), api::SoundID::windReedCrumhornSoprano},
    {core::SoundID::windReedCrumhornTenor(), api::SoundID::windReedCrumhornTenor},
    {core::SoundID::windReedDiple(), api::SoundID::windReedDiple},
    {core::SoundID::windReedDiplica(), api::SoundID::windReedDiplica},
    {core::SoundID::windReedDuduk(), api::SoundID::windReedDuduk},
    {core::SoundID::windReedDulcian(), api::SoundID::windReedDulcian},
    {core::SoundID::windReedDulzaina(), api::SoundID::windReedDulzaina},
    {core::SoundID::windReedEnglishHorn(), api::SoundID::windReedEnglishHorn},
    {core::SoundID::windReedGuanzi(), api::SoundID::windReedGuanzi},
    {core::SoundID::windReedHarmonica(), api::SoundID::windReedHarmonica},
    {core::SoundID::windReedHarmonicaBass(), api::SoundID::windReedHarmonicaBass},
    {core::SoundID::windReedHeckelClarina(), api::SoundID::windReedHeckelClarina},
    {core::SoundID::windReedHeckelphone(), api::SoundID::windReedHeckelphone},
    {core::SoundID::windReedHeckelphonePiccolo(), api::SoundID::windReedHeckelphonePiccolo},
    {core::SoundID::windReedHeckelphoneClarinet(), api::SoundID::windReedHeckelphoneClarinet},
    {core::SoundID::windReedHichiriki(), api::SoundID::windReedHichiriki},
    {core::SoundID::windReedHirtenschalmei(), api::SoundID::windReedHirtenschalmei},
    {core::SoundID::windReedHne(), api::SoundID::windReedHne},
    {core::SoundID::windReedHornpipe(), api::SoundID::windReedHornpipe},
    {core::SoundID::windReedHouguan(), api::SoundID::windReedHouguan},
    {core::SoundID::windReedHulusi(), api::SoundID::windReedHulusi},
    {core::SoundID::windReedJogiBaja(), api::SoundID::windReedJogiBaja},
    {core::SoundID::windReedKenBau(), api::SoundID::windReedKenBau},
    {core::SoundID::windReedKhaenMouthOrgan(), api::SoundID::windReedKhaenMouthOrgan},
    {core::SoundID::windReedLauneddas(), api::SoundID::windReedLauneddas},
    {core::SoundID::windReedMaqrunah(), api::SoundID::windReedMaqrunah},
    {core::SoundID::windReedMelodica(), api::SoundID::windReedMelodica},
    {core::SoundID::windReedMijwiz(), api::SoundID::windReedMijwiz},
    {core::SoundID::windReedMizmar(), api::SoundID::windReedMizmar},
    {core::SoundID::windReedNadaswaram(), api::SoundID::windReedNadaswaram},
    {core::SoundID::windReedOboe(), api::SoundID::windReedOboe},
    {core::SoundID::windReedOboeBass(), api::SoundID::windReedOboeBass},
    {core::SoundID::windReedOboePiccolo(), api::SoundID::windReedOboePiccolo},
    {core::SoundID::windReedOboeDaCaccia(), api::SoundID::windReedOboeDaCaccia},
    {core::SoundID::windReedOboeDamore(), api::SoundID::windReedOboeDamore},
    {core::SoundID::windReedOctavin(), api::SoundID::windReedOctavin},
    {core::SoundID::windReedPi(), api::SoundID::windReedPi},
    {core::SoundID::windReedPibgorn(), api::SoundID::windReedPibgorn},
    {core::SoundID::windReedPiri(), api::SoundID::windReedPiri},
    {core::SoundID::windReedRackett(), api::SoundID::windReedRackett},
    {core::SoundID::windReedRauschpfeife(), api::SoundID::windReedRauschpfeife},
    {core::SoundID::windReedRhaita(), api::SoundID::windReedRhaita},
    {core::SoundID::windReedRothphone(), api::SoundID::windReedRothphone},
    {core::SoundID::windReedSarrusaphone(), api::SoundID::windReedSarrusaphone},
    {core::SoundID::windReedSaxonette(), api::SoundID::windReedSaxonette},
    {core::SoundID::windReedSaxophone(), api::SoundID::windReedSaxophone},
    {core::SoundID::windReedSaxophoneAlto(), api::SoundID::windReedSaxophoneAlto},
    {core::SoundID::windReedSaxophoneAulochrome(), api::SoundID::windReedSaxophoneAulochrome},
    {core::SoundID::windReedSaxophoneBaritone(), api::SoundID::windReedSaxophoneBaritone},
    {core::SoundID::windReedSaxophoneBass(), api::SoundID::windReedSaxophoneBass},
    {core::SoundID::windReedSaxophoneContrabass(), api::SoundID::windReedSaxophoneContrabass},
    {core::SoundID::windReedSaxophoneMelody(), api::SoundID::windReedSaxophoneMelody},
    {core::SoundID::windReedSaxophoneMezzoSoprano(), api::SoundID::windReedSaxophoneMezzoSoprano},
    {core::SoundID::windReedSaxophoneSopranino(), api::SoundID::windReedSaxophoneSopranino},
    {core::SoundID::windReedSaxophoneSopranissimo(), api::SoundID::windReedSaxophoneSopranissimo},
    {core::SoundID::windReedSaxophoneSoprano(), api::SoundID::windReedSaxophoneSoprano},
    {core::SoundID::windReedSaxophoneSubcontrabass(), api::SoundID::windReedSaxophoneSubcontrabass},
    {core::SoundID::windReedSaxophoneTenor(), api::SoundID::windReedSaxophoneTenor},
    {core::SoundID::windReedShawm(), api::SoundID::windReedShawm},
    {core::SoundID::windReedShenai(), api::SoundID::windReedShenai},
    {core::SoundID::windReedSheng(), api::SoundID::windReedSheng},
    {core::SoundID::windReedSipsi(), api::SoundID::windReedSipsi},
    {core::SoundID::windReedSopila(), api::SoundID::windReedSopila},
    {core::SoundID::windReedSorna(), api::SoundID::windReedSorna},
    {core::SoundID::windReedSralai(), api::SoundID::windReedSralai},
    {core::SoundID::windReedSuona(), api::SoundID::windReedSuona},
    {core::SoundID::windReedSurnai(), api::SoundID::windReedSurnai},
    {core::SoundID::windReedTaepyeongso(), api::SoundID::windReedTaepyeongso},
    {core::SoundID::windReedTarogato(), api::SoundID::windReedTarogato},
    {core::SoundID::windReedTarogatoAncient(), api::SoundID::windReedTarogatoAncient},
    {core::SoundID::windReedTrompetaChina(), api::SoundID::windReedTrompetaChina},
    {core::SoundID::windReedTubax(), api::SoundID::windReedTubax},
    {core::SoundID::windReedXaphoon(), api::SoundID::windReedXaphoon},
    {core::SoundID::windReedZhaleika(), api::SoundID::windReedZhaleika},
    {core::SoundID::windReedZurla(), api::SoundID::windReedZurla},
    {core::SoundID::windReedZurna(), api::SoundID::windReedZurna},
    {core::SoundID::woodAgogoBlock(), api::SoundID::woodAgogoBlock},
    {core::SoundID::woodAgungATamlang(), api::SoundID::woodAgungATamlang},
    {core::SoundID::woodAhoko(), api::SoundID::woodAhoko},
    {core::SoundID::woodBones(), api::SoundID::woodBones},
    {core::SoundID::woodCastanets(), api::SoundID::woodCastanets},
    {core::SoundID::woodClaves(), api::SoundID::woodClaves},
    {core::SoundID::woodDrumSticks(), api::SoundID::woodDrumSticks},
    {core::SoundID::woodGourd(), api::SoundID::woodGourd},
    {core::SoundID::woodGraniteBlock(), api::SoundID::woodGraniteBlock},
    {core::SoundID::woodGuban(), api::SoundID::woodGuban},
    {core::SoundID::woodGuiro(), api::SoundID::woodGuiro},
    {core::SoundID::woodHyoushigi(), api::SoundID::woodHyoushigi},
    {core::SoundID::woodIpu(), api::SoundID::woodIpu},
    {core::SoundID::woodJamBlock(), api::SoundID::woodJamBlock},
    {core::SoundID::woodKaekeeke(), api::SoundID::woodKaekeeke},
    {core::SoundID::woodKagul(), api::SoundID::woodKagul},
    {core::SoundID::woodKalaau(), api::SoundID::woodKalaau},
    {core::SoundID::woodKashiklar(), api::SoundID::woodKashiklar},
    {core::SoundID::woodKubing(), api::SoundID::woodKubing},
    {core::SoundID::woodPanClappers(), api::SoundID::woodPanClappers},
    {core::SoundID::woodSandBlock(), api::SoundID::woodSandBlock},
    {core::SoundID::woodSlapstick(), api::SoundID::woodSlapstick},
    {core::SoundID::woodStirDrum(), api::SoundID::woodStirDrum},
    {core::SoundID::woodTempleBlock(), api::SoundID::woodTempleBlock},
    {core::SoundID::woodTicTocBlock(), api::SoundID::woodTicTocBlock},
    {core::SoundID::woodTonetang(), api::SoundID::woodTonetang},
    {core::SoundID::woodWoodBlock(), api::SoundID::woodWoodBlock},
};

const Converter::EnumMap<core::KindValue, api::ChordKind> Converter::kindMap = {
    {core::KindValue::major(), api::ChordKind::major},
    {core::KindValue::minor(), api::ChordKind::minor},
    {core::KindValue::augmented(), api::ChordKind::augmented},
    {core::KindValue::diminished(), api::ChordKind::diminished},
    {core::KindValue::dominant(), api::ChordKind::dominant},
    {core::KindValue::majorSeventh(), api::ChordKind::majorSeventh},
    {core::KindValue::minorSeventh(), api::ChordKind::minorSeventh},
    {core::KindValue::diminishedSeventh(), api::ChordKind::diminishedSeventh},
    {core::KindValue::augmentedSeventh(), api::ChordKind::augmentedSeventh},
    {core::KindValue::halfDiminished(), api::ChordKind::halfDiminished},
    {core::KindValue::majorMinor(), api::ChordKind::majorMinor},
    {core::KindValue::majorSixth(), api::ChordKind::majorSixth},
    {core::KindValue::minorSixth(), api::ChordKind::minorSixth},
    {core::KindValue::dominantNinth(), api::ChordKind::dominantNinth},
    {core::KindValue::majorNinth(), api::ChordKind::majorNinth},
    {core::KindValue::minorNinth(), api::ChordKind::minorNinth},
    {core::KindValue::dominant11th(), api::ChordKind::dominant11Th},
    {core::KindValue::major11th(), api::ChordKind::major11Th},
    {core::KindValue::minor11th(), api::ChordKind::minor11Th},
    {core::KindValue::dominant13th(), api::ChordKind::dominant13Th},
    {core::KindValue::major13th(), api::ChordKind::major13Th},
    {core::KindValue::minor13th(), api::ChordKind::minor13Th},
    {core::KindValue::suspendedSecond(), api::ChordKind::suspendedSecond},
    {core::KindValue::suspendedFourth(), api::ChordKind::suspendedFourth},
    {core::KindValue::neapolitan(), api::ChordKind::neapolitan},
    {core::KindValue::italian(), api::ChordKind::italian},
    {core::KindValue::french(), api::ChordKind::french},
    {core::KindValue::german(), api::ChordKind::german},
    {core::KindValue::pedal(), api::ChordKind::pedal},
    {core::KindValue::power(), api::ChordKind::power},
    {core::KindValue::tristan(), api::ChordKind::tristan},
    {core::KindValue::other(), api::ChordKind::other},
    {core::KindValue::none(), api::ChordKind::none},
};

api::Step Converter::convert(core::Step inStep) const
{
    return findApiItem(stepMap, api::Step::c, inStep);
}

core::Step Converter::convert(api::Step inStep) const
{
    return findCoreItem(stepMap, core::Step::c(), inStep);
}

api::DurationName Converter::convert(core::NoteTypeValue inDur) const
{
    return findApiItem(durationMap, api::DurationName::unspecified, inDur);
}

core::NoteTypeValue Converter::convert(api::DurationName inDur) const
{
    return findCoreItem(durationMap, core::NoteTypeValue::maxima(), inDur);
}

api::Notehead Converter::convert(core::NoteheadValue inValue) const
{
    return findApiItem(noteheadMap, api::Notehead::none, inValue);
}

core::NoteheadValue Converter::convert(api::Notehead inValue) const
{
    return findCoreItem(noteheadMap, core::NoteheadValue::none(), inValue);
}

core::BeamValue Converter::convert(api::Beam value) const
{
    return findCoreItem(beamMap, core::BeamValue::begin(), value);
}

api::Beam Converter::convert(core::BeamValue value) const
{
    return findApiItem(beamMap, api::Beam::unspecified, value);
}

api::Accidental Converter::convert(core::AccidentalValue value) const
{
    return findApiItem(accidentalMap, api::Accidental::none, value);
}

core::AccidentalValue Converter::convert(api::Accidental value) const
{
    return findCoreItem(accidentalMap, core::AccidentalValue::natural(), value);
}

core::ClefSign Converter::convert(api::ClefSymbol value) const
{
    return findCoreItem(clefMap, core::ClefSign::g(), value);
}

api::ClefSymbol Converter::convert(core::ClefSign value) const
{
    return findApiItem(clefMap, api::ClefSymbol::none, value);
}

api::Placement Converter::convert(core::AboveBelow value) const
{
    return findApiItem(placementMap, api::Placement::unspecified, value);
}

core::AboveBelow Converter::convert(api::Placement value) const
{
    return findCoreItem(placementMap, core::AboveBelow::below(), value);
}

api::Bool Converter::convert(core::YesNo value) const
{
    return findApiItem(boolMap, api::Bool::unspecified, value);
}

core::YesNo Converter::convert(api::Bool value) const
{
    return findCoreItem(boolMap, core::YesNo::no(), value);
}

api::VerticalAlignment Converter::convert(core::Valign value) const
{
    return findApiItem(valignMap, api::VerticalAlignment::unspecified, value);
}

core::Valign Converter::convert(api::VerticalAlignment value) const
{
    return findCoreItem(valignMap, core::Valign::baseline(), value);
}

api::HorizontalAlignment Converter::convert(core::LeftCenterRight value) const
{
    return findApiItem(halignMap, api::HorizontalAlignment::unspecified, value);
}

core::LeftCenterRight Converter::convert(api::HorizontalAlignment value) const
{
    return findCoreItem(halignMap, core::LeftCenterRight::left(), value);
}

core::CSSFontSize Converter::convert(api::CssSize value) const
{
    return findCoreItem(cssMap, core::CSSFontSize::medium(), value);
}

api::CssSize Converter::convert(core::CSSFontSize value) const
{
    return findApiItem(cssMap, api::CssSize::unspecified, value);
}

core::FontStyle Converter::convert(api::FontStyle value) const
{
    return findCoreItem(fontStyleMap, core::FontStyle::normal(), value);
}

api::FontStyle Converter::convert(core::FontStyle value) const
{
    return findApiItem(fontStyleMap, api::FontStyle::unspecified, value);
}

core::FontWeight Converter::convert(api::FontWeight value) const
{
    return findCoreItem(fontWeightMap, core::FontWeight::normal(), value);
}

api::FontWeight Converter::convert(core::FontWeight value) const
{
    return findApiItem(fontWeightMap, api::FontWeight::unspecified, value);
}

core::ArticulationsChoice::Kind Converter::convertArticulation(api::MarkType value) const
{
    return findCoreItem(articulationsMap, core::ArticulationsChoice::Kind::otherArticulation, value);
}

api::MarkType Converter::convertArticulation(core::ArticulationsChoice::Kind value) const
{
    return findApiItem(articulationsMap, api::MarkType::unspecified, value);
}

bool Converter::isArticulation(api::MarkType value) const
{
    return value == api::MarkType::accent || value == api::MarkType::strongAccent || value == api::MarkType::staccato ||
           value == api::MarkType::tenuto || value == api::MarkType::detachedLegato ||
           value == api::MarkType::staccatissimo || value == api::MarkType::spiccato || value == api::MarkType::scoop ||
           value == api::MarkType::plop || value == api::MarkType::doit || value == api::MarkType::falloff ||
           value == api::MarkType::breathMark || value == api::MarkType::caesura || value == api::MarkType::stress ||
           value == api::MarkType::unstress || value == api::MarkType::otherArticulation;
}

core::DynamicsChoice::Kind Converter::convertDynamic(api::MarkType value) const
{
    return findCoreItem(dynamicsMap, core::DynamicsChoice::Kind::otherDynamics, value);
}

api::MarkType Converter::convertDynamic(core::DynamicsChoice::Kind value) const
{
    return findApiItem(dynamicsMap, api::MarkType::unspecified, value);
}

core::OrnamentsGroupChoice::Kind Converter::convertOrnament(api::MarkType value) const
{
    // All tremolo variants map to Kind::tremolo; the specific slash count is encoded
    // separately in the Tremolo element's value.
    if (api::isMarkTremolo(value))
    {
        return core::OrnamentsGroupChoice::Kind::tremolo;
    }
    return findCoreItem(ornamentsMap, core::OrnamentsGroupChoice::Kind::otherOrnament, value);
}

api::MarkType Converter::convertOrnament(core::OrnamentsGroupChoice::Kind value) const
{
    return findApiItem(ornamentsMap, api::MarkType::unspecified, value);
}

core::AccidentalValue Converter::convertAccidentalMark(api::MarkType value) const
{
    return findCoreItem(accidentalMarkMap, core::AccidentalValue::natural(), value);
}

api::MarkType Converter::convertAccidentalMark(core::AccidentalValue value) const
{
    return findApiItem(accidentalMarkMap, api::MarkType::unspecified, value);
}

core::TechnicalChoice::Kind Converter::convertTechnicalMark(api::MarkType value) const
{
    return findCoreItem(technicalMarkMap, core::TechnicalChoice::Kind::otherTechnical, value);
}

api::MarkType Converter::convertTechnicalMark(core::TechnicalChoice::Kind value) const
{
    return findApiItem(technicalMarkMap, api::MarkType::unspecified, value);
}

core::MeasureNumberingValue Converter::convertMeasureNumbering(api::MeasureNumbering value) const
{
    return findCoreItem(measureNumberingMap, core::MeasureNumberingValue::none(), value);
}

api::MeasureNumbering Converter::convertMeasureNumbering(core::MeasureNumberingValue value) const
{
    return findApiItem(measureNumberingMap, api::MeasureNumbering::unspecified, value);
}

core::StemValue Converter::convert(api::Stem value) const
{
    return findCoreItem(stemMap, core::StemValue::up(), value);
}

api::Stem Converter::convert(core::StemValue value) const
{
    return findApiItem(stemMap, api::Stem::unspecified, value);
}

core::LineType Converter::convert(api::LineType value) const
{
    return findCoreItem(lineType, core::LineType::solid(), value);
}

api::LineType Converter::convert(core::LineType value) const
{
    return findApiItem(lineType, api::LineType::unspecified, value);
}

core::WedgeType Converter::convert(api::WedgeType value) const
{
    return findCoreItem(wedgeMap, core::WedgeType::continue_(), value);
}

api::WedgeType Converter::convert(core::WedgeType value) const
{
    return findApiItem(wedgeMap, api::WedgeType::unspecified, value);
}

core::BarStyle Converter::convert(api::BarlineType value) const
{
    return findCoreItem(barlineMap, core::BarStyle::regular(), value);
}

core::StartStopDiscontinue Converter::convert(api::EndingType value) const
{
    return findCoreItem(endingMap, core::StartStopDiscontinue::start(), value);
}

api::BarlineType Converter::convert(core::BarStyle value) const
{
    return findApiItem(barlineMap, api::BarlineType::unsupported, value);
}

core::RightLeftMiddle Converter::convertBarlinePlacement(api::HorizontalAlignment value) const
{
    return findCoreItem(barlinePlacementMap, core::RightLeftMiddle::right(), value);
}

api::HorizontalAlignment Converter::convertBarlinePlacement(core::RightLeftMiddle value) const
{
    return findApiItem(barlinePlacementMap, api::HorizontalAlignment::unspecified, value);
}

core::LineEnd Converter::convert(api::LineHook value) const
{
    return findCoreItem(lineStopMap, core::LineEnd::none(), value);
}

api::LineHook Converter::convert(core::LineEnd value) const
{
    return findApiItem(lineStopMap, api::LineHook::unspecified, value);
}

core::GroupSymbolValue Converter::convert(api::BracketType value) const
{
    return findCoreItem(bracketMap, core::GroupSymbolValue::none(), value);
}

api::BracketType Converter::convert(core::GroupSymbolValue value) const
{
    return findApiItem(bracketMap, api::BracketType::unspecified, value);
}

core::FermataShape Converter::convertFermata(api::MarkType value) const
{
    return findCoreItem(fermataMap, core::FermataShape::empty(), value);
}

api::MarkType Converter::convertFermata(core::FermataShape value) const
{
    return findApiItem(fermataMap, api::MarkType::unspecified, value);
}

core::SoundID Converter::convert(api::SoundID value) const
{
    return findCoreItem(instrumentMap, core::SoundID::keyboardPiano(), value);
}

api::SoundID Converter::convert(core::SoundID value) const
{
    return findApiItem(instrumentMap, api::SoundID::unspecified, value);
}

core::KindValue Converter::convert(api::ChordKind value) const
{
    return findCoreItem(kindMap, core::KindValue::none(), value);
}

api::ChordKind Converter::convert(core::KindValue value) const
{
    return findApiItem(kindMap, api::ChordKind::unspecified, value);
}

double Converter::convertToAlter(int semitones, double cents)
{
    double alter = 0.0;
    if (semitones != 0 || cents != 0.0)
    {
        double microtones = 0.0;
        if (cents != 0.0)
        {
            microtones = cents / 100.0;
        }
        alter = semitones + microtones;
        // Round to 6 decimal places to avoid floating-point representation noise
        // (e.g. -99.9999 / 100.0 gives -0.9999990000000001 instead of -0.999999).
        alter = std::round(alter * 1000000.0) / 1000000.0;
    }
    return alter;
}

std::pair<int, double> Converter::convertToSemitonesAndCents(double xmlAlter)
{
    double myCents = 0.0;
    const auto intAlter = static_cast<int>(xmlAlter);
    const auto micro = xmlAlter - static_cast<double>(intAlter);
    const auto microDistance = std::abs(micro);
    if (microDistance >= 0.000000000001)
    {
        const auto theCents = micro * 100.0;
        const auto theNarrowCents = static_cast<decltype(mx::api::PitchData::cents)>(theCents);
        myCents = theNarrowCents;
    }
    return {intAlter, myCents};
}

mx::core::Transpose Converter::convertToTranspose(const mx::api::TransposeData &inTransposeData)
{
    // the transpose element uses an octave-change element for intervals larger than an octave.
    // mx::api does away with the octave-change concept. so a translation is necessary.
    mx::core::Transpose transpose;
    mx::core::TransposeGroup group;
    int octave = 0;

    if (inTransposeData.chromatic != 0)
    {
        octave = inTransposeData.chromatic / 12;
        const auto negative = inTransposeData.chromatic < 0;
        const auto absMod = std::abs(inTransposeData.chromatic) % 12;
        const auto chromatic = negative ? -1 * absMod : absMod;
        if (octave != 0)
        {
            group.setOctaveChange(octave);
        }
        group.setChromatic(mx::core::Semitones{mx::core::Decimal{static_cast<double>(chromatic)}});
    }

    if (inTransposeData.diatonic != 0)
    {
        auto harmonic = inTransposeData.diatonic;
        if (octave != 0)
        {
            harmonic += octave * -7;
        }
        if (harmonic != 0)
        {
            group.setDiatonic(harmonic);
        }
    }

    transpose.setTranspose(std::move(group));
    return transpose;
}

inline int figureOutDiatonic(int inChromatic)
{
    // guess the transposition based on chromatic only. try to choose less evil options based
    // on common transpositions and key signature accidental counts.
    switch (inChromatic)
    {
    case -11:
        return -6; // in Db
    case -10:
        return -6; // in D
    case -9:
        return -5; // in Eb
    case -8:
        return -5; // in E
    case -7:
        return -4; // in F
    case -6:
        return -3; // in Gb
    case -5:
        return -3; // in G
    case -4:
        return -2; // in Ab
    case -3:
        return -2; // in A
    case -2:
        return -1; // in Bb
    case -1:
        return -1; // in B
    case 0:
        return 0; // in C
    case 1:
        return 1; // in Db
    case 2:
        return 1; // in D
    case 3:
        return 2; // in Eb
    case 4:
        return 2; // in E
    case 5:
        return 3; // in F
    case 6:
        return 3; // in F#
    case 7:
        return 4; // in G
    case 8:
        return 5; // in Ab
    case 9:
        return 5; // in A
    case 10:
        return 6; // in Bb
    case 11:
        return 6; // in B
    default:
        return -1; // error
    }
}

mx::api::TransposeData Converter::convertToTransposeData(const mx::core::Transpose &inTranspose)
{
    // the transpose element has restrictions in its usage that are not enforced by XSD range restrictions.
    // we do our best if the transpose element is used incorrectly, or the diatonic element (which is
    // optional in musicxml) is not present. TransposeData does away with the octave-change concept and
    // requires use of the diatonic value, so we translate as necessary.
    const auto &group = inTranspose.transpose();
    const int specifiedChromatic = static_cast<int>(group.chromatic().value().value());
    const std::optional<int> specifiedOctave = group.octaveChange();
    const std::optional<int> specifiedDiatonic = group.diatonic();
    int diatonic = 0;
    if (specifiedDiatonic.has_value())
    {
        // if diatonic is specified we will use it as-is
        diatonic = specifiedDiatonic.value();
    }
    else if (specifiedChromatic >= -11 && specifiedChromatic <= 11)
    {
        // if chromatic is used correctly, and diatonic is not given, we guess its value
        diatonic = figureOutDiatonic(specifiedChromatic);
    }
    else
    {
        // the transpose element is badly misused, make shit up
        const int octaves = specifiedChromatic / 12;
        int semitones = std::abs(specifiedChromatic) % 12;
        semitones *= specifiedChromatic < 0 ? -1 : 1;
        diatonic = figureOutDiatonic(semitones);
        diatonic += octaves * 7;
    }
    const auto octaves = specifiedOctave.has_value() ? specifiedOctave.value() : 0;
    diatonic += octaves * 7;
    const auto chromatic = specifiedChromatic + (octaves * 12);
    return mx::api::TransposeData{chromatic, diatonic};
}
} // namespace impl
} // namespace mx
