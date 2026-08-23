// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/WavyLineFunctions.h"
#include "mx/core/Decimal.h"
#include "mx/core/generated/NumberLevel.h"
#include "mx/core/generated/Percent.h"
#include "mx/core/generated/SmuflWavyLineGlyphName.h"
#include "mx/core/generated/StartNote.h"
#include "mx/core/generated/StartStopContinue.h"
#include "mx/core/generated/TrillBeats.h"
#include "mx/core/generated/TrillStep.h"
#include "mx/core/generated/TwoNoteTurn.h"
#include "mx/core/generated/WavyLine.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/Converter.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/SpannerFunctions.h"

namespace mx
{
namespace impl
{
api::WavyLineStartNote wavyLineFunctionsConvertStartNoteFromCore(core::StartNote inValue)
{
    switch (inValue.tag())
    {
    case core::StartNote::Tag::upper:
        return api::WavyLineStartNote::upper;
    case core::StartNote::Tag::main:
        return api::WavyLineStartNote::main;
    case core::StartNote::Tag::below:
        return api::WavyLineStartNote::below;
    }
    return api::WavyLineStartNote::unspecified;
}

core::StartNote wavyLineFunctionsConvertStartNoteToCore(api::WavyLineStartNote inValue)
{
    switch (inValue)
    {
    case api::WavyLineStartNote::main:
        return core::StartNote::main();
    case api::WavyLineStartNote::below:
        return core::StartNote::below();
    case api::WavyLineStartNote::upper:
    case api::WavyLineStartNote::unspecified:
    default:
        return core::StartNote::upper();
    }
}

api::WavyLineTrillStep wavyLineFunctionsConvertTrillStepFromCore(core::TrillStep inValue)
{
    switch (inValue.tag())
    {
    case core::TrillStep::Tag::whole:
        return api::WavyLineTrillStep::whole;
    case core::TrillStep::Tag::half:
        return api::WavyLineTrillStep::half;
    case core::TrillStep::Tag::unison:
        return api::WavyLineTrillStep::unison;
    }
    return api::WavyLineTrillStep::unspecified;
}

core::TrillStep wavyLineFunctionsConvertTrillStepToCore(api::WavyLineTrillStep inValue)
{
    switch (inValue)
    {
    case api::WavyLineTrillStep::half:
        return core::TrillStep::half();
    case api::WavyLineTrillStep::unison:
        return core::TrillStep::unison();
    case api::WavyLineTrillStep::whole:
    case api::WavyLineTrillStep::unspecified:
    default:
        return core::TrillStep::whole();
    }
}

api::WavyLineTwoNoteTurn wavyLineFunctionsConvertTwoNoteTurnFromCore(core::TwoNoteTurn inValue)
{
    switch (inValue.tag())
    {
    case core::TwoNoteTurn::Tag::whole:
        return api::WavyLineTwoNoteTurn::whole;
    case core::TwoNoteTurn::Tag::half:
        return api::WavyLineTwoNoteTurn::half;
    case core::TwoNoteTurn::Tag::none:
        return api::WavyLineTwoNoteTurn::none;
    }
    return api::WavyLineTwoNoteTurn::unspecified;
}

core::TwoNoteTurn wavyLineFunctionsConvertTwoNoteTurnToCore(api::WavyLineTwoNoteTurn inValue)
{
    switch (inValue)
    {
    case api::WavyLineTwoNoteTurn::half:
        return core::TwoNoteTurn::half();
    case api::WavyLineTwoNoteTurn::none:
        return core::TwoNoteTurn::none();
    case api::WavyLineTwoNoteTurn::whole:
    case api::WavyLineTwoNoteTurn::unspecified:
    default:
        return core::TwoNoteTurn::whole();
    }
}

void parseWavyLine(const core::WavyLine &inWavyLine, api::NoteAttachmentData &outAttachments)
{
    api::SpannerNumber number;
    if (inWavyLine.number().has_value())
    {
        number = api::SpannerNumber(checkNumber(&inWavyLine));
    }

    const api::PositionData positionData = getPositionData(inWavyLine);
    const bool isColorSpecified = checkHasColor(&inWavyLine);
    api::ColorData colorData;
    if (isColorSpecified)
    {
        colorData = getColor(inWavyLine);
    }

    const auto type = inWavyLine.type();

    if (core::StartStopContinue::start() == type)
    {
        api::WavyLineStart start;
        start.number = number;
        start.positionData = positionData;
        start.isColorSpecified = isColorSpecified;
        start.colorData = colorData;

        if (inWavyLine.smufl().has_value())
        {
            start.smufl = inWavyLine.smufl()->toString();
        }
        if (inWavyLine.startNote().has_value())
        {
            start.startNote = wavyLineFunctionsConvertStartNoteFromCore(*inWavyLine.startNote());
        }
        if (inWavyLine.trillStep().has_value())
        {
            start.trillStep = wavyLineFunctionsConvertTrillStepFromCore(*inWavyLine.trillStep());
        }
        if (inWavyLine.twoNoteTurn().has_value())
        {
            start.twoNoteTurn = wavyLineFunctionsConvertTwoNoteTurnFromCore(*inWavyLine.twoNoteTurn());
        }
        if (inWavyLine.accelerate().has_value())
        {
            Converter converter;
            start.accelerate = converter.convert(*inWavyLine.accelerate());
        }
        if (inWavyLine.beats().has_value())
        {
            start.beats = static_cast<double>(inWavyLine.beats()->value().value());
        }
        if (inWavyLine.secondBeat().has_value())
        {
            start.secondBeat = static_cast<double>(inWavyLine.secondBeat()->value().value());
        }
        if (inWavyLine.lastBeat().has_value())
        {
            start.lastBeat = static_cast<double>(inWavyLine.lastBeat()->value().value());
        }

        outAttachments.wavyLineStarts.emplace_back(std::move(start));
    }
    else if (core::StartStopContinue::continue_() == type)
    {
        api::WavyLineContinue continueItem;
        continueItem.number = number;
        continueItem.positionData = positionData;
        continueItem.isColorSpecified = isColorSpecified;
        continueItem.colorData = colorData;
        outAttachments.wavyLineContinuations.emplace_back(std::move(continueItem));
    }
    else
    {
        api::WavyLineStop stop;
        stop.number = number;
        stop.positionData = positionData;
        stop.isColorSpecified = isColorSpecified;
        stop.colorData = colorData;
        outAttachments.wavyLineStops.emplace_back(std::move(stop));
    }
}

core::WavyLine writeWavyLineStart(const api::WavyLineStart &inStart, const std::optional<int> &inResolvedNumber)
{
    core::WavyLine wavyLine;
    wavyLine.setType(core::StartStopContinue::start());

    if (inResolvedNumber.has_value())
    {
        wavyLine.setNumber(core::NumberLevel{*inResolvedNumber});
    }

    if (inStart.smufl.has_value())
    {
        wavyLine.setSmufl(core::SmuflWavyLineGlyphName::parse(*inStart.smufl));
    }

    setAttributesFromPositionData(inStart.positionData, wavyLine);

    if (inStart.isColorSpecified)
    {
        setAttributesFromColorData(inStart.colorData, wavyLine);
    }

    if (inStart.startNote != api::WavyLineStartNote::unspecified)
    {
        wavyLine.setStartNote(wavyLineFunctionsConvertStartNoteToCore(inStart.startNote));
    }
    if (inStart.trillStep != api::WavyLineTrillStep::unspecified)
    {
        wavyLine.setTrillStep(wavyLineFunctionsConvertTrillStepToCore(inStart.trillStep));
    }
    if (inStart.twoNoteTurn != api::WavyLineTwoNoteTurn::unspecified)
    {
        wavyLine.setTwoNoteTurn(wavyLineFunctionsConvertTwoNoteTurnToCore(inStart.twoNoteTurn));
    }
    if (inStart.accelerate != api::Bool::unspecified)
    {
        Converter converter;
        wavyLine.setAccelerate(converter.convert(inStart.accelerate));
    }
    if (inStart.beats.has_value())
    {
        wavyLine.setBeats(core::TrillBeats{core::Decimal{*inStart.beats}});
    }
    if (inStart.secondBeat.has_value())
    {
        wavyLine.setSecondBeat(core::Percent{core::Decimal{*inStart.secondBeat}});
    }
    if (inStart.lastBeat.has_value())
    {
        wavyLine.setLastBeat(core::Percent{core::Decimal{*inStart.lastBeat}});
    }

    return wavyLine;
}

core::WavyLine writeWavyLineContinue(const api::WavyLineContinue &inContinue,
                                     const std::optional<int> &inResolvedNumber)
{
    core::WavyLine wavyLine;
    wavyLine.setType(core::StartStopContinue::continue_());

    if (inResolvedNumber.has_value())
    {
        wavyLine.setNumber(core::NumberLevel{*inResolvedNumber});
    }

    setAttributesFromPositionData(inContinue.positionData, wavyLine);

    if (inContinue.isColorSpecified)
    {
        setAttributesFromColorData(inContinue.colorData, wavyLine);
    }

    return wavyLine;
}

core::WavyLine writeWavyLineStop(const api::WavyLineStop &inStop, const std::optional<int> &inResolvedNumber)
{
    core::WavyLine wavyLine;
    wavyLine.setType(core::StartStopContinue::stop());

    if (inResolvedNumber.has_value())
    {
        wavyLine.setNumber(core::NumberLevel{*inResolvedNumber});
    }

    setAttributesFromPositionData(inStop.positionData, wavyLine);

    if (inStop.isColorSpecified)
    {
        setAttributesFromColorData(inStop.colorData, wavyLine);
    }

    return wavyLine;
}
} // namespace impl
} // namespace mx
