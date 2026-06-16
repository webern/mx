// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/TechnicalFunctions.h"
#include "mx/core/generated/Arrow.h"
#include "mx/core/generated/ArrowChoice.h"
#include "mx/core/generated/ArrowChoiceGroup.h"
#include "mx/core/generated/ArrowDirection.h"
#include "mx/core/generated/Handbell.h"
#include "mx/core/generated/HandbellValue.h"
#include "mx/core/generated/Hole.h"
#include "mx/core/generated/HoleClosed.h"
#include "mx/core/generated/HoleClosedValue.h"
#include "mx/core/generated/Technical.h"
#include "mx/core/generated/TechnicalChoice.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MarkDataFunctions.h"

namespace
{
std::string holeToSmuflName(const mx::core::Hole &hole)
{
    const auto closedValue = hole.holeClosed().value();
    switch (closedValue.tag())
    {
    case mx::core::HoleClosedValue::Tag::yes:
        return "windClosedHole";
    case mx::core::HoleClosedValue::Tag::half:
        return "windHalfClosedHole3";
    case mx::core::HoleClosedValue::Tag::no:
    default:
        return "windOpenHole";
    }
}

std::string arrowToSmuflName(const mx::core::Arrow &arrow)
{
    using Tag = mx::core::ArrowDirection::Tag;
    if (arrow.choice().kind() != mx::core::ArrowChoice::Kind::group)
    {
        return "arrowOpenUp";
    }

    const auto direction = arrow.choice().asGroup().arrowDirection().tag();
    switch (direction)
    {
    case Tag::left:
        return "arrowOpenLeft";
    case Tag::up:
        return "arrowOpenUp";
    case Tag::right:
        return "arrowOpenRight";
    case Tag::down:
        return "arrowOpenDown";
    case Tag::northwest:
        return "arrowOpenUpLeft";
    case Tag::northeast:
        return "arrowOpenUpRight";
    case Tag::southeast:
        return "arrowOpenDownRight";
    case Tag::southwest:
        return "arrowOpenDownLeft";
    default:
        return "arrowOpenUp";
    }
}

std::string handbellToSmuflName(const mx::core::HandbellValue &value)
{
    using Tag = mx::core::HandbellValue::Tag;
    switch (value.tag())
    {
    case Tag::damp:
        return "handbellsDamp3";
    case Tag::echo:
        return "handbellsEcho1";
    case Tag::gyro:
        return "handbellsGyro";
    case Tag::handMartellato:
        return "handbellsHandMartellato";
    case Tag::malletLift:
        return "handbellsMalletLft";
    case Tag::malletTable:
        return "handbellsMalletBellOnTable";
    case Tag::martellato:
        return "handbellsMartellato";
    case Tag::martellatoLift:
        return "handbellsMartellatoLift";
    case Tag::mutedMartellato:
        return "handbellsMutedMartellato";
    case Tag::pluckLift:
        return "handbellsPluckLift";
    case Tag::swing:
        return "handbellsSwing";
    default:
        return "handbellsGyro";
    }
}
} // namespace

namespace mx
{
namespace impl
{
TechnicalFunctions::TechnicalFunctions(std::span<const core::TechnicalChoice> inTechincalChoiceSet, Cursor inCursor)
    : myTechincalChoiceSet{inTechincalChoiceSet}, myCursor{inCursor}
{
}

void TechnicalFunctions::parseTechnicalMarks(std::vector<api::MarkData> &outMarks) const
{
    for (const auto &techChoice : myTechincalChoiceSet)
    {
        const auto choiceKind = techChoice.kind();
        Converter converter;
        const auto markType = converter.convertTechnicalMark(choiceKind);

        if (markType == api::MarkType::unspecified && choiceKind != core::TechnicalChoice::Kind::otherTechnical)
        {
            continue;
        }
        api::MarkData markData;
        markData.markType = markType;
        markData.tickTimePosition = myCursor.tickTimePosition;
        bool isSuccess = parseTechicalMark(techChoice, markData);

        if (isSuccess)
        {
            outMarks.emplace_back(std::move(markData));
        }
    }
}

bool TechnicalFunctions::parseTechicalMark(const core::TechnicalChoice &techicalChoice,
                                           api::MarkData &outMarkData) const
{
    switch (techicalChoice.kind())
    {
    case core::TechnicalChoice::Kind::upBow: {
        parseMarkDataAttributes(techicalChoice.asUpBow(), outMarkData);
        outMarkData.name = "up-bow";
        return true;
    }
    case core::TechnicalChoice::Kind::downBow: {
        parseMarkDataAttributes(techicalChoice.asDownBow(), outMarkData);
        outMarkData.name = "down-bow";
        return true;
    }
    case core::TechnicalChoice::Kind::harmonic: {
        parseMarkDataAttributes(techicalChoice.asHarmonic(), outMarkData);
        outMarkData.name = "harmonic";
        return true;
    }
    case core::TechnicalChoice::Kind::openString: {
        parseMarkDataAttributes(techicalChoice.asOpenString(), outMarkData);
        outMarkData.name = "open-string";
        return true;
    }
    case core::TechnicalChoice::Kind::thumbPosition: {
        parseMarkDataAttributes(techicalChoice.asThumbPosition(), outMarkData);
        outMarkData.name = "thumb-position";
        return true;
    }
    case core::TechnicalChoice::Kind::fingering:
        return false;
    case core::TechnicalChoice::Kind::pluck: {
        parseMarkDataAttributes(techicalChoice.asPluck(), outMarkData);
        outMarkData.name = "pluck";
        return true;
    }
    case core::TechnicalChoice::Kind::doubleTongue: {
        parseMarkDataAttributes(techicalChoice.asDoubleTongue(), outMarkData);
        outMarkData.name = "double-tongue";
        return true;
    }
    case core::TechnicalChoice::Kind::tripleTongue: {
        parseMarkDataAttributes(techicalChoice.asTripleTongue(), outMarkData);
        outMarkData.name = "triple-tongue";
        return true;
    }
    case core::TechnicalChoice::Kind::stopped: {
        parseMarkDataAttributes(techicalChoice.asStopped(), outMarkData);
        outMarkData.name = "stopped";
        return true;
    }
    case core::TechnicalChoice::Kind::snapPizzicato: {
        parseMarkDataAttributes(techicalChoice.asSnapPizzicato(), outMarkData);
        outMarkData.name = "snap-pizzicato";
        return true;
    }
    case core::TechnicalChoice::Kind::fret: {
        const auto &fret = techicalChoice.asFret();
        parseMarkDataAttributes(fret, outMarkData);
        outMarkData.name = std::to_string(fret.value());
        return true;
    }
    case core::TechnicalChoice::Kind::string: {
        const auto &s = techicalChoice.asString();
        parseMarkDataAttributes(s, outMarkData);
        outMarkData.name = std::to_string(s.value().value());
        return true;
    }
    case core::TechnicalChoice::Kind::hammerOn:
        return false;
    case core::TechnicalChoice::Kind::pullOff:
        return false;
    case core::TechnicalChoice::Kind::bend:
        return false;
    case core::TechnicalChoice::Kind::tap:
        return false;
    case core::TechnicalChoice::Kind::heel: {
        parseMarkDataAttributes(techicalChoice.asHeel(), outMarkData);
        outMarkData.name = "heel";
        return true;
    }
    case core::TechnicalChoice::Kind::toe: {
        parseMarkDataAttributes(techicalChoice.asToe(), outMarkData);
        outMarkData.name = "toe";
        return true;
    }
    case core::TechnicalChoice::Kind::fingernails: {
        parseMarkDataAttributes(techicalChoice.asFingernails(), outMarkData);
        outMarkData.name = "fingernails";
        return true;
    }
    case core::TechnicalChoice::Kind::hole: {
        const auto &hole = techicalChoice.asHole();
        parseMarkDataAttributes(hole, outMarkData);
        outMarkData.name = holeToSmuflName(hole);
        return true;
    }
    case core::TechnicalChoice::Kind::arrow: {
        const auto &arrow = techicalChoice.asArrow();
        parseMarkDataAttributes(arrow, outMarkData);
        outMarkData.name = arrowToSmuflName(arrow);
        return true;
    }
    case core::TechnicalChoice::Kind::handbell: {
        const auto &handbell = techicalChoice.asHandbell();
        parseMarkDataAttributes(handbell, outMarkData);
        outMarkData.name = handbellToSmuflName(handbell.value());
        return true;
    }
    case core::TechnicalChoice::Kind::brassBend: {
        parseMarkDataAttributes(techicalChoice.asBrassBend(), outMarkData);
        outMarkData.name = "brass-bend";
        return true;
    }
    case core::TechnicalChoice::Kind::flip: {
        parseMarkDataAttributes(techicalChoice.asFlip(), outMarkData);
        outMarkData.name = "flip";
        return true;
    }
    case core::TechnicalChoice::Kind::smear: {
        parseMarkDataAttributes(techicalChoice.asSmear(), outMarkData);
        outMarkData.name = "smear";
        return true;
    }
    case core::TechnicalChoice::Kind::open: {
        parseMarkDataAttributes(techicalChoice.asOpen(), outMarkData);
        outMarkData.name = "open";
        return true;
    }
    case core::TechnicalChoice::Kind::halfMuted: {
        parseMarkDataAttributes(techicalChoice.asHalfMuted(), outMarkData);
        outMarkData.name = "half-muted";
        return true;
    }
    case core::TechnicalChoice::Kind::harmonMute: {
        parseMarkDataAttributes(techicalChoice.asHarmonMute(), outMarkData);
        outMarkData.name = "harmon-mute";
        return true;
    }
    case core::TechnicalChoice::Kind::golpe: {
        parseMarkDataAttributes(techicalChoice.asGolpe(), outMarkData);
        outMarkData.name = "golpe";
        return true;
    }
    case core::TechnicalChoice::Kind::otherTechnical: {
        const auto &oa = techicalChoice.asOtherTechnical();
        parseMarkDataAttributes(oa, outMarkData);
        outMarkData.name = oa.value();
        return true;
    }
    default:
        return false;
    }
}
} // namespace impl
} // namespace mx
