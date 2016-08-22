// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"
#include "mx/core/Integers.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( TupletAttributes )

        struct TupletAttributes : public AttributesInterface
        {
        public:
            TupletAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            NumberLevel number;
            YesNo bracket;
            ShowTuplet showNumber;
            ShowTuplet showType;
            LineShape lineShape;
            TenthsValue defaultX;
            TenthsValue defaultY;
            TenthsValue relativeX;
            TenthsValue relativeY;
            AboveBelow placement;
            const 	bool hasType;
            bool hasNumber;
            bool hasBracket;
            bool hasShowNumber;
            bool hasShowType;
            bool hasLineShape;
            bool hasDefaultX;
            bool hasDefaultY;
            bool hasRelativeX;
            bool hasRelativeY;
            bool hasPlacement;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
