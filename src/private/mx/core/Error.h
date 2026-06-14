// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include <exception>
#include <string>

namespace mx::core
{

/// The error vocabulary of the two error boundaries (parse and bounded
/// append). No other API in mx::core produces an error (plan §1.2).
enum class ErrorCode
{
    unknownElement,
    unknownAttribute,
    missingRequiredElement,
    missingRequiredAttribute,
    wrongElementOrder,
    tooManyElements,
    invalidDocument,
    unsupportedVersion,
};

struct Error
{
    ErrorCode code;
    std::string path; // e.g. /score-partwise/part[1]/measure[3]/note[2]
    std::string message;
};

/// Internal: generated per-type parse functions throw this; the document
/// level `parse` catches and converts to Result so the public boundary stays
/// exception-free (plan §2.5). Not part of the public contract.
class ParseError : public std::exception
{
  public:
    explicit ParseError(Error error) : m_error{std::move(error)}
    {
    }

    const Error &error() const noexcept
    {
        return m_error;
    }

    Error &error() noexcept
    {
        return m_error;
    }

    const char *what() const noexcept override
    {
        return m_error.message.c_str();
    }

  private:
    Error m_error;
};

} // namespace mx::core
