// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/XsString.h"

namespace mx
{
namespace core
{
XsString::XsString() : myValue("")
{
}

XsString::XsString(const StringType &value) : myValue(value)
{
}

XsString::~XsString()
{
}

StringType XsString::getValue() const
{
    return myValue;
}

void XsString::setValue(const StringType &value)
{
    myValue = value;
}

StringType toString(const XsString &xsstring)
{
    return xsstring.getValue();
}

std::ostream &toStream(std::ostream &os, const XsString &xsstring, bool useXmlEscapeCharacters)
{
    if (!useXmlEscapeCharacters)
    {
        return os << xsstring.getValue();
    }
    for (auto c : xsstring.getValue())
    {
        switch (c)
        {
        case '<':
            os << "&lt;";
            break;

        case '>':
            os << "&gt;";
            break;

        case '&':
            os << "&amp;";
            break;

        case '\r':
            // Per XML 1.0 §2.11 (End-of-line handling), a literal 0x0D byte in
            // element text content is normalized to 0x0A on the next parse. To
            // preserve a carriage return through an XML round-trip, emit it as
            // the numeric character reference &#xd; — character references are
            // not subject to EOL normalization.
            os << "&#xd;";
            break;

        default:
            os << c;
            break;
        }
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const XsString &xsstring)
{
    return toStream(os, xsstring);
}
} // namespace core
} // namespace mx
