// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Id.h"
#include "mx/api/IdAccess.h"
#include "mx/impl/DiagnosticsContext.h"

#include <optional>

namespace mx
{
namespace impl
{
// Read the id attribute from any core element that has one. Absent stays absent. The token moves
// across as a token, so the text is not scrubbed again.
template <typename CORE_TYPE> std::optional<api::Id> getId(const CORE_TYPE &inCoreElement)
{
    if (!inCoreElement.id().has_value())
    {
        return std::nullopt;
    }

    return api::IdAccess::make(*inCoreElement.id());
}

// Write the id attribute onto any core element that has one. An absent id writes no attribute. The
// element receives the token the Id already holds, so the text is not scrubbed again; an Id whose text
// was scrubbed when it was built is reported at location.
template <typename CORE_TYPE>
void setId(const std::optional<api::Id> &inId, CORE_TYPE &outCoreElement, const DiagnosticsContext &diagnostics,
           const api::Location &location)
{
    if (inId.has_value())
    {
        const auto &scrubbedText = api::IdAccess::scrubbedText(*inId);
        if (scrubbedText.has_value())
        {
            diagnostics.report(api::Severity::warning, api::DiagnosticCode::invalidValue, location,
                               "id \"" + *scrubbedText + "\" is not a valid id; using \"" + inId->value() + "\"");
        }
        outCoreElement.setID(api::IdAccess::token(*inId));
    }
}
} // namespace impl
} // namespace mx
