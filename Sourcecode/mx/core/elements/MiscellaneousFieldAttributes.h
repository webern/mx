// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Strings.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MiscellaneousFieldAttributes )

        struct MiscellaneousFieldAttributes : public AttributesInterface
        {
        public:
            MiscellaneousFieldAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken name;
            const bool hasName;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
