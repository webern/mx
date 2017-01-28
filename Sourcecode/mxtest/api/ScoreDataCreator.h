// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"
#include <memory>

namespace mxtest
{
    class ScoreDataCreator;
    using ScoreDataCreatorPtr = std::unique_ptr<const ScoreDataCreator>;
    
    class ScoreDataCreator
    {
    public:
        virtual ~ScoreDataCreator() {}
        virtual mx::api::ScoreData createScoreData() const = 0;
    };
}
