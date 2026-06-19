// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/MiscData.h"
#include "mx/api/PitchData.h"
#include "mx/api/PositionData.h"

namespace mx
{
namespace api
{
enum class ChordKind
{
    unspecified,
    major,
    minor,
    augmented,
    diminished,
    dominant,
    majorSeventh,
    minorSeventh,
    diminishedSeventh,
    augmentedSeventh,
    halfDiminished,
    majorMinor,
    majorSixth,
    minorSixth,
    dominantNinth,
    majorNinth,
    minorNinth,
    dominant11Th,
    major11Th,
    minor11Th,
    dominant13Th,
    major13Th,
    minor13Th,
    suspendedSecond,
    suspendedFourth,
    neapolitan,
    italian,
    french,
    german,
    pedal,
    power,
    tristan,
    other,
    none
};

enum class ExtensionType
{
    add,
    remove,
    alter
};

enum class ExtensionNumber
{
    first,
    second,
    third,
    fourth,
    fifth,
    sixth,
    seventh,
    ninth,
    eleventh,
    thirteenth,
};

enum class ExtensionAlter
{
    none,
    flat,
    sharp,
    doubleFlat,
    doubleSharp
};

class Extension
{
  public:
    Extension();

    ExtensionType extensionType;
    ExtensionAlter extensionAlter;
    ExtensionNumber extensionNumber;
    Bool printObject;

    inline int getAlterValue() const
    {
        switch (extensionAlter)
        {
        case ExtensionAlter::none:
            return 0;
        case ExtensionAlter::flat:
            return -1;
        case ExtensionAlter::sharp:
            return 1;
        case ExtensionAlter::doubleFlat:
            return -2;
        case ExtensionAlter::doubleSharp:
            return 2;
        }
        return 0;
    }

    inline void setAlterValue(int inAlterValue)
    {
        if (inAlterValue <= -2)
        {
            extensionAlter = ExtensionAlter::doubleFlat;
        }
        else if (inAlterValue == -1)
        {
            extensionAlter = ExtensionAlter::flat;
        }
        else if (inAlterValue == 0)
        {
            extensionAlter = ExtensionAlter::none;
        }
        else if (inAlterValue == 1)
        {
            extensionAlter = ExtensionAlter::sharp;
        }
        else if (inAlterValue >= 2)
        {
            extensionAlter = ExtensionAlter::doubleSharp;
        }
    }
};

MXAPI_EQUALS_BEGIN(Extension)
MXAPI_EQUALS_MEMBER(extensionType)
MXAPI_EQUALS_MEMBER(extensionAlter)
MXAPI_EQUALS_MEMBER(extensionNumber)
MXAPI_EQUALS_MEMBER(printObject)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(Extension);

enum class FrameBarre
{
    none,
    start,
    stop
};

class FrameNoteData
{
  public:
    FrameNoteData();

    int stringNumber;
    int fretNumber;
    int fingering;
    bool isFingeringSpecified;
    FrameBarre barre;
};

MXAPI_EQUALS_BEGIN(FrameNoteData)
MXAPI_EQUALS_MEMBER(stringNumber)
MXAPI_EQUALS_MEMBER(fretNumber)
MXAPI_EQUALS_MEMBER(fingering)
MXAPI_EQUALS_MEMBER(isFingeringSpecified)
MXAPI_EQUALS_MEMBER(barre)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(FrameNoteData);

class FrameData
{
  public:
    FrameData();

    int stringCount;
    int fretCount;
    int firstFret;
    bool isFirstFretSpecified;
    std::vector<FrameNoteData> notes;
};

MXAPI_EQUALS_BEGIN(FrameData)
MXAPI_EQUALS_MEMBER(stringCount)
MXAPI_EQUALS_MEMBER(fretCount)
MXAPI_EQUALS_MEMBER(firstFret)
MXAPI_EQUALS_MEMBER(isFirstFretSpecified)
MXAPI_EQUALS_MEMBER(notes)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(FrameData);

// A harmony-chord identifies its pitch source one of three mutually exclusive ways (the MusicXML
// harmony-chord choice): a 'root' (pop-style chord symbol, the default), a 'numeral' (Roman numeral
// / Nashville number for functional harmony, MusicXML 4.0), or the deprecated 'function' string.
enum class HarmonyChordSource
{
    root,
    numeral,
    function
};

// The mode used to interpret a numeral when it differs from the key signature
// (the MusicXML numeral-mode element). MusicXML requires a numeral-mode inside every numeral-key,
// so 'unspecified' is only meaningful when there is no numeral-key; combined with hasNumeralKey it
// is written out as 'major' (see DirectionWriter and the numeralMode field below).
enum class NumeralMode
{
    unspecified,
    major,
    minor,
    naturalMinor,
    melodicMinor,
    harmonicMinor
};

class ChordData
{
  public:
    ChordData();

  public:
    // Which of the mutually exclusive harmony-chord alternatives is in use. Defaults to 'root',
    // which preserves the historical behavior; 'root', 'rootAlter', 'numeral*', and 'functionText'
    // are interpreted according to this value.
    HarmonyChordSource harmonyChordSource;

    Step root;
    int rootAlter;

    // The deprecated MusicXML <function> element: Roman numeral text such as "V". Used only when
    // harmonyChordSource == function.
    std::string functionText;

    // The MusicXML 4.0 <numeral>. Used only when harmonyChordSource == numeral.
    // numeralRoot is a scale degree 1-7; numeralRootText is its optional display text (e.g. "V").
    int numeralRoot;
    std::string numeralRootText;
    // The numeral-alter (chromatic alteration of the numeral root), in semitones.
    int numeralAlter;
    bool hasNumeralAlter;
    // The optional <numeral-key> (key local to the numeral): numeral-fifths plus numeral-mode.
    // When hasNumeralKey is true, numeralMode must name a concrete mode: numeral-mode is required by
    // MusicXML inside numeral-key, so leaving it 'unspecified' is a contradiction that the writer
    // resolves by emitting 'major'.
    bool hasNumeralKey;
    int numeralKeyFifths;
    NumeralMode numeralMode;

    ChordKind chordKind;
    std::string text;
    Bool useSymbols;
    Step bass;
    int bassAlter;
    // The MusicXML <inversion> (0 = root position, 1 = first inversion, ...). Present only when
    // hasInversion is true.
    int inversion;
    bool hasInversion;
    std::vector<Extension> extensions;
    std::vector<MiscData> miscData;
    bool hasFrameData;
    FrameData frameData;
    PositionData positionData;
};

MXAPI_EQUALS_BEGIN(ChordData)
MXAPI_EQUALS_MEMBER(harmonyChordSource)
MXAPI_EQUALS_MEMBER(root)
MXAPI_EQUALS_MEMBER(rootAlter)
MXAPI_EQUALS_MEMBER(functionText)
MXAPI_EQUALS_MEMBER(numeralRoot)
MXAPI_EQUALS_MEMBER(numeralRootText)
MXAPI_EQUALS_MEMBER(numeralAlter)
MXAPI_EQUALS_MEMBER(hasNumeralAlter)
MXAPI_EQUALS_MEMBER(hasNumeralKey)
MXAPI_EQUALS_MEMBER(numeralKeyFifths)
MXAPI_EQUALS_MEMBER(numeralMode)
MXAPI_EQUALS_MEMBER(chordKind)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(useSymbols)
MXAPI_EQUALS_MEMBER(bass)
MXAPI_EQUALS_MEMBER(bassAlter)
MXAPI_EQUALS_MEMBER(inversion)
MXAPI_EQUALS_MEMBER(hasInversion)
MXAPI_EQUALS_MEMBER(extensions)
MXAPI_EQUALS_MEMBER(miscData)
MXAPI_EQUALS_MEMBER(hasFrameData)
MXAPI_EQUALS_MEMBER(frameData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(ChordData);

} // namespace api
} // namespace mx
