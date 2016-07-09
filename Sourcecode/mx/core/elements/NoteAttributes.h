// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"
#include "mx/core/Strings.h"
#include "mx/core/Color.h"
#include "mx/core/FontSize.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( NoteAttributes )

        struct NoteAttributes : public AttributesInterface
        {
        public:
            NoteAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            YesNo printObject;
            YesNo printDot;
            YesNo printSpacing;
            YesNo printLyric;
            NonNegativeDecimal dynamics;
            NonNegativeDecimal endDynamics;
            DivisionsValue attack;
            DivisionsValue release;
            TimeOnly timeOnly;
            YesNo pizzicato;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasPrintObject;
            bool hasPrintDot;
            bool hasPrintSpacing;
            bool hasPrintLyric;
            bool hasDynamics;
            bool hasEndDynamics;
            bool hasAttack;
            bool hasRelease;
            bool hasTimeOnly;
            bool hasPizzicato;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
