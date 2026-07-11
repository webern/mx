// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/SoundFunctions.h"
#include "mx/core/Decimal.h"
#include "mx/core/generated/Empty.h"
#include "mx/core/generated/NonNegativeDecimal.h"
#include "mx/core/generated/Sound.h"
#include "mx/core/generated/Swing.h"
#include "mx/core/generated/SwingChoice.h"
#include "mx/core/generated/SwingChoiceGroup.h"
#include "mx/core/generated/SwingTypeValue.h"
#include "mx/core/generated/YesNo.h"

namespace mx
{
namespace impl
{
api::Bool soundFunctionsToApiBool(const std::optional<core::YesNo> &value)
{
    if (!value.has_value())
    {
        return api::Bool::unspecified;
    }

    return value->tag() == core::YesNo::Tag::yes ? api::Bool::yes : api::Bool::no;
}

api::SwingNoteType soundFunctionsToApiSwingNoteType(const core::SwingTypeValue &value)
{
    return value.tag() == core::SwingTypeValue::Tag::eighth ? api::SwingNoteType::eighth : api::SwingNoteType::dur16th;
}

core::SwingTypeValue soundFunctionsToCoreSwingTypeValue(api::SwingNoteType value)
{
    return value == api::SwingNoteType::dur16th ? core::SwingTypeValue::_16th() : core::SwingTypeValue::eighth();
}

api::SwingData soundFunctionsToApiSwingData(const core::Swing &inSwing)
{
    api::SwingData out{};

    if (inSwing.choice().isStraight())
    {
        out.isStraight = true;
    }
    else
    {
        out.isStraight = false;
        const auto &group = inSwing.choice().asGroup();
        out.ratioFirst = group.first();
        out.ratioSecond = group.second();

        if (group.swingType().has_value())
        {
            out.noteType = soundFunctionsToApiSwingNoteType(*group.swingType());
        }
    }

    if (inSwing.swingStyle().has_value())
    {
        out.style = *inSwing.swingStyle();
    }

    return out;
}

core::Swing soundFunctionsToCoreSwing(const api::SwingData &inSwingData)
{
    core::Swing outSwing{};

    if (inSwingData.isStraight)
    {
        outSwing.setChoice(core::SwingChoice::straight(core::Empty{}));
    }
    else
    {
        core::SwingChoiceGroup group{};
        group.setFirst(inSwingData.ratioFirst);
        group.setSecond(inSwingData.ratioSecond);

        if (inSwingData.noteType != api::SwingNoteType::unspecified)
        {
            group.setSwingType(soundFunctionsToCoreSwingTypeValue(inSwingData.noteType));
        }

        outSwing.setChoice(core::SwingChoice::group(group));
    }

    if (!inSwingData.style.empty())
    {
        outSwing.setSwingStyle(inSwingData.style);
    }

    return outSwing;
}

api::SoundData readSoundData(const core::Sound &inSound)
{
    api::SoundData out{};

    if (inSound.tempo().has_value())
    {
        out.tempo = inSound.tempo()->value().value();
    }

    if (inSound.dynamics().has_value())
    {
        out.dynamics = inSound.dynamics()->value().value();
    }

    out.dacapo = soundFunctionsToApiBool(inSound.dacapo());
    out.forwardRepeat = soundFunctionsToApiBool(inSound.forwardRepeat());
    out.pizzicato = soundFunctionsToApiBool(inSound.pizzicato());

    if (inSound.segno().has_value())
    {
        out.segno = *inSound.segno();
    }

    if (inSound.dalsegno().has_value())
    {
        out.dalsegno = *inSound.dalsegno();
    }

    if (inSound.coda().has_value())
    {
        out.coda = *inSound.coda();
    }

    if (inSound.tocoda().has_value())
    {
        out.tocoda = *inSound.tocoda();
    }

    if (inSound.fine().has_value())
    {
        out.fine = *inSound.fine();
    }

    if (inSound.swing().has_value())
    {
        out.swing = soundFunctionsToApiSwingData(*inSound.swing());
    }

    return out;
}

void writeSoundData(const api::SoundData &inSoundData, core::Sound &outSound)
{
    if (inSoundData.tempo >= 0.0)
    {
        outSound.setTempo(core::NonNegativeDecimal{core::Decimal{inSoundData.tempo}});
    }

    if (inSoundData.dynamics >= 0.0)
    {
        outSound.setDynamics(core::NonNegativeDecimal{core::Decimal{inSoundData.dynamics}});
    }

    if (inSoundData.dacapo != api::Bool::unspecified)
    {
        outSound.setDacapo(inSoundData.dacapo == api::Bool::yes ? core::YesNo::yes() : core::YesNo::no());
    }

    if (inSoundData.forwardRepeat != api::Bool::unspecified)
    {
        outSound.setForwardRepeat(inSoundData.forwardRepeat == api::Bool::yes ? core::YesNo::yes() : core::YesNo::no());
    }

    if (inSoundData.pizzicato != api::Bool::unspecified)
    {
        outSound.setPizzicato(inSoundData.pizzicato == api::Bool::yes ? core::YesNo::yes() : core::YesNo::no());
    }

    if (!inSoundData.segno.empty())
    {
        outSound.setSegno(inSoundData.segno);
    }

    if (!inSoundData.dalsegno.empty())
    {
        outSound.setDalsegno(inSoundData.dalsegno);
    }

    if (!inSoundData.coda.empty())
    {
        outSound.setCoda(inSoundData.coda);
    }

    if (!inSoundData.tocoda.empty())
    {
        outSound.setTocoda(inSoundData.tocoda);
    }

    if (!inSoundData.fine.empty())
    {
        outSound.setFine(inSoundData.fine);
    }

    if (inSoundData.swing.has_value())
    {
        outSound.setSwing(soundFunctionsToCoreSwing(*inSoundData.swing));
    }
}
} // namespace impl
} // namespace mx
