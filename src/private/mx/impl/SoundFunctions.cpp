// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/SoundFunctions.h"
#include "mx/core/Decimal.h"
#include "mx/core/generated/NonNegativeDecimal.h"
#include "mx/core/generated/Sound.h"
#include "mx/core/generated/YesNo.h"

namespace mx
{
namespace impl
{
namespace
{
api::Bool toApiBool(const std::optional<core::YesNo> &value)
{
    if (!value.has_value())
    {
        return api::Bool::unspecified;
    }

    return value->tag() == core::YesNo::Tag::yes ? api::Bool::yes : api::Bool::no;
}
} // namespace

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

    out.dacapo = toApiBool(inSound.dacapo());
    out.forwardRepeat = toApiBool(inSound.forwardRepeat());
    out.pizzicato = toApiBool(inSound.pizzicato());

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
}
} // namespace impl
} // namespace mx
