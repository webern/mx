// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/SoundID.h"
#include "mx/core/generated/SoundID.h"
#include "mx/impl/Converter.h"

using namespace mx;

TEST(windReedClarinetPiccolo_coreToApi, ConverterSoundID)
{
    const impl::Converter converter;
    const auto result = converter.convert(core::SoundID::windReedClarinetPiccolo());
    CHECK(api::SoundID::windReedClarinetPiccolo == result);
}

TEST(windReedClarinetPiccolo_apiToCore, ConverterSoundID)
{
    const impl::Converter converter;
    const auto result = converter.convert(api::SoundID::windReedClarinetPiccolo);
    CHECK(core::SoundID::windReedClarinetPiccolo() == result);
}

// #184 - MusicXML 4.0 instrument-sound ids drum-tabor, drum-tamborim,
// pitched-percussion-handchimes, pluck-cavaquinho, wind-flutes-whistle-tin-c,
// wind-reed-clarinet-d, wind-reed-clarinet-g were missing from instrumentMap,
// so they read back as SoundID::unspecified (data loss).
TEST(soundId40Additions_coreToApi, ConverterSoundID)
{
    const impl::Converter converter;
    CHECK(api::SoundID::drumTabor == converter.convert(core::SoundID::drumTabor()));
    CHECK(api::SoundID::drumTamborim == converter.convert(core::SoundID::drumTamborim()));
    CHECK(api::SoundID::pitchedPercussionHandchimes == converter.convert(core::SoundID::pitchedPercussionHandchimes()));
    CHECK(api::SoundID::pluckCavaquinho == converter.convert(core::SoundID::pluckCavaquinho()));
    CHECK(api::SoundID::windFlutesWhistleTinC == converter.convert(core::SoundID::windFlutesWhistleTinC()));
    CHECK(api::SoundID::windReedClarinetD == converter.convert(core::SoundID::windReedClarinetD()));
    CHECK(api::SoundID::windReedClarinetG == converter.convert(core::SoundID::windReedClarinetG()));
}

TEST(soundId40Additions_apiToCore, ConverterSoundID)
{
    const impl::Converter converter;
    CHECK(core::SoundID::drumTabor() == converter.convert(api::SoundID::drumTabor));
    CHECK(core::SoundID::drumTamborim() == converter.convert(api::SoundID::drumTamborim));
    CHECK(core::SoundID::pitchedPercussionHandchimes() == converter.convert(api::SoundID::pitchedPercussionHandchimes));
    CHECK(core::SoundID::pluckCavaquinho() == converter.convert(api::SoundID::pluckCavaquinho));
    CHECK(core::SoundID::windFlutesWhistleTinC() == converter.convert(api::SoundID::windFlutesWhistleTinC));
    CHECK(core::SoundID::windReedClarinetD() == converter.convert(api::SoundID::windReedClarinetD));
    CHECK(core::SoundID::windReedClarinetG() == converter.convert(api::SoundID::windReedClarinetG));
}

T_END

#endif
