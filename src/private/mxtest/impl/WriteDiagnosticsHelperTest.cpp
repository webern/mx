// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/Diagnostics.h"
#include "mx/api/IdAccess.h"
#include "mx/core/generated/MIDI16.h"
#include "mx/core/generated/Percent.h"
#include "mx/impl/DiagnosticsContext.h"

using namespace mx;
using namespace mx::impl;

TEST(reportAdjustedReportsAClampedInteger, WriteDiagnosticsHelper)
{
    api::Diagnostics diagnostics;
    const DiagnosticsContext context{diagnostics};
    api::Location location;
    location.partIndex = 2;

    const core::MIDI16 channel{17};
    CHECK(reportAdjusted(context, location, "midi-channel", 17, channel.value()));

    REQUIRE(diagnostics.all().size() == 1);
    const auto &diagnostic = diagnostics.all().front();
    CHECK(api::Severity::warning == diagnostic.severity);
    CHECK(api::DiagnosticCode::valueAdjusted == diagnostic.code);
    CHECK_EQUAL(2, diagnostic.location.partIndex);
    CHECK_EQUAL(std::string{"midi-channel 17 is out of range; using 16"}, diagnostic.message);
}

T_END

TEST(reportAdjustedReportsAClampedDecimal, WriteDiagnosticsHelper)
{
    api::Diagnostics diagnostics;
    const DiagnosticsContext context{diagnostics};

    const core::Percent volume{core::Decimal{100.5}};
    CHECK(reportAdjusted(context, api::Location{}, "volume", 100.5, volume.value().value()));

    REQUIRE(diagnostics.all().size() == 1);
    CHECK_EQUAL(std::string{"volume 100.5 is out of range; using 100"}, diagnostics.all().front().message);
}

T_END

TEST(reportAdjustedIsSilentForAnUnchangedValue, WriteDiagnosticsHelper)
{
    api::Diagnostics diagnostics;
    const DiagnosticsContext context{diagnostics};

    const core::MIDI16 channel{16};
    CHECK(!reportAdjusted(context, api::Location{}, "midi-channel", 16, channel.value()));
    CHECK(!reportAdjusted(context, api::Location{}, "volume", 50.25, 50.25));
    CHECK(diagnostics.all().empty());
}

T_END

TEST(idRemembersTheTextItScrubbed, WriteDiagnosticsHelper)
{
    const api::Id scrubbed{"1 note"};
    CHECK_EQUAL(std::string{"note"}, scrubbed.value());
    REQUIRE(api::IdAccess::scrubbedText(scrubbed).has_value());
    CHECK_EQUAL(std::string{"1 note"}, *api::IdAccess::scrubbedText(scrubbed));

    const api::Id copy = scrubbed;
    REQUIRE(api::IdAccess::scrubbedText(copy).has_value());
    CHECK_EQUAL(std::string{"1 note"}, *api::IdAccess::scrubbedText(copy));

    CHECK(!api::IdAccess::scrubbedText(api::Id{"note"}).has_value());
    CHECK(!api::IdAccess::scrubbedText(api::IdAccess::make(core::Token{"a b"})).has_value());
}

T_END

#endif
