// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/generated/Articulations.h"
#include "mx/core/generated/Notations.h"
#include "mx/core/generated/NotationsChoice.h"
#include "mx/core/generated/Ornaments.h"
#include "mx/core/generated/Technical.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MeasureCursor.h"

namespace mx
{
namespace impl
{
class ScoreWriter;

class NotationsWriter
{
  public:
    NotationsWriter(const api::NoteData &inNoteData, const MeasureCursor &inCursor, const ScoreWriter &inScoreWriter);
    core::Notations getNotations() const;

  private:
    const api::NoteData &myNoteData;
    const MeasureCursor &myCursor;
    const ScoreWriter &myScoreWriter;
    const Converter myConverter;

  private:
    core::NotationsChoice makeArticulationsNotationsChoice() const;
    core::NotationsChoice makeOrnamentsNotationsChoice() const;
    core::NotationsChoice makeTechnicalNotationsChoice() const;
    void addArticulation(const api::MarkData &markData, core::Articulations &outArticulations) const;
    void addOrnament(const api::MarkData &markData, core::Ornaments &outOrnaments) const;
    void addTechnical(const api::MarkData &markData, core::Technical &outTechnical) const;

    // Kept as their own (non-inlined) methods rather than loops inlined in getNotations(), the
    // same reason addArticulation/addOrnament/addTechnical are separate: getNotations() is
    // already a very large function, and folding more per-branch locals directly into its own
    // stack frame doesn't scale -- it overflowed the default wasm stack (issue found while
    // fixing #389's Emscripten smoke build).
    void addGlissandoAndSlide(core::Notations &outNotations) const;
    void addGlissandoStop(const api::GlissandoStop &inGlissandoStop, core::Notations &outNotations) const;
    void addWavyLineStopsAndContinues(core::Ornaments &outOrnaments) const;
    void addWavyLineStarts(core::Ornaments &outOrnaments) const;
};
} // namespace impl
} // namespace mx
