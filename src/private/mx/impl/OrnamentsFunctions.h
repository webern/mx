// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/api/NoteAttachmentData.h"
#include "mx/impl/Converter.h"
#include "mx/impl/DiagnosticsContext.h"
#include "mx/impl/MeasureCursor.h"

namespace mx
{
namespace core
{
class Ornaments;
class OrnamentsGroupChoice;
} // namespace core

namespace impl
{
class OrnamentsFunctions
{
  public:
    OrnamentsFunctions(const core::Ornaments &inOrnaments, MeasureCursor inCursor, DiagnosticsContext diagnostics = {});
    ~OrnamentsFunctions() = default;
    OrnamentsFunctions(const OrnamentsFunctions &) = default;
    OrnamentsFunctions(OrnamentsFunctions &&) = default;
    void parseOrnaments(api::NoteAttachmentData &outAttachments) const;

  private:
    const core::Ornaments &myOrnaments;
    const MeasureCursor myCursor;
    DiagnosticsContext myDiagnostics;

  private:
    void parseOrnamentsSet(api::NoteAttachmentData &outAttachments) const;
    void parseAccidentalMarkSet(std::vector<api::MarkData> &outMarks) const;
    void parseOrnament(const core::OrnamentsGroupChoice &choiceObj, api::MarkData &outMark) const;
};
} // namespace impl
} // namespace mx
