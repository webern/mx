// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/SpannerNumber.h"

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

TEST(levelRoundTrip, SpannerNumber)
{
    const SpannerNumber lowest{1};
    CHECK(SpannerNumber::Kind::explicitLevel == lowest.kind());
    CHECK(lowest.isExplicit());
    CHECK(!lowest.isUnspecified());
    CHECK(!lowest.isIdentity());
    CHECK_EQUAL(1, lowest.level());

    const SpannerNumber highest{16};
    CHECK_EQUAL(16, highest.level());
}

T_END

TEST(identityRoundTrip, SpannerNumber)
{
    const SpannerNumber number{std::string{"the-id"}};
    CHECK(SpannerNumber::Kind::identity == number.kind());
    CHECK(number.isIdentity());
    CHECK(!number.isUnspecified());
    CHECK(!number.isExplicit());
    CHECK_EQUAL(std::string{"the-id"}, number.identity());
}

T_END

// Out-of-range levels and empty ids are not programming errors here: they
// collapse to unspecified, the same way an unrepresentable ComplexTimeSignature
// collapses in TimeChoice. Nothing throws.
TEST(outOfRangeLevelCollapsesToUnspecified, SpannerNumber)
{
    CHECK(SpannerNumber{0}.isUnspecified());
    CHECK(SpannerNumber{17}.isUnspecified());
    CHECK(SpannerNumber{-1}.isUnspecified());
}

T_END

TEST(emptyIdentityCollapsesToUnspecified, SpannerNumber)
{
    CHECK(SpannerNumber{std::string{}}.isUnspecified());
}

T_END

// Calling an accessor for the wrong kind returns a harmless default instead of
// throwing: -1 (NUMBER_LEVEL_UNSPECIFIED) for level(), an empty string for
// identity().
TEST(wrongKindAccessorsReturnDefaults, SpannerNumber)
{
    const SpannerNumber unspecified;
    CHECK_EQUAL(NUMBER_LEVEL_UNSPECIFIED, unspecified.level());
    CHECK_EQUAL(std::string{}, unspecified.identity());

    const SpannerNumber level{3};
    CHECK_EQUAL(std::string{}, level.identity());

    const SpannerNumber identity{std::string{"x"}};
    CHECK_EQUAL(NUMBER_LEVEL_UNSPECIFIED, identity.level());
}

T_END

TEST(equality, SpannerNumber)
{
    const SpannerNumber unspecifiedA;
    const SpannerNumber unspecifiedB;
    const SpannerNumber level2{2};
    const SpannerNumber level2Again{2};
    const SpannerNumber level3{3};
    const SpannerNumber identityA{std::string{"a"}};
    const SpannerNumber identityAAgain{std::string{"a"}};
    const SpannerNumber identityB{std::string{"b"}};

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
