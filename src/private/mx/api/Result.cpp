// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/Result.h"

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

    std::string text{"mx: "};
    text += codeName;

    // the place in the document or the score, if known
    std::string where;
    const auto appendWhere = [&where](const char *name, long long value) {
        if (value < 0)
        {
            return;
        }
        if (!where.empty())
        {
            where += ' ';
        }
        where += name;
        where += '=';
        where += std::to_string(value);
    };

    where += error.location.xmlPath;
    appendWhere("part", error.location.partIndex);
    appendWhere("measure", error.location.measureIndex);
    appendWhere("staff", error.location.staffIndex);
    appendWhere("voice", error.location.voiceIndex);
    appendWhere("tick", error.location.tickTimePosition);
    appendWhere("offset", error.location.byteOffset);

    if (!where.empty())
    {
        text += " at ";
        text += where;
    }

    if (!error.message.empty())
    {
        text += ": ";
        text += error.message;
    }

    return text;
}
} // namespace api
} // namespace mx
