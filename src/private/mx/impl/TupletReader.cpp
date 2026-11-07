// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/TupletReader.h"
#include "mx/core/generated/CueNoteGroup.h"
#include "mx/core/generated/NormalNoteGroup.h"
#include "mx/core/generated/Note.h"
#include "mx/core/generated/NoteChoice.h"
#include "mx/core/generated/NoteType.h"
#include "mx/core/generated/TimeModification.h"
#include "mx/core/generated/TimeModificationGroup.h"
#include "mx/core/generated/Tuplet.h"
#include "mx/core/generated/TupletPortion.h"
#include "mx/impl/Converter.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

#include <cmath>
#include <string>
#include <string_view>
#include <utility>

namespace mx
{
namespace impl
{
TupletReader::TupletReader(const core::Tuplet &inMxTuplet, MeasureCursor inCursor, const core::Note &inNote,
                           DiagnosticsContext diagnostics)
    : myTuplet{inMxTuplet}, myNote{inNote}, myCursor{std::move(inCursor)}, myDiagnostics{std::move(diagnostics)}
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
        tupletStart.number = api::SpannerNumber{myTuplet.number()->value()};
    }

    Converter converter;

    if (myTuplet.type() == core::StartStop::stop())
    {
        api::TupletStop tupletStop;
        tupletStop.positionData = tupletStart.positionData;
        tupletStop.number = tupletStart.number;
        tupletStop.id = getId(myTuplet);
        if (myTuplet.bracket().has_value())
        {
            tupletStop.bracket = converter.convert(*myTuplet.bracket());
        }
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

    if (myTuplet.showType().has_value())
    {
        switch (myTuplet.showType()->tag())
        {
        case core::ShowTuplet::Tag::none:
            tupletStart.showActualType = api::Bool::no;
            tupletStart.showNormalType = api::Bool::no;
            break;
        case core::ShowTuplet::Tag::both:
            tupletStart.showActualType = api::Bool::yes;
            tupletStart.showNormalType = api::Bool::yes;
            break;
        case core::ShowTuplet::Tag::actual:
            tupletStart.showActualType = api::Bool::yes;
            tupletStart.showNormalType = api::Bool::no;
            break;
        default:
            break;
        }
    }

    if (myTuplet.bracket().has_value())
    {
        tupletStart.bracket = converter.convert(*myTuplet.bracket());
    }

    if (myTuplet.lineShape().has_value())
    {
        tupletStart.lineShape = converter.convert(*myTuplet.lineShape());
    }

    if (myTuplet.tupletActual().has_value())
    {
        const auto &actual = *myTuplet.tupletActual();

        if (actual.tupletNumber().has_value())
        {
            tupletStart.actualNumber = actual.tupletNumber()->value().value();
        }

        if (actual.tupletType().has_value())
        {
            tupletStart.actualDurationName = converter.convert(actual.tupletType()->value());
        }

        tupletStart.actualDots = static_cast<int>(actual.tupletDot().size());

        // The source stated the actual portion, so say it again on the way out even if the
        // note's time modification already implies it.
        tupletStart.writeActual = api::Bool::yes;
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
            tupletStart.normalNumber = normal.tupletNumber()->value().value();
        }

        if (normal.tupletType().has_value())
        {
            tupletStart.normalDurationName = converter.convert(normal.tupletType()->value());
        }

        tupletStart.normalDots = static_cast<int>(normal.tupletDot().size());
        tupletStart.writeNormal = api::Bool::yes;
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
    outTupletStart.normalNumber = myNote.timeModification()->normalNotes().value();
    guessFigureFromNote(outTupletStart.normalDurationName, outTupletStart.normalDots);
}

void TupletReader::guessActualFromNote(api::TupletStart &outTupletStart)
{
    if (!myNote.timeModification().has_value())
    {
        return;
    }
    outTupletStart.actualNumber = myNote.timeModification()->actualNotes().value();

    // The schema counts both actual-notes and normal-notes in the normal-type, so the actual
    // notes have the same note value as the normal notes, whatever the note's own written type.
    // The schema's example is a quarter note inside an eighth-note triplet: 3 eighths in the time
    // of 2 eighths, not 3 quarters (#485).
    guessFigureFromNote(outTupletStart.actualDurationName, outTupletStart.actualDots);
    checkDurationAgainstGuess();
}

// The note value a time modification counts in: its normal-type and normal-dots when it has
// them, otherwise the note's own written value, dots included (see the time-modification type
// in the MusicXML schema).
void TupletReader::guessFigureFromNote(api::DurationName &outDurationName, int &outDots) const
{
    Converter converter;
    const auto &timeMod = *myNote.timeModification();
    if (timeMod.group().has_value())
    {
        const auto &grp = *timeMod.group();
        outDurationName = converter.convert(grp.normalType());
        outDots = static_cast<int>(grp.normalDot().size());
        return;
    }
    if (myNote.type().has_value())
    {
        outDurationName = converter.convert(myNote.type()->value());
    }
    outDots = static_cast<int>(myNote.dot().size());
}

// A guessed tuplet-actual relies on the file following the schema, where the note sounds for its
// written value times normal-notes over actual-notes. A file might instead count the actual notes
// in the note's own written type, and the note's duration tells the two apart. mx keeps the schema
// reading either way and reports the disagreement so a client can look closer.
void TupletReader::checkDurationAgainstGuess() const
{
    if (!myNote.timeModification().has_value() || !myNote.type().has_value() || myCursor.ticksPerQuarter <= 0)
    {
        return;
    }

    // A grace note does not have a duration to check.
    double duration = 0.0;
    switch (myNote.choice().kind())
    {
    case core::NoteChoice::Kind::normalNoteGroup:
        duration = myNote.choice().asNormalNoteGroup().duration().value().value();
        break;
    case core::NoteChoice::Kind::cueNoteGroup:
        duration = myNote.choice().asCueNoteGroup().duration().value().value();
        break;
    default:
        return;
    }

    const auto &timeMod = *myNote.timeModification();
    const int actualNotes = timeMod.actualNotes().value();
    const int normalNotes = timeMod.normalNotes().value();
    if (actualNotes <= 0 || normalNotes <= 0)
    {
        return;
    }

    Converter converter;
    const auto writtenType = myNote.type()->value();
    const int writtenDots = static_cast<int>(myNote.dot().size());
    const double writtenQuarters = quartersOf(converter.convert(writtenType), writtenDots);
    api::DurationName normalName = api::DurationName::unspecified;
    int normalDots = 0;
    guessFigureFromNote(normalName, normalDots);
    const double normalQuarters = quartersOf(normalName, normalDots);
    if (writtenQuarters <= 0.0 || normalQuarters <= 0.0)
    {
        return;
    }

    const double divisions = static_cast<double>(myCursor.ticksPerQuarter);
    const double ratio = static_cast<double>(normalNotes) / static_cast<double>(actualNotes);
    const double schemaDuration = writtenQuarters * ratio * divisions;
    const double mixedDuration = normalQuarters * ratio * divisions;

    // A file whose divisions are too coarse for the tuplet has to round each duration to a whole
    // number of divisions, for example a triplet eighth at one division per quarter. Allowing one
    // division either way accepts any such rounding.
    constexpr double tolerance = 1.0 + 1e-9;
    if (std::abs(duration - schemaDuration) <= tolerance)
    {
        return;
    }

    // For example "eighth", "dotted eighth", or "eighth with 2 dots". With a noun, the noun follows
    // the note value: "dotted eighth notes", "eighth notes with 2 dots".
    const auto figureText = [](std::string_view inName, int inDots, std::string_view inNoun) {
        std::string text{inName};
        if (!inNoun.empty())
        {
            text += " ";
            text += inNoun;
        }
        if (inDots == 1)
        {
            text = "dotted " + text;
        }
        else if (inDots > 1)
        {
            text += " with " + std::to_string(inDots) + " dots";
        }
        return text;
    };
    const auto guessText = timeMod.group().has_value()
                               ? figureText(timeMod.group()->normalType().toString(), normalDots, "notes")
                               : figureText(writtenType.toString(), writtenDots, "notes");
    const auto writtenText = figureText(writtenType.toString(), writtenDots, "");

    std::string message = "tuplet start has no tuplet-actual, so mx guessed " + std::to_string(actualNotes) + " " +
                          guessText + ", the note value the time-modification counts in; the note's duration is " +
                          formatReportedNumber(duration) + " divisions, but that reading expects " +
                          formatReportedNumber(schemaDuration);

    // When the note is written in the note value the time-modification counts in, both readings
    // expect the same duration, so there is no other reading to offer.
    if (std::abs(schemaDuration - mixedDuration) < 1e-9)
    {
        message += "; the duration does not fit the time-modification";
    }
    else if (std::abs(duration - mixedDuration) <= tolerance)
    {
        message += ". The duration fits counting the actual notes in the note's own written type (" + writtenText +
                   ") instead; a client that needs that reading should look at the time-modification itself";
    }
    else
    {
        message += ", and counting the actual notes in the note's own written type would expect " +
                   formatReportedNumber(mixedDuration) + "; the duration fits neither reading";
    }
    myDiagnostics.report(api::Severity::warning, api::DiagnosticCode::specConflict, myCursor, std::move(message));
}

double TupletReader::quartersOf(api::DurationName inDurationName, int inDots)
{
    double quarters = 0.0;
    switch (inDurationName)
    {
    case api::DurationName::maxima:
        quarters = api::DUR_QUARTERS_VALUE_MAXIMA;
        break;
    case api::DurationName::longa:
        quarters = api::DUR_QUARTERS_VALUE_LONGA;
        break;
    case api::DurationName::breve:
        quarters = api::DUR_QUARTERS_VALUE_BREVE;
        break;
    case api::DurationName::whole:
        quarters = api::DUR_QUARTERS_VALUE_WHOLE;
        break;
    case api::DurationName::half:
        quarters = api::DUR_QUARTERS_VALUE_HALF;
        break;
    case api::DurationName::quarter:
        quarters = api::DUR_QUARTERS_VALUE_QUARTER;
        break;
    case api::DurationName::eighth:
        quarters = api::DUR_QUARTERS_VALUE_EIGHTH;
        break;
    case api::DurationName::dur16th:
        quarters = api::DUR_QUARTERS_VALUE_16TH;
        break;
    case api::DurationName::dur32nd:
        quarters = api::DUR_QUARTERS_VALUE_32ND;
        break;
    case api::DurationName::dur64th:
        quarters = api::DUR_QUARTERS_VALUE_64TH;
        break;
    case api::DurationName::dur128th:
        quarters = api::DUR_QUARTERS_VALUE_128TH;
        break;
    case api::DurationName::dur256th:
        quarters = api::DUR_QUARTERS_VALUE_256TH;
        break;
    case api::DurationName::dur512th:
        quarters = api::DUR_QUARTERS_VALUE_512TH;
        break;
    case api::DurationName::dur1024th:
        quarters = api::DUR_QUARTERS_VALUE_1024TH;
        break;
    default:
        return 0.0;
    }

    // Each dot adds half of what the previous one added: one dot is 1.5 times, two are 1.75.
    return quarters * (2.0 - 1.0 / std::pow(2.0, inDots));
}

} // namespace impl
} // namespace mx
