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

        MX_FORWARD_DECLARE_ATTRIBUTES( InterchangeableAttributes )

        struct InterchangeableAttributes : public AttributesInterface
        {
        public:
            InterchangeableAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TimeSymbol symbol;
            TimeSeparator separator;
            bool hasSymbol;
            bool hasSeparator;

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );
        };
    }
}
