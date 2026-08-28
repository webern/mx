// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/SpannerResolver.h"
#include "mx/api/GlissandoData.h"
#include "mx/impl/OttavaFunctions.h"
#include "mx/utility/Throw.h"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <map>
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
// GlissandoType similarly distinguishes <glissando> from <slide>, two distinct
// elements with independent number attributes.
enum class SpannerNumberClass
{
    slur,
    tie,
    wedge,
    octaveShift,
    bracket,
    dashes,
    glissando,
    slide,
    wavyLine
};

// One start/continue/stop occurrence at its position in the serialized stream. The number is
// held by value: direction spanners are read out of DirectionChoice by copy, so a pointer into
// the api data is not available for them.
struct SpannerNumberEvent
{
    int position;
    const void *object;
    api::SpannerNumber number;
    bool opens;  // a start
    bool closes; // a stop

    // The note the event sits on, used to detect spanners that start and stop
    // on one note. Direction events do not belong to a note and carry nullptr.
    const void *noteTag;
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

// One octave-shift start or stop, in serialized order. Starts carry the OttavaType that states
// the line's size; stops carry the address the writer will present when it asks for that size.
struct SpannerOttavaEvent
{
    const void *object;
    api::SpannerNumber number;
    api::OttavaType ottavaType;
    bool isStart;
};

// The bucket an endpoint pairs within, used for ottava sizes and for same-note span detection:
// identity endpoints pair by id, explicit endpoints by level, and unspecified endpoints with
// each other. The prefix keeps the three kinds from colliding (an identity of "3" is not the
// explicit level 3).
static std::string spannerPairingKey(const api::SpannerNumber &inNumber)
{
    switch (inNumber.kind())
    {
    case api::SpannerNumber::Kind::explicitLevel:
        return "e" + std::to_string(inNumber.level());

    case api::SpannerNumber::Kind::identity:
        return "i" + inNumber.identity();

    default:
        return "u";
    }
}

// Collects spanner events in the exact order MeasureWriter serializes them.
// The position counter is global; only events within the same class are ever
// compared, so cross-class interleaving is irrelevant, but within a class the
// order matches what a streaming reader sees.
class SpannerEventCollector
{
  public:
    void addNote(const api::NoteData &inNote)
    {
        myCurrentNoteTag = &inNote;

        // NotationsWriter emits curve stops, then continues, then starts, and
        // skips curves whose type is neither tie nor slur.
        const auto &attachments = inNote.noteAttachmentData;
        for (const auto &stop : attachments.curveStops)
        {
            addCurve(stop.curveType, &stop, stop.number, false, true);
        }
        for (const auto &curveContinue : attachments.curveContinuations)
        {
            addCurve(curveContinue.curveType, &curveContinue, curveContinue.number, false, false);
        }
        for (const auto &start : attachments.curveStarts)
        {
            addCurve(start.curveType, &start, start.number, true, false);
        }

        // NotationsWriter emits glissando/slide stops, then starts (see #139 stop-before-start).
        for (const auto &stop : attachments.glissandoStops)
        {
            addGlissando(stop.glissandoType, &stop, stop.number, false, true);
        }
        for (const auto &start : attachments.glissandoStarts)
        {
            addGlissando(start.glissandoType, &start, start.number, true, false);
        }

        // NotationsWriter emits wavy-line stops, then continues, then (after any mark-derived
        // ornaments, which carry no number) starts.
        for (const auto &stop : attachments.wavyLineStops)
        {
            add(SpannerNumberClass::wavyLine, &stop, stop.number, false, true);
        }
        for (const auto &wavyLineContinue : attachments.wavyLineContinuations)
        {
            add(SpannerNumberClass::wavyLine, &wavyLineContinue, wavyLineContinue.number, false, false);
        }
        for (const auto &start : attachments.wavyLineStarts)
        {
            add(SpannerNumberClass::wavyLine, &start, start.number, true, false);
        }
    }

    // Mirrors DirectionWriter::emitDirectionTypes: one pass over the ordered direction-type
    // content, registering each spanner event with the address of its DirectionChoice -- the
    // same identity the writer presents when it asks for the emitted number. Pedals are
    // skipped: mx::api does not model <pedal>'s number attribute.
    void addDirection(const api::DirectionData &inDirection)
    {
        myCurrentNoteTag = nullptr;

        for (const auto &choice : inDirection.directionTypes)
        {
            switch (choice.kind())
            {
            case api::DirectionChoice::Kind::wedgeStart:
                add(SpannerNumberClass::wedge, &choice, choice.wedgeStart().number, true, false);
                break;

            case api::DirectionChoice::Kind::wedgeStop:
                add(SpannerNumberClass::wedge, &choice, choice.wedgeStop().number, false, true);
                break;

            case api::DirectionChoice::Kind::ottavaStart: {
                const auto ottavaStart = choice.ottavaStart();
                add(SpannerNumberClass::octaveShift, &choice, ottavaStart.spannerStart.number, true, false);
                myOttavaEvents.push_back(
                    SpannerOttavaEvent{&choice, ottavaStart.spannerStart.number, ottavaStart.ottavaType, true});
                break;
            }

            case api::DirectionChoice::Kind::ottavaStop: {
                const auto ottavaStop = choice.ottavaStop();
                add(SpannerNumberClass::octaveShift, &choice, ottavaStop.spannerStop.number, false, true);
                myOttavaEvents.push_back(
                    SpannerOttavaEvent{&choice, ottavaStop.spannerStop.number, api::OttavaType::unspecified, false});
                break;
            }

            case api::DirectionChoice::Kind::bracketStart:
                add(SpannerNumberClass::bracket, &choice, choice.bracketStart().number, true, false);
                break;

            case api::DirectionChoice::Kind::bracketStop:
                add(SpannerNumberClass::bracket, &choice, choice.bracketStop().number, false, true);
                break;

            case api::DirectionChoice::Kind::dashesStart:
                add(SpannerNumberClass::dashes, &choice, choice.dashesStart().number, true, false);
                break;

            case api::DirectionChoice::Kind::dashesStop:
                add(SpannerNumberClass::dashes, &choice, choice.dashesStop().number, false, true);
                break;

            default:
                break;
            }
        }
    }

    const std::map<SpannerNumberClass, std::vector<SpannerNumberEvent>> &events() const
    {
        return myEvents;
    }

    const std::vector<SpannerOttavaEvent> &ottavaEvents() const
    {
        return myOttavaEvents;
    }

  private:
    void add(SpannerNumberClass inClass, const void *inObject, const api::SpannerNumber &inNumber, bool inOpens,
             bool inCloses)
    {
        myEvents[inClass].push_back(
            SpannerNumberEvent{myPosition, inObject, inNumber, inOpens, inCloses, myCurrentNoteTag});
        ++myPosition;
    }

    void addCurve(api::CurveType inCurveType, const void *inObject, const api::SpannerNumber &inNumber, bool inOpens,
                  bool inCloses)
    {
        if (inCurveType == api::CurveType::slur)
        {
            add(SpannerNumberClass::slur, inObject, inNumber, inOpens, inCloses);
        }
        else if (inCurveType == api::CurveType::tie)
        {
            add(SpannerNumberClass::tie, inObject, inNumber, inOpens, inCloses);
        }
    }

    void addGlissando(api::GlissandoType inGlissandoType, const void *inObject, const api::SpannerNumber &inNumber,
                      bool inOpens, bool inCloses)
    {
        const auto spannerClass =
            inGlissandoType == api::GlissandoType::slide ? SpannerNumberClass::slide : SpannerNumberClass::glissando;
        add(spannerClass, inObject, inNumber, inOpens, inCloses);
    }

    int myPosition = 0;
    const void *myCurrentNoteTag = nullptr;
    std::map<SpannerNumberClass, std::vector<SpannerNumberEvent>> myEvents;
    std::vector<SpannerOttavaEvent> myOttavaEvents;
};

// Assigns numbers within one spanner class. Explicit levels reserve their
// number for the serialized extent of their start/stop pair; identity groups
// then take the lowest number whose reservations and prior assignments do not
// overlap the group's own extent. The pool is scoped to the part: MusicXML's
// number-level documentation judges concurrency in document order within the
// part, so spanners on different staves of the part still conflict when their
// serialized extents overlap.
static void spannerNumberAssignClass(const std::vector<SpannerNumberEvent> &inEvents,
                                     std::unordered_map<const void *, int> &ioResolved)
{
    constexpr int kSpannerNumberMax = 16;

    // occupied[number] -> intervals during which that number is taken
    std::map<int, std::vector<SpannerNumberInterval>> occupied;

    // Match explicit start/stop pairs by level in stream order and reserve
    // their intervals.
    std::map<int, std::vector<const SpannerNumberEvent *>> openExplicit;
    for (const auto &event : inEvents)
    {
        if (!event.number.isExplicit())
        {
            continue;
        }
        const int level = event.number.level();
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
                occupied[level].push_back(SpannerNumberInterval{start->position, event.position});
            }
            else
            {
                // a stop with no visible start (authoring error); reserve its
                // own position so at least the stop itself cannot collide
                occupied[level].push_back(SpannerNumberInterval{event.position, event.position});
            }
        }
        else
        {
            // a continue between its start and stop is already covered by the
            // pair's interval; this point reservation only matters when the
            // continue dangles
            occupied[level].push_back(SpannerNumberInterval{event.position, event.position});
        }
    }
    for (const auto &levelAndStack : openExplicit)
    {
        for (const auto *start : levelAndStack.second)
        {
            // a start with no stop stays open to the end of the part
            occupied[levelAndStack.first].push_back(
                SpannerNumberInterval{start->position, std::numeric_limits<int>::max()});
        }
    }

    // Group identity events: same id (within this class and part) is the same
    // logical spanner. Groups are assigned in order of first appearance.
    struct SpannerNumberGroup
    {
        SpannerNumberInterval interval;
        std::vector<const void *> objects;
    };

    std::vector<SpannerNumberGroup> groups;
    std::map<std::string, std::size_t> groupIndexByIdentity;
    for (const auto &event : inEvents)
    {
        if (!event.number.isIdentity())
        {
            continue;
        }
        const auto found = groupIndexByIdentity.emplace(event.number.identity(), groups.size());
        if (found.second)
        {
            groups.push_back(SpannerNumberGroup{SpannerNumberInterval{event.position, event.position}, {}});
        }
        auto &group = groups.at(found.first->second);
        group.interval.first = std::min(group.interval.first, event.position);
        group.interval.last = std::max(group.interval.last, event.position);
        group.objects.push_back(event.object);
    }

    for (const auto &group : groups)
    {
        int chosen = 0;
        for (int candidate = 1; candidate <= kSpannerNumberMax && chosen == 0; ++candidate)
        {
            bool isFree = true;
            const auto numberIter = occupied.find(candidate);
            if (numberIter != occupied.cend())
            {
                for (const auto &interval : numberIter->second)
                {
                    if (spannerNumberIntervalsOverlap(interval, group.interval))
                    {
                        isFree = false;
                        break;
                    }
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
        occupied[chosen].push_back(group.interval);
        for (const void *object : group.objects)
        {
            ioResolved[object] = chosen;
        }
    }
}

// Pairs each ottava stop with the start it closes and records that start's size. Within one
// pairing bucket a stop closes the most recently opened start still open, so a stop that opens
// and closes inside another ottava of the same bucket does not steal the outer line's start. A
// stop with nothing open is left out of ioResolved and falls back to size 8 at write time.
static void spannerResolveOttavaSizes(const std::vector<SpannerOttavaEvent> &inEvents,
                                      std::unordered_map<const void *, int> &ioResolved)
{
    std::map<std::string, std::vector<api::OttavaType>> openStarts;

    for (const auto &event : inEvents)
    {
        auto &stack = openStarts[spannerPairingKey(event.number)];
        if (event.isStart)
        {
            stack.push_back(event.ottavaType);
            continue;
        }
        if (stack.empty())
        {
            continue;
        }
        ioResolved[event.object] = ottavaTypeSize(stack.back());
        stack.pop_back();
    }
}

// Finds spanners whose start and stop sit on the same note and records the two endpoints as
// partners. The events arrive in the writer's plain stop-before-start order, which is what makes
// the decision possible: a stop that closes a spanner opened on an earlier note belongs to that
// spanner (a chain), and a stop that closes nothing pairs with a same-bucket start on its own
// note. A leftover stop with no open start and no same-note start is an authoring error and is
// left unpaired, the same as before this detection existed.
static void spannerDetectSameNoteSpans(const std::vector<SpannerNumberEvent> &inEvents,
                                       std::unordered_map<const void *, const void *> &ioPartners)
{
    std::map<std::string, int> openCounts;
    std::map<std::string, std::vector<const SpannerNumberEvent *>> unopenedStops;

    for (const auto &event : inEvents)
    {
        const auto key = spannerPairingKey(event.number);
        if (event.closes)
        {
            auto &openCount = openCounts[key];
            if (openCount > 0)
            {
                --openCount;
            }
            else
            {
                unopenedStops[key].push_back(&event);
            }
        }
        else if (event.opens)
        {
            auto &stops = unopenedStops[key];
            const auto sameNoteStop =
                std::find_if(stops.cbegin(), stops.cend(), [&event](const SpannerNumberEvent *stop) {
                    return stop->noteTag != nullptr && stop->noteTag == event.noteTag;
                });
            if (sameNoteStop != stops.cend())
            {
                ioPartners[event.object] = (*sameNoteStop)->object;
                ioPartners[(*sameNoteStop)->object] = event.object;
                stops.erase(sameNoteStop);
            }
            else
            {
                ++openCounts[key];
            }
        }
    }
}

void SpannerResolver::resolvePart(const api::PartData &inPart)
{
    SpannerEventCollector collector;

    for (const auto &measure : inPart.measures)
    {
        for (const auto &staff : measure.staves)
        {
            for (const auto &voicePair : staff.voices)
            {
                for (const auto &note : voicePair.second.notes)
                {
                    collector.addNote(note);
                }
            }
            for (const auto &direction : staff.directions)
            {
                collector.addDirection(direction);
            }
        }
    }

    for (const auto &classAndEvents : collector.events())
    {
        spannerNumberAssignClass(classAndEvents.second, myResolved);

        const auto spannerClass = classAndEvents.first;
        if (spannerClass == SpannerNumberClass::glissando || spannerClass == SpannerNumberClass::slide ||
            spannerClass == SpannerNumberClass::wavyLine)
        {
            spannerDetectSameNoteSpans(classAndEvents.second, mySameNoteSpanPartners);
        }
    }

    spannerResolveOttavaSizes(collector.ottavaEvents(), myOttavaStopSizes);
}

std::optional<int> SpannerResolver::emittedNumber(const api::SpannerNumber &inNumber, const void *inObject) const
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

int SpannerResolver::ottavaStopSize(const void *inObject) const
{
    const auto iter = myOttavaStopSizes.find(inObject);
    if (iter == myOttavaStopSizes.cend())
    {
        return ottavaTypeSize(api::OttavaType::unspecified);
    }
    return iter->second;
}

const void *SpannerResolver::sameNoteSpanPartner(const void *inObject) const
{
    const auto iter = mySameNoteSpanPartners.find(inObject);
    if (iter == mySameNoteSpanPartners.cend())
    {
        return nullptr;
    }
    return iter->second;
}
} // namespace impl
} // namespace mx
