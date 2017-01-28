// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/api/Smufl.h"
#include "mx/impl/Converter.h"
#include "mx/core/Enums.h"

namespace mxtest
{
    inline mx::api::ScoreData apiK016aMiscScoreData()
    {
        using namespace mx::api;
        ScoreData scoreData;
        scoreData.encoding.software.emplace_back( "komp" );
        scoreData.encoding.miscelaneousFields.emplace_back( "komp-version-major", "1" );
        scoreData.encoding.miscelaneousFields.emplace_back( "komp-version-minor", "2" );
        scoreData.encoding.miscelaneousFields.emplace_back( "komp-version-patch", "3" );
        scoreData.parts.emplace_back( PartData{} );
        auto& part = scoreData.parts.back();
        part.name = "hello world";
        part.uniqueId = "ID";
        part.measures.emplace_back( MeasureData{} );
        auto& measure = part.measures.back();
        measure.staves.emplace_back( StaffData{} );
        return scoreData;
    }
}
