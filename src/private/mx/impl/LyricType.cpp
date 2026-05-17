// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/LyricType.h"

namespace mx
{
namespace impl
{
LyricType::LyricType() : myText{}, mySyllabic(core::SyllabicEnum::single)
{
    (void)myText;
    (void)mySyllabic;
}

LyricType::LyricType(const std::string &value, core::SyllabicEnum syllabic) : myText{value}, mySyllabic{syllabic}
{
    (void)myText;
    (void)mySyllabic;
}
} // namespace impl
} // namespace mx
