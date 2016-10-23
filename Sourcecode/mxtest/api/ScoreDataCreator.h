// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
