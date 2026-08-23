// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/ScoreData.h"

#include <algorithm>

namespace mx
{
namespace api
{

ScoreData::ScoreData()
    : musicXmlVersion{api::MusicXmlVersion::unspecified}, declaredMusicXmlVersion{}, musicXmlType{"partwise"},
      workTitle{}, workNumber{}, movementTitle{}, movementNumber{}, composer{}, lyricist{}, copyright{},
      copyrightType{"copyright"}, source{}, encoding{}, pageTextItems{}, defaults{}, parts{}, partGroups{},
      ticksPerQuarter{DEFAULT_TICKS_PER_QUARTER}
{
}

int ScoreData::getNumMeasures() const
{
    int numMeasures = 0;

    for (const auto &part : parts)
    {
        int temp = static_cast<int>(part.measures.size());
        if (temp > numMeasures)
        {
            numMeasures = temp;
        }
    }

    return numMeasures;
}

int ScoreData::getNumStavesPerSystem() const
{
    int numStaves = 0;

    for (const auto &p : parts)
    {
        numStaves += p.getNumStaves();
    }

    return numStaves;
}

// Sorts by tick position only, so items sharing a tick keep the order they were
// given. That order is meaningful and must not be disturbed: the members of a
// chord all sit at one tick, and MusicXML encodes a chord by omitting <chord>
// from its first note. A plain std::sort is unstable, so the chord's spelling
// would depend on the standard library implementation.
void ScoreData::sort()
{
    for (auto &part : parts)
    {
        for (auto &measure : part.measures)
        {
            for (auto &staff : measure.staves)
            {

                const auto clefCompare = [](const ClefData &inLeft, const ClefData &inRight) {
                    return inLeft.tickTimePosition < inRight.tickTimePosition;
                };

                std::stable_sort(std::begin(staff.clefs), std::end(staff.clefs), clefCompare);

                const auto directionCompare = [](const DirectionData &inLeft, const DirectionData &inRight) {
                    return inLeft.tickTimePosition < inRight.tickTimePosition;
                };

                std::stable_sort(std::begin(staff.directions), std::end(staff.directions), directionCompare);

                for (auto &voice : staff.voices)
                {
                    const auto noteCompare = [](const NoteData &inLeft, const NoteData &inRight) {
                        return inLeft.tickTimePosition < inRight.tickTimePosition;
                    };

                    std::stable_sort(std::begin(voice.second.notes), std::end(voice.second.notes), noteCompare);
                }
            }
        }
    }
}

std::vector<PartData>::iterator findPart(std::vector<PartData> &inParts, const std::string &inPartId)
{
    auto predicate = [&inPartId](const PartData &partData) { return inPartId == partData.uniqueId; };

    auto it = std::find_if(inParts.begin(), inParts.end(), predicate);
    return it;
}
} // namespace api
} // namespace mx
