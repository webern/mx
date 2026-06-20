// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/BarlineData.h"
#include "mx/api/ClefData.h"
#include "mx/api/MarkData.h"
#include "mx/api/NoteData.h"
#include "mx/api/PositionData.h"
#include "mx/api/ScoreData.h"
#include "mx/api/SoundID.h"
#include "mx/core/generated/AboveBelow.h"
#include "mx/core/generated/AccidentalValue.h"
#include "mx/core/generated/ArticulationsChoice.h"
#include "mx/core/generated/BarStyle.h"
#include "mx/core/generated/BeamValue.h"
#include "mx/core/generated/CSSFontSize.h"
#include "mx/core/generated/ClefSign.h"
#include "mx/core/generated/DynamicsChoice.h"
#include "mx/core/generated/FermataShape.h"
#include "mx/core/generated/FontStyle.h"
#include "mx/core/generated/FontWeight.h"
#include "mx/core/generated/GroupBarlineValue.h"
#include "mx/core/generated/GroupSymbolValue.h"
#include "mx/core/generated/KindValue.h"
#include "mx/core/generated/LeftCenterRight.h"
#include "mx/core/generated/LineEnd.h"
#include "mx/core/generated/LineType.h"
#include "mx/core/generated/MeasureNumberingValue.h"
#include "mx/core/generated/NoteTypeValue.h"
#include "mx/core/generated/NoteheadValue.h"
#include "mx/core/generated/OrnamentsGroupChoice.h"
#include "mx/core/generated/RightLeftMiddle.h"
#include "mx/core/generated/SoundID.h"
#include "mx/core/generated/StartStopDiscontinue.h"
#include "mx/core/generated/StemValue.h"
#include "mx/core/generated/Step.h"
#include "mx/core/generated/TechnicalChoice.h"
#include "mx/core/generated/Transpose.h"
#include "mx/core/generated/Valign.h"
#include "mx/core/generated/WedgeType.h"
#include "mx/core/generated/YesNo.h"

#include <algorithm>
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
    bool isArticulation(api::MarkType value) const;

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

    core::StemValue convert(api::Stem value) const;
    api::Stem convert(core::StemValue value) const;

    core::LineType convert(api::LineType value) const;
    api::LineType convert(core::LineType value) const;

    core::WedgeType convert(api::WedgeType value) const;
    api::WedgeType convert(core::WedgeType value) const;

    core::BarStyle convert(api::BarlineType value) const;
    api::BarlineType convert(core::BarStyle value) const;

    core::StartStopDiscontinue convert(api::EndingType value) const;

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
    const static EnumMap<core::StemValue, api::Stem> stemMap;
    const static EnumMap<core::LineType, api::LineType> lineType;
    const static EnumMap<core::WedgeType, api::WedgeType> wedgeMap;
    const static EnumMap<core::BarStyle, api::BarlineType> barlineMap;
    const static EnumMap<core::RightLeftMiddle, api::HorizontalAlignment> barlinePlacementMap;
    const static EnumMap<core::StartStopDiscontinue, api::EndingType> endingMap;
    const static EnumMap<core::LineEnd, api::LineHook> lineStopMap;
    const static EnumMap<core::GroupSymbolValue, api::BracketType> bracketMap;
    const static EnumMap<core::GroupBarlineValue, api::GroupBarline> groupBarlineMap;
    const static EnumMap<core::FermataShape, api::MarkType> fermataMap;
    const static EnumMap<core::SoundID, api::SoundID> instrumentMap;
    const static EnumMap<core::KindValue, api::ChordKind> kindMap;

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
