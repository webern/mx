// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

namespace mx
{
namespace api
{
// Whether an instrument's playback sound is a single player or a section.
enum class SoloOrEnsemble
{
    unspecified,
    solo,
    ensemble
};
} // namespace api
} // namespace mx
