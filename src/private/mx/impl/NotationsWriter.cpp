// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NotationsWriter.h"
#include "mx/core/Token.h"
#include "mx/core/generated/AccidentalMark.h"
#include "mx/core/generated/Arpeggiate.h"
#include "mx/core/generated/ArrowChoice.h"
#include "mx/core/generated/ArrowChoiceGroup.h"
#include "mx/core/generated/ArrowDirection.h"
#include "mx/core/generated/ArticulationsChoice.h"
#include "mx/core/generated/BreathMark.h"
#include "mx/core/generated/Caesura.h"
#include "mx/core/generated/CaesuraValue.h"
#include "mx/core/generated/EmptyLine.h"
#include "mx/core/generated/EmptyPlacement.h"
#include "mx/core/generated/EmptyPlacementSmufl.h"
#include "mx/core/generated/EmptyTrillSound.h"
#include "mx/core/generated/Fermata.h"
#include "mx/core/generated/FermataShape.h"
#include "mx/core/generated/Fingering.h"
#include "mx/core/generated/Fret.h"
#include "mx/core/generated/Handbell.h"
#include "mx/core/generated/HandbellValue.h"
#include "mx/core/generated/HarmonMute.h"
#include "mx/core/generated/Harmonic.h"
#include "mx/core/generated/HarmonicChoice.h"
#include "mx/core/generated/HarmonicChoice2.h"
#include "mx/core/generated/HeelToe.h"
#include "mx/core/generated/Hole.h"
#include "mx/core/generated/HoleClosed.h"
#include "mx/core/generated/HoleClosedValue.h"
#include "mx/core/generated/HorizontalTurn.h"
#include "mx/core/generated/Mordent.h"
#include "mx/core/generated/NonArpeggiate.h"
#include "mx/core/generated/NotationsChoice.h"
#include "mx/core/generated/NumberLevel.h"
#include "mx/core/generated/OrnamentsGroup.h"
#include "mx/core/generated/OrnamentsGroupChoice.h"
#include "mx/core/generated/OtherPlacementText.h"
#include "mx/core/generated/PlacementText.h"
#include "mx/core/generated/ShowTuplet.h"
#include "mx/core/generated/Slur.h"
#include "mx/core/generated/SmuflGlyphName.h"
#include "mx/core/generated/String.h"
#include "mx/core/generated/StringNumber.h"
#include "mx/core/generated/StrongAccent.h"
#include "mx/core/generated/TechnicalChoice.h"
#include "mx/core/generated/Tied.h"
#include "mx/core/generated/TopBottom.h"
#include "mx/core/generated/Tremolo.h"
#include "mx/core/generated/TremoloMarks.h"
#include "mx/core/generated/TremoloType.h"
#include "mx/core/generated/Tuplet.h"
#include "mx/core/generated/TupletDot.h"
#include "mx/core/generated/TupletNumber.h"
#include "mx/core/generated/TupletPortion.h"
#include "mx/core/generated/TupletType.h"
#include "mx/core/generated/UpDown.h"
#include "mx/core/generated/UprightInverted.h"
#include "mx/core/generated/WavyLine.h"
#include "mx/impl/Converter.h"
#include "mx/impl/CurveFunctions.h"
#include "mx/impl/DynamicsWriter.h"
#include "mx/impl/GlissandoFunctions.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/impl/WavyLineFunctions.h"

#include <cstddef>
#include <string>
#include <vector>

namespace mx
{
namespace impl
{
void notationsWriterSetMordentSpecificAttributes(const api::MarkData &mark, core::Mordent &mordent)
{
    Converter converter;

    if (mark.hasMordentLong)
    {
        mordent.setLong(converter.convert(mark.mordentLong));
    }

    if (mark.hasMordentApproach && mark.mordentApproach != api::Placement::unspecified)
    {
        mordent.setApproach(converter.convert(mark.mordentApproach));
    }

    if (mark.hasMordentDeparture && mark.mordentDeparture != api::Placement::unspecified)
    {
        mordent.setDeparture(converter.convert(mark.mordentDeparture));
    }
}

core::NotationsChoice notationsWriterMakeTupletStop(const api::TupletStop &inTupletStop,
                                                    const std::optional<int> &inResolvedNumber,
                                                    const Converter &inConverter, const DiagnosticsContext &diagnostics,
                                                    const api::Location &location)
{
    core::Tuplet tuplet;
    tuplet.setType(core::StartStop::stop());
    setAttributesFromPositionData(inTupletStop.positionData, tuplet);
    setId(inTupletStop.id, tuplet, diagnostics, location);

    if (inResolvedNumber.has_value())
    {
        tuplet.setNumber(core::NumberLevel{*inResolvedNumber});
    }

    if (inTupletStop.bracket != api::Bool::unspecified)
    {
        tuplet.setBracket(inConverter.convert(inTupletStop.bracket));
    }

    return core::NotationsChoice::tuplet(tuplet);
}

// What one portion of a tuplet ratio says: how many notes, of what written value, with how many
// dots. VALUE_UNSPECIFIED and DurationName::unspecified mean the portion leaves that part out.
struct NotationsWriterTupletPortion
{
    int number;
    api::DurationName durationName;
    int dots;
};

// True when nothing in the portion would reach the file.
bool notationsWriterTupletPortionIsEmpty(const NotationsWriterTupletPortion &inPortion)
{
    return inPortion.number == api::VALUE_UNSPECIFIED && inPortion.durationName == api::DurationName::unspecified &&
           inPortion.dots <= 0;
}

// True when every part the portion states matches what the note's time modification already
// says, so writing the portion would only repeat the note. MusicXML reads an absent portion
// from the time modification, which is why the repetition can be left out.
bool notationsWriterTupletPortionIsRedundant(const NotationsWriterTupletPortion &inPortion,
                                             const NotationsWriterTupletPortion &inImplied)
{
    if (inPortion.number != api::VALUE_UNSPECIFIED && inPortion.number != inImplied.number)
    {
        return false;
    }
    if (inPortion.durationName != api::DurationName::unspecified && inPortion.durationName != inImplied.durationName)
    {
        return false;
    }
    if (inPortion.dots != api::VALUE_UNSPECIFIED && inPortion.dots != inImplied.dots)
    {
        return false;
    }
    return true;
}

// NoteWriter reads 1:1 as "this note has no time modification" and leaves the element out, so a
// portion on such a note has nothing to repeat and is always written.
bool notationsWriterNoteHasTimeModification(const api::DurationData &inDuration)
{
    return inDuration.timeModificationActualNotes > 0 && inDuration.timeModificationNormalNotes > 0 &&
           (inDuration.timeModificationActualNotes > 1 || inDuration.timeModificationNormalNotes > 1);
}

// Decides whether one portion reaches the file. The fidelity fields force the answer; left
// unspecified, a portion is written when it says something the note does not already say.
bool notationsWriterWriteTupletPortion(api::Bool inFidelity, const NotationsWriterTupletPortion &inPortion,
                                       const NotationsWriterTupletPortion &inImplied, bool inNoteHasTimeModification)
{
    if (inFidelity == api::Bool::yes)
    {
        return true;
    }
    if (inFidelity == api::Bool::no)
    {
        return false;
    }
    if (notationsWriterTupletPortionIsEmpty(inPortion))
    {
        return false;
    }
    if (!inNoteHasTimeModification)
    {
        return true;
    }
    return !notationsWriterTupletPortionIsRedundant(inPortion, inImplied);
}

core::TupletPortion notationsWriterMakeTupletPortion(const NotationsWriterTupletPortion &inPortion,
                                                     const Converter &inConverter)
{
    core::TupletPortion outPortion;

    if (inPortion.number != api::VALUE_UNSPECIFIED)
    {
        core::TupletNumber tupletNumber;
        tupletNumber.setValue(inPortion.number);
        outPortion.setTupletNumber(tupletNumber);
    }

    if (inPortion.durationName != api::DurationName::unspecified)
    {
        core::TupletType tupletType;
        tupletType.setValue(inConverter.convert(inPortion.durationName));
        outPortion.setTupletType(tupletType);
    }

    for (int dot = 0; dot < inPortion.dots; ++dot)
    {
        outPortion.addTupletDot(core::TupletDot{});
    }

    return outPortion;
}

// The two show-* attributes each cover a pair of api fields, one for the actual side of the
// ratio and one for the normal side.
std::optional<core::ShowTuplet> notationsWriterShowTuplet(api::Bool inShowActual, api::Bool inShowNormal)
{
    if (inShowActual == api::Bool::unspecified)
    {
        return std::nullopt;
    }
    if (inShowActual == api::Bool::no)
    {
        return core::ShowTuplet::none();
    }
    if (inShowNormal == api::Bool::yes)
    {
        return core::ShowTuplet::both();
    }
    return core::ShowTuplet::actual();
}

NotationsWriter::NotationsWriter(const api::NoteData &inNoteData, const MeasureCursor &inCursor,
                                 const ScoreWriter &inScoreWriter)
    : myNoteData{inNoteData}, myCursor{inCursor}, myScoreWriter{inScoreWriter}, myConverter{}
{
}

core::Notations NotationsWriter::getNotations() const
{
    core::Notations outNotations;
    core::Articulations articulations;
    core::Ornaments ornaments;
    core::Technical technicals;

    const auto &spannerResolver = myScoreWriter.getSpannerResolver();

    for (const auto &curve : myNoteData.noteAttachmentData.curveStops)
    {
        if (curve.curveType != api::CurveType::tie && curve.curveType != api::CurveType::slur)
        {
            myScoreWriter.getDiagnostics().report(api::Severity::error, api::DiagnosticCode::droppedData,
                                                  cursorLocation(myCursor),
                                                  "a curve with an unspecified type is not written");
            continue;
        }
        const auto resolvedNumber = spannerResolver.emittedNumber(curve.number, &curve);
        if (curve.curveType == api::CurveType::tie)
        {
            core::Tied tied;
            writeAttributesFromCurveStop(curve, tied, resolvedNumber, myScoreWriter.getDiagnostics(),
                                         cursorLocation(myCursor));
            outNotations.addChoice(core::NotationsChoice::tied(tied));
        }
        else if (curve.curveType == api::CurveType::slur)
        {
            core::Slur slur;
            writeAttributesFromCurveStop(curve, slur, resolvedNumber, myScoreWriter.getDiagnostics(),
                                         cursorLocation(myCursor));
            outNotations.addChoice(core::NotationsChoice::slur(slur));
        }
    }

    for (const auto &curve : myNoteData.noteAttachmentData.curveContinuations)
    {
        if (curve.curveType != api::CurveType::tie && curve.curveType != api::CurveType::slur)
        {
            myScoreWriter.getDiagnostics().report(api::Severity::error, api::DiagnosticCode::droppedData,
                                                  cursorLocation(myCursor),
                                                  "a curve with an unspecified type is not written");
            continue;
        }
        const auto resolvedNumber = spannerResolver.emittedNumber(curve.number, &curve);
        if (curve.curveType == api::CurveType::tie)
        {
            core::Tied tied;
            writeAttributesFromCurveContinue(curve, tied, resolvedNumber, myScoreWriter.getDiagnostics(),
                                             cursorLocation(myCursor));
            outNotations.addChoice(core::NotationsChoice::tied(tied));
        }
        else if (curve.curveType == api::CurveType::slur)
        {
            core::Slur slur;
            writeAttributesFromCurveContinue(curve, slur, resolvedNumber, myScoreWriter.getDiagnostics(),
                                             cursorLocation(myCursor));
            outNotations.addChoice(core::NotationsChoice::slur(slur));
        }
    }

    for (const auto &curve : myNoteData.noteAttachmentData.curveStarts)
    {
        if (curve.curveType != api::CurveType::tie && curve.curveType != api::CurveType::slur)
        {
            myScoreWriter.getDiagnostics().report(api::Severity::error, api::DiagnosticCode::droppedData,
                                                  cursorLocation(myCursor),
                                                  "a curve with an unspecified type is not written");
            continue;
        }
        const auto resolvedNumber = spannerResolver.emittedNumber(curve.number, &curve);
        if (curve.curveType == api::CurveType::tie)
        {
            core::Tied tied;
            writeAttributesFromCurveStart(curve, tied, resolvedNumber, myScoreWriter.getDiagnostics(),
                                          cursorLocation(myCursor));
            outNotations.addChoice(core::NotationsChoice::tied(tied));
        }
        else if (curve.curveType == api::CurveType::slur)
        {
            core::Slur slur;
            writeAttributesFromCurveStart(curve, slur, resolvedNumber, myScoreWriter.getDiagnostics(),
                                          cursorLocation(myCursor));
            outNotations.addChoice(core::NotationsChoice::slur(slur));
        }
    }

    // A laissez-vibrer / let-ring tie is a lone <tied type="let-ring"> with no
    // start/stop pairing, so it is emitted from its own field rather than the
    // curve vectors above.
    if (myNoteData.tieLetRing.has_value())
    {
        core::Tied tied;
        writeAttributesFromTieLetRing(*myNoteData.tieLetRing, tied, myScoreWriter.getDiagnostics(),
                                      cursorLocation(myCursor));
        outNotations.addChoice(core::NotationsChoice::tied(tied));
    }

    addGlissandoAndSlide(outNotations);

    // A tuplet contained in a single note -- an inner tuplet covering exactly one note of its
    // outer tuplet, or Finale's export of a one-note tuplet -- has its start and its stop on
    // that one note, and the start must be written first (#429). So starts are written before
    // stops, each start followed directly by the same-note stop that carries the same number
    // when there is one, and the remaining stops (closing tuplets begun on earlier notes)
    // follow. Matching numbers on one note always mean a single-note tuplet: two different
    // tuplets can never share a note, because a note carries only one time-modification.
    const auto &tupletStops = myNoteData.noteAttachmentData.tupletStops;
    std::vector<bool> tupletStopWritten(tupletStops.size(), false);

    const auto &duration = myNoteData.durationData;
    const bool noteHasTimeModification = notationsWriterNoteHasTimeModification(duration);

    // What the note's time modification already says about each side of the ratio. An absent
    // <normal-type> means the normal notes carry the note's own written value (#428).
    const NotationsWriterTupletPortion impliedActual{duration.timeModificationActualNotes, duration.durationName,
                                                     duration.durationDots};
    const bool hasNormalType = duration.timeModificationNormalType != api::DurationName::unspecified;
    const NotationsWriterTupletPortion impliedNormal{
        duration.timeModificationNormalNotes,
        hasNormalType ? duration.timeModificationNormalType : duration.durationName,
        hasNormalType ? duration.timeModificationNormalTypeDots : duration.durationDots};

    for (const auto &tupletStart : myNoteData.noteAttachmentData.tupletStarts)
    {
        core::Tuplet tuplet;
        tuplet.setType(core::StartStop::start());
        setAttributesFromPositionData(tupletStart.positionData, tuplet);
        setId(tupletStart.id, tuplet, myScoreWriter.getDiagnostics(), cursorLocation(myCursor));

        const NotationsWriterTupletPortion actual{tupletStart.actualNumber, tupletStart.actualDurationName,
                                                  tupletStart.actualDots};
        if (notationsWriterWriteTupletPortion(tupletStart.writeActual, actual, impliedActual, noteHasTimeModification))
        {
            tuplet.setTupletActual(notationsWriterMakeTupletPortion(actual, myConverter));
        }

        const NotationsWriterTupletPortion normal{tupletStart.normalNumber, tupletStart.normalDurationName,
                                                  tupletStart.normalDots};
        if (notationsWriterWriteTupletPortion(tupletStart.writeNormal, normal, impliedNormal, noteHasTimeModification))
        {
            tuplet.setTupletNormal(notationsWriterMakeTupletPortion(normal, myConverter));
        }

        const auto resolvedNumber = spannerResolver.emittedNumber(tupletStart.number, &tupletStart);
        if (resolvedNumber.has_value())
        {
            tuplet.setNumber(core::NumberLevel{*resolvedNumber});
        }

        if (tupletStart.bracket != api::Bool::unspecified)
        {
            tuplet.setBracket(myConverter.convert(tupletStart.bracket));
        }

        if (tupletStart.lineShape != api::TupletLineShape::unspecified)
        {
            tuplet.setLineShape(myConverter.convert(tupletStart.lineShape));
        }

        tuplet.setShowNumber(notationsWriterShowTuplet(tupletStart.showActualNumber, tupletStart.showNormalNumber));
        tuplet.setShowType(notationsWriterShowTuplet(tupletStart.showActualType, tupletStart.showNormalType));

        outNotations.addChoice(core::NotationsChoice::tuplet(tuplet));

        for (std::size_t stopIndex = 0; stopIndex < tupletStops.size(); ++stopIndex)
        {
            if (!tupletStopWritten[stopIndex] && tupletStops[stopIndex].number == tupletStart.number)
            {
                const auto &tupletStop = tupletStops[stopIndex];
                outNotations.addChoice(notationsWriterMakeTupletStop(
                    tupletStop, spannerResolver.emittedNumber(tupletStop.number, &tupletStop), myConverter,
                    myScoreWriter.getDiagnostics(), cursorLocation(myCursor)));
                tupletStopWritten[stopIndex] = true;
                break;
            }
        }
    }

    for (std::size_t stopIndex = 0; stopIndex < tupletStops.size(); ++stopIndex)
    {
        if (!tupletStopWritten[stopIndex])
        {
            const auto &tupletStop = tupletStops[stopIndex];
            outNotations.addChoice(
                notationsWriterMakeTupletStop(tupletStop, spannerResolver.emittedNumber(tupletStop.number, &tupletStop),
                                              myConverter, myScoreWriter.getDiagnostics(), cursorLocation(myCursor)));
        }
    }

    // Wavy lines live inside <ornaments>, alongside trill-mark/shake/etc. Stops and continues are
    // emitted before the mark-derived ornaments below (stop-before-start, as with glissando/slide
    // above); starts are emitted after, matching the common <trill-mark/><wavy-line type="start"/>
    // shape real files use. A wavy line that starts and stops on the same note is the exception:
    // its stop is held back and written right after its start (#429).
    addWavyLineStopsAndContinues(ornaments);

    for (const auto &mark : myNoteData.noteAttachmentData.marks)
    {
        if (isMarkArticulation(mark.markType) || isMarkCustom(mark.markType))
        {
            this->addArticulation(mark, articulations);
        }
        if (isMarkOrnament(mark.markType))
        {
            this->addOrnament(mark, ornaments);
        }
        if (isMarkAccidentalMark(mark.markType))
        {
            this->addAccidentalMark(mark, outNotations);
        }
        if (isMarkTechnical(mark.markType))
        {
            this->addTechnical(mark, technicals);
        }
        else if (isMarkDynamic(mark.markType))
        {
            DynamicsWriter dynamicsWriter{mark, myCursor, myScoreWriter.getDiagnostics()};
            outNotations.addChoice(core::NotationsChoice::dynamics(dynamicsWriter.getDynamics()));
        }
        else if (isMarkFermata(mark.markType))
        {
            core::Fermata fermata;
            impl::setAttributesFromMarkData(mark, fermata);

            if (mark.markType == api::MarkType::fermata)
            {
                fermata.setValue(core::FermataShape::empty());
            }
            else if (mark.markType == api::MarkType::fermataNormal)
            {
                fermata.setValue(core::FermataShape::normal());
            }
            else if (mark.markType == api::MarkType::fermataAngled)
            {
                fermata.setValue(core::FermataShape::angled());
            }
            else if (mark.markType == api::MarkType::fermataSquare)
            {
                fermata.setValue(core::FermataShape::square());
            }
            else if (mark.markType == api::MarkType::fermataDoubleAngled)
            {
                fermata.setValue(core::FermataShape::doubleAngled());
            }
            else if (mark.markType == api::MarkType::fermataDoubleSquare)
            {
                fermata.setValue(core::FermataShape::doubleSquare());
            }
            else if (mark.markType == api::MarkType::fermataDoubleDot)
            {
                fermata.setValue(core::FermataShape::doubleDot());
            }
            else if (mark.markType == api::MarkType::fermataHalfCurve)
            {
                fermata.setValue(core::FermataShape::halfCurve());
            }
            else if (mark.markType == api::MarkType::fermataCurlew)
            {
                fermata.setValue(core::FermataShape::curlew());
            }
            else if (mark.markType == api::MarkType::fermataUpright)
            {
                fermata.setValue(core::FermataShape::empty());
                fermata.setType(core::UprightInverted::upright());
            }
            else if (mark.markType == api::MarkType::fermataNormalUpright)
            {
                fermata.setValue(core::FermataShape::normal());
                fermata.setType(core::UprightInverted::upright());
            }
            else if (mark.markType == api::MarkType::fermataAngledUpright)
            {
                fermata.setValue(core::FermataShape::angled());
                fermata.setType(core::UprightInverted::upright());
            }
            else if (mark.markType == api::MarkType::fermataSquareUpright)
            {
                fermata.setValue(core::FermataShape::square());
                fermata.setType(core::UprightInverted::upright());
            }
            else if (mark.markType == api::MarkType::fermataInverted)
            {
                fermata.setValue(core::FermataShape::empty());
                fermata.setType(core::UprightInverted::inverted());
            }
            else if (mark.markType == api::MarkType::fermataNormalInverted)
            {
                fermata.setValue(core::FermataShape::normal());
                fermata.setType(core::UprightInverted::inverted());
            }
            else if (mark.markType == api::MarkType::fermataAngledInverted)
            {
                fermata.setValue(core::FermataShape::angled());
                fermata.setType(core::UprightInverted::inverted());
            }
            else if (mark.markType == api::MarkType::fermataSquareInverted)
            {
                fermata.setValue(core::FermataShape::square());
                fermata.setType(core::UprightInverted::inverted());
            }

            outNotations.addChoice(core::NotationsChoice::fermata(fermata));
        }
        else if (isMarkNonArpeggiate(mark.markType))
        {
            core::NonArpeggiate nonArpeggiate;
            impl::setAttributesFromMarkData(mark, nonArpeggiate);

            const auto nonArpeggiateData = mark.choice.nonArpeggiate();
            nonArpeggiate.setType(nonArpeggiateData.placement == api::NonArpeggiatePlacement::bottom
                                      ? core::TopBottom::bottom()
                                      : core::TopBottom::top());
            if (nonArpeggiateData.number.has_value())
            {
                const core::NumberLevel numberLevel{*nonArpeggiateData.number};
                reportAdjusted(myScoreWriter.getDiagnostics(), cursorLocation(myCursor), "non-arpeggiate number",
                               *nonArpeggiateData.number, numberLevel.value());
                nonArpeggiate.setNumber(numberLevel);
            }
            setId(nonArpeggiateData.id, nonArpeggiate, myScoreWriter.getDiagnostics(), cursorLocation(myCursor));

            outNotations.addChoice(core::NotationsChoice::nonArpeggiate(nonArpeggiate));
        }
        else if (isMarkArpeggiate(mark.markType))
        {
            core::Arpeggiate arpeggiate;
            impl::setAttributesFromMarkData(mark, arpeggiate);

            if (mark.markType == api::MarkType::arpeggiate)
            {
                // TODO: fixme - MusicXML 4.0 adds UpDownNone with 'none' value
            }
            else if (mark.markType == api::MarkType::arpeggiateUp)
            {
                arpeggiate.setDirection(core::UpDown::up());
            }
            else if (mark.markType == api::MarkType::arpeggiateDown)
            {
                arpeggiate.setDirection(core::UpDown::down());
            }

            const auto arpeggiateData = mark.choice.arpeggiate();
            if (arpeggiateData.number.has_value())
            {
                const core::NumberLevel numberLevel{*arpeggiateData.number};
                reportAdjusted(myScoreWriter.getDiagnostics(), cursorLocation(myCursor), "arpeggiate number",
                               *arpeggiateData.number, numberLevel.value());
                arpeggiate.setNumber(numberLevel);
            }
            if (arpeggiateData.unbroken != api::Bool::unspecified)
            {
                Converter converter;
                arpeggiate.setUnbroken(converter.convert(arpeggiateData.unbroken));
            }
            setId(arpeggiateData.id, arpeggiate, myScoreWriter.getDiagnostics(), cursorLocation(myCursor));

            outNotations.addChoice(core::NotationsChoice::arpeggiate(arpeggiate));
        }
        else if (isMarkOtherNotation(mark.markType))
        {
            core::OtherNotation other;
            impl::setAttributesFromMarkData(mark, other);
            other.setValue(mark.name);

            const auto payload = mark.choice.otherNotation();
            other.setType(myConverter.convert(payload.type));
            if (payload.number.has_value())
            {
                const core::NumberLevel numberLevel{*payload.number};
                reportAdjusted(myScoreWriter.getDiagnostics(), cursorLocation(myCursor), "other-notation number",
                               *payload.number, numberLevel.value());
                other.setNumber(numberLevel);
            }
            if (payload.smufl.has_value())
            {
                other.setSmufl(core::SmuflGlyphName{*payload.smufl});
            }
            setId(payload.id, other, myScoreWriter.getDiagnostics(), cursorLocation(myCursor));

            outNotations.addChoice(core::NotationsChoice::otherNotation(other));
        }
        else if (isMarkPedal(mark.markType))
        {
            myScoreWriter.getDiagnostics().report(api::Severity::error, api::DiagnosticCode::droppedData,
                                                  cursorLocation(myCursor),
                                                  "a pedal mark on a note is not written; write it in a direction");
        }
    }

    addWavyLineStarts(ornaments);

    if (!articulations.choice().empty())
    {
        outNotations.addChoice(core::NotationsChoice::articulations(articulations));
    }

    if (!ornaments.group().empty())
    {
        outNotations.addChoice(core::NotationsChoice::ornaments(ornaments));
    }

    if (!technicals.choice().empty())
    {
        outNotations.addChoice(core::NotationsChoice::technical(technicals));
    }

    return outNotations;
}

void NotationsWriter::addGlissandoAndSlide(core::Notations &outNotations) const
{
    const auto &spannerResolver = myScoreWriter.getSpannerResolver();

    // Glissando and slide are top-level <notations> children, like slur/tie. Stops are emitted
    // before starts so a chain of glissandi on one note keeps score order (see #139). The
    // exception is a glissando or slide that starts and stops on this same note, as when a line
    // is drawn from a note toward a rest: its stop is held back and written right after its
    // start (#429). The SpannerResolver decides which stops those are -- a chained note and a
    // single-note span can carry the same number, and only the stream of earlier events can
    // tell them apart.
    for (const auto &glissandoStop : myNoteData.noteAttachmentData.glissandoStops)
    {
        if (spannerResolver.sameNoteSpanPartner(&glissandoStop) != nullptr)
        {
            continue;
        }
        addGlissandoStop(glissandoStop, outNotations);
    }

    for (const auto &glissandoStart : myNoteData.noteAttachmentData.glissandoStarts)
    {
        const auto resolvedNumber = spannerResolver.emittedNumber(glissandoStart.number, &glissandoStart);
        if (glissandoStart.glissandoType == api::GlissandoType::slide)
        {
            core::Slide slide;
            writeAttributesFromGlissandoStart(glissandoStart, slide, resolvedNumber, myScoreWriter.getDiagnostics(),
                                              cursorLocation(myCursor));
            outNotations.addChoice(core::NotationsChoice::slide(slide));
        }
        else
        {
            core::Glissando glissando;
            writeAttributesFromGlissandoStart(glissandoStart, glissando, resolvedNumber, myScoreWriter.getDiagnostics(),
                                              cursorLocation(myCursor));
            outNotations.addChoice(core::NotationsChoice::glissando(glissando));
        }

        const void *sameNoteStop = spannerResolver.sameNoteSpanPartner(&glissandoStart);
        if (sameNoteStop != nullptr)
        {
            for (const auto &glissandoStop : myNoteData.noteAttachmentData.glissandoStops)
            {
                if (&glissandoStop == sameNoteStop)
                {
                    addGlissandoStop(glissandoStop, outNotations);
                }
            }
        }
    }
}

void NotationsWriter::addGlissandoStop(const api::GlissandoStop &inGlissandoStop, core::Notations &outNotations) const
{
    const auto &spannerResolver = myScoreWriter.getSpannerResolver();
    const auto resolvedNumber = spannerResolver.emittedNumber(inGlissandoStop.number, &inGlissandoStop);

    if (inGlissandoStop.glissandoType == api::GlissandoType::slide)
    {
        core::Slide slide;
        writeAttributesFromGlissandoStop(inGlissandoStop, slide, resolvedNumber, myScoreWriter.getDiagnostics(),
                                         cursorLocation(myCursor));
        outNotations.addChoice(core::NotationsChoice::slide(slide));
    }
    else
    {
        core::Glissando glissando;
        writeAttributesFromGlissandoStop(inGlissandoStop, glissando, resolvedNumber, myScoreWriter.getDiagnostics(),
                                         cursorLocation(myCursor));
        outNotations.addChoice(core::NotationsChoice::glissando(glissando));
    }
}

void NotationsWriter::addWavyLineStopsAndContinues(core::Ornaments &outOrnaments) const
{
    const auto &spannerResolver = myScoreWriter.getSpannerResolver();

    // A stop whose wavy line starts on this same note is skipped here; addWavyLineStarts writes
    // it right after its start (#429).
    for (const auto &wavyLineStop : myNoteData.noteAttachmentData.wavyLineStops)
    {
        if (spannerResolver.sameNoteSpanPartner(&wavyLineStop) != nullptr)
        {
            continue;
        }
        const auto resolvedNumber = spannerResolver.emittedNumber(wavyLineStop.number, &wavyLineStop);
        core::OrnamentsGroup group;
        group.setChoice(core::OrnamentsGroupChoice::wavyLine(writeWavyLineStop(wavyLineStop, resolvedNumber)));
        outOrnaments.addGroup(group);
    }

    for (const auto &wavyLineContinue : myNoteData.noteAttachmentData.wavyLineContinuations)
    {
        const auto resolvedNumber = spannerResolver.emittedNumber(wavyLineContinue.number, &wavyLineContinue);
        core::OrnamentsGroup group;
        group.setChoice(core::OrnamentsGroupChoice::wavyLine(writeWavyLineContinue(wavyLineContinue, resolvedNumber)));
        outOrnaments.addGroup(group);
    }
}

void NotationsWriter::addWavyLineStarts(core::Ornaments &outOrnaments) const
{
    const auto &spannerResolver = myScoreWriter.getSpannerResolver();

    for (const auto &wavyLineStart : myNoteData.noteAttachmentData.wavyLineStarts)
    {
        const auto resolvedNumber = spannerResolver.emittedNumber(wavyLineStart.number, &wavyLineStart);
        core::OrnamentsGroup group;
        group.setChoice(core::OrnamentsGroupChoice::wavyLine(writeWavyLineStart(wavyLineStart, resolvedNumber)));
        outOrnaments.addGroup(group);

        // A wavy line that starts and stops on this same note gets its stop here, right after
        // the start (#429).
        const void *sameNoteStop = spannerResolver.sameNoteSpanPartner(&wavyLineStart);
        if (sameNoteStop != nullptr)
        {
            for (const auto &wavyLineStop : myNoteData.noteAttachmentData.wavyLineStops)
            {
                if (&wavyLineStop == sameNoteStop)
                {
                    const auto stopNumber = spannerResolver.emittedNumber(wavyLineStop.number, &wavyLineStop);
                    core::OrnamentsGroup stopGroup;
                    stopGroup.setChoice(
                        core::OrnamentsGroupChoice::wavyLine(writeWavyLineStop(wavyLineStop, stopNumber)));
                    outOrnaments.addGroup(stopGroup);
                }
            }
        }
    }
}

core::NotationsChoice NotationsWriter::makeArticulationsNotationsChoice() const
{
    return core::NotationsChoice::articulations(core::Articulations{});
}

core::NotationsChoice NotationsWriter::makeOrnamentsNotationsChoice() const
{
    return core::NotationsChoice::ornaments(core::Ornaments{});
}

core::NotationsChoice NotationsWriter::makeTechnicalNotationsChoice() const
{
    return core::NotationsChoice::technical(core::Technical{});
}

void NotationsWriter::addArticulation(const api::MarkData &mark, core::Articulations &outArticulations) const
{
    if (!api::isMarkArticulation(mark.markType) && !api::isMarkCustom(mark.markType))
    {
        return;
    }

    const auto kind = myConverter.convertArticulation(mark.markType);

    switch (kind)
    {
    case core::ArticulationsChoice::Kind::accent: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::accent(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::strongAccent: {
        core::StrongAccent sa;
        setAttributesFromPositionData(mark.positionData, sa);
        if (mark.markType == api::MarkType::strongAccentUp)
        {
            sa.setType(core::UpDown::up());
        }
        else if (mark.markType == api::MarkType::strongAccentDown)
        {
            sa.setType(core::UpDown::down());
        }
        outArticulations.addChoice(core::ArticulationsChoice::strongAccent(sa));
        break;
    }
    case core::ArticulationsChoice::Kind::staccato: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::staccato(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::tenuto: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::tenuto(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::detachedLegato: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::detachedLegato(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::staccatissimo: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::staccatissimo(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::spiccato: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::spiccato(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::scoop: {
        core::EmptyLine el;
        setAttributesFromPositionData(mark.positionData, el);
        outArticulations.addChoice(core::ArticulationsChoice::scoop(el));
        break;
    }
    case core::ArticulationsChoice::Kind::plop: {
        core::EmptyLine el;
        setAttributesFromPositionData(mark.positionData, el);
        outArticulations.addChoice(core::ArticulationsChoice::plop(el));
        break;
    }
    case core::ArticulationsChoice::Kind::doit: {
        core::EmptyLine el;
        setAttributesFromPositionData(mark.positionData, el);
        outArticulations.addChoice(core::ArticulationsChoice::doit(el));
        break;
    }
    case core::ArticulationsChoice::Kind::falloff: {
        core::EmptyLine el;
        setAttributesFromPositionData(mark.positionData, el);
        outArticulations.addChoice(core::ArticulationsChoice::falloff(el));
        break;
    }
    case core::ArticulationsChoice::Kind::breathMark: {
        core::BreathMark bm;
        setAttributesFromPositionData(mark.positionData, bm);
        outArticulations.addChoice(core::ArticulationsChoice::breathMark(bm));
        break;
    }
    case core::ArticulationsChoice::Kind::caesura: {
        core::Caesura c;
        setAttributesFromPositionData(mark.positionData, c);
        // MarkType::caesura is the common empty form <caesura/>; the variants carry an
        // explicit text value.
        auto caesuraValue = core::CaesuraValue::empty();
        if (mark.markType == api::MarkType::caesuraNormal)
        {
            caesuraValue = core::CaesuraValue::normal();
        }
        else if (mark.markType == api::MarkType::caesuraThick)
        {
            caesuraValue = core::CaesuraValue::thick();
        }
        else if (mark.markType == api::MarkType::caesuraShort)
        {
            caesuraValue = core::CaesuraValue::short_();
        }
        else if (mark.markType == api::MarkType::caesuraCurved)
        {
            caesuraValue = core::CaesuraValue::curved();
        }
        else if (mark.markType == api::MarkType::caesuraSingle)
        {
            caesuraValue = core::CaesuraValue::single();
        }
        c.setValue(caesuraValue);
        outArticulations.addChoice(core::ArticulationsChoice::caesura(c));
        break;
    }
    case core::ArticulationsChoice::Kind::stress: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::stress(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::unstress: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::unstress(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::softAccent: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outArticulations.addChoice(core::ArticulationsChoice::softAccent(ep));
        break;
    }
    case core::ArticulationsChoice::Kind::otherArticulation: {
        core::OtherPlacementText opt;
        setAttributesFromPositionData(mark.positionData, opt);
        setAttributesFromPrintData(mark.printData, opt);
        if (api::isMarkCustom(mark.markType))
        {
            opt.setValue(api::getCustomMarkName(mark.markType));
        }
        else
        {
            opt.setValue(mark.name);
        }
        if (mark.choice.otherMark().smufl.has_value())
        {
            opt.setSmufl(core::SmuflGlyphName{*mark.choice.otherMark().smufl});
        }
        outArticulations.addChoice(core::ArticulationsChoice::otherArticulation(opt));
        break;
    }
    default:
        break;
    }
}

void NotationsWriter::addOrnament(const api::MarkData &mark, core::Ornaments &outOrnaments) const
{
    if (!isMarkOrnament(mark.markType))
    {
        return;
    }

    const auto kind = myConverter.convertOrnament(mark.markType);
    core::OrnamentsGroup group;

    switch (kind)
    {
    case core::OrnamentsGroupChoice::Kind::trillMark: {
        core::EmptyTrillSound ets;
        setAttributesFromPositionData(mark.positionData, ets);
        group.setChoice(core::OrnamentsGroupChoice::trillMark(ets));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::turn: {
        core::HorizontalTurn ht;
        setAttributesFromPositionData(mark.positionData, ht);
        group.setChoice(core::OrnamentsGroupChoice::turn(ht));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::delayedTurn: {
        core::HorizontalTurn ht;
        setAttributesFromPositionData(mark.positionData, ht);
        group.setChoice(core::OrnamentsGroupChoice::delayedTurn(ht));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::invertedTurn: {
        core::HorizontalTurn ht;
        setAttributesFromPositionData(mark.positionData, ht);
        group.setChoice(core::OrnamentsGroupChoice::invertedTurn(ht));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::delayedInvertedTurn: {
        core::HorizontalTurn ht;
        setAttributesFromPositionData(mark.positionData, ht);
        group.setChoice(core::OrnamentsGroupChoice::delayedInvertedTurn(ht));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::verticalTurn: {
        core::EmptyTrillSound ets;
        setAttributesFromPositionData(mark.positionData, ets);
        group.setChoice(core::OrnamentsGroupChoice::verticalTurn(ets));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::invertedVerticalTurn: {
        core::EmptyTrillSound ets;
        setAttributesFromPositionData(mark.positionData, ets);
        group.setChoice(core::OrnamentsGroupChoice::invertedVerticalTurn(ets));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::shake: {
        core::EmptyTrillSound ets;
        setAttributesFromPositionData(mark.positionData, ets);
        group.setChoice(core::OrnamentsGroupChoice::shake(ets));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::mordent: {
        core::Mordent m;
        setAttributesFromPositionData(mark.positionData, m);
        notationsWriterSetMordentSpecificAttributes(mark, m);
        group.setChoice(core::OrnamentsGroupChoice::mordent(m));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::invertedMordent: {
        core::Mordent m;
        setAttributesFromPositionData(mark.positionData, m);
        notationsWriterSetMordentSpecificAttributes(mark, m);
        group.setChoice(core::OrnamentsGroupChoice::invertedMordent(m));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::schleifer: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        group.setChoice(core::OrnamentsGroupChoice::schleifer(ep));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::tremolo: {
        core::Tremolo tremolo;
        setAttributesFromPositionData(mark.positionData, tremolo);
        const auto tremoloData = mark.choice.isTremolo() ? mark.choice.tremolo() : api::TremoloMarkData{};

        if (mark.markType == api::MarkType::tremoloStart || mark.markType == api::MarkType::tremoloStop)
        {
            tremolo.setType(mark.markType == api::MarkType::tremoloStart ? core::TremoloType::start()
                                                                         : core::TremoloType::stop());
            tremolo.setValue(core::TremoloMarks{tremoloData.tremoloMarks.value_or(3)});
        }
        else if (mark.markType == api::MarkType::tremoloUnmeasured)
        {
            tremolo.setType(core::TremoloType::unmeasured());
            tremolo.setValue(core::TremoloMarks{0});
        }
        else
        {
            tremolo.setType(core::TremoloType::single());
            tremolo.setValue(core::TremoloMarks{api::numTremoloSlashes(mark.markType)});
        }

        if (tremoloData.smufl.has_value() && !tremoloData.smufl->empty())
        {
            tremolo.setSmufl(core::SmuflGlyphName{*tremoloData.smufl});
        }

        group.setChoice(core::OrnamentsGroupChoice::tremolo(tremolo));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::haydn: {
        core::EmptyTrillSound ets;
        setAttributesFromPositionData(mark.positionData, ets);
        group.setChoice(core::OrnamentsGroupChoice::haydn(ets));
        break;
    }
    case core::OrnamentsGroupChoice::Kind::otherOrnament: {
        core::OtherPlacementText opt;
        setAttributesFromPositionData(mark.positionData, opt);
        setAttributesFromPrintData(mark.printData, opt);
        if (!mark.name.empty())
        {
            opt.setValue(mark.name);
        }
        if (mark.choice.otherMark().smufl.has_value())
        {
            opt.setSmufl(core::SmuflGlyphName{*mark.choice.otherMark().smufl});
        }
        group.setChoice(core::OrnamentsGroupChoice::otherOrnament(opt));
        break;
    }
    default:
        return;
    }

    outOrnaments.addGroup(group);
}

// An accidental mark is written at the notations level. MusicXML also allows one inside
// <ornaments>, next to the ornament it decorates, but the api does not record which of the two
// places a mark came from -- both arrive here as the same MarkType -- and an <ornaments> group is
// required to carry an ornament, so putting one there would mean inventing an ornament. Marks
// from either place are written here instead.
void NotationsWriter::addAccidentalMark(const api::MarkData &mark, core::Notations &outNotations) const
{
    if (!isMarkAccidentalMark(mark.markType))
    {
        return;
    }

    core::AccidentalMark accidentalMark;
    accidentalMark.setValue(myConverter.convertAccidentalMark(mark.markType));
    setAttributesFromPositionData(mark.positionData, accidentalMark);
    outNotations.addChoice(core::NotationsChoice::accidentalMark(accidentalMark));
}

void NotationsWriter::addTechnical(const api::MarkData &mark, core::Technical &outTechnical) const
{
    if (!isMarkTechnical(mark.markType))
    {
        return;
    }

    const auto kind = myConverter.convertTechnicalMark(mark.markType);

    switch (kind)
    {
    case core::TechnicalChoice::Kind::upBow: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::upBow(ep));
        break;
    }
    case core::TechnicalChoice::Kind::downBow: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::downBow(ep));
        break;
    }
    case core::TechnicalChoice::Kind::harmonic: {
        core::Harmonic h;
        impl::setAttributesFromMarkData(mark, h);
        const auto harmonicData = mark.choice.isHarmonic() ? mark.choice.harmonic() : api::HarmonicMarkData{};

        switch (harmonicData.kind)
        {
        case api::HarmonicKind::natural:
            h.setChoice(core::HarmonicChoice::natural(core::Empty{}));
            break;
        case api::HarmonicKind::artificial:
            h.setChoice(core::HarmonicChoice::artificial(core::Empty{}));
            break;
        case api::HarmonicKind::unspecified:
            break;
        }

        switch (harmonicData.pitch)
        {
        case api::HarmonicPitch::basePitch:
            h.setChoice2(core::HarmonicChoice2::basePitch(core::Empty{}));
            break;
        case api::HarmonicPitch::touchingPitch:
            h.setChoice2(core::HarmonicChoice2::touchingPitch(core::Empty{}));
            break;
        case api::HarmonicPitch::soundingPitch:
            h.setChoice2(core::HarmonicChoice2::soundingPitch(core::Empty{}));
            break;
        case api::HarmonicPitch::unspecified:
            break;
        }

        outTechnical.addChoice(core::TechnicalChoice::harmonic(h));
        break;
    }
    case core::TechnicalChoice::Kind::openString: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::openString(ep));
        break;
    }
    case core::TechnicalChoice::Kind::thumbPosition: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::thumbPosition(ep));
        break;
    }
    case core::TechnicalChoice::Kind::doubleTongue: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::doubleTongue(ep));
        break;
    }
    case core::TechnicalChoice::Kind::tripleTongue: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::tripleTongue(ep));
        break;
    }
    case core::TechnicalChoice::Kind::stopped: {
        core::EmptyPlacementSmufl eps;
        setAttributesFromPositionData(mark.positionData, eps);
        outTechnical.addChoice(core::TechnicalChoice::stopped(eps));
        break;
    }
    case core::TechnicalChoice::Kind::snapPizzicato: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::snapPizzicato(ep));
        break;
    }
    case core::TechnicalChoice::Kind::fret: {
        core::Fret f;
        int fretValue = 0;
        std::size_t length = 0;
        if (!mark.name.empty())
        {
            try
            {
                fretValue = std::stoi(mark.name, &length);
            }
            catch (...)
            {
            }
        }
        // reported outside the try so a throwing diagnostic handler is not swallowed
        if (length != mark.name.size())
        {
            myScoreWriter.getDiagnostics().report(
                api::Severity::warning, api::DiagnosticCode::invalidValue, cursorLocation(myCursor),
                "fret \"" + mark.name + "\" is not a number; using " + std::to_string(fretValue));
        }
        f.setValue(fretValue);
        outTechnical.addChoice(core::TechnicalChoice::fret(f));
        break;
    }
    case core::TechnicalChoice::Kind::string: {
        core::String s;
        core::ValueParseOutcome outcome = core::ValueParseOutcome::valid;
        const auto stringNumber = core::StringNumber::parse(mark.name, outcome);
        if (outcome != core::ValueParseOutcome::valid)
        {
            myScoreWriter.getDiagnostics().report(
                api::Severity::warning, api::DiagnosticCode::invalidValue, cursorLocation(myCursor),
                "string \"" + mark.name + "\" is not a string number; using " + std::to_string(stringNumber.value()));
        }
        s.setValue(stringNumber);
        outTechnical.addChoice(core::TechnicalChoice::string(s));
        break;
    }
    case core::TechnicalChoice::Kind::fingering: {
        core::Fingering fingering;
        setAttributesFromPositionData(mark.positionData, fingering);
        fingering.setValue(mark.name);
        if (mark.fingeringSubstitution != api::Bool::unspecified)
        {
            fingering.setSubstitution(myConverter.convert(mark.fingeringSubstitution));
        }
        if (mark.fingeringAlternate != api::Bool::unspecified)
        {
            fingering.setAlternate(myConverter.convert(mark.fingeringAlternate));
        }
        outTechnical.addChoice(core::TechnicalChoice::fingering(fingering));
        break;
    }
    case core::TechnicalChoice::Kind::pluck: {
        core::PlacementText pt;
        setAttributesFromPositionData(mark.positionData, pt);
        pt.setValue(mark.name);
        outTechnical.addChoice(core::TechnicalChoice::pluck(pt));
        break;
    }
    case core::TechnicalChoice::Kind::heel: {
        core::HeelToe ht;
        setAttributesFromPositionData(mark.positionData, ht);
        outTechnical.addChoice(core::TechnicalChoice::heel(ht));
        break;
    }
    case core::TechnicalChoice::Kind::toe: {
        core::HeelToe ht;
        setAttributesFromPositionData(mark.positionData, ht);
        outTechnical.addChoice(core::TechnicalChoice::toe(ht));
        break;
    }
    case core::TechnicalChoice::Kind::fingernails: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::fingernails(ep));
        break;
    }
    case core::TechnicalChoice::Kind::hole: {
        core::Hole hole;
        setAttributesFromPositionData(mark.positionData, hole);
        core::HoleClosed holeClosed;
        core::HoleClosedValue closedValue = core::HoleClosedValue::no();
        if (mark.name == "windClosedHole")
            closedValue = core::HoleClosedValue::yes();
        else if (mark.name == "windHalfClosedHole3")
            closedValue = core::HoleClosedValue::half();
        holeClosed.setValue(closedValue);
        hole.setHoleClosed(holeClosed);
        outTechnical.addChoice(core::TechnicalChoice::hole(hole));
        break;
    }
    case core::TechnicalChoice::Kind::arrow: {
        core::Arrow arrow;
        setAttributesFromPositionData(mark.positionData, arrow);
        core::ArrowDirection direction = core::ArrowDirection::up();
        if (mark.name == "arrowOpenLeft")
            direction = core::ArrowDirection::left();
        else if (mark.name == "arrowOpenRight")
            direction = core::ArrowDirection::right();
        else if (mark.name == "arrowOpenDown")
            direction = core::ArrowDirection::down();
        else if (mark.name == "arrowOpenUpLeft")
            direction = core::ArrowDirection::northwest();
        else if (mark.name == "arrowOpenUpRight")
            direction = core::ArrowDirection::northeast();
        else if (mark.name == "arrowOpenDownRight")
            direction = core::ArrowDirection::southeast();
        else if (mark.name == "arrowOpenDownLeft")
            direction = core::ArrowDirection::southwest();
        core::ArrowChoiceGroup acg;
        acg.setArrowDirection(direction);
        arrow.setChoice(core::ArrowChoice::group(acg));
        outTechnical.addChoice(core::TechnicalChoice::arrow(arrow));
        break;
    }
    case core::TechnicalChoice::Kind::handbell: {
        using HB = core::HandbellValue;
        HB value = HB::gyro();
        if (mark.name == "handbellsDamp3")
            value = HB::damp();
        else if (mark.name == "handbellsEcho1")
            value = HB::echo();
        else if (mark.name == "handbellsHandMartellato")
            value = HB::handMartellato();
        else if (mark.name == "handbellsMalletLft")
            value = HB::malletLift();
        else if (mark.name == "handbellsMalletBellOnTable")
            value = HB::malletTable();
        else if (mark.name == "handbellsMartellato")
            value = HB::martellato();
        else if (mark.name == "handbellsMartellatoLift")
            value = HB::martellatoLift();
        else if (mark.name == "handbellsMutedMartellato")
            value = HB::mutedMartellato();
        else if (mark.name == "handbellsPluckLift")
            value = HB::pluckLift();
        else if (mark.name == "handbellsSwing")
            value = HB::swing();
        core::Handbell handbell;
        handbell.setValue(value);
        outTechnical.addChoice(core::TechnicalChoice::handbell(handbell));
        break;
    }
    case core::TechnicalChoice::Kind::brassBend: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::brassBend(ep));
        break;
    }
    case core::TechnicalChoice::Kind::flip: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::flip(ep));
        break;
    }
    case core::TechnicalChoice::Kind::smear: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::smear(ep));
        break;
    }
    case core::TechnicalChoice::Kind::open: {
        core::EmptyPlacementSmufl eps;
        setAttributesFromPositionData(mark.positionData, eps);
        outTechnical.addChoice(core::TechnicalChoice::open(eps));
        break;
    }
    case core::TechnicalChoice::Kind::halfMuted: {
        core::EmptyPlacementSmufl eps;
        setAttributesFromPositionData(mark.positionData, eps);
        outTechnical.addChoice(core::TechnicalChoice::halfMuted(eps));
        break;
    }
    case core::TechnicalChoice::Kind::harmonMute: {
        core::HarmonMute hm;
        outTechnical.addChoice(core::TechnicalChoice::harmonMute(hm));
        break;
    }
    case core::TechnicalChoice::Kind::golpe: {
        core::EmptyPlacement ep;
        setAttributesFromPositionData(mark.positionData, ep);
        outTechnical.addChoice(core::TechnicalChoice::golpe(ep));
        break;
    }
    case core::TechnicalChoice::Kind::otherTechnical: {
        core::OtherPlacementText opt;
        setAttributesFromPositionData(mark.positionData, opt);
        setAttributesFromPrintData(mark.printData, opt);
        if (!mark.name.empty())
        {
            opt.setValue(mark.name);
        }
        if (mark.choice.otherMark().smufl.has_value())
        {
            opt.setSmufl(core::SmuflGlyphName{*mark.choice.otherMark().smufl});
        }
        outTechnical.addChoice(core::TechnicalChoice::otherTechnical(opt));
        break;
    }
    default:
        break;
    }
}
} // namespace impl
} // namespace mx
