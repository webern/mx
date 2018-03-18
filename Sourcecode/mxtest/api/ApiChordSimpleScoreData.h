// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/api/Smufl.h"
#include "mx/impl/Converter.h"
#include "mx/core/Enums.h"


namespace mxtest
{
    inline mx::api::ScoreData apiChordSimpleScoreData()
    {
        using namespace mx::api;
        ScoreData score;
        score.musicXmlVersion = mx::api::MusicXmlVersion::ThreePointZero;
        score.ticksPerQuarter = 101;
        score.parts.emplace_back();
        auto& part = score.parts.back();
        part.name = "P1";
        part.uniqueId = "P1";

        // Measure 1
        part.measures.emplace_back();
        auto measure = &part.measures.back();
        measure->staves.emplace_back();
        auto staff = &measure->staves.back();
        staff->directions.emplace_back();
        auto direction = &staff->directions.back();
        direction->tickTimePosition = 0;
        direction->isStaffValueSpecified = true;
        direction->chords.emplace_back();
        auto chord = &direction->chords.back();
        chord->root = Step::c;
        chord->chordKind = ChordKind::majorSeventh;
        chord->text = "maj7";

        // Measure 2
        part.measures.emplace_back();
        measure = &part.measures.back();
        measure->staves.emplace_back();
        staff = &measure->staves.back();
        staff->directions.emplace_back();
        direction = &staff->directions.back();
        direction->tickTimePosition = 0;
        direction->isStaffValueSpecified = true;
        direction->chords.emplace_back();
        chord = &direction->chords.back();
        chord->root = Step::d;
        chord->chordKind = ChordKind::minor;
        chord->text = "m";
        chord->bass = Step::c;
        chord->bassAlter = 1;

        // Measure 3
        part.measures.emplace_back();
        measure = &part.measures.back();
        measure->staves.emplace_back();
        staff = &measure->staves.back();
        staff->directions.emplace_back();
        direction = &staff->directions.back();
        direction->tickTimePosition = 0;
        direction->isStaffValueSpecified = true;
        direction->chords.emplace_back();
        chord = &direction->chords.back();
        chord->root = Step::e;
        chord->rootAlter = -1;
        chord->chordKind = ChordKind::dominant;
        chord->text = "7";

        // Measure 4
        part.measures.emplace_back();
        measure = &part.measures.back();
        measure->staves.emplace_back();
        staff = &measure->staves.back();
        staff->directions.emplace_back();
        direction = &staff->directions.back();
        direction->tickTimePosition = 109;
        direction->isStaffValueSpecified = true;
        direction->chords.emplace_back();
        chord = &direction->chords.back();
        chord->root = Step::b;
        chord->rootAlter = 0;
        chord->chordKind = ChordKind::diminishedSeventh;
        chord->text = "dim7";
        return score;
    }
}
