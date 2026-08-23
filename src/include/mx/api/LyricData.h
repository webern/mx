// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/Id.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// unspecified -> <syllabic> is omitted (a plain, non-hyphenated lyric word).
// single/begin/end/middle -> <syllabic> is written verbatim.
enum class LyricSyllabic
{
    unspecified,
    single,
    begin,
    end,
    middle
};

// unspecified writes the legacy bare <extend/> form. The other values identify the
// beginning, continuation, or end of a word extension across notes.
enum class LyricExtendType
{
    unspecified,
    start,
    continue_,
    stop
};

class LyricData
{
  public:
    LyricData()
        : text{}, verseNumber{}, verseName{}, syllabic{LyricSyllabic::unspecified}, hasExtend{false},
          extendType{LyricExtendType::unspecified}, positionData{}, printData{}, id{}
    {
    }

    std::string text;
    std::string verseNumber;
    std::string verseName;
    LyricSyllabic syllabic;
    bool hasExtend;
    LyricExtendType extendType;
    PositionData positionData;
    PrintData printData;

    // The <lyric> element's id attribute (see Id.h).
    std::optional<Id> id;
};

MXAPI_EQUALS_BEGIN(LyricData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(verseNumber)
MXAPI_EQUALS_MEMBER(verseName)
MXAPI_EQUALS_MEMBER(syllabic)
MXAPI_EQUALS_MEMBER(hasExtend)
MXAPI_EQUALS_MEMBER(extendType)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(LyricData);
} // namespace api
} // namespace mx
