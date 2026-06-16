// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/MarkData.h"
#include "mx/core/generated/DynamicsChoice.h"
#include "mx/core/generated/FermataShape.h"
#include "mx/impl/Converter.h"

namespace mx
{
namespace api
{
namespace
{
// The wire literal of a dynamics alternative (the old core's
// toString(DynamicsEnum); the new variant Kind carries no string).
std::string dynamicsKindToString(core::DynamicsChoice::Kind kind)
{
    switch (kind)
    {
    case core::DynamicsChoice::Kind::p:
        return "p";
    case core::DynamicsChoice::Kind::pp:
        return "pp";
    case core::DynamicsChoice::Kind::ppp:
        return "ppp";
    case core::DynamicsChoice::Kind::pppp:
        return "pppp";
    case core::DynamicsChoice::Kind::ppppp:
        return "ppppp";
    case core::DynamicsChoice::Kind::pppppp:
        return "pppppp";
    case core::DynamicsChoice::Kind::f:
        return "f";
    case core::DynamicsChoice::Kind::ff:
        return "ff";
    case core::DynamicsChoice::Kind::fff:
        return "fff";
    case core::DynamicsChoice::Kind::ffff:
        return "ffff";
    case core::DynamicsChoice::Kind::fffff:
        return "fffff";
    case core::DynamicsChoice::Kind::ffffff:
        return "ffffff";
    case core::DynamicsChoice::Kind::mp:
        return "mp";
    case core::DynamicsChoice::Kind::mf:
        return "mf";
    case core::DynamicsChoice::Kind::sf:
        return "sf";
    case core::DynamicsChoice::Kind::sfp:
        return "sfp";
    case core::DynamicsChoice::Kind::sfpp:
        return "sfpp";
    case core::DynamicsChoice::Kind::fp:
        return "fp";
    case core::DynamicsChoice::Kind::rf:
        return "rf";
    case core::DynamicsChoice::Kind::rfz:
        return "rfz";
    case core::DynamicsChoice::Kind::sfz:
        return "sfz";
    case core::DynamicsChoice::Kind::sffz:
        return "sffz";
    case core::DynamicsChoice::Kind::fz:
        return "fz";
    case core::DynamicsChoice::Kind::n:
        return "n";
    case core::DynamicsChoice::Kind::pf:
        return "pf";
    case core::DynamicsChoice::Kind::sfzp:
        return "sfzp";
    default:
        return "other-dynamics";
    }
}
} // namespace

bool isMarkDynamic(MarkType markType)
{
    return (markType == MarkType::p) || (markType == MarkType::p) || (markType == MarkType::pp) ||
           (markType == MarkType::ppp) || (markType == MarkType::pppp) || (markType == MarkType::ppppp) ||
           (markType == MarkType::pppppp) || (markType == MarkType::f) || (markType == MarkType::ff) ||
           (markType == MarkType::fff) || (markType == MarkType::ffff) || (markType == MarkType::fffff) ||
           (markType == MarkType::ffffff) || (markType == MarkType::mp) || (markType == MarkType::mf) ||
           (markType == MarkType::sf) || (markType == MarkType::sfp) || (markType == MarkType::sfpp) ||
           (markType == MarkType::fp) || (markType == MarkType::rf) || (markType == MarkType::rfz) ||
           (markType == MarkType::sfz) || (markType == MarkType::sffz) || (markType == MarkType::fz) ||
           (markType == MarkType::n) || (markType == MarkType::pf) || (markType == MarkType::sfzp) ||
           (markType == MarkType::otherDynamics);
}

bool isMarkArpeggiate(MarkType markType)
{
    return (markType == MarkType::arpeggiate) || (markType == MarkType::arpeggiateDown) ||
           (markType == MarkType::arpeggiateUp);
}

bool isMarkArticulation(MarkType markType)
{
    return (markType == MarkType::accent) || (markType == MarkType::strongAccent) || (markType == MarkType::staccato) ||
           (markType == MarkType::tenuto) || (markType == MarkType::detachedLegato) ||
           (markType == MarkType::staccatissimo) || (markType == MarkType::spiccato) || (markType == MarkType::scoop) ||
           (markType == MarkType::plop) || (markType == MarkType::doit) || (markType == MarkType::falloff) ||
           (markType == MarkType::breathMark) || (markType == MarkType::caesura) || (markType == MarkType::stress) ||
           (markType == MarkType::unstress) || (markType == MarkType::softAccent) ||
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
           (markType == MarkType::tremoloSingleFive);
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
    if (markType == MarkType::tremoloSingleOne)
    {
        return 1;
    }

    if (markType == MarkType::tremoloSingleTwo)
    {
        return 2;
    }

    if (markType == MarkType::tremoloSingleThree)
    {
        return 3;
    }

    if (markType == MarkType::tremoloSingleFour)
    {
        return 4;
    }

    if (markType == MarkType::tremoloSingleFive)
    {
        return 5;
    }

    return -1;
}

MarkData::MarkData()
    : markType(MarkType::unspecified), name{}, tickTimePosition{0}, printData{}, positionData{}, mordentLong{Bool::no},
      hasMordentLong{false}, mordentApproach{Placement::unspecified}, hasMordentApproach{false},
      mordentDeparture{Placement::unspecified}, hasMordentDeparture{false}, fingeringSubstitution{Bool::unspecified},
      fingeringAlternate{Bool::unspecified}
{
}

MarkData::MarkData(MarkType inMarkType)
    : markType(inMarkType), name{}, tickTimePosition{0}, printData{}, positionData{}, mordentLong{Bool::no},
      hasMordentLong{false}, mordentApproach{Placement::unspecified}, hasMordentApproach{false},
      mordentDeparture{Placement::unspecified}, hasMordentDeparture{false}, fingeringSubstitution{Bool::unspecified},
      fingeringAlternate{Bool::unspecified}
{
    impl::Converter converter;
    if (isMarkDynamic(markType))
    {
        name = dynamicsKindToString(converter.convertDynamic(markType));
    }
    else if (isMarkArticulation(markType))
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
      fingeringAlternate{Bool::unspecified}
{
    positionData.placement = inPlacement;
    impl::Converter converter;
    if (isMarkDynamic(markType))
    {
        name = dynamicsKindToString(converter.convertDynamic(markType));
    }
    else if (isMarkArticulation(markType))
    {
        name = "articulation";
    }
    else if (isMarkFermata(markType))
    {
        name = std::string{converter.convertFermata(markType).toString()};
    }
}
} // namespace api
} // namespace mx
