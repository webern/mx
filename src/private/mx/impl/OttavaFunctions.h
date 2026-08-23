// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/OttavaData.h"

namespace mx
{
namespace impl
{
// The octave-shift size an ottava line encodes: 8 for 8va/8vb, 15 for 15ma/15mb, 22 for 22ma/22mb.
// An unspecified type falls back to MusicXML's default size of 8. This is the single source of the
// size fact -- the start writes it from its own OttavaType and the matching stop borrows it.
inline int ottavaTypeSize(api::OttavaType inOttavaType)
{
    switch (inOttavaType)
    {
    case api::OttavaType::o15ma:
    case api::OttavaType::o15mb:
        return 15;

    case api::OttavaType::o22ma:
    case api::OttavaType::o22mb:
        return 22;

    default:
        return 8;
    }
}
} // namespace impl
} // namespace mx
