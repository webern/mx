// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MusicXml.h"
#include "mxtest/control/CompileControl.h"
#include "mxtest/file/MxFileRepository.h"
#include "mxtest/file/Path.h"
#include <sstream>

namespace mxtest
{
constexpr const char *const roundTripFileName = "k007a_Notations_Dynamics.xml";

inline void roundTrip()
{
    const std::string path{MxFileRepository::getFullPath(roundTripFileName)};
    auto docResult = mx::api::MusicXml::fromFile(path);
    if (!docResult.ok())
        return;
    const auto scoreDataResult = mx::api::getScore(std::move(docResult).value());
    if (!scoreDataResult.ok())
        return;
    auto scoreData = scoreDataResult.value();
    auto doc2Result = mx::api::fromScore(scoreData);
    if (!doc2Result.ok())
        return;
    const std::string outputPath = getResourcesDirectoryPath() + "testOutput" + FILE_PATH_SEPARATOR + "output.xml";
    std::move(doc2Result).value().writeToFile(outputPath);
}

inline mx::api::ScoreData roundTrip(const mx::api::ScoreData inScoreData)
{
    auto docResult = mx::api::fromScore(inScoreData);
    if (!docResult.ok())
        return {};
    std::stringstream ss;
    const auto writeResult = std::move(docResult).value().writeToStream(ss);
    if (!writeResult.ok())
        return {};
    const auto xmlData = ss.str();
    std::istringstream iss{xmlData};
    auto doc2Result = mx::api::MusicXml::fromStream(iss);
    if (!doc2Result.ok())
        return {};
    const auto outScoreDataResult = mx::api::getScore(std::move(doc2Result).value());
    if (!outScoreDataResult.ok())
        return {};
    return outScoreDataResult.value();
}
} // namespace mxtest
