// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include "mx/core/Error.h"

#include <cassert>
#include <optional>
#include <utility>
#include <variant>

namespace mx::core
{

/// A small expected-like result (C++20 has no std::expected). Errors appear
/// in exactly two places — rejected parse and bounded-cardinality append
/// (plan §1.2) — and both speak this type.
template <typename T> class Result
{
  public:
    Result(T value) : m_data{std::in_place_index<0>, std::move(value)}
    {
    }

    Result(Error error) : m_data{std::in_place_index<1>, std::move(error)}
    {
    }

    bool ok() const noexcept
    {
        return m_data.index() == 0;
    }

    explicit operator bool() const noexcept
    {
        return ok();
    }

    /// Precondition: ok().
    const T &value() const & noexcept
    {
        assert(ok());
        return *std::get_if<0>(&m_data);
    }

    T &value() & noexcept
    {
        assert(ok());
        return *std::get_if<0>(&m_data);
    }

    T &&value() && noexcept
    {
        assert(ok());
        return std::move(*std::get_if<0>(&m_data));
    }

    /// Precondition: !ok().
    const Error &error() const noexcept
    {
        assert(!ok());
        return *std::get_if<1>(&m_data);
    }

  private:
    std::variant<T, Error> m_data;
};

template <> class Result<void>
{
  public:
    Result() = default;

    Result(Error error) : m_error{std::move(error)}
    {
    }

    bool ok() const noexcept
    {
        return !m_error.has_value();
    }

    explicit operator bool() const noexcept
    {
        return ok();
    }

    /// Precondition: !ok().
    const Error &error() const noexcept
    {
        assert(!ok());
        return *m_error;
    }

  private:
    std::optional<Error> m_error;
};

} // namespace mx::core
