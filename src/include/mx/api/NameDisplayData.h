// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <string>

namespace mx
{
namespace api
{
// A part name as it is shown at the left of a system, with its formatting. printData.printObject
// set to no hides the name.
struct NameDisplayData
{
    std::string text;
    PrintData printData;
    PositionData positionData;
};

MXAPI_EQUALS_BEGIN(NameDisplayData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(NameDisplayData);
} // namespace api
} // namespace mx
