// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"
#include "mx/core/Integers.h"
#include "mx/core/Color.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( TiedAttributes )

        struct TiedAttributes : public AttributesInterface
        {
        public:
            TiedAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStopContinue type;
            NumberLevel number;
            LineType lineType;
            TenthsValue dashLength;
            TenthsValue spaceLength;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            OverUnder orientation;
            DivisionsValue bezierOffset;
            DivisionsValue bezierOffset2;
            TenthsValue bezierX;
            TenthsValue bezierY;
            TenthsValue bezierX2;
            TenthsValue bezierY2;
            Color color;
            const bool hasType;
            bool hasNumber;
            bool hasLineType;
            bool hasDashLength;
            bool hasSpaceLength;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;
            bool hasOrientation;
            bool hasBezierOffset;
            bool hasBezierOffset2;
            bool hasBezierX;
            bool hasBezierY;
            bool hasBezierX2;
            bool hasBezierY2;
            bool hasColor;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
