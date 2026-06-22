// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>

namespace mx
{
namespace api
{
// Mirrors the MusicXML <level> 'type' attribute (start-stop-single): whether the editorial
// information applies to the start of a series of symbols, the end, or a single symbol.
enum class StartStopSingle
{
    unspecified,
    start,
    stop,
    single
};

// Mirrors the MusicXML <level> 'size' attribute (symbol-size).
enum class SymbolSize
{
    unspecified,
    full,
    cue,
    graceCue,
    large
};

// The MusicXML <level> element specifies editorial information for the parent element. Its text
// content is descriptive; the attributes control how the editorial marking is rendered. An
// `unspecified` enum (or empty `value`) means the source carried no such attribute and none is
// written back.
class LevelData
{
  public:
    std::string value;
    Bool reference;
    StartStopSingle type;
    Bool parentheses;
    Bool bracket;
    SymbolSize size;

    LevelData()
        : value{}, reference{Bool::unspecified}, type{StartStopSingle::unspecified}, parentheses{Bool::unspecified},
          bracket{Bool::unspecified}, size{SymbolSize::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(LevelData)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(reference)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(parentheses)
MXAPI_EQUALS_MEMBER(bracket)
MXAPI_EQUALS_MEMBER(size)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(LevelData);
} // namespace api
} // namespace mx
