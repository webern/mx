// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PartData.h"
#include "mx/impl/DiagnosticsContext.h"
#include "mx/impl/MeasureCursor.h"

#include <memory>
#include <mutex>

namespace mx
{
namespace core
{
class ScorePartwise;
class PartwisePart;
class ScorePart;
class ScoreInstrument;
class PartName;
class NameDisplay;
class ScorePartMIDIGroup;
class MIDIInstrument;
} // namespace core

namespace impl
{
class PartReader
{
  public:
    PartReader(const core::ScorePart &inScorePart, const core::PartwisePart &inPartwisePartRef,
               int globalTicksPerMeasure, int partIndex, int inDivisionsValue, DiagnosticsContext diagnostics = {});

    api::PartData getPartData();
    impl::MeasureCursor getCursor() const;

  private:
    static bool nameHasDeprecatedFormatting(const core::PartName &name);
    static void readNameDisplay(const core::PartName &nameElement, const std::optional<core::NameDisplay> &display,
                                std::string &outText, api::PrintData &outPrintData, api::PositionData &outPositionData);

    const core::PartwisePart &myPartwisePart;
    const core::ScorePart &myScorePart;
    int myNumStaves;
    bool myIsStavesElementPresent;
    const int myGlobalTicksPerMeasure;
    int myPartIndex;
    const int myConstructedDivisionsValue;
    DiagnosticsContext myDiagnostics;
    MeasureCursor myCurrentCursor;
    MeasureCursor myPreviousCursor;

  private:
    mutable std::mutex myMutex;
    mutable api::PartData myOutPartData;

    // Returns the part's staff count. outIsStavesElementPresent reports whether the source said
    // so with a <staves> element, which the count alone cannot tell you when it is one.
    int calculateNumStaves(bool &outIsStavesElementPresent) const;
    void parseScorePart() const;
    void parseScoreInstrument(const core::ScoreInstrument &scoreInstrument, api::InstrumentData &outInstrument) const;
    // The instrument of myOutPartData that a midi group names by id, or nullptr when it names none.
    // outIsAdditional reports whether that is one of the part's additional instruments.
    api::InstrumentData *findMidiGroupInstrument(const core::ScorePartMIDIGroup &grp, bool &outIsAdditional) const;
    void parseMidiDeviceInstrumentGroup(const core::ScorePartMIDIGroup &grp, bool isAdditional,
                                        api::InstrumentData &outInstrument) const;
    void parseMidiInstrument(const core::MIDIInstrument &inst, api::MidiData &outMidiData) const;

    template <typename ELEMENT_TYPE> void updateNumStaves(const ELEMENT_TYPE &element, int &outNumStaves) const
    {
        if (element.staff().has_value())
        {
            int temp = element.staff()->value();
            if (temp > outNumStaves)
            {
                outNumStaves = temp;
            }
        }
    }
};
} // namespace impl
} // namespace mx
