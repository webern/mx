// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/impl/Cursor.h"

#include <string>

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

    MeasureCursor(int numStaves, int globalTicksPerQuarter)
        : Cursor{numStaves, globalTicksPerQuarter}, partIndex{0}, measureIndex{0}, instrumentId{}
    {
    }

    MeasureCursor() : Cursor{-1, -1}, partIndex{-1}, measureIndex{-1}, instrumentId{}
    {
    }
};
} // namespace impl
} // namespace mx
