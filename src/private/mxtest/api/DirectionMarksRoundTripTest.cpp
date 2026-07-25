// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"

#include <sstream>

using namespace std;
using namespace mx::api;

// Round-trips a DirectionData through the full serialize -> deserialize path and
// returns the directions read back from the first staff. Used by the direction-type
// data-loss regression tests below (#324).
static std::vector<DirectionData> roundTripDirectionData(const DirectionData &inDirectionData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.directions.push_back(inDirectionData);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    if (!r1.ok())
        return {};
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    if (!r2.ok())
        return {};
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    mgr.destroyDocument(docId);
    if (!rd.ok())
        return {};
    const auto &oscore = rd.value();

    return oscore.parts.back().measures.back().staves.back().directions;
}

TEST(Damp, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.directionTypes.emplace_back(DirectionChoice{DampData{}});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    CHECK(directions.front().directionTypes.front().isDamp());
}

T_END;

TEST(DampAll, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.directionTypes.emplace_back(DirectionChoice{DampAllData{}});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    CHECK(directions.front().directionTypes.front().isDampAll());
}

T_END;

TEST(Eyeglasses, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.directionTypes.emplace_back(DirectionChoice{EyeglassesData{}});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    CHECK(directions.front().directionTypes.front().isEyeglasses());
}

T_END;

TEST(StringMute, DirectionMarksRoundTrip)
{
    DirectionData direction;
    StringMuteData stringMute;
    stringMute.type = StringMuteType::off;
    direction.directionTypes.emplace_back(DirectionChoice{stringMute});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isStringMute());
    CHECK(directions.front().directionTypes.front().stringMute().type == StringMuteType::off);
}

T_END;

TEST(StaffDivide, DirectionMarksRoundTrip)
{
    DirectionData direction;
    StaffDivideData staffDivide;
    staffDivide.type = StaffDivideType::upDown;
    direction.directionTypes.emplace_back(DirectionChoice{staffDivide});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isStaffDivide());
    CHECK(directions.front().directionTypes.front().staffDivide().type == StaffDivideType::upDown);
}

T_END;

TEST(PrincipalVoice, DirectionMarksRoundTrip)
{
    DirectionData direction;
    PrincipalVoiceData principalVoice;
    principalVoice.type = PrincipalVoiceType::start;
    principalVoice.symbol = PrincipalVoiceSymbol::nebenstimme;
    principalVoice.text = "N";
    direction.directionTypes.emplace_back(DirectionChoice{principalVoice});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isPrincipalVoice());
    const auto out = directions.front().directionTypes.front().principalVoice();
    CHECK(out.type == PrincipalVoiceType::start);
    CHECK(out.symbol == PrincipalVoiceSymbol::nebenstimme);
    CHECK_EQUAL("N", out.text);
}

T_END;

TEST(OtherDirection, DirectionMarksRoundTrip)
{
    DirectionData direction;
    OtherDirectionData otherDirection;
    otherDirection.text = "con sordino misterioso";
    otherDirection.printObject = Bool::no;
    otherDirection.smufl = "luteFingeringRHThumb";
    direction.directionTypes.emplace_back(DirectionChoice{otherDirection});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isOtherDirection());
    const auto out = directions.front().directionTypes.front().otherDirection();
    CHECK_EQUAL("con sordino misterioso", out.text);
    CHECK(out.printObject == Bool::no);
    REQUIRE(out.smufl.has_value());
    CHECK_EQUAL("luteFingeringRHThumb", *out.smufl);
}

T_END;

TEST(Image, DirectionMarksRoundTrip)
{
    DirectionData direction;
    ImageData image;
    image.source = "logo.png";
    image.type = "image/png";
    image.height = 40.0;
    image.width = 80.0;
    image.positionData.verticalAlignment = VerticalAlignment::middle;
    direction.directionTypes.emplace_back(DirectionChoice{image});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isImage());
    const auto out = directions.front().directionTypes.front().image();
    CHECK_EQUAL("logo.png", out.source);
    CHECK_EQUAL("image/png", out.type);
    REQUIRE(out.height.has_value());
    CHECK_DOUBLES_EQUAL(40.0, *out.height, 0.0001);
    REQUIRE(out.width.has_value());
    CHECK_DOUBLES_EQUAL(80.0, *out.width, 0.0001);
    CHECK(out.positionData.verticalAlignment == VerticalAlignment::middle);
}

T_END;

TEST(AccordionRegistration, DirectionMarksRoundTrip)
{
    DirectionData direction;
    AccordionRegistrationData accordion;
    accordion.high = true;
    accordion.middle = 2;
    accordion.low = true;
    direction.directionTypes.emplace_back(DirectionChoice{accordion});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isAccordionRegistration());
    const auto out = directions.front().directionTypes.front().accordionRegistration();
    CHECK(out.high);
    REQUIRE(out.middle.has_value());
    CHECK_EQUAL(2, *out.middle);
    CHECK(out.low);
}

T_END;

TEST(AccordionRegistrationEmpty, DirectionMarksRoundTrip)
{
    // A registration with nothing engaged is legal and draws the empty diagram.
    DirectionData direction;
    direction.directionTypes.emplace_back(DirectionChoice{AccordionRegistrationData{}});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isAccordionRegistration());
    const auto out = directions.front().directionTypes.front().accordionRegistration();
    CHECK(!out.high);
    CHECK(!out.middle.has_value());
    CHECK(!out.low);
}

T_END;

TEST(HarpPedals, DirectionMarksRoundTrip)
{
    DirectionData direction;
    HarpPedalsData harpPedals;
    harpPedals.pedalTunings.emplace_back(Step::d, 0);
    harpPedals.pedalTunings.emplace_back(Step::c, -1);
    harpPedals.pedalTunings.emplace_back(Step::b, 1);
    direction.directionTypes.emplace_back(DirectionChoice{harpPedals});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isHarpPedals());
    const auto out = directions.front().directionTypes.front().harpPedals();
    REQUIRE(out.pedalTunings.size() == 3);
    CHECK(out.pedalTunings.at(0).step == Step::d);
    CHECK_EQUAL(0, out.pedalTunings.at(0).alter);
    CHECK(out.pedalTunings.at(1).step == Step::c);
    CHECK_EQUAL(-1, out.pedalTunings.at(1).alter);
    CHECK(out.pedalTunings.at(2).step == Step::b);
    CHECK_EQUAL(1, out.pedalTunings.at(2).alter);
}

T_END;

TEST(PedalAllKinds, DirectionMarksRoundTrip)
{
    // Every pedal-line type must survive a round-trip; before #324 only start/stop did and the
    // rest (sostenuto/change/continueLine/discontinue/resume) were silently dropped.
    const PedalLineKind kinds[] = {
        PedalLineKind::start,        PedalLineKind::stop,        PedalLineKind::sostenuto, PedalLineKind::change,
        PedalLineKind::continueLine, PedalLineKind::discontinue, PedalLineKind::resume,
    };

    for (const auto kind : kinds)
    {
        DirectionData direction;
        direction.directionTypes.emplace_back(DirectionChoice{PedalLineData{kind}});
        const auto directions = roundTripDirectionData(direction);
        REQUIRE(directions.size() == 1);
        REQUIRE(directions.front().directionTypes.size() == 1);
        REQUIRE(directions.front().directionTypes.front().isPedal());
        CHECK(directions.front().directionTypes.front().pedal().kind == kind);
    }
}

T_END;

TEST(PedalPlacement, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.placement = Placement::below;
    PedalLineData pedal{PedalLineKind::start};
    pedal.positionData.placement = Placement::below;
    direction.directionTypes.emplace_back(DirectionChoice{pedal});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isPedal());
    CHECK(directions.front().directionTypes.front().pedal().kind == PedalLineKind::start);
    CHECK(directions.front().placement == Placement::below);
}

T_END;

TEST(Scordatura, DirectionMarksRoundTrip)
{
    DirectionData direction;
    ScordaturaData scordatura;
    AccordData accord;
    accord.stringNumber = 6;
    accord.tuningStep = Step::d;
    accord.tuningOctave = 2;
    scordatura.accords.push_back(accord);
    direction.directionTypes.emplace_back(DirectionChoice{scordatura});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isScordatura());
    const auto out = directions.front().directionTypes.front().scordatura();
    REQUIRE(out.accords.size() == 1);
    REQUIRE(out.accords.front().stringNumber.has_value());
    CHECK_EQUAL(6, *out.accords.front().stringNumber);
    CHECK(out.accords.front().tuningStep == Step::d);
    CHECK_EQUAL(0, out.accords.front().tuningAlter);
    CHECK_EQUAL(2, out.accords.front().tuningOctave);
}

T_END;

TEST(ScordaturaNoStringNumbers, DirectionMarksRoundTrip)
{
    // When stringNumber is absent the accords run through the strings in order.
    DirectionData direction;
    ScordaturaData scordatura;
    AccordData accord;
    accord.tuningStep = Step::a;
    accord.tuningAlter = -1;
    accord.tuningOctave = 3;
    scordatura.accords.push_back(accord);
    direction.directionTypes.emplace_back(DirectionChoice{scordatura});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isScordatura());
    const auto out = directions.front().directionTypes.front().scordatura();
    REQUIRE(out.accords.size() == 1);
    CHECK(!out.accords.front().stringNumber.has_value());
    CHECK(out.accords.front().tuningStep == Step::a);
    CHECK_EQUAL(-1, out.accords.front().tuningAlter);
    CHECK_EQUAL(3, out.accords.front().tuningOctave);
}

T_END;

TEST(PercussionGlass, DirectionMarksRoundTrip)
{
    DirectionData direction;
    PercussionData percussion;
    GlassPercussion glass;
    glass.value = GlassInstrument::windChimes;
    percussion.choice = PercussionDataChoice{glass};
    percussion.enclosure = PercussionEnclosure::rectangle;
    direction.directionTypes.emplace_back(DirectionChoice{percussion});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isPercussion());
    const auto out = directions.front().directionTypes.front().percussion();
    REQUIRE(out.choice.isGlass());
    CHECK(out.choice.glass().value == GlassInstrument::windChimes);
    CHECK(out.enclosure == PercussionEnclosure::rectangle);
}

T_END;

TEST(PercussionBeaterTip, DirectionMarksRoundTrip)
{
    DirectionData direction;
    PercussionData percussion;
    BeaterPercussion beater;
    beater.value = BeaterValue::wireBrush;
    beater.tip = TipDirection::northeast;
    percussion.choice = PercussionDataChoice{beater};
    direction.directionTypes.emplace_back(DirectionChoice{percussion});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isPercussion());
    const auto out = directions.front().directionTypes.front().percussion();
    REQUIRE(out.choice.isBeater());
    CHECK(out.choice.beater().value == BeaterValue::wireBrush);
    CHECK(out.choice.beater().tip == TipDirection::northeast);
}

T_END;

TEST(PercussionStick, DirectionMarksRoundTrip)
{
    DirectionData direction;
    PercussionData percussion;
    StickPercussion stick;
    stick.stickType = StickType::timpani;
    stick.stickMaterial = StickMaterial::hard;
    stick.tip = TipDirection::down;
    stick.parentheses = Bool::yes;
    percussion.choice = PercussionDataChoice{stick};
    direction.directionTypes.emplace_back(DirectionChoice{percussion});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isPercussion());
    const auto out = directions.front().directionTypes.front().percussion();
    REQUIRE(out.choice.isStick());
    CHECK(out.choice.stick().stickType == StickType::timpani);
    CHECK(out.choice.stick().stickMaterial == StickMaterial::hard);
    CHECK(out.choice.stick().tip == TipDirection::down);
    CHECK(out.choice.stick().parentheses == Bool::yes);
}

T_END;

TEST(PercussionTimpaniSmufl, DirectionMarksRoundTrip)
{
    DirectionData direction;
    PercussionData percussion;
    TimpaniPercussion timpani;
    timpani.smufl = "pictGong";
    percussion.choice = PercussionDataChoice{timpani};
    direction.directionTypes.emplace_back(DirectionChoice{percussion});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isPercussion());
    const auto out = directions.front().directionTypes.front().percussion();
    REQUIRE(out.choice.isTimpani());
    REQUIRE(out.choice.timpani().smufl.has_value());
    CHECK_EQUAL("pictGong", *out.choice.timpani().smufl);
}

T_END;

TEST(PercussionStickLocationAndOther, DirectionMarksRoundTrip)
{
    DirectionData direction;
    PercussionData stickLocationPercussion;
    stickLocationPercussion.choice = PercussionDataChoice{StickLocation::cymbalEdge};
    direction.directionTypes.emplace_back(DirectionChoice{stickLocationPercussion});
    PercussionData otherPercussionData;
    OtherPercussion other;
    other.text = "ocean drum";
    otherPercussionData.choice = PercussionDataChoice{other};
    direction.directionTypes.emplace_back(DirectionChoice{otherPercussionData});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 2);
    REQUIRE(directions.front().directionTypes.at(0).isPercussion());
    REQUIRE(directions.front().directionTypes.at(0).percussion().choice.isStickLocation());
    CHECK(directions.front().directionTypes.at(0).percussion().choice.stickLocation() == StickLocation::cymbalEdge);
    REQUIRE(directions.front().directionTypes.at(1).isPercussion());
    REQUIRE(directions.front().directionTypes.at(1).percussion().choice.isOtherPercussion());
    CHECK_EQUAL("ocean drum", directions.front().directionTypes.at(1).percussion().choice.otherPercussion().text);
}

T_END;

TEST(PercussionMembraneMetalWoodPitchedEffect, DirectionMarksRoundTrip)
{
    DirectionData direction;
    PercussionData membraneData;
    MembranePercussion membrane;
    membrane.value = MembraneInstrument::congaDrum;
    membraneData.choice = PercussionDataChoice{membrane};
    direction.directionTypes.emplace_back(DirectionChoice{membraneData});
    PercussionData metalData;
    MetalPercussion metal;
    metal.value = MetalInstrument::tamTamWithBeater;
    metalData.choice = PercussionDataChoice{metal};
    direction.directionTypes.emplace_back(DirectionChoice{metalData});
    PercussionData woodData;
    WoodPercussion wood;
    wood.value = WoodInstrument::templeBlock;
    woodData.choice = PercussionDataChoice{wood};
    direction.directionTypes.emplace_back(DirectionChoice{woodData});
    PercussionData pitchedData;
    PitchedPercussion pitched;
    pitched.value = PitchedInstrument::steelDrums;
    pitchedData.choice = PercussionDataChoice{pitched};
    direction.directionTypes.emplace_back(DirectionChoice{pitchedData});
    PercussionData effectData;
    EffectPercussion effect;
    effect.value = EffectInstrument::windMachine;
    effectData.choice = PercussionDataChoice{effect};
    direction.directionTypes.emplace_back(DirectionChoice{effectData});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 5);
    const auto &out = directions.front().directionTypes;
    CHECK(out.at(0).percussion().choice.membrane().value == MembraneInstrument::congaDrum);
    CHECK(out.at(1).percussion().choice.metal().value == MetalInstrument::tamTamWithBeater);
    CHECK(out.at(2).percussion().choice.wood().value == WoodInstrument::templeBlock);
    CHECK(out.at(3).percussion().choice.pitched().value == PitchedInstrument::steelDrums);
    CHECK(out.at(4).percussion().choice.effect().value == EffectInstrument::windMachine);
}

T_END;

TEST(DampFormatting, DirectionMarksRoundTrip)
{
    DirectionData direction;
    DampData damp;
    damp.positionData.isDefaultXSpecified = true;
    damp.positionData.defaultX = 5.0;
    damp.id = "damp-1";
    direction.directionTypes.emplace_back(DirectionChoice{damp});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isDamp());
    const auto outDamp = directions.front().directionTypes.front().damp();
    CHECK(outDamp.positionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(5.0, outDamp.positionData.defaultX, 0.0001);
    REQUIRE(outDamp.id.has_value());
    CHECK_EQUAL("damp-1", *outDamp.id);
}

T_END;

// Issue #294: a <direction-type> holding an ordered mix of <words> and <symbol> elements must
// survive the api round trip with the interleaving intact -- e.g. "gliss." followed by an
// arrowBlackUp glyph followed by "al niente".
TEST(WordsSymbolInterleaved, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.placement = Placement::below;

    std::vector<WordsChoice> run;
    WordsData before;
    before.text = "gliss. ";
    run.emplace_back(before);
    SymbolData glyph;
    glyph.smufl = "arrowBlackUp";
    run.emplace_back(glyph);
    WordsData after;
    after.text = " al niente";
    run.emplace_back(after);
    direction.directionTypes.emplace_back(DirectionChoice{run});

    MarkData dynamic{MarkType::ff};
    direction.directionTypes.emplace_back(DirectionChoice{dynamic});

    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 2);

    REQUIRE(directions.front().directionTypes.front().isWordsRun());
    const auto outRun = directions.front().directionTypes.front().wordsRun();
    REQUIRE(outRun.size() == 3);
    REQUIRE(outRun.at(0).isWords());
    CHECK_EQUAL("gliss. ", outRun.at(0).words().text);
    REQUIRE(outRun.at(1).isSymbol());
    CHECK_EQUAL("arrowBlackUp", outRun.at(1).symbol().smufl);
    REQUIRE(outRun.at(2).isWords());
    CHECK_EQUAL(" al niente", outRun.at(2).words().text);

    REQUIRE(directions.front().directionTypes.back().isMark());
    CHECK(directions.front().directionTypes.back().mark().markType == MarkType::ff);
}

T_END;

// Issue #294: two words-only direction-types stay separate (they are not merged into one), and a
// words direction-type followed by a dynamics direction-type keeps its order.
TEST(WordsBeforeDynamicsOrderPreserved, DirectionMarksRoundTrip)
{
    DirectionData direction;
    direction.placement = Placement::below;

    WordsData piu;
    piu.text = "più";
    direction.directionTypes.emplace_back(DirectionChoice{std::vector<WordsChoice>{WordsChoice{piu}}});
    direction.directionTypes.emplace_back(DirectionChoice{MarkData{MarkType::f}});
    WordsData troppo;
    troppo.text = "ma non troppo";
    direction.directionTypes.emplace_back(DirectionChoice{std::vector<WordsChoice>{WordsChoice{troppo}}});

    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 3);
    REQUIRE(directions.front().directionTypes.at(0).isWordsRun());
    REQUIRE(directions.front().directionTypes.at(0).wordsRun().size() == 1);
    CHECK_EQUAL("più", directions.front().directionTypes.at(0).wordsRun().front().words().text);
    REQUIRE(directions.front().directionTypes.at(1).isMark());
    CHECK(directions.front().directionTypes.at(1).mark().markType == MarkType::f);
    REQUIRE(directions.front().directionTypes.at(2).isWordsRun());
    REQUIRE(directions.front().directionTypes.at(2).wordsRun().size() == 1);
    CHECK_EQUAL("ma non troppo", directions.front().directionTypes.at(2).wordsRun().front().words().text);
}

T_END;

// Words with an enclosure attribute round-trip; an unspecified enclosure emits no attribute.
TEST(WordsEnclosure, DirectionMarksRoundTrip)
{
    DirectionData direction;
    WordsData words;
    words.text = "boxed";
    words.enclosure = RehearsalEnclosure::rectangle;
    direction.directionTypes.emplace_back(DirectionChoice{std::vector<WordsChoice>{WordsChoice{words}}});
    const auto directions = roundTripDirectionData(direction);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isWordsRun());
    const auto outRun = directions.front().directionTypes.front().wordsRun();
    REQUIRE(outRun.size() == 1);
    REQUIRE(outRun.front().isWords());
    CHECK(outRun.front().words().enclosure == RehearsalEnclosure::rectangle);
}

T_END;

#endif
