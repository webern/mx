// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Enums.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( DirectionAttributes )

        struct DirectionAttributes : public AttributesInterface
        {
        public:
            DirectionAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            AboveBelow placement;
            YesNo directive;
            bool hasPlacement;
            bool hasDirective;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
