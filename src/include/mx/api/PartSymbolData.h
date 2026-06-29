// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PartGroupData.h"

namespace mx
{
namespace api
{

class PartSymbolData
{
  public:
    BracketType value;

    // 1-based staff numbers indicating which staves the symbol spans.
    // -1 means unspecified (the symbol covers all staves in the part).
    int topStaff;
    int bottomStaff;

    PartSymbolData() : value{BracketType::unspecified}, topStaff{-1}, bottomStaff{-1}
    {
    }
};

MXAPI_EQUALS_BEGIN(PartSymbolData)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(topStaff)
MXAPI_EQUALS_MEMBER(bottomStaff)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PartSymbolData);
} // namespace api
} // namespace mx
