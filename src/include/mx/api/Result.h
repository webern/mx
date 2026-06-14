// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <cassert>
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
// business knowing. No exceptions escape the DocumentManager boundary.
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
    badDocumentId,      // handle not in the registry           (api-level)
    internalError,      // caught exception; nothing escapes    (api-level)
};

struct ApiError
{
    ResultCode code = ResultCode::internalError;
    std::string path; // e.g. /score-partwise/part[1]/measure[3]/note[2]
    std::string message;
};

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
