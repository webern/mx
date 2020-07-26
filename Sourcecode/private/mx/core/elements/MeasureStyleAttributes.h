// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Color.h"
#include "mx/core/CommaSeparatedText.h"
#include "mx/core/Enums.h"
#include "mx/core/FontSize.h"
#include "mx/core/Integers.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MeasureStyleAttributes )

        struct MeasureStyleAttributes : public AttributesInterface
        {
        public:
            MeasureStyleAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            CommaSeparatedText fontFamily;
            FontStyle fontStyle;
            FontSize fontSize;
            FontWeight fontWeight;
            Color color;
            bool hasNumber;
            bool hasFontFamily;
            bool hasFontStyle;
            bool hasFontSize;
            bool hasFontWeight;
            bool hasColor;

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );
        };
    }
}
