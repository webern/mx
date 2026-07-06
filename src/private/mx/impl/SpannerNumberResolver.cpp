// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/SpannerNumberResolver.h"
#include "mx/utility/Throw.h"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace mx
{
namespace impl
{
// Spanner element families that draw numbers from separate pools. CurveType
// distinguishes slur from tied; the SpannerStart/Stop structs serve several
// elements (octave-shift, bracket, dashes) whose number attributes are
// independent of each other in MusicXML, so each gets its own pool.
enum class SpannerNumberClass
{
    slur,
    tie,
    wedge,
    octaveShift,
    bracket,
    dashes
};

// One start/continue/stop occurrence at its position in the serialized stream.
struct SpannerNumberEvent
{
    int position;
    int staffIndex;
    const void *object;
    const api::SpannerNumber *number;
    bool opens;  // a start
    bool closes; // a stop
};

// [first, last] positions (inclusive) during which a spanner is open in the
// serialized stream.
struct SpannerNumberInterval
{
    int first;
    int last;
};

inline bool spannerNumberIntervalsOverlap(const SpannerNumberInterval &inLeft, const SpannerNumberInterval &inRight)
{
    return inLeft.first <= inRight.last && inRight.first <= inLeft.last;
}

// Collects spanner events in the exact order MeasureWriter serializes them.
// The position counter is global; only events within the same class (and
// staff pool) are ever compared, so cross-class interleaving is irrelevant,
// but within a class the order matches what a streaming reader sees.
class SpannerNumberEventCollector
{
  public:
    void addNote(int inStaffIndex, const api::NoteData &inNote)
    {
        // NotationsWriter emits curve stops, then continues, then starts, and
        // skips curves whose type is neither tie nor slur.
        const auto &attachments = inNote.noteAttachmentData;
        for (const auto &stop : attachments.curveStops)
        {
            addCurve(inStaffIndex, stop.curveType, &stop, stop.number, false, true);
        }
        for (const auto &curveContinue : attachments.curveContinuations)
        {
            addCurve(inStaffIndex, curveContinue.curveType, &curveContinue, curveContinue.number, false, false);
        }
        for (const auto &start : attachments.curveStarts)
        {
            addCurve(inStaffIndex, start.curveType, &start, start.number, true, false);
        }
    }

    void addDirection(int inStaffIndex, const api::DirectionData &inDirection)
    {
        if (inDirection.orderedComponents.empty())
        {
            addDirectionFixedOrder(inStaffIndex, inDirection);
        }
        else
        {
            addDirectionOrderedComponents(inStaffIndex, inDirection);
        }
    }

    const std::map<SpannerNumberClass, std::vector<SpannerNumberEvent>> &events() const
    {
        return myEvents;
    }

  private:
    void add(SpannerNumberClass inClass, int inStaffIndex, const void *inObject, const api::SpannerNumber &inNumber,
             bool inOpens, bool inCloses)
    {
        myEvents[inClass].push_back(
            SpannerNumberEvent{myPosition, inStaffIndex, inObject, &inNumber, inOpens, inCloses});
        ++myPosition;
    }

    void addCurve(int inStaffIndex, api::CurveType inCurveType, const void *inObject,
                  const api::SpannerNumber &inNumber, bool inOpens, bool inCloses)
    {
        if (inCurveType == api::CurveType::slur)
        {
            add(SpannerNumberClass::slur, inStaffIndex, inObject, inNumber, inOpens, inCloses);
        }
        else if (inCurveType == api::CurveType::tie)
        {
            add(SpannerNumberClass::tie, inStaffIndex, inObject, inNumber, inOpens, inCloses);
        }
    }

    // Mirrors DirectionWriter::emitFixedOrder. Pedals are skipped: <pedal> has
    // no number attribute.
    void addDirectionFixedOrder(int inStaffIndex, const api::DirectionData &inDirection)
    {
        for (const auto &item : inDirection.wedgeStops)
        {
            add(SpannerNumberClass::wedge, inStaffIndex, &item, item.number, false, true);
        }
        for (const auto &item : inDirection.wedgeStarts)
        {
            add(SpannerNumberClass::wedge, inStaffIndex, &item, item.number, true, false);
        }
        for (const auto &item : inDirection.ottavaStops)
        {
            add(SpannerNumberClass::octaveShift, inStaffIndex, &item, item.number, false, true);
        }
        for (const auto &item : inDirection.ottavaStarts)
        {
            add(SpannerNumberClass::octaveShift, inStaffIndex, &item.spannerStart, item.spannerStart.number, true,
                false);
        }
        for (const auto &item : inDirection.bracketStarts)
        {
            add(SpannerNumberClass::bracket, inStaffIndex, &item, item.number, true, false);
        }
        for (const auto &item : inDirection.bracketStops)
        {
            add(SpannerNumberClass::bracket, inStaffIndex, &item, item.number, false, true);
        }
        for (const auto &item : inDirection.dashesStarts)
        {
            add(SpannerNumberClass::dashes, inStaffIndex, &item, item.number, true, false);
        }
        for (const auto &item : inDirection.dashesStops)
        {
            add(SpannerNumberClass::dashes, inStaffIndex, &item, item.number, false, true);
        }
    }

    // Mirrors DirectionWriter::emitOrderedComponents (same bounds checks).
    void addDirectionOrderedComponents(int inStaffIndex, const api::DirectionData &inDirection)
    {
        for (const auto &component : inDirection.orderedComponents)
        {
            const int i = component.index;
            if (i < 0)
            {
                continue;
            }
            const auto index = static_cast<std::size_t>(i);

            switch (component.kind)
            {
            case api::DirectionComponentKind::wedgeStop:
                if (index < inDirection.wedgeStops.size())
                {
                    const auto &item = inDirection.wedgeStops.at(index);
                    add(SpannerNumberClass::wedge, inStaffIndex, &item, item.number, false, true);
                }
                break;

            case api::DirectionComponentKind::wedgeStart:
                if (index < inDirection.wedgeStarts.size())
                {
                    const auto &item = inDirection.wedgeStarts.at(index);
                    add(SpannerNumberClass::wedge, inStaffIndex, &item, item.number, true, false);
                }
                break;

            case api::DirectionComponentKind::ottavaStop:
                if (index < inDirection.ottavaStops.size())
                {
                    const auto &item = inDirection.ottavaStops.at(index);
                    add(SpannerNumberClass::octaveShift, inStaffIndex, &item, item.number, false, true);
                }
                break;

            case api::DirectionComponentKind::ottavaStart:
                if (index < inDirection.ottavaStarts.size())
                {
                    const auto &item = inDirection.ottavaStarts.at(index);
                    add(SpannerNumberClass::octaveShift, inStaffIndex, &item.spannerStart, item.spannerStart.number,
                        true, false);
                }
                break;

            case api::DirectionComponentKind::bracketStart:
                if (index < inDirection.bracketStarts.size())
                {
                    const auto &item = inDirection.bracketStarts.at(index);
                    add(SpannerNumberClass::bracket, inStaffIndex, &item, item.number, true, false);
                }
                break;

            case api::DirectionComponentKind::bracketStop:
                if (index < inDirection.bracketStops.size())
                {
                    const auto &item = inDirection.bracketStops.at(index);
                    add(SpannerNumberClass::bracket, inStaffIndex, &item, item.number, false, true);
                }
                break;

            case api::DirectionComponentKind::dashesStart:
                if (index < inDirection.dashesStarts.size())
                {
                    const auto &item = inDirection.dashesStarts.at(index);
                    add(SpannerNumberClass::dashes, inStaffIndex, &item, item.number, true, false);
                }
                break;

            case api::DirectionComponentKind::dashesStop:
                if (index < inDirection.dashesStops.size())
                {
                    const auto &item = inDirection.dashesStops.at(index);
                    add(SpannerNumberClass::dashes, inStaffIndex, &item, item.number, false, true);
                }
                break;

            default:
                break;
            }
        }
    }

    int myPosition = 0;
    std::map<SpannerNumberClass, std::vector<SpannerNumberEvent>> myEvents;
};

// Assigns numbers within one spanner class. Explicit levels reserve their
// number for the serialized extent of their start/stop pair; identity groups
// then take the lowest number whose reservations and prior assignments do not
// overlap the group's own extent, per staff pool.
static void spannerNumberAssignClass(const std::vector<SpannerNumberEvent> &inEvents,
                                     std::unordered_map<const void *, int> &ioResolved)
{
    constexpr int kSpannerNumberMax = 16;

    // occupied[staffIndex][number] -> intervals during which that number is taken
    std::map<int, std::map<int, std::vector<SpannerNumberInterval>>> occupied;

    const auto reserve = [&occupied](int inNumber, const SpannerNumberInterval &inInterval, int inStaffA,
                                     int inStaffB) {
        occupied[inStaffA][inNumber].push_back(inInterval);
        if (inStaffB != inStaffA)
        {
            occupied[inStaffB][inNumber].push_back(inInterval);
        }
    };

    // Match explicit start/stop pairs by level in stream order and reserve
    // their intervals. A pair that spans staves reserves in both pools.
    std::map<int, std::vector<const SpannerNumberEvent *>> openExplicit;
    for (const auto &event : inEvents)
    {
        if (!event.number->isExplicit())
        {
            continue;
        }
        const int level = event.number->level();
        if (event.opens)
        {
            openExplicit[level].push_back(&event);
        }
        else if (event.closes)
        {
            auto &stack = openExplicit[level];
            if (!stack.empty())
            {
                const auto *start = stack.back();
                stack.pop_back();
                reserve(level, SpannerNumberInterval{start->position, event.position}, start->staffIndex,
                        event.staffIndex);
            }
            else
            {
                // a stop with no visible start (authoring error); reserve its
                // own position so at least the stop itself cannot collide
                reserve(level, SpannerNumberInterval{event.position, event.position}, event.staffIndex,
                        event.staffIndex);
            }
        }
        else
        {
            // a continue between its start and stop is already covered by the
            // pair's interval; this point reservation only matters when the
            // continue dangles
            reserve(level, SpannerNumberInterval{event.position, event.position}, event.staffIndex, event.staffIndex);
        }
    }
    for (const auto &levelAndStack : openExplicit)
    {
        for (const auto *start : levelAndStack.second)
        {
            // a start with no stop stays open to the end of the part
            reserve(levelAndStack.first, SpannerNumberInterval{start->position, std::numeric_limits<int>::max()},
                    start->staffIndex, start->staffIndex);
        }
    }

    // Group identity events: same id (within this class and part) is the same
    // logical spanner. Groups are assigned in order of first appearance.
    struct SpannerNumberGroup
    {
        SpannerNumberInterval interval;
        std::vector<const void *> objects;
        std::set<int> staves;
    };

    std::vector<SpannerNumberGroup> groups;
    std::map<std::string, std::size_t> groupIndexByIdentity;
    for (const auto &event : inEvents)
    {
        if (!event.number->isIdentity())
        {
            continue;
        }
        const auto found = groupIndexByIdentity.emplace(event.number->identity(), groups.size());
        if (found.second)
        {
            groups.push_back(SpannerNumberGroup{SpannerNumberInterval{event.position, event.position}, {}, {}});
        }
        auto &group = groups.at(found.first->second);
        group.interval.first = std::min(group.interval.first, event.position);
        group.interval.last = std::max(group.interval.last, event.position);
        group.objects.push_back(event.object);
        group.staves.insert(event.staffIndex);
    }

    for (const auto &group : groups)
    {
        int chosen = 0;
        for (int candidate = 1; candidate <= kSpannerNumberMax && chosen == 0; ++candidate)
        {
            bool isFree = true;
            for (const int staffIndex : group.staves)
            {
                const auto staffIter = occupied.find(staffIndex);
                if (staffIter == occupied.cend())
                {
                    continue;
                }
                const auto numberIter = staffIter->second.find(candidate);
                if (numberIter == staffIter->second.cend())
                {
                    continue;
                }
                for (const auto &interval : numberIter->second)
                {
                    if (spannerNumberIntervalsOverlap(interval, group.interval))
                    {
                        isFree = false;
                        break;
                    }
                }
                if (!isFree)
                {
                    break;
                }
            }
            if (isFree)
            {
                chosen = candidate;
            }
        }
        if (chosen == 0)
        {
            MX_THROW("more than 16 spanners of one type are open at the same point in the serialized "
                     "stream; MusicXML number attributes only range from 1 to 16");
        }
        for (const int staffIndex : group.staves)
        {
            occupied[staffIndex][chosen].push_back(group.interval);
        }
        for (const void *object : group.objects)
        {
            ioResolved[object] = chosen;
        }
    }
}

void SpannerNumberResolver::resolvePart(const api::PartData &inPart)
{
    SpannerNumberEventCollector collector;

    for (const auto &measure : inPart.measures)
    {
        int staffIndex = 0;
        for (const auto &staff : measure.staves)
        {
            for (const auto &voicePair : staff.voices)
            {
                for (const auto &note : voicePair.second.notes)
                {
                    collector.addNote(staffIndex, note);
                }
            }
            for (const auto &direction : staff.directions)
            {
                collector.addDirection(staffIndex, direction);
            }
            ++staffIndex;
        }
    }

    for (const auto &classAndEvents : collector.events())
    {
        spannerNumberAssignClass(classAndEvents.second, myResolved);
    }
}

std::optional<int> SpannerNumberResolver::emittedNumber(const api::SpannerNumber &inNumber, const void *inObject) const
{
    switch (inNumber.kind())
    {
    case api::SpannerNumber::Kind::unspecified:
        return std::nullopt;
    case api::SpannerNumber::Kind::explicitLevel:
        return inNumber.level();
    case api::SpannerNumber::Kind::identity: {
        const auto iter = myResolved.find(inObject);
        if (iter == myResolved.cend())
        {
            MX_THROW("identity spanner was not visited by resolvePart; the writer would emit an "
                     "unnumbered spanner that could collide with another");
        }
        return iter->second;
    }
    default:
        return std::nullopt;
    }
}
} // namespace impl
} // namespace mx
