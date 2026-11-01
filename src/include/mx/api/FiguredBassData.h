// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/Id.h"

#include <optional>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
// Whether a figure's continuation line begins, carries on through, or ends at this figure.
// unspecified is a bare <extend/>: the line is drawn without saying whether it begins, continues,
// or ends here.
enum class FigureExtendType
{
    unspecified,
    start,
    continue_,
    stop
};

// The continuation line drawn after a figure, showing that the figure still holds over the bass
// notes that follow.
struct FigureExtend
{
    FigureExtendType type;

    FigureExtend() : type{FigureExtendType::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(FigureExtend)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(FigureExtend);

// A single <figure> within a <figured-bass>. Each part is optional; an empty string means the
// corresponding child element (<prefix>, <figure-number>, <suffix>) is absent.
class FigureData
{
  public:
    std::string prefix;
    std::string figureNumber;
    std::string suffix;
    // The figure's continuation line. Absent when the figure has none.
    std::optional<FigureExtend> extend;

    FigureData() : prefix{}, figureNumber{}, suffix{}, extend{}
    {
    }
};

MXAPI_EQUALS_BEGIN(FigureData)
MXAPI_EQUALS_MEMBER(prefix)
MXAPI_EQUALS_MEMBER(figureNumber)
MXAPI_EQUALS_MEMBER(suffix)
MXAPI_EQUALS_MEMBER(extend)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(FigureData);

// The MusicXML <figured-bass> element: an ordered list of figures plus optional parentheses and an
// optional duration. Figured bass takes its position from the first regular note that follows it in
// score order, so it carries no explicit tick position of its own here; it is attached to the
// DirectionData whose tickTimePosition locates it.
class FiguredBassData
{
  public:
    std::vector<FigureData> figures;
    // parentheses around the whole figured-bass group. unspecified means the attribute is absent
    // (MusicXML treats absent as "no").
    Bool parentheses;
    // The optional <duration>, in ticks. A value less than 0 means 'unspecified' (no duration child).
    int durationTimeTicks;

    // The <figured-bass> element's id attribute (see Id.h).
    std::optional<Id> id;

    FiguredBassData() : figures{}, parentheses{Bool::unspecified}, durationTimeTicks{VALUE_UNSPECIFIED}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(FiguredBassData)
MXAPI_EQUALS_MEMBER(figures)
MXAPI_EQUALS_MEMBER(parentheses)
MXAPI_EQUALS_MEMBER(durationTimeTicks)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(FiguredBassData);

} // namespace api
} // namespace mx
