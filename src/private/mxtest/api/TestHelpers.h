// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MusicXml.h"
#include "mx/utility/Throw.h"
#include <sstream>

namespace mxtest
{
inline std::string toXml(const mx::api::ScoreData &inScoreData)
{
    using namespace mx::api;
    auto docResult = fromScore(inScoreData);
    if (!docResult.ok())
        return {};
    std::stringstream ss;
    const auto writeResult = std::move(docResult).value().writeToStream(ss);
    if (!writeResult.ok())
        return {};
    return ss.str();
}

inline mx::api::ScoreData fromXml(const std::string &inXml)
{
    using namespace mx::api;
    std::istringstream iss{inXml};
    auto docResult = MusicXml::fromStream(iss);
    if (!docResult.ok())
        return {};
    const auto scoreResult = getScore(std::move(docResult).value());
    if (!scoreResult.ok())
        return {};
    return scoreResult.value();
}
} // namespace mxtest
