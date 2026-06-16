// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PageTextData.h"

namespace mx
{
namespace api
{
class ScoreData;
}

namespace core
{
class ScoreHeaderGroup;
}

namespace impl
{
// Reads all `<credit>` elements from the header into the score's
// pageTextItems (credit-words / no-words credits) and pageImageItems
// (credit-image).
void createCredits(const core::ScoreHeaderGroup &inHeader, api::ScoreData &outScoreData);

// Writes the score's pageTextItems and pageImageItems back out as
// `<credit>` elements on the header.
void createCredits(const api::ScoreData &inScoreData, core::ScoreHeaderGroup &outHeader);
} // namespace impl
} // namespace mx
