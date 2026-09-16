// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/LocationFormatting.h"

namespace mx
{
namespace api
{
void appendFormattedLocationAndMessage(std::string &text, const Location &location, const std::string &message)
{
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

    where += location.xmlPath;
    appendWhere("part", location.partIndex);
    appendWhere("measure", location.measureIndex);
    appendWhere("staff", location.staffIndex);
    appendWhere("voice", location.voiceIndex);
    appendWhere("tick", location.tickTimePosition);
    appendWhere("offset", location.byteOffset);

    if (!where.empty())
    {
        text += " at ";
        text += where;
    }

    if (!message.empty())
    {
        text += ": ";
        text += message;
    }
}
} // namespace api
} // namespace mx
