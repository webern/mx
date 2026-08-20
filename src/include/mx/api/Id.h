// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

namespace mx
{
namespace api
{

// The id attribute of a MusicXML element. An id is a name that identifies one element within the
// document. Software uses it to point at a particular note, measure, or marking -- to line playback
// up with the score, to hang an annotation on a note, or to link one file to another.
//
// An id must follow the XML name rules. The first character is a letter or an underscore. The rest
// are letters, digits, dots, hyphens, or underscores. Building an Id scrubs out anything else, so
// an id that breaks the rules cannot exist. Text with nothing usable left, empty text included,
// becomes "X". Building the Id is the only place this happens: mx writes the id exactly as the Id
// holds it.
//
// Scrubbing is silent. To find out whether your text was already a legal id, compare it with the
// id you built:
//
//     const auto id = Id{myText};
//     if (id.value() != myText)
//     {
//         // myText was not a legal id; decide what to do about it
//     }
//
// An id must also be unique within the document. Uniqueness is a property of the whole score, not
// of one name, so Id cannot check it. An id you invent must not collide with one already in the
// score.
//
// To write no id attribute at all, leave the std::optional that holds the Id empty.
//
// Copy, assign, compare, sort, and hash an Id the way you would any other value. Two Ids are equal
// when their text is equal, and std::hash is specialized for Id, so an Id works as a key in both
// std::map and std::unordered_map.
class Id
{
  public:
    Id(std::string text);
    Id(const char *text);

    Id(const Id &other);
    Id(Id &&other) noexcept;
    Id &operator=(const Id &other);
    Id &operator=(Id &&other) noexcept;
    ~Id();

    const std::string &value() const;

    bool operator==(const Id &other) const;
    bool operator<(const Id &other) const;

  private:
    // Holds the same type mx writes the attribute with, which is why the text cannot be scrubbed a
    // second time. The definition is private to mx.
    class Impl;
    explicit Id(std::shared_ptr<const Impl> impl);
    friend struct IdAccess;

    std::shared_ptr<const Impl> myImpl;
};

MXAPI_NOT_EQUALS_AND_VECTORS(Id);

inline bool operator>(const Id &lhs, const Id &rhs)
{
    return rhs < lhs;
}

inline bool operator<=(const Id &lhs, const Id &rhs)
{
    return !(rhs < lhs);
}

inline bool operator>=(const Id &lhs, const Id &rhs)
{
    return !(lhs < rhs);
}

} // namespace api
} // namespace mx

namespace std
{
template <> struct hash<mx::api::Id>
{
    std::size_t operator()(const mx::api::Id &id) const noexcept
    {
        return std::hash<std::string>{}(id.value());
    }
};
} // namespace std
