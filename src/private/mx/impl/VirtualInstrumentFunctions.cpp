// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/VirtualInstrumentFunctions.h"
#include "mx/core/generated/Empty.h"
#include "mx/core/generated/InstrumentSound.h"
#include "mx/core/generated/PositiveIntegerOrEmpty.h"
#include "mx/core/generated/VirtualInstrument.h"
#include "mx/core/generated/VirtualInstrumentDataGroup.h"
#include "mx/core/generated/VirtualInstrumentDataGroupChoice.h"
#include "mx/impl/Converter.h"

namespace mx
{
namespace impl
{
api::InstrumentChangeData readVirtualInstrumentData(const core::VirtualInstrumentDataGroup &inGroup,
                                                    const DiagnosticsContext &diagnostics,
                                                    const api::Location &location)
{
    api::InstrumentChangeData out{};

    if (inGroup.choice().has_value())
    {
        const auto &choice = *inGroup.choice();
        if (choice.isEnsemble())
        {
            out.soloOrEnsemble = api::SoloOrEnsemble::ensemble;
        }
        else if (choice.isSolo())
        {
            out.soloOrEnsemble = api::SoloOrEnsemble::solo;
        }
    }

    if (inGroup.instrumentSound().has_value())
    {
        const auto &instrSound = *inGroup.instrumentSound();
        if (instrSound.isSoundID())
        {
            Converter c;
            out.soundID = c.convert(instrSound.asSoundID());
        }
        else
        {
            diagnostics.report(api::Severity::error, api::DiagnosticCode::droppedData, location,
                               "instrument-sound \"" + instrSound.asString() +
                                   "\" is not a known sound; it is not read");
        }
    }

    if (inGroup.virtualInstrument().has_value())
    {
        const auto &virtualInstrument = *inGroup.virtualInstrument();

        if (virtualInstrument.virtualName().has_value())
        {
            out.virtualName = *virtualInstrument.virtualName();
        }

        if (virtualInstrument.virtualLibrary().has_value())
        {
            out.virtualLibrary = *virtualInstrument.virtualLibrary();
        }
    }

    return out;
}

core::VirtualInstrumentDataGroup writeVirtualInstrumentData(const api::InstrumentChangeData &inData)
{
    core::VirtualInstrumentDataGroup outGroup{};

    if (inData.soloOrEnsemble == api::SoloOrEnsemble::ensemble)
    {
        outGroup.setChoice(
            core::VirtualInstrumentDataGroupChoice::ensemble(core::PositiveIntegerOrEmpty::positiveInteger(0)));
    }
    else if (inData.soloOrEnsemble == api::SoloOrEnsemble::solo)
    {
        outGroup.setChoice(core::VirtualInstrumentDataGroupChoice::solo(core::Empty{}));
    }

    if (inData.virtualName.size() > 0 || inData.virtualLibrary.size() > 0)
    {
        core::VirtualInstrument virtualInstrument{};

        if (inData.virtualName.size() > 0)
        {
            virtualInstrument.setVirtualName(inData.virtualName);
        }

        if (inData.virtualLibrary.size() > 0)
        {
            virtualInstrument.setVirtualLibrary(inData.virtualLibrary);
        }

        outGroup.setVirtualInstrument(virtualInstrument);
    }

    if (inData.soundID != api::SoundID::unspecified && inData.soundID != api::SoundID::errorBadString)
    {
        Converter c;
        outGroup.setInstrumentSound(core::InstrumentSound::soundID(c.convert(inData.soundID)));
    }

    return outGroup;
}
} // namespace impl
} // namespace mx
