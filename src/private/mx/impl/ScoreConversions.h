// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/generated/ScorePartwise.h"
#include "mx/core/generated/ScoreTimewise.h"

namespace mx
{
namespace impl
{

// The timewise <-> partwise pivot: a regroup of parts-of-measures
// <-> measures-of-parts plus header. Its only consumer is DocumentManager.
// Under value semantics the old shallow copies become real copies --
// strictly safer, behavior-identical for this use.

// Converts a 'score-partwise' to 'score-timewise'.
core::ScoreTimewise partwiseTimewise(const core::ScorePartwise &inScore);

// Converts a 'score-timewise' to 'score-partwise'.
core::ScorePartwise timewisePartwise(const core::ScoreTimewise &inScore);

} // namespace impl
} // namespace mx
