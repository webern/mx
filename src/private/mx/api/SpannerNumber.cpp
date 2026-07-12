// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/SpannerNumber.h"

#include <utility>

namespace mx
{
namespace api
{

SpannerNumber::SpannerNumber(int level)
{
    if (level >= 1 && level <= 16)
    {
        myKind = Kind::explicitLevel;
        myLevel = level;
    }
}

SpannerNumber::SpannerNumber(std::string identity)
{
    if (!identity.empty())
    {
        myKind = Kind::identity;
        myIdentity = std::move(identity);
    }
}

SpannerNumber::Kind SpannerNumber::kind() const
{
    return myKind;
}

bool SpannerNumber::isUnspecified() const
{
    return myKind == Kind::unspecified;
}

bool SpannerNumber::isExplicit() const
{
    return myKind == Kind::explicitLevel;
}

bool SpannerNumber::isIdentity() const
{
    return myKind == Kind::identity;
}

int SpannerNumber::level() const
{
    return myKind == Kind::explicitLevel ? myLevel : NUMBER_LEVEL_UNSPECIFIED;
}

std::string SpannerNumber::identity() const
{
    return myKind == Kind::identity ? myIdentity : std::string{};
}

bool SpannerNumber::operator==(const SpannerNumber &other) const
{
    if (myKind != other.myKind)
    {
        return false;
    }
    switch (myKind)
    {
    case Kind::explicitLevel:
        return myLevel == other.myLevel;
    case Kind::identity:
        return myIdentity == other.myIdentity;
    case Kind::unspecified:
    default:
        return true;
    }
}

} // namespace api
} // namespace mx
