// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/Token.h"

#include <optional>
#include <string>

namespace mx
{
namespace impl
{
// Read the id attribute from any core element that has one. Absent stays absent.
template <typename CORE_TYPE> std::optional<std::string> getId(const CORE_TYPE &inCoreElement)
{
    if (!inCoreElement.id().has_value())
    {
        return std::nullopt;
    }

    return inCoreElement.id()->value();
}

// Write the id attribute onto any core element that has one. An absent or empty id writes no
// attribute. core::Token repairs an id that is not a valid XML name.
template <typename CORE_TYPE> void setId(const std::optional<std::string> &inId, CORE_TYPE &outCoreElement)
{
    if (inId.has_value() && !inId->empty())
    {
        outCoreElement.setID(core::Token{*inId});
    }
}
} // namespace impl
} // namespace mx
