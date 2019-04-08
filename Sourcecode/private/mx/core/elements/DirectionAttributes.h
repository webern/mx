// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );
        };
    }
}
