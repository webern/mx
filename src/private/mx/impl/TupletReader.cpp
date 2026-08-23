// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/TupletReader.h"
#include "mx/core/generated/Note.h"
#include "mx/core/generated/TimeModification.h"
#include "mx/core/generated/TimeModificationGroup.h"
#include "mx/core/generated/Tuplet.h"
#include "mx/core/generated/TupletPortion.h"
#include "mx/impl/Converter.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{
TupletReader::TupletReader(const core::Tuplet &inMxTuplet, impl::Cursor inCursor, const core::Note &inNote)
    : myTuplet{inMxTuplet}, myNote{inNote}, myCursor{inCursor}
{
}

void TupletReader::parseTuplet(std::vector<api::TupletStart> &outTupletStarts,
                               std::vector<api::TupletStop> &outTupletStops)
{
    api::TupletStart tupletStart;
    tupletStart.positionData = impl::getPositionData(myTuplet);
    tupletStart.id = getId(myTuplet);

    if (myTuplet.number().has_value())
    {
        tupletStart.numberLevel = myTuplet.number()->value();
    }

    if (myTuplet.type() == core::StartStop::stop())
    {
        api::TupletStop tupletStop;
        tupletStop.positionData = tupletStart.positionData;
        tupletStop.numberLevel = tupletStart.numberLevel;
        tupletStop.id = getId(myTuplet);
        outTupletStops.emplace_back(std::move(tupletStop));
        return;
    }

    if (myTuplet.showNumber().has_value())
    {
        switch (myTuplet.showNumber()->tag())
        {
        case core::ShowTuplet::Tag::none:
            tupletStart.showActualNumber = api::Bool::no;
            tupletStart.showNormalNumber = api::Bool::no;
            break;
        case core::ShowTuplet::Tag::both:
            tupletStart.showActualNumber = api::Bool::yes;
            tupletStart.showNormalNumber = api::Bool::yes;
            break;
        case core::ShowTuplet::Tag::actual:
            tupletStart.showActualNumber = api::Bool::yes;
            tupletStart.showNormalNumber = api::Bool::no;
            break;
        default:
            break;
        }
    }

    Converter converter;

    if (myTuplet.bracket().has_value())
    {
        tupletStart.bracket = converter.convert(*myTuplet.bracket());
    }

    if (myTuplet.tupletActual().has_value())
    {
        const auto &actual = *myTuplet.tupletActual();

        if (actual.tupletNumber().has_value())
        {
            tupletStart.actualNumber = actual.tupletNumber()->value();
        }

        if (actual.tupletType().has_value())
        {
            tupletStart.actualDurationName = converter.convert(actual.tupletType()->value());
        }

        tupletStart.actualDots = static_cast<int>(actual.tupletDot().size());
    }
    else
    {
        guessActualFromNote(tupletStart);
    }

    if (myTuplet.tupletNormal().has_value())
    {
        const auto &normal = *myTuplet.tupletNormal();

        if (normal.tupletNumber().has_value())
        {
            tupletStart.normalNumber = normal.tupletNumber()->value();
        }

        if (normal.tupletType().has_value())
        {
            tupletStart.normalDurationName = converter.convert(normal.tupletType()->value());
        }

        tupletStart.normalDots = static_cast<int>(normal.tupletDot().size());
    }
    else
    {
        guessNormalFromNote(tupletStart);
    }

    outTupletStarts.emplace_back(std::move(tupletStart));
}

void TupletReader::guessNormalFromNote(api::TupletStart &outTupletStart)
{
    if (!myNote.timeModification().has_value())
    {
        return;
    }
    const auto &timeMod = *myNote.timeModification();
    outTupletStart.actualNumber = timeMod.actualNotes();

    Converter converter;
    if (timeMod.group().has_value())
    {
        const auto &grp = *timeMod.group();
        outTupletStart.normalDurationName = converter.convert(grp.normalType());
        outTupletStart.normalDots = static_cast<int>(grp.normalDot().size());
    }
    else if (myNote.type().has_value())
    {
        outTupletStart.normalDurationName = converter.convert(myNote.type()->value());
    }
}

void TupletReader::guessActualFromNote(api::TupletStart &outTupletStart)
{
    if (!myNote.timeModification().has_value())
    {
        return;
    }
    const auto &timeMod = *myNote.timeModification();
    outTupletStart.normalNumber = timeMod.normalNotes();

    Converter converter;
    if (timeMod.group().has_value())
    {
        const auto &grp = *timeMod.group();
        outTupletStart.actualDurationName = converter.convert(grp.normalType());
        outTupletStart.actualDots = static_cast<int>(grp.normalDot().size());
    }
    else if (myNote.type().has_value())
    {
        outTupletStart.actualDurationName = converter.convert(myNote.type()->value());
    }
}

} // namespace impl
} // namespace mx
