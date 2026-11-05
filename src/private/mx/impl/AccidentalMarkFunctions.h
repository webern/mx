// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

namespace mx
{
namespace core
{
class AccidentalMark;
}

namespace impl
{
class AccidentalMarkFunctions
{
  public:
    AccidentalMarkFunctions(const core::AccidentalMark &inAccidentalMark, impl::Cursor inCursor);
    ~AccidentalMarkFunctions() = default;
    AccidentalMarkFunctions(const AccidentalMarkFunctions &) = default;
    AccidentalMarkFunctions(AccidentalMarkFunctions &&) = default;
    api::MarkData parseAccidentalMark() const;

    // Reads an accidental-mark that follows an ornament inside <ornaments> as one of that
    // ornament's accidentals. An accidental value the api has no member for comes back as none.
    api::OrnamentAccidental parseOrnamentAccidental() const;

  private:
    const core::AccidentalMark &myAccidentalMark;
    const impl::Cursor myCursor;
};
} // namespace impl
} // namespace mx
