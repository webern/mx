// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/FermataFunctions.h"
#include "mx/core/generated/Fermata.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{

FermataFunctions::FermataFunctions(const core::Fermata &inFermata, impl::Cursor inCursor)
    : myFermata{inFermata}, myCursor{inCursor}
{
}

api::MarkData FermataFunctions::parseFermata() const
{
    const auto shape = myFermata.value();
    const bool hasType = myFermata.type().has_value();
    const bool isUpright = hasType && *myFermata.type() == core::UprightInverted::upright();
    const bool isInverted = hasType && *myFermata.type() == core::UprightInverted::inverted();
    auto markType = api::MarkType::fermata;

    if (shape == core::FermataShape::empty())
    {
        if (!hasType)
        {
            markType = api::MarkType::fermata;
        }
        else if (isUpright)
        {
            markType = api::MarkType::fermataUpright;
        }
        else if (isInverted)
        {
            markType = api::MarkType::fermataInverted;
        }
    }
    else if (shape == core::FermataShape::normal())
    {
        if (!hasType)
        {
            markType = api::MarkType::fermataNormal;
        }
        else if (isUpright)
        {
            markType = api::MarkType::fermataNormalUpright;
        }
        else if (isInverted)
        {
            markType = api::MarkType::fermataNormalInverted;
        }
    }
    else if (shape == core::FermataShape::angled())
    {
        if (!hasType)
        {
            markType = api::MarkType::fermataAngled;
        }
        else if (isUpright)
        {
            markType = api::MarkType::fermataAngledUpright;
        }
        else if (isInverted)
        {
            markType = api::MarkType::fermataAngledInverted;
        }
    }
    else if (shape == core::FermataShape::square())
    {
        if (!hasType)
        {
            markType = api::MarkType::fermataSquare;
        }
        else if (isUpright)
        {
            markType = api::MarkType::fermataSquareUpright;
        }
        else if (isInverted)
        {
            markType = api::MarkType::fermataSquareInverted;
        }
    }
    else if (shape == core::FermataShape::doubleAngled())
    {
        markType = api::MarkType::fermataDoubleAngled;
    }
    else if (shape == core::FermataShape::doubleSquare())
    {
        markType = api::MarkType::fermataDoubleSquare;
    }
    else if (shape == core::FermataShape::doubleDot())
    {
        markType = api::MarkType::fermataDoubleDot;
    }
    else if (shape == core::FermataShape::halfCurve())
    {
        markType = api::MarkType::fermataHalfCurve;
    }
    else if (shape == core::FermataShape::curlew())
    {
        markType = api::MarkType::fermataCurlew;
    }

    api::MarkData markData{markType};
    impl::parseMarkDataAttributes(myFermata, markData);
    markData.tickTimePosition = myCursor.tickTimePosition;

    return markData;
}
} // namespace impl
} // namespace mx
