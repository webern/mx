// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/impl/Cursor.h"

#include <string>
#include <vector>

namespace mx
{
namespace impl
{
class MeasureCursor : public Cursor
{
  public:
    int partIndex;
    int measureIndex;

    // The id of the part's instrument, which the part's instrument changes refer to.
    std::string instrumentId;

    // The uniqueIds of all of the part's instruments, which the part's notes may name.
    std::vector<std::string> partInstrumentIds;

    MeasureCursor(int numStaves, int globalTicksPerQuarter)
        : Cursor{numStaves, globalTicksPerQuarter}, partIndex{0}, measureIndex{0}, instrumentId{}, partInstrumentIds{}
    {
    }

    MeasureCursor() : Cursor{-1, -1}, partIndex{-1}, measureIndex{-1}, instrumentId{}, partInstrumentIds{}
    {
    }
};
} // namespace impl
} // namespace mx
