// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/ArticulationsFunctions.h"
#include "mx/core/generated/ArticulationsChoice.h"
#include "mx/core/generated/CaesuraValue.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"

namespace mx
{
namespace impl
{
ArticulationsFunctions::ArticulationsFunctions(std::span<const core::ArticulationsChoice> inArticulations,
                                               impl::Cursor inCursor)
    : myArticulations{inArticulations}, myCursor{inCursor}
{
}

void ArticulationsFunctions::parseArticulations(std::vector<api::MarkData> &outMarks) const
{
    for (const auto &articulation : myArticulations)
    {
        Converter converter;
        const auto markType = converter.convertArticulation(articulation.kind());
        auto markData = api::MarkData{};
        markData.markType = markType;
        markData.tickTimePosition = myCursor.tickTimePosition;

        parseArticulation(articulation, markData);
        outMarks.emplace_back(std::move(markData));
    }
}

void ArticulationsFunctions::parseArticulation(const core::ArticulationsChoice &inArticulation,
                                               api::MarkData &outMark) const
{
    switch (inArticulation.kind())
    {
    case core::ArticulationsChoice::Kind::accent: {
        parseMarkDataAttributes(inArticulation.asAccent(), outMark);
        outMark.name = "accent";
        break;
    }
    case core::ArticulationsChoice::Kind::strongAccent: {
        const auto &strongAccent = inArticulation.asStrongAccent();
        parseMarkDataAttributes(strongAccent, outMark);
        outMark.name = "strong-accent";
        if (strongAccent.type().has_value())
        {
            outMark.markType = strongAccent.type()->tag() == core::UpDown::Tag::up ? api::MarkType::strongAccentUp
                                                                                   : api::MarkType::strongAccentDown;
        }
        break;
    }
    case core::ArticulationsChoice::Kind::staccato: {
        parseMarkDataAttributes(inArticulation.asStaccato(), outMark);
        outMark.name = "staccato";
        break;
    }
    case core::ArticulationsChoice::Kind::tenuto: {
        parseMarkDataAttributes(inArticulation.asTenuto(), outMark);
        outMark.name = "tenuto";
        break;
    }
    case core::ArticulationsChoice::Kind::detachedLegato: {
        parseMarkDataAttributes(inArticulation.asDetachedLegato(), outMark);
        outMark.name = "detached-legato";
        break;
    }
    case core::ArticulationsChoice::Kind::staccatissimo: {
        parseMarkDataAttributes(inArticulation.asStaccatissimo(), outMark);
        outMark.name = "staccatissimo";
        break;
    }
    case core::ArticulationsChoice::Kind::spiccato: {
        parseMarkDataAttributes(inArticulation.asSpiccato(), outMark);
        outMark.name = "spiccato";
        break;
    }
    case core::ArticulationsChoice::Kind::scoop: {
        parseMarkDataAttributes(inArticulation.asScoop(), outMark);
        outMark.name = "scoop";
        break;
    }
    case core::ArticulationsChoice::Kind::plop: {
        parseMarkDataAttributes(inArticulation.asPlop(), outMark);
        outMark.name = "plop";
        break;
    }
    case core::ArticulationsChoice::Kind::doit: {
        parseMarkDataAttributes(inArticulation.asDoit(), outMark);
        outMark.name = "doit";
        break;
    }
    case core::ArticulationsChoice::Kind::falloff: {
        parseMarkDataAttributes(inArticulation.asFalloff(), outMark);
        outMark.name = "falloff";
        break;
    }
    case core::ArticulationsChoice::Kind::breathMark: {
        parseMarkDataAttributes(inArticulation.asBreathMark(), outMark);
        outMark.name = "breath-mark";
        break;
    }
    case core::ArticulationsChoice::Kind::caesura: {
        parseMarkDataAttributes(inArticulation.asCaesura(), outMark);
        outMark.name = "caesura";
        switch (inArticulation.asCaesura().value().tag())
        {
        case core::CaesuraValue::Tag::empty:
            outMark.markType = api::MarkType::caesura;
            break;
        case core::CaesuraValue::Tag::normal:
            outMark.markType = api::MarkType::caesuraNormal;
            break;
        case core::CaesuraValue::Tag::thick:
            outMark.markType = api::MarkType::caesuraThick;
            break;
        case core::CaesuraValue::Tag::short_:
            outMark.markType = api::MarkType::caesuraShort;
            break;
        case core::CaesuraValue::Tag::curved:
            outMark.markType = api::MarkType::caesuraCurved;
            break;
        case core::CaesuraValue::Tag::single:
            outMark.markType = api::MarkType::caesuraSingle;
            break;
        }
        break;
    }
    case core::ArticulationsChoice::Kind::stress: {
        parseMarkDataAttributes(inArticulation.asStress(), outMark);
        outMark.name = "stress";
        break;
    }
    case core::ArticulationsChoice::Kind::unstress: {
        parseMarkDataAttributes(inArticulation.asUnstress(), outMark);
        outMark.name = "unstress";
        break;
    }
    case core::ArticulationsChoice::Kind::softAccent: {
        parseMarkDataAttributes(inArticulation.asSoftAccent(), outMark);
        outMark.name = "soft-accent";
        break;
    }
    case core::ArticulationsChoice::Kind::otherArticulation: {
        const auto &oa = inArticulation.asOtherArticulation();
        parseMarkDataAttributes(oa, outMark);
        outMark.name = oa.value();

        const auto possibleCustomMarkType = mx::api::getMarkTypeFromCustomString(outMark.name);
        if (possibleCustomMarkType != mx::api::MarkType::customErrorUnknown)
        {
            outMark.markType = possibleCustomMarkType;
        }

        break;
    }
    default:
        break;
    }
}
} // namespace impl
} // namespace mx
