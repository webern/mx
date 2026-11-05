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
class OrnamentsGroup;
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
    void parseOrnament(const core::OrnamentsGroupChoice &choiceObj, api::MarkData &outMark) const;
    api::OrnamentMarkData parseOwnedAccidentals(const core::OrnamentsGroup &group) const;
    void parseStandaloneAccidentals(const core::OrnamentsGroup &group, std::vector<api::MarkData> &outMarks) const;
};
} // namespace impl
} // namespace mx
