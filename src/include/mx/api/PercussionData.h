// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"
#include "mx/api/Id.h"
#include "mx/api/PositionData.h"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace mx
{
namespace api
{
// Glass percussion pictograms (MusicXML's <glass> element).
enum class GlassInstrument
{
    glassHarmonica,
    glassHarp,
    windChimes
};

// Metal percussion pictograms (MusicXML's <metal> element).
enum class MetalInstrument
{
    agogo,
    almglocken,
    bell,
    bellPlate,
    bellTree,
    brakeDrum,
    cencerro,
    chainRattle,
    chineseCymbal,
    cowbell,
    crashCymbals,
    crotale,
    cymbalTongs,
    domedGong,
    fingerCymbals,
    flexatone,
    gong,
    hiHat,
    highHatCymbals,
    handbell,
    jawHarp,
    jingleBells,
    musicalSaw,
    shellBells,
    sistrum,
    sizzleCymbal,
    sleighBells,
    suspendedCymbal,
    tamTam,
    tamTamWithBeater,
    triangle,
    vietnameseHat
};

// Wood percussion pictograms (MusicXML's <wood> element).
enum class WoodInstrument
{
    bambooScraper,
    boardClapper,
    cabasa,
    castanets,
    castanetsWithHandle,
    claves,
    footballRattle,
    guiro,
    logDrum,
    maraca,
    maracas,
    quijada,
    rainstick,
    ratchet,
    recoReco,
    sandpaperBlocks,
    slitDrum,
    templeBlock,
    vibraslap,
    whip,
    woodBlock
};

// Pitched percussion pictograms (MusicXML's <pitched> element).
enum class PitchedInstrument
{
    celesta,
    chimes,
    glockenspiel,
    lithophone,
    mallet,
    marimba,
    steelDrums,
    tubaphone,
    tubularChimes,
    vibraphone,
    xylophone
};

// Membrane (drum) percussion pictograms (MusicXML's <membrane> element).
enum class MembraneInstrument
{
    bassDrum,
    bassDrumOnSide,
    bongos,
    chineseTomtom,
    congaDrum,
    cuica,
    gobletDrum,
    indoAmericanTomtom,
    japaneseTomtom,
    militaryDrum,
    snareDrum,
    snareDrumSnaresOff,
    tabla,
    tambourine,
    tenorDrum,
    timbales,
    tomtom
};

// Effect percussion pictograms (MusicXML's <effect> element).
enum class EffectInstrument
{
    anvil,
    autoHorn,
    birdWhistle,
    cannon,
    duckCall,
    gunShot,
    klaxonHorn,
    lionsRoar,
    lotusFlute,
    megaphone,
    policeWhistle,
    siren,
    slideWhistle,
    thunderSheet,
    windMachine,
    windWhistle
};

// Beater pictograms for percussion notation (MusicXML's <beater> element).
enum class BeaterValue
{
    bow,
    chimeHammer,
    coin,
    drumStick,
    finger,
    fingernail,
    fist,
    guiroScraper,
    hammer,
    hand,
    jazzStick,
    knittingNeedle,
    metalHammer,
    slideBrushOnGong,
    snareStick,
    spoonMallet,
    superball,
    triangleBeater,
    triangleBeaterPlain,
    wireBrush
};

// The direction a beater or stick points in a percussion pictogram.
enum class TipDirection
{
    unspecified,
    up,
    down,
    left,
    right,
    northwest,
    northeast,
    southeast,
    southwest
};

// Stick types for percussion pictograms (MusicXML's <stick-type> element).
enum class StickType
{
    bassDrum,
    doubleBassDrum,
    glockenspiel,
    gum,
    hammer,
    superball,
    timpani,
    wound,
    xylophone,
    yarn
};

// Stick materials for percussion pictograms (MusicXML's <stick-material> element).
enum class StickMaterial
{
    soft,
    medium,
    hard,
    shaded,
    x
};

// Where a stick or beater strikes the instrument (MusicXML's <stick-location> element).
enum class StickLocation
{
    center,
    rim,
    cymbalBell,
    cymbalEdge
};

// A pictogram identified by its instrument family enum plus an optional SMuFL glyph override.
// smufl, when present, names the exact SMuFL pictogram glyph to draw instead of the default
// glyph for the value.
struct GlassPercussion
{
    GlassInstrument value;
    std::optional<std::string> smufl;

    GlassPercussion() : value{GlassInstrument::glassHarmonica}, smufl{}
    {
    }
};

MXAPI_EQUALS_BEGIN(GlassPercussion)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(GlassPercussion);

struct MetalPercussion
{
    MetalInstrument value;
    std::optional<std::string> smufl;

    MetalPercussion() : value{MetalInstrument::agogo}, smufl{}
    {
    }
};

MXAPI_EQUALS_BEGIN(MetalPercussion)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MetalPercussion);

struct WoodPercussion
{
    WoodInstrument value;
    std::optional<std::string> smufl;

    WoodPercussion() : value{WoodInstrument::bambooScraper}, smufl{}
    {
    }
};

MXAPI_EQUALS_BEGIN(WoodPercussion)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WoodPercussion);

struct PitchedPercussion
{
    PitchedInstrument value;
    std::optional<std::string> smufl;

    PitchedPercussion() : value{PitchedInstrument::celesta}, smufl{}
    {
    }
};

MXAPI_EQUALS_BEGIN(PitchedPercussion)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PitchedPercussion);

struct MembranePercussion
{
    MembraneInstrument value;
    std::optional<std::string> smufl;

    MembranePercussion() : value{MembraneInstrument::bassDrum}, smufl{}
    {
    }
};

MXAPI_EQUALS_BEGIN(MembranePercussion)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MembranePercussion);

struct EffectPercussion
{
    EffectInstrument value;
    std::optional<std::string> smufl;

    EffectPercussion() : value{EffectInstrument::anvil}, smufl{}
    {
    }
};

MXAPI_EQUALS_BEGIN(EffectPercussion)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(EffectPercussion);

// The timpani pictogram (MusicXML's <timpani> element): an empty element, optionally overridden
// with a specific SMuFL pictogram glyph.
struct TimpaniPercussion
{
    std::optional<std::string> smufl;
};

MXAPI_EQUALS_BEGIN(TimpaniPercussion)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TimpaniPercussion);

// A beater pictogram (MusicXML's <beater> element): what strikes the instrument, and which way
// the tip points.
struct BeaterPercussion
{
    BeaterValue value;
    TipDirection tip;

    BeaterPercussion() : value{BeaterValue::bow}, tip{TipDirection::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(BeaterPercussion)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(tip)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(BeaterPercussion);

// A stick pictogram (MusicXML's <stick> element): the stick's type and material, which way its
// tip points, and whether the pictogram is shown in parentheses or a dashed circle (both used
// for alternate or optional stickings).
struct StickPercussion
{
    StickType stickType;
    StickMaterial stickMaterial;
    TipDirection tip;
    Bool parentheses;
    Bool dashedCircle;

    StickPercussion()
        : stickType{StickType::bassDrum}, stickMaterial{StickMaterial::soft}, tip{TipDirection::unspecified},
          parentheses{Bool::unspecified}, dashedCircle{Bool::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(StickPercussion)
MXAPI_EQUALS_MEMBER(stickType)
MXAPI_EQUALS_MEMBER(stickMaterial)
MXAPI_EQUALS_MEMBER(tip)
MXAPI_EQUALS_MEMBER(parentheses)
MXAPI_EQUALS_MEMBER(dashedCircle)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(StickPercussion);

// A percussion instrument or technique with no dedicated pictogram element, carried by
// MusicXML's <other-percussion> catch-all: descriptive text plus an optional SMuFL glyph name.
struct OtherPercussion
{
    std::string text;
    std::optional<std::string> smufl;
};

MXAPI_EQUALS_BEGIN(OtherPercussion)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OtherPercussion);

// The pictogram carried by one PercussionData: exactly one of the instrument-family or
// beater/stick alternatives. Follows the TimeChoice/MarkDataChoice pattern: a wrong-kind
// accessor returns a default-constructed copy, never throws.
class PercussionDataChoice
{
  public:
    enum class Kind
    {
        glass,
        metal,
        wood,
        pitched,
        membrane,
        effect,
        timpani,
        beater,
        stick,
        stickLocation,
        otherPercussion
    };

    // Defaults to a glass pictogram, the first alternative.
    PercussionDataChoice();

    PercussionDataChoice(GlassPercussion value);
    PercussionDataChoice(MetalPercussion value);
    PercussionDataChoice(WoodPercussion value);
    PercussionDataChoice(PitchedPercussion value);
    PercussionDataChoice(MembranePercussion value);
    PercussionDataChoice(EffectPercussion value);
    PercussionDataChoice(TimpaniPercussion value);
    PercussionDataChoice(BeaterPercussion value);
    PercussionDataChoice(StickPercussion value);
    PercussionDataChoice(StickLocation value);
    PercussionDataChoice(OtherPercussion value);

    Kind kind() const;
    bool isGlass() const;
    bool isMetal() const;
    bool isWood() const;
    bool isPitched() const;
    bool isMembrane() const;
    bool isEffect() const;
    bool isTimpani() const;
    bool isBeater() const;
    bool isStick() const;
    bool isStickLocation() const;
    bool isOtherPercussion() const;

    // Each accessor returns a copy of the held alternative. Check the matching is...() first;
    // a wrong-kind access returns a default-constructed value.
    GlassPercussion glass() const;
    MetalPercussion metal() const;
    WoodPercussion wood() const;
    PitchedPercussion pitched() const;
    MembranePercussion membrane() const;
    EffectPercussion effect() const;
    TimpaniPercussion timpani() const;
    BeaterPercussion beater() const;
    StickPercussion stick() const;
    StickLocation stickLocation() const;
    OtherPercussion otherPercussion() const;

    bool operator==(const PercussionDataChoice &other) const;

  private:
    std::variant<GlassPercussion, MetalPercussion, WoodPercussion, PitchedPercussion, MembranePercussion,
                 EffectPercussion, TimpaniPercussion, BeaterPercussion, StickPercussion, StickLocation, OtherPercussion>
        myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(PercussionDataChoice);

// A percussion pictogram, MusicXML's <percussion> element: a symbol for an unpitched instrument
// or the implement striking it, used in percussion parts and legends. Several PercussionData in
// one direction read as a group (for example a membrane pictogram followed by a beater).
// positionData captures default/relative x-y plus the horizontal and vertical alignment; its
// placement member is unused here because <percussion> has no placement attribute (placement
// lives on the parent <direction>).
class PercussionData
{
  public:
    PercussionDataChoice choice;

    // A shape drawn around the pictogram. Enclosure::unspecified draws no enclosure;
    // Enclosure::none states explicitly that there is none.
    Enclosure enclosure;

    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<Id> id;

    PercussionData() : choice{}, enclosure{Enclosure::unspecified}, positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(PercussionData)
MXAPI_EQUALS_MEMBER(choice)
MXAPI_EQUALS_MEMBER(enclosure)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PercussionData);
} // namespace api
} // namespace mx
