// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/Strings.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( OpusAttributes )

        struct OpusAttributes : public AttributesInterface
        {
        public:
            OpusAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XlinkHref href;
            const XlinkType type;
            XlinkRole role;
            XlinkTitle title;
            XlinkShow show;
            XlinkActuate actuate;
            const bool hasHref;
            bool hasType;
            bool hasRole;
            bool hasTitle;
            bool hasShow;
            bool hasActuate;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
