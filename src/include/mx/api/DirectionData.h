// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ChordData.h"
#include "mx/api/DirectionChoice.h"
#include "mx/api/FiguredBassData.h"
#include "mx/api/SoundData.h"

#include <optional>

namespace mx
{
namespace api
{
// MusicXML Documentation: A direction is a musical indication that is not attached to a specific
// note. Two or more may be combined to indicate starts and stops of wedges, dashes, etc.
//
// By default, a series of direction-type elements and a series of child elements of a direction-
// type within a single direction element follow one another in sequence visually. For a series of
// direction-type children, non-positional formatting attributes are carried over from the
// previous element by default.
struct DirectionData
{
    int tickTimePosition;
    Placement placement;

    // The source's <offset>, in divisions, or absent when it had none. An <offset> nudges only
    // where the direction is *drawn*, shifting it away from the note it is anchored to; it does not
    // move that anchor. tickTimePosition holds the anchor -- the musical location the direction
    // belongs to -- so the drawn position is tickTimePosition + offset. This is a round-trip
    // fidelity knob: leave it absent when authoring and set tickTimePosition to where the direction
    // belongs; the reader fills it in only to preserve an <offset> already present in the source.
    // MusicXML Documentation: An offset is represented in terms of divisions, and indicates where
    // the direction will appear relative to the current musical location. This affects the visual
    // appearance of the direction. If an element within a direction includes a default-x
    // attribute, the offset value will be ignored when determining the appearance of that element.
    std::optional<int> offset;

    // a voice value of VALUE_UNSPECIFIED means unspecified
    int voice;

    // Direction elements are placed inside the StaffData object in mx api.  This would mean that
    // they are alyways assigned to a staff, however MusicXML makes the staff element optional.
    // in the case that we want a direction element to have no <staff> element then me must place
    // the direction on staffIndex = 0 and set isStaffValueSpecified. otherwise the we will
    // assume that the <staff> element is present with a value of staffIndex + 1. This bool is
    // only relevant for Directions placed on staff zero, it is otherwise ignored.
    bool isStaffValueSpecified;

    // The <sound> element carried by this direction. It is only meaningful when
    // isSoundDataSpecified is true. A direction whose only content is a sound round-trips as a
    // standalone <sound> within the measure; a direction with other content writes the sound as a
    // child of the <direction> element.
    bool isSoundDataSpecified;
    SoundData soundData;

    // The direction's content: one DirectionChoice per <direction-type>, in document order.
    // Order is meaningful -- consecutive direction-types read as one visual sequence, so the
    // words "più" followed by a forte dynamic are two entries in that order.
    std::vector<DirectionChoice> directionTypes;

    // Harmony chord symbols anchored at this direction's musical position. A chord symbol is not
    // direction-type content; it serializes as its own <harmony> element alongside the
    // <direction>.
    std::vector<ChordData> chords;

    // Figured-bass figures anchored at this direction's musical position. Like chords, these
    // serialize as their own <figured-bass> elements, not as direction-type content.
    std::vector<FiguredBassData> figuredBasses;

    DirectionData()
        : tickTimePosition{0}, placement{Placement::unspecified}, offset{}, voice{VALUE_UNSPECIFIED},
          isStaffValueSpecified{true}, isSoundDataSpecified{false}, soundData{}, directionTypes{}, chords{},
          figuredBasses{}
    {
    }
};

inline bool isDirectionDataEmpty(const DirectionData &directionData)
{
    return directionData.directionTypes.empty() && directionData.chords.empty() &&
           directionData.figuredBasses.empty() && !directionData.isSoundDataSpecified;
}

MXAPI_EQUALS_BEGIN(DirectionData)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(placement)
MXAPI_EQUALS_MEMBER(offset)
MXAPI_EQUALS_MEMBER(voice)
MXAPI_EQUALS_MEMBER(isStaffValueSpecified)
MXAPI_EQUALS_MEMBER(isSoundDataSpecified)
MXAPI_EQUALS_MEMBER(soundData)
MXAPI_EQUALS_MEMBER(directionTypes)
MXAPI_EQUALS_MEMBER(chords)
MXAPI_EQUALS_MEMBER(figuredBasses)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(DirectionData);
} // namespace api
} // namespace mx
