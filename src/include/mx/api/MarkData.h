// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DynamicsData.h"
#include "mx/api/MarkDataChoice.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
enum class MarkType
{
    unspecified,

    // articulations
    accent,
    strongAccent, // marcato, direction unspecified
    strongAccentUp,
    strongAccentDown,
    staccato,
    tenuto,
    detachedLegato, // tenuto with dot
    staccatissimo,
    spiccato, // ? using "StaccatissimoWedge"
    scoop,
    plop,
    doit,
    falloff,
    breathMark,
    caesura, // an empty <caesura/>, the common form; renders the same as caesuraNormal
    caesuraNormal,
    caesuraThick,
    caesuraShort,
    caesuraCurved,
    caesuraSingle,
    stress,
    unstress,
    softAccent,
    otherArticulation,

    // dynamics
    dynamics, ///< The symbol itself is in MarkData::choice -- a StandardDynamic such as ff, or a
              ///< CompoundDynamicsData for marks like ffz that MusicXML spells with several symbols

    // ornaments
    trillMark,
    turn,
    delayedTurn,
    invertedTurn,
    delayedInvertedTurn,
    verticalTurn,
    invertedVerticalTurn,
    haydn,
    shake,
    mordent,
    invertedMordent,
    schleifer,
    // One-note tremolos: slashes struck through the stem of a single note, not a spanner across
    // two. MusicXML allows 1 through 8 of them. SMuFL provides a precomposed glyph for only the
    // first five (tremolo1 through tremolo5); because those are combining marks, a renderer builds
    // the higher counts by stacking them.
    tremoloSingleOne,   ///< A one-note tremolo with 1 slash
    tremoloSingleTwo,   ///< A one-note tremolo with 2 slashes
    tremoloSingleThree, ///< A one-note tremolo with 3 slashes, the customary number
    tremoloSingleFour,  ///< A one-note tremolo with 4 slashes
    tremoloSingleFive,  ///< A one-note tremolo with 5 slashes
    tremoloSingleSix,   ///< A one-note tremolo with 6 slashes
    tremoloSingleSeven, ///< A one-note tremolo with 7 slashes
    tremoloSingleEight, ///< A one-note tremolo with 8 slashes
    tremoloStart,       ///< The first note of a measured (two-note) tremolo; slash count is in MarkData::choice
    tremoloStop,        ///< The second note of a measured (two-note) tremolo; slash count is in MarkData::choice
    tremoloUnmeasured,  ///< An unmeasured tremolo (buzz roll); its glyph is named in MarkData::choice
    otherOrnament,      ///< MusicXML's 'other-ornament' value
    unknownOrnament,    ///< Error state

    // accidental marks
    accidentalMarkSharp,
    accidentalMarkNatural,
    accidentalMarkFlat,
    accidentalMarkDoubleSharp,
    accidentalMarkSharpSharp,
    accidentalMarkFlatFlat,
    accidentalMarkNaturalSharp,
    accidentalMarkNaturalFlat,
    accidentalMarkQuarterFlat,
    accidentalMarkQuarterSharp,
    accidentalMarkThreeQuartersFlat,
    accidentalMarkThreeQuartersSharp,
    accidentalMarkSharpDown,
    accidentalMarkSharpUp,
    accidentalMarkNaturalDown,
    accidentalMarkNaturalUp,
    accidentalMarkFlatDown,
    accidentalMarkFlatUp,
    accidentalMarkDoubleSharpDown,
    accidentalMarkDoubleSharpUp,
    accidentalMarkFlatFlatDown,
    accidentalMarkFlatFlatUp,
    accidentalMarkArrowDown,
    accidentalMarkArrowUp,
    accidentalMarkTripleSharp,
    accidentalMarkTripleFlat,
    accidentalMarkSlashQuarterSharp,
    accidentalMarkSlashSharp,
    accidentalMarkSlashFlat,
    accidentalMarkDoubleSlashFlat,
    accidentalMarkSharp1,
    accidentalMarkSharp2,
    accidentalMarkSharp3,
    accidentalMarkSharp5,
    accidentalMarkFlat1,
    accidentalMarkFlat2,
    accidentalMarkFlat3,
    accidentalMarkFlat4,
    accidentalMarkSori,
    accidentalMarkKoron,
    accidentalUnknown,

    // technical
    upBow,
    downBow,
    harmonic,
    openString,
    thumbPosition,
    fingering, ///< carries text (e.g. "1", "2-3") in MarkData::name plus optional substitution/alternate flags
    pluck,     ///< carries text (e.g. "p", "i", "m", "a") in MarkData::name
    doubleTongue,
    tripleTongue,
    stopped,
    snapPizzicato,
    fret,
    string_,
    // hammerOn,
    // pullOff,
    // bend,
    // tap,
    heel,
    toe,
    fingernails,
    hole,
    arrow,
    handbell,
    brassBend,
    flip,
    smear,
    open,
    halfMuted,
    harmonMute,
    golpe,
    otherTechnical,
    unknownTechnical,

    // fermata
    fermata,             // <fermata/>
    fermataNormal,       // <fermata>normal</fermata>
    fermataAngled,       // <fermata>angled</fermata>
    fermataSquare,       // <fermata>square</fermata>
    fermataDoubleAngled, // <fermata>double-angled</fermata> (MusicXML 4.0)
    fermataDoubleSquare, // <fermata>double-square</fermata> (MusicXML 4.0)
    fermataDoubleDot,    // <fermata>double-dot</fermata> (MusicXML 4.0)
    fermataHalfCurve,    // <fermata>half-curve</fermata> (MusicXML 4.0)
    fermataCurlew,       // <fermata>curlew</fermata> (MusicXML 4.0)

    // sadly, in MusicXML we do not have an 'above' 'below' attribute for the fermata.
    // instead it has an attribute 'type' that is either 'upright' or 'inverted'.
    // it appears that implementations interpret 'upright' to mean the 'normal' or
    // correct orientation and 'inverted' to mean that the fermata should be 'flipped'
    // from its correct orientation.  in other words the implementation has to figure
    // out if the fermata is above or below the note on its own (using its own
    // algorithm combined with any default-y and relative-y values).  this mess is
    // going to be beyond the scope of the MusicXML Class Library at this time.  You
    // cause the fermata 'type' attribute to be set using the enum values below, but
    // it's not possible for this library to calculate the correct fermata glyph.
    fermataUpright,        // <fermata type="upright"/>
    fermataNormalUpright,  // <fermata type="upright">normal</fermata>
    fermataAngledUpright,  // <fermata type="upright">angled</fermata>
    fermataSquareUpright,  // <fermata type="upright">square</fermata>
    fermataInverted,       // <fermata type="inverted"/>
    fermataNormalInverted, // <fermata type="inverted">normal</fermata>
    fermataAngledInverted, // <fermata type="inverted">angled</fermata>
    fermataSquareInverted, // <fermata type="inverted">square</fermata>
    unknownFermata,        // bad state, unknown type, error

    pedal, // the typical piano pedal mark. for pedal marks with lines, see PedalLineData
    damp,  // the typical piano end-pedal mark. for pedal marks with lines, see PedalLineData

    // arpeggiate
    arpeggiate,
    arpeggiateDown,
    arpeggiateUp,

    // nonArpeggiate
    nonArpeggiate,

    // general notation extension
    otherNotation,

    // these are cust additions that will be written to, and read from, the
    // other-articulations (or other-*) elements.
    customErrorUnknown, // used to represent an error when parsing from a string
    customAccentTenuto,
};

static constexpr const char *const markStringCustomErrorUnknown = "";
static constexpr const char *const markStringCustomAccentTenuto = "gjhsldjhglskdjhfg";

bool isMarkOrnament(MarkType);
bool isMarkPedal(MarkType);
bool isMarkTechnical(MarkType);
bool isMarkArticulation(MarkType);
bool isMarkTremolo(MarkType);
bool isMarkDynamic(MarkType);
bool isMarkFermata(MarkType);
bool isMarkArpeggiate(MarkType);
bool isMarkNonArpeggiate(MarkType);
bool isMarkOtherNotation(MarkType);

bool isMarkCustom(MarkType);
std::string getCustomMarkName(MarkType);
MarkType getMarkTypeFromCustomString(const std::string &);
int numTremoloSlashes(MarkType);

struct MarkData
{
    // Fields common to (nearly) every mark, regardless of markType.
    MarkType markType;

    // The mark's text. For marks whose text is the data -- fingering, pluck, fret, string, and the
    // other-* marks -- this is what gets written. For marks that name themselves -- articulations,
    // fermatas, dynamics -- it spells the mark out ("ff", "ffz") and the writer ignores it,
    // emitting whatever markType and choice say. A self-naming mark fills this in when it is
    // constructed, so if you replace choice afterwards, re-derive it: toString() in DynamicsData.h
    // spells a dynamic.
    std::string name;
    int tickTimePosition;
    PrintData printData;
    PositionData positionData;

    // Fields below apply only to specific mark types, kept as direct fields for backward
    // compatibility (see AGENTS.md "mx::api conventions" / issue #249; these are not migrated to
    // 'choice'). Do not follow this shape for new mark-specific data -- add a new MarkDataChoice
    // alternative instead (see 'choice' below).

    // Mordent payload attributes (MusicXML <mordent> 'long', 'approach', 'departure'). Only
    // meaningful when markType == MarkType::mordent or MarkType::invertedMordent.
    Bool mordentLong;
    bool hasMordentLong;
    Placement mordentApproach;
    bool hasMordentApproach;
    Placement mordentDeparture;
    bool hasMordentDeparture;

    // Fingering payload attributes (MusicXML <fingering> 'substitution' and
    // 'alternate'). Only meaningful when markType == MarkType::fingering. The
    // fingering text itself (e.g. "1", "2-3") is carried in 'name'.
    Bool fingeringSubstitution;
    Bool fingeringAlternate;

    // Payload for mark types whose data does not fit the common fields above. Its Kind SHOULD
    // correspond to markType (e.g. MarkDataChoice::Kind::tremolo pairs with the MarkType::tremolo*
    // values), but this is a convention that MarkData does not enforce. New mark-specific data
    // belongs here, as a new MarkDataChoice alternative -- see MarkDataChoice.h.
    MarkDataChoice choice;

    MarkData();
    MarkData(MarkType inMarkType);
    MarkData(Placement inPlacement, MarkType inMarkType);

    // Builds a dynamic mark: markType is MarkType::dynamics, choice holds the symbol, and name is
    // its letters.
    MarkData(StandardDynamic inDynamic);

    // Builds a dynamic mark spelled with several symbols, such as ff followed by z for ffz. name
    // becomes the letters of the whole mark. A lone standard symbol collapses to the same mark the
    // StandardDynamic constructor builds.
    MarkData(CompoundDynamicsData inDynamics);
};

MXAPI_EQUALS_BEGIN(MarkData)
MXAPI_EQUALS_MEMBER(markType)
MXAPI_EQUALS_MEMBER(name)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(mordentLong)
MXAPI_EQUALS_MEMBER(hasMordentLong)
MXAPI_EQUALS_MEMBER(mordentApproach)
MXAPI_EQUALS_MEMBER(hasMordentApproach)
MXAPI_EQUALS_MEMBER(mordentDeparture)
MXAPI_EQUALS_MEMBER(hasMordentDeparture)
MXAPI_EQUALS_MEMBER(fingeringSubstitution)
MXAPI_EQUALS_MEMBER(fingeringAlternate)
MXAPI_EQUALS_MEMBER(choice)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MarkData);
} // namespace api
} // namespace mx
