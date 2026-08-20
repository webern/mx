// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/Id.h"

#include "mx/api/IdAccess.h"

#include <utility>

namespace mx
{
namespace api
{

// core::Token is the type mx writes an id attribute with. It cannot hold text that breaks the XML
// name rules, so an Id that holds one is valid for as long as it exists, and mx::impl hands the
// token itself to mx::core rather than converting the text again.
//
// An Id never changes its token, so copies share one Impl instead of allocating another.
class Id::Impl
{
  public:
    explicit Impl(core::Token inToken) : token{std::move(inToken)}
    {
    }

    // What a moved-from Id is left holding. An Id always holds an Impl, so reading one that has
    // been moved from is harmless rather than undefined.
    static const std::shared_ptr<const Impl> &movedFrom();

    core::Token token;
};

const std::shared_ptr<const Id::Impl> &Id::Impl::movedFrom()
{
    static const std::shared_ptr<const Impl> value = std::make_shared<const Impl>(core::Token{});
    return value;
}

Id::Id(std::string text) : myImpl{std::make_shared<const Impl>(core::Token{std::move(text)})}
{
}

Id::Id(const char *text) : Id{std::string{text != nullptr ? text : ""}}
{
}

Id::Id(std::shared_ptr<const Impl> impl) : myImpl{std::move(impl)}
{
}

Id::Id(const Id &other) = default;

Id::Id(Id &&other) noexcept : myImpl{std::move(other.myImpl)}
{
    other.myImpl = Impl::movedFrom();
}

Id &Id::operator=(const Id &other) = default;

Id &Id::operator=(Id &&other) noexcept
{
    if (this != &other)
    {
        myImpl = std::move(other.myImpl);
        other.myImpl = Impl::movedFrom();
    }
    return *this;
}

Id::~Id() = default;

const std::string &Id::value() const
{
    return myImpl->token.value();
}

bool Id::operator==(const Id &other) const
{
    return myImpl == other.myImpl || myImpl->token == other.myImpl->token;
}

bool Id::operator<(const Id &other) const
{
    return myImpl->token.value() < other.myImpl->token.value();
}

const core::Token &IdAccess::token(const Id &inId)
{
    return inId.myImpl->token;
}

Id IdAccess::make(core::Token inToken)
{
    return Id{std::make_shared<const Id::Impl>(std::move(inToken))};
}

} // namespace api
} // namespace mx
