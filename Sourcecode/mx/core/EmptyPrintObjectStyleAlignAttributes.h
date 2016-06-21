// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include "mx/core/AttributesInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/Strings.h"
#include "mx/core/Enums.h"
#include "mx/core/FontSize.h"
//#include "mx/core/TenthsValue.h"
//#include "mx/core/CommaSeparatedText.h"
//#include "mx/core/FontStyle.h"
//#include "mx/core/FontWeight.h"
//#include "mx/core/LeftCenterRight.h"

namespace mx
{
	namespace core
	{
        struct EmptyPrintObjectStyleAlignAttributes;
        using EmptyPrintObjectStyleAlignAttributesPtr = std::shared_ptr<EmptyPrintObjectStyleAlignAttributes>;
        
        struct EmptyPrintObjectStyleAlignAttributes : public AttributesInterface
        {
        public:
            EmptyPrintObjectStyleAlignAttributes();
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
            LeftCenterRight halign;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasHalign;
        };
	}
}
