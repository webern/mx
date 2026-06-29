// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/generated/Bracket.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/Stem.h"
#include "mx/impl/PositionFunctions.h"

#include <sstream>

using namespace mx;
using namespace mx::impl;

TEST(setAttributesFromPositionDataBracket, PositionFunctions)
{
    core::Bracket attr;
    api::PositionData positionData;

    positionData.isDefaultXSpecified = true;
    positionData.defaultX = 0.1;
    positionData.isDefaultYSpecified = true;
    positionData.defaultY = 0.2;

    positionData.isRelativeXSpecified = true;
    positionData.relativeX = 0.3;
    positionData.isRelativeYSpecified = true;
    positionData.relativeY = 0.4;

    impl::setAttributesFromPositionData(positionData, attr);

    CHECK(attr.defaultX().has_value());
    CHECK_DOUBLES_EQUAL(0.1, attr.defaultX()->value().value(), 0.0001);

    CHECK(attr.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(0.2, attr.defaultY()->value().value(), 0.0001);

    CHECK(attr.relativeX().has_value());
    CHECK_DOUBLES_EQUAL(0.3, attr.relativeX()->value().value(), 0.0001);

    CHECK(attr.relativeY().has_value());
    CHECK_DOUBLES_EQUAL(0.4, attr.relativeY()->value().value(), 0.0001);
}

T_END

class TotallyBogusClassToTestSetAttributesFromPositionData
{
};

TEST(setAttributesFromPositionDataFakeClass, PositionFunctions)
{
    TotallyBogusClassToTestSetAttributesFromPositionData fake;
    api::PositionData positionData;
    impl::setAttributesFromPositionData(positionData, fake);

    // only check that it compiles
    CHECK(true);
}

T_END

TEST(setAttributesFromPositionDataDirectionAttributes, PositionFunctions)
{
    core::Direction attr;
    api::PositionData positionData;

    positionData.placement = api::Placement::below;

    impl::setAttributesFromPositionData(positionData, attr);

    CHECK(attr.placement().has_value());
    CHECK(attr.placement() == core::AboveBelow::below());
}

T_END

TEST(setAttributesFromPositionDataStem, PositionFunctions)
{
    core::Stem stem;
    api::PositionData positionData;

    positionData.isDefaultYSpecified = true;
    positionData.defaultY = 5.0;
    positionData.isRelativeYSpecified = true;
    positionData.relativeY = -3.0;

    impl::setAttributesFromPositionData(positionData, stem);

    CHECK(stem.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(5.0, stem.defaultY()->value().value(), 0.0001);
    CHECK(stem.relativeY().has_value());
    CHECK_DOUBLES_EQUAL(-3.0, stem.relativeY()->value().value(), 0.0001);
    CHECK(!stem.defaultX().has_value());
    CHECK(!stem.relativeX().has_value());
}

T_END

TEST(setAttributesFromPositionDataStemZero, PositionFunctions)
{
    core::Stem stem;
    api::PositionData positionData;

    positionData.isDefaultYSpecified = true;
    positionData.defaultY = 0.0;

    impl::setAttributesFromPositionData(positionData, stem);

    CHECK(stem.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(0.0, stem.defaultY()->value().value(), 0.0001);
}

T_END

TEST(getPositionDataStem, PositionFunctions)
{
    core::Stem stem;
    stem.setDefaultY(core::Tenths{core::Decimal{0.0}});
    stem.setRelativeY(core::Tenths{core::Decimal{-2.5}});

    auto positionData = impl::getPositionData(stem);

    CHECK(positionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(0.0, positionData.defaultY, 0.0001);
    CHECK(positionData.isRelativeYSpecified);
    CHECK_DOUBLES_EQUAL(-2.5, positionData.relativeY, 0.0001);
    CHECK(!positionData.isDefaultXSpecified);
    CHECK(!positionData.isRelativeXSpecified);
}

T_END

TEST(stemPositionRoundTrip, PositionFunctions)
{
    core::Stem original;
    original.setDefaultY(core::Tenths{core::Decimal{0.0}});

    auto positionData = impl::getPositionData(original);
    CHECK(positionData.isDefaultYSpecified);

    core::Stem roundTripped;
    impl::setAttributesFromPositionData(positionData, roundTripped);

    CHECK(roundTripped.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(0.0, roundTripped.defaultY()->value().value(), 0.0001);
}

T_END

TEST(stemDefaultYZeroApiRoundTrip, PositionFunctions)
{
    const char *xml = R"(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 4.0 Partwise//EN"
    "http://www.musicxml.org/dtds/partwise.dtd">
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1"><part-name/></score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <time><beats>4</beats><beat-type>4</beat-type></time>
        <clef><sign>G</sign><line>2</line></clef>
      </attributes>
      <note>
        <pitch><step>C</step><octave>4</octave></pitch>
        <duration>4</duration>
        <type>whole</type>
        <stem default-y="0">up</stem>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    auto &mgr = api::DocumentManager::getInstance();
    std::istringstream iss{xml};
    auto docIdResult = mgr.createFromStream(iss);
    CHECK(docIdResult.ok());
    auto docId = docIdResult.value();

    auto scoreResult = mgr.getData(docId);
    CHECK(scoreResult.ok());
    auto scoreData = scoreResult.value();
    mgr.destroyDocument(docId);

    // Verify the reader populated stemPositionData
    const auto &note = scoreData.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    CHECK(note.stem == api::Stem::up);
    CHECK(note.stemPositionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(0.0, note.stemPositionData.defaultY, 0.0001);

    // Round-trip back to XML
    auto docId2Result = mgr.createFromScore(scoreData);
    CHECK(docId2Result.ok());
    auto docId2 = docId2Result.value();
    std::stringstream ss;
    mgr.writeToStream(docId2, ss);
    mgr.destroyDocument(docId2);

    const auto output = ss.str();
    CHECK(output.find("default-y") != std::string::npos);
}

T_END

#endif
