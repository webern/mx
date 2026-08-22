// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NonArpeggiateFunctions.h"
#include "mx/core/generated/NonArpeggiate.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{

NonArpeggiateFunctions::NonArpeggiateFunctions(const core::NonArpeggiate &inNonArpeggiate, impl::Cursor inCursor)
    : myNonArpeggiate{inNonArpeggiate}, myCursor{inCursor}
{
}

api::MarkData NonArpeggiateFunctions::parseNonArpeggiate() const
{
    api::MarkData markData{api::MarkType::nonArpeggiate};
    impl::parseMarkDataAttributes(myNonArpeggiate, markData);
    markData.tickTimePosition = myCursor.tickTimePosition;

    api::NonArpeggiateMarkData nonArpeggiateData{};
    nonArpeggiateData.placement = myNonArpeggiate.type().tag() == core::TopBottom::Tag::bottom
                                      ? api::NonArpeggiatePlacement::bottom
                                      : api::NonArpeggiatePlacement::top;
    if (myNonArpeggiate.number().has_value())
    {
        nonArpeggiateData.number = myNonArpeggiate.number()->value();
    }
    nonArpeggiateData.id = getId(myNonArpeggiate);
    markData.choice = nonArpeggiateData;

    return markData;
}
} // namespace impl
} // namespace mx
