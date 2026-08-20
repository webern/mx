// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Id.h"
#include "mx/core/Token.h"

namespace mx
{
namespace api
{

// Reaches the core::Token that an api::Id holds. mx::impl uses this to carry an id between mx::api
// and mx::core as a token, so the text is scrubbed when the Id is built and never again. This
// header is private to mx and is not part of the public api.
struct IdAccess
{
    // The token an Id holds. Pass it straight to a core element's setID.
    static const core::Token &token(const Id &inId);

    // An Id holding a token that mx::core already read from a file.
    static Id make(core::Token inToken);
};

} // namespace api
} // namespace mx
