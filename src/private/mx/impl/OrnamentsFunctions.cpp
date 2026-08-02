// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/OrnamentsFunctions.h"
#include "mx/core/generated/Mordent.h"
#include "mx/core/generated/Ornaments.h"
#include "mx/core/generated/OrnamentsGroup.h"
#include "mx/core/generated/OrnamentsGroupChoice.h"
#include "mx/core/generated/Tremolo.h"
#include "mx/core/generated/TremoloType.h"
#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{
void ornamentsFunctionsParseMordentSpecificAttributes(const core::Mordent &m, api::MarkData &outMark)
{
    Converter converter;

    if (m.long_().has_value())
    {
        outMark.hasMordentLong = true;
        outMark.mordentLong = converter.convert(*m.long_());
    }

    if (m.approach().has_value())
    {
        outMark.hasMordentApproach = true;
        outMark.mordentApproach = converter.convert(*m.approach());
    }

    if (m.departure().has_value())
    {
        outMark.hasMordentDeparture = true;
        outMark.mordentDeparture = converter.convert(*m.departure());
    }
}

OrnamentsFunctions::OrnamentsFunctions(const core::Ornaments &inOrnaments, impl::Cursor inCursor)
    : myOrnaments{inOrnaments}, myCursor{inCursor}
{
}

void OrnamentsFunctions::parseOrnaments(std::vector<api::MarkData> &outMarks) const
{
    parseOrnamentsSet(outMarks);
    parseAccidentalMarkSet(outMarks);
}

void OrnamentsFunctions::parseOrnamentsSet(std::vector<api::MarkData> &outMarks) const
{
    for (const auto &group : myOrnaments.group())
    {
        const auto &choiceObj = group.choice();
        Converter converter;
        const auto markType = converter.convertOrnament(choiceObj.kind());
        auto markData = api::MarkData{};
        markData.markType = markType;
        parseOrnament(choiceObj, markData);
        markData.tickTimePosition = myCursor.tickTimePosition;

        if (markData.markType != api::MarkType::unknownOrnament)
        {
            outMarks.emplace_back(std::move(markData));
        }
    }
}

void OrnamentsFunctions::parseAccidentalMarkSet(std::vector<api::MarkData> &outMarks) const
{
    for (const auto &group : myOrnaments.group())
    {
        for (const auto &am : group.accidentalMark())
        {
            AccidentalMarkFunctions funcs{am, myCursor};
            outMarks.emplace_back(funcs.parseAccidentalMark());
        }
    }
}

void OrnamentsFunctions::parseOrnament(const core::OrnamentsGroupChoice &choiceObj, api::MarkData &outMark) const
{
    switch (choiceObj.kind())
    {
    case core::OrnamentsGroupChoice::Kind::trillMark: {
        outMark.name = "trill-mark";
        parseMarkDataAttributes(choiceObj.asTrillMark(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::turn: {
        outMark.name = "turn";
        parseMarkDataAttributes(choiceObj.asTurn(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::delayedTurn: {
        outMark.name = "delayed-turn";
        parseMarkDataAttributes(choiceObj.asDelayedTurn(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::invertedTurn: {
        outMark.name = "inverted-turn";
        parseMarkDataAttributes(choiceObj.asInvertedTurn(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::delayedInvertedTurn: {
        outMark.name = "delayed-inverted-turn";
        parseMarkDataAttributes(choiceObj.asDelayedInvertedTurn(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::verticalTurn: {
        outMark.name = "vertical-turn";
        parseMarkDataAttributes(choiceObj.asVerticalTurn(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::invertedVerticalTurn: {
        outMark.name = "inverted-vertical-turn";
        parseMarkDataAttributes(choiceObj.asInvertedVerticalTurn(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::shake: {
        outMark.name = "shake";
        parseMarkDataAttributes(choiceObj.asShake(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::wavyLine: {
        outMark.name = "wavy-line";
        parseMarkDataAttributes(choiceObj.asWavyLine(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::mordent: {
        outMark.name = "mordent";
        const auto &m = choiceObj.asMordent();
        parseMarkDataAttributes(m, outMark);
        ornamentsFunctionsParseMordentSpecificAttributes(m, outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::invertedMordent: {
        outMark.name = "inverted-mordent";
        const auto &m = choiceObj.asInvertedMordent();
        parseMarkDataAttributes(m, outMark);
        ornamentsFunctionsParseMordentSpecificAttributes(m, outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::schleifer: {
        outMark.name = "schleifer";
        parseMarkDataAttributes(choiceObj.asSchleifer(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::tremolo: {
        const auto &tremolo = choiceObj.asTremolo();
        const auto type = tremolo.type().value_or(core::TremoloType::single()).tag();

        if (type == core::TremoloType::Tag::start || type == core::TremoloType::Tag::stop)
        {
            outMark.name = "tremolo";
            parseMarkDataAttributes(tremolo, outMark);
            outMark.markType =
                (type == core::TremoloType::Tag::start) ? api::MarkType::tremoloStart : api::MarkType::tremoloStop;
            outMark.choice = api::TremoloMarkData{tremolo.value().value()};
            break;
        }

        if (type != core::TremoloType::Tag::single)
        {
            // unmeasured tremolo -- not yet representable
            outMark.name = "this tremolo is not a mark";
            outMark.markType = api::MarkType::unknownOrnament;
            return;
        }

        outMark.name = "tremolo";
        parseMarkDataAttributes(tremolo, outMark);
        const auto numSlashes = tremolo.value().value();

        if (numSlashes == 0)
        {
            outMark.markType = api::MarkType::tremoloSingleThree;
        }
        else if (numSlashes == 1)
        {
            outMark.markType = api::MarkType::tremoloSingleOne;
        }
        else if (numSlashes == 2)
        {
            outMark.markType = api::MarkType::tremoloSingleTwo;
        }
        else if (numSlashes == 3)
        {
            outMark.markType = api::MarkType::tremoloSingleThree;
        }
        else if (numSlashes == 4)
        {
            outMark.markType = api::MarkType::tremoloSingleFour;
        }
        else if (numSlashes == 5)
        {
            outMark.markType = api::MarkType::tremoloSingleFive;
        }

        break;
    }
    case core::OrnamentsGroupChoice::Kind::haydn: {
        outMark.name = "haydn";
        parseMarkDataAttributes(choiceObj.asHaydn(), outMark);
        break;
    }
    case core::OrnamentsGroupChoice::Kind::otherOrnament: {
        const auto &oa = choiceObj.asOtherOrnament();
        parseMarkDataAttributes(oa, outMark);
        const auto &value = oa.value();
        api::OtherMarkData payload;
        if (oa.smufl().has_value())
        {
            payload.smufl = oa.smufl()->toString();
        }
        outMark.choice = std::move(payload);

        if (value.empty())
        {
            outMark.name = "other-ornament";
        }
        else
        {
            outMark.name = value;
        }

        break;
    }
    default:
        break;
    }
}
} // namespace impl
} // namespace mx
