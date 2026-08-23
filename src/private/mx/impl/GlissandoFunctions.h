// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/GlissandoData.h"
#include "mx/api/NoteAttachmentData.h"
#include "mx/core/Decimal.h"
#include "mx/core/generated/Glissando.h"
#include "mx/core/generated/NumberLevel.h"
#include "mx/core/generated/Percent.h"
#include "mx/core/generated/Slide.h"
#include "mx/core/generated/StartStop.h"
#include "mx/core/generated/TrillBeats.h"
#include "mx/impl/Converter.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/SpannerFunctions.h"

#include <optional>
#include <type_traits>

namespace mx
{
namespace impl
{
// <glissando> and <slide> are distinct core types with an identical shared attribute set; deduce
// which one inElement is the same way curveTypeOf<> distinguishes slur from tie in
// CurveFunctions.h.
template <typename GLISSANDO_OR_SLIDE_TYPE> constexpr api::GlissandoType glissandoTypeOf()
{
    return std::is_same_v<std::decay_t<GLISSANDO_OR_SLIDE_TYPE>, core::Glissando> ? api::GlissandoType::glissando
                                                                                  : api::GlissandoType::slide;
}

template <typename GLISSANDO_OR_SLIDE_TYPE>
api::GlissandoStart parseGlissandoStart(const GLISSANDO_OR_SLIDE_TYPE &inElement)
{
    api::GlissandoStart start{glissandoTypeOf<GLISSANDO_OR_SLIDE_TYPE>()};

    if (inElement.number().has_value())
    {
        start.number = api::SpannerNumber(checkNumber(&inElement));
    }

    start.text = inElement.value();
    start.positionData = getPositionData(inElement);
    start.printData = getPrintData(inElement);
    start.lineData = getLineData(inElement);
    start.id = getId(inElement);

    // Playback (trill-sound) attributes exist on <slide> only.
    if constexpr (std::is_same_v<std::decay_t<GLISSANDO_OR_SLIDE_TYPE>, core::Slide>)
    {
        if (inElement.accelerate().has_value())
        {
            Converter converter;
            start.accelerate = converter.convert(*inElement.accelerate());
        }
        if (inElement.beats().has_value())
        {
            start.beats = static_cast<double>(inElement.beats()->value().value());
        }
        if (inElement.firstBeat().has_value())
        {
            start.firstBeat = static_cast<double>(inElement.firstBeat()->value().value());
        }
        if (inElement.lastBeat().has_value())
        {
            start.lastBeat = static_cast<double>(inElement.lastBeat()->value().value());
        }
    }

    return start;
}

template <typename GLISSANDO_OR_SLIDE_TYPE>
api::GlissandoStop parseGlissandoStop(const GLISSANDO_OR_SLIDE_TYPE &inElement)
{
    api::GlissandoStop stop{glissandoTypeOf<GLISSANDO_OR_SLIDE_TYPE>()};

    if (inElement.number().has_value())
    {
        stop.number = api::SpannerNumber(checkNumber(&inElement));
    }

    stop.positionData = getPositionData(inElement);
    stop.lineData = getLineData(inElement);
    stop.id = getId(inElement);
    return stop;
}

// Reads a <glissando> or <slide> element into whichever start/stop vector on outAttachments
// matches its type attribute.
template <typename GLISSANDO_OR_SLIDE_TYPE>
void parseGlissandoOrSlide(const GLISSANDO_OR_SLIDE_TYPE &inElement, api::NoteAttachmentData &outAttachments)
{
    if (core::StartStop::start() == inElement.type())
    {
        outAttachments.glissandoStarts.emplace_back(parseGlissandoStart(inElement));
    }
    else
    {
        outAttachments.glissandoStops.emplace_back(parseGlissandoStop(inElement));
    }
}

// The number written is the one the SpannerNumberResolver pass resolved for the object -- mirrors
// writeAttributesFromCurveStart/Stop in CurveFunctions.h.
template <typename GLISSANDO_OR_SLIDE_TYPE>
void writeAttributesFromGlissandoStart(const api::GlissandoStart &inStart, GLISSANDO_OR_SLIDE_TYPE &outElement,
                                       const std::optional<int> &inResolvedNumber)
{
    outElement.setType(core::StartStop::start());
    setId(inStart.id, outElement);
    setAttributesFromPositionData(inStart.positionData, outElement);
    setAttributesFromPrintData(inStart.printData, outElement);
    setAttributesFromLineData(inStart.lineData, outElement);
    outElement.setValue(inStart.text);

    if (inResolvedNumber.has_value())
    {
        outElement.setNumber(core::NumberLevel{*inResolvedNumber});
    }

    if constexpr (std::is_same_v<std::decay_t<GLISSANDO_OR_SLIDE_TYPE>, core::Slide>)
    {
        if (inStart.accelerate != api::Bool::unspecified)
        {
            Converter converter;
            outElement.setAccelerate(converter.convert(inStart.accelerate));
        }
        if (inStart.beats.has_value())
        {
            outElement.setBeats(core::TrillBeats{core::Decimal{*inStart.beats}});
        }
        if (inStart.firstBeat.has_value())
        {
            outElement.setFirstBeat(core::Percent{core::Decimal{*inStart.firstBeat}});
        }
        if (inStart.lastBeat.has_value())
        {
            outElement.setLastBeat(core::Percent{core::Decimal{*inStart.lastBeat}});
        }
    }
}

template <typename GLISSANDO_OR_SLIDE_TYPE>
void writeAttributesFromGlissandoStop(const api::GlissandoStop &inStop, GLISSANDO_OR_SLIDE_TYPE &outElement,
                                      const std::optional<int> &inResolvedNumber)
{
    outElement.setType(core::StartStop::stop());
    setId(inStop.id, outElement);
    setAttributesFromPositionData(inStop.positionData, outElement);
    setAttributesFromLineData(inStop.lineData, outElement);

    if (inResolvedNumber.has_value())
    {
        outElement.setNumber(core::NumberLevel{*inResolvedNumber});
    }
}
} // namespace impl
} // namespace mx
