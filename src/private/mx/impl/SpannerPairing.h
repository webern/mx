// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/SpannerNumber.h"

#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace mx
{
namespace impl
{
// The bucket a spanner endpoint pairs within: identity endpoints pair by id, explicit endpoints
// by level, and unspecified endpoints with each other. The prefix keeps the three kinds from
// colliding (an identity of "3" is not the explicit level 3).
inline std::string spannerPairingKey(const api::SpannerNumber &inNumber)
{
    switch (inNumber.kind())
    {
    case api::SpannerNumber::Kind::explicitLevel:
        return "e" + std::to_string(inNumber.level());

    case api::SpannerNumber::Kind::identity:
        return "i" + inNumber.identity();

    default:
        return "u";
    }
}

// Pairs the starts and stops of one kind of spanner within a part, fed in the order they appear
// in the document. Endpoints pair within their bucket (see spannerPairingKey), and within a
// bucket a stop closes the most recently opened start that is still open. So overlapping
// spanners with distinct numbers never cross, a spanner nested inside another of the same number
// does not steal the outer one's start, and a pair can span measures and staves. Each start
// carries a value that the stop closing it receives.
template <typename VALUE_TYPE> class SpannerPairing
{
  public:
    void open(const api::SpannerNumber &inNumber, VALUE_TYPE inValue)
    {
        myOpenStarts[spannerPairingKey(inNumber)].push_back(std::move(inValue));
    }

    // The value of the start this stop closes, or nullopt when no start with its number is open.
    std::optional<VALUE_TYPE> close(const api::SpannerNumber &inNumber)
    {
        auto &stack = myOpenStarts[spannerPairingKey(inNumber)];
        if (stack.empty())
        {
            return std::nullopt;
        }
        auto value = std::move(stack.back());
        stack.pop_back();
        return value;
    }

    // The values of the starts that no stop has closed.
    std::vector<VALUE_TYPE> unclosed() const
    {
        std::vector<VALUE_TYPE> result;
        for (const auto &keyAndStack : myOpenStarts)
        {
            result.insert(result.end(), keyAndStack.second.cbegin(), keyAndStack.second.cend());
        }
        return result;
    }

  private:
    std::map<std::string, std::vector<VALUE_TYPE>> myOpenStarts;
};
} // namespace impl
} // namespace mx
