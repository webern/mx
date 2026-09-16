// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/Result.h"

#include "mx/api/LocationFormatting.h"

namespace mx
{
namespace api
{
std::string formatError(const ApiError &error)
{
    const char *codeName = "internalError";
    switch (error.code)
    {
    case ResultCode::ioError:
        codeName = "ioError";
        break;
    case ResultCode::xmlSyntaxError:
        codeName = "xmlSyntaxError";
        break;
    case ResultCode::unknownElement:
        codeName = "unknownElement";
        break;
    case ResultCode::unknownAttribute:
        codeName = "unknownAttribute";
        break;
    case ResultCode::missingRequiredElement:
        codeName = "missingRequiredElement";
        break;
    case ResultCode::missingRequiredAttribute:
        codeName = "missingRequiredAttribute";
        break;
    case ResultCode::wrongElementOrder:
        codeName = "wrongElementOrder";
        break;
    case ResultCode::tooManyElements:
        codeName = "tooManyElements";
        break;
    case ResultCode::invalidDocument:
        codeName = "invalidDocument";
        break;
    case ResultCode::unsupportedVersion:
        codeName = "unsupportedVersion";
        break;
    case ResultCode::outOfMemory:
        codeName = "outOfMemory";
        break;
    case ResultCode::internalError:
        codeName = "internalError";
        break;
    }

    std::string result{"mx: "};
    result += codeName;
    result += formatLocationAndMessage(error.location, error.message);
    return result;
}
} // namespace api
} // namespace mx
