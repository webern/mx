// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Result.h"

#include <string>

namespace mx
{
namespace api
{
void appendFormattedLocationAndMessage(std::string &text, const Location &location, const std::string &message);
} // namespace api
} // namespace mx
