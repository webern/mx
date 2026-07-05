// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/CurveData.h"
#include "mx/api/DurationData.h"
#include "mx/api/LyricData.h"
#include "mx/api/NoteAttachmentData.h"
#include "mx/api/PitchData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <optional>
#include <vector>

namespace mx
{
namespace api
{
enum class Notehead
{
    slash,
    triangle,
    diamond,
    square,
    cross,
    x,
    circleX,
    invertedTriangle,
    arrowDown,
    arrowUp,
    slashed,
    backSlashed,
    normal,
    cluster,
    circleDot,
    leftTriangle,
    rectangle,
    none,
    do_,
    re,
    mi,
    fa,
    faUp,
    so,
    la,
    ti,
    circled, // MusicXML 4.0
    other    // MusicXML's 'other' notehead value
};

enum class Beam
{
    unspecified,
    begin,
    end,
    extend,
    forwardBroken,
    backwardBroken
};

enum class Stem
{
    unspecified,
    up,
    down,
    none,
    both
};

// A lone <tied type="let-ring">: no matching stop, and no sound-level <tie>
// counterpart (whose type is start/stop only). Modeled as its own struct
// rather than folding into NoteData's isTieStart / isTieStop.
struct TieLetRing
{
    // Visual attributes carried by the <tied type="let-ring"> element.
    PositionData positionData; // default-x/y, relative-x/y, placement
    CurveOrientation curveOrientation;
    bool isColorSpecified;
    ColorData colorData;

    TieLetRing() : positionData{}, curveOrientation{CurveOrientation::unspecified}, isColorSpecified{false}, colorData{}
    {
    }
};

MXAPI_EQUALS_BEGIN(TieLetRing)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(curveOrientation)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TieLetRing);

class NoteData
{
  public:
    NoteData();

    bool isRest;                       // if isRest is true then isUnpitched can be ignored
    bool isMeasureRest;                // only valid if isRest is true
    bool isUnpitched;                  // only relevant if isRest is false
    bool isDisplayStepOctaveSpecified; // MusicXML can optionally specify display step and octave for rests and
                                       // unpitched notes

    // This does *not* indicate the presence of absence of a <chord> tag.  Instead isChord
    // denotes whether or not the note is a member of a chord.  This is translated from/to
    // MusicXML's <chord> usage rules (the first note of a chord does *not* have a <chord>
    // tag, but subsequent chord notes do have the tag).
    bool isChord;

    // One field, two encodings: on write these emit both <tie> (sound) and
    // <tied> (notation), so the two can never contradict each other.
    bool isTieStart;
    bool isTieStop;

    // A laissez-vibrer / let-ring tie on this note (a lone <tied type="let-ring">
    // with no matching stop). Independent of isTieStart / isTieStop. See TieLetRing.
    std::optional<TieLetRing> tieLetRing;

    // MusicXML's four <note> flavors -- normal, grace, cue, and grace-cue --
    // are the four combinations of these two independent flags. Schema facts:
    // a grace note carries no <duration> on the wire (durationTimeTicks reads
    // as 0 and is ignored on write), and cue notes -- including grace-cue
    // notes -- cannot carry <tie> (ties on them are silently dropped on write).
    bool isGrace;
    bool isCue;

    Notehead notehead;
    PitchData pitchData; // step, alter, octave, accidental, etc
    int userRequestedVoiceNumber;

    // The zero-based index of the staff on which this note is displayed, for the rare case
    // that it differs from the staff that contains the note (cross-staff notation, e.g. a
    // beamed piano run or a chord that dips onto the other staff). The note stays in its
    // home staff's voice - containment still governs voice membership, timing, and beam
    // adjacency - and the writer emits this value in the note's <staff> element instead of
    // the containing staff's number. Leave it empty for normal notes; only set it to the
    // index of another existing staff in the same part. The reader sets it only for notes
    // whose <staff> diverges from their beam group's or chord's home staff.
    std::optional<int> crossStaffIndex;

    Stem stem;
    PositionData stemPositionData;

    // the time location of the note, within the measure, denominated in ticksPerQuarter which
    // is defined in ScoreData. in each measure, the note with tickTimePosition 0 is located at
    // the start of the measure. if ScoreData defines ticksPerQuarter as N, then the note
    // located at tickTimePosition N will be located one quarter note after the start of the
    // measure. MusicXML's <forward> and <backup> tags will be automatically placed into the
    // MusicXML as required to facilitate the correct timing of notes based on their
    // tickTimePosition values.
    int tickTimePosition;
    DurationData durationData;

    std::vector<Beam> beams;
    PositionData positionData;
    PrintData printData;

    NoteAttachmentData noteAttachmentData;
    std::vector<LyricData> lyrics;

    // these strings will be stuffed into the editorial <footnote> element
    // preceded by the string ##misc-data##. for example if you have the
    // strings Bob, Cat, Hello then we will get:
    // <footnote print-object="no">##misc-data##Bob,Cat,Hello</footnote>
    // similarly when parsing if we see ##misc-data## in this element, it
    // will be parsed into these strings. do not use commas in your misc
    // data strings as these are the delimiter
    std::vector<std::string> miscData;
};

MXAPI_EQUALS_BEGIN(NoteData)
MXAPI_EQUALS_MEMBER(isRest)
MXAPI_EQUALS_MEMBER(isMeasureRest)
MXAPI_EQUALS_MEMBER(isUnpitched)
MXAPI_EQUALS_MEMBER(isDisplayStepOctaveSpecified)
MXAPI_EQUALS_MEMBER(isChord)
MXAPI_EQUALS_MEMBER(isTieStart)
MXAPI_EQUALS_MEMBER(isTieStop)
MXAPI_EQUALS_MEMBER(tieLetRing)
MXAPI_EQUALS_MEMBER(isGrace)
MXAPI_EQUALS_MEMBER(isCue)
MXAPI_EQUALS_MEMBER(pitchData)
MXAPI_EQUALS_MEMBER(userRequestedVoiceNumber)
MXAPI_EQUALS_MEMBER(crossStaffIndex)
MXAPI_EQUALS_MEMBER(stem)
MXAPI_EQUALS_MEMBER(stemPositionData)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(durationData)
MXAPI_EQUALS_MEMBER(beams)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(noteAttachmentData)
MXAPI_EQUALS_MEMBER(lyrics)
MXAPI_EQUALS_MEMBER(miscData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(NoteData);
} // namespace api
} // namespace mx
