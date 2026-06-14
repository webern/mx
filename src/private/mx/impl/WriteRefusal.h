// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Result.h"

#include <exception>
#include <utility>

namespace mx
{
namespace impl
{

// Internal to the impl layer (createFromScore): when a ScoreData describes
// something the new core won't represent (e.g. a
// ninth beam against the bounded addBeam), the writer refuses rather than
// silently dropping user data. Writers throw this; DocumentManager catches
// it at the api boundary and returns the carried ApiError. Never escapes
// mx::api.
class WriteRefusal : public std::exception
{
  public:
    explicit WriteRefusal(api::ApiError error) : myError{std::move(error)}
    {
    }

    const api::ApiError &error() const noexcept
    {
        return myError;
    }

    const char *what() const noexcept override
    {
        return myError.message.c_str();
    }

  private:
    api::ApiError myError;
};

} // namespace impl
} // namespace mx
