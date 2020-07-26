// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/CommaSeparatedText.h"
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

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );
        };
    }
}
