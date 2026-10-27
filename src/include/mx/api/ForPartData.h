// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ClefData.h"
#include "mx/api/Id.h"

#include <optional>

namespace mx
{
namespace api
{
// The clef a transposed part uses when it differs from the clef in the concert score. A bass
// clarinet, for example, may read bass clef in the concert score and treble clef in its own part.
struct PartClefData
{
    ClefSymbol symbol = DEFAULT_CLEF_SYMBOL;

    // The staff line the clef sits on, counted from the bottom line as 1.
    int line = DEFAULT_CLEF_LINE;

    // How many octaves the clef transposes, as on an octave treble clef. 0 for an ordinary clef.
    int octaveChange = DEFAULT_CLEF_OCTAVE_CHANGE;
};

MXAPI_EQUALS_BEGIN(PartClefData)
MXAPI_EQUALS_MEMBER(symbol)
MXAPI_EQUALS_MEMBER(line)
MXAPI_EQUALS_MEMBER(octaveChange)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PartClefData);

// In a concert score, every instrument is written at sounding pitch, but the players still read
// transposed parts. ForPartData says how to make the part for one of those instruments: the
// interval its music is transposed by, and optionally the clef it is written in.
//
// MusicXML calls this <for-part>. It is only meaningful in a concert score, so whenever any
// measure carries one, the score is written as a concert score (see DefaultsData::concertScore).
//
// A ForPartData stays in effect until the next one for the same staff. To return to concert pitch
// in the part, add one with a chromatic and diatonic of 0.
struct ForPartData
{
    // The interval from concert pitch to the part's written pitch, in the same terms as
    // TransposeData: chromatic is the number of semitones, diatonic the number of letter-name
    // steps, both including any whole octaves. A Bb clarinet part is chromatic -2, diatonic -1.
    int chromatic = 0;
    int diatonic = 0;

    // The clef used in the part, when it should differ from the clef in the score.
    std::optional<PartClefData> partClef;

    // Which staff of the part this applies to, zero-based from the top staff. INDEX_UNSPECIFIED
    // applies it to every staff of the part.
    int staffIndex = INDEX_UNSPECIFIED;

    // Where in the measure the change takes effect.
    int tickTimePosition = 0;

    // The <for-part> element's id attribute (see Id.h).
    std::optional<Id> id;
};

MXAPI_EQUALS_BEGIN(ForPartData)
MXAPI_EQUALS_MEMBER(chromatic)
MXAPI_EQUALS_MEMBER(diatonic)
MXAPI_EQUALS_MEMBER(partClef)
MXAPI_EQUALS_MEMBER(staffIndex)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(ForPartData);
} // namespace api
} // namespace mx
