// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

namespace mx
{
namespace api
{
enum class LyricSyllabic
{
    single,
    begin,
    end,
    middle
};

class LyricData
{
  public:
    LyricData()
        : text{}, verseNumber{}, verseName{}, syllabic{LyricSyllabic::single}, hasExtend{false}, positionData{},
          printData{}
    {
    }

    std::string text;
    std::string verseNumber;
    std::string verseName;
    LyricSyllabic syllabic;
    bool hasExtend;
    PositionData positionData;
    PrintData printData;
};

MXAPI_EQUALS_BEGIN(LyricData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(verseNumber)
MXAPI_EQUALS_MEMBER(verseName)
MXAPI_EQUALS_MEMBER(syllabic)
MXAPI_EQUALS_MEMBER(hasExtend)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(LyricData);
} // namespace api
} // namespace mx
