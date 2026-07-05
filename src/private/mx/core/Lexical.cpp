// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Lexical.h"

#include <charconv>
#include <clocale>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace mx::core
{

// XML whitespace is the only decoration a lexically-strict parse tolerates.
std::string_view trimWhitespace(std::string_view text)
{
    constexpr std::string_view space = " \t\r\n";
    const auto first = text.find_first_not_of(space);
    if (first == std::string_view::npos)
    {
        return {};
    }
    const auto last = text.find_last_not_of(space);
    return text.substr(first, last - first + 1);
}

bool tryParseInt(std::string_view text, int &out)
{
    const std::string_view t = trimWhitespace(text);
    if (t.empty())
    {
        return false;
    }
    // std::from_chars accepts a leading minus but not a plus; xs:integer
    // allows an explicit plus sign.
    const std::string_view digits = t.front() == '+' ? t.substr(1) : t;
    int value = 0;
    const auto [ptr, ec] = std::from_chars(digits.data(), digits.data() + digits.size(), value);
    if (ec != std::errc{} || ptr != digits.data() + digits.size())
    {
        return false;
    }
    out = value;
    return true;
}

int parseInt(std::string_view text)
{
    int value = 0;
    return tryParseInt(text, value) ? value : 0;
}

bool tryParseDouble(std::string_view text, double &out)
{
    const std::string_view t = trimWhitespace(text);
    if (t.empty())
    {
        return false;
    }
    // std::from_chars(double) is unimplemented in AppleClang's libc++ (only the
    // integral overloads exist), so doubles go through strtod. strtod needs a
    // NUL-terminated string and honors the active locale's decimal point; XML
    // decimals always use '.', so under a locale whose separator differs (e.g.
    // ',') translate before parsing. strtod also accepts a leading '+', which
    // from_chars rejected and the old code stripped by hand -- no longer needed.
    const char decimalPoint = *std::localeconv()->decimal_point;
    std::string buffer(t);
    if (decimalPoint != '.')
    {
        for (char &c : buffer)
        {
            if (c == '.')
            {
                c = decimalPoint;
            }
        }
    }
    // Reject hex floats ("0x1p4") and strtod's "nan(...)" spellings: xs:decimal
    // and xs:float never use them, and from_chars(general) rejected them too.
    if (buffer.find_first_of("xX") != std::string::npos)
    {
        return false;
    }
    const char *const begin = buffer.c_str();
    char *end = nullptr;
    const double value = std::strtod(begin, &end);
    // Lexically strict: some text must parse, and all of it must be consumed.
    if (end == begin || end != begin + buffer.size())
    {
        return false;
    }
    if (std::isnan(value) || std::isinf(value))
    {
        return false;
    }
    out = value;
    return true;
}

double parseDouble(std::string_view text)
{
    double value = 0.0;
    return tryParseDouble(text, value) ? value : 0.0;
}

std::string formatInt(int value)
{
    char buf[16];
    const auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value);
    return std::string(buf, ptr);
}

namespace
{

void splitVersion(std::string_view v, int &major, int &minor)
{
    major = 0;
    minor = 0;
    const auto dot = v.find('.');
    if (dot == std::string_view::npos)
    {
        tryParseInt(v, major);
        return;
    }
    tryParseInt(v.substr(0, dot), major);
    tryParseInt(v.substr(dot + 1), minor);
}

} // namespace

bool musicXmlVersionExceeds(std::string_view declared, std::string_view supported)
{
    int dMajor = 0;
    int dMinor = 0;
    int sMajor = 0;
    int sMinor = 0;
    splitVersion(declared, dMajor, dMinor);
    splitVersion(supported, sMajor, sMinor);
    return dMajor > sMajor || (dMajor == sMajor && dMinor > sMinor);
}

std::string formatDouble(double value)
{
    // Bounded shortest fixed-notation string: the shortest spelling that
    // round-trips the exact double, capped at 8 fractional digits; never
    // exponent notation (xs:decimal forbids it). std::to_chars(double) is
    // unavailable on AppleClang's libc++ below macOS 13.3 / iOS 16.3, and mx
    // targets older, so reproduce the shortest-round-trip contract with
    // snprintf: grow the fractional precision until the printed text parses
    // back to exactly the input. snprintf and strtod share the active locale's
    // decimal point, so the round-trip check is locale-agnostic; only the
    // returned spelling is normalized back to '.'.
    //
    // The 8-digit cap (issue #248) stops client arithmetic noise (a double a
    // few ULPs from a clean value) from being amplified to 16-17 significant
    // digits: when no precision within 8 round-trips, the value is rounded to
    // 8 places and trailing zeros trimmed. The introduced error is at most
    // 5e-9, below MX_API_EQUALITY_EPSILON (1e-8), so api round-trip equality
    // is unaffected. Large magnitudes round-trip at precision 0 (their exact
    // decimal expansion is an integer-digit string), so %.8f of any double
    // stays well inside the buffer.
    const char decimalPoint = *std::localeconv()->decimal_point;
    char buf[400];
    int n = 0;
    bool exact = false;
    for (int precision = 0; precision <= 8 && !exact; ++precision)
    {
        n = std::snprintf(buf, sizeof(buf), "%.*f", precision, value);
        if (n <= 0 || static_cast<std::size_t>(n) >= sizeof(buf))
        {
            n = 0;
            continue;
        }
        if (std::strtod(buf, nullptr) == value)
        {
            exact = true;
        }
    }
    if (!exact)
    {
        n = std::snprintf(buf, sizeof(buf), "%.8f", value);
        if (n <= 0 || static_cast<std::size_t>(n) >= sizeof(buf))
        {
            return "0";
        }
    }
    std::string result(buf, static_cast<std::size_t>(n));
    if (decimalPoint != '.')
    {
        for (char &c : result)
        {
            if (c == decimalPoint)
            {
                c = '.';
            }
        }
    }
    // The capped spelling can carry trailing zeros ("0.12000000"); the exact
    // search cannot (a shorter precision would have round-tripped first), but
    // trimming is spelling-safe either way.
    if (result.find('.') != std::string::npos)
    {
        while (!result.empty() && result.back() == '0')
        {
            result.pop_back();
        }
        if (!result.empty() && result.back() == '.')
        {
            result.pop_back();
        }
    }
    // Rounding a tiny negative to 8 places (or printing -0.0) leaves "-0".
    if (result == "-0")
    {
        result = "0";
    }
    return result;
}

} // namespace mx::core
