// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DurationData.h"
#include "mx/api/TupletData.h"
#include "mx/impl/DiagnosticsContext.h"
#include "mx/impl/MeasureCursor.h"

#include <vector>

namespace mx
{
namespace core
{
class Tuplet;
class Note;
} // namespace core

namespace impl
{
class TupletReader
{
  public:
    TupletReader(const core::Tuplet &inMxTuplet, MeasureCursor inCursor, const core::Note &inNote,
                 DiagnosticsContext diagnostics = {});
    void parseTuplet(std::vector<api::TupletStart> &outTupletStarts, std::vector<api::TupletStop> &outTupletStops);

  private:
    const core::Tuplet &myTuplet;
    const core::Note &myNote;
    const MeasureCursor myCursor;
    DiagnosticsContext myDiagnostics;

  private:
    void guessNormalFromNote(api::TupletStart &outTupletStart);
    void guessActualFromNote(api::TupletStart &outTupletStart);
    void guessFigureFromNote(api::DurationName &outDurationName, int &outDots) const;
    void checkDurationAgainstGuess() const;
    static double quartersOf(api::DurationName inDurationName, int inDots);
};
} // namespace impl
} // namespace mx
