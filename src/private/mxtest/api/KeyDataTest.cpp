// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#include <sstream>

#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Cancel.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/Key.h"
#include "mx/core/generated/KeyChoice.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/NonTraditionalKeyGroup.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PartwisePart.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/core/generated/TraditionalKeyGroup.h"
#include "mxtest/api/TestHelpers.h"

using namespace std;
using namespace mx::api;
namespace core = mx::core;

// using namespace mxtest;

namespace
{
ScoreData putKeyInScore(KeyData key)
{
    ScoreData score;
    score.parts.push_back(PartData{});
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.keys.push_back(key);
    return score;
}

/// Helper: get the first Key element from the first Attributes element of the first measure of the first part.
const mx::core::Key &getFirstCoreKey(const mx::core::DocumentPtr &corePtr)
{
    REQUIRE(corePtr != nullptr);
    REQUIRE(corePtr->isScorePartwise());
    const auto &scorePartwise = corePtr->asScorePartwise();
    const auto parts = scorePartwise.part();
    REQUIRE(!parts.empty());
    const auto &part = parts[0];
    const auto measures = part.measure();
    REQUIRE(!measures.empty());
    const auto &measure = measures[0];
    const auto &mdcSet = measure.musicData();
    REQUIRE(!mdcSet.empty());
    const auto &mdc = mdcSet.front();
    REQUIRE(mdc.isAttributes());
    const auto &keys = mdc.asAttributes().key();
    REQUIRE(!keys.empty());
    return keys.front();
}

/// Helper: build a single-key score with the given fifths and mode, serialize it, and return the XML.
std::string keyModeXml(int fifths, KeyMode mode)
{
    KeyData key;
    key.fifths = fifths;
    key.mode = mode;
    return mxtest::toXml(putKeyInScore(key));
}

/// Helper: wrap a key element's children in a minimal score-partwise document.
std::string keyXmlDocument(const std::string &inKeyChildren)
{
    return R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1">
      <part-name>P</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <key>
)" + inKeyChildren +
           R"(
        </key>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";
}

/// Helper: parse a document whose key holds the given children, and return the first KeyData.
KeyData keyFromXml(const std::string &inKeyChildren)
{
    const auto score = mxtest::fromXml(keyXmlDocument(inKeyChildren));
    REQUIRE(!score.parts.empty());
    REQUIRE(!score.parts.at(0).measures.empty());
    const auto &keys = score.parts.at(0).measures.at(0).keys;
    REQUIRE(!keys.empty());
    return keys.at(0);
}
} // namespace

TEST(EMajor, KeyData)
{
    KeyData key;
    key.fifths = 4;
    key.cancel = -1;
    key.mode = KeyMode::major;
    key.staffIndex = 0;

    const auto original = putKeyInScore(key);
    auto &docMgr = DocumentManager::getInstance();
    const auto originalIdResult = docMgr.createFromScore(original);
    REQUIRE(originalIdResult.ok());
    const int originalId = originalIdResult.value();
    const mx::core::DocumentPtr corePtr = docMgr.getDocument(originalId);

    const auto &coreKey = getFirstCoreKey(corePtr);
    const auto &coreKeyChoice = coreKey.choice();
    CHECK(coreKeyChoice.isTraditionalKey());
    const auto &coreTraditionalKey = coreKeyChoice.asTraditionalKey();

    // check if the staff number was written
    CHECK(coreKey.number().has_value())
    if (coreKey.number().has_value())
    {
        CHECK_EQUAL(key.staffIndex, coreKey.number()->value() - 1);
    }

    // check the mode
    CHECK(coreTraditionalKey.mode().has_value())
    if (coreTraditionalKey.mode().has_value())
    {
        CHECK_EQUAL(std::string{"major"}, coreTraditionalKey.mode()->value());
    }

    // check the cancel
    CHECK(coreTraditionalKey.cancel().has_value())
    if (coreTraditionalKey.cancel().has_value())
    {
        CHECK_EQUAL(-1, coreTraditionalKey.cancel()->value().value());
    }

    // check the fifths
    CHECK_EQUAL(4, coreTraditionalKey.fifths().value());

    // serialize and deserialize
    std::stringstream xml;
    docMgr.writeToStream(originalId, xml);
    docMgr.destroyDocument(originalId);
    std::istringstream iss{xml.str()};
    const auto deserializedIdResult = docMgr.createFromStream(iss);
    REQUIRE(deserializedIdResult.ok());
    const int deserializedId = deserializedIdResult.value();
    const auto deserializedScoreResult = docMgr.getData(deserializedId);
    docMgr.destroyDocument(deserializedId);
    REQUIRE(deserializedScoreResult.ok());
    const auto &deserializedScore = deserializedScoreResult.value();
    const auto &deserializedKeys = deserializedScore.parts.at(0).measures.at(0).keys;
    CHECK_EQUAL(1, deserializedKeys.size())
    const auto deserializedKey = deserializedKeys.at(0);

    CHECK_EQUAL(key.staffIndex, deserializedKey.staffIndex);
    CHECK_EQUAL(key.cancel, deserializedKey.cancel);
    CHECK_EQUAL(key.fifths, deserializedKey.fifths);
    CHECK_EQUAL(key.mode, deserializedKey.mode);
    CHECK_EQUAL(0, deserializedKey.tickTimePosition);
    CHECK(deserializedKey.nonTraditional.empty());
}

TEST(AbMinor, KeyData)
{
    KeyData key;
    key.fifths = -7;
    key.cancel = 0;
    key.mode = KeyMode::minor;
    key.staffIndex = -10;

    const auto original = putKeyInScore(key);
    auto &docMgr = DocumentManager::getInstance();
    const auto originalIdResult = docMgr.createFromScore(original);
    REQUIRE(originalIdResult.ok());
    const int originalId = originalIdResult.value();
    const mx::core::DocumentPtr corePtr = docMgr.getDocument(originalId);

    const auto &coreKey = getFirstCoreKey(corePtr);
    const auto &coreKeyChoice = coreKey.choice();
    CHECK(coreKeyChoice.isTraditionalKey());
    const auto &coreTraditionalKey = coreKeyChoice.asTraditionalKey();

    // check if the staff number was written (staffIndex -10 means no staff number)
    CHECK(!coreKey.number().has_value())

    // check the mode
    CHECK(coreTraditionalKey.mode().has_value())
    if (coreTraditionalKey.mode().has_value())
    {
        CHECK_EQUAL(std::string{"minor"}, coreTraditionalKey.mode()->value());
    }

    // check the cancel (cancel==0 means no cancel element)
    CHECK(!coreTraditionalKey.cancel().has_value())

    // check the fifths
    CHECK_EQUAL(key.fifths, coreTraditionalKey.fifths().value());

    // serialize and deserialize
    std::stringstream xml;
    docMgr.writeToStream(originalId, xml);
    docMgr.destroyDocument(originalId);
    std::istringstream iss{xml.str()};
    const auto deserializedIdResult = docMgr.createFromStream(iss);
    REQUIRE(deserializedIdResult.ok());
    const int deserializedId = deserializedIdResult.value();
    const auto deserializedScoreResult = docMgr.getData(deserializedId);
    docMgr.destroyDocument(deserializedId);
    REQUIRE(deserializedScoreResult.ok());
    const auto &deserializedScore = deserializedScoreResult.value();
    const auto &deserializedKeys = deserializedScore.parts.at(0).measures.at(0).keys;
    CHECK_EQUAL(1, deserializedKeys.size())
    const auto deserializedKey = deserializedKeys.at(0);

    CHECK_EQUAL(-1, deserializedKey.staffIndex);
    CHECK_EQUAL(key.cancel, deserializedKey.cancel);
    CHECK_EQUAL(key.fifths, deserializedKey.fifths);
    CHECK_EQUAL(key.mode, deserializedKey.mode);
    CHECK_EQUAL(0, deserializedKey.tickTimePosition);
    CHECK(deserializedKey.nonTraditional.empty());
}

TEST(NonTraditional1, KeyData)
{
    KeyData key;
    key.fifths = 1; // should be ignored
    key.cancel = 2;
    key.mode = KeyMode::minor; // should be ignored
    key.staffIndex = -1;
    KeyComponent cSharp{Step::c, 1, 0.0, Accidental::sharp};
    KeyComponent dQuarterFlat{Step::d, 0, -50.0, Accidental::quarterFlat};
    key.nonTraditional.push_back(cSharp);
    key.nonTraditional.push_back(dQuarterFlat);

    const auto original = putKeyInScore(key);
    auto &docMgr = DocumentManager::getInstance();
    const auto originalIdResult = docMgr.createFromScore(original);
    REQUIRE(originalIdResult.ok());
    const int originalId = originalIdResult.value();
    const mx::core::DocumentPtr corePtr = docMgr.getDocument(originalId);

    const auto &coreKey = getFirstCoreKey(corePtr);
    const auto &coreKeyChoice = coreKey.choice();
    CHECK(coreKeyChoice.isNonTraditionalKey());
    const auto &coreKeyComponents = coreKeyChoice.asNonTraditionalKey();
    REQUIRE(2 == coreKeyComponents.size());

    // check if the staff number was written (staffIndex -1 means no staff number)
    CHECK(!coreKey.number().has_value())

    // check first key component (C#)
    const auto &comp0 = coreKeyComponents.at(0);
    CHECK(core::Step::Tag::c == comp0.keyStep().tag());
    CHECK_DOUBLES_EQUAL(1.0, comp0.keyAlter().value().value(), 0.0001)
    CHECK(comp0.keyAccidental().has_value());
    if (comp0.keyAccidental().has_value())
    {
        CHECK(core::AccidentalValue::Tag::sharp == comp0.keyAccidental()->value().tag());
    }

    // check second key component (D quarter-flat)
    const auto &comp1 = coreKeyComponents.at(1);
    CHECK(core::Step::Tag::d == comp1.keyStep().tag());
    CHECK_DOUBLES_EQUAL(-0.50, comp1.keyAlter().value().value(), 0.0001)
    CHECK(comp1.keyAccidental().has_value());
    if (comp1.keyAccidental().has_value())
    {
        CHECK(core::AccidentalValue::Tag::quarterFlat == comp1.keyAccidental()->value().tag());
    }

    // serialize and deserialize
    std::stringstream xml;
    docMgr.writeToStream(originalId, xml);
    docMgr.destroyDocument(originalId);
    std::istringstream iss{xml.str()};
    const auto deserializedIdResult = docMgr.createFromStream(iss);
    REQUIRE(deserializedIdResult.ok());
    const int deserializedId = deserializedIdResult.value();
    const auto deserializedScoreResult = docMgr.getData(deserializedId);
    docMgr.destroyDocument(deserializedId);
    REQUIRE(deserializedScoreResult.ok());
    const auto &deserializedScore = deserializedScoreResult.value();
    const auto &deserializedKeys = deserializedScore.parts.at(0).measures.at(0).keys;
    CHECK_EQUAL(1, deserializedKeys.size())
    const auto deserializedKey = deserializedKeys.at(0);

    CHECK_EQUAL(-1, deserializedKey.staffIndex);
    CHECK_EQUAL(0, deserializedKey.cancel); // Non-Traditional Keys Cannot have a Cancel Component
    CHECK_EQUAL(0, deserializedKey.fifths);
    CHECK_EQUAL(KeyMode::unspecified, deserializedKey.mode);
    CHECK_EQUAL(0, deserializedKey.tickTimePosition);
    CHECK_EQUAL(2, deserializedKey.nonTraditional.size());

    CHECK_EQUAL(cSharp, deserializedKey.nonTraditional.at(0));
    CHECK_EQUAL(dQuarterFlat, deserializedKey.nonTraditional.at(1));
}

TEST(KeyDataEquality_change_fifths, KeyData)
{
    KeyData key1;
    key1.fifths = 4;
    key1.cancel = -1;
    key1.mode = KeyMode::major;
    key1.staffIndex = 0;
    key1.nonTraditional.push_back(KeyComponent{Step::c, 1, 0.0, Accidental::sharp});
    key1.nonTraditional.push_back(KeyComponent{Step::d, 0, -50.0, Accidental::quarterFlat});
    key1.tickTimePosition = 13;
    auto key2 = key1;
    CHECK(key1 == key2);
    CHECK(!(key1 != key2));

    // change one thing
    key1.fifths += 1;
    CHECK(key1 != key2);
    CHECK(!(key1 == key2));
}

TEST(KeyDataEquality_change_cancel, KeyData)
{
    KeyData key1;
    key1.fifths = 4;
    key1.cancel = -1;
    key1.mode = KeyMode::major;
    key1.staffIndex = 0;
    key1.nonTraditional.push_back(KeyComponent{Step::c, 1, 0.0, Accidental::sharp});
    key1.nonTraditional.push_back(KeyComponent{Step::d, 0, -50.0, Accidental::quarterFlat});
    key1.tickTimePosition = 13;
    auto key2 = key1;
    CHECK(key1 == key2);
    CHECK(!(key1 != key2));

    // change one thing
    key1.cancel += 1;
    CHECK(key1 != key2);
    CHECK(!(key1 == key2));
}

TEST(KeyDataEquality_change_mode, KeyData)
{
    KeyData key1;
    key1.fifths = 4;
    key1.cancel = -1;
    key1.mode = KeyMode::major;
    key1.staffIndex = 0;
    key1.nonTraditional.push_back(KeyComponent{Step::c, 1, 0.0, Accidental::sharp});
    key1.nonTraditional.push_back(KeyComponent{Step::d, 0, -50.0, Accidental::quarterFlat});
    key1.tickTimePosition = 13;
    auto key2 = key1;
    CHECK(key1 == key2);
    CHECK(!(key1 != key2));

    // change one thing
    key1.mode = KeyMode::unspecified;
    CHECK(key1 != key2);
    CHECK(!(key1 == key2));
}

TEST(KeyDataEquality_change_staffIndex, KeyData)
{
    KeyData key1;
    key1.fifths = 4;
    key1.cancel = -1;
    key1.mode = KeyMode::major;
    key1.staffIndex = 0;
    key1.nonTraditional.push_back(KeyComponent{Step::c, 1, 0.0, Accidental::sharp});
    key1.nonTraditional.push_back(KeyComponent{Step::d, 0, -50.0, Accidental::quarterFlat});
    key1.tickTimePosition = 13;
    auto key2 = key1;
    CHECK(key1 == key2);
    CHECK(!(key1 != key2));

    // change one thing
    key1.staffIndex += 1;
    CHECK(key1 != key2);
    CHECK(!(key1 == key2));
}

TEST(KeyDataEquality_change_nonTraditionalKey, KeyData)
{
    KeyData key1;
    key1.fifths = 4;
    key1.cancel = -1;
    key1.mode = KeyMode::major;
    key1.staffIndex = 0;
    key1.nonTraditional.push_back(KeyComponent{Step::c, 1, 0.0, Accidental::sharp});
    key1.nonTraditional.push_back(KeyComponent{Step::d, 0, -50.0, Accidental::quarterFlat});
    key1.tickTimePosition = 13;
    auto key2 = key1;
    CHECK(key1 == key2);
    CHECK(!(key1 != key2));

    // change one thing
    key1.nonTraditional.at(0).cents += 0.01;
    CHECK(key1 != key2);
    CHECK(!(key1 == key2));
}

TEST(KeyComponentEquality_change_alter, KeyComponent)
{
    KeyComponent kc1;
    kc1.cents = 1.0;
    kc1.alter = 2;
    kc1.accidental = Accidental::sori;
    kc1.step = Step::d;
    auto kc2 = kc1;
    CHECK(kc1 == kc2);
    CHECK(!(kc1 != kc2));

    // change one thing
    kc1.alter += 1;
    CHECK(kc1 != kc2);
    CHECK(!(kc1 == kc2));
}

TEST(KeyComponentEquality_change_accidental, KeyComponent)
{
    KeyComponent kc1;
    kc1.cents = 1.0;
    kc1.alter = 2;
    kc1.accidental = Accidental::sori;
    kc1.step = Step::d;
    auto kc2 = kc1;
    CHECK(kc1 == kc2);
    CHECK(!(kc1 != kc2));

    // change one thing
    kc1.accidental = Accidental::flat4;
    CHECK(kc1 != kc2);
    CHECK(!(kc1 == kc2));
}

TEST(KeyComponentEquality_change_step, KeyComponent)
{
    KeyComponent kc1;
    kc1.cents = 1.0;
    kc1.alter = 2;
    kc1.accidental = Accidental::sori;
    kc1.step = Step::d;
    auto kc2 = kc1;
    CHECK(kc1 == kc2);
    CHECK(!(kc1 != kc2));

    // change one thing
    kc1.step = Step::e;
    CHECK(kc1 != kc2);
    CHECK(!(kc1 == kc2));
}

TEST(IsInitialized, KeyComponent)
{
    // https://github.com/webern/mx/issues/85
    KeyComponent k;
    CHECK(k.step == Step{});
    CHECK(k.alter == 0);
    CHECK(k.cents == 0.0);
    CHECK(k.accidental == Accidental{});
}

TEST(CancelLocationBeforeBarline, KeyData)
{
    // https://github.com/webern/mx/issues/272
    KeyData key;
    key.fifths = 0;
    key.cancel = 2;
    key.cancelLocation = CancelLocation::beforeBarline;

    const auto original = putKeyInScore(key);
    auto &docMgr = DocumentManager::getInstance();
    const auto originalIdResult = docMgr.createFromScore(original);
    REQUIRE(originalIdResult.ok());
    const int originalId = originalIdResult.value();
    const mx::core::DocumentPtr corePtr = docMgr.getDocument(originalId);

    const auto &coreKey = getFirstCoreKey(corePtr);
    const auto &coreKeyChoice = coreKey.choice();
    CHECK(coreKeyChoice.isTraditionalKey());
    const auto &coreTraditionalKey = coreKeyChoice.asTraditionalKey();

    // check that the cancel and its location were written
    REQUIRE(coreTraditionalKey.cancel().has_value());
    CHECK_EQUAL(2, coreTraditionalKey.cancel()->value().value());
    CHECK(coreTraditionalKey.cancel()->location().has_value())
    if (coreTraditionalKey.cancel()->location().has_value())
    {
        CHECK(core::CancelLocation::Tag::beforeBarline == coreTraditionalKey.cancel()->location()->tag());
    }

    // serialize and deserialize
    std::stringstream xml;
    docMgr.writeToStream(originalId, xml);
    docMgr.destroyDocument(originalId);
    CHECK(xml.str().find("location=\"before-barline\"") != std::string::npos);
    std::istringstream iss{xml.str()};
    const auto deserializedIdResult = docMgr.createFromStream(iss);
    REQUIRE(deserializedIdResult.ok());
    const int deserializedId = deserializedIdResult.value();
    const auto deserializedScoreResult = docMgr.getData(deserializedId);
    docMgr.destroyDocument(deserializedId);
    REQUIRE(deserializedScoreResult.ok());
    const auto &deserializedScore = deserializedScoreResult.value();
    const auto &deserializedKeys = deserializedScore.parts.at(0).measures.at(0).keys;
    CHECK_EQUAL(1, deserializedKeys.size())
    const auto deserializedKey = deserializedKeys.at(0);

    CHECK_EQUAL(key.cancel, deserializedKey.cancel);
    CHECK_EQUAL(CancelLocation::beforeBarline, deserializedKey.cancelLocation);
}

TEST(CancelLocationUnspecified, KeyData)
{
    // when cancelLocation is unspecified, no location attribute is written
    KeyData key;
    key.fifths = 0;
    key.cancel = -3;

    const auto original = putKeyInScore(key);
    auto &docMgr = DocumentManager::getInstance();
    const auto originalIdResult = docMgr.createFromScore(original);
    REQUIRE(originalIdResult.ok());
    const int originalId = originalIdResult.value();
    const mx::core::DocumentPtr corePtr = docMgr.getDocument(originalId);

    const auto &coreKey = getFirstCoreKey(corePtr);
    const auto &coreKeyChoice = coreKey.choice();
    CHECK(coreKeyChoice.isTraditionalKey());
    const auto &coreTraditionalKey = coreKeyChoice.asTraditionalKey();

    REQUIRE(coreTraditionalKey.cancel().has_value());
    CHECK(!coreTraditionalKey.cancel()->location().has_value())

    // serialize and deserialize
    std::stringstream xml;
    docMgr.writeToStream(originalId, xml);
    docMgr.destroyDocument(originalId);
    CHECK(xml.str().find("location=") == std::string::npos);
    std::istringstream iss{xml.str()};
    const auto deserializedIdResult = docMgr.createFromStream(iss);
    REQUIRE(deserializedIdResult.ok());
    const int deserializedId = deserializedIdResult.value();
    const auto deserializedScoreResult = docMgr.getData(deserializedId);
    docMgr.destroyDocument(deserializedId);
    REQUIRE(deserializedScoreResult.ok());
    const auto &deserializedScore = deserializedScoreResult.value();
    const auto &deserializedKeys = deserializedScore.parts.at(0).measures.at(0).keys;
    CHECK_EQUAL(1, deserializedKeys.size())
    const auto deserializedKey = deserializedKeys.at(0);

    CHECK_EQUAL(key.cancel, deserializedKey.cancel);
    CHECK_EQUAL(CancelLocation::unspecified, deserializedKey.cancelLocation);
}

TEST(CancelLocationFromXml, KeyData)
{
    // mirrors data/synthetic/cancel.location.3.0.xml, but with location="right"
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>P</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <key>
          <cancel location="right">2</cancel>
          <fifths>0</fifths>
          <mode>major</mode>
        </key>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";
    const auto score = mxtest::fromXml(xml);
    REQUIRE(!score.parts.empty());
    REQUIRE(!score.parts.at(0).measures.empty());
    const auto &keys = score.parts.at(0).measures.at(0).keys;
    REQUIRE(!keys.empty());
    CHECK_EQUAL(2, keys.at(0).cancel);
    CHECK_EQUAL(CancelLocation::right, keys.at(0).cancelLocation);
}

TEST(ModeSerializationAllValues, KeyData)
{
    // every mode with a MusicXML spelling writes that exact spelling
    CHECK(keyModeXml(0, KeyMode::major).find("<mode>major</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::minor).find("<mode>minor</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::dorian).find("<mode>dorian</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::phrygian).find("<mode>phrygian</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::lydian).find("<mode>lydian</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::mixolydian).find("<mode>mixolydian</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::aeolian).find("<mode>aeolian</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::ionian).find("<mode>ionian</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::locrian).find("<mode>locrian</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::none).find("<mode>none</mode>") != std::string::npos);

    // unspecified and unsupported have no spelling, so no <mode> element is written
    CHECK(keyModeXml(0, KeyMode::unspecified).find("<mode>") == std::string::npos);
    CHECK(keyModeXml(0, KeyMode::unsupported).find("<mode>") == std::string::npos);
}

TEST(ModeDeserializationAllValues, KeyData)
{
    CHECK_EQUAL(KeyMode::major, keyFromXml("<fifths>0</fifths><mode>major</mode>").mode);
    CHECK_EQUAL(KeyMode::minor, keyFromXml("<fifths>0</fifths><mode>minor</mode>").mode);
    CHECK_EQUAL(KeyMode::dorian, keyFromXml("<fifths>0</fifths><mode>dorian</mode>").mode);
    CHECK_EQUAL(KeyMode::phrygian, keyFromXml("<fifths>0</fifths><mode>phrygian</mode>").mode);
    CHECK_EQUAL(KeyMode::lydian, keyFromXml("<fifths>0</fifths><mode>lydian</mode>").mode);
    CHECK_EQUAL(KeyMode::mixolydian, keyFromXml("<fifths>0</fifths><mode>mixolydian</mode>").mode);
    CHECK_EQUAL(KeyMode::aeolian, keyFromXml("<fifths>0</fifths><mode>aeolian</mode>").mode);
    CHECK_EQUAL(KeyMode::ionian, keyFromXml("<fifths>0</fifths><mode>ionian</mode>").mode);
    CHECK_EQUAL(KeyMode::locrian, keyFromXml("<fifths>0</fifths><mode>locrian</mode>").mode);
    CHECK_EQUAL(KeyMode::none, keyFromXml("<fifths>0</fifths><mode>none</mode>").mode);
}

TEST(ModeAbsentIsUnspecified, KeyData)
{
    // <mode> is optional; when it is absent the key states no mode
    const auto key = keyFromXml("<fifths>-3</fifths>");
    CHECK_EQUAL(KeyMode::unspecified, key.mode);
    CHECK_EQUAL(-3, key.fifths);
}

TEST(ModeOutsideVocabularyIsUnsupported, KeyData)
{
    // <mode> is an open vocabulary in MusicXML; a value we do not model reads as unsupported
    CHECK_EQUAL(KeyMode::unsupported, keyFromXml("<fifths>0</fifths><mode>banana</mode>").mode);
}

TEST(ModeNoneRoundTrip, KeyData)
{
    // a keyless signature: zero fifths with <mode>none</mode>
    KeyData key;
    key.fifths = 0;
    key.mode = KeyMode::none;

    const auto xml = mxtest::toXml(putKeyInScore(key));
    CHECK(xml.find("<fifths>0</fifths>") != std::string::npos);
    CHECK(xml.find("<mode>none</mode>") != std::string::npos);

    const auto score = mxtest::fromXml(xml);
    REQUIRE(!score.parts.empty());
    REQUIRE(!score.parts.at(0).measures.empty());
    const auto &keys = score.parts.at(0).measures.at(0).keys;
    REQUIRE(!keys.empty());
    const auto &deserializedKey = keys.at(0);
    CHECK_EQUAL(0, deserializedKey.fifths);
    CHECK_EQUAL(KeyMode::none, deserializedKey.mode);
    CHECK(deserializedKey.nonTraditional.empty());
    CHECK_EQUAL(key, deserializedKey);
}

TEST(ModeRoundTripAllValues, KeyData)
{
    // every mode with a spelling survives a write/read round trip, and fifths is untouched by it
    const std::vector<KeyMode> modes{KeyMode::major,      KeyMode::minor,   KeyMode::dorian, KeyMode::phrygian,
                                     KeyMode::lydian,     KeyMode::aeolian, KeyMode::ionian, KeyMode::locrian,
                                     KeyMode::mixolydian, KeyMode::none};
    for (const auto mode : modes)
    {
        KeyData key;
        key.fifths = 2;
        key.mode = mode;
        const auto score = mxtest::fromXml(mxtest::toXml(putKeyInScore(key)));
        REQUIRE(!score.parts.empty());
        REQUIRE(!score.parts.at(0).measures.empty());
        const auto &keys = score.parts.at(0).measures.at(0).keys;
        REQUIRE(!keys.empty());
        CHECK_EQUAL(mode, keys.at(0).mode);
        CHECK_EQUAL(2, keys.at(0).fifths);
    }
}

TEST(ZeroFifthsModesAreDistinct, KeyData)
{
    // zero fifths does not imply any particular mode: C major, A minor, keyless, and mode-less are
    // four different key signatures
    KeyData cMajor;
    cMajor.fifths = 0;
    cMajor.mode = KeyMode::major;

    KeyData aMinor = cMajor;
    aMinor.mode = KeyMode::minor;

    KeyData keyless = cMajor;
    keyless.mode = KeyMode::none;

    KeyData modeless = cMajor;
    modeless.mode = KeyMode::unspecified;

    CHECK(cMajor != aMinor);
    CHECK(cMajor != keyless);
    CHECK(cMajor != modeless);
    CHECK(aMinor != keyless);
    CHECK(aMinor != modeless);
    CHECK(keyless != modeless);

    // and the distinction survives serialization
    CHECK(keyModeXml(0, KeyMode::major).find("<mode>major</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::minor).find("<mode>minor</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::none).find("<mode>none</mode>") != std::string::npos);
    CHECK(keyModeXml(0, KeyMode::unspecified).find("<mode>") == std::string::npos);
}

TEST(ModeNoneIsNotNonTraditional, KeyData)
{
    // KeyMode::none is a mode value, not a nontraditional key; it writes a traditional key
    KeyData key;
    key.fifths = 0;
    key.mode = KeyMode::none;

    const auto original = putKeyInScore(key);
    auto &docMgr = DocumentManager::getInstance();
    const auto originalIdResult = docMgr.createFromScore(original);
    REQUIRE(originalIdResult.ok());
    const int originalId = originalIdResult.value();
    const mx::core::DocumentPtr corePtr = docMgr.getDocument(originalId);

    const auto &coreKey = getFirstCoreKey(corePtr);
    CHECK(coreKey.choice().isTraditionalKey());
    const auto &coreTraditionalKey = coreKey.choice().asTraditionalKey();
    CHECK_EQUAL(0, coreTraditionalKey.fifths().value());
    REQUIRE(coreTraditionalKey.mode().has_value());
    CHECK_EQUAL(std::string{"none"}, coreTraditionalKey.mode()->value());

    docMgr.destroyDocument(originalId);
}

TEST(KeyDataEquality_change_cancelLocation, KeyData)
{
    KeyData key1;
    key1.fifths = 4;
    key1.cancel = -1;
    key1.cancelLocation = CancelLocation::left;
    key1.mode = KeyMode::major;
    key1.staffIndex = 0;
    key1.tickTimePosition = 13;
    auto key2 = key1;
    CHECK(key1 == key2);
    CHECK(!(key1 != key2));

    // change one thing
    key1.cancelLocation = CancelLocation::right;
    CHECK(key1 != key2);
    CHECK(!(key1 == key2));
}

#endif
