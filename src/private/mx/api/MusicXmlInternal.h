// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MusicXml.h"

namespace mx
{
namespace core
{
class Document;
} // namespace core

namespace api
{
// Internal to mx: the core document owned by a MusicXml. The public api does
// not expose the core model; mx's own layers and its tests reach it through
// here.
const core::Document &coreDocumentOf(const MusicXml &document) noexcept;
} // namespace api
} // namespace mx
