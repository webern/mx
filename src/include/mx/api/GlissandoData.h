// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/Id.h"
#include "mx/api/LineData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"
#include "mx/api/SpannerNumber.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// A glissando and a slide are notated the same way and carry the same attributes; they differ
// only in the pitch motion implied between the two notes they connect. A glissando sounds the
// discrete pitches in between (its line defaults to wavy); a slide is a continuous portamento
// (its line defaults to solid).
enum class GlissandoType
{
    unspecified,
    glissando,
    slide
};

struct GlissandoStart
{
    GlissandoType glissandoType;
    SpannerNumber number;

    // Text printed alongside the line, e.g. "gliss." -- the <glissando>/<slide> element value.
    std::string text;

    PositionData positionData;
    PrintData printData; // font and color
    LineData lineData;   // line-type, dash-length, space-length

    // Playback timing (MusicXML's trill-sound attributes). These exist on <slide> only -- a
    // glissando has no accelerate/beats/first-beat/last-beat -- and are left at their defaults
    // when glissandoType is glissando.
    Bool accelerate;
    std::optional<double> beats;
    std::optional<double> firstBeat;
    std::optional<double> lastBeat;

    // The id attribute of the element this spanner end is written as -- <glissando> or <slide>
    // (see Id.h).
    std::optional<Id> id;

    GlissandoStart(GlissandoType inGlissandoType)
        : glissandoType{inGlissandoType}, number{}, text{}, positionData{}, printData{}, lineData{},
          accelerate{Bool::unspecified}, beats{std::nullopt}, firstBeat{std::nullopt}, lastBeat{std::nullopt}, id{}
    {
    }
};

struct GlissandoStop
{
    GlissandoType glissandoType;
    SpannerNumber number;
    PositionData positionData;
    LineData lineData;

    // The id attribute of the element this spanner end is written as -- <glissando> or <slide>
    // (see Id.h).
    std::optional<Id> id;

    GlissandoStop(GlissandoType inGlissandoType)
        : glissandoType{inGlissandoType}, number{}, positionData{}, lineData{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(GlissandoStart)
MXAPI_EQUALS_MEMBER(glissandoType)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_MEMBER(accelerate)
MXAPI_EQUALS_MEMBER(beats)
MXAPI_EQUALS_MEMBER(firstBeat)
MXAPI_EQUALS_MEMBER(lastBeat)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(GlissandoStart);

MXAPI_EQUALS_BEGIN(GlissandoStop)
MXAPI_EQUALS_MEMBER(glissandoType)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(GlissandoStop);
} // namespace api
} // namespace mx
