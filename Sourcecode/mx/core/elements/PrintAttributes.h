// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"
#include "mx/core/Integers.h"
#include "mx/core/Strings.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PrintAttributes )

        struct PrintAttributes : public AttributesInterface
        {
        public:
            PrintAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            TenthsValue staffSpacing;
            YesNo newSystem;
            YesNo newPage;
            PositiveInteger blankPage;
            XsToken pageNumber;
            bool hasStaffSpacing;
            bool hasNewSystem;
            bool hasNewPage;
            bool hasBlankPage;
            bool hasPageNumber;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
