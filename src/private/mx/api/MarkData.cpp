// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/MarkData.h"
#include "mx/core/generated/FermataShape.h"
#include "mx/impl/Converter.h"

namespace mx
{
namespace api
{
bool isMarkDynamic(MarkType markType)
{
    return markType == MarkType::dynamics;
}

bool isMarkArpeggiate(MarkType markType)
{
    return (markType == MarkType::arpeggiate) || (markType == MarkType::arpeggiateDown) ||
           (markType == MarkType::arpeggiateUp);
}

bool isMarkArticulation(MarkType markType)
{
    return (markType == MarkType::accent) || (markType == MarkType::strongAccent) ||
           (markType == MarkType::strongAccentUp) || (markType == MarkType::strongAccentDown) ||
           (markType == MarkType::staccato) || (markType == MarkType::tenuto) ||
           (markType == MarkType::detachedLegato) || (markType == MarkType::staccatissimo) ||
           (markType == MarkType::spiccato) || (markType == MarkType::scoop) || (markType == MarkType::plop) ||
           (markType == MarkType::doit) || (markType == MarkType::falloff) || (markType == MarkType::breathMark) ||
           (markType == MarkType::caesura) || (markType == MarkType::caesuraNormal) ||
           (markType == MarkType::caesuraThick) || (markType == MarkType::caesuraShort) ||
           (markType == MarkType::caesuraCurved) || (markType == MarkType::caesuraSingle) ||
           (markType == MarkType::stress) || (markType == MarkType::unstress) || (markType == MarkType::softAccent) ||
           (markType == MarkType::otherArticulation);
}

bool isMarkOrnament(MarkType markType)
{
    return (markType == MarkType::trillMark) || (markType == MarkType::turn) || (markType == MarkType::delayedTurn) ||
           (markType == MarkType::invertedTurn) || (markType == MarkType::delayedInvertedTurn) ||
           (markType == MarkType::verticalTurn) || (markType == MarkType::invertedVerticalTurn) ||
           (markType == MarkType::haydn) || (markType == MarkType::shake) || (markType == MarkType::wavyLine) ||
           (markType == MarkType::mordent) || (markType == MarkType::invertedMordent) ||
           (markType == MarkType::schleifer) || (markType == MarkType::tremoloSingleOne) ||
           (markType == MarkType::tremoloSingleTwo) || (markType == MarkType::tremoloSingleThree) ||
           (markType == MarkType::tremoloSingleFour) || (markType == MarkType::tremoloSingleFive) ||
           (markType == MarkType::tremoloSingleSix) || (markType == MarkType::tremoloSingleSeven) ||
           (markType == MarkType::tremoloSingleEight) || (markType == MarkType::tremoloStart) ||
           (markType == MarkType::tremoloStop) || (markType == MarkType::tremoloUnmeasured) ||
           (markType == MarkType::otherOrnament) || (markType == MarkType::unknownOrnament);
}

bool isMarkFermata(MarkType markType)
{
    return (markType == MarkType::fermata) || (markType == MarkType::fermataNormal) ||
           (markType == MarkType::fermataAngled) || (markType == MarkType::fermataSquare) ||
           (markType == MarkType::fermataDoubleAngled) || (markType == MarkType::fermataDoubleSquare) ||
           (markType == MarkType::fermataDoubleDot) || (markType == MarkType::fermataHalfCurve) ||
           (markType == MarkType::fermataCurlew) || (markType == MarkType::fermataUpright) ||
           (markType == MarkType::fermataNormalUpright) || (markType == MarkType::fermataAngledUpright) ||
           (markType == MarkType::fermataSquareUpright) || (markType == MarkType::fermataInverted) ||
           (markType == MarkType::fermataNormalInverted) || (markType == MarkType::fermataAngledInverted) ||
           (markType == MarkType::fermataSquareInverted);
}

bool isMarkNonArpeggiate(MarkType markType)
{
    return (markType == MarkType::nonArpeggiate);
}

bool isMarkOtherNotation(MarkType markType)
{
    return markType == MarkType::otherNotation;
}

bool isMarkPedal(MarkType markType)
{
    return (markType == MarkType::pedal) || (markType == MarkType::damp);
}

bool isMarkTechnical(MarkType markType)
{
    return (markType == MarkType::upBow) || (markType == MarkType::downBow) || (markType == MarkType::harmonic) ||
           (markType == MarkType::openString) || (markType == MarkType::thumbPosition) ||
           (markType == MarkType::doubleTongue) || (markType == MarkType::tripleTongue) ||
           (markType == MarkType::stopped) || (markType == MarkType::snapPizzicato) || (markType == MarkType::fret) ||
           (markType == MarkType::string_) || (markType == MarkType::fingering) || (markType == MarkType::pluck) ||
           (markType == MarkType::heel) || (markType == MarkType::toe) || (markType == MarkType::fingernails) ||
           (markType == MarkType::hole) || (markType == MarkType::arrow) || (markType == MarkType::handbell) ||
           (markType == MarkType::brassBend) || (markType == MarkType::flip) || (markType == MarkType::smear) ||
           (markType == MarkType::open) || (markType == MarkType::halfMuted) || (markType == MarkType::harmonMute) ||
           (markType == MarkType::golpe) || (markType == MarkType::otherTechnical);
}

bool isMarkTremolo(MarkType markType)
{
    return (markType == MarkType::tremoloSingleOne) || (markType == MarkType::tremoloSingleTwo) ||
           (markType == MarkType::tremoloSingleThree) || (markType == MarkType::tremoloSingleFour) ||
           (markType == MarkType::tremoloSingleFive) || (markType == MarkType::tremoloSingleSix) ||
           (markType == MarkType::tremoloSingleSeven) || (markType == MarkType::tremoloSingleEight) ||
           (markType == MarkType::tremoloStart) || (markType == MarkType::tremoloStop) ||
           (markType == MarkType::tremoloUnmeasured);
}

bool isMarkCustom(MarkType markType)
{
    return (markType == MarkType::customAccentTenuto);
}

std::string getCustomMarkName(MarkType markType)
{
    switch (markType)
    {
    case MarkType::customAccentTenuto:
        return markStringCustomAccentTenuto;
    default:
        break;
    }

    return "";
}

MarkType getMarkTypeFromCustomString(const std::string &inString)
{
    if (inString == markStringCustomAccentTenuto)
    {
        return api::MarkType::customAccentTenuto;
    }

    return api::MarkType::customErrorUnknown;
}

int numTremoloSlashes(MarkType markType)
{
    switch (markType)
    {
    case MarkType::tremoloSingleOne:
        return 1;
    case MarkType::tremoloSingleTwo:
        return 2;
    case MarkType::tremoloSingleThree:
        return 3;
    case MarkType::tremoloSingleFour:
        return 4;
    case MarkType::tremoloSingleFive:
        return 5;
    case MarkType::tremoloSingleSix:
        return 6;
    case MarkType::tremoloSingleSeven:
        return 7;
    case MarkType::tremoloSingleEight:
        return 8;
    default:
        break;
    }

    return VALUE_UNSPECIFIED;
}

MarkData::MarkData()
    : markType(MarkType::unspecified), name{}, tickTimePosition{0}, printData{}, positionData{}, mordentLong{Bool::no},
      hasMordentLong{false}, mordentApproach{Placement::unspecified}, hasMordentApproach{false},
      mordentDeparture{Placement::unspecified}, hasMordentDeparture{false}, fingeringSubstitution{Bool::unspecified},
      fingeringAlternate{Bool::unspecified}, choice{}
{
}

MarkData::MarkData(MarkType inMarkType)
    : markType(inMarkType), name{}, tickTimePosition{0}, printData{}, positionData{}, mordentLong{Bool::no},
      hasMordentLong{false}, mordentApproach{Placement::unspecified}, hasMordentApproach{false},
      mordentDeparture{Placement::unspecified}, hasMordentDeparture{false}, fingeringSubstitution{Bool::unspecified},
      fingeringAlternate{Bool::unspecified}, choice{}
{
    impl::Converter converter;
    if (isMarkArticulation(markType))
    {
        name = "articulation";
    }
    else if (isMarkFermata(markType))
    {
        name = std::string{converter.convertFermata(markType).toString()};
    }
}

MarkData::MarkData(Placement inPlacement, MarkType inMarkType)
    : markType(inMarkType), name{}, tickTimePosition{0}, printData{}, positionData{}, mordentLong{Bool::no},
      hasMordentLong{false}, mordentApproach{Placement::unspecified}, hasMordentApproach{false},
      mordentDeparture{Placement::unspecified}, hasMordentDeparture{false}, fingeringSubstitution{Bool::unspecified},
      fingeringAlternate{Bool::unspecified}, choice{}
{
    positionData.placement = inPlacement;
    impl::Converter converter;
    if (isMarkArticulation(markType))
    {
        name = "articulation";
    }
    else if (isMarkFermata(markType))
    {
        name = std::string{converter.convertFermata(markType).toString()};
    }
}

MarkData::MarkData(StandardDynamic inDynamic)
    : markType(MarkType::dynamics), name{toString(inDynamic)}, tickTimePosition{0}, printData{}, positionData{},
      mordentLong{Bool::no}, hasMordentLong{false}, mordentApproach{Placement::unspecified}, hasMordentApproach{false},
      mordentDeparture{Placement::unspecified}, hasMordentDeparture{false}, fingeringSubstitution{Bool::unspecified},
      fingeringAlternate{Bool::unspecified}, choice{inDynamic}
{
}

MarkData::MarkData(CompoundDynamicsData inDynamics)
    : markType(MarkType::dynamics), name{toString(inDynamics)}, tickTimePosition{0}, printData{}, positionData{},
      mordentLong{Bool::no}, hasMordentLong{false}, mordentApproach{Placement::unspecified}, hasMordentApproach{false},
      mordentDeparture{Placement::unspecified}, hasMordentDeparture{false}, fingeringSubstitution{Bool::unspecified},
      fingeringAlternate{Bool::unspecified}, choice{std::move(inDynamics)}
{
}
} // namespace api
} // namespace mx
