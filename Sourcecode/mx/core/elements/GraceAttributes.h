// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( GraceAttributes )

        struct GraceAttributes : public AttributesInterface
        {
        public:
            GraceAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            Percent stealTimePrevious;
            Percent stealTimeFollowing;
            DivisionsValue makeTime;
            YesNo slash;
            bool hasStealTimePrevious;
            bool hasStealTimeFollowing;
            bool hasMakeTime;
            bool hasSlash;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
