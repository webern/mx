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

T_END

#endif
