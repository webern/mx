// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/impl/Cursor.h"

namespace mx
{
namespace core
{
class Dynamics;
} // namespace core

namespace impl
{

class DynamicsReader
{
  public:
    DynamicsReader(const core::Dynamics &inDynamic, impl::Cursor inCursor);
    ~DynamicsReader() = default;
    DynamicsReader(const DynamicsReader &) = delete;
    DynamicsReader(DynamicsReader &&) = delete;
    DynamicsReader &operator=(const DynamicsReader &) = delete;
    DynamicsReader &operator=(DynamicsReader &&) = delete;

    void parseDynamics(std::vector<api::MarkData> &outMarks) const;

  private:
    const core::Dynamics &myDynamic;
    const impl::Cursor myCursor;
};
} // namespace impl
} // namespace mx
