// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>

namespace mx
{
namespace api
{

// A spanner's number identity, shared by curves (slur/tied), wedges, and the
// generic spanner start/stop structs. Exactly one of three states; a
// contradictory state (both an explicit level and an identity) is
// unrepresentable by construction.
//
//   - unspecified   : no number information. The writer applies its default
//                     (omit the attribute for a lone spanner).
//   - explicit(n)   : a literal MusicXML number, 1..16. Emitted verbatim. Set by
//                     the reader to preserve an imported number, or by a caller
//                     who wants to control the number themselves.
//   - identity(id)  : an author-only, NEVER-serialized label. Start/continue/stop
//                     that share the same id are the same logical spanner; the
//                     writer assigns the MusicXML number from serialization
//                     order. The reader never produces this state.
//
// Follows the TimeChoice pattern: misuse never throws. A level outside [1, 16]
// or an empty id collapses to unspecified at construction, and calling
// level()/identity() for the wrong kind returns a harmless default (the
// NUMBER_LEVEL_UNSPECIFIED sentinel, or an empty string) instead of throwing.
// Check kind() (or isExplicit()/isIdentity()) before reading a payload.
class SpannerNumber
{
  public:
    enum class Kind
    {
        unspecified,
        explicitLevel,
        identity
    };

    // Default is unspecified.
    SpannerNumber() = default;

    // An explicit MusicXML number. A level outside [1, 16] collapses to
    // unspecified.
    SpannerNumber(int level);

    // An author identity label. An empty id collapses to unspecified (which is
    // already mx::api's "no identity" state).
    SpannerNumber(std::string identity);

    Kind kind() const;
    bool isUnspecified() const;
    bool isExplicit() const;
    bool isIdentity() const;

    // Returns the literal number 1..16, or NUMBER_LEVEL_UNSPECIFIED (-1) if this
    // is not an explicit level.
    int level() const;

    // Returns a copy of the id, or an empty string if this is not an identity.
    std::string identity() const;

    bool operator==(const SpannerNumber &other) const;

  private:
    Kind myKind = Kind::unspecified;
    int myLevel = 0;
    std::string myIdentity;
};

MXAPI_NOT_EQUALS_AND_VECTORS(SpannerNumber);

} // namespace api
} // namespace mx
