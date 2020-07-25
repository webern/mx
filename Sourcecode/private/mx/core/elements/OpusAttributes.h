// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/XlinkHref.h"
#include "mx/core/XlinkRole.h"
#include "mx/core/XlinkTitle.h"

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

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );
        };
    }
}
