// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/FootnoteData.h"
#include "mx/api/LevelData.h"

namespace mx
{
namespace api
{
// The MusicXML editorial group (<footnote> + <level>) carries editorial information for a parent
// element. It appears on several elements (e.g. <part-group>, <direction>); this reusable type
// captures it once. The is...Specified flags distinguish "absent" (write nothing back) from a
// present-but-empty child.
class EditorialData
{
  public:
    bool isFootnoteSpecified;
    FootnoteData footnote;
    bool isLevelSpecified;
    LevelData level;

    EditorialData() : isFootnoteSpecified{false}, footnote{}, isLevelSpecified{false}, level{}
    {
    }
};

MXAPI_EQUALS_BEGIN(EditorialData)
MXAPI_EQUALS_MEMBER(isFootnoteSpecified)
MXAPI_EQUALS_MEMBER(footnote)
MXAPI_EQUALS_MEMBER(isLevelSpecified)
MXAPI_EQUALS_MEMBER(level)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(EditorialData);
} // namespace api
} // namespace mx
