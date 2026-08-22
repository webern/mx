// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/MetronomeReader.h"
#include "mx/core/generated/BeamLevel.h"
#include "mx/core/generated/BeatUnitGroup.h"
#include "mx/core/generated/BeatUnitTied.h"
#include "mx/core/generated/Metronome.h"
#include "mx/core/generated/MetronomeBeam.h"
#include "mx/core/generated/MetronomeChoice.h"
#include "mx/core/generated/MetronomeChoiceGroup.h"
#include "mx/core/generated/MetronomeChoiceGroup2.h"
#include "mx/core/generated/MetronomeChoiceGroup2Group.h"
#include "mx/core/generated/MetronomeChoiceGroupChoice.h"
#include "mx/core/generated/MetronomeChoiceGroupChoiceGroup.h"
#include "mx/core/generated/MetronomeNote.h"
#include "mx/core/generated/MetronomeTied.h"
#include "mx/core/generated/MetronomeTuplet.h"
#include "mx/core/generated/PerMinute.h"
#include "mx/core/generated/ShowTuplet.h"
#include "mx/core/generated/StartStop.h"
#include "mx/core/generated/TimeModificationGroup.h"
#include "mx/impl/Converter.h"
#include "mx/impl/FontFunctions.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"

namespace mx
{
namespace impl
{
namespace
{
api::BeatUnit readBeatUnitGroup(const Converter &converter, const core::BeatUnitGroup &group)
{
    api::BeatUnit beatUnit;
    beatUnit.type = converter.convert(group.beatUnit());
    beatUnit.dots = static_cast<int>(group.beatUnitDot().size());
    return beatUnit;
}

// Reads the beat-unit (type + dots) and any tied continuation beat-units from a holder that
// exposes beatUnit() and beatUnitTied() -- the left group and the right (metric-modulation) group
// share this shape.
template <typename BeatUnitHolder>
void readBeatUnitWithTies(const Converter &converter, const BeatUnitHolder &holder, api::DurationName &outName,
                          int &outDots, std::vector<api::BeatUnit> &outTied)
{
    outName = converter.convert(holder.beatUnit().beatUnit());
    outDots = static_cast<int>(holder.beatUnit().beatUnitDot().size());
    for (const auto &tied : holder.beatUnitTied())
    {
        outTied.push_back(readBeatUnitGroup(converter, tied.beatUnit()));
    }
}

api::MetronomeNoteData readMetronomeNote(const Converter &converter, const core::MetronomeNote &note)
{
    api::MetronomeNoteData out;
    out.metronomeType = converter.convert(note.metronomeType());
    out.dots = static_cast<int>(note.metronomeDot().size());

    for (const auto &beam : note.metronomeBeam())
    {
        api::MetronomeBeam apiBeam;
        apiBeam.value = converter.convert(beam.value());
        if (beam.number().has_value())
        {
            apiBeam.number = beam.number()->value();
        }
        out.beams.push_back(apiBeam);
    }

    if (note.metronomeTied().has_value())
    {
        out.tie = note.metronomeTied()->type().tag() == core::StartStop::Tag::start ? api::MetronomeTieType::start
                                                                                    : api::MetronomeTieType::stop;
    }

    if (note.metronomeTuplet().has_value())
    {
        const auto &tuplet = *note.metronomeTuplet();
        api::MetronomeTuplet apiTuplet;
        apiTuplet.actualNotes = tuplet.actualNotes();
        apiTuplet.normalNotes = tuplet.normalNotes();
        if (tuplet.group().has_value())
        {
            apiTuplet.normalType = converter.convert(tuplet.group()->normalType());
            apiTuplet.normalDots = static_cast<int>(tuplet.group()->normalDot().size());
        }
        apiTuplet.type = tuplet.type().tag() == core::StartStop::Tag::start ? api::MetronomeTupletType::start
                                                                            : api::MetronomeTupletType::stop;
        if (tuplet.bracket().has_value())
        {
            apiTuplet.bracket = converter.convert(*tuplet.bracket());
        }
        if (tuplet.showNumber().has_value())
        {
            switch (tuplet.showNumber()->tag())
            {
            case core::ShowTuplet::Tag::actual:
                apiTuplet.showNumber = api::MetronomeShowNumber::actual;
                break;
            case core::ShowTuplet::Tag::both:
                apiTuplet.showNumber = api::MetronomeShowNumber::both;
                break;
            case core::ShowTuplet::Tag::none:
                apiTuplet.showNumber = api::MetronomeShowNumber::none;
                break;
            }
        }
        out.tuplet = apiTuplet;
    }

    return out;
}
} // namespace

MetronomeReader::MetronomeReader(MetronomeReaderParameters &&params)
    : myMutex{}, myOutTempoData{}, myMetronome{params.metronome},
      myPreviousTempoData{std::move(params.previousTempoData)}, myCursor{std::move(params.cursor)},
      myBeatUnitPerOrNoteRelationNoteChoice{myMetronome.choice()}
{
}

api::TempoData MetronomeReader::getTempoData() const
{
    std::lock_guard<std::mutex> lock{myMutex};
    myOutTempoData = api::TempoData{};

    Converter converter;

    // print-style-align + print-object + justify + parentheses + id attributes.
    myOutTempoData.positionData = getPositionData(myMetronome);
    myOutTempoData.fontData = getFontData(myMetronome);
    if (myMetronome.color().has_value())
    {
        myOutTempoData.color = getColor(myMetronome);
    }
    myOutTempoData.id = getId(myMetronome);
    if (myMetronome.justify().has_value())
    {
        myOutTempoData.justify = converter.convert(*myMetronome.justify());
    }
    myOutTempoData.printObject = getPrintObject(myMetronome);
    if (myMetronome.parentheses().has_value())
    {
        myOutTempoData.isParenthetical = converter.convert(*myMetronome.parentheses());
    }

    // The 'group' alternative is the beat-unit form (note = number, or note = note); 'group2' is
    // the metronome-note form (a metric relationship drawn with note figures).
    using FirstChoice = core::MetronomeChoice::Kind;
    const auto firstChoice = myBeatUnitPerOrNoteRelationNoteChoice.kind();

    switch (firstChoice)
    {
    case FirstChoice::group: {
        parseBeatUnitPer();
        break;
    }
    case FirstChoice::group2: {
        parseNoteRelationNote();
        break;
    }
    default:
        break;
    }

    api::TempoData temp{std::move(myOutTempoData)};
    myOutTempoData = api::TempoData{};
    return temp;
}

void MetronomeReader::parseBeatUnitPer() const
{
    const auto &beatUnitPer = myBeatUnitPerOrNoteRelationNoteChoice.asGroup();
    const auto choice = beatUnitPer.choice().kind();

    switch (choice)
    {
    case core::MetronomeChoiceGroupChoice::Kind::perMinute: {
        parseBeatsPerMinute();
        break;
    }
    case core::MetronomeChoiceGroupChoice::Kind::group: {
        parseMetronomeModulation();
        break;
    }
    default:
        break;
    }
}

void MetronomeReader::parseNoteRelationNote() const
{
    // The metronome-note form: one or more metronome-note figures, optionally followed by a
    // metronome-relation symbol and a second group of figures. See NoteRelation.
    const auto &group2 = myBeatUnitPerOrNoteRelationNoteChoice.asGroup2();
    Converter converter;

    api::NoteRelation noteRelation;
    noteRelation.arrows = group2.metronomeArrows();
    for (const auto &note : group2.metronomeNote())
    {
        noteRelation.notes.push_back(readMetronomeNote(converter, note));
    }

    if (group2.group().has_value())
    {
        api::MetronomeRelation relation;
        relation.symbol = group2.group()->metronomeRelation();
        for (const auto &note : group2.group()->metronomeNote())
        {
            relation.notes.push_back(readMetronomeNote(converter, note));
        }
        noteRelation.relation = std::move(relation);
    }

    myOutTempoData.choice = api::TempoChoice{std::move(noteRelation)};
}

void MetronomeReader::parseBeatsPerMinute() const
{
    const auto &beatUnitPer = myBeatUnitPerOrNoteRelationNoteChoice.asGroup();
    Converter converter;

    api::BeatsPerMinute bpm;
    readBeatUnitWithTies(converter, beatUnitPer, bpm.durationName, bpm.dots, bpm.tiedBeatUnits);
    // per-minute is xs:string in MusicXML ("120", "ca. 76", a range, ...); keep it verbatim.
    bpm.beatsPerMinute = beatUnitPer.choice().asPerMinute().value();

    myOutTempoData.choice = api::TempoChoice{std::move(bpm)};
}

void MetronomeReader::parseMetronomeModulation() const
{
    // Metric modulation: <beat-unit>..</beat-unit> = <beat-unit>..</beat-unit>. The left beat-unit
    // lives directly on the group; the right beat-unit is the 'group' alternative of the choice.
    const auto &beatUnitPer = myBeatUnitPerOrNoteRelationNoteChoice.asGroup();
    Converter converter;

    api::MetricModulation mm;
    readBeatUnitWithTies(converter, beatUnitPer, mm.leftDurationName, mm.leftDots, mm.leftTiedBeatUnits);
    const auto &rightGroup = beatUnitPer.choice().asGroup();
    readBeatUnitWithTies(converter, rightGroup, mm.rightDurationName, mm.rightDots, mm.rightTiedBeatUnits);

    myOutTempoData.choice = api::TempoChoice{std::move(mm)};
}
} // namespace impl
} // namespace mx
