// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/MeasureData.h"
#include "mx/api/SoundID.h"
#include "mx/api/TransposeData.h"

#include <optional>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
enum class SoloOrEnsemble
{
    unspecified,
    solo,
    ensemble
};

struct MidiData
{
    std::string virtualLibrary;
    std::string virtualName;
    std::string device;
    std::string name;

    // -1 indicates absence of value
    int bank;

    // -1 indicates absence of value
    int channel;

    // -1 indicates absence of value
    int program;

    // -1 indicates absence of value
    int unpitched;

    // percent, valid range 0.0 to 100.0
    double volume;
    bool isVolumeSpecified;

    // rotation degrees, valid range -180.0 to +180.0
    double pan;
    bool isPanSpecified;

    // rotation degrees, valid range -180.0 to +180.0
    double elevation;
    bool isElevationSpecified;

    MidiData()
        : virtualLibrary{}, virtualName{}, device{}, name{}, bank{-1}, channel{-1}, program{-1}, unpitched{-1},
          volume{0.0}, isVolumeSpecified{false}, pan{0.0}, isPanSpecified{false}, elevation{0.0},
          isElevationSpecified{false}
    {
    }
};

struct InstrumentData
{
    std::string uniqueId;
    std::string name;
    std::string abbreviation;
    SoundID soundID;
    SoloOrEnsemble soloOrEnsemble;
    MidiData midiData;

    InstrumentData()
        : uniqueId{}, name{}, abbreviation{}, soundID{SoundID::unspecified},
          soloOrEnsemble{SoloOrEnsemble::unspecified}, midiData{}
    {
    }
};

class PartData
{
  public:
    std::string uniqueId;

    // ---------------------------------------------------------------------
    // Part name / abbreviation, and the MusicXML "two places" problem
    // ---------------------------------------------------------------------
    //
    // MusicXML can express how a part is named in two overlapping ways, and
    // that overlap is a perennial source of confusion. mx::api deliberately
    // collapses it into a single, simpler model. The reasoning:
    //
    //   1. <part-name> (and <part-abbreviation>) carry the canonical, plain-
    //      text name. <part-name> is REQUIRED by the schema, so mx always
    //      writes it (even when empty). Its *formatting* attributes -- font,
    //      color, justify and the print-style position attributes -- were
    //      deprecated in MusicXML 2.0 in favor of the *-display elements.
    //
    //   2. <part-name-display> (and <part-abbreviation-display>) are the
    //      modern, non-deprecated home for a formatted rendering of the name.
    //
    // So the same *formatting* has two possible homes. mx::api keeps exactly
    // one -- the display* fields below -- and follows these rules:
    //
    //   * On read, formatting is taken from wherever it lives. Formatting
    //     found on the deprecated <part-name> attributes is migrated into the
    //     display* model. If a *-display element is also present it WINS over
    //     the deprecated attributes (the non-deprecated location is canonical).
    //   * On write, formatting is emitted ONLY to the non-deprecated *-display
    //     elements. mx never writes the deprecated formatting attributes back
    //     onto <part-name>. A document that used the deprecated attributes is
    //     therefore normalized to the modern form; by design this does not
    //     round-trip byte-for-byte.
    //
    // This is purely about *formatting*. The two name strings are not pure
    // duplicates: <part-name> is the canonical text, while <part-name-display>
    // may hold a different, richer rendering (e.g. with accidental glyphs).
    // That is why both `name` and `displayName` are retained.

    // The canonical part name: the text of the required <part-name> element.
    // May be empty; see effectiveName() for the "fall back to the display
    // name when this is empty" convenience.
    std::string name;

    // The canonical part abbreviation: the text of <part-abbreviation>. The
    // element is optional, so this is only written when non-empty.
    std::string abbreviation;

    // Visibility of <part-name> / <part-abbreviation> via the print-object
    // attribute. print-object is NOT part of the 2.0 formatting deprecation:
    // it is an independent property controlling whether the name is rendered,
    // and mx round-trips it faithfully:
    //   unspecified -> no print-object attribute is written (defaults to shown)
    //   yes / no    -> print-object="yes" / "no" is written verbatim
    // (Historically mx force-wrote print-object="no" on every part, silently
    // hiding otherwise-visible names. That conflated print-object with the
    // formatting deprecation; it was incorrect and has been removed.)
    Bool namePrintObject = Bool::unspecified;
    Bool abbreviationPrintObject = Bool::unspecified;

    std::string displayName;
    PrintData displayNamePrintData;
    PositionData displayNamePositionData;

    std::string displayAbbreviation;
    PrintData displayAbbreviationPrintData;
    PositionData displayAbbreviationPositionData;

    InstrumentData instrumentData;

    /// The initial transposition for the part. If the music entered into the part is not in
    /// concert pitch, then this field is used to specify the transposition that is in-
    /// effect. Note, MusicXML encodes transposition in the measure data, not in the part
    /// data. Here we offer a convient way to set the transposition in the first measure of
    /// the part. Subsequent transposition changes are not currently supported by mx::api.
    std::optional<TransposeData> transposition;

    std::vector<MeasureData> measures;

    // MusicXML requires <part-name>, but real files often leave it empty and
    // place the human-readable name only in <part-name-display>. These helpers
    // return the name to actually show: the canonical `name` when present,
    // otherwise the `displayName` (and likewise for the abbreviation). The
    // stored fields are left untouched, so the original document still
    // round-trips.
    inline const std::string &effectiveName() const
    {
        return !name.empty() ? name : displayName;
    }

    inline const std::string &effectiveAbbreviation() const
    {
        return !abbreviation.empty() ? abbreviation : displayAbbreviation;
    }

    inline int getNumStaves() const
    {
        int numStaves = 0;

        for (const auto &measure : measures)
        {
            int temp = static_cast<int>(measure.staves.size());
            if (temp > numStaves)
            {
                numStaves = temp;
            }
        }

        return numStaves;
    }

    inline int getMaxVoice() const
    {
        int maxVoice = 0;

        for (const auto &measure : measures)
        {
            for (const auto &staff : measure.staves)
            {
                for (const auto &voice : staff.voices)
                {
                    int temp = static_cast<int>(voice.first);
                    if (temp > maxVoice)
                    {
                        maxVoice = temp;
                    }
                }
            }
        }

        return maxVoice;
    }
};

MXAPI_EQUALS_BEGIN(MidiData)
MXAPI_EQUALS_MEMBER(virtualLibrary)
MXAPI_EQUALS_MEMBER(virtualName)
MXAPI_EQUALS_MEMBER(device)
MXAPI_EQUALS_MEMBER(name)
MXAPI_EQUALS_MEMBER(bank)
MXAPI_EQUALS_MEMBER(channel)
MXAPI_EQUALS_MEMBER(program)
MXAPI_EQUALS_MEMBER(unpitched)
MXAPI_EQUALS_MEMBER(volume)
MXAPI_EQUALS_MEMBER(isVolumeSpecified)
MXAPI_EQUALS_MEMBER(pan)
MXAPI_EQUALS_MEMBER(isPanSpecified)
MXAPI_EQUALS_MEMBER(elevation)
MXAPI_EQUALS_MEMBER(isElevationSpecified)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MidiData);

MXAPI_EQUALS_BEGIN(InstrumentData)
MXAPI_EQUALS_MEMBER(uniqueId)
MXAPI_EQUALS_MEMBER(name)
MXAPI_EQUALS_MEMBER(abbreviation)
MXAPI_EQUALS_MEMBER(soundID)
MXAPI_EQUALS_MEMBER(soloOrEnsemble)
MXAPI_EQUALS_MEMBER(midiData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(InstrumentData);

MXAPI_EQUALS_BEGIN(PartData)
MXAPI_EQUALS_MEMBER(uniqueId)
MXAPI_EQUALS_MEMBER(name)
MXAPI_EQUALS_MEMBER(abbreviation)
MXAPI_EQUALS_MEMBER(namePrintObject)
MXAPI_EQUALS_MEMBER(abbreviationPrintObject)
MXAPI_EQUALS_MEMBER(displayName)
MXAPI_EQUALS_MEMBER(displayNamePrintData)
MXAPI_EQUALS_MEMBER(displayNamePositionData)
MXAPI_EQUALS_MEMBER(displayAbbreviation)
MXAPI_EQUALS_MEMBER(displayAbbreviationPrintData)
MXAPI_EQUALS_MEMBER(displayAbbreviationPositionData)
MXAPI_EQUALS_MEMBER(instrumentData)
MXAPI_EQUALS_MEMBER(transposition)
MXAPI_EQUALS_MEMBER(measures)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PartData);
} // namespace api
} // namespace mx
