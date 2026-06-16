// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/ChordData.h"

namespace mx
{
namespace api
{

////////////////////////////////////////////////////////////////////////////////
// CTOR AND COPY ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Extension::Extension()
    : extensionType{ExtensionType::add}, extensionAlter{ExtensionAlter::none}, extensionNumber{ExtensionNumber::first},
      printObject{Bool::unspecified}
{
}

FrameNoteData::FrameNoteData()
    : stringNumber{1}, fretNumber{0}, fingering{0}, isFingeringSpecified{false}, barre{FrameBarre::none}
{
}

FrameData::FrameData() : stringCount{6}, fretCount{4}, firstFret{1}, isFirstFretSpecified{false}, notes{}
{
}

ChordData::ChordData()
    : harmonyChordSource{HarmonyChordSource::root}, root{Step::c}, rootAlter{0}, functionText{}, numeralRoot{0},
      numeralRootText{}, numeralAlter{0}, hasNumeralAlter{false}, hasNumeralKey{false}, numeralKeyFifths{0},
      numeralMode{NumeralMode::unspecified}, chordKind{ChordKind::unspecified}, text{}, useSymbols{Bool::unspecified},
      bass{Step::unspecified}, bassAlter{0}, inversion{0}, hasInversion{false}, extensions{}, miscData{},
      hasFrameData{false}, frameData{}, positionData{}
{
}

////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

} // namespace api
} // namespace mx
