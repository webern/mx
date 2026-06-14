// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NoteFunctions.h"
#include "mx/api/MarkData.h"
#include "mx/api/SpannerData.h"
#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/impl/ArpeggiateFunctions.h"
#include "mx/impl/ArticulationsFunctions.h"
#include "mx/impl/CurveFunctions.h"
#include "mx/impl/DynamicsReader.h"
#include "mx/impl/FermataFunctions.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/NonArpeggiateFunctions.h"
#include "mx/impl/NoteReader.h"
#include "mx/impl/OrnamentsFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/SlideFunctions.h"
#include "mx/impl/TechnicalFunctions.h"
#include "mx/impl/TimeReader.h"
#include "mx/impl/TupletReader.h"
#include "mx/utility/Throw.h"

#include <algorithm>

namespace mx
{
namespace impl
{
NoteFunctions::NoteFunctions(const core::Note &inMxNote, impl::Cursor inCursor)
    : myNote{inMxNote}, myCursor{inCursor}, myOutNoteData{}
{
}

api::NoteData NoteFunctions::parseNote() const
{
    // lock it because we have the function marked as const
    // but we are writing to a mutable NoteData (for convenience)
    std::lock_guard<std::mutex> guard{myMutex};
    myOutNoteData = api::NoteData{};
    impl::NoteReader reader{myNote};

    if (reader.getIsRest())
    {
        myOutNoteData.isRest = true;
        myOutNoteData.isMeasureRest = reader.getIsMeasureRest();
    }

    myOutNoteData.isUnpitched = reader.getIsUnpitched();
    myOutNoteData.isDisplayStepOctaveSpecified = reader.getIsDisplayStepOctaveSpecified();
    myOutNoteData.isChord = reader.getIsChord();

    if (reader.getIsCue())
    {
        myOutNoteData.noteType = api::NoteType::cue;
    }

    if (reader.getIsGrace())
    {
        myOutNoteData.noteType = api::NoteType::grace;
    }

    if (reader.getIsNormal())
    {
        myOutNoteData.noteType = api::NoteType::normal;
    }

    auto converter = Converter{};
    myOutNoteData.pitchData.step = converter.convert(reader.getStep());
    myOutNoteData.pitchData.alter = reader.getAlter();
    myOutNoteData.pitchData.cents = reader.getCents();

    myOutNoteData.pitchData.accidental = api::Accidental::none;

    if (reader.getHasAccidental())
    {
        myOutNoteData.pitchData.accidental = converter.convert(reader.getAccidental());
        myOutNoteData.pitchData.isAccidentalParenthetical = reader.getIsAccidentalParenthetical();
        myOutNoteData.pitchData.isAccidentalCautionary = reader.getIsAccidentalCautionary();
        myOutNoteData.pitchData.isAccidentalEditorial = reader.getIsAccidentalEditorial();
        myOutNoteData.pitchData.isAccidentalBracketed = reader.getIsAccidentalBracketed();
    }

    myOutNoteData.pitchData.octave = reader.getOctave();
    // myOutNoteData.staffIndex = reader.getStaffNumber() - 1;
    myOutNoteData.userRequestedVoiceNumber = reader.getVoiceNumber();

    myOutNoteData.notehead = converter.convert(reader.getNoteheadValue());

    if (reader.getIsDurationTypeSpecified())
    {
        myOutNoteData.durationData.durationName = converter.convert(reader.getDurationType());
    }
    else
    {
        myOutNoteData.durationData.durationName = deriveNoteTypeFromDurationValue(reader);
    }

    myOutNoteData.durationData.durationDots = reader.getNumDots();
    myOutNoteData.durationData.durationTimeTicks = myCursor.convertDurationToGlobalTickScale(reader.getDurationValue());
    myOutNoteData.tickTimePosition = myCursor.tickTimePosition;

    for (const auto &coreBeamVal : reader.getBeams())
    {
        myOutNoteData.beams.push_back(converter.convert(coreBeamVal));
    }

    myOutNoteData.durationData.timeModificationActualNotes = reader.getTimeModificationActualNotes();
    myOutNoteData.durationData.timeModificationNormalNotes = reader.getTimeModificationNormalNotes();
    myOutNoteData.durationData.timeModificationNormalType = converter.convert(reader.getTimeModificationNormalType());
    myOutNoteData.durationData.timeModificationNormalTypeDots = reader.getTimeModificationNormalTypeDots();

    const core::NoteTypeValue timeModType = reader.getTimeModificationNormalType();
    const int timeModTypeDots = reader.getTimeModificationNormalTypeDots();
    bool isTimeModTypeSpecified = (timeModTypeDots > 0) && (timeModType != reader.getDurationType());

    if (isTimeModTypeSpecified)
    {
        myOutNoteData.durationData.timeModificationNormalType = converter.convert(timeModType);
        myOutNoteData.durationData.timeModificationNormalTypeDots = timeModTypeDots;
    }
    else
    {
        myOutNoteData.durationData.timeModificationNormalType = api::DurationName::unspecified;
        myOutNoteData.durationData.timeModificationNormalTypeDots = 0;
    }
    parseNotations();
    myOutNoteData.positionData = impl::getPositionData(myNote);
    myOutNoteData.printData = impl::getPrintData(myNote);

    if (reader.getIsStemSpecified())
    {
        myOutNoteData.stem = converter.convert(reader.getStem());
    }
    myOutNoteData.isTieStart = reader.getIsTieStart();
    myOutNoteData.isTieStop = reader.getIsTieStop();
    myOutNoteData.lyrics = reader.getLyrics();

    parseMiscData();
    myOutNoteData.positionData = impl::getPositionData(myNote);

    return myOutNoteData;
}

// TODO - make this work even if notes are dotted and if they are tupleted
api::DurationName NoteFunctions::deriveNoteTypeFromDurationValue(const NoteReader &reader) const
{
    const double durationValue = reader.getDurationValue();
    const double ticksPerQuarter = static_cast<double>(myCursor.getGlobalTicksPerQuarter());

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_QUARTER * ticksPerQuarter))
    {
        return api::DurationName::quarter;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_HALF * ticksPerQuarter))
    {
        return api::DurationName::half;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_WHOLE * ticksPerQuarter))
    {
        return api::DurationName::whole;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_EIGHTH * ticksPerQuarter))
    {
        return api::DurationName::eighth;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_16TH * ticksPerQuarter))
    {
        return api::DurationName::dur16th;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_32ND * ticksPerQuarter))
    {
        return api::DurationName::dur32nd;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_64TH * ticksPerQuarter))
    {
        return api::DurationName::dur64th;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_128TH * ticksPerQuarter))
    {
        return api::DurationName::dur128th;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_512TH * ticksPerQuarter))
    {
        return api::DurationName::dur512th;
    }

    if (api::areSame(durationValue, api::DUR_QUARTERS_VALUE_1024TH * ticksPerQuarter))
    {
        return api::DurationName::dur1024th;
    }

    return api::DurationName::unspecified;
}

void NoteFunctions::parseNotations() const
{
    for (const auto &notations : myNote.notations())
    {
        for (const auto &notationsChoice : notations.choice())
        {
            const auto choice = notationsChoice.kind();

            switch (choice)
            {
            case core::NotationsChoice::Kind::tied: {
                parseCurve(notationsChoice.asTied(), myOutNoteData);
                break;
            }
            case core::NotationsChoice::Kind::slur: {
                parseCurve(notationsChoice.asSlur(), myOutNoteData);
                break;
            }
            case core::NotationsChoice::Kind::tuplet: {
                TupletReader reader{notationsChoice.asTuplet(), myCursor, myNote};
                reader.parseTuplet(myOutNoteData.noteAttachmentData.tupletStarts,
                                   myOutNoteData.noteAttachmentData.tupletStops);
                break;
            }
            case core::NotationsChoice::Kind::slide: {
                // TODO - import slide
                // SlideFunctions funcs{ notationsChoice.asSlide(), myCursor };
                // myOutNoteData.noteAttachmentData.marks.emplace_back( funcs.parseSlide() );
                break;
            }
            case core::NotationsChoice::Kind::glissando: {
                break;
            }
            case core::NotationsChoice::Kind::ornaments: {
                // TODO - some ornaments should be treated as spanners instead of marks
                OrnamentsFunctions funcs{notationsChoice.asOrnaments(), myCursor};
                funcs.parseOrnaments(myOutNoteData.noteAttachmentData.marks);
                break;
            }
            case core::NotationsChoice::Kind::technical: {
                TechnicalFunctions funcs{notationsChoice.asTechnical().choice(), myCursor};
                funcs.parseTechnicalMarks(myOutNoteData.noteAttachmentData.marks);
                break;
            }
            case core::NotationsChoice::Kind::articulations: {
                ArticulationsFunctions funcs{notationsChoice.asArticulations().choice(), myCursor};
                funcs.parseArticulations(myOutNoteData.noteAttachmentData.marks);
                break;
            }
            case core::NotationsChoice::Kind::dynamics: {
                DynamicsReader funcs{notationsChoice.asDynamics(), myCursor};
                funcs.parseDynamics(myOutNoteData.noteAttachmentData.marks);
                break;
            }
            case core::NotationsChoice::Kind::fermata: {
                FermataFunctions funcs{notationsChoice.asFermata(), myCursor};
                myOutNoteData.noteAttachmentData.marks.emplace_back(funcs.parseFermata());
                break;
            }
            case core::NotationsChoice::Kind::arpeggiate: {
                ArpeggiateFunctions funcs{notationsChoice.asArpeggiate(), myCursor};
                myOutNoteData.noteAttachmentData.marks.emplace_back(funcs.parseArpeggiate());
                break;
            }
            case core::NotationsChoice::Kind::nonArpeggiate: {
                NonArpeggiateFunctions funcs{notationsChoice.asNonArpeggiate(), myCursor};
                myOutNoteData.noteAttachmentData.marks.emplace_back(funcs.parseNonArpeggiate());
                break;
            }
            case core::NotationsChoice::Kind::accidentalMark: {
                AccidentalMarkFunctions funcs{notationsChoice.asAccidentalMark(), myCursor};
                myOutNoteData.noteAttachmentData.marks.emplace_back(funcs.parseAccidentalMark());
                break;
            }
            case core::NotationsChoice::Kind::otherNotation: {
                // TODO - import otherNotation
                break;
            }
            default:
                break;
            }
        }
    }
}

void NoteFunctions::parseMiscData() const
{
    if (!myNote.editorialVoice().footnote().has_value())
    {
        return;
    }

    const auto &footnote = myNote.editorialVoice().footnote().value();

    if (!footnote.value().empty())
    {
        // we expect this element to be unused if it is
        // stuffed with ##misc-data## in the font-family
        return;
    }

    if (!footnote.fontFamily().has_value())
    {
        return;
    }

    const auto &miscData = footnote.fontFamily().value();
    const auto items = miscData.items();

    if (items.empty())
    {
        return;
    }

    const auto &firstString = items[0];

    if (firstString.substr(0, 13) != "##misc-data##")
    {
        return;
    }

    myOutNoteData.miscData.push_back(firstString.substr(13));

    for (size_t i = 1; i < items.size(); ++i)
    {
        myOutNoteData.miscData.push_back(items[i]);
    }
}
} // namespace impl
} // namespace mx
