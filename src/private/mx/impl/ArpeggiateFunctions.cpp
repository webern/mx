// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/ArpeggiateFunctions.h"
#include "mx/core/generated/Arpeggiate.h"
#include "mx/impl/Converter.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{

ArpeggiateFunctions::ArpeggiateFunctions(const core::Arpeggiate &inArpeggiate, impl::Cursor inCursor)
    : myArpeggiate{inArpeggiate}, myCursor{inCursor}
{
}

api::MarkData ArpeggiateFunctions::parseArpeggiate() const
{
    auto markType = api::MarkType::arpeggiate;
    if (myArpeggiate.direction().has_value())
    {
        switch (myArpeggiate.direction()->tag())
        {
        case core::UpDown::Tag::up:
            markType = api::MarkType::arpeggiateUp;
            break;

        case core::UpDown::Tag::down:
            markType = api::MarkType::arpeggiateDown;
            break;
        }
    }
    api::MarkData markData{markType};
    impl::parseMarkDataAttributes(myArpeggiate, markData);
    markData.tickTimePosition = myCursor.tickTimePosition;

    api::ArpeggiateMarkData arpeggiateData{};
    if (myArpeggiate.number().has_value())
    {
        arpeggiateData.number = myArpeggiate.number()->value();
    }
    if (myArpeggiate.unbroken().has_value())
    {
        Converter converter;
        arpeggiateData.unbroken = converter.convert(*myArpeggiate.unbroken());
    }
    arpeggiateData.id = getId(myArpeggiate);
    markData.choice = arpeggiateData;

    return markData;
}
} // namespace impl
} // namespace mx
