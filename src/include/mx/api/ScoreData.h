// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/DefaultsData.h"
#include "mx/api/EncodingData.h"
#include "mx/api/LayoutData.h"
#include "mx/api/PageData.h"
#include "mx/api/PageImageData.h"
#include "mx/api/PageTextData.h"
#include "mx/api/PartData.h"
#include "mx/api/PartGroupData.h"
#include "mx/api/SystemData.h"

#include <map>
#include <vector>

namespace mx
{
namespace api
{
// Frozen for source compatibility; read-side informational only. The reader
// sets ThreePointZero iff the parsed document's declared version string is
// exactly "3.0". This field no longer influences output: mx writes MusicXML
// 4.0 documents, and the written root version attribute is always "4.0".
enum class MusicXmlVersion
{
    unspecified,
    ThreePointZero,
};

// ScoreData is the root of the api's data model. Notes live several levels
// down a containment hierarchy: ScoreData::parts (PartData) ->
// PartData::measures (MeasureData) -> MeasureData::staves (StaffData) ->
// StaffData::voices (VoiceData, keyed by voice number) -> VoiceData::notes
// (NoteData). NoteData::tickTimePosition locates a note within its measure
// only (see the comment on that field in NoteData.h); getting a note's
// absolute position in the part requires summing prior measures' lengths
// yourself. For a worked example of walking this hierarchy and computing
// absolute tick positions, see
// src/private/mx/examples/IterateNotesAndTiming.cpp in the mx repo.
class ScoreData
{
  public:
    ScoreData();
    MusicXmlVersion musicXmlVersion;

    // Read side, verbatim preservation: exactly what the parsed root's
    // version attribute carried, empty if absent. Not used when writing.
    std::string declaredMusicXmlVersion;
    std::string musicXmlType;
    std::string workTitle;
    std::string workNumber;
    std::string movementTitle;
    std::string movementNumber;
    std::string composer;
    std::string lyricist;
    std::string arranger;
    std::string publisher;
    std::string copyright;
    EncodingData encoding;
    std::vector<PageTextData> pageTextItems;

    /// `<credit-image>` items. Each corresponds to a `<credit>` whose
    /// content is an image rather than text.
    std::vector<PageImageData> pageImageItems;

    /// Specifies the scaling factor (`tenths` and `millimeters`), the default page sizes and margins, and the
    /// default system spacing and margins. Corresponds to the `<defaults>` MusicXML element. (Note this field
    /// used to be named `layout` and was renamed to `defaults` in v0.5.0.)
    DefaultsData defaults;
    std::vector<PartData> parts;
    std::vector<PartGroupData> partGroups;
    int ticksPerQuarter;

    /// Specifies page breaks, system breaks, and changes to system and page layout. Global/default page and
    /// system layout settings should be set in `defaults`. MeasureIndex is the index of the measure in which
    /// the layout/break is applied. For example, a newPage at MeasureIndex 10 indicates that measure index 10
    /// will be the first measure on a new page. `layout` corresponds to the `<print>` MusicXML element. (Note:
    /// prior to v0.5.0 we had `std::set<SystemData> systems`. The `layout` map replaces `systems`.)
    std::map<MeasureIndex, LayoutData> layout;

    int getNumMeasures() const;
    int getNumStavesPerSystem() const;

    /// sorts all of the events, directions, etc.
    /// it is good to call this before writing to xml.
    void sort();
};

std::vector<PartData>::iterator findPart(std::vector<PartData> &inParts, const std::string &inPartId);

MXAPI_EQUALS_BEGIN(ScoreData)
MXAPI_EQUALS_MEMBER(musicXmlVersion)
MXAPI_EQUALS_MEMBER(declaredMusicXmlVersion)
MXAPI_EQUALS_MEMBER(musicXmlType)
MXAPI_EQUALS_MEMBER(workTitle)
MXAPI_EQUALS_MEMBER(workNumber)
MXAPI_EQUALS_MEMBER(movementTitle)
MXAPI_EQUALS_MEMBER(movementNumber)
MXAPI_EQUALS_MEMBER(composer)
MXAPI_EQUALS_MEMBER(lyricist)
MXAPI_EQUALS_MEMBER(arranger)
MXAPI_EQUALS_MEMBER(publisher)
MXAPI_EQUALS_MEMBER(copyright)
MXAPI_EQUALS_MEMBER(encoding)
MXAPI_EQUALS_MEMBER(pageTextItems)
MXAPI_EQUALS_MEMBER(pageImageItems)
MXAPI_EQUALS_MEMBER(defaults)
MXAPI_EQUALS_MEMBER(parts)
MXAPI_EQUALS_MEMBER(partGroups)
MXAPI_EQUALS_MEMBER(ticksPerQuarter)
MXAPI_EQUALS_MEMBER(layout)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(ScoreData);
} // namespace api
} // namespace mx
