// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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

        MX_FORWARD_DECLARE_ATTRIBUTES( SlurAttributes )

        /*
         <xs:attribute name="type" type="start-stop-continue" use="required"/>
         <xs:attribute name="number" type="number-level" default="1"/>
         <xs:attributeGroup ref="line-type"/>
         <xs:attributeGroup ref="dashed-formatting"/>
         <xs:attributeGroup ref="position"/>
         <xs:attributeGroup ref="placement"/>
         <xs:attributeGroup ref="orientation"/>
         <xs:attributeGroup ref="bezier"/>
         <xs:attributeGroup ref="color"/>
         */

         /*
        <xs:attribute name="bezier-offset" type="divisions"/>
        <xs:attribute name="bezier-offset2" type="divisions"/>
        <xs:attribute name="bezier-x" type="tenths"/>
        <xs:attribute name="bezier-y" type="tenths"/>
        <xs:attribute name="bezier-x2" type="tenths"/>
        <xs:attribute name="bezier-y2" type="tenths"/>
        */

        struct SlurAttributes : public AttributesInterface
        {
        public:
            SlurAttributes();
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
