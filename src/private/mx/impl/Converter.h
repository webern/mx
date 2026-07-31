// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/BarlineData.h"
#include "mx/api/ClefData.h"
#include "mx/api/KeyData.h"
#include "mx/api/MarkData.h"
#include "mx/api/NoteData.h"
#include "mx/api/PercussionData.h"
#include "mx/api/PositionData.h"
#include "mx/api/ScoreData.h"
#include "mx/api/SoundID.h"
#include "mx/core/generated/AboveBelow.h"
#include "mx/core/generated/AccidentalValue.h"
#include "mx/core/generated/ArticulationsChoice.h"
#include "mx/core/generated/BackwardForward.h"
#include "mx/core/generated/BarStyle.h"
#include "mx/core/generated/BeamValue.h"
#include "mx/core/generated/BeaterValue.h"
#include "mx/core/generated/CSSFontSize.h"
#include "mx/core/generated/CancelLocation.h"
#include "mx/core/generated/ClefSign.h"
#include "mx/core/generated/DynamicsChoice.h"
#include "mx/core/generated/EffectValue.h"
#include "mx/core/generated/EnclosureShape.h"
#include "mx/core/generated/FermataShape.h"
#include "mx/core/generated/FontStyle.h"
#include "mx/core/generated/FontWeight.h"
#include "mx/core/generated/GlassValue.h"
#include "mx/core/generated/GroupBarlineValue.h"
#include "mx/core/generated/GroupSymbolValue.h"
#include "mx/core/generated/KindValue.h"
#include "mx/core/generated/LeftCenterRight.h"
#include "mx/core/generated/LineEnd.h"
#include "mx/core/generated/LineType.h"
#include "mx/core/generated/MeasureNumberingValue.h"
#include "mx/core/generated/MembraneValue.h"
#include "mx/core/generated/MetalValue.h"
#include "mx/core/generated/Mode.h"
#include "mx/core/generated/NoteTypeValue.h"
#include "mx/core/generated/NoteheadValue.h"
#include "mx/core/generated/OrnamentsGroupChoice.h"
#include "mx/core/generated/PitchedValue.h"
#include "mx/core/generated/RightLeftMiddle.h"
#include "mx/core/generated/SoundID.h"
#include "mx/core/generated/StartStopDiscontinue.h"
#include "mx/core/generated/StemValue.h"
#include "mx/core/generated/Step.h"
#include "mx/core/generated/StickLocation.h"
#include "mx/core/generated/StickMaterial.h"
#include "mx/core/generated/StickType.h"
#include "mx/core/generated/SystemRelation.h"
#include "mx/core/generated/SystemRelationNumber.h"
#include "mx/core/generated/TechnicalChoice.h"
#include "mx/core/generated/TimeRelation.h"
#include "mx/core/generated/TimeSeparator.h"
#include "mx/core/generated/TimeSymbol.h"
#include "mx/core/generated/TipDirection.h"
#include "mx/core/generated/Transpose.h"
#include "mx/core/generated/Valign.h"
#include "mx/core/generated/WedgeType.h"
#include "mx/core/generated/Winged.h"
#include "mx/core/generated/WoodValue.h"
#include "mx/core/generated/YesNo.h"

#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

namespace mx
{
namespace impl
{
class Converter
{
  public:
    // TODO - all of these functions should be static

    // The bridge tables. The old core's plain enums are now factory-only
    // wrapper classes without an ordering, so the tables are
    // declaration-ordered pair lists instead of std::maps; variant choice
    // alternatives are bridged by their nested Kind enums.
    template <typename CORE_TYPE, typename API_TYPE> using EnumMap = std::vector<std::pair<CORE_TYPE, API_TYPE>>;

    core::Step convert(api::Step value) const;
    api::Step convert(core::Step value) const;

    core::NoteTypeValue convert(api::DurationName value) const;
    api::DurationName convert(core::NoteTypeValue value) const;

    api::Notehead convert(core::NoteheadValue value) const;
    core::NoteheadValue convert(api::Notehead value) const;

    core::BeamValue convert(api::Beam value) const;
    api::Beam convert(core::BeamValue value) const;

    core::AccidentalValue convert(api::Accidental value) const;
    api::Accidental convert(core::AccidentalValue value) const;

    core::ClefSign convert(api::ClefSymbol value) const;
    api::ClefSymbol convert(core::ClefSign value) const;

    core::AboveBelow convert(api::Placement value) const;
    api::Placement convert(core::AboveBelow value) const;

    core::YesNo convert(api::Bool value) const;
    api::Bool convert(core::YesNo value) const;

    core::Valign convert(api::VerticalAlignment value) const;
    api::VerticalAlignment convert(core::Valign value) const;

    core::LeftCenterRight convert(api::HorizontalAlignment value) const;
    api::HorizontalAlignment convert(core::LeftCenterRight value) const;

    core::CSSFontSize convert(api::CssSize value) const;
    api::CssSize convert(core::CSSFontSize value) const;

    core::FontStyle convert(api::FontStyle value) const;
    api::FontStyle convert(core::FontStyle value) const;

    core::FontWeight convert(api::FontWeight value) const;
    api::FontWeight convert(core::FontWeight value) const;

    core::ArticulationsChoice::Kind convertArticulation(api::MarkType value) const;
    api::MarkType convertArticulation(core::ArticulationsChoice::Kind value) const;

    core::DynamicsChoice::Kind convertDynamic(api::MarkType value) const;
    api::MarkType convertDynamic(core::DynamicsChoice::Kind value) const;

    core::OrnamentsGroupChoice::Kind convertOrnament(api::MarkType value) const;
    api::MarkType convertOrnament(core::OrnamentsGroupChoice::Kind value) const;

    core::AccidentalValue convertAccidentalMark(api::MarkType value) const;
    api::MarkType convertAccidentalMark(core::AccidentalValue value) const;

    core::TechnicalChoice::Kind convertTechnicalMark(api::MarkType value) const;
    api::MarkType convertTechnicalMark(core::TechnicalChoice::Kind value) const;

    core::MeasureNumberingValue convertMeasureNumbering(api::MeasureNumbering value) const;
    api::MeasureNumbering convertMeasureNumbering(core::MeasureNumberingValue value) const;

    core::SystemRelationNumber convertSystemRelation(api::SystemRelation value) const;
    api::SystemRelation convertSystemRelation(core::SystemRelationNumber value) const;

    // <direction system="..."> uses a narrower vocabulary than <measure-numbering system="...">:
    // only-top, also-top and none. api::SystemRelation::onlyBottom and ::alsoBottom have no
    // direction equivalent and, like unspecified, yield nullopt so the caller writes no attribute.
    std::optional<core::SystemRelation> convertDirectionSystemRelation(api::SystemRelation value) const;
    api::SystemRelation convertDirectionSystemRelation(core::SystemRelation value) const;

    core::StemValue convert(api::Stem value) const;
    api::Stem convert(core::StemValue value) const;

    core::LineType convert(api::LineType value) const;
    api::LineType convert(core::LineType value) const;

    core::WedgeType convert(api::WedgeType value) const;
    api::WedgeType convert(core::WedgeType value) const;

    core::BarStyle convert(api::BarlineType value) const;
    api::BarlineType convert(core::BarStyle value) const;

    core::StartStopDiscontinue convert(api::EndingType value) const;

    core::BackwardForward convert(api::RepeatDirection value) const;
    api::RepeatDirection convert(core::BackwardForward value) const;

    core::Winged convert(api::RepeatWinged value) const;
    api::RepeatWinged convert(core::Winged value) const;

    core::RightLeftMiddle convertBarlinePlacement(api::HorizontalAlignment value) const;
    api::HorizontalAlignment convertBarlinePlacement(core::RightLeftMiddle value) const;

    core::LineEnd convert(api::LineHook value) const;
    api::LineHook convert(core::LineEnd value) const;

    core::GroupSymbolValue convert(api::BracketType value) const;
    api::BracketType convert(core::GroupSymbolValue value) const;

    core::GroupBarlineValue convert(api::GroupBarline value) const;
    api::GroupBarline convert(core::GroupBarlineValue value) const;

    core::FermataShape convertFermata(api::MarkType value) const;
    api::MarkType convertFermata(core::FermataShape value) const;

    core::SoundID convert(api::SoundID value) const;
    api::SoundID convert(core::SoundID value) const;

    core::KindValue convert(api::ChordKind value) const;
    api::ChordKind convert(core::KindValue value) const;

    core::CancelLocation convert(api::CancelLocation value) const;
    api::CancelLocation convert(core::CancelLocation value) const;

    // <mode> is an open vocabulary, so a core Mode holds an arbitrary string. api::KeyMode::unspecified
    // and api::KeyMode::unsupported have no wire spelling and convert to an empty core::Mode; callers
    // write no <mode> element for an empty Mode. A core Mode outside the standard vocabulary (including
    // the empty one) converts to api::KeyMode::unsupported.
    core::Mode convert(api::KeyMode value) const;
    api::KeyMode convert(const core::Mode &value) const;

    // Simple (narrow) symbol: common/cut only. unspecified maps to core normal(); callers skip the
    // attribute entirely for unspecified (absent and normal mean the same thing on the wire).
    core::TimeSymbol convert(api::TimeSignatureSymbol value) const;

    // Complex (full) symbol: the whole MusicXML time-symbol vocabulary. The reader always builds a
    // ComplexTimeSymbol (the TimeChoice constructor collapses to simple when possible), so core->api goes
    // through this one.
    core::TimeSymbol convert(api::ComplexTimeSymbol value) const;
    api::ComplexTimeSymbol convert(core::TimeSymbol value) const;

    // api::TimeSeparator::unspecified means "write no attribute"; callers skip it.
    core::TimeSeparator convert(api::TimeSeparator value) const;
    api::TimeSeparator convert(core::TimeSeparator value) const;

    // api::TimeRelation::unspecified means "write no element"; callers skip it.
    core::TimeRelation convert(api::TimeRelation value) const;
    api::TimeRelation convert(core::TimeRelation value) const;

    api::GlassInstrument convert(core::GlassValue value) const;
    core::GlassValue convert(api::GlassInstrument value) const;
    api::MetalInstrument convert(core::MetalValue value) const;
    core::MetalValue convert(api::MetalInstrument value) const;
    api::WoodInstrument convert(core::WoodValue value) const;
    core::WoodValue convert(api::WoodInstrument value) const;
    api::PitchedInstrument convert(core::PitchedValue value) const;
    core::PitchedValue convert(api::PitchedInstrument value) const;
    api::MembraneInstrument convert(core::MembraneValue value) const;
    core::MembraneValue convert(api::MembraneInstrument value) const;
    api::EffectInstrument convert(core::EffectValue value) const;
    core::EffectValue convert(api::EffectInstrument value) const;
    api::BeaterValue convert(core::BeaterValue value) const;
    core::BeaterValue convert(api::BeaterValue value) const;
    api::StickType convert(core::StickType value) const;
    core::StickType convert(api::StickType value) const;
    api::StickMaterial convert(core::StickMaterial value) const;
    core::StickMaterial convert(api::StickMaterial value) const;
    api::StickLocation convert(core::StickLocation value) const;
    core::StickLocation convert(api::StickLocation value) const;
    api::TipDirection convert(core::TipDirection value) const;
    core::TipDirection convert(api::TipDirection value) const;
    api::Enclosure convert(core::EnclosureShape value) const;
    core::EnclosureShape convert(api::Enclosure value) const;

    static double convertToAlter(int semitones, double cents);
    static std::pair<int, double> convertToSemitonesAndCents(double alter);

    static mx::core::Transpose convertToTranspose(const mx::api::TransposeData &inTransposeData);
    static mx::api::TransposeData convertToTransposeData(const mx::core::Transpose &inTranspose);

    const static EnumMap<core::Step, api::Step> stepMap;
    const static EnumMap<core::NoteTypeValue, api::DurationName> durationMap;
    const static EnumMap<core::NoteheadValue, api::Notehead> noteheadMap;
    const static EnumMap<core::BeamValue, api::Beam> beamMap;
    const static EnumMap<core::AccidentalValue, api::Accidental> accidentalMap;
    const static EnumMap<core::ClefSign, api::ClefSymbol> clefMap;
    const static EnumMap<core::AboveBelow, api::Placement> placementMap;
    const static EnumMap<core::YesNo, api::Bool> boolMap;
    const static EnumMap<core::Valign, api::VerticalAlignment> valignMap;
    const static EnumMap<core::LeftCenterRight, api::HorizontalAlignment> halignMap;
    const static EnumMap<core::CSSFontSize, api::CssSize> cssMap;
    const static EnumMap<core::FontStyle, api::FontStyle> fontStyleMap;
    const static EnumMap<core::FontWeight, api::FontWeight> fontWeightMap;
    const static EnumMap<core::ArticulationsChoice::Kind, api::MarkType> articulationsMap;
    const static EnumMap<core::DynamicsChoice::Kind, api::MarkType> dynamicsMap;
    const static EnumMap<core::OrnamentsGroupChoice::Kind, api::MarkType> ornamentsMap;
    const static EnumMap<core::AccidentalValue, api::MarkType> accidentalMarkMap;
    const static EnumMap<core::TechnicalChoice::Kind, api::MarkType> technicalMarkMap;
    const static EnumMap<core::MeasureNumberingValue, api::MeasureNumbering> measureNumberingMap;
    const static EnumMap<core::SystemRelationNumber, api::SystemRelation> systemRelationMap;
    const static EnumMap<core::SystemRelation, api::SystemRelation> directionSystemRelationMap;
    const static EnumMap<core::StemValue, api::Stem> stemMap;
    const static EnumMap<core::LineType, api::LineType> lineType;
    const static EnumMap<core::WedgeType, api::WedgeType> wedgeMap;
    const static EnumMap<core::BarStyle, api::BarlineType> barlineMap;
    const static EnumMap<core::RightLeftMiddle, api::HorizontalAlignment> barlinePlacementMap;
    const static EnumMap<core::StartStopDiscontinue, api::EndingType> endingMap;
    const static EnumMap<core::BackwardForward, api::RepeatDirection> repeatDirectionMap;
    const static EnumMap<core::Winged, api::RepeatWinged> wingedMap;
    const static EnumMap<core::LineEnd, api::LineHook> lineStopMap;
    const static EnumMap<core::GroupSymbolValue, api::BracketType> bracketMap;
    const static EnumMap<core::GroupBarlineValue, api::GroupBarline> groupBarlineMap;
    const static EnumMap<core::FermataShape, api::MarkType> fermataMap;
    const static EnumMap<core::SoundID, api::SoundID> instrumentMap;
    const static EnumMap<core::KindValue, api::ChordKind> kindMap;
    const static EnumMap<core::CancelLocation, api::CancelLocation> cancelLocationMap;
    const static EnumMap<core::Mode, api::KeyMode> keyModeMap;
    const static EnumMap<core::TimeSymbol, api::TimeSignatureSymbol> simpleTimeSymbolMap;
    const static EnumMap<core::TimeSymbol, api::ComplexTimeSymbol> complexTimeSymbolMap;
    const static EnumMap<core::TimeSeparator, api::TimeSeparator> timeSeparatorMap;
    const static EnumMap<core::TimeRelation, api::TimeRelation> timeRelationMap;
    const static EnumMap<core::GlassValue, api::GlassInstrument> glassMap;
    const static EnumMap<core::MetalValue, api::MetalInstrument> metalMap;
    const static EnumMap<core::WoodValue, api::WoodInstrument> woodMap;
    const static EnumMap<core::PitchedValue, api::PitchedInstrument> pitchedMap;
    const static EnumMap<core::MembraneValue, api::MembraneInstrument> membraneMap;
    const static EnumMap<core::EffectValue, api::EffectInstrument> effectMap;
    const static EnumMap<core::BeaterValue, api::BeaterValue> beaterMap;
    const static EnumMap<core::StickType, api::StickType> stickTypeMap;
    const static EnumMap<core::StickMaterial, api::StickMaterial> stickMaterialMap;
    const static EnumMap<core::StickLocation, api::StickLocation> stickLocationMap;
    const static EnumMap<core::TipDirection, api::TipDirection> tipDirectionMap;
    const static EnumMap<core::EnclosureShape, api::Enclosure> enclosureMap;

  private:
    template <typename CORE_TYPE, typename API_TYPE>
    API_TYPE findApiItem(const EnumMap<CORE_TYPE, API_TYPE> &enumMap, API_TYPE defaultToReturn,
                         CORE_TYPE itemToFind) const
    {
        auto compare = [&itemToFind](const std::pair<CORE_TYPE, API_TYPE> &v) { return v.first == itemToFind; };
        auto it = std::find_if(enumMap.cbegin(), enumMap.cend(), compare);
        if (it == enumMap.cend())
        {
            return defaultToReturn;
        }
        return it->second;
    }

    template <typename CORE_TYPE, typename API_TYPE>
    CORE_TYPE findCoreItem(const EnumMap<CORE_TYPE, API_TYPE> &enumMap, CORE_TYPE defaultToReturn,
                           API_TYPE itemToFind) const
    {
        auto compare = [&itemToFind](const std::pair<CORE_TYPE, API_TYPE> &v) { return v.second == itemToFind; };
        auto it = std::find_if(enumMap.cbegin(), enumMap.cend(), compare);
        if (it == enumMap.cend())
        {
            return defaultToReturn;
        }
        return it->first;
    }
};
} // namespace impl
} // namespace mx
