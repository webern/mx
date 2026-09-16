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
class VirtualInstrument;
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
               int globalTicksPerMeasure, const core::ScorePartwise &inScore, int inDivisionsValue,
               DiagnosticsContext diagnostics = {});

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
    const core::ScorePartwise &myScore;
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
    void parseScoreInstrument(const core::ScoreInstrument &scoreInstrument) const;
    void parseVirtualInstrument(const core::VirtualInstrument &virtualInstrument) const;
    void parseMidiDeviceInstrumentGroup(const core::ScorePartMIDIGroup &grp) const;
    void parseMidiInstrument(const core::MIDIInstrument &inst) const;
    int findPartIndex(const std::string &inPartId) const;

    template <typename ELEMENT_TYPE> void updateNumStaves(const ELEMENT_TYPE &element, int &outNumStaves) const
    {
        if (element.staff().has_value())
        {
            int temp = *element.staff();
            if (temp > outNumStaves)
            {
                outNumStaves = temp;
            }
        }
    }
};
} // namespace impl
} // namespace mx
