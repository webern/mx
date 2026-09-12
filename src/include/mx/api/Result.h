// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <cassert>
#include <exception>
#include <optional>
#include <string>
#include <utility>
#include <variant>

namespace mx
{
namespace api
{

// The mx::api error vocabulary. mx::api owns its own codes: the core-boundary
// failures are mirrored (public headers never
// include private mx::core headers), and the api adds the codes core has no
// business knowing. No exceptions escape the MusicXml boundary.
enum class ResultCode
{
    ioError,        // file open/read/write failure          (api-level)
    xmlSyntaxError, // the XML bytes could not be parsed     (api-level)
    unknownElement,
    unknownAttribute,
    missingRequiredElement,
    missingRequiredAttribute,
    wrongElementOrder,
    tooManyElements,
    invalidDocument,
    unsupportedVersion, // mirrored from the core parse boundary
    outOfMemory,        // the machine ran out of memory        (api-level)
    internalError,      // any other caught exception            (api-level)
};

// Where an error happened, as far as mx knows. An error may know the place
// in the XML (xmlPath, byteOffset), the place in the music (part, measure,
// staff, voice, and time), or neither. Unknown values are -1 for the
// numbers and empty for the path.
struct Location
{
    // The path through the XML elements to where the error is, e.g.
    // /score-partwise/part[1]/measure[3]/note[2]
    std::string xmlPath;

    // Zero-based positions in the score. -1 means mx does not know.
    int partIndex = -1;
    int measureIndex = -1;
    int staffIndex = -1;
    int voiceIndex = -1;

    // When in the music the error is, counted in ticks the way
    // ScoreData::ticksPerQuarter describes. -1 means mx does not know.
    int tickTimePosition = -1;

    // How many bytes into the raw XML the error is. This is set when the XML
    // itself could not be parsed. -1 means mx does not know.
    long long byteOffset = -1;
};

struct ApiError
{
    ResultCode code = ResultCode::internalError;

    // Where the error happened, where mx knows it.
    Location location;

    // A human-readable description of what went wrong.
    std::string message;

    // When mx catches an exception, the exception itself is kept here so a
    // caller can look at it or throw it again. Most errors are not
    // exceptions, and this is null for those.
    std::exception_ptr cause = nullptr;
};

// Writes an error on one line, the same way every time, so that logs and
// test output are consistent and easy to read.
// e.g. "mx: tooManyElements at part=0 measure=0: at most 8 beam occurrences"
std::string formatError(const ApiError &error);

// A small expected-like result, the same shape as mx::core's Result.
template <typename T> class Result
{
  public:
    Result(T value) : myData{std::in_place_index<0>, std::move(value)}
    {
    }

    Result(ApiError error) : myData{std::in_place_index<1>, std::move(error)}
    {
    }

    bool ok() const noexcept
    {
        return myData.index() == 0;
    }

    explicit operator bool() const noexcept
    {
        return ok();
    }

    // Precondition: ok().
    const T &value() const & noexcept
    {
        assert(ok());
        return *std::get_if<0>(&myData);
    }

    T &value() & noexcept
    {
        assert(ok());
        return *std::get_if<0>(&myData);
    }

    T &&value() && noexcept
    {
        assert(ok());
        return std::move(*std::get_if<0>(&myData));
    }

    // Precondition: !ok().
    const ApiError &error() const noexcept
    {
        assert(!ok());
        return *std::get_if<1>(&myData);
    }

  private:
    std::variant<T, ApiError> myData;
};

template <> class Result<void>
{
  public:
    Result() = default;

    Result(ApiError error) : myError{std::move(error)}
    {
    }

    bool ok() const noexcept
    {
        return !myError.has_value();
    }

    explicit operator bool() const noexcept
    {
        return ok();
    }

    // Precondition: !ok().
    const ApiError &error() const noexcept
    {
        assert(!ok());
        return *myError;
    }

  private:
    std::optional<ApiError> myError;
};

} // namespace api
} // namespace mx
