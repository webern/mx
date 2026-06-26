// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>
#include <vector>

namespace mx
{
namespace api
{
class SupportedItem
{
  public:
    std::string elementName;
    std::string attributeName;
    std::string specificValue;
    bool isSupported;

    SupportedItem() : elementName(), attributeName(), specificValue(), isSupported(true)
    {
    }
};

class MiscellaneousField
{
  public:
    std::string key;
    std::string value;

    MiscellaneousField(std::string inKey, std::string inValue) : key{inKey}, value{inValue}
    {
    }
};

class EncodingDate
{
  public:
    int year;
    int month;
    int day;

    EncodingDate() : year(-1), month(-1), day(-1)
    {
    }

    static EncodingDate today();
};

class EncodingData
{
  public:
    EncodingDate encodingDate;
    std::string encoder;
    std::string encodingDescription;
    std::vector<std::string> software;
    std::vector<SupportedItem> supportedItems;
    std::vector<MiscellaneousField> miscelaneousFields;

    // mx stamps its own provenance <software> node (the mx URL plus the build's git
    // SHA) into <encoding> on every api write. The intent is to aid in diagnosing and
    // fixing mx defects: when a file mx wrote turns out to be wrong, the stamp ties
    // the bad output back to the exact mx build that produced it. That stamp is
    // deliberately intrusive -- it injects a node the source never had into every
    // file mx writes -- so this flag exists to turn it off when byte-faithful output
    // is needed (e.g. reproducing a defect without mx's own marker in the diff).
    //
    // Defaults true, including for files parsed without the stamp. When false,
    // writeToFile/writeToStream omit mx's node; the user's own <software> entries are
    // always written regardless.
    bool writeMxVersion = true;
};

MXAPI_EQUALS_BEGIN(SupportedItem)
MXAPI_EQUALS_MEMBER(elementName)
MXAPI_EQUALS_MEMBER(attributeName)
MXAPI_EQUALS_MEMBER(specificValue)
MXAPI_EQUALS_MEMBER(isSupported)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SupportedItem);

MXAPI_EQUALS_BEGIN(MiscellaneousField)
MXAPI_EQUALS_MEMBER(key)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MiscellaneousField);

MXAPI_EQUALS_BEGIN(EncodingDate)
MXAPI_EQUALS_MEMBER(year)
MXAPI_EQUALS_MEMBER(month)
MXAPI_EQUALS_MEMBER(day)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(EncodingDate);

MXAPI_EQUALS_BEGIN(EncodingData)
MXAPI_EQUALS_MEMBER(encodingDate)
MXAPI_EQUALS_MEMBER(encoder)
MXAPI_EQUALS_MEMBER(encodingDescription)
MXAPI_EQUALS_MEMBER(software)
MXAPI_EQUALS_MEMBER(supportedItems)
MXAPI_EQUALS_MEMBER(miscelaneousFields)
MXAPI_EQUALS_MEMBER(writeMxVersion)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(EncodingData);
} // namespace api
} // namespace mx
