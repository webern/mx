// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/TempoData.h"

#include <utility>

namespace mx
{
namespace api
{

TempoChoice::TempoChoice() : myValue{BeatsPerMinute{}}
{
}

TempoChoice::TempoChoice(BeatsPerMinute value) : myValue{std::move(value)}
{
}

TempoChoice::TempoChoice(MetricModulation value) : myValue{std::move(value)}
{
}

TempoChoice::TempoChoice(NoteRelation value) : myValue{std::move(value)}
{
}

TempoChoice::Kind TempoChoice::kind() const
{
    return static_cast<Kind>(myValue.index());
}

bool TempoChoice::isBeatsPerMinute() const
{
    return std::holds_alternative<BeatsPerMinute>(myValue);
}

bool TempoChoice::isMetricModulation() const
{
    return std::holds_alternative<MetricModulation>(myValue);
}

bool TempoChoice::isNoteRelation() const
{
    return std::holds_alternative<NoteRelation>(myValue);
}

BeatsPerMinute TempoChoice::beatsPerMinute() const
{
    if (const auto *value = std::get_if<BeatsPerMinute>(&myValue))
    {
        return *value;
    }
    return BeatsPerMinute{};
}

MetricModulation TempoChoice::metricModulation() const
{
    if (const auto *value = std::get_if<MetricModulation>(&myValue))
    {
        return *value;
    }
    return MetricModulation{};
}

NoteRelation TempoChoice::noteRelation() const
{
    if (const auto *value = std::get_if<NoteRelation>(&myValue))
    {
        return *value;
    }
    return NoteRelation{};
}

bool TempoChoice::operator==(const TempoChoice &other) const
{
    return myValue == other.myValue;
}

} // namespace api
} // namespace mx
