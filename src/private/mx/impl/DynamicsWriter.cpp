// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DynamicsWriter.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/DynamicsChoice.h"
#include "mx/core/generated/Empty.h"
#include "mx/core/generated/OtherText.h"
#include "mx/core/generated/SmuflGlyphName.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/utility/Throw.h"

namespace mx
{
namespace impl
{

core::DynamicsChoice dynamicsWriterMakeChoice(core::DynamicsChoice::Kind kind, const std::string &otherName,
                                              const std::optional<std::string> &smufl)
{
    using K = core::DynamicsChoice::Kind;
    core::Empty empty{};
    switch (kind)
    {
    case K::p:
        return core::DynamicsChoice::p(empty);
    case K::pp:
        return core::DynamicsChoice::pp(empty);
    case K::ppp:
        return core::DynamicsChoice::ppp(empty);
    case K::pppp:
        return core::DynamicsChoice::pppp(empty);
    case K::ppppp:
        return core::DynamicsChoice::ppppp(empty);
    case K::pppppp:
        return core::DynamicsChoice::pppppp(empty);
    case K::f:
        return core::DynamicsChoice::f(empty);
    case K::ff:
        return core::DynamicsChoice::ff(empty);
    case K::fff:
        return core::DynamicsChoice::fff(empty);
    case K::ffff:
        return core::DynamicsChoice::ffff(empty);
    case K::fffff:
        return core::DynamicsChoice::fffff(empty);
    case K::ffffff:
        return core::DynamicsChoice::ffffff(empty);
    case K::mp:
        return core::DynamicsChoice::mp(empty);
    case K::mf:
        return core::DynamicsChoice::mf(empty);
    case K::sf:
        return core::DynamicsChoice::sf(empty);
    case K::sfp:
        return core::DynamicsChoice::sfp(empty);
    case K::sfpp:
        return core::DynamicsChoice::sfpp(empty);
    case K::fp:
        return core::DynamicsChoice::fp(empty);
    case K::rf:
        return core::DynamicsChoice::rf(empty);
    case K::rfz:
        return core::DynamicsChoice::rfz(empty);
    case K::sfz:
        return core::DynamicsChoice::sfz(empty);
    case K::sffz:
        return core::DynamicsChoice::sffz(empty);
    case K::fz:
        return core::DynamicsChoice::fz(empty);
    case K::n:
        return core::DynamicsChoice::n(empty);
    case K::pf:
        return core::DynamicsChoice::pf(empty);
    case K::sfzp:
        return core::DynamicsChoice::sfzp(empty);
    case K::otherDynamics: {
        core::OtherText ot;
        ot.setValue(otherName);
        if (smufl.has_value())
        {
            ot.setSmufl(core::SmuflGlyphName{*smufl});
        }
        return core::DynamicsChoice::otherDynamics(ot);
    }
    default:
        return core::DynamicsChoice::p(empty);
    }
}

DynamicsWriter::DynamicsWriter(const api::MarkData &inMark, impl::Cursor inCursor)
    : myMarkData{inMark}, myCursor{inCursor}, myConverter{}
{
    MX_ASSERT(isMarkDynamic(inMark.markType));
}

DynamicsWriter::DynamicsWriter(const api::MarkData &inMark, impl::Cursor inCursor,
                               api::Placement /*directionPlacement*/)
    : myMarkData{inMark}, myCursor{inCursor}, myConverter{}
{
    MX_ASSERT(isMarkDynamic(inMark.markType));
}

core::Dynamics DynamicsWriter::getDynamics() const
{
    core::Dynamics dyn;
    if (myMarkData.markType == api::MarkType::compoundDynamics)
    {
        for (const auto &component : myMarkData.choice.compoundDynamics().components)
        {
            if (component.isOther())
            {
                const auto other = component.other();
                dyn.addChoice(
                    dynamicsWriterMakeChoice(core::DynamicsChoice::Kind::otherDynamics, other.text, other.smufl));
            }
            else
            {
                dyn.addChoice(dynamicsWriterMakeChoice(myConverter.convert(component.standard()), {}, {}));
            }
        }
    }
    else
    {
        const auto kind = myConverter.convertDynamic(myMarkData.markType);
        const bool isOther = kind == core::DynamicsChoice::Kind::otherDynamics;
        const auto &otherName = isOther ? myMarkData.name : std::string{};
        const auto smufl = isOther ? myMarkData.choice.otherMark().smufl : std::optional<std::string>{};
        dyn.addChoice(dynamicsWriterMakeChoice(kind, otherName, smufl));
    }
    impl::setAttributesFromMarkData(myMarkData, dyn);
    return dyn;
}
} // namespace impl
} // namespace mx
