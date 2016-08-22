// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Color.h"
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"
#include "mx/core/Integers.h"
#include "mx/core/Strings.h"
#include "mx/core/FontSize.h"
#include "mx/core/NumberOrNormal.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( CreditWordsAttributes )

        struct CreditWordsAttributes : public AttributesInterface
        {
        public:
            CreditWordsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            LeftCenterRight justify;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            LeftCenterRight halign;
            Valign valign;
            NumberOfLines underline;
            NumberOfLines overline;
            NumberOfLines lineThrough;
            RotationDegrees rotation;
            NumberOrNormal letterSpacing;
            NumberOrNormal lineHeight;
            XmlLang lang;
            XmlSpace space;
            EnclosureShape enclosure;
            bool hasJustify;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;
            bool hasHalign;
            bool hasValign;
            bool hasUnderline;
            bool hasOverline;
            bool hasLineThrough;
            bool hasRotation;
            bool hasLetterSpacing;
            bool hasLineHeight;
            bool hasLang;
            bool hasSpace;
            bool hasEnclosure;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
