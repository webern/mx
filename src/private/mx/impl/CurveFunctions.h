// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/CurveData.h"
#include "mx/api/NoteData.h"
#include "mx/core/generated/Slur.h"
#include "mx/core/generated/Tied.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/SpannerFunctions.h"

#include <cmath>
#include <type_traits>

namespace mx
{
namespace impl
{

// In the new core the curve element's type attribute is element-specific
// (Slur: StartStopContinue, Tied: TiedType); deduce it from the element.
// The old code asked getElementName() == "slur"; the element's static type
// answers the same question.
template <typename SLUR_OR_TIE_ELEMENT_TYPE> constexpr api::CurveType curveTypeOf()
{
    return std::is_same_v<std::decay_t<SLUR_OR_TIE_ELEMENT_TYPE>, core::Slur> ? api::CurveType::slur
                                                                              : api::CurveType::tie;
}

template <typename ATTRIBUTES_TYPE> api::CurvePoints parseCurvePoints(const ATTRIBUTES_TYPE &inAttributes)
{
    api::CurvePoints p;
    p.positionData = impl::getPositionData(inAttributes);

    if (inAttributes.bezierX().has_value())
    {
        p.isBezierXSpecified = true;
        p.bezierX = inAttributes.bezierX()->value().value();
    }

    if (inAttributes.bezierY().has_value())
    {
        p.isBezierYSpecified = true;
        p.bezierY = inAttributes.bezierY()->value().value();
    }

    if (inAttributes.bezierOffset().has_value())
    {
        p.isBezierOffsetSpecified = true;
        p.bezierOffset = static_cast<int>(std::ceil(inAttributes.bezierOffset()->value().value() - 0.5));
    }

    return p;
}

template <typename ATTRIBUTES_TYPE>
void writeAttributesFromCurvePoints(const api::CurvePoints &curvePoints, ATTRIBUTES_TYPE &outAttributes)
{
    impl::setAttributesFromPositionData(curvePoints.positionData, outAttributes);

    if (curvePoints.isBezierXSpecified)
    {
        outAttributes.setBezierX(core::Tenths{core::Decimal{static_cast<double>(curvePoints.bezierX)}});
    }

    if (curvePoints.isBezierYSpecified)
    {
        outAttributes.setBezierY(core::Tenths{core::Decimal{static_cast<double>(curvePoints.bezierY)}});
    }

    if (curvePoints.isBezierOffsetSpecified)
    {
        outAttributes.setBezierOffset(core::Divisions{core::Decimal{static_cast<double>(curvePoints.bezierOffset)}});
    }
}

template <typename SLUR_OR_TIE_ELEMENT_TYPE>
api::CurveStart parseCurveStart(const SLUR_OR_TIE_ELEMENT_TYPE &inSlurOrTie)
{
    const auto &inAttributes = inSlurOrTie;
    const auto curveType = curveTypeOf<SLUR_OR_TIE_ELEMENT_TYPE>();
    api::CurveStart c{curveType};

    if (inAttributes.number().has_value())
    {
        c.numberLevel = impl::checkNumber(&inAttributes);
    }
    else
    {
        c.numberLevel = -1;
    }

    c.curvePoints = parseCurvePoints(inAttributes);
    c.lineData = impl::getLineData(inAttributes);
    c.isColorSpecified = checkHasColor(&inAttributes);

    if (c.isColorSpecified)
    {
        c.colorData = impl::getColor(inAttributes);
    }

    if (inAttributes.orientation().has_value())
    {
        c.curveOrientation = *inAttributes.orientation() == core::OverUnder::over() ? api::CurveOrientation::overhand
                                                                                    : api::CurveOrientation::underhand;
    }
    return c;
}

template <typename SLUR_OR_TIE_ELEMENT_TYPE>
api::CurveContinue parseCurveContinue(const SLUR_OR_TIE_ELEMENT_TYPE &inSlurOrTie)
{
    const auto &inAttributes = inSlurOrTie;
    const auto curveType = curveTypeOf<SLUR_OR_TIE_ELEMENT_TYPE>();
    api::CurveContinue c{curveType};

    if (inAttributes.number().has_value())
    {
        c.numberLevel = impl::checkNumber(&inAttributes);
    }
    else
    {
        c.numberLevel = -1;
    }

    if (inAttributes.bezierX2().has_value())
    {
        c.isBezierX2Specified = true;
        c.bezierX2 = inAttributes.bezierX2()->value().value();
    }

    if (inAttributes.bezierY2().has_value())
    {
        c.isBezierY2Specified = true;
        c.bezierY2 = inAttributes.bezierY2()->value().value();
    }

    if (inAttributes.bezierOffset2().has_value())
    {
        c.isBezierOffset2Specified = true;
        c.bezierOffset2 = inAttributes.bezierOffset2()->value().value();
    }

    c.curvePoints = parseCurvePoints(inAttributes);
    return c;
}

template <typename SLUR_OR_TIE_ELEMENT_TYPE> api::CurveStop parseCurveStop(const SLUR_OR_TIE_ELEMENT_TYPE &inSlurOrTie)
{
    const auto &inAttributes = inSlurOrTie;
    const auto curveType = curveTypeOf<SLUR_OR_TIE_ELEMENT_TYPE>();
    api::CurveStop c{curveType};

    if (inAttributes.number().has_value())
    {
        c.numberLevel = impl::checkNumber(&inAttributes);
    }
    else
    {
        c.numberLevel = -1;
    }

    c.curvePoints = parseCurvePoints(inAttributes);
    return c;
}

template <typename ATTRIBUTES_TYPE>
void writeAttributesFromCurveStart(const api::CurveStart inCurve, ATTRIBUTES_TYPE &outAttributes)
{
    using CurveTypeAttribute = std::decay_t<decltype(outAttributes.type())>;
    outAttributes.setType(CurveTypeAttribute::start());
    impl::setAttributesFromPositionData(inCurve.curvePoints.positionData, outAttributes);
    impl::setAttributesFromLineData(inCurve.lineData, outAttributes);

    if (inCurve.isColorSpecified)
    {
        setAttributesFromColorData(inCurve.colorData, outAttributes);
    }

    if (inCurve.numberLevel > 0)
    {
        outAttributes.setNumber(core::NumberLevel{inCurve.numberLevel});
    }

    if (inCurve.curvePoints.isBezierOffsetSpecified)
    {
        outAttributes.setBezierOffset(
            core::Divisions{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierOffset)}});
    }

    if (inCurve.curvePoints.isBezierXSpecified)
    {
        outAttributes.setBezierX(core::Tenths{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierX)}});
    }

    if (inCurve.curvePoints.isBezierYSpecified)
    {
        outAttributes.setBezierY(core::Tenths{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierY)}});
    }

    if (inCurve.curveOrientation != api::CurveOrientation::unspecified)
    {
        outAttributes.setOrientation(inCurve.curveOrientation == api::CurveOrientation::overhand
                                         ? core::OverUnder::over()
                                         : core::OverUnder::under());
    }
}

template <typename ATTRIBUTES_TYPE>
void writeAttributesFromCurveContinue(const api::CurveContinue inCurve, ATTRIBUTES_TYPE &outAttributes)
{
    using CurveTypeAttribute = std::decay_t<decltype(outAttributes.type())>;
    outAttributes.setType(CurveTypeAttribute::continue_());
    impl::setAttributesFromPositionData(inCurve.curvePoints.positionData, outAttributes);

    if (inCurve.numberLevel > 0)
    {
        outAttributes.setNumber(core::NumberLevel{inCurve.numberLevel});
    }

    if (inCurve.curvePoints.isBezierOffsetSpecified)
    {
        outAttributes.setBezierOffset(
            core::Divisions{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierOffset)}});
    }

    if (inCurve.curvePoints.isBezierXSpecified)
    {
        outAttributes.setBezierX(core::Tenths{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierX)}});
    }

    if (inCurve.curvePoints.isBezierYSpecified)
    {
        outAttributes.setBezierY(core::Tenths{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierY)}});
    }

    if (inCurve.isBezierX2Specified)
    {
        outAttributes.setBezierX2(core::Tenths{core::Decimal{static_cast<double>(inCurve.bezierX2)}});
    }

    if (inCurve.isBezierY2Specified)
    {
        outAttributes.setBezierY2(core::Tenths{core::Decimal{static_cast<double>(inCurve.bezierY2)}});
    }

    if (inCurve.isBezierOffset2Specified)
    {
        outAttributes.setBezierOffset2(core::Divisions{core::Decimal{static_cast<double>(inCurve.bezierOffset2)}});
    }
}

template <typename ATTRIBUTES_TYPE>
void writeAttributesFromCurveStop(const api::CurveStop inCurve, ATTRIBUTES_TYPE &outAttributes)
{
    using CurveTypeAttribute = std::decay_t<decltype(outAttributes.type())>;
    outAttributes.setType(CurveTypeAttribute::stop());
    impl::setAttributesFromPositionData(inCurve.curvePoints.positionData, outAttributes);

    if (inCurve.numberLevel > 0)
    {
        outAttributes.setNumber(core::NumberLevel{inCurve.numberLevel});
    }

    if (inCurve.curvePoints.isBezierOffsetSpecified)
    {
        outAttributes.setBezierOffset(
            core::Divisions{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierOffset)}});
    }

    if (inCurve.curvePoints.isBezierXSpecified)
    {
        outAttributes.setBezierX(core::Tenths{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierX)}});
    }

    if (inCurve.curvePoints.isBezierYSpecified)
    {
        outAttributes.setBezierY(core::Tenths{core::Decimal{static_cast<double>(inCurve.curvePoints.bezierY)}});
    }
}

// Emits a lone <tied type="let-ring"> from an api::TieLetRing, carrying its
// shared visual attributes (position, orientation, color).
inline void writeAttributesFromTieLetRing(const api::TieLetRing &inTie, core::Tied &outTied)
{
    outTied.setType(core::TiedType::letRing());
    impl::setAttributesFromPositionData(inTie.positionData, outTied);

    if (inTie.isColorSpecified)
    {
        setAttributesFromColorData(inTie.colorData, outTied);
    }

    if (inTie.curveOrientation != api::CurveOrientation::unspecified)
    {
        outTied.setOrientation(inTie.curveOrientation == api::CurveOrientation::overhand ? core::OverUnder::over()
                                                                                         : core::OverUnder::under());
    }
}

// Parses a lone <tied type="let-ring"> into an api::TieLetRing. A let-ring
// tie has no start/stop pairing, so it is captured on its own rather than in
// the curve vectors. The visual attributes it shares with any tied element
// (position, orientation, color) are carried across.
inline api::TieLetRing parseTieLetRing(const core::Tied &inTied)
{
    api::TieLetRing c;
    c.positionData = impl::getPositionData(inTied);
    c.isColorSpecified = checkHasColor(&inTied);

    if (c.isColorSpecified)
    {
        c.colorData = impl::getColor(inTied);
    }

    if (inTied.orientation().has_value())
    {
        c.curveOrientation = *inTied.orientation() == core::OverUnder::over() ? api::CurveOrientation::overhand
                                                                              : api::CurveOrientation::underhand;
    }
    return c;
}

// takes either an mx::core::Tied or an mx::core::Slur
// populates the outNoteData.curveStart, cureContinuations
// or curveStop vector with the result
template <typename SLUR_OR_TIE_ELEMENT_TYPE>
void parseCurve(const SLUR_OR_TIE_ELEMENT_TYPE &slurOrTie, api::NoteData &outNoteData)
{
    // Slur's type is StartStopContinue; Tied's is TiedType, which also has a
    // let-ring alternative. A let-ring value only occurs on <tied>, so the
    // if constexpr below keeps this branch out of the slur instantiation (whose
    // type has no letRing) and routes the lone <tied type="let-ring"> into
    // NoteData::tieLetRing instead of the start/continue/stop curve vectors.
    const auto outputType = slurOrTie.type();
    using CurveTypeAttribute = std::decay_t<decltype(outputType)>;

    if constexpr (std::is_same_v<std::decay_t<SLUR_OR_TIE_ELEMENT_TYPE>, core::Tied>)
    {
        if (core::TiedType::letRing() == outputType)
        {
            outNoteData.tieLetRing = parseTieLetRing(slurOrTie);
            return;
        }
    }

    if (CurveTypeAttribute::start() == outputType)
    {
        outNoteData.noteAttachmentData.curveStarts.emplace_back(parseCurveStart(slurOrTie));
    }
    else if (CurveTypeAttribute::continue_() == outputType)
    {
        outNoteData.noteAttachmentData.curveContinuations.emplace_back(parseCurveContinue(slurOrTie));
    }
    else if (CurveTypeAttribute::stop() == outputType)
    {
        outNoteData.noteAttachmentData.curveStops.emplace_back(parseCurveStop(slurOrTie));
    }
}
} // namespace impl
} // namespace mx
