// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/ApiCommon.h"

#include <stdexcept>

using namespace mx::api;

TEST(defaultIsUnspecified, SpannerNumber)
{
    const SpannerNumber number;
    CHECK(SpannerNumber::Kind::unspecified == number.kind());
    CHECK(number.isUnspecified());
    CHECK(!number.isExplicit());
    CHECK(!number.isIdentity());
}

T_END

TEST(makeLevelRoundTrip, SpannerNumber)
{
    const auto lowest = SpannerNumber::makeLevel(1);
    CHECK(SpannerNumber::Kind::explicitLevel == lowest.kind());
    CHECK(lowest.isExplicit());
    CHECK(!lowest.isUnspecified());
    CHECK(!lowest.isIdentity());
    CHECK_EQUAL(1, lowest.level());

    const auto highest = SpannerNumber::makeLevel(16);
    CHECK_EQUAL(16, highest.level());
}

T_END

TEST(makeIdentityRoundTrip, SpannerNumber)
{
    const auto number = SpannerNumber::makeIdentity("the-id");
    CHECK(SpannerNumber::Kind::identity == number.kind());
    CHECK(number.isIdentity());
    CHECK(!number.isUnspecified());
    CHECK(!number.isExplicit());
    CHECK_EQUAL(std::string{"the-id"}, number.identity());
}

T_END

TEST(makeLevelRangePrecondition, SpannerNumber)
{
    CHECK_THROWS_AS(SpannerNumber::makeLevel(0), std::logic_error);
    CHECK_THROWS_AS(SpannerNumber::makeLevel(17), std::logic_error);
    CHECK_THROWS_AS(SpannerNumber::makeLevel(-1), std::logic_error);
}

T_END

TEST(makeIdentityEmptyPrecondition, SpannerNumber)
{
    CHECK_THROWS_AS(SpannerNumber::makeIdentity(""), std::logic_error);
}

T_END

TEST(wrongKindAccessors, SpannerNumber)
{
    const SpannerNumber unspecified;
    CHECK_THROWS_AS(unspecified.level(), std::logic_error);
    CHECK_THROWS_AS(unspecified.identity(), std::logic_error);

    const auto level = SpannerNumber::makeLevel(3);
    CHECK_THROWS_AS(level.identity(), std::logic_error);

    const auto identity = SpannerNumber::makeIdentity("x");
    CHECK_THROWS_AS(identity.level(), std::logic_error);
}

T_END

TEST(equality, SpannerNumber)
{
    const SpannerNumber unspecifiedA;
    const SpannerNumber unspecifiedB;
    const auto level2 = SpannerNumber::makeLevel(2);
    const auto level2Again = SpannerNumber::makeLevel(2);
    const auto level3 = SpannerNumber::makeLevel(3);
    const auto identityA = SpannerNumber::makeIdentity("a");
    const auto identityAAgain = SpannerNumber::makeIdentity("a");
    const auto identityB = SpannerNumber::makeIdentity("b");

    // same kind
    CHECK(unspecifiedA == unspecifiedB);
    CHECK(level2 == level2Again);
    CHECK(level2 != level3);
    CHECK(identityA == identityAAgain);
    CHECK(identityA != identityB);

    // different kinds are never equal
    CHECK(unspecifiedA != level2);
    CHECK(unspecifiedA != identityA);
    CHECK(level2 != identityA);
}

T_END

#endif
