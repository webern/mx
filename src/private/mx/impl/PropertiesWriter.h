// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ClefData.h"
#include "mx/api/KeyData.h"
#include "mx/api/PartSymbolData.h"
#include "mx/api/TimeSignatureData.h"
#include "mx/api/TransposeData.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Key.h"
#include "mx/core/generated/PartwiseMeasure.h"

namespace mx
{
namespace impl
{

/// Provides a way to ensure that properties (i.e. things in the `<attributes>` element) are
/// added to a single <attributes> element instead of creating a new <attributes> element
/// for each property.
///
/// The design is weird. It holds the current measure that we are writing, and acts like a
/// buffer of properties. You can write things like clefs and time signatures to the
/// PropertiesWriter, and then when you call `flushBuffer()` everything gets written to the
/// measure.
class PropertiesWriter
{
  public:
    explicit PropertiesWriter(core::PartwiseMeasure &inPartwiseMeasure);

    // destroy and reallocate a new properties element
    // without inserting it into the measure
    void clearBuffer();

    // if the properties element has any data it will
    // be added to the measure before being cleared
    // and reallocated.  if the properties element
    // does not have any data then nothing happens.
    void flushBuffer();

    // returns true if the properties element does
    // not contain any information
    bool isPropertiesEmpty();

    void writeDivisions(int value);
    void writeKey(int staffIndex, const api::KeyData &inKeyData);
    static void writeTraditionalKey(const api::KeyData &inKeyData, core::Key &ioKey);
    static void writeNonTraditionalKey(const api::KeyData &inKeyData, core::Key &ioKey);
    void writeTime(const api::TimeSignatureData &value);
    void writeNumStaves(int value);
    void writeStaffDetails(int staffIndex, int staffLines);
    void writeStaffDetails(int staffIndex, int staffLines, double staffSize);
    void writeStaffDetails(int staffIndex, int staffLines, double staffSize, double staffScaling);
    void writeClef(int staffIndex, const api::ClefData &inClefData);
    void writePartSymbol(const api::PartSymbolData &inPartSymbolData);
    void writeTranspose(const api::TransposeData &inTransposeData);

  private:
    void allocate();

  private:
    core::Attributes myAttributes;
    bool myHasContent;
    core::PartwiseMeasure &myPartwiseMeasure;
};

} // namespace impl
} // namespace mx
